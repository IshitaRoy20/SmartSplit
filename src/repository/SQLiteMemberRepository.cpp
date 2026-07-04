#include "../include/repository/SQLiteMemberRepository.h"
#include "../include/database/Database.h"

#include <iostream>

SQLiteMemberRepository::SQLiteMemberRepository()
    : cache(20)
{}

Member SQLiteMemberRepository::addMember(int groupId, const std::string& name)
{
    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    int nextId = 1;

    sqlite3_prepare_v2(
        db,
        "SELECT COALESCE(MAX(id), 0) + 1 FROM members WHERE group_id = ?",
        -1,
        &stmt,
        nullptr
    );
    sqlite3_bind_int(stmt, 1, groupId);

    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        nextId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    const char* sql = "INSERT INTO members (group_id, id, name) VALUES (?, ?, ?)";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, groupId);
    sqlite3_bind_int(stmt, 2, nextId);
    sqlite3_bind_text(stmt, 3, name.c_str(), -1, SQLITE_TRANSIENT);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error: Could not save member.\n";
    }

    sqlite3_finalize(stmt);

    cache.remove(groupId);

    return Member(nextId, groupId, name);
}

std::vector<Member> SQLiteMemberRepository::getMembersByGroup(int groupId)
{
    std::vector<Member> cached;

    if(cache.get(groupId, cached))
    {
        return cached;
    }

    std::vector<Member> members;

    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, group_id, name FROM members WHERE group_id = ? ORDER BY id";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, groupId);

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        int gId = sqlite3_column_int(stmt, 1);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        members.emplace_back(id, gId, name);
    }

    sqlite3_finalize(stmt);

    cache.put(groupId, members);

    return members;
}

void SQLiteMemberRepository::deleteMember(int groupId, int memberId)
{
    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    const char* sql = "DELETE FROM members WHERE group_id = ? AND id = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, groupId);
    sqlite3_bind_int(stmt, 2, memberId);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error: Could not delete member.\n";
    }

    sqlite3_finalize(stmt);

    cache.remove(groupId);
}

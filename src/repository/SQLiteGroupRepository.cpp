#include "../include/repository/SQLiteGroupRepository.h"
#include "../include/database/Database.h"

#include <iostream>

static const int ALL_GROUPS_KEY = 0;

SQLiteGroupRepository::SQLiteGroupRepository()
    : cache(5)
{}

Group SQLiteGroupRepository::addGroup(const std::string& name)
{
    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    int nextId = 1;

    sqlite3_prepare_v2(db, "SELECT COALESCE(MAX(id), 0) + 1 FROM groups", -1, &stmt, nullptr);

    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        nextId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    const char* sql = "INSERT INTO groups (id, name) VALUES (?, ?)";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, nextId);
    sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error: Could not save group.\n";
    }

    sqlite3_finalize(stmt);

    cache.remove(ALL_GROUPS_KEY);

    return Group(nextId, name);
}

std::vector<Group> SQLiteGroupRepository::getAllGroups()
{
    std::vector<Group> cached;

    if(cache.get(ALL_GROUPS_KEY, cached))
    {
        return cached;
    }

    std::vector<Group> groups;

    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, name FROM groups ORDER BY id";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        groups.emplace_back(id, name);
    }

    sqlite3_finalize(stmt);

    cache.put(ALL_GROUPS_KEY, groups);

    return groups;
}
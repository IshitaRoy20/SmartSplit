#include "../include/database/Database.h"
#include "../include/repository/SQLiteGroupRepository.h"
#include "../include/repository/SQLiteMemberRepository.h"
#include <sqlite3.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <mutex>

using Clock = std::chrono::high_resolution_clock;

double elapsedMs(Clock::time_point start, Clock::time_point end)
{
    return std::chrono::duration<double, std::milli>(end - start).count();
}

std::vector<Member> queryMembersNoCache(int groupId)
{
    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    std::vector<Member> members;

    sqlite3_prepare_v2(db, "SELECT id, group_id, name FROM members WHERE group_id = ? ORDER BY id", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, groupId);

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        int gId = sqlite3_column_int(stmt, 1);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        members.emplace_back(id, gId, name);
    }

    sqlite3_finalize(stmt);
    return members;
}

void benchmarkCache(SQLiteMemberRepository& memberRepo, int groupId, int iterations)
{
    std::cout << "\n===== CACHE BENCHMARK (" << iterations << " reads) =====\n";

    auto start1 = Clock::now();
    for(int i = 0; i < iterations; i++)
    {
        queryMembersNoCache(groupId);
    }
    auto end1 = Clock::now();
    double noCacheTime = elapsedMs(start1, end1);

    memberRepo.getMembersByGroup(groupId);

    auto start2 = Clock::now();
    for(int i = 0; i < iterations; i++)
    {
        memberRepo.getMembersByGroup(groupId);
    }
    auto end2 = Clock::now();
    double cacheTime = elapsedMs(start2, end2);

    std::cout << "Without cache : " << noCacheTime << " ms  (" << (noCacheTime / iterations) << " ms/call)\n";
    std::cout << "With cache    : " << cacheTime << " ms  (" << (cacheTime / iterations) << " ms/call)\n";
    std::cout << "Speedup       : " << (noCacheTime / cacheTime) << "x\n";
}

void unsafeAddMember(int groupId, const std::string& name)
{
    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    int nextId = 1;
    sqlite3_prepare_v2(db, "SELECT COALESCE(MAX(id), 0) + 1 FROM members WHERE group_id = ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, groupId);
    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        nextId = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    std::this_thread::sleep_for(std::chrono::microseconds(200));

    sqlite3_prepare_v2(db, "INSERT INTO members (group_id, id, name) VALUES (?, ?, ?)", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, groupId);
    sqlite3_bind_int(stmt, 2, nextId);
    sqlite3_bind_text(stmt, 3, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void benchmarkThreadSafety(SQLiteGroupRepository& groupRepo, SQLiteMemberRepository& memberRepo, int threadCount)
{
    std::cout << "\n===== THREAD SAFETY BENCHMARK (" << threadCount << " concurrent inserts) =====\n";

    Group unsafeGroup = groupRepo.addGroup("UnsafeTestGroup");
    Group safeGroup = groupRepo.addGroup("SafeTestGroup");

    std::vector<std::thread> unsafeThreads;
    for(int i = 0; i < threadCount; i++)
    {
        unsafeThreads.emplace_back([&, i]()
        {
            unsafeAddMember(unsafeGroup.getId(), "U" + std::to_string(i));
        });
    }
    for(auto& t : unsafeThreads) t.join();

    std::vector<std::thread> safeThreads;
    std::vector<Member> safeResults(threadCount);
    for(int i = 0; i < threadCount; i++)
    {
        safeThreads.emplace_back([&, i]()
        {
            safeResults[i] = memberRepo.addMember(safeGroup.getId(), "S" + std::to_string(i));
        });
    }
    for(auto& t : safeThreads) t.join();

    auto unsafeRows = queryMembersNoCache(unsafeGroup.getId());
    std::set<int> unsafeIds;
    for(auto& m : unsafeRows) unsafeIds.insert(m.getId());

    std::set<int> safeIds;
    for(auto& m : safeResults) safeIds.insert(m.getId());
    auto safeRows = memberRepo.getMembersByGroup(safeGroup.getId());

    std::cout << "Without mutex : requested " << threadCount
              << " inserts, rows actually in DB = " << unsafeRows.size()
              << ", unique ids = " << unsafeIds.size()
              << (unsafeRows.size() < threadCount ? "  <-- LOST INSERTS / COLLISIONS" : "")
              << "\n";

    std::cout << "With mutex    : requested " << threadCount
              << " inserts, rows actually in DB = " << safeRows.size()
              << ", unique ids = " << safeIds.size()
              << (safeRows.size() == (size_t)threadCount ? "  <-- ALL SAFE" : "")
              << "\n";
}

int main()
{
    Database& db = Database::getInstance();
    db.open();
    db.initialize();

    SQLiteGroupRepository groupRepo;
    SQLiteMemberRepository memberRepo;

    Group benchGroup = groupRepo.addGroup("BenchmarkGroup");

    for(int i = 0; i < 200; i++)
    {
        memberRepo.addMember(benchGroup.getId(), "Member" + std::to_string(i));
    }

    benchmarkCache(memberRepo, benchGroup.getId(), 2000);

    benchmarkThreadSafety(groupRepo, memberRepo, 100);

    return 0;
}

#include "../include/repository/SQLiteExpenseRepository.h"
#include "../include/database/Database.h"

#include <iostream>

SQLiteExpenseRepository::SQLiteExpenseRepository()
    : cache(20)
{}

Expense SQLiteExpenseRepository::addExpense(int groupId, const std::string& title, double amount)
{
    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    int nextId = 1;

    sqlite3_prepare_v2(db, "SELECT COALESCE(MAX(id), 0) + 1 FROM expenses", -1, &stmt, nullptr);

    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        nextId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    const char* sql = "INSERT INTO expenses (id, group_id, title, amount) VALUES (?, ?, ?, ?)";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, nextId);
    sqlite3_bind_int(stmt, 2, groupId);
    sqlite3_bind_text(stmt, 3, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, amount);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error: Could not save expense.\n";
    }

    sqlite3_finalize(stmt);

    cache.remove(groupId);

    return Expense(nextId, groupId, title, amount);
}

std::vector<Expense> SQLiteExpenseRepository::getExpensesByGroup(int groupId)
{
    std::vector<Expense> cached;

    if(cache.get(groupId, cached))
    {
        return cached;
    }

    std::vector<Expense> expenses;

    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT id, group_id, title, amount FROM expenses WHERE group_id = ? ORDER BY id";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, groupId);

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        int gId = sqlite3_column_int(stmt, 1);
        std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double amount = sqlite3_column_double(stmt, 3);

        expenses.emplace_back(id, gId, title, amount);
    }

    sqlite3_finalize(stmt);

    cache.put(groupId, expenses);

    return expenses;
}

void SQLiteExpenseRepository::deleteExpense(int groupId, int expenseId)
{
    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    const char* sql = "DELETE FROM expenses WHERE group_id = ? AND id = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, groupId);
    sqlite3_bind_int(stmt, 2, expenseId);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error: Could not delete expense.\n";
    }

    sqlite3_finalize(stmt);

    cache.remove(groupId);
}

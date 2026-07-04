#include "../include/repository/SQLiteExpensePaymentRepository.h"
#include "../include/database/Database.h"

#include <iostream>

SQLiteExpensePaymentRepository::SQLiteExpensePaymentRepository()
    : cache(20)
{}

void SQLiteExpensePaymentRepository::savePayment(const ExpensePayment& payment)
{
    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    const char* sql = "INSERT INTO expense_payments (expense_id, group_id, member_id, amount_paid) VALUES (?, ?, ?, ?)";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, payment.getExpenseId());
    sqlite3_bind_int(stmt, 2, payment.getGroupId());
    sqlite3_bind_int(stmt, 3, payment.getMemberId());
    sqlite3_bind_double(stmt, 4, payment.getAmountPaid());

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error: Could not save payment.\n";
    }

    sqlite3_finalize(stmt);

    cache.remove(payment.getGroupId());
}

std::vector<ExpensePayment> SQLiteExpensePaymentRepository::getPaymentsByGroup(int groupId)
{
    std::vector<ExpensePayment> cached;

    if(cache.get(groupId, cached))
    {
        return cached;
    }

    std::vector<ExpensePayment> payments;

    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    const char* sql = "SELECT expense_id, group_id, member_id, amount_paid FROM expense_payments WHERE group_id = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, groupId);

    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        int expenseId = sqlite3_column_int(stmt, 0);
        int gId = sqlite3_column_int(stmt, 1);
        int memberId = sqlite3_column_int(stmt, 2);
        double amount = sqlite3_column_double(stmt, 3);

        payments.emplace_back(expenseId, gId, memberId, amount);
    }

    sqlite3_finalize(stmt);

    cache.put(groupId, payments);

    return payments;
}

void SQLiteExpensePaymentRepository::deletePaymentsByExpenseId(int groupId, int expenseId)
{
    std::lock_guard<std::mutex> lock(Database::getInstance().getMutex());

    sqlite3* db = Database::getInstance().getConnection();
    sqlite3_stmt* stmt;

    const char* sql = "DELETE FROM expense_payments WHERE expense_id = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, expenseId);

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error: Could not delete payments.\n";
    }

    sqlite3_finalize(stmt);

    cache.remove(groupId);
}

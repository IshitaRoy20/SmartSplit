#pragma once

#include "IExpenseRepository.h"
#include "../cache/LRUCache.h"

class SQLiteExpenseRepository
    : public IExpenseRepository
{
private:

    LRUCache<int, std::vector<Expense>> cache;

public:

    SQLiteExpenseRepository();

    Expense addExpense(
        int groupId,
        const std::string& title,
        double amount
    ) override;

    std::vector<Expense>
    getExpensesByGroup(
        int groupId
    ) override;

    void deleteExpense(
        int groupId,
        int expenseId
    ) override;
};

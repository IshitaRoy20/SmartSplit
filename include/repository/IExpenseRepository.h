#pragma once

#include "../models/Expense.h"

#include <vector>

class IExpenseRepository
{
public:

    virtual Expense addExpense(
        int groupId,
        const std::string& title,
        double amount
    ) = 0;

    virtual std::vector<Expense>
    getExpensesByGroup(
        int groupId
    ) = 0;

    virtual void deleteExpense(
        int groupId,
        int expenseId
    ) = 0;

    virtual ~IExpenseRepository() = default;
};

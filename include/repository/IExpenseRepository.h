#pragma once

#include "../models/Expense.h"

#include <vector>

class IExpenseRepository
{
public:

    virtual void saveExpense(
        const Expense& expense
    ) = 0;

    virtual std::vector<Expense>
    getAllExpenses() = 0;

    virtual ~IExpenseRepository() = default;
};
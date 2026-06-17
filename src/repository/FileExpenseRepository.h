#pragma once

#include "IExpenseRepository.h"

class FileExpenseRepository
    : public IExpenseRepository
{
public:

    void saveExpense(
        const Expense& expense
    ) override;
    void deleteExpense(
    int expenseId
    );
    std::vector<Expense>
    getAllExpenses() override;
};
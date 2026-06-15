#pragma once

#include "IExpenseRepository.h"

class FileExpenseRepository
    : public IExpenseRepository
{
public:

    void saveExpense(
        const Expense& expense
    ) override;

    std::vector<Expense>
    getAllExpenses() override;
};
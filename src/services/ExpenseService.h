#pragma once

#include "../repository/FileExpenseRepository.h"
#include "../repository/FileExpensePaymentRepository.h"

#include <iostream>

class ExpenseService
{
private:

    FileExpenseRepository expenseRepo;

    FileExpensePaymentRepository paymentRepo;

    int nextExpenseId = 1;

public:

    ExpenseService()
    {
        auto expenses =
            expenseRepo.getAllExpenses();

        if(!expenses.empty())
        {
            nextExpenseId =
                expenses.back().getId()
                + 1;
        }
    }

    int createExpense(
        int groupId,
        const std::string& title,
        double amount
    )
    {
        Expense expense(
            nextExpenseId,
            groupId,
            title,
            amount
        );

        expenseRepo.saveExpense(
            expense
        );

        return nextExpenseId++;
    }

    void addPayment(
        int expenseId,
        int memberId,
        double amount
    )
    {
        paymentRepo.savePayment(
            ExpensePayment(
                expenseId,
                memberId,
                amount
            )
        );
    }

    void viewExpenses(
        int groupId
    )
    {
        auto expenses =
            expenseRepo.getAllExpenses();

        for(auto& expense : expenses)
        {
            if(expense.getGroupId()
               == groupId)
            {
                std::cout
                    << expense.getId()
                    << ". "
                    << expense.getTitle()
                    << " ₹"
                    << expense.getAmount()
                    << "\n";
            }
        }
    }
};
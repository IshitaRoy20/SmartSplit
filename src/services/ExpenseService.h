#pragma once

#include "../repository/FileExpenseRepository.h"
#include "../repository/FileExpensePaymentRepository.h"

#include <iostream>
#include <unordered_map>

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

    void viewBalances(
    int groupId,
    const std::vector<Member>& members
    )
    {
        auto expenses =
            expenseRepo.getAllExpenses();

        auto payments =
            paymentRepo.getAllPayments();

        std::unordered_map<int,double>
            balances;

        for(const auto& member : members)
        {
            if(member.getGroupId() == groupId)
            {
                balances[
                    member.getId()
                ] = 0;
            }
        }

        for(const auto& payment : payments)
        {
            balances[
                payment.getMemberId()
            ] += payment.getAmountPaid();
        }

        for(const auto& expense : expenses)
        {
            if(expense.getGroupId()
            != groupId)
            {
                continue;
            }

            int memberCount = 0;

            for(const auto& member : members)
            {
                if(member.getGroupId()
                == groupId)
                {
                    memberCount++;
                }
            }

            double share =
                expense.getAmount()
                / memberCount;

            for(const auto& member : members)
            {
                if(member.getGroupId()
                == groupId)
                {
                    balances[
                        member.getId()
                    ] -= share;
                }
            }
        }

        std::cout
            << "\n===== BALANCES =====\n";

        for(const auto& member : members)
        {
            if(member.getGroupId()
            != groupId)
            {
                continue;
            }

            std::cout
                << member.getName()
                << " : "
                << balances[
                    member.getId()
                ]
                << "\n";
        }
    }

};
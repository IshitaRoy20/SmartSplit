#pragma once

#include "../repository/FileExpenseRepository.h"
#include "../repository/FileExpensePaymentRepository.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

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

    bool found = false;

    for(auto& expense : expenses)
    {
        if(expense.getGroupId()
           == groupId)
        {
            found = true;

            std::cout
                << expense.getId()
                << ". "
                << expense.getTitle()
                << " ₹"
                << expense.getAmount()
                << "\n";
        }
    }

    if(!found)
    {
        std::cout
            << "\nNo Expenses Found\n";
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
void viewSettlements(
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
            if(member.getGroupId() == groupId)
            {
                std::cout
                    << member.getName()
                    << " : "
                    << balances[
                        member.getId()
                    ]
                    << "\n";
            }
        }

        struct Person
        {
            int memberId;
            double amount;
        };

        std::vector<Person>
            creditors;

        std::vector<Person>
            debtors;

        for(const auto& member : members)
        {
            if(member.getGroupId()
            != groupId)
            {
                continue;
            }

            double balance =
                balances[
                    member.getId()
                ];

            if(balance > 0.01)
            {
                creditors.push_back(
                {
                    member.getId(),
                    balance
                });
            }
            else if(balance < -0.01)
            {
                debtors.push_back(
                {
                    member.getId(),
                    -balance
                });
            }
        }

        std::cout
            << "\n===== SETTLEMENTS =====\n";

        int i = 0;
        int j = 0;

        bool found = false;

        while(i < debtors.size() &&
            j < creditors.size())
        {
            double amount =
                std::min(
                    debtors[i].amount,
                    creditors[j].amount
                );

            std::string debtorName;
            std::string creditorName;

            for(const auto& member : members)
            {
                if(member.getId()
                == debtors[i].memberId)
                {
                    debtorName =
                        member.getName();
                }

                if(member.getId()
                == creditors[j].memberId)
                {
                    creditorName =
                        member.getName();
                }
            }

            std::cout
                << debtorName
                << " pays "
                << creditorName
                << " ₹"
                << amount
                << "\n";

            found = true;

            debtors[i].amount -= amount;
            creditors[j].amount -= amount;

            if(debtors[i].amount < 0.01)
            {
                i++;
            }

            if(creditors[j].amount < 0.01)
            {
                j++;
            }
        }

        if(!found)
        {
            std::cout
                << "No Settlements Required.\n";
        }
    }
};
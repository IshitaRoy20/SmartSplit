#pragma once

#include "../repository/SQLiteExpenseRepository.h"
#include "../repository/SQLiteExpensePaymentRepository.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

class ExpenseService
{
private:

    SQLiteExpenseRepository expenseRepo;

    SQLiteExpensePaymentRepository paymentRepo;

public:

    int createExpense(
        int groupId,
        const std::string& title,
        double amount
    )
    {
        Expense expense = expenseRepo.addExpense(groupId, title, amount);

        return expense.getId();
    }

    void addPayment(
        int expenseId,
        int groupId,
        int memberId,
        double amount
    )
    {
        paymentRepo.savePayment(
            ExpensePayment(
                expenseId,
                groupId,
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
            expenseRepo.getExpensesByGroup(groupId);

        if(expenses.empty())
        {
            std::cout
                << "\nNo Expenses Found\n";

            return;
        }

        for(auto& expense : expenses)
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

    std::unordered_map<int, double> computeBalances(
        int groupId,
        const std::vector<Member>& members
    )
    {
        auto expenses =
            expenseRepo.getExpensesByGroup(groupId);

        auto payments =
            paymentRepo.getPaymentsByGroup(groupId);

        std::unordered_map<int, double> balances;

        for(const auto& member : members)
        {
            balances[member.getId()] = 0;
        }

        for(const auto& payment : payments)
        {
            balances[payment.getMemberId()] += payment.getAmountPaid();
        }

        int memberCount = static_cast<int>(members.size());

        for(const auto& expense : expenses)
        {
            double share = expense.getAmount() / memberCount;

            for(const auto& member : members)
            {
                balances[member.getId()] -= share;
            }
        }

        return balances;
    }

    void viewBalances(
        int groupId,
        const std::vector<Member>& members
    )
    {
        auto balances = computeBalances(groupId, members);

        std::cout
            << "\n===== BALANCES =====\n";

        for(const auto& member : members)
        {
            std::cout
                << member.getName()
                << " : "
                << balances[member.getId()]
                << "\n";
        }
    }

    void viewSettlements(
        int groupId,
        const std::vector<Member>& members
    )
    {
        auto balances = computeBalances(groupId, members);

        std::cout
            << "\n===== BALANCES =====\n";

        for(const auto& member : members)
        {
            std::cout
                << member.getName()
                << " : "
                << balances[member.getId()]
                << "\n";
        }

        struct Person
        {
            int memberId;
            double amount;
        };

        std::vector<Person> creditors;
        std::vector<Person> debtors;

        for(const auto& member : members)
        {
            double balance = balances[member.getId()];

            if(balance > 0.01)
            {
                creditors.push_back({member.getId(), balance});
            }
            else if(balance < -0.01)
            {
                debtors.push_back({member.getId(), -balance});
            }
        }

        int rawTransactions = static_cast<int>(debtors.size() * creditors.size());
        int optimizedTransactions = 0;

        std::cout
            << "\n===== SETTLEMENT ANALYSIS =====\n";

        std::cout << "\nDebtors : " << debtors.size();
        std::cout << "\nCreditors : " << creditors.size();
        std::cout << "\nPotential Transactions : " << rawTransactions;
        std::cout << "\n---------------------\n";

        size_t i = 0;
        size_t j = 0;

        bool found = false;

        while(i < debtors.size() && j < creditors.size())
        {
            double amount = std::min(debtors[i].amount, creditors[j].amount);

            std::string debtorName;
            std::string creditorName;

            for(const auto& member : members)
            {
                if(member.getId() == debtors[i].memberId)
                {
                    debtorName = member.getName();
                }

                if(member.getId() == creditors[j].memberId)
                {
                    creditorName = member.getName();
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
            optimizedTransactions++;

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
            std::cout << "No Settlements Required.\n";
        }
        else
        {
            double reduction = 0;

            if(rawTransactions > 0)
            {
                reduction =
                    (rawTransactions - optimizedTransactions)
                    * 100.0
                    / rawTransactions;
            }

            std::cout << "\n\nOptimized Transactions : " << optimizedTransactions;
            std::cout << "\nTransaction Reduction : " << reduction << "%\n";
        }
    }

    void viewExpenseDetails(
        int groupId,
        const std::vector<Member>& members
    )
    {
        auto expenses =
            expenseRepo.getExpensesByGroup(groupId);

        auto payments =
            paymentRepo.getPaymentsByGroup(groupId);

        if(expenses.empty())
        {
            std::cout << "\nNo Expenses Found\n";
            return;
        }

        for(const auto& expense : expenses)
        {
            std::cout << "\n---------------------\n";
            std::cout << "Expense ID: " << expense.getId() << "\n";
            std::cout << "Title: " << expense.getTitle() << "\n";
            std::cout << "Amount: ₹" << expense.getAmount() << "\n";
            std::cout << "\nPayments:\n";

            for(const auto& payment : payments)
            {
                if(payment.getExpenseId() != expense.getId())
                {
                    continue;
                }

                std::string memberName = "Unknown";

                for(const auto& member : members)
                {
                    if(member.getId() == payment.getMemberId())
                    {
                        memberName = member.getName();
                        break;
                    }
                }

                std::cout << memberName << " ₹" << payment.getAmountPaid() << "\n";
            }
        }
    }

    void viewDashboard(
        int groupId,
        const std::vector<Member>& members
    )
    {
        auto expenses =
            expenseRepo.getExpensesByGroup(groupId);

        auto payments =
            paymentRepo.getPaymentsByGroup(groupId);

        int memberCount = static_cast<int>(members.size());
        int expenseCount = 0;

        double totalSpending = 0;
        double largestAmount = 0;

        std::string largestExpense = "None";

        std::unordered_map<int, double> contributionMap;
        std::unordered_map<int, int> paymentFrequency;

        for(const auto& expense : expenses)
        {
            expenseCount++;
            totalSpending += expense.getAmount();

            if(expense.getAmount() > largestAmount)
            {
                largestAmount = expense.getAmount();
                largestExpense = expense.getTitle();
            }
        }

        for(const auto& payment : payments)
        {
            contributionMap[payment.getMemberId()] += payment.getAmountPaid();
            paymentFrequency[payment.getMemberId()]++;
        }

        double averageExpense = 0;

        if(expenseCount > 0)
        {
            averageExpense = totalSpending / expenseCount;
        }

        double maxContribution = 0;
        std::string topContributor = "None";

        int maxFrequency = 0;
        std::string frequentPayer = "None";

        for(const auto& member : members)
        {
            if(contributionMap[member.getId()] > maxContribution)
            {
                maxContribution = contributionMap[member.getId()];
                topContributor = member.getName();
            }

            if(paymentFrequency[member.getId()] > maxFrequency)
            {
                maxFrequency = paymentFrequency[member.getId()];
                frequentPayer = member.getName();
            }
        }

        std::cout
            << "\n=================================\n"
            << "         DASHBOARD\n"
            << "=================================\n";

        std::cout << "\nMembers : " << memberCount;
        std::cout << "\nExpenses : " << expenseCount;
        std::cout << "\nTotal Spending : ₹" << totalSpending;
        std::cout << "\nAverage Expense : ₹" << averageExpense;
        std::cout << "\n\nLargest Expense : " << largestExpense << " ₹" << largestAmount;
        std::cout << "\n\nTop Contributor : " << topContributor << " ₹" << maxContribution;
        std::cout << "\nMost Frequent Payer : " << frequentPayer << " (" << maxFrequency << " payments)";
        std::cout << "\n=================================\n";
    }

    void deleteExpense(
        int groupId,
        int expenseId
    )
    {
        expenseRepo.deleteExpense(groupId, expenseId);
        paymentRepo.deletePaymentsByExpenseId(groupId, expenseId);
    }

    std::vector<Expense> getExpensesByGroup(
        int groupId
    )
    {
        return expenseRepo.getExpensesByGroup(groupId);
    }

    double getMemberBalance(
        int groupId,
        int memberId,
        const std::vector<Member>& members
    )
    {
        auto balances = computeBalances(groupId, members);

        return balances[memberId];
    }
};
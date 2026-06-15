#include "FileExpenseRepository.h"

#include <fstream>
#include <sstream>
#include <iostream>

void FileExpenseRepository::saveExpense(const Expense& expense)
{
    std::ofstream file("../data/expenses.txt", std::ios::app);

    file << expense.getId()
         << ","
         << expense.getGroupId()
         << ","
         << expense.getTitle()
         << ","
         << expense.getAmount()
         << "\n";
}

std::vector<Expense> FileExpenseRepository::getAllExpenses()
{
    std::vector<Expense> expenses;

    std::ifstream file("../data/expenses.txt");

    std::string line;

    while(std::getline(file, line))
    {
        if(line.empty()) continue;

        std::stringstream ss(line);

        std::string id, groupId, title, amount;

        std::getline(ss, id, ',');
        std::getline(ss, groupId, ',');
        std::getline(ss, title, ',');
        std::getline(ss, amount);

        expenses.emplace_back(
            std::stoi(id),
            std::stoi(groupId),
            title,
            std::stod(amount)
        );
    }

    return expenses;
}
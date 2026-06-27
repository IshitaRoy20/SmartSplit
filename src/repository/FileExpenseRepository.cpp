#include "../include/repository/FileExpenseRepository.h"
#include "../include/utilities/ConfigPath.h"
#include <fstream>
#include <sstream>
#include <iostream>

void FileExpenseRepository::saveExpense(const Expense& expense)
{
    std::ofstream file(Config::Path::getExpensesFile(), std::ios::app);

    if(!file.is_open())
    {
        std::cerr << "Error: Could not open expenses file for writing.\n";
        return;
    }

    file << expense.getId() << "," << expense.getGroupId() << "," 
         << expense.getTitle() << "," << expense.getAmount() << "\n";
    file.close();
}

std::vector<Expense> FileExpenseRepository::getAllExpenses()
{
    std::vector<Expense> expenses;
    std::ifstream file(Config::Path::getExpensesFile());

    if(!file.is_open())
        return expenses;

    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty()) 
            continue;

        std::stringstream ss(line);
        std::string id, groupId, title, amount;

        std::getline(ss, id, ',');
        std::getline(ss, groupId, ',');
        std::getline(ss, title, ',');
        std::getline(ss, amount);

        try
        {
            expenses.emplace_back(std::stoi(id), std::stoi(groupId), title, std::stod(amount));
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error parsing expense: " << e.what() << "\n";
            continue;
        }
    }

    file.close();
    return expenses;
}

void FileExpenseRepository::deleteExpense(int expenseId)
{
    auto expenses = getAllExpenses();
    std::ofstream file(Config::Path::getExpensesFile());

    if(!file.is_open())
    {
        std::cerr << "Error: Could not open expenses file for writing.\n";
        return;
    }

    for(const auto& expense : expenses)
    {
        if(expense.getId() != expenseId)
        {
            file << expense.getId() << "," << expense.getGroupId() << "," 
                 << expense.getTitle() << "," << expense.getAmount() << "\n";
        }
    }

    file.close();
}
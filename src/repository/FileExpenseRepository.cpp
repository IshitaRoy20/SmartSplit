#include "FileExpenseRepository.h"

#include <fstream>
#include <sstream>

void FileExpenseRepository::saveExpense(
    const Expense& expense
)
{
    std::ofstream file(
        "../data/expenses.txt",
        std::ios::app
    );

    file
        << expense.getId()
        << ","
        << expense.getGroupId()
        << ","
        << expense.getTitle()
        << ","
        << expense.getAmount()
        << "\n";
}

std::vector<Expense>
FileExpenseRepository::getAllExpenses()
{
    std::vector<Expense> expenses;

    std::ifstream file(
        "../data/expenses.txt"
    );

    std::string line;

    while(std::getline(file,line))
    {
        if(line.empty())
        {
            continue;
        }

        std::stringstream ss(line);

        std::string idStr;
        std::string groupIdStr;
        std::string title;
        std::string amountStr;

        std::getline(ss,idStr,',');
        std::getline(ss,groupIdStr,',');
        std::getline(ss,title,',');
        std::getline(ss,amountStr);

        expenses.emplace_back(
            std::stoi(idStr),
            std::stoi(groupIdStr),
            title,
            std::stod(amountStr)
        );
    }

    return expenses;
}
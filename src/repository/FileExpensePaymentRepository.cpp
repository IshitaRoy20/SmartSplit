#include "../include/repository/FileExpensePaymentRepository.h"
#include "../include/utilities/ConfigPath.h"
#include <fstream>
#include <sstream>
#include <iostream>

void FileExpensePaymentRepository::savePayment(const ExpensePayment& payment)
{
    std::ofstream file(Config::Path::getExpensePaymentsFile(), std::ios::app);

    if(!file.is_open())
    {
        std::cerr << "Error: Could not open expense payments file for writing.\n";
        return;
    }

    file << payment.getExpenseId() << "," << payment.getMemberId() << "," 
         << payment.getAmountPaid() << "\n";
    file.close();
}

std::vector<ExpensePayment> FileExpensePaymentRepository::getAllPayments()
{
    std::vector<ExpensePayment> payments;
    std::ifstream file(Config::Path::getExpensePaymentsFile());

    if(!file.is_open())
        return payments;

    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty()) 
            continue;

        std::stringstream ss(line);
        std::string eId, mId, amt;

        std::getline(ss, eId, ',');
        std::getline(ss, mId, ',');
        std::getline(ss, amt);

        try
        {
            payments.emplace_back(std::stoi(eId), std::stoi(mId), std::stod(amt));
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error parsing payment: " << e.what() << "\n";
            continue;
        }
    }

    file.close();
    return payments;
}

void FileExpensePaymentRepository::deletePaymentsByExpenseId(int expenseId)
{
    auto payments = getAllPayments();
    std::ofstream file(Config::Path::getExpensePaymentsFile());

    if(!file.is_open())
    {
        std::cerr << "Error: Could not open expense payments file for writing.\n";
        return;
    }

    for(const auto& payment : payments)
    {
        if(payment.getExpenseId() != expenseId)
        {
            file << payment.getExpenseId() << "," << payment.getMemberId() << "," 
                 << payment.getAmountPaid() << "\n";
        }
    }

    file.close();
}
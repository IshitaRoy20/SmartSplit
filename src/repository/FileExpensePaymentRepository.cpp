#include "../include/repository/FileExpensePaymentRepository.h"

#include <fstream>
#include <sstream>

void FileExpensePaymentRepository::savePayment(const ExpensePayment& payment)
{
    std::ofstream file("../data/expense_payments.txt", std::ios::app);

    file << payment.getExpenseId()
         << ","
         << payment.getMemberId()
         << ","
         << payment.getAmountPaid()
         << "\n";
}

std::vector<ExpensePayment> FileExpensePaymentRepository::getAllPayments()
{
    std::vector<ExpensePayment> payments;

    std::ifstream file("../data/expense_payments.txt");

    std::string line;

    while(std::getline(file, line))
    {
        if(line.empty()) continue;

        std::stringstream ss(line);

        std::string eId, mId, amt;

        std::getline(ss, eId, ',');
        std::getline(ss, mId, ',');
        std::getline(ss, amt);

        payments.emplace_back(
            std::stoi(eId),
            std::stoi(mId),
            std::stod(amt)
        );
    }

    return payments;
}
void FileExpensePaymentRepository::
deletePaymentsByExpenseId(
    int expenseId
)
{
    auto payments =
        getAllPayments();

    std::ofstream file(
        "../data/expense_payments.txt"
    );

    for(const auto& payment :
        payments)
    {
        if(payment.getExpenseId()
           == expenseId)
        {
            continue;
        }

        file
            << payment.getExpenseId()
            << ","
            << payment.getMemberId()
            << ","
            << payment.getAmountPaid()
            << "\n";
    }
}
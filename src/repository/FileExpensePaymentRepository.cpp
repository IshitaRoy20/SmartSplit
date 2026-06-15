#include "FileExpensePaymentRepository.h"

#include <fstream>
#include <sstream>

void FileExpensePaymentRepository::savePayment(
    const ExpensePayment& payment
)
{
    std::ofstream file(
        "../data/expense_payments.txt",
        std::ios::app
    );

    file
        << payment.getExpenseId()
        << ","
        << payment.getMemberId()
        << ","
        << payment.getAmountPaid()
        << "\n";
}

std::vector<ExpensePayment>
FileExpensePaymentRepository::getAllPayments()
{
    std::vector<ExpensePayment> payments;

    std::ifstream file(
        "../data/expense_payments.txt"
    );

    std::string line;

    while(std::getline(file,line))
    {
        if(line.empty())
        {
            continue;
        }

        std::stringstream ss(line);

        std::string expenseId;
        std::string memberId;
        std::string amount;

        std::getline(ss,expenseId,',');
        std::getline(ss,memberId,',');
        std::getline(ss,amount);

        payments.emplace_back(
            std::stoi(expenseId),
            std::stoi(memberId),
            std::stod(amount)
        );
    }

    return payments;
}
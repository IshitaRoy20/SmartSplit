#pragma once

#include "IExpensePaymentRepository.h"

class FileExpensePaymentRepository
    : public IExpensePaymentRepository
{
public:

    void savePayment(
        const ExpensePayment& payment
    ) override;
    void deletePaymentsByExpenseId(
    int expenseId
    );
    std::vector<ExpensePayment>
    getAllPayments() override;
};
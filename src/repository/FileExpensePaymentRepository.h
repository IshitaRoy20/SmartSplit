#pragma once

#include "IExpensePaymentRepository.h"

class FileExpensePaymentRepository
    : public IExpensePaymentRepository
{
public:

    void savePayment(
        const ExpensePayment& payment
    ) override;

    std::vector<ExpensePayment>
    getAllPayments() override;
};
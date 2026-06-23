#pragma once

#include "../models/ExpensePayment.h"

#include <vector>

class IExpensePaymentRepository
{
public:

    virtual void savePayment(
        const ExpensePayment& payment
    ) = 0;

    virtual std::vector<ExpensePayment>
    getAllPayments() = 0;

    virtual ~IExpensePaymentRepository() = default;
};
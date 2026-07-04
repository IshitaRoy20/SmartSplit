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
    getPaymentsByGroup(
        int groupId
    ) = 0;

    virtual void deletePaymentsByExpenseId(
        int groupId,
        int expenseId
    ) = 0;

    virtual ~IExpensePaymentRepository() = default;
};

#pragma once

#include "IExpensePaymentRepository.h"
#include "../cache/LRUCache.h"

class SQLiteExpensePaymentRepository
    : public IExpensePaymentRepository
{
private:

    LRUCache<int, std::vector<ExpensePayment>> cache;

public:

    SQLiteExpensePaymentRepository();

    void savePayment(
        const ExpensePayment& payment
    ) override;

    std::vector<ExpensePayment>
    getPaymentsByGroup(
        int groupId
    ) override;

    void deletePaymentsByExpenseId(
        int groupId,
        int expenseId
    ) override;
};

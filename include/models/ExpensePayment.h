#pragma once

class ExpensePayment
{
private:

    int expenseId;

    int memberId;

    double amountPaid;

public:

    ExpensePayment() = default;

    ExpensePayment(
        int expenseId,
        int memberId,
        double amountPaid
    )
        : expenseId(expenseId),
          memberId(memberId),
          amountPaid(amountPaid)
    {}

    int getExpenseId() const
    {
        return expenseId;
    }

    int getMemberId() const
    {
        return memberId;
    }

    double getAmountPaid() const
    {
        return amountPaid;
    }
};
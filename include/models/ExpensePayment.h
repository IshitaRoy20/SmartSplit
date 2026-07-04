#pragma once

class ExpensePayment
{
private:

    int expenseId;

    int groupId;

    int memberId;

    double amountPaid;

public:

    ExpensePayment() = default;

    ExpensePayment(
        int expenseId,
        int groupId,
        int memberId,
        double amountPaid
    )
        : expenseId(expenseId),
          groupId(groupId),
          memberId(memberId),
          amountPaid(amountPaid)
    {}

    int getExpenseId() const
    {
        return expenseId;
    }

    int getGroupId() const
    {
        return groupId;
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

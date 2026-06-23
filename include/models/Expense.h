#pragma once

#include <string>

class Expense
{
private:

    int id;
    int groupId;

    std::string title;

    double amount;

public:

    Expense() = default;

    Expense(
        int id,
        int groupId,
        const std::string& title,
        double amount
    )
        : id(id),
          groupId(groupId),
          title(title),
          amount(amount)
    {}

    int getId() const
    {
        return id;
    }

    int getGroupId() const
    {
        return groupId;
    }

    std::string getTitle() const
    {
        return title;
    }

    double getAmount() const
    {
        return amount;
    }
};
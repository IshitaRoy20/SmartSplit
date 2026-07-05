#pragma once

struct Balance
{
    int memberId;
    double amount;
};

struct SettlementTransaction
{
    int fromMemberId;
    int toMemberId;
    double amount;
};

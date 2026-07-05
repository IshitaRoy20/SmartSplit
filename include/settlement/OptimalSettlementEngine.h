#pragma once

#include "ISettlementEngine.h"

class OptimalSettlementEngine
    : public ISettlementEngine
{
private:

    int bestTransactionCount;

    std::vector<SettlementTransaction> bestTransactions;

    void solve(
        std::vector<double>& amounts,
        std::vector<int>& memberIds,
        int index,
        std::vector<SettlementTransaction>& current
    );

public:

    std::vector<SettlementTransaction> computeSettlements(
        std::vector<Balance> balances
    ) override;

    std::string name() const override;

    std::string complexity() const override;
};

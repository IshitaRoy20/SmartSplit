#pragma once

#include "ISettlementEngine.h"

class HybridGreedySettlementEngine
    : public ISettlementEngine
{
public:

    std::vector<SettlementTransaction> computeSettlements(
        std::vector<Balance> balances
    ) override;

    std::string name() const override;

    std::string complexity() const override;
};

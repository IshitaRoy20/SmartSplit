#pragma once

#include "SettlementTypes.h"

#include <vector>
#include <string>

class ISettlementEngine
{
public:

    virtual std::vector<SettlementTransaction> computeSettlements(
        std::vector<Balance> balances
    ) = 0;

    virtual std::string name() const = 0;

    virtual std::string complexity() const = 0;

    virtual ~ISettlementEngine() = default;
};

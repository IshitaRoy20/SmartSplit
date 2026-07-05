#pragma once

#include "SettlementTypes.h"

#include <vector>

class ExactMatchPreprocessor
{
public:

    static std::vector<SettlementTransaction> extractExactMatches(
        std::vector<Balance>& balances
    );
};

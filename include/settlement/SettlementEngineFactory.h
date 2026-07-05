#pragma once

#include "ISettlementEngine.h"
#include "OptimalSettlementEngine.h"
#include "HybridGreedySettlementEngine.h"

#include <memory>

class SettlementEngineFactory
{
public:

    static const int OPTIMAL_THRESHOLD = 15;

    static std::unique_ptr<ISettlementEngine> create(int nonZeroParticipants)
    {
        if(nonZeroParticipants <= OPTIMAL_THRESHOLD)
        {
            return std::make_unique<OptimalSettlementEngine>();
        }

        return std::make_unique<HybridGreedySettlementEngine>();
    }
};

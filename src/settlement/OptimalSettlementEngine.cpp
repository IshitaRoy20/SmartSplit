#include "../../include/settlement/OptimalSettlementEngine.h"

#include <cmath>
#include <algorithm>

void OptimalSettlementEngine::solve(
    std::vector<double>& amounts,
    std::vector<int>& memberIds,
    int index,
    std::vector<SettlementTransaction>& current
)
{
    while(index < static_cast<int>(amounts.size()) && std::abs(amounts[index]) < 0.01)
    {
        index++;
    }

    if(index == static_cast<int>(amounts.size()))
    {
        if(static_cast<int>(current.size()) < bestTransactionCount)
        {
            bestTransactionCount = static_cast<int>(current.size());
            bestTransactions = current;
        }

        return;
    }

    if(static_cast<int>(current.size()) >= bestTransactionCount)
    {
        return;
    }

    for(int j = index + 1; j < static_cast<int>(amounts.size()); j++)
    {
        if(amounts[j] * amounts[index] < 0)
        {
            double settleAmount = std::min(std::abs(amounts[index]), std::abs(amounts[j]));

            SettlementTransaction txn;

            if(amounts[index] < 0)
            {
                txn.fromMemberId = memberIds[index];
                txn.toMemberId = memberIds[j];
            }
            else
            {
                txn.fromMemberId = memberIds[j];
                txn.toMemberId = memberIds[index];
            }

            txn.amount = settleAmount;
            current.push_back(txn);

            double originalIndexAmount = amounts[index];
            double originalJAmount = amounts[j];

            if(amounts[index] < 0)
            {
                amounts[index] += settleAmount;
                amounts[j] -= settleAmount;
            }
            else
            {
                amounts[index] -= settleAmount;
                amounts[j] += settleAmount;
            }

            int nextIndex = (std::abs(amounts[index]) < 0.01) ? index + 1 : index;

            solve(amounts, memberIds, nextIndex, current);

            current.pop_back();
            amounts[index] = originalIndexAmount;
            amounts[j] = originalJAmount;
        }
    }
}

std::vector<SettlementTransaction> OptimalSettlementEngine::computeSettlements(
    std::vector<Balance> balances
)
{
    std::vector<double> amounts;
    std::vector<int> memberIds;

    for(const auto& balance : balances)
    {
        amounts.push_back(balance.amount);
        memberIds.push_back(balance.memberId);
    }

    bestTransactionCount = static_cast<int>(amounts.size());
    bestTransactions.clear();

    std::vector<SettlementTransaction> current;

    solve(amounts, memberIds, 0, current);

    return bestTransactions;
}

std::string OptimalSettlementEngine::name() const
{
    return "Optimal (Exact Backtracking)";
}

std::string OptimalSettlementEngine::complexity() const
{
    return "Exponential worst case, only run on small participant counts";
}

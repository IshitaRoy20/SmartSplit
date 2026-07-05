#include "../../include/settlement/ExactMatchPreprocessor.h"

#include <unordered_map>
#include <unordered_set>
#include <cmath>

std::vector<SettlementTransaction> ExactMatchPreprocessor::extractExactMatches(
    std::vector<Balance>& balances
)
{
    std::unordered_map<long long, std::vector<int>> creditorsByAmount;
    std::unordered_map<long long, std::vector<int>> debtorsByAmount;

    for(const auto& balance : balances)
    {
        long long cents = std::llround(std::abs(balance.amount) * 100.0);

        if(balance.amount > 0.01)
        {
            creditorsByAmount[cents].push_back(balance.memberId);
        }
        else if(balance.amount < -0.01)
        {
            debtorsByAmount[cents].push_back(balance.memberId);
        }
    }

    std::vector<SettlementTransaction> settled;
    std::unordered_set<int> resolvedMemberIds;

    for(auto& entry : creditorsByAmount)
    {
        long long cents = entry.first;
        auto& creditorIds = entry.second;

        auto it = debtorsByAmount.find(cents);

        if(it == debtorsByAmount.end())
        {
            continue;
        }

        auto& debtorIds = it->second;

        size_t pairCount = std::min(creditorIds.size(), debtorIds.size());

        for(size_t k = 0; k < pairCount; k++)
        {
            SettlementTransaction txn;
            txn.fromMemberId = debtorIds[k];
            txn.toMemberId = creditorIds[k];
            txn.amount = cents / 100.0;

            settled.push_back(txn);

            resolvedMemberIds.insert(debtorIds[k]);
            resolvedMemberIds.insert(creditorIds[k]);
        }
    }

    std::vector<Balance> remaining;

    for(const auto& balance : balances)
    {
        if(resolvedMemberIds.find(balance.memberId) == resolvedMemberIds.end())
        {
            remaining.push_back(balance);
        }
    }

    balances = remaining;

    return settled;
}

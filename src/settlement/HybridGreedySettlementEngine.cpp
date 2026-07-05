#include "../../include/settlement/HybridGreedySettlementEngine.h"

#include <queue>
#include <vector>

namespace
{
    struct HeapItem
    {
        double amount;
        int memberId;
    };

    struct CompareByAmount
    {
        bool operator()(const HeapItem& a, const HeapItem& b) const
        {
            return a.amount < b.amount;
        }
    };

    using MaxHeap = std::priority_queue<HeapItem, std::vector<HeapItem>, CompareByAmount>;
}

std::vector<SettlementTransaction> HybridGreedySettlementEngine::computeSettlements(
    std::vector<Balance> balances
)
{
    MaxHeap creditors;
    MaxHeap debtors;

    for(const auto& balance : balances)
    {
        if(balance.amount > 0.01)
        {
            creditors.push({balance.amount, balance.memberId});
        }
        else if(balance.amount < -0.01)
        {
            debtors.push({-balance.amount, balance.memberId});
        }
    }

    std::vector<SettlementTransaction> transactions;

    while(!creditors.empty() && !debtors.empty())
    {
        HeapItem creditor = creditors.top();
        creditors.pop();

        HeapItem debtor = debtors.top();
        debtors.pop();

        double amount = std::min(creditor.amount, debtor.amount);

        SettlementTransaction txn;
        txn.fromMemberId = debtor.memberId;
        txn.toMemberId = creditor.memberId;
        txn.amount = amount;

        transactions.push_back(txn);

        creditor.amount -= amount;
        debtor.amount -= amount;

        if(creditor.amount > 0.01)
        {
            creditors.push(creditor);
        }

        if(debtor.amount > 0.01)
        {
            debtors.push(debtor);
        }
    }

    return transactions;
}

std::string HybridGreedySettlementEngine::name() const
{
    return "Hybrid Greedy (Priority Queue)";
}

std::string HybridGreedySettlementEngine::complexity() const
{
    return "O(n log n)";
}

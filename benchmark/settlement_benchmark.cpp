#include "../include/settlement/OptimalSettlementEngine.h"
#include "../include/settlement/HybridGreedySettlementEngine.h"
#include "../include/settlement/ExactMatchPreprocessor.h"
#include "../include/settlement/SettlementEngineFactory.h"

#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

std::vector<Balance> generateWorstCaseBalances(int n, std::mt19937& rng)
{
    std::uniform_real_distribution<double> dist(10.0, 999.0);

    std::vector<double> amounts;
    double total = 0;

    for(int i = 0; i < n - 1; i++)
    {
        double amt = dist(rng);
        int sign = (i % 2 == 0) ? 1 : -1;
        amounts.push_back(sign * amt);
        total += sign * amt;
    }

    amounts.push_back(-total);
    std::shuffle(amounts.begin(), amounts.end(), rng);

    std::vector<Balance> balances;

    for(int i = 0; i < n; i++)
    {
        balances.push_back({i, amounts[i]});
    }

    return balances;
}

void benchmarkOptimalGrowth()
{
    std::cout << "\n===== OPTIMAL (EXACT BACKTRACKING) — GROWTH BY GROUP SIZE =====\n";
    std::cout << "n\tTime (ms)\tTransactions\n";

    for(int n = 4; n <= 18; n++)
    {
        std::mt19937 rng(100);

        auto balances = generateWorstCaseBalances(n, rng);

        OptimalSettlementEngine engine;

        auto start = Clock::now();
        auto txns = engine.computeSettlements(balances);
        auto end = Clock::now();

        double ms = std::chrono::duration<double, std::milli>(end - start).count();

        std::cout << n << "\t" << ms << "\t\t" << txns.size() << "\n" << std::flush;

        if(ms > 5000)
        {
            std::cout << "Optimal engine exceeded 5 seconds at n = " << n << ". Stopping growth scan.\n" << std::flush;
            break;
        }
    }
}

void benchmarkOptimalVsGreedy()
{
    std::cout << "\n===== OPTIMAL vs HYBRID GREEDY — HEAD TO HEAD =====\n";
    std::cout << "n\tOptimal(ms)\tGreedy(ms)\tOptimal_txns\tGreedy_txns\n";

    for(int n = 4; n <= 16; n += 2)
    {
        std::mt19937 rng(42);

        auto balances = generateWorstCaseBalances(n, rng);

        OptimalSettlementEngine optimal;
        auto start = Clock::now();
        auto optTxns = optimal.computeSettlements(balances);
        auto end = Clock::now();
        double optMs = std::chrono::duration<double, std::milli>(end - start).count();

        HybridGreedySettlementEngine greedy;
        start = Clock::now();
        auto greedyTxns = greedy.computeSettlements(balances);
        end = Clock::now();
        double greedyMs = std::chrono::duration<double, std::milli>(end - start).count();

        std::cout << n << "\t" << optMs << "\t\t" << greedyMs
                   << "\t\t" << optTxns.size() << "\t\t" << greedyTxns.size() << "\n";
    }
}

void benchmarkExactMatchPreprocessor()
{
    std::cout << "\n===== EXACT MATCH PREPROCESSOR — SHRINKING THE INPUT =====\n";

    std::mt19937 rng(7);

    std::vector<Balance> balances;

    for(int i = 0; i < 10; i++)
    {
        balances.push_back({i, 100.0 + i});
    }

    for(int i = 10; i < 20; i++)
    {
        balances.push_back({i, -(100.0 + (i - 10))});
    }

    std::cout << "Participants before exact-match pass : " << balances.size() << "\n";

    auto exactMatches = ExactMatchPreprocessor::extractExactMatches(balances);

    std::cout << "Exact matches pre-settled            : " << exactMatches.size() << "\n";
    std::cout << "Participants remaining for the engine : " << balances.size() << "\n";
}

int main()
{
    benchmarkOptimalGrowth();
    benchmarkOptimalVsGreedy();
    benchmarkExactMatchPreprocessor();

    std::cout << "\n===== FACTORY DISPATCH CHECK =====\n";
    auto engineSmall = SettlementEngineFactory::create(10);
    auto engineLarge = SettlementEngineFactory::create(20);
    std::cout << "10 participants -> " << engineSmall->name() << "\n";
    std::cout << "20 participants -> " << engineLarge->name() << "\n";

    return 0;
}

# 🚀 SmartSplit — Adaptive, SQLite-Backed, Thread-Safe Expense Settlement Engine

SmartSplit splits expenses and settles debts with an **adaptive settlement engine**: exact backtracking that *guarantees* the minimum number of transactions for small groups, auto-switching to an O(n log n) priority-queue greedy algorithm past the point where exact search gets exponential — a cutover measured with real benchmarks (1.5s at 15 people, 110s at 17), not guessed.

Underneath that: **SQLite** persistence, a hand-built **LRU cache** (~47x faster repeated reads), and **mutex-guarded thread safety** proven under real concurrent load (100/100 concurrent inserts land safely, vs. 6/100 without the fix).

---

## 🎯 Why SmartSplit?

Most expense apps answer "who paid how much." SmartSplit answers **"who owes whom, in the *minimum* number of transactions — computed adaptively, safely, and fast, even under concurrent access."**

---

# 🧠 Adaptive Debt Settlement Engine

A plain greedy match doesn't always minimize transaction count:

```
Creditors: A +500  B +400        Naive greedy:  4 transactions
Debtors:   C -400  D -200  E -300  True optimal: 3 transactions (C→B, D→A, E→A)
```

Finding the 3-transaction answer needs a real search, not a linear pass. So SmartSplit runs it as a **Strategy + Factory** pipeline:

```
ExactMatchPreprocessor  →  cancels perfect debtor/creditor pairs via hash map, O(1) each
        │
SettlementEngineFactory →  picks the strategy by remaining participant count
        │
        ├── OptimalSettlementEngine       exact backtracking + pruning — guaranteed minimum
        └── HybridGreedySettlementEngine  two max-heaps — O(n log n), scales to any size
```

**The exact solver is deliberately not called "DP"** — minimizing transaction count is a subset-sum/partition variant, NP-hard in general, with no polynomial DP formulation. It's real backtracking: try every debtor/creditor pairing, transfer `min(debt, credit)`, recurse, prune any branch that can't beat the best found so far.

**The size cutoff is measured, not guessed** — worst-case (adversarial, no lucky cancellations) benchmark of the exact solver:

| Participants | 13 | 14 | **15** | 16 | 17 |
|---|---|---|---|---|---|
| Time | 45ms | 285ms | **1.5s** | 5.5s | ~110s |

Roughly 6-7x growth per participant — the exponential signature of the NP-hard problem, visible in real numbers. The threshold sits right where that curve is still tolerable for an interactive session.

`ExpenseService` contains zero settlement math — it hands balances to the module and prints back whatever comes out. A fourth algorithm later is one new class and one factory line.

---

# ✨ Core Capabilities

* **Group lifecycle** — independent groups, isolated ledgers, group-specific expense tracking.
* **Group-scoped member IDs** — composite primary key `(group_id, id)`, so Group A's member `#1` and Group B's member `#1` are different people, correctly disambiguated everywhere (balances, payments, settlements) without ever leaking across groups.
* **Multi-payer expenses** — one bill, multiple contributors, each contribution tracked and reconciled.
* **Full expense ledger** — persisted in SQLite: id, title, amount, contributors, contribution amounts.
* **Financial reconciliation** — `Net Balance = Total Paid − Fair Share`, live per group.
* **Dashboard analytics** — total members/expenses/spending, average expense, largest expense, top contributor, most frequent payer.
* **Validation layer** — duplicate/invalid member and group protection, negative-payment rejection, expense-payment mismatch rejection (`Total Paid ≠ Expense Amount` is refused outright), debt-aware member removal.

---

# ⚡ LRU Caching

Every repository sits behind a generic, hand-built `LRUCache<Key, Value>` (doubly-linked list + hash map, O(1) get/put), invalidated per-group on every write.

**Benchmarked:** ~0.053 ms/call direct-to-SQLite vs. ~0.001 ms/call cached — **~47x faster** on repeated reads.

---

# 🔒 Thread Safety

One `Database` singleton owns the SQLite connection and a `std::mutex` guarding every query; the LRU cache has its own separate mutex. ID generation and insertion (`addMember`, `addGroup`, `addExpense`) are single-lock atomic operations, closing the classic check-then-act race.

**Proven:** 100 concurrent inserts through an intentionally unprotected path lost 94 to id collisions. The same 100 through the real mutex-protected path: **100/100 landed, all unique.**

---

# 🏗️ Architecture

```text
SmartSplit-main/
├── include/cli/, src/cli/            Menu (presentation only)
├── include/services/                 GroupService, MemberService,
│                                      ExpenseService (balances only — no settlement code)
├── include/repository/, src/repository/  I*Repository interfaces + SQLite* implementations
├── include/settlement/, src/settlement/  ISettlementEngine, OptimalSettlementEngine,
│                                      HybridGreedySettlementEngine, SettlementEngineFactory,
│                                      ExactMatchPreprocessor, SettlementTypes
├── include/cache/                    LRUCache.h (generic, thread-safe)
├── include/database/, src/database/  Database (singleton, shared mutex, schema init)
├── include/models/                   Group, Member, Expense, ExpensePayment
├── benchmark/                        benchmark.cpp, settlement_benchmark.cpp
├── data/                             smartsplit.db
└── src/main.cpp
```

**Patterns demonstrated:** Repository, Singleton, **Strategy + Factory** (settlement engines), Layered Architecture, OOP encapsulation, exact/greedy algorithm design, LRU caching, mutex-based concurrency control.

---

# ⚙️ Tech Stack

C++17 · SQLite3 · CMake · STL (`mutex`, `list`, `unordered_map`, `priority_queue`, `thread`) · custom LRU cache · exact backtracking + priority-queue greedy algorithms

---

# 🚀 Running It

```bash
mkdir build && cd build
cmake .. && make
./SmartSplit
```

Database and schema auto-create on first run — no manual setup.

```bash
sqlite3 data/smartsplit.db          # inspect directly
.headers on
.mode column
SELECT * FROM groups;
```

Full ER diagram and constraints: `DATABASE_SCHEMA.md`.

Reproduce the settlement benchmark table:

```bash
g++ -std=c++17 -O2 -Iinclude benchmark/settlement_benchmark.cpp \
  src/settlement/ExactMatchPreprocessor.cpp \
  src/settlement/OptimalSettlementEngine.cpp \
  src/settlement/HybridGreedySettlementEngine.cpp \
  -o settlement_benchmark && ./settlement_benchmark
```

---

# 💡 What Makes This Stand Out

Not a CRUD app. It's SQLite-backed persistence, a proven ~47x LRU cache speedup, a real race condition found-and-fixed under mutex protection, group-scoped composite-key identity, and an adaptive Strategy+Factory settlement engine that guarantees minimal transactions where it can and stays fast where it can't — with every claim above backed by a number that was actually measured, not asserted.
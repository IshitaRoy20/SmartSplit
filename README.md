# 🚀 SmartSplit — Thread-Safe, SQLite-Backed, LRU-Cached Expense Sharing & Debt Optimization Engine

SmartSplit is a modular C++ expense-splitting and debt-settlement engine backed by **SQLite**, accelerated with a hand-built **LRU cache** (~47x faster repeated reads), and made **thread-safe** with mutex-guarded, atomic data access — proven under real concurrent load, not just assumed.

---

## 🎯 Why SmartSplit?

Most expense-sharing applications answer:

> "Who paid how much?"

SmartSplit answers:

> "Who actually owes whom, by how much, and what is the minimum number of transactions required to settle everything — safely, fast, and correctly, even under concurrent access?"

The system transforms raw payment records into an optimized settlement network, helping groups settle debts efficiently while preserving complete financial history in a real relational database.

---

# ✨ Core Capabilities

## 👥 Group Lifecycle Management

Create and manage multiple independent expense groups.

Examples:

* College roommates
* Trip planning groups
* Office teams
* Event organizers
* Shared apartment expenses

Features:

* Group creation
* Group isolation
* Group-specific expense tracking
* Independent financial ledgers

---

## 🧑‍🤝‍🧑 Member Management Engine — Now with Group-Scoped Identity

SmartSplit maintains group membership while enforcing financial integrity — and members are no longer just *tagged* with a group, they're **identified by it**.

### 🔑 Group-Scoped Member IDs

Every member's ID is scoped to their group via a composite primary key `(group_id, id)`, not a single global counter. That means:

* Group A's member `#1` and Group B's member `#1` can both exist — they're different people, correctly disambiguated everywhere in the system.
* Member IDs stay small, sequential, and human-friendly per group instead of climbing into the hundreds across the whole application.
* Every balance, payment, and settlement query is group-scoped end to end, so there's zero risk of one group's financial data leaking into another's — even when IDs coincide.

### Implemented Safeguards

✅ Duplicate member prevention

✅ Empty-input validation

✅ Invalid member detection

✅ Invalid group protection

✅ Debt-aware member removal

A member cannot be removed if unresolved financial obligations still exist within the group.

---

## 💰 Multi-Payer Expense Processing

Unlike many beginner expense trackers, SmartSplit supports **multiple contributors for a single expense**.

Example:

Restaurant Bill ₹3000

Rahul paid ₹2000

Priya paid ₹1000

SmartSplit records the payment distribution and incorporates it into future settlement calculations.

---

## 🧾 Expense Ledger System

Every expense is permanently recorded in SQLite and remains available for inspection.

Stored information includes:

* Expense ID
* Title
* Amount
* Contributors
* Contribution amounts

Users can:

* Add expenses
* View expenses
* Inspect expense details
* Delete expenses

---

# ⚡ Financial Reconciliation Engine

The heart of SmartSplit.

For every group, the system continuously computes:

```text
Net Balance =
Total Amount Paid
−
Fair Share Of Expenses
```

This instantly identifies:

* Creditors (members owed money)
* Debtors (members who owe money)

The result is a live financial state for the entire group.

---

# 🧠 Debt Settlement Optimization

One of SmartSplit's strongest features.

Instead of producing a large number of redundant repayments, SmartSplit performs settlement minimization using a creditor-debtor matching strategy.

### Example

💡 Settlement Optimization Example

```
Before Optimization:

Rahul → Amit  ₹500
Rahul → Priya ₹300
Karan → Amit  ₹400
Karan → Priya ₹200
Neha  → Amit  ₹100
Neha  → Priya ₹500

Transactions Required: 6

After SmartSplit Optimization:

Rahul → Amit  ₹800
Karan → Priya ₹600
Neha  → Amit  ₹200
Neha  → Priya ₹400

Transactions Required: 4

📉 Transaction Reduction: 33.33%
```

The system automatically generates a minimal settlement plan from the balance graph.

---

# 📊 Settlement Analytics

SmartSplit doesn't just generate settlements.

It evaluates their efficiency.

Metrics include:

* Number of debtors
* Number of creditors
* Potential transaction count
* Optimized transaction count
* Transaction reduction percentage

This transforms settlement generation from a simple output feature into an algorithmic optimization problem.

---

# 📈 Financial Intelligence Dashboard

SmartSplit provides high-level financial analytics for every group.

### Dashboard Metrics

👥 Total Members

🧾 Total Expenses

💵 Total Group Spending

📊 Average Expense Value

🏆 Largest Expense Recorded

🥇 Top Contributor

🔥 Most Frequent Payer

These metrics provide a quick snapshot of the group's financial activity.

---

# ⚡ Performance Layer — LRU Caching

SmartSplit doesn't hit the database on every single read.

Every repository (`SQLiteGroupRepository`, `SQLiteMemberRepository`, `SQLiteExpenseRepository`, `SQLiteExpensePaymentRepository`) sits behind a hand-built, generic `LRUCache<Key, Value>` — a doubly-linked list plus a hash map, giving **O(1) get/put** with true least-recently-used eviction.

* Reads for a group's members, expenses, and payments are served from memory once cached.
* Any write immediately invalidates the affected group's cache entry, so data is never stale.
* **Benchmarked result:** repeated reads dropped from ~0.053 ms/call (hitting SQLite directly) to ~0.001 ms/call (served from cache) — a **~47x speedup** on hot-path reads.

---

# 🔒 Thread Safety — Mutex-Guarded, Race-Free by Design

SmartSplit isn't just structured for a single-threaded CLI session — its data layer is genuinely **safe under concurrent access**.

* A single shared `Database` singleton owns the SQLite connection and a `std::mutex` that guards every query.
* The LRU cache is independently protected by its own mutex, so cache reads/writes from multiple threads can never corrupt its internal list or map.
* ID generation and record insertion (`addMember`, `addGroup`, `addExpense`) are **atomic single-lock operations** — the "compute next ID" and "insert the row" steps happen under one uninterrupted lock, closing the classic check-then-act race condition.

**Proven, not assumed:** a stress test firing 100 concurrent inserts at an intentionally-unprotected code path lost 94 of them to id collisions. The same 100 concurrent inserts against SmartSplit's actual mutex-protected `addMember` landed **100 out of 100**, every id unique.

---

# 🛡️ Data Integrity & Validation Layer

A major focus of the project was preventing invalid financial states.

Implemented validations include:

* Invalid group IDs
* Invalid member IDs
* Duplicate payers
* Negative payments
* Invalid payer counts
* Empty names
* Duplicate members
* Expense-payment mismatch detection
* Outstanding debt protection

An expense is rejected if:

```text
Total Paid
≠
Expense Amount
```

preventing ghost transactions and inconsistent ledger states.

---

# 🏗️ Software Architecture

SmartSplit was redesigned around a persistent, cached, thread-safe data layer while preserving clean separation of concerns.

```text
SmartSplit-main/
│
├── src/cli/           Menu.cpp
├── include/cli/        Menu.h
│
├── include/services/    GroupService, MemberService, ExpenseService
│
├── include/repository/  IGroupRepository, IMemberRepository,
│                        IExpenseRepository, IExpensePaymentRepository
│                        SQLiteGroupRepository, SQLiteMemberRepository,
│                        SQLiteExpenseRepository, SQLiteExpensePaymentRepository
├── src/repository/      SQLite*.cpp (implementations)
│
├── include/cache/       LRUCache.h  (generic, thread-safe LRU cache)
│
├── include/database/    Database.h   (singleton, shared mutex, schema init)
├── src/database/        Database.cpp
│
├── include/models/      Group, Member, Expense, ExpensePayment
│
├── data/                smartsplit.db  (SQLite database file)
│
├── benchmark/           benchmark.cpp  (cache & concurrency benchmarks)
│
└── src/main.cpp
```

---

# 🧩 Design Patterns & Concepts Demonstrated

### Repository Pattern

Abstracts storage operations from business logic — swapped from file-based persistence to SQLite without changing a single service's public interface.

### Singleton Pattern

`Database` centralizes the one shared SQLite connection and its mutex, avoiding connection sprawl across repositories.

### Layered Architecture

CLI → Services → Repositories → SQLite Persistence

### Object-Oriented Design

Encapsulation of:

* Groups
* Members
* Expenses
* Payments

### Financial State Modeling

Represents real-world debt relationships using computed balances.

### Greedy Optimization

Used for settlement minimization and transaction reduction.

### Data Validation Pipelines

Protects the application from invalid financial states.

### Caching Strategy

Generic LRU cache with per-group invalidation-on-write, cutting repeated-read latency by ~47x.

### Concurrency Control

Mutex-protected singleton database access with atomic id-generation-plus-insert operations, verified under real concurrent load.

---

# ⚙️ Technologies

* C++17
* STL (`std::mutex`, `std::list`, `std::unordered_map`, `std::thread`)
* **SQLite3** (relational persistence)
* CMake
* Custom **LRU Cache** implementation
* **Thread-safe**, mutex-guarded data access
* Object-Oriented Programming

---

# 🚀 Running SmartSplit

### Build

```bash
mkdir build
cd build
cmake ..
make
```

### Run

```bash
./SmartSplit
```

The SQLite database (`data/smartsplit.db`) and its schema are created automatically on first launch — no manual setup required.

### Inspect the database directly

```bash
sqlite3 data/smartsplit.db
```

```sql
.headers on
.mode column
.tables
SELECT * FROM groups;
SELECT * FROM members;
```

See `DATABASE_SCHEMA.md` for the full ER diagram, table-by-table breakdown, and constraints.

---

# 💡 What Makes This Project Stand Out?

SmartSplit is not a CRUD application.

It combines:

✅ Real SQLite-backed persistence (not flat files)

✅ A hand-built, thread-safe LRU cache with a proven ~47x speedup

✅ Mutex-guarded concurrency, with a real race condition found, reproduced, and fixed

✅ Group-scoped member identity via composite primary keys

✅ Financial reconciliation

✅ Debt optimization

✅ Settlement generation

✅ Contribution analytics

✅ Ledger management

✅ Validation-driven workflows

✅ Repository-based, interface-driven architecture

The project demonstrates how software engineering principles — persistence design, caching, and concurrency control — can be applied to solve real-world financial coordination problems while maintaining clean architecture, modularity, and extensibility.

In short, SmartSplit behaves less like a simple expense tracker and more like a lightweight, concurrent, cache-accelerated financial settlement engine.
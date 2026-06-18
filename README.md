# 🚀 SmartSplit — Expense Sharing, Debt Optimization & Financial Settlement Engine

SmartSplit is a modular C++ financial management system that goes beyond traditional expense splitting by combining **expense tracking, balance reconciliation, debt settlement optimization, contribution analytics, and financial integrity validation** into a single CLI application.

Unlike basic bill-splitting applications that merely record expenses, SmartSplit computes real-time financial obligations between group members, minimizes settlement transactions, enforces consistency constraints, and provides analytical insights into spending behavior and contribution patterns.

---

## 🎯 Why SmartSplit?

Most expense-sharing applications answer:

> "Who paid how much?"

SmartSplit answers:

> "Who actually owes whom, by how much, and what is the minimum number of transactions required to settle everything?"

The system transforms raw payment records into an optimized settlement network, helping groups settle debts efficiently while preserving complete financial history.

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

## 🧑‍🤝‍🧑 Member Management Engine

SmartSplit maintains group membership while enforcing financial integrity.

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

Every expense is permanently recorded and remains available for inspection.

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

SmartSplit was designed using layered architecture principles to maintain separation of concerns and future scalability.

```text
SmartSplit/
│
├── cli/
│   ├── Menu.cpp
│   └── Menu.h
│
├── services/
│   ├── GroupService
│   ├── MemberService
│   └── ExpenseService
│
├── repository/
│   ├── FileGroupRepository
│   ├── FileMemberRepository
│   ├── FileExpenseRepository
│   └── FileExpensePaymentRepository
│
├── models/
│   ├── Group
│   ├── Member
│   ├── Expense
│   └── ExpensePayment
│
├── data/
│   ├── groups.txt
│   ├── members.txt
│   ├── expenses.txt
│   └── expense_payments.txt
│
└── main.cpp
```

---

# 🧩 Design Patterns & Concepts Demonstrated

### Repository Pattern

Abstracts storage operations from business logic.

### Layered Architecture

CLI → Services → Repositories → Persistence

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

---

# ⚙️ Technologies

* C++17
* STL
* CMake
* File-Based Persistence
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

---

# 💡 What Makes This Project Stand Out?

SmartSplit is not a CRUD application.

It combines:

✅ Financial reconciliation

✅ Debt optimization

✅ Settlement generation

✅ Contribution analytics

✅ Ledger management

✅ Validation-driven workflows

✅ Repository-based architecture

✅ Persistent storage

The project demonstrates how software engineering principles can be applied to solve real-world financial coordination problems while maintaining clean architecture, modularity, and extensibility.

In short, SmartSplit behaves less like a simple expense tracker and more like a lightweight financial settlement engine.

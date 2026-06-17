📌 SmartSplit

A modular C++17 CLI-based distributed-style expense management system designed to simulate real-world bill splitting and settlement optimization. The system supports group-based expense tracking, multi-party payments, and computes net balances using a graph-inspired settlement minimization algorithm to reduce the number of transactions required to settle debts.

SmartSplit follows a layered architecture (CLI → Service → Repository) with persistent file-based storage, emphasizing separation of concerns, deterministic financial computation, and extensible backend design.

🚀 Key Technical Features
-----------------------------
🧑‍🤝‍🧑 Group-Based Context Isolation
Each group acts as an isolated financial domain
Supports independent expense graphs per group
------
💰 Multi-Source Expense Modeling
Supports multiple payers per expense
Models expense contributions as weighted edges:
Member → Expense → Contribution Weight
------
📊 Deterministic Balance Computation Engine
Computes net balance per member using:
Total paid amount aggregation
Equal distribution assumption per expense
Implements a balance mapping structure (unordered_map) for O(n) aggregation
------
⚖️ Transaction Minimization (Greedy Settlement Engine)
Converts net balances into:
Creditors (positive balance)
Debtors (negative balance)
Uses a two-pointer greedy matching algorithm to minimize settlement count
Instead of O(n²) pairwise settlements → O(n log n) greedy resolution
------
📂 Persistent Storage Layer (File-Based DB Simulation)
Flat-file storage system simulating relational persistence:
groups.txt
members.txt
expenses.txt
expense_payments.txt
Implements append + rewrite strategy for CRUD consistency
------------------------------------------
🏗️ Layered Architecture Design
CLI Layer (Menu System)
        ↓
Service Layer (Business Logic Engine)
        ↓
Repository Layer (File I/O Abstraction)
-----------------------------------------
Design Principles:
-----------------------------------------
Separation of concerns
Dependency isolation via repository abstraction
Stateless service computation per request cycle
🧠 Core Algorithmic Concepts Used
1. Balance Sheet Construction
Uses hash maps for O(1) updates per member
2. Settlement Optimization (Greedy Matching)
Splits users into:
Creditors list
Debtors list
Matches iteratively to reduce transaction graph edges
3. Expense Normalization
Converts multi-payer contributions into normalized ledger entries
⚙️ System Capabilities
Group lifecycle management (create/list/select)
Member CRUD with validation layer
Expense creation with multi-payer allocation
Real-time balance computation
Settlement suggestion engine (debt graph reduction)

File-based persistence ensuring state recovery
📁 Project Architecture
SmartSplit/
│
├── cli/
│   └── Menu.cpp / Menu.h
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
│   ├── *.txt (persistent storage layer)
│
└── main.cpp

⚙️ How to Run SmartSplit
🔧 Prerequisites

Make sure you have:

C++17 or higher compiler (g++ recommended)
CMake (version 3.10+ recommended)
Linux / WSL / macOS (Windows via MinGW also works)
📁 Build Instructions

From the project root directory:

mkdir build
cd build
cmake ..
make
▶️ Run the Application

After successful build:

./SmartSplit
📂 Data Directory Setup

Ensure the following folder exists at project root:

data/

And it should contain (auto-created if empty, otherwise manually ensure files exist):

groups.txt
members.txt
expenses.txt
expense_payments.txt

💡 Example Flow

Once the program starts:
1. Create Group
2. Manage Group
3. List Groups
4. Exit

Inside a group:

1. Add Member
2. View Members
3. Remove Member
4. Add Expense
5. View Expenses
6. View Expense Details
7. View Balances
8. View Settlements
9. Dashboard
10. Delete Expense
11. Back

⚠️ Notes
Run from build/ directory only (important for relative paths like ../data/)
If you see file errors, ensure data/ exists at project root
All data is persisted using file-based storage (no database required)

🔬 Technical Highlights
Designed a file-based persistence engine simulating relational storage
Built a debt resolution system using greedy graph reduction
Implemented multi-party transaction normalization
Applied layered architecture with service-repository separation
Ensured deterministic financial computation with aggregation maps
Developed CLI-based stateful workspace navigation system

🔮 Future Enhancements
Percentage-based and weighted split system (dynamic allocation engine)
SQLite migration (relational persistence upgrade)
Role-based access control (RBAC model for groups)
Audit logging system (event sourcing style ledger)
Advanced settlement optimization using graph cycle reduction (min-cash-flow problem)

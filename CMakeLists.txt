cmake_minimum_required(VERSION 3.10)

project(SmartSplit)

set(CMAKE_CXX_STANDARD 17)
find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)
include_directories(src)
include_directories(include)
add_executable(
    SmartSplit
    src/main.cpp
    src/cli/Menu.cpp
    src/repository/SQLiteGroupRepository.cpp
    src/repository/SQLiteMemberRepository.cpp
    src/repository/SQLiteExpenseRepository.cpp
    src/repository/SQLiteExpensePaymentRepository.cpp
    src/settlement/ExactMatchPreprocessor.cpp
    src/settlement/OptimalSettlementEngine.cpp
    src/settlement/HybridGreedySettlementEngine.cpp
    src/utilities/ConfigPath.cpp
    src/database/Database.cpp
)
target_link_libraries(SmartSplit
    SQLite::SQLite3
    Threads::Threads
)

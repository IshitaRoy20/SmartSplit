cmake_minimum_required(VERSION 3.10)

project(SmartSplit)

set(CMAKE_CXX_STANDARD 17)
find_package(SQLite3 REQUIRED)
include_directories(src)
include_directories(include)
add_executable(
    SmartSplit
    src/main.cpp
    src/cli/Menu.cpp
    src/repository/FileGroupRepository.cpp
    src/repository/FileMemberRepository.cpp
    src/repository/FileExpenseRepository.cpp
src/repository/FileExpensePaymentRepository.cpp
src/utilities/ConfigPath.cpp
 src/database/Database.cpp
)
target_link_libraries(SmartSplit
    SQLite::SQLite3
)
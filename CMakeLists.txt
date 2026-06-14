cmake_minimum_required(VERSION 3.10)

project(SmartSplit)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(
    SmartSplit
    src/main.cpp
    src/cli/Menu.cpp
    src/repository/FileGroupRepository.cpp
)
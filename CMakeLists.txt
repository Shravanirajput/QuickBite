cmake_minimum_required(VERSION 3.16)
project(QuickBite)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QuickBite
    src/main.cpp
    src/Menu.cpp
    src/MenuItem.cpp
    src/FileManager.cpp
    src/Order.cpp
    src/OrderItem.cpp
    src\QueueManager.cpp
)

target_include_directories(QuickBite PRIVATE include)

cmake_minimum_required(VERSION 2.8)
project(aelliptic_bot)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
include_directories(tgbot-cpp/include)

set(SOURCE_FILES main.cpp)
add_executable(aelliptic_bot ${SOURCE_FILES})
target_link_libraries(aelliptic_bot ${Boost_LIBRARIES} TgBot)

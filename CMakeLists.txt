cmake_minimum_required(VERSION 3.26)
project(ORDERBOOK)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_subdirectory(core)
add_subdirectory(tests)


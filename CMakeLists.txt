cmake_minimum_required(VERSION 3.26)
project(ORDERBOOK)

set(CMAKE_CXX_STANDARD 17)

set(ENABLE_TESTS ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_subdirectory(core)

if(${ENABLE_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()

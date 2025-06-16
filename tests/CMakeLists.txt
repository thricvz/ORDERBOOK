cmake_minimum_required(VERSION 3.26)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(general_tests test_main.cpp)
add_executable(Limit_order_tests test_limit_orders.cpp)
add_executable(Market_order_tests test_market_orders.cpp)


include_directories(${CMAKE_SOURCE_DIR}/include)

target_link_libraries(general_tests GTest::gtest_main orderbookModule)
target_link_libraries(Limit_order_tests GTest::gtest_main orderbookModule)
target_link_libraries(Market_order_tests GTest::gtest_main orderbookModule)

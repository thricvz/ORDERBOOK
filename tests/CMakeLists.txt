cmake_minimum_required(VERSION 3.26)

add_executable(general_tests test_main.cpp)
add_executable(Limit_order_tests test_limit_orders.cpp)
add_executable(Market_order_tests test_market_orders.cpp)
add_subdirectory(googletest)

include_directories(${CMAKE_SOURCE_DIR}/include)

target_link_libraries(general_tests GTest::gtest_main)
target_link_libraries(Limit_order_tests GTest::gtest_main)
target_link_libraries(Market_order_tests GTest::gtest_main)

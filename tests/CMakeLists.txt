cmake_minimum_required(VERSION 3.26)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(testFunctions)


if(ENABLE_TESTS)
    add_library(additionalTestFunctions testFunctions/AdditionalTestFunctions.cpp)

    add_executable(general_tests test_main.cpp)
    target_link_libraries(general_tests GTest::gtest_main OrderbookModule additionalTestFunctions)
    add_test(NAME OrderbookDataStructuresTests COMMAND $<TARGET_FILE:general_tests>)


    add_executable(Limit_order_tests test_limit_orders.cpp)
    target_link_libraries(Limit_order_tests GTest::gtest_main OrderbookModule additionalTestFunctions)
    add_test(NAME OrderbookLimitOrderTests COMMAND $<TARGET_FILE:Limit_order_tests>)


    add_executable(Market_order_tests test_market_orders.cpp)
    target_link_libraries(Market_order_tests GTest::gtest_main OrderbookModule additionalTestFunctions)
    add_test(NAME OrderbookMarketOrderTests COMMAND $<TARGET_FILE:Market_order_tests>)

endif()
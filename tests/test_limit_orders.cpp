#include <gtest/gtest.h>
#include "orderbook.hpp"
#define DEFAULT_OWNERID 0 
//IMPROVEMENTS FOR FUTURE VERSIONS
//NO MAGIC NUMBERS IN THE TEST (ESPECIALLY IN THE ID FIELDS)

static bool equalMatchesList(MatchesList input,MatchesList expected) {
    if (input.matches.size() != expected.matches.size()) {
        return false;
    }
    int valuesFound=0;
    for (auto value:expected.matches) {
        for (int i=0; i<input.matches.size(); i++) {
            if (input.matches[i] == value) {
                valuesFound++;
                input.matches.erase(input.matches.begin()+i);
            }
        }

    }
    return valuesFound==expected.matches.size();
}

TEST(ORDERBOOK,MATCH_BUY_LIMIT_ORDER) {
    //Construct first Price Level
    Price Level1Price(6,7);
    PriceLevel *PriceLevel1=new PriceLevel(Level1Price);

    Order *order1=new Order(OrderType::MARKET,OrderSide::SELL,5,Level1Price.dollars,Level1Price.cents);
    order1->setId(1);
    PriceLevel1->add_order(order1);

    //Price 57.8
    Price Level2Price(57,8);
    PriceLevel *PriceLevel2=new PriceLevel(Level2Price);

    Order *order2=new Order(OrderType::MARKET,OrderSide::SELL,1,Level2Price.dollars,Level2Price.cents);
    order2->setId(2);

    Order *order3=new Order(OrderType::MARKET,OrderSide::SELL,1,Level2Price.dollars,Level2Price.cents);
    order3->setId(3);

    PriceLevel2->add_order(order2);
    PriceLevel2->add_order(order3);

    // Price 100 this shouldn't be matched against
    Price Level3Price(100,0);
    PriceLevel *PriceLevel3=new PriceLevel(Level3Price);

    Order *order4=new Order(OrderType::MARKET,OrderSide::SELL,1,Level3Price.dollars,Level3Price.cents);
    order4->setId(4);

    Order *order5= new Order(OrderType::MARKET,OrderSide::SELL,13,Level3Price.dollars,Level3Price.cents);
    order5->setId(5);

    Order *order6= new Order(OrderType::MARKET,OrderSide::SELL,10,Level3Price.dollars,Level3Price.cents);
    order6->setId(6);

    PriceLevel3->add_order(order5);
    PriceLevel3->add_order(order6);

    //add all levels to the Orderbook
    Fifo MatchingAlgorithhm;
    OrderBook OrderBook_(&MatchingAlgorithhm);
    OrderBook_.chargeTestOrders({},{PriceLevel1,PriceLevel2,PriceLevel3});


    // the buy order
    Order *buyOrder=new Order(OrderType::LIMIT,OrderSide::BUY,7,60,0);

    //THE MATCHING RESULT
    MatchesList expectedResult;
    OrderMatch match1(DEFAULT_OWNERID,1,5,Level1Price,OrderFillState::FULL);
    OrderMatch match2(DEFAULT_OWNERID,2,1,Level2Price,OrderFillState::FULL);
    OrderMatch match3(DEFAULT_OWNERID,3,1,Level2Price,OrderFillState::FULL);

    expectedResult.addMatch(match1);
    expectedResult.addMatch(match2);
    expectedResult.addMatch(match3);


    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchBuyLimit(buyOrder);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
    EXPECT_EQ(std::get<1>(MatchReturn),OrderFillState::FULL);
}

TEST(ORDERBOOK,MATCH_BUY_LIMIT_ORDER_NO_MATCH) {
    Price Level1Price(99,0);
    PriceLevel *PriceLevel1=new PriceLevel(Level1Price);

    Order *order1=new Order(OrderType::MARKET,OrderSide::SELL,5,Level1Price.dollars,Level1Price.cents);
    order1->setId(1);
    PriceLevel1->add_order(order1);

    //Price 57.8
    Price Level2Price(100,7);
    PriceLevel *PriceLevel2=new PriceLevel(Level2Price);

    Order *order2=new Order(OrderType::MARKET,OrderSide::SELL,1,Level2Price.dollars,Level2Price.cents);
    order2->setId(2);

    Order *order3=new Order(OrderType::MARKET,OrderSide::SELL,1,Level2Price.dollars,Level2Price.cents);
    order3->setId(3);

    PriceLevel2->add_order(order2);
    PriceLevel2->add_order(order3);


    //BUY ORDER LIMIT THAT DOESNT MATCH ANY ORDER
    Order *buyOrder=new Order(OrderType::LIMIT,OrderSide::BUY,7,60,0);

    MatchesList expectedResult;

    Fifo MatchingAlgorithm;
    OrderBook OrderBook_(&MatchingAlgorithm);
    OrderBook_.chargeTestOrders({},{PriceLevel1,PriceLevel2});

    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchBuyLimit(buyOrder);

    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
    EXPECT_EQ(std::get<1>(MatchReturn),OrderFillState::NOFILL);
    EXPECT_EQ(buyOrder->quantity,7);




}

TEST(ORDERBOOK, MATCH_BUY_LIMIT_ORDER_AGAINST_EXCEEDING_ORDERS) {
    Price Level1Price(80,453);
    PriceLevel *PriceLevel1=new PriceLevel(Level1Price);

    Order *order1=new Order(OrderType::MARKET,OrderSide::SELL,5,Level1Price.dollars,Level1Price.cents);
    order1->setId(1);
    PriceLevel1->add_order(order1);

    //Price 57.8
    Price Level2Price(88,8);
    PriceLevel *PriceLevel2=new PriceLevel(Level2Price);

    Order *order2=new Order(OrderType::MARKET,OrderSide::SELL,1,Level2Price.dollars,Level2Price.cents);
    order2->setId(2);

    Order *order3=new Order(OrderType::MARKET,OrderSide::SELL,1,Level2Price.dollars,Level2Price.cents);
    order3->setId(3);

    PriceLevel2->add_order(order2);
    PriceLevel2->add_order(order3);

    // Price 100 this shouldn't be matched against
    Price Level3Price(100,0);
    PriceLevel *PriceLevel3=new PriceLevel(Level3Price);

    Order *order4=new Order(OrderType::MARKET,OrderSide::SELL,1,Level3Price.dollars,Level3Price.cents);
    order4->setId(4);

    Order *order5=new Order(OrderType::MARKET,OrderSide::SELL,13,Level3Price.dollars,Level3Price.cents);
    order5->setId(5);

    Order *order6=new Order(OrderType::MARKET,OrderSide::SELL,10,Level3Price.dollars,Level3Price.cents);
    order6->setId(6);

    PriceLevel3->add_order(order4);
    PriceLevel3->add_order(order5);
    PriceLevel3->add_order(order6);

    //add all levels to the Orderbook
    Fifo MatchingAlgorithhm;
    OrderBook OrderBook_(&MatchingAlgorithhm);
    OrderBook_.chargeTestOrders({},{PriceLevel1,PriceLevel2,PriceLevel3});


    // the buy order
    Order *buyOrder=new Order(OrderType::LIMIT,OrderSide::BUY,1000,120,0);

    //THE MATCHING RESULT
    MatchesList expectedResult;
    OrderMatch match1(DEFAULT_OWNERID,1,5,Level1Price,OrderFillState::FULL);
    OrderMatch match2(DEFAULT_OWNERID,2,1,Level2Price,OrderFillState::FULL);
    OrderMatch match3(DEFAULT_OWNERID,3,1,Level2Price,OrderFillState::FULL);
    OrderMatch match4(DEFAULT_OWNERID,4,1,Level3Price,OrderFillState::FULL);
    OrderMatch match5(DEFAULT_OWNERID,5,13,Level3Price,OrderFillState::FULL);
    OrderMatch match6(DEFAULT_OWNERID,6,10,Level3Price,OrderFillState::FULL);

    expectedResult.addMatch(match1);
    expectedResult.addMatch(match2);
    expectedResult.addMatch(match3);
    expectedResult.addMatch(match4);
    expectedResult.addMatch(match5);
    expectedResult.addMatch(match6);


    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchBuyLimit(buyOrder);
    EXPECT_EQ(std::get<1>(MatchReturn),OrderFillState::PARTIAL);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
    EXPECT_EQ(buyOrder->quantity,969);

}


TEST(ORDERBOOK,MATCH_SELL_LIMIT_ORDER) {
    //Construct first Price Level
    Price Level1Price(6,7);
    PriceLevel *PriceLevel1=new PriceLevel(Level1Price);

    //these should not be matched
    Order *order1=new Order(OrderType::MARKET,OrderSide::BUY,5,Level1Price.dollars,Level1Price.cents);
    order1->setId(1);
    PriceLevel1->add_order(order1);

    //Price 57.8
    Price Level2Price(57,8);
    PriceLevel *PriceLevel2=new PriceLevel(Level2Price);

    Order *order2=new Order(OrderType::MARKET,OrderSide::BUY,1,Level2Price.dollars,Level2Price.cents);
    order2->setId(2);

    Order *order3=new Order(OrderType::MARKET,OrderSide::BUY,1,Level2Price.dollars,Level2Price.cents);
    order3->setId(3);

    PriceLevel2->add_order(order2);
    PriceLevel2->add_order(order3);

    Price Level3Price(100,0);
    PriceLevel *PriceLevel3=new PriceLevel(Level3Price);

    Order *order4=new Order(OrderType::MARKET,OrderSide::BUY,1,Level3Price.dollars,Level3Price.cents);
    order4->setId(4);

    Order *order5=new Order(OrderType::MARKET,OrderSide::BUY,13,Level3Price.dollars,Level3Price.cents);
    order5->setId(5);

    Order *order6=new Order(OrderType::MARKET,OrderSide::BUY,10,Level3Price.dollars,Level3Price.cents);
    order6->setId(6);

    PriceLevel3->add_order(order4);
    PriceLevel3->add_order(order5);
    PriceLevel3->add_order(order6);

    //add all levels to the Orderbook
    Fifo MatchingAlgorithhm;
    OrderBook OrderBook_(&MatchingAlgorithhm);
    OrderBook_.chargeTestOrders({PriceLevel1,PriceLevel2,PriceLevel3},{});


    // the buy order
    Order *sellOrder=new Order(OrderType::LIMIT,OrderSide::SELL,7,4,0);

    //THE MATCHING RESULT
    MatchesList expectedResult;
    OrderMatch match1(DEFAULT_OWNERID,4,1,Level3Price,OrderFillState::FULL);
    OrderMatch match2(DEFAULT_OWNERID,5,6,Level3Price,OrderFillState::PARTIAL);

    expectedResult.addMatch(match1);
    expectedResult.addMatch(match2);


    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchSellLimit(sellOrder);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
    EXPECT_EQ(std::get<1>(MatchReturn),OrderFillState::FULL);
    EXPECT_EQ(sellOrder->quantity,0);

}

TEST(ORDERBOOK,MATCH_SELL_LIMIT_ORDER_NO_MATCH) {
    Price Level1Price(99,0);
    PriceLevel *PriceLevel1=new PriceLevel(Level1Price);

    Order *order1=new Order(OrderType::MARKET,OrderSide::BUY,5,Level1Price.dollars,Level1Price.cents);
    order1->setId(1);
    PriceLevel1->add_order(order1);

    //Price 57.8
    Price Level2Price(100,7);
    PriceLevel *PriceLevel2=new PriceLevel(Level2Price);

    Order *order2=new Order(OrderType::MARKET,OrderSide::BUY,1,Level2Price.dollars,Level2Price.cents);
    order2->setId(2);

    Order *order3=new Order(OrderType::MARKET,OrderSide::BUY,1,Level2Price.dollars,Level2Price.cents);
    order3->setId(3);

    PriceLevel2->add_order(order2);
    PriceLevel2->add_order(order3);


    //BUY ORDER LIMIT THAT DOESNT MATCH ANY ORDER
    Order *sellOrder=new Order(OrderType::LIMIT,OrderSide::SELL,7,200,0);

    MatchesList expectedResult;

    Fifo MatchingAlgorithm;
    OrderBook OrderBook_(&MatchingAlgorithm);
    OrderBook_.chargeTestOrders({},{PriceLevel1,PriceLevel2});

    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchSellLimit(sellOrder);

    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
    EXPECT_EQ(std::get<1>(MatchReturn),OrderFillState::NOFILL);
    EXPECT_EQ(sellOrder->quantity,7);



}


TEST(ORDERBOOK, MATCH_SELL_LIMIT_ORDER_AGAINST_EXCEEDING_ORDERS) {
    // LEAST HE IS WILLING TO SELL FOR IS FOR 20
    
    Price Level1Price(30,0);
    PriceLevel *PriceLevel1=new PriceLevel(Level1Price);

    Order *order1=new Order(OrderType::MARKET,OrderSide::BUY,5,Level1Price.dollars,Level1Price.cents);
    order1->setId(1);
    PriceLevel1->add_order(order1);

    Order *order2=new Order(OrderType::MARKET,OrderSide::BUY,2,Level1Price.dollars,Level1Price.cents);
    order2->setId(2);
    PriceLevel1->add_order(order2);

    Price Level2Price(100,7);
    PriceLevel *PriceLevel2=new PriceLevel(Level2Price);

    Order *order3=new Order(OrderType::MARKET,OrderSide::BUY,7,Level2Price.dollars,Level2Price.cents);
    order3->setId(3);
    PriceLevel2->add_order(order3);



    Order *sellOrder=new Order(OrderType::LIMIT,OrderSide::SELL,3,20,25);

    MatchesList expectedResult;
    OrderMatch match1(DEFAULT_OWNERID,3,3,Level2Price  ,OrderFillState::PARTIAL);
    expectedResult.addMatch(match1);


    Fifo MatchingAlgorithm;
    OrderBook OrderBook_(&MatchingAlgorithm);
    OrderBook_.chargeTestOrders({PriceLevel1,PriceLevel2},{});

    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchSellLimit(sellOrder);
    EXPECT_EQ(std::get<1>(MatchReturn),OrderFillState::FULL);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));

}





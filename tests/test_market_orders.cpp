#include <gtest/gtest.h>
#include "orderbook.h"

bool equalMatchesList(MatchesList input,MatchesList expected) {
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

TEST(BUY_MARKET_ORDER, NO_MATCHES) {
    Price Level1Price(890,2);


    Order *buyOrder = new Order(MARKET,BUY,10);


    MatchesList expectedResult;

    Fifo MatchingAlgorithm;
    OrderBook OrderBook_(&MatchingAlgorithm);
    OrderBook_.chargeTestOrders({},{});

    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchBuyMarket(buyOrder);
    EXPECT_EQ(std::get<1>(MatchReturn),OrderFillState::NOFILL);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
}




TEST(BUY_MARKET_ORDER,FULL_MATCH){

    Price Level1Price(890,2);
    PriceLevel * PriceLevel1 = new PriceLevel(Level1Price);

    Order *order1 = new Order(MARKET,SELL,20,Level1Price.dollars,Level1Price.cents);
    order1->addId(1);

    PriceLevel1->add_order(order1);


    //Building second pricelevel
    Price Level2Price(50,2);
    PriceLevel * PriceLevel2 = new PriceLevel(Level2Price);

    Order *order2 = new Order(MARKET,SELL,8,Level2Price.dollars,Level2Price.cents);
    order2->addId(2);

    PriceLevel2->add_order(order2);


    Order *buyOrder = new Order(LIMIT,BUY,22,0,0);

    MatchesList expectedResult;

    OrderMatch match1(2,8,Level2Price,FULL);
    OrderMatch match2(1,14,Level1Price,PARTIAL);

    expectedResult.addMatch(match1);
    expectedResult.addMatch(match2);

    Fifo MatchingAlgorithm;
    OrderBook OrderBook_(&MatchingAlgorithm);
    OrderBook_.chargeTestOrders({},{PriceLevel1,PriceLevel2});

    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchBuyMarket(buyOrder);
    EXPECT_EQ(std::get<1>(MatchReturn),FULL);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
}

TEST(BUY_MARKET_ORDER,PARTIAL_MATCH){

    Price Level1Price(890,2);
    PriceLevel * PriceLevel1 = new PriceLevel(Level1Price);

    Order *order1 = new Order(MARKET,SELL,10,Level1Price.dollars,Level1Price.cents);
    order1->addId(1);

    PriceLevel1->add_order(order1);


    //Building second pricelevel
    Price Level2Price(50,2);
    PriceLevel * PriceLevel2 = new PriceLevel(Level2Price);


    Order *order2 = new Order(MARKET,SELL,8,Level2Price.dollars,Level2Price.cents);
    order2->addId(2);

    PriceLevel2->add_order(order2);


    Order *buyOrder= new Order(LIMIT,BUY,22,0,0);

    MatchesList expectedResult;

    OrderMatch match1(2,8,Level2Price,FULL);
    OrderMatch match2(1,10,Level1Price,FULL);

    expectedResult.addMatch(match1);
    expectedResult.addMatch(match2);

    Fifo MatchingAlgorithm;
    OrderBook OrderBook_(&MatchingAlgorithm);
    OrderBook_.chargeTestOrders({},{PriceLevel1,PriceLevel2});

    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchBuyMarket(buyOrder);
    EXPECT_EQ(std::get<1>(MatchReturn),PARTIAL);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
}




// sell orders list
TEST(SELL_MARKET_ORDER,NO_MATCHES) {
    Price Level1Price (890,2);


    Order *sellOrder= new Order(MARKET,SELL,10);


    MatchesList expectedResult;

    Fifo MatchingAlgorithm;
    OrderBook OrderBook_(&MatchingAlgorithm);
    OrderBook_.chargeTestOrders({},{});

    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchSellMarket(sellOrder);
    EXPECT_EQ(std::get<1>(MatchReturn),OrderFillState::NOFILL);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
}


//correct tests
TEST(SELL_MARKET_ORDER,FULL_MATCH){

    Price Level1Price(200,2);
    PriceLevel *PriceLevel1 = new PriceLevel(Level1Price);

    Order *order1=new Order(MARKET,BUY,10,Level1Price.dollars,Level1Price.cents);
    order1->addId(1);

    PriceLevel1->add_order(order1);


    //Building second pricelevel
    Price Level2Price(50,2);
    PriceLevel *PriceLevel2=new PriceLevel(Level2Price);

    Order *order2=new Order(MARKET,BUY,12,Level2Price.dollars,Level2Price.cents);
    order2->addId(2);

    PriceLevel2->add_order(order2);


    Order *sellOrder=new Order(LIMIT,SELL,22,0,0);

    MatchesList expectedResult;

    OrderMatch match1(2,12,Level2Price,FULL);
    OrderMatch match2(1,10,Level1Price,FULL);

    expectedResult.addMatch(match1);
    expectedResult.addMatch(match2);

    Fifo MatchingAlgorithm;
    OrderBook OrderBook_(&MatchingAlgorithm);
    OrderBook_.chargeTestOrders({PriceLevel1,PriceLevel2},{});

    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchSellMarket(sellOrder);
    EXPECT_EQ(std::get<1>(MatchReturn),FULL);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));


}

TEST(SELL_MARKET_ORDER,PARTIAL_MATCH){



    Price Level1Price(890,2);
    PriceLevel *PriceLevel1=new PriceLevel(Level1Price);

    Order *order1=new Order(MARKET,BUY,20,Level1Price.dollars,Level1Price.cents);
    order1->addId(1);

    PriceLevel1->add_order(order1);


    //Building second pricelevel
    Price Level2Price(50,2);
    PriceLevel *PriceLevel2=new PriceLevel(Level2Price);

    Order *order2=new Order(MARKET,BUY,8,Level2Price.dollars,Level2Price.cents);
    order2->addId(2);

    PriceLevel2->add_order(order2);


    Order *sellOrder=new Order(LIMIT,SELL,50,0,0);

    MatchesList expectedResult;

    OrderMatch match1(1,20,Level1Price,FULL);
    OrderMatch match2(2,8,Level2Price,FULL);

    expectedResult.addMatch(match1);
    expectedResult.addMatch(match2);

    Fifo MatchingAlgorithm;
    OrderBook OrderBook_(&MatchingAlgorithm);
    OrderBook_.chargeTestOrders({PriceLevel1,PriceLevel2},{});

    std::pair<MatchesList,OrderFillState> MatchReturn = OrderBook_.matchSellMarket(sellOrder);
    EXPECT_EQ(std::get<1>(MatchReturn),PARTIAL);
    EXPECT_TRUE(equalMatchesList(std::get<0>(MatchReturn),expectedResult));
}



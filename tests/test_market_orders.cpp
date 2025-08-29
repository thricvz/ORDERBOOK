#include <gtest/gtest.h>
#include "orderbook.hpp"
#include "testFunctions/AdditionalTestFunctions.hpp"

TEST(BUY_MARKET_ORDER, CANT_MATCH_MARKET) {
	Price sellPrice{0,0};	
	PriceLevel* sellPriceLevel = new PriceLevel(sellPrice);	

	int sellOrder1ID = 1;
	int sellOrder2ID = 2;
	int sellOrder3ID = 3;

	Order *sellOrder1 = new Order(OrderType::MARKET,OrderSide::SELL,10,sellOrder1ID); 		
	Order *sellOrder2 = new Order(OrderType::MARKET,OrderSide::SELL,67,sellOrder2ID); 		
	Order *sellOrder3 = new Order(OrderType::MARKET,OrderSide::SELL,1,sellOrder3ID); 		

	sellPriceLevel->add_order(sellOrder1);
	sellPriceLevel->add_order(sellOrder2);
	sellPriceLevel->add_order(sellOrder3);
	

	int buyOrderID = 44;	
	Order *buyOrder  = new Order(OrderType::MARKET,OrderSide::BUY,44,buyOrderID); 		
   	
	Fifo fifoAlgorithm;
	OrderBook orderbook(&fifoAlgorithm);	 

	orderbook.chargeTestOrders({},{sellPriceLevel});
	auto matchResult = orderbook.addOrder(buyOrder);

	
	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::NOFILL);
	EXPECT_TRUE(std::get<MATCHED_ORDERS_LIST>(matchResult).noMatchesMade());
}



TEST(BUY_MARKET_ORDER,FULL_MATCH){
}

TEST(BUY_MARKET_ORDER,PARTIAL_MATCH){
}




// sell orders list
TEST(SELL_MARKET_ORDER,NO_MATCHES) {
}


//correct tests
TEST(SELL_MARKET_ORDER,FULL_MATCH){

}

TEST(SELL_MARKET_ORDER,PARTIAL_MATCH){
}



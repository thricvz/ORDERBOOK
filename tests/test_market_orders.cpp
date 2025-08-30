#include <gtest/gtest.h>
#include "orderbook.hpp"
#include "testFunctions/AdditionalTestFunctions.hpp"

TEST(BUY_MARKET_ORDER, CANT_MATCH_MARKET) {
	int sellOrder1ID = 1;
	int sellOrder2ID = 2;
	int sellOrder3ID = 3;

	Price sellPrice{0,0};	
	PriceLevel* sellPriceLevel = createPriceLevel(OrderType::MARKET,OrderSide::SELL,sellPrice,
	{
		{87,sellOrder1ID},
		{9,sellOrder2ID},
		{2,sellOrder3ID}
	});	


	int buyOrderID = 44;	
	Order *buyOrder  = new Order(OrderType::MARKET,OrderSide::BUY,44,buyOrderID); 		
   	
	Fifo fifoAlgorithm;
	auto matchResult = createOrderBookAndMatch(buyOrder,{sellPriceLevel},&fifoAlgorithm);	

	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::NOFILL);
	EXPECT_TRUE(std::get<MATCHED_ORDERS_LIST>(matchResult).noMatchesMade());
}



TEST(BUY_MARKET_ORDER,FULL_MATCH){
	int sellOrder1ID = 1;
	int sellOrder2ID = 2;
	int sellOrder3ID = 3;

	Price sellPrice1{40,4};
	PriceLevel* sellPriceLevel1 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,sellPrice1,
	{
		{87,sellOrder1ID},
		{9,sellOrder2ID},
		{2,sellOrder3ID}
	});	


	int buyOrderID = 44;	
	Order *buyOrder  = new Order(OrderType::MARKET,OrderSide::BUY,44,buyOrderID); 		
   	
	Fifo fifoAlgorithm;
	auto matchResult = createOrderBookAndMatch(buyOrder,{sellPriceLevel},&fifoAlgorithm);	

	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::NOFILL);
	EXPECT_TRUE(std::get<MATCHED_ORDERS_LIST>(matchResult).noMatchesMade());
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



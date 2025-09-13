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
	int sellOrder4ID = 4;

	Price sellPrice1{40,4};
	PriceLevel* sellPriceLevel1 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,sellPrice1,
	{
		{8,sellOrder1ID},
	});	

	Price sellPrice2{3,0};
	PriceLevel* sellPriceLevel2 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,sellPrice2,
	{
		{4,sellOrder2ID},
	});	

	Price sellPrice3{100,67};
	PriceLevel* sellPriceLevel3 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,sellPrice3,
	{
		{2,sellOrder3ID},
		{1,sellOrder4ID},
	});	


	int buyOrderID = 34;	
	Order *buyOrder  = new Order(OrderType::MARKET,OrderSide::BUY,15,buyOrderID); 		
   	
	Fifo fifoAlgorithm;
	auto matchResult = createOrderBookAndMatch(buyOrder,{sellPriceLevel1,
	sellPriceLevel2,sellPriceLevel3},&fifoAlgorithm);	
	auto resultMatchesList = std::get<MATCHED_ORDERS_LIST>(matchResult); 	

	auto expectedMatchesList = MatchesList{{
		OrderMatch{sellOrder1ID,DEFAULT_OWNERID,8,sellPrice1,OrderFillState::FULL},
		OrderMatch{sellOrder2ID,DEFAULT_OWNERID,4,sellPrice2,OrderFillState::FULL},
		OrderMatch{sellOrder3ID,DEFAULT_OWNERID,2,sellPrice3,OrderFillState::FULL},
		OrderMatch{sellOrder4ID,DEFAULT_OWNERID,1,sellPrice3,OrderFillState::FULL}
	}}; 

	
	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::FULL);
	EXPECT_TRUE(MatchesListAreEqual(resultMatchesList,expectedMatchesList));
}

TEST(BUY_MARKET_ORDER,PARTIAL_MATCH){
	int sellOrder1ID = 1;
	int sellOrder2ID = 2;

	Price sellPrice1{344,75};
	PriceLevel* sellPriceLevel1 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,sellPrice1,
	{
		{70,sellOrder1ID},
	});	

	Price sellPrice2{200,0};
	PriceLevel* sellPriceLevel2 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,sellPrice2,
	{
		{530,sellOrder2ID},
	});	



	int buyOrderID = 53;	
	Order *buyOrder  = new Order(OrderType::MARKET,OrderSide::BUY,700,buyOrderID); 		
   	
	Fifo fifoAlgorithm;
	auto matchResult = createOrderBookAndMatch(buyOrder,{sellPriceLevel1,
	sellPriceLevel2},&fifoAlgorithm);	
	auto resultMatchesList = std::get<MATCHED_ORDERS_LIST>(matchResult); 	

	auto expectedMatchesList = MatchesList{{
		OrderMatch{sellOrder2ID,DEFAULT_OWNERID,530,sellPrice2,OrderFillState::FULL},
		OrderMatch{sellOrder1ID,DEFAULT_OWNERID,70,sellPrice1,OrderFillState::FULL},
	}}; 

	
	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::PARTIAL);
	EXPECT_TRUE(MatchesListAreEqual(resultMatchesList,expectedMatchesList));
}




TEST(SELL_MARKET_ORDER,NO_MATCHES) {
	int buyOrder1ID = 1;
	int buyOrder2ID = 2;
	int buyOrder3ID = 3;

	Price buyPrice{0,0};	
	PriceLevel* buyPriceLevel = createPriceLevel(OrderType::MARKET,OrderSide::BUY,buyPrice,
	{
		{450,buyOrder1ID},
		{49,buyOrder2ID},
		{2342,buyOrder3ID}
	});	


	int sellOrderID = 44;	
	Order *sellOrder  = new Order(OrderType::MARKET,OrderSide::SELL,200,sellOrderID); 		
   	
	Fifo fifoAlgorithm;
	auto matchResult = createOrderBookAndMatch(sellOrder,{buyPriceLevel},&fifoAlgorithm);	

	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::NOFILL);
	EXPECT_TRUE(std::get<MATCHED_ORDERS_LIST>(matchResult).noMatchesMade());
}


TEST(SELL_MARKET_ORDER,FULL_MATCH){
	int buyOrder1ID = 1;
	int buyOrder2ID = 2;
	int buyOrder3ID = 3;
	int buyOrder4ID = 4;
	int buyOrder5ID = 5;


	Price buyPrice1{55,0};
	PriceLevel* buyPriceLevel1 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,buyPrice1,
	{
		{80,buyOrder1ID},
	});	

	Price buyPrice2{65,0};
	PriceLevel* buyPriceLevel2 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,buyPrice2,
	{
		{10,buyOrder2ID},
	});	

	Price buyPrice3{89,0};
	PriceLevel* buyPriceLevel3 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,buyPrice3,
	{
		{10,buyOrder3ID},
	});	

	Price buyPrice4{120,80};
	PriceLevel* buyPriceLevel4 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,buyPrice4,
	{
		{9,buyOrder4ID},
		{25,buyOrder5ID},
	});	

	int sellOrderID = 404;	

	Order *sellOrder  = new Order(OrderType::MARKET,OrderSide::SELL,43,sellOrderID); 		
   	
	Fifo fifoAlgorithm;
	auto matchResult = createOrderBookAndMatch(sellOrder,{buyPriceLevel1,
	buyPriceLevel2,buyPriceLevel3,buyPriceLevel4},&fifoAlgorithm);	

	auto resultMatchesList = std::get<MATCHED_ORDERS_LIST>(matchResult); 	

	auto expectedMatchesList = MatchesList{{
		OrderMatch{buyOrder4ID,DEFAULT_OWNERID,9,buyPrice4,OrderFillState::FULL},
		OrderMatch{buyOrder5ID,DEFAULT_OWNERID,25,buyPrice4,OrderFillState::FULL},
		OrderMatch{buyOrder3ID,DEFAULT_OWNERID,9,buyPrice3,OrderFillState::PARTIAL},
	}}; 

	
	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::FULL);
	EXPECT_TRUE(MatchesListAreEqual(resultMatchesList,expectedMatchesList));
}

TEST(SELL_MARKET_ORDER,PARTIAL_MATCH){
	int buyOrder1ID = 1;
	int buyOrder2ID = 2;

	Price buyPrice1{200,0};
	PriceLevel* buyPriceLevel1 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,buyPrice1,
	{
		{80,buyOrder1ID},
	});	

	Price buyPrice2{150,99};
	PriceLevel* buyPriceLevel2 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,buyPrice2,
	{
		{6,buyOrder2ID},
	});	

	int sellOrderID = 334;	
	Order *sellOrder  = new Order(OrderType::MARKET,OrderSide::SELL,90,sellOrderID); 		
   	
	Fifo fifoAlgorithm;
	auto matchResult = createOrderBookAndMatch(sellOrder,{buyPriceLevel1,
	buyPriceLevel2},&fifoAlgorithm);	

	auto resultMatchesList = std::get<MATCHED_ORDERS_LIST>(matchResult); 	

	auto expectedMatchesList = MatchesList{{
		OrderMatch{buyOrder1ID,DEFAULT_OWNERID,80,buyPrice1,OrderFillState::FULL},
		OrderMatch{buyOrder2ID,DEFAULT_OWNERID,6,buyPrice2,OrderFillState::FULL},
	}}; 

	
	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::PARTIAL);
	EXPECT_TRUE(MatchesListAreEqual(resultMatchesList,expectedMatchesList));
}



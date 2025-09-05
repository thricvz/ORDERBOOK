#include <gtest/gtest.h>
#include "orderbook.hpp"
#include "AdditionalTestFunctions.hpp"

TEST(SELL_LIMIT_ORDER,BUY_LIMIT_ORDER_EXACT_MATCH) {
	int buyOrder1ID = 1;
	int buyOrder2ID = 2;
	int buyOrder3ID = 3;
	
	Price accessiblePrice1{123,8};
	PriceLevel *accessiblePriceLevel1 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,accessiblePrice1,
	{
		{809,buyOrder1ID},
		{6,buyOrder2ID}
	});

	Price accessiblePrice2{55,2};
	PriceLevel *accessiblePriceLevel2 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,accessiblePrice2,
	{
		{4,buyOrder3ID}
	});

	Price minSellPrice{50,1};
	int sellOrderID = 78;
	Order *sellOrder = new Order(OrderType::LIMIT,OrderSide::SELL,819,minSellPrice,sellOrderID);

	auto fifoAlgorithm = Fifo();
	auto matchResult =  createOrderBookAndMatch(sellOrder,{accessiblePriceLevel1,accessiblePriceLevel2},&fifoAlgorithm);
	auto resultMatchesList= std::get<MATCHED_ORDERS_LIST>(matchResult);	

	auto expectedMatchesList = MatchesList{{
		OrderMatch{DEFAULT_OWNERID,buyOrder1ID,809,accessiblePrice1,OrderFillState::FULL},
		OrderMatch{DEFAULT_OWNERID,buyOrder2ID,6,accessiblePrice1,OrderFillState::FULL},
		OrderMatch{DEFAULT_OWNERID,buyOrder3ID,4,accessiblePrice2,OrderFillState::FULL}
	}}; 

	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::FULL);	
	EXPECT_TRUE(MatchesListAreEqual(resultMatchesList,expectedMatchesList));


};
TEST(SELL_LIMIT_ORDER,BUY_LIMIT_ORDER_UNSUCCESSFUL_MATCH) {
	int buyOrder1ID = 1;
	int buyOrder2ID = 2;
	int buyOrder3ID = 3;
	
	Price accessiblePrice1{123,8};
	PriceLevel *accessiblePriceLevel1 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,accessiblePrice1,
	{
		{809,buyOrder1ID},
		{6,buyOrder2ID}
	});

	Price accessiblePrice2{55,2};
	PriceLevel *accessiblePriceLevel2 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,accessiblePrice2,
	{
		{4,buyOrder3ID}
	});

	Price minSellPrice{200,0};
	int sellOrderID = 78;
	Order *sellOrder = new Order(OrderType::LIMIT,OrderSide::SELL,819,minSellPrice,sellOrderID);

	auto fifoAlgorithm = Fifo();
	auto matchResult =  createOrderBookAndMatch(sellOrder,{accessiblePriceLevel1,accessiblePriceLevel2},&fifoAlgorithm);
	auto resultMatchesList= std::get<MATCHED_ORDERS_LIST>(matchResult);	

	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::NOFILL);	
	EXPECT_TRUE(resultMatchesList.noMatchesMade());
}

TEST(SELL_LIMIT_ORDER,BUY_LIMIT_ORDER_EXCEEDING_MATCH) {
	int buyOrder1ID = 1;
	int buyOrder2ID = 2;
	int buyOrder3ID = 3;
	int buyOrder4ID = 4;

	
	Price accessiblePrice1{123,8};
	PriceLevel *accessiblePriceLevel1 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,accessiblePrice1,
	{
		{2,buyOrder1ID},
		{3,buyOrder2ID}
	});

	Price unAccessiblePrice{10,3};
	PriceLevel *unAccessiblePriceLevel = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,unAccessiblePrice,
	{
		{60,buyOrder3ID}
	});

	Price accessiblePrice2{110,0};
	PriceLevel *accessiblePriceLevel2 = createPriceLevel(OrderType::LIMIT,OrderSide::BUY,accessiblePrice2,
	{
		{5,buyOrder4ID}
	});

	Price minSellPrice{99,3};
	int sellOrderID = 78;
	Order *sellOrder = new Order(OrderType::LIMIT,OrderSide::SELL,7,minSellPrice,sellOrderID);

	auto fifoAlgorithm = Fifo();
	auto matchResult =  createOrderBookAndMatch(sellOrder,{accessiblePriceLevel1,accessiblePriceLevel2,unAccessiblePriceLevel},&fifoAlgorithm);
	auto resultMatchesList = std::get<MATCHED_ORDERS_LIST>(matchResult);	

	auto expectedMatchesList = MatchesList{{
		OrderMatch{DEFAULT_OWNERID,buyOrder1ID,2,accessiblePrice1,OrderFillState::FULL},
		OrderMatch{DEFAULT_OWNERID,buyOrder2ID,3,accessiblePrice1,OrderFillState::FULL},
		OrderMatch{DEFAULT_OWNERID,buyOrder4ID,2,accessiblePrice2,OrderFillState::PARTIAL},
	}};	  
	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::FULL);	
	EXPECT_TRUE(MatchesListAreEqual(resultMatchesList,expectedMatchesList));
}

TEST(BUY_LIMIT_ORDER,SELL_LIMIT_ORDER_EXACT_MATCH) {
}

TEST(BUY_LIMIT_ORDER,SELL_LIMIT_ORDER_UNSUCCESSFUL_MATCH) {

}

TEST(BUY_LIMIT_ORDER,SELL_LIMIT_ORDER_EXCEEDING_MATCH) {

}






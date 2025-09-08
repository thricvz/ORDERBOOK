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
		OrderMatch{buyOrder1ID,DEFAULT_OWNERID,809,accessiblePrice1,OrderFillState::FULL},
		OrderMatch{buyOrder2ID,DEFAULT_OWNERID,6,accessiblePrice1,OrderFillState::FULL},
		OrderMatch{buyOrder3ID,DEFAULT_OWNERID,4,accessiblePrice2,OrderFillState::FULL}
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
		OrderMatch{buyOrder1ID,DEFAULT_OWNERID,2,accessiblePrice1,OrderFillState::FULL},
		OrderMatch{buyOrder2ID,DEFAULT_OWNERID,3,accessiblePrice1,OrderFillState::FULL},
		OrderMatch{buyOrder4ID,DEFAULT_OWNERID,2,accessiblePrice2,OrderFillState::PARTIAL},
	}};	  
	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::FULL);	
	EXPECT_TRUE(MatchesListAreEqual(resultMatchesList,expectedMatchesList));
}

TEST(BUY_LIMIT_ORDER,SELL_LIMIT_ORDER_EXACT_MATCH) {
	int sellOrder1ID = 1;
	int sellOrder2ID = 2;

	
	Price accessiblePrice{169,0};
	PriceLevel *accessiblePriceLevel = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,accessiblePrice,
	{
		{7,sellOrder1ID},
		{30,sellOrder2ID}
	});

	Price maxBuyPrice{300,75};
	int buyOrderID = 78;
	Order *buyOrder = new Order(OrderType::LIMIT,OrderSide::BUY,37,maxBuyPrice,buyOrderID);

	auto fifoAlgorithm = Fifo();
	auto matchResult =  createOrderBookAndMatch(buyOrder,{accessiblePriceLevel},&fifoAlgorithm);
	auto resultMatchesList = std::get<MATCHED_ORDERS_LIST>(matchResult);	

	auto expectedMatchesList = MatchesList{{
		OrderMatch{sellOrder1ID,DEFAULT_OWNERID,7,accessiblePrice,OrderFillState::FULL},
		OrderMatch{sellOrder2ID,DEFAULT_OWNERID,30,accessiblePrice,OrderFillState::FULL},
	}};	  

	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::FULL);	
	EXPECT_TRUE(MatchesListAreEqual(resultMatchesList,expectedMatchesList));
}

TEST(BUY_LIMIT_ORDER,SELL_LIMIT_ORDER_UNSUCCESSFUL_MATCH) {
	int sellOrder1ID = 1;
	int sellOrder2ID = 2;
	int sellOrder3ID = 3;

	
	Price unaccessiblePrice1{34,0};
	PriceLevel *unaccessiblePriceLevel1 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,unaccessiblePrice1,
	{
		{32,sellOrder1ID},
	});

	Price unaccessiblePrice2{169,0};
	PriceLevel *unaccessiblePriceLevel2 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,unaccessiblePrice2,
	{
		{700,sellOrder2ID},
	});

	Price unaccessiblePrice3{49,99};
	PriceLevel *unaccessiblePriceLevel3 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,unaccessiblePrice3,
	{
		{7,sellOrder3ID},
	});

	Price maxBuyPrice{30,0};
	int buyOrderID = 42;
	Order *buyOrder = new Order(OrderType::LIMIT,OrderSide::BUY,10000,maxBuyPrice,buyOrderID);

	auto fifoAlgorithm = Fifo();
	auto matchResult =  createOrderBookAndMatch(buyOrder,{unaccessiblePriceLevel1,unaccessiblePriceLevel2,unaccessiblePriceLevel3},&fifoAlgorithm);
	auto resultMatchesList = std::get<MATCHED_ORDERS_LIST>(matchResult);	


	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::NOFILL);	
	EXPECT_TRUE(resultMatchesList.noMatchesMade());

}

TEST(BUY_LIMIT_ORDER,SELL_LIMIT_ORDER_EXCEEDING_MATCH) {
	int sellOrder1ID = 1;
	int sellOrder2ID = 2;
	int sellOrder3ID = 3;
	
	
	Price accessiblePrice{169,0};
	PriceLevel *accessiblePriceLevel = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,accessiblePrice,
	{
		{7,sellOrder1ID},
		{30,sellOrder2ID}
	});

	
	Price accessiblePrice2{299,99};
	PriceLevel *accessiblePriceLevel2 = createPriceLevel(OrderType::LIMIT,OrderSide::SELL,accessiblePrice2,
	{
		{233,sellOrder3ID}
	});

	Price maxBuyPrice{300,75};
	int buyOrderID = 78;
	Order *buyOrder = new Order(OrderType::LIMIT,OrderSide::BUY,40,maxBuyPrice,buyOrderID);

	auto fifoAlgorithm = Fifo();
	auto matchResult =  createOrderBookAndMatch(buyOrder,{accessiblePriceLevel,accessiblePriceLevel2},&fifoAlgorithm);
	auto resultMatchesList = std::get<MATCHED_ORDERS_LIST>(matchResult);	

	auto expectedMatchesList = MatchesList{{
		OrderMatch{sellOrder1ID,DEFAULT_OWNERID,7,accessiblePrice,OrderFillState::FULL},
		OrderMatch{sellOrder2ID,DEFAULT_OWNERID,30,accessiblePrice,OrderFillState::FULL},
		OrderMatch{sellOrder3ID,DEFAULT_OWNERID,3,accessiblePrice2,OrderFillState::PARTIAL},
	}};	  

	EXPECT_EQ(std::get<ORDER_STATE>(matchResult),OrderFillState::FULL);	
	EXPECT_TRUE(MatchesListAreEqual(resultMatchesList,expectedMatchesList));
}






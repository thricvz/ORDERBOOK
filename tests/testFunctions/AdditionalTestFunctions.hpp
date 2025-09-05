#include "orderbook.hpp"
#include <vector>
#define DEFAULT_OWNERID 0

#define MATCHED_ORDERS_LIST 0
#define ORDER_STATE 1

bool equalVectorPtr(std::vector<int*> input,std::vector<int*> expected);

bool MatchesListAreEqual(MatchesList input,MatchesList expected);

struct orderDetails{
	int orderQuantity{};
	int orderID{};
};
PriceLevel* createPriceLevel(OrderType orderType,OrderSide orderSide,
			Price priceLevelPrice, std::vector<orderDetails> orders);


std::pair<MatchesList,OrderFillState> createOrderBookAndMatch(Order *entryOrder,std::vector<PriceLevel*> restingPriceLevels,matchingAlgorithm* algorithm);



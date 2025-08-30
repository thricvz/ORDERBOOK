#include "AdditionalTestFunctions.hpp"
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


bool equalVectorPtr(std::vector<int*> input,std::vector<int*> expected) {
  if (input.size() != expected.size()) {
    return false;
  }
  int valuesFound=0;
  for (auto value:expected) {
    for (int i=0; i<input.size(); i++) {
      if (input[i] == value) {
        valuesFound++;
        input.erase(input.begin()+i);
      }
    }

  }
  return valuesFound==expected.size();
}


PriceLevel* createPriceLevel(OrderType orderType,OrderSide orderSide,
			Price priceLevelPrice, std::vector<orderDetails> orders){

		PriceLevel *newPriceLevel = new PriceLevel(priceLevelPrice);	
		for(auto orderDetail:orders){
			Order *newOrder = orderType == OrderType::MARKET? 
				new Order(OrderType::MARKET,orderSide,orderDetail.orderQuantity,orderDetail.orderID):
				new Order(OrderType::LIMIT,orderSide,orderDetail.orderQuantity,priceLevelPrice,orderDetail.orderID); 	

			newPriceLevel->add_order(newOrder);	
		};
		return newPriceLevel;
};



std::pair<MatchesList,OrderFillState> createOrderBookAndMatch(Order *entryOrder,std::vector<PriceLevel*> restingPriceLevels,matchingAlgorithm* algorithm){
	
	OrderBook orderbook(algorithm);	 
	if(entryOrder->side == OrderSide::BUY)
		orderbook.chargeTestOrders({},restingPriceLevels);
	else
		orderbook.chargeTestOrders(restingPriceLevels,{});

	auto matchResult = orderbook.addOrder(entryOrder);
	return matchResult;
}; 

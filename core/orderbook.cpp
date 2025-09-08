#include "orderbook.hpp"




bool Price::operator==(const Price& __rhs) const {
    return (dollars == __rhs.dollars) && (cents == __rhs.cents);
}
bool Price::operator<(const Price& __rhs) const {
    if(dollars < __rhs.dollars)
            return true;
    else if(dollars > __rhs.dollars)
        return false;
    else if(cents < __rhs.cents)
        return true;
    return false;
}
Order::Order(OrderType _type,OrderSide _side,int quantity_,Price price,int id){
                type = _type;
                side = _side;
                quantity = quantity_;
                oriqinalQuantity = quantity_;
                this->price = price;
                timestamp=std::chrono::steady_clock::now();
		this->id = id;
    };
Order::Order(OrderType _type,OrderSide _side,int quantity_,int id) : Order(_type,_side,quantity_,{0,0},id){
    
};

bool ordersAreCompatible(const Order& rhs,const Order& lhs){
		if((rhs.type == lhs.type) && (lhs.type == OrderType::MARKET))
			return false;
		return true;
};	
Order::Order(){
    type;
    side ;
    price  =  Price();
    quantity = 0;
    oriqinalQuantity = 0;
    int id{};
};

void Order::setId(int id_) {
    id = id_;
}


OrderFillState Order::getOrderFillState() {
    OrderFillState OrderState;
    switch (quantity) {
        case 0:
            OrderState = OrderFillState::FULL;
        break;
        default:
            quantity == oriqinalQuantity ? OrderState = OrderFillState::NOFILL: OrderState = OrderFillState::PARTIAL;
    }
    return OrderState;
}


PriceLevel::PriceLevel(Price price_){
    orders = std::vector<Order*>(0);
    price =  price_;
};


PriceLevel::PriceLevel(){
    orders = std::vector<Order*>(0);
    price =  Price();
};
void PriceLevel::add_order(Order *order){
    orders.push_back(order);
};

void PriceLevel::remove_order(int orderId){
    for(int orderIndex=0;orderIndex < orders.size();orderIndex++){
        if(orders[orderIndex]->id == orderId){
            delete orders[orderIndex];
            orders.erase(orders.begin()+orderIndex);
            return;
        }
    }
}

PriceLevel::~PriceLevel(){
	for(auto order:orders){
		delete order;
	};
};

bool OrderMatch::operator==(const OrderMatch &lhs) const {
    bool idEqual = (orderId == lhs.orderId);
    bool quantityEqual = (quantity == lhs.quantity);
    bool matchingResulEqual = (matchingResult == lhs.matchingResult);
    bool priceEqual = (price == lhs.price);
    bool ownerIdEqual = (ownerId == lhs.ownerId);
    return idEqual && quantityEqual && matchingResulEqual && priceEqual;

}




void MatchesList::addMatch(OrderMatch order){
    matches.push_back(order);
};

bool MatchesList::operator==(const MatchesList & rhs) const {
    if (rhs.matches.size() != matches.size()) {
        return false;
    }
    for (int i = 0; i < rhs.matches.size(); i++) {
        if (!(rhs.matches[i]==matches[i]))
            return false;
    }
    return true;
};

void MatchesList::extend(const MatchesList &entryList) {
    for (auto match : entryList.matches) {
        addMatch(match);
    }
}

bool MatchesList::noMatchesMade() {
    return matches.size()==0;
}


//should return true if order1 has priority over order2
bool FifoPriorityCriteria(Order order1,Order order2) {
    return order1.timestamp < order2.timestamp;
};

void Fifo::sortPriorities(PriceLevel *priceLevel,std::function<bool(Order,Order)> priority_function){
        quickSort<Order>(priceLevel->orders,0,priceLevel->orders.size()-1,FifoPriorityCriteria);

};

MatchesList Fifo::match(Order *order, PriceLevel *priceLevel){

		MatchesList matches{};

		for( auto* matchOrder :priceLevel->orders){
			if(order->quantity = 0)
					break;		

			if(!ordersAreCompatible(*order,*matchOrder));	
				continue;	
			
				
      if (matchOrder->quantity > order->quantity) {

            OrderMatch matchedOrder{matchOrder->id,matchOrder->ownerID,order->quantity,matchOrder->price,OrderFillState::PARTIAL};
            matches.addMatch(matchedOrder);
            matchOrder->quantity-=order->quantity;
            order->quantity=0;
            return matches;
			}

			
			OrderMatch matchedOrder{matchOrder->id,matchOrder->ownerID,matchOrder->quantity,matchOrder->price,OrderFillState::FULL};
			matches.addMatch(matchedOrder);
			order->quantity-=matchOrder->quantity;
			matchOrder->quantity=0;


		}
    return matches;
};


bool PriceLevelSortCompare(const PriceLevel& a, const PriceLevel&  b) {
    return a.price < b.price;
}
bool PriceLevelReverseSortCompare(const PriceLevel& a,const PriceLevel& b) {
        return b.price < a.price;
}




std::pair<MatchesList,OrderFillState> OrderBook::match(Order *order) {
    std::pair<MatchesList,OrderFillState> matchResult;
		
		if(order->side == OrderSide::BUY){
			matchResult = matchBuyOrder(order);
		}else{
			matchResult = matchSellOrder(order);
		};	

    return matchResult;
};


//need to implement a destructor for the orderbook class
 std::pair<MatchesList,OrderFillState> OrderBook::matchSellOrder(Order *order){
    //quickSort<PriceLevel>(SellOrders, 0, SellOrders.size() - 1, PriceLevelSortCompare);
    MatchesList allMatches{};
    quickSort<PriceLevel>(SellOrders, 0, SellOrders.size() - 1, PriceLevelSortCompare);

		bool isLimitOrder	= order->type==OrderType::LIMIT;

		for (auto* buyPriceLevel : SellOrders){
				if(isLimitOrder && (buyPriceLevel->price < order->price))
					break;				

				MatchesList matchesInPriceLevel = algorithm->match(order,buyPriceLevel);
        allMatches.extend(matchesInPriceLevel);


        //delete every filled order from the priceLevel
        for (auto match: matchesInPriceLevel.matches) {
            if (match.matchingResult == OrderFillState::FULL)
                buyPriceLevel->remove_order(match.orderId);
        }
		};
    
		OrderFillState orderFinalState = order->getOrderFillState();
    return std::pair<MatchesList, OrderFillState>(allMatches, orderFinalState);
};

 std::pair<MatchesList,OrderFillState> OrderBook::matchBuyOrder(Order *order){
    MatchesList allMatches{};
    quickSort<PriceLevel>(BuyOrders, 0, BuyOrders.size() - 1, PriceLevelReverseSortCompare);

		bool isLimitOrder	= order->type==OrderType::LIMIT;

		for (auto* sellPriceLevel : BuyOrders){
				if(isLimitOrder && (order->price < sellPriceLevel->price))
					break;				

				MatchesList matchesInPriceLevel = algorithm->match(order,sellPriceLevel);
        allMatches.extend(matchesInPriceLevel);


        //delete every filled order from the priceLevel
		
        for (auto match: matchesInPriceLevel.matches) {
            if (match.matchingResult == OrderFillState::FULL)
                sellPriceLevel->remove_order(match.orderId);
        }
		};
    
		OrderFillState orderFinalState = order->getOrderFillState();
    return std::pair<MatchesList, OrderFillState>(allMatches, orderFinalState);
};

OrderBook::~OrderBook(){

	for(auto pricelevel:SellOrders){
		delete pricelevel;	
	};

	for(auto pricelevel:BuyOrders){
		delete pricelevel;	
	};
}; 

std::pair<MatchesList,OrderFillState> OrderBook::addOrder(Order *entryOrder) {
    std::pair<MatchesList,OrderFillState> matchResult =  match(entryOrder);


    //erase empty pricelevels
    if (matchResult.second == OrderFillState::FULL) {
        delete entryOrder;
        return matchResult;
    }

    //add to existing priceleve
    std::vector<PriceLevel*> *OrderSideList =nullptr;

    if(entryOrder->side == SELL) {
        OrderSideList = &SellOrders;
    } else {
        OrderSideList = &BuyOrders;
    }
    //or create a new one
    for (auto priceLevelPtr : *OrderSideList ) {
        if (priceLevelPtr->price == entryOrder->price) {
            priceLevelPtr->add_order(entryOrder);
            return matchResult;
        }
    }

    PriceLevel * entryOrderPricelevel = new PriceLevel(entryOrder->price);
    OrderSideList->push_back(entryOrderPricelevel);
    return matchResult;
};

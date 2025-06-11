#ifndef ORDERBOOK_H
#define ORDERBOOK_H
#include <iostream>
#include <vector>
#include <chrono>
#include <quicksort.h>

enum OrderType{MARKET,LIMIT};
enum OrderSide{BUY,SELL};
enum OrderFillState{PARTIAL,FULL,NOFILL};

class Price{
    public:
        int dollars;
        int cents;
        Price(int dollars_,int cents_){
            dollars=dollars_;
            cents=cents_;
        }
        Price(){
            dollars=NULL;
            cents=NULL;
        }

        bool operator==(const Price& __rhs) const {
            return (dollars == __rhs.dollars) && (cents == __rhs.cents);
        }
        bool operator<(const Price& __rhs) const {
            if(dollars < __rhs.dollars)
                 return true;
            else if(dollars > __rhs.dollars)
                return false;
            else if(cents < __rhs.cents)
                return true;
            return false;
        }
};

class Order{
    public:
        OrderType type;
        OrderSide side;
        Price price;
        int quantity;
        int id;
        int oriqinalQuantity;
        //need to implement a timestamp
        std::chrono::time_point<std::chrono::steady_clock> timestamp;
        Order(OrderType _type,OrderSide _side,int quantity_,int _dollars=0,int _cents=0){
            type = _type;
            side = _side;
            quantity = quantity_;
            oriqinalQuantity = quantity_;
            price = Price(_dollars,_cents);
            int id{};
            timestamp=std::chrono::steady_clock::now();
        };
        Order(){
            type;
            side ;
            price  =  Price();
            quantity = oriqinalQuantity = 0;
            int id{};
        };
        void addId(int id_) {
            id = id_;
        }


        OrderFillState getOrderFillState() {
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

};


//adapt code for creating orders in a dynamic way
class PriceLevel{
    public:
        Price price;
        std::vector<Order*> orders;

        PriceLevel(Price price_){
           orders = std::vector<Order*>(0);
           price =  price_;
        };


        PriceLevel(){
            orders = std::vector<Order*>(0);
            price =  Price();
        };
        void add_order(Order *order){
            orders.push_back(order);
        };

        void remove_order(int orderId){
            for(int orderIndex=0;orderIndex < orders.size();orderIndex++){
                if(orders[orderIndex]->id == orderId){
                    delete orders[orderIndex];
                    orders.erase(orders.begin()+orderIndex);
                }

            }
        };
};
struct OrderMatch{
    int orderId;
    int quantity;
    OrderFillState matchingResult;
    Price price;
    OrderMatch(int id,int quantity_,Price price_,OrderFillState result){
        orderId=id;
        quantity=quantity_;
        matchingResult=result;
        price = price_;
    }
    bool operator==(const OrderMatch &lhs) const {
        bool idEqual = (orderId == lhs.orderId);
        bool quantityEqual = (quantity == lhs.quantity);
        bool matchingResulEqual = (matchingResult == lhs.matchingResult);
        bool priceEqual = (price == lhs.price);
        return idEqual && quantityEqual && matchingResulEqual && priceEqual;
    }
};

class MatchesList{
    public:
    std::vector<OrderMatch> matches;
    void addMatch(OrderMatch order){
        matches.push_back(order);
    };

    bool operator==(const MatchesList & rhs) const {
        if (rhs.matches.size() != matches.size()) {
            return false;
        }
        for (int i = 0; i < rhs.matches.size(); i++) {
            if (!(rhs.matches[i]==matches[i]))
                return false;
        }
        return true;
    };

    void extend(const MatchesList &entryList) {
        for (auto match : entryList.matches) {
            addMatch(match);
        }
    }

    bool noMatchesMade() {
        return matches.size()==0;
    }
};
class matchingAlgorithm{
    public:
        virtual void sortPriorities(PriceLevel *price_level,std::function<bool(Order,Order)> priorityDirection)=0;

        virtual MatchesList match(Order *order, PriceLevel *priceLevel) = 0;

        matchingAlgorithm() { };


};


class Fifo: public matchingAlgorithm{
    public:
        void sortPriorities(PriceLevel *priceLevel,std::function<bool(Order,Order)> priorityDirection);

        MatchesList match(Order *order, PriceLevel *priceLevel);

        Fifo():matchingAlgorithm(){};

};

//should return true if order1 has priority over order2
bool FifoPriorityCriteria(Order order1,Order order2) {
    return order1.timestamp < order2.timestamp;
};

void Fifo::sortPriorities(PriceLevel *priceLevel,std::function<bool(Order,Order)> priority_function){
        quickSort<Order>(priceLevel->orders,0,priceLevel->orders.size()-1,FifoPriorityCriteria);

};

//the matching algorithm should modify the order
MatchesList Fifo::match(Order *order, PriceLevel *priceLevel){
    //need to check if the other order can be traded with
    MatchesList matches;
    sortPriorities(priceLevel,FifoPriorityCriteria);
    int currentOrderIndex =0;
    while (order->quantity>0 &&  currentOrderIndex < priceLevel->orders.size()) {
        Order *currentOrder = priceLevel->orders[currentOrderIndex];

        if (currentOrder->type==OrderType::LIMIT ) {
            if ((currentOrder->side == OrderSide::SELL && order->price < currentOrder->price) || (currentOrder->side == OrderSide::BUY && currentOrder->price < order->price) ) {
                currentOrderIndex++;
                continue;
            }
        }

        if (currentOrder->quantity>order->quantity) {

            OrderMatch matchedOrder(currentOrder->id,order->quantity,currentOrder->price,OrderFillState::PARTIAL);
            matches.addMatch(matchedOrder);
            currentOrder->quantity-=order->quantity;
            order->quantity=0;
            return matches;
        }else {
            OrderMatch matchedOrder(currentOrder->id,currentOrder->quantity,currentOrder->price,OrderFillState::FULL);
            matches.addMatch(matchedOrder);
            order->quantity-=currentOrder->quantity;
            currentOrder->quantity=0;
            currentOrderIndex++;
        }

    }
    return matches;
};


bool PriceLevelSortCompare(PriceLevel a, PriceLevel b) {
    return a.price < b.price;
}
bool PriceLevelReverseSortCompare(PriceLevel a, PriceLevel b) {
        return b.price < a.price;
}
class OrderBook{
    private:
        std::vector<PriceLevel*> BuyOrders;
        std::vector<PriceLevel*> SellOrders;
        matchingAlgorithm *algorithm;

public:

    OrderBook(matchingAlgorithm *algo) {
        algorithm = algo;
        SellOrders = std::vector<PriceLevel*>(0);
        BuyOrders  = std::vector<PriceLevel*>(0);
    }

    std::pair<MatchesList,OrderFillState> addOrder(Order *entryOrder);
    //returns a pair with the list of matches and final state of the order matched
    std::pair<MatchesList,OrderFillState> match(Order *order);


    // implement a function for each case
    std::pair<MatchesList,OrderFillState> matchBuyLimit(Order *order);
    std::pair<MatchesList,OrderFillState> matchSellLimit(Order *order);
    std::pair<MatchesList,OrderFillState> matchBuyMarket(Order *order);
    std::pair<MatchesList,OrderFillState> matchSellMarket(Order *order);

    // To provide a list of orders for testing the matching functions independently from the addOrder Method
    void chargeTestOrders(std::vector<PriceLevel*> TestBuyOrders,std::vector<PriceLevel*> TestSellOrders) {
        SellOrders = TestSellOrders;
        BuyOrders = TestBuyOrders;
    };
};

std::pair<MatchesList,OrderFillState> OrderBook::match(Order *order) {
    //decide how to match according to the order type(Market,Limit)
    std::pair<MatchesList,OrderFillState> MatchState;
    if (order->side == OrderSide::BUY) {
        if(order->type == OrderType::MARKET){
            MatchState = matchBuyMarket(order);
        }
        if(order->type == OrderType::LIMIT){
            MatchState = matchBuyLimit(order);
        }
    }

    if (order->side == OrderSide::SELL) {
        if(order->type == OrderType::MARKET){
            MatchState = matchSellMarket(order);
        }
        if(order->type == OrderType::LIMIT){
            MatchState = matchSellLimit(order);
        }
    }
    return MatchState;
};


///sell limit at least for this amount

//buy limit at most this amount

std::pair<MatchesList,OrderFillState> OrderBook::matchBuyLimit(Order *order) {
    int currentPriceLevel = 0;
    MatchesList allMatches;
    quickSort<PriceLevel>(SellOrders, 0, SellOrders.size() - 1, PriceLevelSortCompare);
    //i should have done a <= operator for prices :'(
    int originalOrderQuantity = order->quantity;
    while (
        currentPriceLevel < SellOrders.size() && (
            (SellOrders[currentPriceLevel]->price < order->price) || (SellOrders[currentPriceLevel]->price == order->price))
        && order->quantity > 0) {
        MatchesList currentPriceLevelMatches = algorithm->match(order, SellOrders[currentPriceLevel]);
        allMatches.extend(currentPriceLevelMatches);


        //delete every filled order from the priceLevel
        for (auto match: currentPriceLevelMatches.matches) {
            if (match.matchingResult == OrderFillState::FULL)
                SellOrders[currentPriceLevel]->remove_order(match.orderId);
        }
        currentPriceLevel++;
    }

    //determine if the order was filled or not
    OrderFillState entryOrderState = order->getOrderFillState();
    return std::pair<MatchesList, OrderFillState>(allMatches, entryOrderState);
}
;

std::pair<MatchesList,OrderFillState> OrderBook::matchSellLimit(Order *order){
    int currentPriceLevel = 0;
    MatchesList allMatches;
    quickSort<PriceLevel>(BuyOrders, 0, BuyOrders.size() - 1, PriceLevelReverseSortCompare);
    //i should have done a <= operator for prices :'(
    int originalOrderQuantity = order->quantity;
    while (
        currentPriceLevel < BuyOrders.size() && ((order->price < BuyOrders[currentPriceLevel]->price) || (BuyOrders[currentPriceLevel]->price == order->price)) && order->quantity > 0) {
        MatchesList currentPriceLevelMatches = algorithm->match(order, BuyOrders[currentPriceLevel]);
        allMatches.extend(currentPriceLevelMatches);


        //delete every filled order from the priceLevel
        for (auto match: currentPriceLevelMatches.matches) {
            if (match.matchingResult == OrderFillState::FULL)
                BuyOrders[currentPriceLevel]->remove_order(match.orderId);
        }
        currentPriceLevel++;
        }

    //determine if the order was filled or not
    OrderFillState entryOrderState = order->getOrderFillState();
    return std::pair<MatchesList, OrderFillState>(allMatches, entryOrderState);

};

std::pair<MatchesList,OrderFillState> OrderBook::matchBuyMarket(Order *order){
    int currentPriceLevel = 0;
    MatchesList allMatches;
    quickSort<PriceLevel>(SellOrders, 0, SellOrders.size() - 1, PriceLevelSortCompare);
    //i should have done a <= operator for prices :'(
    int originalOrderQuantity = order->quantity;
    while ((currentPriceLevel < SellOrders.size()) && (order->quantity > 0)) {
        MatchesList currentPriceLevelMatches = algorithm->match(order, SellOrders[currentPriceLevel]);
        allMatches.extend(currentPriceLevelMatches);


        //delete every filled order from the priceLevel
        for (auto match: currentPriceLevelMatches.matches) {
            if (match.matchingResult == OrderFillState::FULL)
                SellOrders[currentPriceLevel]->remove_order(match.orderId);
        }
        currentPriceLevel++;
        }

    //determine if the order was filled or not
    OrderFillState entryOrderState = order->getOrderFillState();
    return std::pair<MatchesList, OrderFillState>(allMatches, entryOrderState);
};
std::pair<MatchesList,OrderFillState> OrderBook::matchSellMarket(Order *order){
    int currentPriceLevel = 0;
    MatchesList allMatches;
    quickSort<PriceLevel>(BuyOrders, 0, BuyOrders.size() - 1, PriceLevelReverseSortCompare);
    //i should have done a <= operator for prices :'(
    int originalOrderQuantity = order->quantity;
    while ((currentPriceLevel < BuyOrders.size()) && (order->quantity > 0)) {
        MatchesList currentPriceLevelMatches = algorithm->match(order, BuyOrders[currentPriceLevel]);
        allMatches.extend(currentPriceLevelMatches);


        //delete every filled order from the priceLevel
        for (auto match: currentPriceLevelMatches.matches) {
            if (match.matchingResult == OrderFillState::FULL)
                BuyOrders[currentPriceLevel]->remove_order(match.orderId);
        }
        currentPriceLevel++;
    }

    //determine if the order was filled or not
    OrderFillState entryOrderState = order->getOrderFillState();
    return std::pair<MatchesList, OrderFillState>(allMatches, entryOrderState);
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

#endif
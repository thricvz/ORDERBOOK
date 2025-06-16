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
        Price(int dollars_,int cents_);
        Price();

        bool operator==(const Price& __rhs) const;
        bool operator<(const Price& __rhs) const;
};

class Order{
    public:
        OrderType type;
        OrderSide side;
        Price price;
        int quantity;
        int id;
        int oriqinalQuantity;
        std::chrono::time_point<std::chrono::steady_clock> timestamp;
        Order(OrderType _type,OrderSide _side,int quantity_,int _dollars,int _cents);
        Order(OrderType _type,OrderSide _side,int quantity_);
        Order();

        void setId(int id_);
        OrderFillState getOrderFillState();
};


class PriceLevel{
    public:
        Price price;
        std::vector<Order*> orders;

        PriceLevel(Price price_);
        PriceLevel();

        void add_order(Order *order);
        void remove_order(int orderId);
};
struct OrderMatch{
    
    int orderId;
    int quantity;
    OrderFillState matchingResult;
    Price price;

    OrderMatch(int id,int quantity_,Price price_,OrderFillState result);
    bool operator==(const OrderMatch &lhs) const;
};

class MatchesList{
    public:
    std::vector<OrderMatch> matches;

    void addMatch(OrderMatch order);

    bool operator==(const MatchesList & rhs) const;

    void extend(const MatchesList &entryList);
    bool noMatchesMade();
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

#endif
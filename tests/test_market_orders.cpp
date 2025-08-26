#include <gtest/gtest.h>
#include "orderbook.hpp"
#define DEFAULT_OWNERID 0

//this file must be updated

#define MATCHES_LIST 0
#define ORDER_STATE 1

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

TEST(BUY_MARKET_ORDER, NO_MATCHES) {
    
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



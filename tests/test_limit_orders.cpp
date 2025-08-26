#include <gtest/gtest.h>
#include "orderbook.hpp"
#define DEFAULT_OWNERID 0


#define MATCHES_LIST 0
#define ORDER_STATE 1
//NO MAGIC NUMBERS IN THE TEST (ESPECIALLY IN THE ID FIELDS)

static bool equalMatchesList(MatchesList input,MatchesList expected) {
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

TEST(ORDERBOOK,MATCH_BUY_LIMIT_ORDER) {
}

TEST(ORDERBOOK,MATCH_BUY_LIMIT_ORDER_NO_MATCH) {

}

TEST(ORDERBOOK, MATCH_BUY_LIMIT_ORDER_AGAINST_EXCEEDING_ORDERS) {

}


TEST(ORDERBOOK,MATCH_SELL_LIMIT_ORDER) {

}

TEST(ORDERBOOK,MATCH_SELL_LIMIT_ORDER_NO_MATCH) {

}


TEST(ORDERBOOK, MATCH_SELL_LIMIT_ORDER_AGAINST_EXCEEDING_ORDERS) {

}





#include "orderbook.hpp"
#include <vector>
#define DEFAULT_OWNERID 0

#define MATCHED_ORDERS_LIST 0
#define ORDER_STATE 1

bool equalVectorPtr(std::vector<int*> input,std::vector<int*> expected);

bool equalMatchesList(MatchesList input,MatchesList expected);

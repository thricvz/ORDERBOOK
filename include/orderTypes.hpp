#pragma once
enum OrderType:uint8_t{
    MARKET,
    LIMIT
};
enum OrderSide:uint8_t{
    BUY,
    SELL
};
enum OrderFillState:uint8_t{
    PARTIAL,FULL,NOFILL
};

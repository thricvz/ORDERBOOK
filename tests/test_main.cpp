#include <gtest/gtest.h>
#include "orderbook.hpp"
#include "AdditionalTestFunctions.hpp"


#define DEFAULT_OWNERID 0


TEST(ORDER,CREATE_OBJECT){
  Order order1(OrderType::LIMIT,OrderSide::BUY,96,Price{30,12},2);
  ASSERT_EQ(order1.type ,OrderType::LIMIT);
  ASSERT_EQ(order1.side ,OrderSide::BUY);
  ASSERT_EQ(order1.price.cents ,12);
  ASSERT_EQ(order1.price.dollars ,30);
  ASSERT_EQ(order1.id ,2);
	
}


TEST(PRICE_LEVEL,CREATE_OBJECT){
  Price price{30,68};
  PriceLevel level(price);
  ASSERT_EQ(level.price.cents ,68);
  ASSERT_EQ(level.price.dollars ,30);
  ASSERT_EQ(level.orders.size(),0);
}

TEST(PRICE_LEVEL,ADD_ELEMENTS){
  Price price{30,68};
  PriceLevel level(price);
  Order* order1= new Order(OrderType::LIMIT,OrderSide::BUY,26,{30,12},1);

  level.add_order(order1);
  ASSERT_EQ(level.orders.size(),1);

  ASSERT_EQ(level.orders[0]->type ,OrderType::LIMIT);
  ASSERT_EQ(level.orders[0]->side ,OrderSide::BUY);
  ASSERT_EQ(level.orders[0]->price.cents ,12);
  ASSERT_EQ(level.orders[0]->price.dollars ,30);
}


TEST(PRICE_LEVEL,REMOVE_ELEMENTS){
  Price price{30,68};
  PriceLevel level(price);
  Order *order1=new Order(OrderType::LIMIT,OrderSide::BUY,20,{30,12},1);
  Order *order2=new Order(OrderType::LIMIT,OrderSide::BUY,25,{30,12},2);



  order1->setId(1);
  level.add_order(order1);

  order2->setId(2);
  //test if the order is intact
  level.add_order(order2);
  level.remove_order(2);

  ASSERT_EQ(level.orders.size(),1);

  ASSERT_EQ(level.orders[0]->type ,order1->type);
  ASSERT_EQ(level.orders[0]->side ,order1->side);
  ASSERT_EQ(level.orders[0]->price.cents ,order1->price.cents);
  ASSERT_EQ(level.orders[0]->price.dollars ,order1->price.dollars);

}

bool smaller(int a ,int b) {
  return a<b;
};

TEST(QUICKSORT,SORTING){

  int int_1 = 1;
  int int_89 = 1;
  int int_9 = 1;
  int int_67 = 1;
  int int_899 = 899;
  std::vector<int*> TestVector = {&int_1,&int_89,&int_9,&int_67};
  quickSort<int>(TestVector,0,TestVector.size()-1,smaller);
  EXPECT_TRUE(equalVectorPtr(TestVector,std::vector<int*>({&int_1,&int_9,&int_67,&int_89})));

  std::vector<int*> TestVector2 = {&int_1,&int_89,&int_899,&int_9,&int_67};
  quickSort<int>(TestVector2,0,TestVector2.size()-1,smaller);
  EXPECT_TRUE(equalVectorPtr(TestVector2,std::vector<int*>({&int_1,&int_9,&int_67,&int_89,&int_899})));
}

bool bigger(int a ,int b) {
    return a >b;
}
TEST(QUICKSORT,REVERSE_SORT) {

  int int_1 = 1;
  int int_89 = 1;
  int int_9 = 1;
  int int_67 = 1;
  std::vector<int*> TestVector = {&int_1,&int_89,&int_9,&int_67};
  quickSort<int>(TestVector,0,TestVector.size()-1,bigger);
  EXPECT_TRUE(equalVectorPtr(TestVector,std::vector<int*>({&int_89,&int_67,&int_9,&int_1})));


}

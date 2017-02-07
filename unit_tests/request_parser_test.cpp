#include "gtest/gtest.h"
#include "../request_parser.hpp"
#include "../request.hpp"
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
// test fixture used in following test
class Request_parser_Test:public::testing::Test{
protected:
  http::server::request* rqst_ptr;
  http::server::request_parser* rqst_parser_ptr;
  boost::array<char, 5> buffer= {{'G', 'E', 'T', '\r', '\n'}};

  boost::array<char, 15> input = {{'G', 'E', 'T', ' ', ' ', 'H', 'T','T', 'P', '/', '1', '.', '1', '\r', '\n'}};
  virtual void SetUp(){
    rqst_ptr = new http::server::request();
    rqst_parser_ptr = new http::server::request_parser();
  }
  virtual void TearDown(){
    delete rqst_ptr;
    delete rqst_parser_ptr;
  }
};

// test reset
TEST_F(Request_parser_Test, reset_Test){
  rqst_parser_ptr->reset();
  EXPECT_TRUE(rqst_parser_ptr->isStart());
}


//
TEST_F(Request_parser_Test, consume_Test){
  rqst_parser_ptr->reset();
  EXPECT_FALSE(rqst_parser_ptr->consume(*rqst_ptr, '\n'));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, 'G')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, ' ')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, ' ')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, 'H')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, 'T')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, 'T')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, 'P')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, '/')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, '1')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, '.')));
  EXPECT_TRUE(boost::indeterminate(rqst_parser_ptr->consume(*rqst_ptr, '1')));
  EXPECT_FALSE(rqst_parser_ptr->consume(*rqst_ptr, 20));

}

// test parse func
TEST_F(Request_parser_Test, simple_parse_Test) {
  //rqst_parser_ptr->reset();
  //EXPECT_EQ(boost::make_tuple(false, buffer.begin()), rqst_parser_ptr->parse(*rqst_ptr, buffer.begin(), buffer.end()));

  EXPECT_FALSE(rqst_parser_ptr->parse(*rqst_ptr, buffer.begin(), buffer.end()).get<0>());
  EXPECT_EQ("GET  HTTP/0.0\r\n\r\n", rqst_ptr->toString());
}


// test is_ctl
/*TEST_F(Request_parser_Test, input_parser_Test){
  EXPECT_FALSE(rqst_parser_ptr->parse(*rqst_ptr, buffer.begin(), buffer.end()).get<0>());
}


// test is_tspecial
TEST_F(Request_parser_Test, tspecial_Test){

  EXPECT_TRUE(rqst_parser_test.is_ctl('\t'));
  EXPECT_TRUE(rqst_parser_test.is_ctl('&'));
  EXPECT_FALSE(rqst_parser_test.is_ctl(14));
}

// test is_digit
TEST_F(Request_parser_Test, ctl_Test){

  EXPECT_TRUE(rqst_parser_test.is_ctl('5'));
  EXPECT_TRUE(rqst_parser_test.is_ctl('6'));
  EXPECT_FALSE(rqst_parser_test.is_ctl('c'));
}
*/

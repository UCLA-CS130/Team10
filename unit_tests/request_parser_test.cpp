#include "gtest/gtest.h"
#include "../request_parser.hpp"

// test fixture used in following test
class Request_parser_Test:public::testing::Test{
protected:
  http::server::request_parser rqst_parser_test;
};

/*
// test is_char
TEST_F(Request_parser_Test, char_Test) {
  EXPECT_FALSE(rqst_parser_test.is_char(-12));
  EXPECT_TRUE(rqst_parser_test.is_char(1));
}


// test is_ctl
TEST_F(Request_parser_Test, ctl_Test){

  EXPECT_TRUE(rqst_parser_test.is_ctl(0));
  EXPECT_TRUE(rqst_parser_test.is_ctl(127));
  EXPECT_FALSE(rqst_parser_test.is_ctl(128));
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

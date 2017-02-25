#include "gtest/gtest.h"
#include "../response.hpp"
#include <string>
#include <boost/lexical_cast.hpp>

// test fixture used in following test
class ResponseTest:public::testing::Test{
protected:
  Response* response;
  void SetUp()
  {
  	response = new Response;
  	response->SetStatus(Response::ok);
    response->AddHeader("abc", "def");
    response->SetBody("123456789");

  }
  void TearDown(){
    delete response;
  }
};

// test content_length
TEST_F(ResponseTest, content_length_Test){
  EXPECT_EQ(9, response->ContentLength());
}

// test Get_response_code
TEST_F(ResponseTest, get_response_code_Test){
  EXPECT_EQ("200", response->GetResponseCode());
}

// test to_string
TEST_F(ResponseTest, tostring_Test){
  std::string result = "HTTP/1.0 200 OK\r\nabc: def\r\n\r\n123456789";

  EXPECT_EQ(result, response->ToString());
}
#include "gtest/gtest.h"
//#include "gmock/gmock.h"
#include "../echo_handler.hpp"
#include <string>
// test fixture used in following test
class EchoHandler_Test:public::testing::Test{
protected:
  EchoHandler handler;
  Response response;
  NginxConfig config;
  std::string uri_prefix = "/echo";
  std::string request_url  = "GET /echo HTTP/1.0\r\ntest\r\n\r\n";
  std::unique_ptr<Request> request = Request::Parse(request_url);
};

// test Init
TEST_F(EchoHandler_Test, Init_Test){
  EXPECT_EQ(RequestHandler::OK, handler.Init(uri_prefix, config));
}


// test HandleRequest
TEST_F(EchoHandler_Test, HandleRequest_Test){
  EXPECT_EQ(RequestHandler::OK, handler.HandleRequest(*request, &response));
  
  std::string result = "HTTP/1.0 200 OK\r\nContent-Length: " + std::to_string(request_url.size()) + "\r\nContent-Type: text/plain\r\n\r\n" 
  + request_url;
  
  EXPECT_EQ(result, response.ToString());
}

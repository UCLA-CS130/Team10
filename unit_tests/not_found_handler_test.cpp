#include "gtest/gtest.h"
#include "../not_found_handler.hpp"

// test fixture used in following test
class NotFoundHandler_Test:public::testing::Test{
protected:
  NotFoundHandler handler;
  Response response;
  NginxConfig config;
  std::string uri_prefix = "";
  std::string request_url  = "GET /echo HTTP/1.1\r\n\r\n";
  std::unique_ptr<Request> request = Request::Parse(request_url);
};

// test Init
TEST_F(NotFoundHandler_Test, Init_Test){
  EXPECT_EQ(RequestHandler::OK, handler.Init(uri_prefix, config));
}


// test HandleRequest
TEST_F(NotFoundHandler_Test, HandleRequest_Test){
  EXPECT_EQ(RequestHandler::OK, handler.HandleRequest(*request, &response));
  
  std::string result = "HTTP/1.0 404 Not Found\r\nContent-Type: text/plain\r\n\r\nHTTP/1.0 404 Not Found\r\n";
  
  EXPECT_EQ(result, response.ToString());
}

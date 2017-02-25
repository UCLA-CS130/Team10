#include "gtest/gtest.h"
#include "../static_handler.hpp"
#include "../config_parser.hpp"



// test fixture used in following test
class StaticHandler_Test:public::testing::Test{
protected:
  StaticHandler handler;
  Response response;
  NginxConfig config_;
  std::shared_ptr<NginxConfig> config = std::make_shared<NginxConfig>();
  std::shared_ptr<NginxConfigStatement> statement = std::make_shared<NginxConfigStatement>();

  std::string uri_prefix = "/static";
  std::string request_url  = "GET /static/index.html HTTP/1.1\r\ntest\r\n\r\n";
  std::unique_ptr<Request> request = Request::Parse(request_url);

  std::string in1 = "http://github.com";
  std::string in2  = "%";
  std::string in3 = "%dfadsfasf%";
  std::string in4  = "http://sdc+.com";
  std::string out = "";
  std::string root = "root";
  std::string dir = "./demo/static";
};


// test Init
TEST_F(StaticHandler_Test, Init_Test){
  EXPECT_EQ(RequestHandler::INVALID, handler.Init(uri_prefix, config_));
}


// test HandleRequest
TEST_F(StaticHandler_Test, HandleRequest_Test){
  statement->tokens_.push_back(root);
  statement->tokens_.push_back(dir);
  config->statements_.push_back(statement);
  EXPECT_EQ(RequestHandler::OK, handler.Init(uri_prefix, *config));
  EXPECT_EQ(RequestHandler::OK, handler.HandleRequest(*request, &response));
  std::string result = "HTTP/1.0 200 OK\r\nContent-Length: 107\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<body>\n\n<h1>CS130 is the best class ever.</h1>\n\n<p>I <3 Google</p>\n\n</body>\n</html>\n";
  EXPECT_EQ(result, response.ToString());
}

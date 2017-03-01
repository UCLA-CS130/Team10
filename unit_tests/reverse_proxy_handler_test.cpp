#include "gtest/gtest.h"
#include "../reverse_proxy_handler.hpp"
#include "../config_parser.hpp"



// test fixture used in following test
class ReverseProxyHandler_Test:public::testing::Test{
protected:
  ReverseProxyHandler handler;
  Response response;
  NginxConfig config_;
  std::shared_ptr<NginxConfig> config = std::make_shared<NginxConfig>();
  std::shared_ptr<NginxConfigStatement> statement = std::make_shared<NginxConfigStatement>();
  std::shared_ptr<NginxConfigStatement> statement2 = std::make_shared<NginxConfigStatement>();

  std::string uri_prefix = "/proxy";
  std::string request_url  = "GET /proxy/static/index.html HTTP/1.1\r\ntest\r\n\r\n";
  std::unique_ptr<Request> request = Request::Parse(request_url);

  std::string rh = "remote_host";
  std::string host = "localhost";
  std::string rp = "remote_port";
  std::string port = "3001";
};


// test Init
TEST_F(ReverseProxyHandler_Test, Init_Test){
  EXPECT_EQ(RequestHandler::INVALID, handler.Init(uri_prefix, config_));
}


// test HandleRequest
TEST_F(ReverseProxyHandler_Test, HandleRequest_Test){
  statement->tokens_.push_back(rh);
  statement->tokens_.push_back(host);
  config->statements_.push_back(statement);
  statement2->tokens_.push_back(rp);
  statement2->tokens_.push_back(port);
  config->statements_.push_back(statement2);
  EXPECT_EQ(RequestHandler::OK, handler.Init(uri_prefix, *config));
  
  //TODO: Make mock webserver?
  EXPECT_EQ(RequestHandler::OK, handler.HandleRequest(*request, &response));
  std::string result = "HTTP/1.0 200 OK\r\nContent-Length: 107\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html>\n<body>\n\n<h1>CS130 is the best class ever.</h1>\n\n<p>I <3 Google</p>\n\n</body>\n</html>\n";
  EXPECT_EQ(result, response.ToString());
}

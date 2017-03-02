#include "gtest/gtest.h"
#include "../reverse_proxy_handler.hpp"
#include "../config_parser.hpp"

// test fixture used in following test
class ReverseProxyHandler_Test: public::testing::Test {
protected:
  ReverseProxyHandler handler;
  Response response;
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

class SendProxyRequest_Test: public::testing::Test {
protected:
  ReverseProxyHandler handler;
  Response response;
  std::shared_ptr<NginxConfig> config = std::make_shared<NginxConfig>();
  std::shared_ptr<NginxConfigStatement> statement = std::make_shared<NginxConfigStatement>();
  std::string uri_prefix = "/proxy";
  std::string rh = "remote_host";
  std::string host = "www.ucla.edu";

};


TEST_F(SendProxyRequest_Test, Valid){
  statement->tokens_.push_back(rh);
  statement->tokens_.push_back(host);
  config->statements_.push_back(statement);
  handler.Init(uri_prefix, *config);
  std::string request_string = "GET / HTTP/1.0\r\n\r\n";
  RequestHandler::Status status = handler.SendProxyRequest(request_string, host, &response);
  EXPECT_EQ(RequestHandler::OK, status);
}

//give invalid host
TEST_F(SendProxyRequest_Test, InvalidHost){
  std::string invalid_host = "wwwwww";  
  statement->tokens_.push_back(rh);
  statement->tokens_.push_back(invalid_host);
  config->statements_.push_back(statement);
  handler.Init(uri_prefix, *config);
  std::string request_string = "GET / HTTP/1.0\r\n\r\n";
  RequestHandler::Status status = handler.SendProxyRequest(request_string, invalid_host, &response);

  EXPECT_EQ(RequestHandler::INVALID, status);
}

TEST_F(SendProxyRequest_Test, Redirect){
  statement->tokens_.push_back(rh);
  statement->tokens_.push_back(host);
  config->statements_.push_back(statement);
  handler.Init(uri_prefix, *config);
  std::string request_string = "GET /about/chancellor HTTP/1.0\r\n\r\n";
  RequestHandler::Status status = handler.SendProxyRequest(request_string, host, &response);
  EXPECT_EQ(RequestHandler::OK, status);
}

// Test Failing Init (empty)
TEST_F(ReverseProxyHandler_Test, Init_Fail){
  EXPECT_EQ(RequestHandler::INVALID, handler.Init(uri_prefix, *config));
}


// test valid Init config
TEST_F(ReverseProxyHandler_Test, Init_Pass){
  statement->tokens_.push_back(rh);
  statement->tokens_.push_back(host);
  config->statements_.push_back(statement);
  statement2->tokens_.push_back(rp);
  statement2->tokens_.push_back(port);
  config->statements_.push_back(statement2);
  EXPECT_EQ(RequestHandler::OK, handler.Init(uri_prefix, *config));
}

// test ProcessHeaderLine
TEST_F(ReverseProxyHandler_Test, ProcessHeaderLine){
  std::string header = "Content-Length: 10\r";
  std::pair<std::string, std::string> parsed_header = handler.ProcessHeaderLine(header);
  EXPECT_EQ("Content-Length", parsed_header.first);
  EXPECT_EQ("10", parsed_header.second);
}

// test ParseRedirect
TEST_F(ReverseProxyHandler_Test, ParseRedirect){
  std::string redirect_URI = "";
  std::string redirect_host = "";
  std::string response_string = "HTTP/1.1 302 Moved Temporarily\r\nLocation: http://www.ucla.edu/\r\n\r\n";
  std::istringstream response_stream(response_string);
  ASSERT_TRUE(handler.ParseRedirect(response_stream, redirect_URI, redirect_host));
  EXPECT_EQ("www.ucla.edu", redirect_host);
  EXPECT_EQ("/", redirect_URI);
}

// test Valid ParseRedirect
TEST_F(ReverseProxyHandler_Test, ValidParseRedirect){
  std::string redirect_URI = "";
  std::string redirect_host = "";
  std::string response_string = "HTTP/1.1 302 Moved Temporarily\r\nLocation: http://www.ucla.edu/\r\n\r\n";
  std::istringstream response_stream(response_string);
  ASSERT_TRUE(handler.ParseRedirect(response_stream, redirect_URI, redirect_host));
  EXPECT_EQ("www.ucla.edu", redirect_host);
  EXPECT_EQ("/", redirect_URI);
}

// test Invalid ParseRedirect (No Location)
TEST_F(ReverseProxyHandler_Test, NoLocation){
  std::string redirect_URI = "";
  std::string redirect_host = "";
  std::string response_string = "HTTP/1.1 302 Moved Temporarily\r\n\r\n";
  std::istringstream response_stream(response_string);
  ASSERT_FALSE(handler.ParseRedirect(response_stream, redirect_URI, redirect_host));
}

// test Invalid ParseRedirect (Invalid Location URL)
TEST_F(ReverseProxyHandler_Test, InvalidRedirectURL){
  std::string redirect_URI = "";
  std::string redirect_host = "";
  std::string response_string = "HTTP/1.1 302 Moved Temporarily\r\nLocation: http://\r\n\r\n";
  std::istringstream response_stream(response_string);
  ASSERT_FALSE(handler.ParseRedirect(response_stream, redirect_URI, redirect_host));
}

// test ParseBody
TEST_F(ReverseProxyHandler_Test, ParseBody){
  statement->tokens_.push_back(rh);
  statement->tokens_.push_back(host);
  config->statements_.push_back(statement);
  statement2->tokens_.push_back(rp);
  statement2->tokens_.push_back(port);
  config->statements_.push_back(statement2);
  ASSERT_EQ(RequestHandler::OK, handler.Init(uri_prefix, *config));

  boost::asio::streambuf response_buf;
  std::ostream response_stream(&response_buf);
  response_stream << "<a href=\"/about\">About</a><img src=\"/img/test.jpg\">";

  std::string expected_body = "<a href=\"/proxy/about\">About</a><img src=\"/proxy/img/test.jpg\">";
  EXPECT_EQ(expected_body, handler.ParseBody(&response_buf));
}


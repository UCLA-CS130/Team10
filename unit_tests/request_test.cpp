#include <string>
#include "../request.hpp"
#include "gtest/gtest.h"

TEST(RequestTest, ToStringTest){
  std::string raw_request = "GET /echo HTTP/1.1\r\ntest\r\n\r\n";
  auto request = Request::Parse(raw_request);
  EXPECT_EQ("GET",request->method());
  EXPECT_EQ("HTTP/1.1", request->version());
  EXPECT_EQ("/echo", request->uri());
}
#include <string>
#include "../request.hpp"
#include "gtest/gtest.h"

TEST(RequestTest, ToStringTest){

  http::server::request rqst;
  rqst.method = "GET";
  rqst.http_version_major = 1;
  rqst.http_version_minor = 1;
  http::server::header h1;
  h1.name = "Host";
  h1.value = "localhost:3000";
  rqst.headers.push_back(h1);
  EXPECT_EQ("GET  HTTP/1.1\r\nHost: localhost:3000\r\n\r\n", rqst.toString());
}

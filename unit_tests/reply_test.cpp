#include "gtest/gtest.h"
#include "../response.hpp"
#include <string>
#include <boost/lexical_cast.hpp>

// // test fixture used in following test
// class ResponseTest:public::testing::Test{
// protected:
//   response rply;
//   response out;
//   header header;
// };
//
// // test stock_reply
// TEST_F(ResponseTest, stock_reply_Test){
//   out = rply.stock_reply(http::server::reply::ok);
//   EXPECT_TRUE(out.status == 200);
//   EXPECT_EQ(200, out.status);
//   EXPECT_EQ("", out.content);
//   EXPECT_EQ("Content-Length", out.headers[0].name);
//   EXPECT_EQ("Content-Type", out.headers[1].name);
//   EXPECT_EQ("text/html", out.headers[1].value);
// }

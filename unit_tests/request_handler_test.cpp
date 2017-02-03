#include "gtest/gtest.h"
//#include "gmock/gmock.h"
#include "../request_handler.hpp"
#include "../request.hpp"
#include "../reply.hpp"
#include "../mime_types.hpp"
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>

// test fixture used in following test
class Request_handler_Test:public::testing::Test{
protected:
  http::server::request_handler* rqst_hndlr_ptr;
  http::server::reply rply;
  http::server::request rqst;

  std::string in1 = "http://github.com";
  std::string in2  = "%";
  std::string in3 = "%dfadsfasf%";
  std::string in4  = "http://sdc+.com";
  std::string out = "";


  virtual void SetUp(){
    rqst_hndlr_ptr = new http::server::request_handler("~");
  }
  virtual void TearDown(){
    delete rqst_hndlr_ptr;
  }
};

// test url_decode
TEST_F(Request_handler_Test, decode_Test){
  EXPECT_TRUE(rqst_hndlr_ptr->url_decode(in1, out));
  EXPECT_EQ(in1, out);
  EXPECT_FALSE(rqst_hndlr_ptr->url_decode(in2, out));
  EXPECT_FALSE(rqst_hndlr_ptr->url_decode(in3, out));
  out = "";
  EXPECT_TRUE(rqst_hndlr_ptr->url_decode(in4, out));
  EXPECT_EQ("http://sdc .com", out);
}


// test handle_request

TEST_F(Request_handler_Test, handle_request_Test){
  rqst.uri = "..";
  rqst_hndlr_ptr->handle_request(rqst, rply);

  EXPECT_EQ(400, rply.status);
  rqst.uri = in3;
  rqst.uri="/foo.txt";
  rqst_hndlr_ptr->handle_request(rqst, rply);
  EXPECT_EQ(404, rply.status);
}




#include "gtest/gtest.h"
#include "../connection.hpp"

// test fixture used in following test
class Connection_Test:public::testing::Test{
protected:

  boost::asio::io_service io_service_test;
  boost::asio::ip::tcp::socket* skt_ptr; 
  http::server::connection_manager* conn_mngr_ptr;
  http::server::request_handler* rqst_hndlr_ptr;
  http::server::connection* conn_ptr;

  virtual void SetUp(){
    skt_ptr = new boost::asio::ip::tcp::socket(io_service_test);
    //conn_mngr_ptr = new http::server::connection_manager();
    rqst_hndlr_ptr = new http::server::request_handler("");
    conn_ptr = new http::server::connection(io_service_test, *conn_mngr_ptr, *rqst_hndlr_ptr);
  }

  virtual void TearDown(){
    delete skt_ptr;
    //delete conn_mngr_ptr;
    delete rqst_hndlr_ptr;
    delete conn_ptr;
  }
};


// test socket accessor
TEST_F(Connection_Test, Socket_Test) {
  EXPECT_FALSE(conn_ptr->socket().is_open());
}

TEST_F(Connection_Test, is_Socket_open){
  conn_ptr->start();
  EXPECT_FALSE(conn_ptr->socket().is_open());
  conn_ptr->stop();
  EXPECT_FALSE(conn_ptr->socket().is_open());
}

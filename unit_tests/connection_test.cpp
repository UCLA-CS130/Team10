#include "gtest/gtest.h"
#include "../connection.hpp"
#include "../request_handler.hpp"

//test fixture used in following test
class Connection_Test:public::testing::Test{
protected:

  boost::asio::io_service io_service_test;
  boost::asio::ip::tcp::socket* skt_ptr;
  connection_manager* conn_mngr_ptr;
  connection* conn_ptr;

  virtual void SetUp(){
    skt_ptr = new boost::asio::ip::tcp::socket(io_service_test);
    conn_ptr = new connection(io_service_test, *conn_mngr_ptr);
  }

  virtual void TearDown(){
    delete skt_ptr;
    delete conn_ptr;
  }
};


// test socket accessor
TEST_F(Connection_Test, Socket_Test) {
  EXPECT_FALSE(conn_ptr->socket().is_open());
}
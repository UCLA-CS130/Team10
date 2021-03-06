#include "gtest/gtest.h"
#include "../connection_manager.hpp"
#include <algorithm>
#include <boost/bind.hpp>
#include "../request_handler.hpp"


// test fixture used in following test
class Connection_manager_Test:public::testing::Test{
protected:

  boost::asio::io_service io_service_test;
  boost::asio::ip::tcp::socket* skt_ptr;
  connection_manager* conn_mngr_ptr;
  connection_ptr conn_ptr;

  virtual void SetUp(){
    skt_ptr = new boost::asio::ip::tcp::socket(io_service_test);
    conn_mngr_ptr = new connection_manager();
    conn_ptr.reset(new connection(io_service_test, *conn_mngr_ptr));//, *rqst_hndlr_ptr));
  }

  virtual void TearDown(){
    delete skt_ptr;
    delete conn_mngr_ptr;
  }
};


// test start and stop
TEST_F(Connection_manager_Test, Socket_Test) {
  conn_mngr_ptr->start(conn_ptr);
  EXPECT_FALSE(false);
  conn_mngr_ptr->stop(conn_ptr);
  EXPECT_FALSE(conn_ptr->socket().is_open());
}

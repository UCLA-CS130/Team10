#include "../server.hpp"
#include <gmock/gmock.h>
#include <signal.h>
#include <boost/bind.hpp>
using ::testing::AtLeast;

class MockServer :  public http::server::server{
public:
  explicit MockServer() : http::server::server("0.0.0.0", "3000", ""){}
  MOCK_METHOD0(run, void());
  MOCK_METHOD0(start_accept, void());
  MOCK_METHOD0(handle_stop, void());
};

//test server function call times
TEST(ServerTest, runTest){

  MockServer server_mock;
  EXPECT_CALL(server_mock, start_accept()).Times(AtLeast(0));
  EXPECT_CALL(server_mock, run()).Times(AtLeast(1));
  EXPECT_CALL(server_mock, handle_stop()).Times(AtLeast(0));
  server_mock.run();
  //delete server_ptr;
}

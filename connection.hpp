//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "response.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "Server_config.hpp"
#include "log.hpp"
//#include "request_parser.hpp"


class connection_manager;

/// Represents a single connection from a client.
class connection
  : public boost::enable_shared_from_this<connection>,
    private boost::noncopyable
{
public:
  /// Construct a connection with the given io_service.
  explicit connection(boost::asio::io_service& io_service,
      connection_manager& manager);//, HandlerMap handler_map);

  /// Get the socket associated with the connection.
  boost::asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the connection.
  void start();

  /// Stop all asynchronous operations associated with the connection.
  void stop();

private:
  // find if request_url has the pattern which is key to uri-handler map
  // if so, return key
  // otherwise, return ""
  std::string find_key(std::string request_url) const;

  // Convert buffer to string
  std::string buffer_to_string();

  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e,
      std::size_t bytes_transferred);

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e);

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  /// The manager for this connection.
  connection_manager& connection_manager_;

  /// The handler used to process the incoming request.
  //RequestHandler& request_handler_;
  //HandlerMap handler_map_;

  /// Buffer for incoming data.
  //boost::array<char, 8192> buffer_;
  boost::asio::streambuf buffer;

  /// The incoming request.
  //Request request_;

  /// The parser for the incoming request.
  //request_parser request_parser_;

  /// The reply to be sent back to the client.
  //Response reply_;
};

typedef boost::shared_ptr<connection> connection_ptr;


#endif // HTTP_CONNECTION_HPP

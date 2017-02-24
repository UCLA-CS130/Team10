//
// connection.cpp
// ~~~~~~~~~~~~~~
//
#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include <iostream>


connection::connection(boost::asio::io_service& io_service,
    connection_manager& manager)
  : socket_(io_service),
    connection_manager_(manager)
{
}

boost::asio::ip::tcp::socket& connection::socket()
{
  return socket_;
}

void connection::start()
{
  std::cout << "Start to async read...\n";
  boost::asio::async_read_until(socket_, buffer, "\r\n\r\n",
      boost::bind(&connection::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void connection::stop()
{
  socket_.close();
}

void connection::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  if (!e)
  {
    std::string raw_request = buffer_to_string();
    auto request = Request::Parse(raw_request);
    std::string key_to_use = find_key(request->uri());
    std::shared_ptr<RequestHandler> handler_ptr = Log::instance()->get_map()[key_to_use].second;//handler_map_[key_to_use].second;
    if(handler_ptr == NULL)
      handler_ptr = Log::instance()->get_map()[""].second;
    Response response;

    // if StaticHandler cannot handle the request
    // call NotFoundHanlder
    if(handler_ptr->HandleRequest(*request, &response) == RequestHandler::INVALID){
      std::cerr << "Cannot handle request...\n";
      handler_ptr = Log::instance()->get_map()[""].second;
      handler_ptr->HandleRequest(*request, &response);
    }

    Log::instance()->add_record(request->uri(), response.GetResponseCode());
    // Write the reponse back to the socket.
    boost::asio::streambuf out_streambuf;
    std::ostream out(&out_streambuf);
    out << response.ToString();
    std::cout << "Start to asycn write...\n";
    boost::asio::async_write(socket_, out_streambuf,
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));

    /*boost::tribool result;
    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);

    if (result)
    {
      request_handler_.handle_request(request_, reply_);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else if (!result)
    {
      reply_ = reply::stock_reply(reply::bad_request);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
      socket_.async_read_some(boost::asio::buffer(buffer_),
          boost::bind(&connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }*/
  }
  else if (e != boost::asio::error::operation_aborted)
  {
    connection_manager_.stop(shared_from_this());
  }
}

void connection::handle_write(const boost::system::error_code& e)
{
  if (!e)
  {
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }

  if (e != boost::asio::error::operation_aborted)
  {
    connection_manager_.stop(shared_from_this());
  }
}

std::string connection::buffer_to_string()
{
  std::string s{
    buffers_begin(buffer.data()),
    buffers_end(buffer.data())
  };
  return s;
}

std::string connection::find_key(std::string request_url) const
{
  for(auto const &pair : Log::instance()->get_map()){
    std::string key = pair.first;
    if(key != "" && request_url.size() >= key.size() && request_url.substr(0, key.size()) == key)
      return key;
  }
  return "";
}
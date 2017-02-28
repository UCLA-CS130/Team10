#include "reverse_proxy.hpp"

RequestHandler::Status ReverseProxy::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
  m_uri_prefix; = uri_prefix;
  m_remote_host = "";
  m_remote_port = "";

  for ( auto statement : config.statements_ )
  {
    for (auto token: statement->tokens_)
    {
      if (token == "remote_host" && statement->tokens_.size() == 2)
      {
        m_remote_host = statement->tokens_[1];
      }
      else if (token == "remote_port" && statement->tokens_.size() == 2)
      {
        m_remote_port = statement->tokens_[1];
      }
    }
  }

  // Need both the host and the port config values
  if (remote_host.empty() || remote_port.empty())
  {
    return RequestHandler::INVALID;
  }

  return RequestHandler::OK;
}

RequestHandler::Status ReverseProxy::HandleRequest(const Request& request,
                               Response* response)
{
  // Get the request URI (e.g. /proxy/static/file.txt)
  std::string new_uri = request.uri();

  // Generate new URI (/static/file.txt)
  boost::replace_all(new_uri, m_uri_prefix, "");
  // Generate new request
  //    GET new_URI HTTP/1.0\r\n\r\n
  std::string new_request = "GET " + new_uri + "HTTP/1.0\r\n\r\n";

  return SendProxyRequest(new_request, response);
}


RequestHandler::Status ReverseProxy::SendProxyRequest(const std::string& request_string, Response* response,
                                                      const int& attempt_num = 0, const std::string& new_host = m_remote_host) {
  // Create connection to m_remote_host:m_remote_port
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query(m_remote_host, m_remote_port);
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  boost::asio::ip::tcp::resolver::iterator end;

  boost::asio::ip::tcp::socket socket(io_service);
  boost::system::error_code error = boost::asio::error::host_not_found;
  // There might be multiple endpoints. Connect to the first one that works.
  while (error && endpoint_iterator != end)
  {
    socket.close();
    socket.connect(*endpoint_iterator++, error);
  }

  // Send request to m_remote_host:m_remote_port
  boost::asio::write(socket, boost::asio::buffer(request_string));

  // Parse the response
  boost::asio::streambuf response_buf;
  // Read response status line
  boost::asio::read_until(socket, response_buf, "\r\n\r\n");

  // Check that response is OK. 
  // Adapted from http://www.boost.org/doc/libs/1_57_0/doc/html/boost_asio/example/cpp03/http/client/sync_client.cpp
  std::istream response_stream(&response_buf);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
  {
    std::cerr << "Invalid response\n";
    return RequestHandler::INVALID;
  }
  if (status_code == Response::moved_temporarily)
  {
    // Check attempt_num. If >= 5, then we've been redirected too many times.
    if (attempt_num >= 5)
    {
      response = null;
      return RequestHandler::INVALID;
    }
    // Read the headers until we find the location
    // Read the response headers, which are terminated by a blank line.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r")
    {
      // Find the new location header
      if (!header.empty())
      {
        std::pair<std::string, std::string> parsed_header = ProcessHeaderLine(header);
        if (parsed_header.first == "Location")
        {
          // Send proxy request to new location
          return SendProxyRequest(request_string, response, attempt_num + 1, parsed_header.second);
        }
      }
    }

    // Could not find the location header.
    return RequestHandler::INVALID;
  }
  else if (status_code != Response::ok)
  {
    return RequestHandler::INVALID;
  }

  // Process the response headers.
  std::string header;
  while (std::getline(response_stream, header) && header != "\r")
  {
    // Get the header attribute
    // Get header value
    // Set header attribute/value in response
    if (!header.empty()) {
      std::pair<std::string, std::string> parsed_header = ProcessHeaderLine(header);
      if (parsed_header.first != "" && parsed_header.second != "")
      {
        response->AddHeader(parsed_header.first, parsed_header.second);
      }
    }
  }

  // Read to end of response
  // Modify response (prepend /proxy to beginning of all href attributes)
  // (use boost::replace_all?)

  return RequestHandler::OK;
}

std::pair<std::string, std::string> ReverseProxy::ProcessHeaderLine(std::string header)
{
  std::string header_field = "";
  std::string header_value = "";
  std::size_t field_index = header.find_first_of(":");

  if (field_index != std::string::npos) {
    header_field = header.substr(0, field_index);
    header_value = header.substr(field_index+2, std::string::npos);
    //delete carriage return
    header_value = header_value.substr(0, header_value.length()-1);
  }

  return std::make_pair( header_field, header_value);
}

//
// response.hpp
// ~~~~~~~~~
//


#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "header.hpp"


// Represents an HTTP response.
//
// Usage:
//   Response r;
//   r.SetStatus(RESPONSE_200);
//   r.SetBody(...);
//   return r.ToString();
//
// Constructed by the RequestHandler, after which the server should call ToString
// to serialize.
class Response
{
public:
  /// The status of the response.
  enum ResponseCode
  {
    ok = 200,
    created = 201,
    accepted = 202,
    no_content = 204,
    multiple_choices = 300,
    moved_permanently = 301,
    moved_temporarily = 302,
    not_modified = 304,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503
  };

  // Change status of the response. 
  void SetStatus(const ResponseCode response_code);

  // Add a header.
  void AddHeader(const std::string& header_name, const std::string& header_value);

  // Set body content of response.
  void SetBody(const std::string& body);
  
  // Convert the content of response to string.
  std::string ToString();

private:
  // The response status to be included in the response.
  ResponseCode m_status;

  /// The headers to be included in the response.
  std::vector<header> m_headers;

  /// The body to be sent in the response.
  std::string m_body;

  /// Convert the response into a vector of buffers. The buffers do not own the
  /// underlying memory blocks, therefore the reply object must remain valid and
  /// not be changed until the write operation has completed.
  //std::vector<boost::asio::const_buffer> to_buffers();
/*
  /// Get a stock reply.
  static reply stock_reply(status_type status);*/
};



#endif // HTTP_RESPONSE_HPP

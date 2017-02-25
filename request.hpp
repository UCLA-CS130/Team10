//
// request.hpp
// ~~~~~~~~~~~
//


#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <vector>
#include <memory>



/// A request received from a client.
class Request
{
public:

  // Construct request
  Request();

  static std::unique_ptr<Request> Parse(const std::string& raw_request);
  std::string raw_request() const;
  std::string method() const;
  std::string uri() const;
  std::string version() const;

  using Headers = std::vector<std::pair<std::string, std::string>>;
  Headers headers() const;

  std::string body() const;
private:
  std::string m_raw_request;
  std::string m_method;
  std::string m_uri;
  std::string m_version;
  std::string m_body;
  Headers m_headers;
};


#endif // HTTP_REQUEST_HPP

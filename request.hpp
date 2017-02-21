//
// request.hpp
// ~~~~~~~~~~~
//


#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <vector>


/// A request received from a client.
class Request
{
public:
  static unique_ptr<Request> Parse(const std::string& raw_request);

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
  Headers m_headers;
  
  /*std::string toString() const{
    std::string result = "";
    result += method;
    result += " ";
    result += uri;
    result += " HTTP/";
    result += std::to_string(http_version_major);
    result += ".";
    result += std::to_string(http_version_minor);
    result += "\r\n";
    for (const auto& header : headers) {
        result += header.name;
        result += ": ";
        result += header.value;
        result += "\r\n";
    }
    result += "\r\n";
    return result;
  }*/
};


#endif // HTTP_REQUEST_HPP

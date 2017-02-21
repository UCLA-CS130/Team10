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

  // Construct request
  Request();
  /// Reset to initial parser state.
  void reset();
  bool isStart();

  static unique_ptr<Request> Parse(const std::string& raw_request);

  /// Consume next input character
  boost::tribool consume(const std::string& raw_request, char input);

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


  /// Check if a byte is an HTTP character.
  static bool is_char(int c);

  /// Check if a byte is an HTTP control character.
  static bool is_ctl(int c);

  /// Check if a byte is defined as an HTTP tspecial character.
  static bool is_tspecial(int c);

  /// Check if a byte is a digit.
  static bool is_digit(int c);

  /// The current state of the parser.
  enum state
  {
    method_start,
    method,
    uri,
    http_version_h,
    http_version_t_1,
    http_version_t_2,
    http_version_p,
    http_version_slash,
    http_version_major_start,
    http_version_major,
    http_version_minor_start,
    http_version_minor,
    expecting_newline_1,
    header_line_start,
    header_lws,
    header_name,
    space_before_header_value,
    header_value,
    expecting_newline_2,
    expecting_newline_3
  } m_state;

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

#include "request.hpp"

namespace http {
namespace server {

request_parser::request_parser()
  : m_state(method_start)
{
}

void request_parser::reset()
{
  m_state = method_start;
}


bool request_parser::isStart()
{
  return m_state == method_start;
}

std::unique_ptr<Request> Request::Parse(const std::string& raw_request)
{
  //TODO: Parse will use consume() to handle the raw_request character by character

}

boost::tribool request::consume(std::unique_ptr& req, char input)
{

  // TODO: test consume with Parse method

  switch(m_state)
  {
    case method_start:

      if (!is_char(input) || is_ctl(input) || is_tspecial(input))
      {
        return false;
      }
      else
      {
        m_state = method;
        req->m_method.push_back(input);
        return boost::indeterminate;
      }

    case method:

      if (input == ' ')
      {
        m_state = uri;
        return boost::indeterminate;
      }
      else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
      {
        return false;
      }
      else
      {
        req->m_method.push_back(input);
        return boost::indeterminate;
      }

    case uri:

      if (input == ' ')
      {
        m_state = http_version_h;
        return boost::indeterminate;
      }
      else if (is_ctl(input))
      {
        return false;
      }
      else
      {
        req->m_uri.push_back(input);
        return boost::indeterminate;
      }

    case http_version_h:

      if (input == 'H')
      {
        m_state = http_version_t_1;
        return boost::indeterminate;
      }
      else
      {
        return false;
      }

    case http_version_t_1:

      if (input == 'T')
      {
        m_state = http_version_t_2;
        return boost::indeterminate;
      }
      else
      {
        return false;
      }

    case http_version_t_2:

      if (input == 'T')
      {
        m_state = http_version_p;
        return boost::indeterminate;
      }
      else
      {
        return false;
      }

    case http_version_p:

      if (input == 'P')
      {
        m_state = http_version_slash;
        return boost::indeterminate;
      }
      else
      {
        return false;
      }

    case http_version_slash:

      if (input == '/')
      {
        m_state = http_version_major_start;
        return boost::indeterminate;
      }
      else
      {
        return false;
      }

      // TODO: Check how version major and minor are stored 
      // TODO: Possibly combine into one version variable?
    case http_version_major_start:

      if (is_digit(input))
      {
        req.m_version = req.m_version * 10 + input - '0';
        state_ = http_version_major;
        return boost::indeterminate;
      }
      else
      {
        return false;
      }

    case http_version_major:

      if (input == '.')
      {
        req->m_version.push_back(input);
        m_state = http_version_minor_start;
        return boost::indeterminate;
      }
      else if (is_digit(input))
      {
        req->m_version = req->m_version * 10 + input - '0';
        return boost::indeterminate;
      }
      else
      {
        return false;
      }
    case http_version_minor_start:
      if (is_digit(input))
      {
        req->m_version.push_back(input);
        m_state = http_version_minor;
        return boost::indeterminate;
      }
      else
      {
        return false;
      }
    case http_version_minor:
      if (input == '\r')
      {
        state_ = expecting_newline_1;
        return boost::indeterminate;
      }
      else if (is_digit(input))
      {
        req->m_version.push_back(input);
        return boost::indeterminate;
      }
      else
      {
        return false;
      }

    /// TODO: Headers
  }
}

std::string Request::raw_request() const
{
  return m_raw_request;
}

std::string Request::method() const
{
  return m_method;
}

std::string Request::uri() const
{
  return m_uri;
}

std::string Request::version() const
{
  return m_version;
}

Headers Request::headers() const
{
  return m_headers;
}

std::string Request::body() const
{
  return m_body;
}


// character validators

bool request_parser::is_char(int c)
{
  return c >= 0 && c <= 127;
}

bool request_parser::is_ctl(int c)
{
  return (c >= 0 && c <= 31) || (c == 127);
}

bool request_parser::is_tspecial(int c)
{
  switch (c)
  {
  case '(': case ')': case '<': case '>': case '@':
  case ',': case ';': case ':': case '\\': case '"':
  case '/': case '[': case ']': case '?': case '=':
  case '{': case '}': case ' ': case '\t':
    return true;
  default:
    return false;
  }
}

bool request_parser::is_digit(int c)
{
  return c >= '0' && c <= '9';
}

} // namespace server
} // namespace http

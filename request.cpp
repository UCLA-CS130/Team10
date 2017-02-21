#include "request.hpp"

std::unique_ptr<Request> Request::Parse(const std::string& raw_request)
{
  //TODO: code for Parse
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



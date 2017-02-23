//
// file_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "file_handler.hpp"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include "mime_types.hpp"


StaticHandler::StaticHandler()
{

}

RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
  m_uri_prefix = uri_prefix;
  for ( auto statement : config.statements_ )
  {
    for (auto token: statement->tokens_)
    {
      if (token == "root" && statement->tokens_.size()==2)
        m_static_path= statement->tokens_[1];
    }
  }
  return RequestHandler::OK;
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request,
                               Response* response)
{
  std::cout << "in FileHandler HandleRequest...\n";
  // Decode url to path.
  std::string request_path;
  if (!url_decode(request.uri(), request_path))
  {
    // TODO: make the line below work again please.
    //response = reply::stock_reply(reply::bad_request);
    return RequestHandler::OK;
  }


  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    // TODO:
    // response = reply::stock_reply(reply::bad_request);
    return RequestHandler::OK;
  }


  if (request_path.substr(0,m_uri_prefix.length()) != m_uri_prefix) {
    // TODO:
    //rep = reply::stock_reply(reply::bad_request);
    //std::cout << request_path<<'\n';
    //std::cout << m_static_path<<'\n';
    return RequestHandler::OK;
  }
  

  // If path ends in slash (i.e. is a directory) then add "index.html".
  if (request_path[request_path.size() - 1] == '/')
  {
    request_path += "index.html";
  }

  // Determine the file extension.
  std::size_t last_slash_pos = request_path.find_last_of("/");
  std::size_t last_dot_pos = request_path.find_last_of(".");
  std::string extension;
  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
  {
    extension = request_path.substr(last_dot_pos + 1);
  }

  // Open the file to send back.
  std::string full_path = request_path;
  boost::replace_all(full_path, m_uri_prefix, m_static_path);
  std::cout << "m_static_path is " << m_static_path << '\n' << "m_uri_prefix is " << m_uri_prefix <<'\n'<< "full path is " << full_path << '\n';
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is)
  {
    // TODO:
    // rep = reply::stock_reply(reply::not_found);
    std::cerr<<"Cannot open file...\n";
    return RequestHandler::OK;
  }

  std::cout << "Start to read the file...\n";
  response->SetStatus(Response::ok);
  char buf[512];
  while (is.read(buf, sizeof(buf)).gcount() > 0)
  {
    // TODO: Need an append function instead of a set function!
    std::string to_send;
    to_send.append(buf, is.gcount());
    response->SetBody(to_send);
  }
  response->AddHeader("Content-Length", std::to_string(response->ContentLength()));
  response->AddHeader("Content-Type", mime_types::extension_to_type(extension));
  return RequestHandler::OK;
}


bool StaticHandler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

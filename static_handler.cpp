//
// file_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "static_handler.hpp"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include "mime_types.hpp"
#include "encoder.hpp"
#include "markdown.hpp"


StaticHandler::StaticHandler()
{}

RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
  m_uri_prefix = uri_prefix;
  m_compression_enabled = false;
  m_static_path = "";

  for ( auto statement : config.statements_ )
  {
    for (auto token: statement->tokens_)
    {
      if (token == "root" && statement->tokens_.size()==2)
      {
        m_static_path= statement->tokens_[1];
      }
      else if (token == "compression" && statement->tokens_.size()==2)
      {
        if (statement->tokens_[1] == "enabled")
          m_compression_enabled = true;
      }
    }
  }
  if (m_static_path == "")
    return RequestHandler::INVALID;
  return RequestHandler::OK;
}

RequestHandler::Status StaticHandler::HandleRequest(const Request& request,
                               Response* response)
{
  std::cout << "FileHandler HandleRequest...\n";

  std::string accepted_encodings = "";
  for (auto const& header: request.headers()) 
  {
      if (header.first == "Accept-Encoding")
      {
        accepted_encodings = header.second;
      }
  }

  // Decode url to path.
  std::string request_path;
  if (!url_decode(request.uri(), request_path))
  {
    std::cerr << "Invalid url\n";
    return RequestHandler::INVALID;
  }

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    std::cerr << "Request path must be absolute and should not contain \"..\"\n";
    return RequestHandler::INVALID;
  }

  // This case may never happen. It is catched by NotFoundHandler
  if (request_path.substr(0,m_uri_prefix.length()) != m_uri_prefix) {
    return RequestHandler::INVALID;
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
  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!is)
  {
    std::cerr << "Cannot open file \"" << full_path << "\"...\n";
    return RequestHandler::INVALID;
  }

  std::cout << "Start reading the file...\n";

  char buf[512];
  std::string to_send;
  // May not work with a large file
  if(extension == "md"){
    markdown::Document doc;
    std::istream *in = &is;
    std::ostringstream ss;
    doc.read(*in);
    doc.write(ss);
    to_send = ss.str();
  }
  else{
    while (is.read(buf, sizeof(buf)).gcount() > 0){
      to_send.append(buf, is.gcount());
    }
  }

  std::pair<std::string, std::string> content_encoding_header(std::string("Content-Encoding"), std::string("identity"));

  Encoder::encode(to_send, content_encoding_header, accepted_encodings);
  
  response->SetBody(to_send);
  response->SetStatus(Response::ok);
  response->AddHeader(content_encoding_header);
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

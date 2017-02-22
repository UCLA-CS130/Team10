//
// echo_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//

#include "echo_handler.hpp"
#include <fstream>
#include <sstream>
//#include <string>
//#include <boost/lexical_cast.hpp>
//#include "response.hpp"
//#include "request.hpp"

EchoHandler::EchoHandler()
{

}

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix,
                      const NginxConfig& config)
{
  return RequestHandler::OK;
}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request,
                               Response* response)
{
  response->SetStatus(Response::ok);
  response->AddHeader("Content-Length", std::to_string(request.raw_request().size()));
  response->AddHeader("Content-Type", "text/plain"); 
  response->SetBody(request.raw_request());
  return RequestHandler::OK;
}

/*void request_handler_echo::handle_request(const request& req, reply& rep)
{
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }

  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }


  // Fill out the reply to be sent to the client.
  rep.status = reply::ok;
  rep.content = req.toString();
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/plain";
}

bool request_handler_echo::url_decode(const std::string& in, std::string& out)
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
*/

#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "config_parser.hpp"
#include "request_handler.hpp"

typedef std::map <std::string, std::shared_ptr<RequestHandler>> HandlerMap;
//typedef std::unordered_map <std::string, std::string> Dictionary;

class ServerConfig {
public:
  ServerConfig();
  bool Init(const char* config_file);

  std::string Port() const;
 
  HandlerMap Handler_map() const;


private:
  HandlerMap m_handler_map;
  std::string m_port;
};

#endif // SERVER_CONFIG_HPP

#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "config_parser.hpp"
#include "request_handler.hpp"

typedef std::map <std::string, std::pair<std::string, std::shared_ptr<RequestHandler>>> HandlerMap;
//typedef std::unordered_map <std::string, std::string> Dictionary;

class ServerConfig {
public:
  // Constructor
  ServerConfig();

  // Initialize the class
  // return true if config_file is legal. Otherwise, return false.
  bool Init(const char* config_file);

  // get port number
  std::string Port() const;

  // get uri-handler map
  HandlerMap Handler_map() const;


private:
  // URI to request_handler map
  HandlerMap m_handler_map;
  // Port number
  std::string m_port;
};

#endif // SERVER_CONFIG_HPP

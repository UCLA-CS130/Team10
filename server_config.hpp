#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.hpp"

#include "config_parser.hpp"


typedef std::unordered_map <std::string, std::string> Dictionary;

class ServerConfig {
public:
  ServerConfig();
  bool Init(const NginxConfig* config);
  std::string Root();
  std::string Port();
  std::string Echo();
  std::string Static();


private:
  Dictionary m_params;
};

#endif // SERVER_CONFIG_HPP

#ifndef LOG_H
#define LOG_H

#include <vector>
#include <string>
#include <iostream>
#include "request_handler.hpp"

struct Record{
  std::string request_url;
  std::string response_code;
};

typedef std::map <std::string, std::pair<std::string, std::shared_ptr<RequestHandler>>> HandlerMap;


class Log{
public:
  static Log *instance();

  void add_record(const std::string& url, const std::string& response_code);
  void add_mapping(const std::string& url_prefix, const std::string& handler_name, std::shared_ptr<RequestHandler> handler_ptr);
  HandlerMap get_map() const;

  std::string map_to_string() const;
  std::string records_to_string() const;

private:
  static Log *m_instance;
  HandlerMap m_handler_map;
  std::vector<Record> m_records;
};

#endif
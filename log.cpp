#include "log.hpp"

Log *Log::m_instance;

Log *Log::instance(){
	if(!m_instance){
		m_instance = new Log;
	}
	return m_instance;
}

void Log::add_record(const std::string& url, const std::string& response_code)/*, const std::string& handler_name, const std::string& url_prefix)*/{
	Record rec;
	rec.request_url = url;
	rec.response_code = response_code;
	m_records.push_back(rec);
}

std::string Log::records_to_string() const{
	std::string result = "";
	result += "Number of connections: ";
	result += std::to_string(m_records.size());
	result += '\n';
	for(unsigned int i = 0; i < m_records.size(); i++){
		result += m_records[i].request_url;
		result += ": ";
		result += m_records[i].response_code;
		result += '\n';
	}
	return result;
}

void Log::add_mapping(const std::string& url_prefix, const std::string& handler_name, std::shared_ptr<RequestHandler> handler_ptr)
{
  m_handler_map[url_prefix] = std::make_pair(handler_name, std::move(handler_ptr));
}

std::string Log::map_to_string() const {
  std::string result = "";
  for(auto const &pair : m_handler_map){
    result += pair.first;
    result += " -> ";
    result += pair.second.first;
    result += '\n';  
  }
  return result;
}

HandlerMap Log::get_map() const
{
	return m_handler_map;
}
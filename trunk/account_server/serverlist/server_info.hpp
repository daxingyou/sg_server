#ifndef __SERVER_INFO_HPP__
#define __SERVER_INFO_HPP__

#include "macros.hpp"
#include <string>

class server_info_t 
{
public:
	void init(uint32_t server_id, uint8_t status, std::string &name, uint8_t label, std::string &makr_type );
public:
	std::string m_name = "";
	uint32_t m_server_id = 0;
	uint8_t	 m_status = 0;
	uint8_t	 m_label= 0;
	std::string  m_mark_type = "";
};

typedef boost::shared_ptr<server_info_t> server_info_ptr;

#endif

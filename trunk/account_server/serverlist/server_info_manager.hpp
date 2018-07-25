#ifndef __SERVER_INFO_MANAGER_HPP__
#define __SERVER_INFO_MANAGER_HPP__

#include "web_curl_wrapper.hpp"
#include "server_info.hpp"
#include <map>
#include <string>

typedef std::map<uint32_t, server_info_ptr> server_info_map_t;		
typedef std::map <std::string, uint8_t>   compatible_verson_map_t;		//兼容版本列表
class server_info_manager_t 
{
public:
	static bool is_commpatible_version(const std::string &server_list_type, std::string version);

	static bool is_in_login_server_list(uint32_t server_id);
public:
	static bool load_server_info();

	//server_list_type 对应sg_update表中的mark_type。
	//原本以为只有服务端使用结果客户端使用了mark_type,这里仍然使用server_list_type;
	static bool get_login_server_info_requrest( std::string &server_list_type );

	static bool get_commpatible_info_request( std::string &server_list_type);

	static bool load_login_server_list_from_web(std::string &server_list_type, std::string  json_data);

	static bool load_commpatible_info_request(std::string &server_list_type, std::string  json_data);
public:
	static std::map<std::string, server_info_map_t> m_server_info_map;		//服务器标记对应服务器状态列表
	static std::map<std::string, compatible_verson_map_t> m_compatible_map;	//兼容版本列表
	static std::vector<std::string> m_server_list_type_vec;					//服务器标记列表(用于过滤服务器标记)	
};


#endif
#ifndef __SERVER_INFO_MANAGER_HPP__
#define __SERVER_INFO_MANAGER_HPP__

#include "web_curl_wrapper.hpp"
#include "server_info.hpp"
#include <map>
#include <string>

typedef std::map<uint32_t, server_info_ptr> server_info_map_t;		
typedef std::map <std::string, uint8_t>   compatible_verson_map_t;		//���ݰ汾�б�
class server_info_manager_t 
{
public:
	static bool is_commpatible_version(const std::string &server_list_type, std::string version);

	static bool is_in_login_server_list(uint32_t server_id);
public:
	static bool load_server_info();

	//server_list_type ��Ӧsg_update���е�mark_type��
	//ԭ����Ϊֻ�з����ʹ�ý���ͻ���ʹ����mark_type,������Ȼʹ��server_list_type;
	static bool get_login_server_info_requrest( std::string &server_list_type );

	static bool get_commpatible_info_request( std::string &server_list_type);

	static bool load_login_server_list_from_web(std::string &server_list_type, std::string  json_data);

	static bool load_commpatible_info_request(std::string &server_list_type, std::string  json_data);
public:
	static std::map<std::string, server_info_map_t> m_server_info_map;		//��������Ƕ�Ӧ������״̬�б�
	static std::map<std::string, compatible_verson_map_t> m_compatible_map;	//���ݰ汾�б�
	static std::vector<std::string> m_server_list_type_vec;					//����������б�(���ڹ��˷��������)	
};


#endif
#define BOOST_SPIRIT_THREADSAFE 
#include "main/account_server.hpp"
#include "server_info_manager.hpp"
#include "web_curl_wrapper.hpp"
#include "channel/channel_manager.hpp"
#include "log.hpp"
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


std::map<std::string, server_info_map_t> server_info_manager_t::m_server_info_map;			//服务器标记对应服务器状态列表
std::map <std::string, compatible_verson_map_t> server_info_manager_t::m_compatible_map;	//兼容版本列表
std::vector<std::string> server_info_manager_t::m_server_list_type_vec;					//服务器列表标记列表


bool server_info_manager_t::load_login_server_list_from_web(std::string &server_list_type, std::string  json_data)
{
	try 
	{
		if (json_data.empty()) 
		{
			log_error("json_data is empty");
			return false;
		}

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		std::string status = pt.get<std::string>("status");
		if ("success" == status)
		{
			//读取login_server_list列表
			server_info_map_t server_info_list;
			boost::property_tree::ptree server_list = pt.get_child("message.server_list");
			BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, server_list)
			{
				boost::property_tree::ptree node_server_id = vt.second.get_child("server_id");
				boost::property_tree::ptree node_status    = vt.second.get_child("status");
				boost::property_tree::ptree node_label	   = vt.second.get_child("label");
				boost::property_tree::ptree node_name	   = vt.second.get_child("name");
				boost::property_tree::ptree node_type	   = vt.second.get_child("server_list_type");
				
				uint32_t server_id	= node_server_id.get_value<uint32_t>();
				uint8_t  status		= node_status.get_value<uint8_t>();
				uint8_t  label		= node_label.get_value<uint8_t>();
				std::string server_name	= node_name.get_value<std::string>();
				std::string server_list_type  = node_type.get_value<std::string>();

				server_info_ptr p_server_info(new server_info_t());
				p_server_info->init(server_id, status, server_name, label, server_list_type);

				//生成标记对应列表
				auto iter_f = server_info_manager_t::m_server_info_map.find(server_list_type);
				if (iter_f == server_info_manager_t::m_server_info_map.end())
				{
					server_info_map_t mark_server_info_map;
					mark_server_info_map.insert(std::make_pair(server_id, p_server_info));
					server_info_manager_t::m_server_info_map.insert(std::make_pair(server_list_type, mark_server_info_map));
				}
				else
				{
					auto &mark_server_info_map = iter_f->second;
					mark_server_info_map.insert(std::make_pair(server_id, p_server_info));
				}
				log_info("load server_list_type %s, server_id:%d, status:%d, server_name:%s", server_list_type.c_str(), server_id, status, server_name.c_str());
			}
		}
	}
	catch (boost::property_tree::ptree_error& e) 
	{
		log_error("parse json[%s] failed, error[%s] json_data[%s]", json_data.c_str(), e.what());
		return false;
	}

	return true;
}

bool server_info_manager_t::get_login_server_info_requrest(std::string &server_list_type) 
{
	char url[512];
	memset(url, 0, sizeof(url));
	sprintf(url, "%s/login_version.php?server_list_type=%s", env::xml->web_server_url.c_str(), server_list_type.c_str());
	log_info("get url:=%s", url);
	web_curl_wapper_ptr p_curl(new web_curl_wapper_t());
	if (!p_curl->init(url))
	{
		p_curl->clean();
		return false;
	}
	
	std::string json_data = p_curl->get_http_request();
	p_curl->clean();
	return load_login_server_list_from_web(server_list_type, json_data);
}

bool server_info_manager_t::get_commpatible_info_request(std::string &server_list_type)
{
	char url[512];
	memset(url, 0, sizeof(url));
	sprintf(url, "%s/compatile_version.php?server_list_type=%s", env::xml->web_server_url.c_str(), server_list_type.c_str());
	log_info("get url:=%s", url);
	web_curl_wapper_ptr p_curl(new web_curl_wapper_t());
	if (!p_curl->init(url))
	{
		p_curl->clean();
		return false;
	}

	std::string json_data = p_curl->get_http_request();
	p_curl->clean();
	return load_commpatible_info_request(server_list_type, json_data);
}

bool server_info_manager_t::load_commpatible_info_request(std::string &server_list_type, std::string  json_data)
{
	try
	{
		if (json_data.empty())
		{
			log_error("json_data is empty");
			return false;
		}

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		std::string status = pt.get<std::string>("status");
		if ("success" == status)
		{
			//读取兼容列表
			compatible_verson_map_t compatible_verson_list;
			boost::property_tree::ptree node_comp_version = pt.get_child("message.comp_version_list");
			BOOST_FOREACH(boost::property_tree::ptree::value_type &vt, node_comp_version)
			{
				std::string version = vt.second.get_value<std::string>();
				compatible_verson_list.insert(std::make_pair(version, 1));
				log_info("load server_list_type %s compatible version :%s", server_list_type.c_str(), version.c_str());
			}
			server_info_manager_t::m_compatible_map.insert(std::make_pair(server_list_type, compatible_verson_list));
		}
	}
	catch (boost::property_tree::ptree_error& e)
	{
		log_error("parse json[%s] failed, error[%s] json_data[%s]", json_data.c_str(), e.what());
		return false;
	}

	return true;
}


bool server_info_manager_t::load_server_info() 
{
	server_info_manager_t::m_server_info_map.clear();
	server_info_manager_t::m_compatible_map.clear();

	//获取服务器列表
	for (auto it = server_info_manager_t::m_server_list_type_vec.begin(); it != server_info_manager_t::m_server_list_type_vec.end(); ++it)
	{
		if (!get_login_server_info_requrest(*it)) 
		{
			log_error("load login_server_list, server_list_type[%s] info failed!", (*it).c_str() );
			continue;
		}

		//获取兼容版本列表
		if (!get_commpatible_info_request(*it)) 
		{
			log_error( "load commpatible_version, server_list_type:[%s] info failed!", (*it).c_str() );
			continue;
		}
	}
	log_info("m_channel_list:%d", channel_manager_t::m_channel_list.size());
	return true;
}

//判断server_id是否可以连接account
bool server_info_manager_t::is_in_login_server_list(uint32_t server_id) 
{
	for (auto iter = server_info_manager_t::m_server_info_map.begin(); iter != server_info_manager_t::m_server_info_map.end(); ++iter) 
	{
		auto &login_server_list = iter->second;
		auto it_login = login_server_list.begin();
		auto it_end = login_server_list.end();
		for (; it_login != it_end; ++it_login)
		{
			if (server_id == it_login->first)
			{
				return true;
			}
		}
	}
	return false;
}

bool server_info_manager_t::is_commpatible_version(const std::string &server_list_type, std::string version)
{
	auto iter_f = server_info_manager_t::m_compatible_map.find(server_list_type);
	if (iter_f == server_info_manager_t::m_compatible_map.end())
	{
		return false;
	}

	auto &version_list = iter_f->second;
	if (version_list.empty()) 
	{
		return true;
	}

	auto iter_version_f = version_list.find(version);
	if (iter_version_f == version_list.end())
	{
		return false;
	}
	return true;
}

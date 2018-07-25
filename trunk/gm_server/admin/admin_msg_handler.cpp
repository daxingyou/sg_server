#include "admin_msg_handler.hpp"
#include "main/gm_server.hpp"
#include "admin_manager.hpp"
#include "common/common_struct.hpp"
#include "admin_func_manager.hpp"
#include <boost/property_tree/json_parser.hpp>
#include "common/rapidjson/document.h"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::TM_BEGIN, op_cmd::TM_END, admin_msg_handler_t::msg_handler_func> admin_msg_handler_t::m_admin_msg_handler;

bool admin_msg_handler_t::init_msg_handler()
{
    bool ret = true;

	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_send_mail_request, handle_tm_send_mail_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_get_user_ol_num_request, handle_tm_get_user_ol_num_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_send_notice_request, handle_tm_send_notice_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_transport_request, handle_tm_transport_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_set_user_create_limit_request, handle_tm_set_user_create_limit_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_refresh_login_list, handle_tm_refresh_login_list);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_manage_role_request, handle_tm_manage_role_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_search_role_byid_request, handle_tm_search_role_byid_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_search_role_byname_request, handle_tm_search_role_byname_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_reload_config_request, handle_tm_reload_config_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_reload_server_config_request, handle_tm_reload_server_config_request);
    return ret;
}


bool admin_msg_handler_t::init_center_msg_handler()
{
	bool ret = true;

	return ret;
}


//发送邮件
bool admin_msg_handler_t::handle_tm_send_mail_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	char json_data[2048] = {0};
	memcpy(json_data, msg_buf->data(), msg_buf->size());

	//回包
	std::string buf_str( json_data );
	if (!admin_func_manager_t::send_mail_to_center(buf_str))
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1) );
		env::server->post_msg_to_gm( s, p_ret_buf);
	}
	else
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("0", 1) );
		env::server->post_msg_to_gm( s, p_ret_buf);
	}
	return true;
}

bool admin_msg_handler_t::handle_tm_get_user_ol_num_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	admin_ptr p_admin = admin_manager_t::get_admin(s);
	if (p_admin)
		admin_func_manager_t::get_user_num_from_center( p_admin->get_admin_id() );
	return true;
}


bool admin_msg_handler_t::handle_tm_send_notice_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	char json_data[2048];
	bzero(json_data, sizeof(json_data));
	memcpy(json_data, msg_buf->data(), msg_buf->size());
	json_data[2047] = '\0';

	//回包
	std::string buf_str(json_data);
	if (!admin_func_manager_t::send_notice_to_center(buf_str))
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	else
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("0", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	return true;
}

bool admin_msg_handler_t::handle_tm_transport_request(const network::tcp_socket_ptr & s, const network::msg_buf_ptr & msg_buf)
{
	char json_data[2048];
	bzero(json_data, sizeof(json_data));
	memcpy(json_data, msg_buf->data(), msg_buf->size());
	json_data[2047] = '\0';

	//回包
	std::string buf_str(json_data);
	if (!admin_func_manager_t::transport_to_center(buf_str))
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	else
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("0", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	return true;
}

bool admin_msg_handler_t::handle_tm_set_user_create_limit_request(const network::tcp_socket_ptr & s, const network::msg_buf_ptr & msg_buf)
{
	char json_data[2048];
	bzero(json_data, sizeof(json_data));
	memcpy(json_data, msg_buf->data(), msg_buf->size());
	json_data[2047] = '\0';

	//回包
	std::string buf_str(json_data);
	if (!admin_func_manager_t::set_user_create_limit(buf_str))
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	else
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("0", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	return true;
}

bool admin_msg_handler_t::handle_tm_refresh_login_list(const network::tcp_socket_ptr & s, const network::msg_buf_ptr & msg_buf)
{
	if (!admin_func_manager_t::refresh_ac_login_list())
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	else
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("0", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	return true;
}

bool admin_msg_handler_t::handle_tm_manage_role_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	char json_data[2048] = { 0 };
	memcpy(json_data, msg_buf->data(), msg_buf->size());

	//回包
	std::string buf_str(json_data);
	
	admin_ptr p_admin = admin_manager_t::get_admin(s);
	if (NULL == p_admin)
	{
		log_error("NULL == p_admin");
		return false;
	}
	if (!admin_func_manager_t::manage_role(p_admin->get_admin_id(), buf_str))
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}

	return true;
}

bool admin_msg_handler_t::handle_tm_search_role_byid_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	char json_data[2048] = { 0 };
	memcpy(json_data, msg_buf->data(), msg_buf->size());

	//回包
	std::string buf_str(json_data);

	admin_ptr p_admin = admin_manager_t::get_admin(s);
	if (p_admin)
		admin_func_manager_t::search_role_byid(s, buf_str);
	return true;
}

bool admin_msg_handler_t::handle_tm_search_role_byname_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	char json_data[2048] = { 0 };
	memcpy(json_data, msg_buf->data(), msg_buf->size());

	//回包
	std::string buf_str(json_data);

	admin_ptr p_admin = admin_manager_t::get_admin(s);
	if (p_admin)
		admin_func_manager_t::search_role_byname(s, buf_str);
	return true;
}

bool admin_msg_handler_t::handle_tm_reload_config_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	char json_data[2048] = { 0 };
	memcpy(json_data, msg_buf->data(), msg_buf->size());

	//回包
	std::string buf_str(json_data);

	admin_ptr p_admin = admin_manager_t::get_admin(s);
	if (NULL == p_admin)
	{
		log_error("NULL == p_admin");
		return false;
	}
	if (!admin_func_manager_t::reload_config(buf_str))
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("1", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	else
	{
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("0", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	return true;
}

bool admin_msg_handler_t::handle_tm_reload_server_config_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	try{
		rapidjson::Document d;
		d.Parse(msg_buf->data(), msg_buf->size());

		rapidjson::Value& st = d["server_type"];
		rapidjson::Value& f = d["oper_name"];

		std::string server_type = st.GetString();;
		std::string file = f.GetString();

		std::cout << file<< std::endl;

		std::ofstream ofs("../server.xml", std::ios::out);
		ofs.write(file.c_str(), file.size());
		//TODO notify server to reload xml
		proto::gm::mall_reload_xml_notify req;
		req.set_dest(server_type);
		env::server->send_msg_to_center(op_cmd::me_reload_xml_request, 0, req);
		network::msg_buf_ptr p_ret_buf(new network::msg_buf_t("0", 1));
		env::server->post_msg_to_gm(s, p_ret_buf);
	}
	catch(std::exception& e){
		std::cout << e.what() << std::endl;
		return false;
	}
	return true;
}
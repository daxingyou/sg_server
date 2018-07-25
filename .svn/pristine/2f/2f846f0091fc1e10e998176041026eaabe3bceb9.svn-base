#include "admin_msg_handler.hpp"
#include "main/gm_server.hpp"
#include "admin_manager.hpp"
#include "common/common_struct.hpp"
#include "admin_func_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::TM_BEGIN, op_cmd::TM_END, admin_msg_handler_t::msg_handler_func> admin_msg_handler_t::m_admin_msg_handler;

bool admin_msg_handler_t::init_msg_handler()
{
    bool ret = true;

	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_test_request, handle_tm_test_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_send_mail_request, handle_tm_send_mail_request);
	ret &= admin_msg_handler_t::m_admin_msg_handler.register_func(op_cmd::tm_get_user_ol_num_request, handle_tm_get_user_ol_num_request);

    return ret;
}


bool admin_msg_handler_t::init_center_msg_handler()
{
	bool ret = true;

	return ret;
}


bool admin_msg_handler_t::handle_tm_test_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	std::string socket_address = s->socket().remote_endpoint().address().to_string();
	if (env::xml->ip_access.compare("0") != 0)
	{
		if (socket_address.compare(env::xml->ip_access.c_str()) != 0)
			return true;
	}

	proto::gm::tm_test_request request_msg;

	request_msg.ParseFromString(msg_buf->data());

	printf("Test:1 %u 2 %u \n", request_msg.test1(), request_msg.test2());

	proto::gm::mg_reload_config_notify reload_cfg_msg;
	env::server->send_msg_to_center(op_cmd::mg_reload_config_notify, 0, reload_cfg_msg);

	proto::gm::ma_masked_msg_notify masked_msg;
	masked_msg.add_masked_list(request_msg.test1());
	masked_msg.set_masked_flag(request_msg.test2());
	env::server->send_msg_to_center(op_cmd::ma_masked_msg_notify, 0, masked_msg);

	proto::gm::mt_test_reply reply_msg;
	reply_msg.set_test1(4294967295);
	reply_msg.set_test2(4294967295);
	env::server->send_msg_to_gm_tool(op_cmd::mt_test_reply, reply_msg);

	return true;
}


//发送邮件
bool admin_msg_handler_t::handle_tm_send_mail_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	char json_data[2048];
	bzero(json_data, sizeof(json_data));
	memcpy(json_data, msg_buf->data(), msg_buf->size());
	json_data[2047] = '\0';


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



#include "admin_func_manager.hpp"
#include "main/gm_server.hpp"
#include "log.hpp"
#include <string>
#include "protocpp/protos_fwd.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <common/utility.hpp>
#include "admin/admin_manager.hpp"

USING_NS_COMMON;

bool admin_func_manager_t::send_mail_to_center( std::string &json_data )
{
	try {
		printf("json_data:[%s] ", json_data.c_str() );

		std::stringstream ss(json_data);
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(ss, pt);

		//读取玩家列表		
		uint64_t uid		= pt.get<uint64_t>("uid");
		uint32_t type		= pt.get<uint32_t>("mail_data.type");
		std::string title	= pt.get<std::string>("mail_data.title");
		std::string content	= pt.get<std::string>("mail_data.content");
		std::string send_name  = pt.get<std::string>("mail_data.send_name");
		uint32_t send_time  = pt.get<uint32_t>("mail_data.send_time");
		uint32_t end_time	= pt.get<uint32_t>("mail_data.end_time");
		uint32_t on_time	= pt.get<uint32_t>("mail_data.on_time");
		std::string items	= pt.get<std::string>("mail_data.items");
		uint32_t limit_level = pt.get<uint32_t>("mail_data.limit_level");
		uint32_t limit_cond = pt.get<uint32_t>("mail_data.limit_cond");

		proto::gm::me_send_mail_request request;
		proto::gm_common::global_mail_single *p_mail = request.mutable_mail_data();
		p_mail->set_uid(uid);
		p_mail->set_type(type);
		p_mail->set_send_name(send_name);
		p_mail->set_title(title);
		p_mail->set_content(content);
		p_mail->set_send_time(send_time);
		p_mail->set_send_time(end_time);
		p_mail->set_send_hour(on_time);
		p_mail->set_items(items);
		p_mail->mutable_limit()->set_level(limit_level);
		p_mail->mutable_limit()->set_regist_type(limit_cond);
		
		env::server->send_msg_to_center(op_cmd::me_send_mail_request, 0, request);
	}
	catch (boost::property_tree::ptree_error& e)
	{
		printf("\033[31mthrow error[%s], [%s]\033[0m\n", e.what(), json_data.c_str() );
		return false;
	}

	return true;
}

bool admin_func_manager_t::get_user_num_from_center( uint32_t admin_id )
{
	proto::gm::me_get_user_ol_num_request request;
	request.set_admin_id( admin_id );
	env::server->send_msg_to_center(op_cmd::me_get_user_ol_num_request, 0,  request);
	return true;
}

bool admin_func_manager_t::send_user_num_to_tools(uint32_t admin_id, uint32_t num)
{
	admin_ptr p_admin = admin_manager_t::get_admin(admin_id);
	if (!p_admin)
	{
		log_debug("send user_num to tools , admin has offline");
		return false;
	}
	std::string num_str = string_util_t::uint32_to_string(num);
	network::msg_buf_ptr p_ret_buf(new network::msg_buf_t(num_str.c_str(), num_str.size()));
	env::server->post_msg_to_gm(p_admin->get_socket(), p_ret_buf);
	return true;
}


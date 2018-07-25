#include "gate_msg_handler.hpp"
#include "gate/gate_manager.hpp"
#include "main/login_server.hpp"
#include "main/login_server_fwd.hpp"
#include "network.hpp"
#include "user/user_manager.hpp"
#include "user/user_msg_handler.hpp"
#include "common/global_id.hpp"
#include "common/common_struct.hpp"

#include "utility.hpp"
#include <boost/tuple/tuple.hpp>
#include <vector>

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::AL_BEGIN, op_cmd::AL_END, gate_msg_handler_t::msg_handler_func> gate_msg_handler_t::m_gate_msg_handle;

bool gate_msg_handler_t::init_msg_handler()
{
    m_gate_msg_handle.register_func(op_cmd::al_update_gate_user_num_notify,     handle_al_update_gate_user_num_notify);
    m_gate_msg_handle.register_func(op_cmd::al_login_reply,                     handle_al_login_reply);
    m_gate_msg_handle.register_func(op_cmd::al_kick_role_notify,                handle_al_kick_role_notify);
    m_gate_msg_handle.register_func(op_cmd::al_syn_gate_info_notify,            handle_al_syn_gate_info_notify);
    m_gate_msg_handle.register_func(op_cmd::al_send_gm_msg_to_account_requst,	handle_al_send_gm_msg_to_account_requst);
	m_gate_msg_handle.register_func(op_cmd::al_kick_gate_all_user_notify, handle_al_kick_gate_all_user_notify);

    return true;
}

bool gate_msg_handler_t::handle_al_update_gate_user_num_notify(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::al_update_gate_user_num_notify);

    gate_ptr p_gate = gate_manager_t::get_gate_by_socket(s);
    if (NULL == p_gate)
    {
        log_error("NULL == p_gate");
        return false;
    }
    p_gate->set_online_users_num(msg.num());

    uint32_t all_online_users_num = 0;
    std::string gate_user_num_str = "";
    boost::tie(all_online_users_num, gate_user_num_str) = gate_manager_t::get_all_online_users_num();

    proto::server::ln_user_num_notify noti;
    noti.set_server_id(env::xml->srv_group.group_id);
    noti.set_user_num(all_online_users_num);
    env::server->send_msg_to_all_accounts(op_cmd::ln_user_num_notify, noti);

    log_info("all %d online users: %s", all_online_users_num, gate_user_num_str.c_str());
    return true;
}

bool gate_msg_handler_t::handle_al_login_reply(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::al_login_reply);

    user_ptr p_user = user_manager_t::get_user(uid);
    if (NULL == p_user)
    {
        log_error("user[%lu] was not exist!", uid);
        return false;
    }

    proto::client::lc_login_reply llr;
    if (0 == msg.reply_code())
    {
        //llr.set_ch_type(p_user->get_ch_type());
		llr.set_reply_code(LRC_OK);
        llr.set_uid(string_util_t::uint64_to_string(uid));
        llr.set_gate_ip(msg.gate_ip());
        llr.set_gate_port(msg.gate_port());
		proto::common::channel_general *p_general = llr.mutable_general();
		p_general->set_guid(msg.guid());
		p_general->set_session_id(msg.session_id());
    }
    else
    {
        log_error("user[%lu] get gate info failed: error[%d]", uid, msg.reply_code());
        llr.set_reply_code(LRC_GATE_ERROR);
    }
    p_user->send_msg_to_client(op_cmd::lc_login_reply, llr);

    return true;
}

bool gate_msg_handler_t::handle_al_kick_role_notify(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::al_kick_role_notify);

    gate_manager_t::del_user_from_gate(s, uid);

    return true;
}


bool gate_msg_handler_t::handle_al_kick_gate_all_user_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::al_kick_gate_all_user_notify);

	gate_manager_t::del_all_gate_user(msg.gate_id());

	return true;
}

bool gate_msg_handler_t::handle_al_syn_gate_info_notify(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::al_syn_gate_info_notify);

    uint32_t gate_id = msg.gate_id();
    gate_manager_t::set_gate(s, gate_id);

    for (int32_t i = 0; i < msg.user_list_size(); ++i)
    {
        uint64_t user_uid = msg.user_list(i);
        gate_manager_t::add_user_to_gate(gate_id, user_uid);
    }

    return true;
}

bool gate_msg_handler_t::handle_al_send_gm_msg_to_account_requst(const tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::al_send_gm_msg_to_account_requst);

	proto::server::ln_send_gm_msg_to_account_request request;
	request.set_server_id(env::xml->srv_group.group_id) ;
	request.set_cmd_type(msg.cmd_type());
	request.mutable_msg()->CopyFrom( msg.msg() );
	env::server->send_msg_to_all_accounts( op_cmd::ln_send_gm_msg_to_account_request , request );

	//如果是修改登录权限， 文件写入特殊处理
	if (msg.cmd_type() == op_cmd::tm_set_user_create_limit_request)
	{
		try 
		{
			const proto::common::gm_msg_to_account &p_data = msg.msg();
			boost::property_tree::ptree pt;
			boost::property_tree::xml_parser::read_xml(SERVER_XML_PATH, pt, boost::property_tree::xml_parser::trim_whitespace);
			boost::property_tree::xml_writer_settings<char> settings('\t', 1, "utf8");
			pt.put<uint32_t>("server.can_create_new_role", p_data.user_create_flag());
			boost::property_tree::xml_parser::write_xml(SERVER_XML_PATH, pt, std::locale(), settings);
		}
		catch (boost::property_tree::ptree_error& e)
		{
			printf("\033[31mthrow error[%s]\033[0m\n", e.what());
			return false;
		}
	}
	return true;
}
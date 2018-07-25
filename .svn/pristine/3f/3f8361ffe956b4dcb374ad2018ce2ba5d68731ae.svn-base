#include "db_msg_handler.hpp"
#include "network.hpp"
#include "config/config_manager.hpp"
#include "errcode_enum.hpp"
#include "role/money_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::DG_BEGIN, op_cmd::DG_END, db_msg_handler_t::msg_handler_func> db_msg_handler_t::m_db_msg_handle;

bool db_msg_handler_t::init_msg_handler()
{
    bool ret = true;
    ret &= m_db_msg_handle.register_func(op_cmd::dg_role_data_reply, handle_dg_role_data_reply);
	ret &= m_db_msg_handle.register_func(op_cmd::dg_change_name_reply, handle_dg_change_name_reply);

    return ret;
}

bool db_msg_handler_t::handle_dg_role_data_reply(const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::dg_role_data_reply);

	role_manager_t::on_user_record(uid, msg);

    return true;
}

bool db_msg_handler_t::handle_dg_change_name_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::dg_change_name_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	proto::client::gc_change_name_reply reply;
	reply.set_reply_code(msg.reply_code());
	if (msg.reply_code() == errcode_enum::error_ok)
	{
		//成功改名
		p_role->set_name(msg.new_name());
		if (0 == p_role->get_change_name())
		{
			p_role->set_change_name(1);
		}
		p_role->notify_object_common_info_for_nearbies();
		p_role->get_personal_info(reply.mutable_rcd()->mutable_per_info());
		reply.set_new_name(msg.new_name());
	}
	else
	{
		//失败返还
		if (msg.cost() > 0)
		{
			money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_YUANBAO, msg.cost(), log_enum::source_type_role_change_name, 0);
		}
	}
	p_role->send_msg_to_client(op_cmd::gc_change_name_reply, reply);
	return true;
}

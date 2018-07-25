#include "equip_msg_handle.hpp"
#include "equip_manager.hpp"
#include "common/log_enum.hpp"
#include "role/role.hpp"
#include "config/config_manager.hpp"
USING_NS_NETWORK;
USING_NS_COMMON;

bool equip_msg_handle_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_exchange_request, handle_cg_equip_exchange_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_on_request, handle_cg_equip_on_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_off_request, handle_cg_equip_off_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_remake_attr_request, handle_cg_equip_remake_attr_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_remake_exattr_request, handle_cg_equip_remake_exattr_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_replace_exattr_request, handle_cg_equip_replace_exattr_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_smelt_request, handle_cg_equip_smelt_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_smelt_data_request, handle_cg_equip_smelt_data_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_smelt_box_request, handle_cg_equip_smelt_box_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_equip_strengthen_request, handle_cg_equip_strengthen_request);
	return true;
}

bool equip_msg_handle_t::handle_cg_equip_exchange_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::client::cg_equip_exchange_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}

	proto::client::gc_equip_exchange_reply reply;
	uint32_t reply_code = equip_manager_t::exchange_equip(p_role, msg.type(), msg.exchange_id(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);

	p_role->send_msg_to_client(op_cmd::gc_equip_exchange_reply, reply);
    return true;
}

bool equip_msg_handle_t::handle_cg_equip_on_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_equip_on_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	uint64_t equip_uid = string_util_t::string_to_uint64(msg.equip_uid());
	uint64_t hero_uid = string_util_t::string_to_uint64(msg.hero_uid());
	proto::client::gc_equip_on_reply reply;
	uint32_t reply_code = equip_manager_t::equip_on(p_role, equip_uid, hero_uid, reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_equip_on_reply, reply);
	return true;
}

bool equip_msg_handle_t::handle_cg_equip_off_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_equip_off_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	uint64_t equip_uid = string_util_t::string_to_uint64(msg.equip_uid());
	uint64_t hero_uid = string_util_t::string_to_uint64(msg.hero_uid());
	proto::client::gc_equip_off_reply reply;
	uint32_t reply_code = equip_manager_t::equip_off(p_role, equip_uid, hero_uid, reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_equip_off_reply, reply);
	return true;
}

bool equip_msg_handle_t::handle_cg_equip_remake_attr_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_equip_remake_attr_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}

	uint64_t equip_uid = string_util_t::string_to_uint64(msg.equip_uid());
	proto::client::gc_equip_remake_attr_reply reply;
	uint32_t reply_code = equip_manager_t::remake_attr(p_role, equip_uid, msg.gold_flag(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_equip_remake_attr_reply, reply);
	return true;
}

bool equip_msg_handle_t::handle_cg_equip_remake_exattr_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_equip_remake_exattr_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	std::vector<uint32_t> lock_vec;
	for (int32_t i = 0; i < msg.lock_index_size(); ++i)
	{
		lock_vec.push_back(msg.lock_index(i));
	}

	uint64_t equip_uid = string_util_t::string_to_uint64(msg.equip_uid());
	proto::client::gc_equip_remake_exattr_reply reply;
	uint32_t reply_code = equip_manager_t::remake_exattr(p_role, equip_uid, lock_vec, reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_equip_remake_exattr_reply, reply);
	return true;
}

bool equip_msg_handle_t::handle_cg_equip_replace_exattr_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_equip_replace_exattr_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	uint64_t equip_uid = string_util_t::string_to_uint64(msg.equip_uid());
	proto::client::gc_equip_replace_exattr_reply reply;
	uint32_t reply_code = equip_manager_t::replace_exattr(p_role, equip_uid, reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_equip_replace_exattr_reply, reply);
	return true;
}

bool equip_msg_handle_t::handle_cg_equip_smelt_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_equip_smelt_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	std::vector<uint64_t> equip_list;
	for (int32_t i = 0; i < msg.equip_uid_size(); ++i)
	{
		uint64_t equip_uid = string_util_t::string_to_uint64(msg.equip_uid(i));
		equip_list.push_back(equip_uid);
	}
	
	proto::client::gc_equip_smelt_reply reply;
	uint32_t reply_code = equip_manager_t::equip_smelt(p_role, equip_list, reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_equip_smelt_reply, reply);
	return true;
}

bool equip_msg_handle_t::handle_cg_equip_smelt_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_equip_smelt_data_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	
	proto::client::gc_equip_smelt_data_reply reply;
	equip_manager_t::peek_equip_smelt_data(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_equip_smelt_data_reply, reply);
	return true;
}

bool equip_msg_handle_t::handle_cg_equip_smelt_box_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_equip_smelt_box_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}

	proto::client::gc_equip_smelt_box_reply reply;
	uint32_t reply_code = equip_manager_t::equip_smelt_box(p_role, msg.box_id(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_equip_smelt_box_reply, reply);
	return true;
}

bool equip_msg_handle_t::handle_cg_equip_strengthen_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_equip_strengthen_request);

	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	uint64_t equip_uid = string_util_t::string_to_uint64(msg.equip_uid());

	proto::client::gc_equip_strengthen_reply reply;
	uint32_t reply_code = equip_manager_t::equip_strengthen(p_role, equip_uid, msg.count(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_equip_strengthen_reply, reply);
	return true;
}

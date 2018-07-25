#include "field_boss_msg_handle.hpp"
#include "center/center_msg_handler.hpp"
#include "common.pb.h"
#include "server.pb.h"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "field_boss_manager.hpp"
#include "monster/monster_manager.hpp"
#include "field_boss/field_boss.hpp"
#include "gate/gate_msg_handler.hpp"
#include "tblh/errcode_enum.hpp"

bool field_boss_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_summon_field_boss_request, handle_eg_summon_field_boss_request);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_field_boss_die_notify, handle_eg_field_boss_die_notify);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_clear_all_boss_notify, handle_eg_clear_all_boss_notify);

	return bresult;
}

bool field_boss_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_field_boss_request, handle_cg_field_boss_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_field_boss_attack_request, handle_cg_field_boss_attack_request);

	return bresult;
}

bool field_boss_msg_handle_t::handle_eg_summon_field_boss_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_summon_field_boss_request);

	proto::server::ge_summon_field_boss_replay reply;
	if (!field_boss_manager_t::summon_field_boss(msg.id(), msg.scene_id(), msg.monster_id(), msg.object_id(), msg.pos_x(), msg.pos_y(), msg.dir())) {
		log_error("game summon field boss[%lu, %u] in scene[%lu] failed", msg.object_id(), msg.monster_id(), msg.scene_id());
		reply.set_reply_code(1);
	} else {
		BOSS_LOG("game summon field boss[%lu, %u] in scene[%lu] pos[%u, %u] success", msg.object_id(), msg.monster_id(), msg.scene_id(), msg.pos_x(), msg.pos_y());
		reply.set_reply_code(0);
	}

	reply.set_id(msg.id());
	reply.set_object_id(msg.object_id());
	env::server->send_msg_to_center(op_cmd::ge_summon_field_boss_replay, 0, reply);

	return true;
}

bool field_boss_msg_handle_t::handle_eg_field_boss_die_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_field_boss_die_notify);
	
	field_boss_manager_t::delete_field_boss(msg.object_id(), msg.mode());

	return true;
}

bool field_boss_msg_handle_t::handle_eg_clear_all_boss_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_clear_all_boss_notify);

	for (int32_t i = 0; i < msg.object_ids_size(); ++i) {
		field_boss_manager_t::delete_field_boss(msg.object_ids(i), proto::server::FIELD_BOSS_NONE);
	}

	return true;
}

bool field_boss_msg_handle_t::handle_cg_field_boss_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_field_boss_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null", uid);
		return false;
	}

	uint64_t object_id = common::string_util_t::string_to_uint64(msg.object_id());
	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster) {
		log_error("field boss[%lu] can't find", object_id);
		return false;
	}

	field_boss_ptr p_field_boss = p_monster->get_field_boss();
	if (NULL == p_field_boss) {
		log_error("monster[%lu] is not field boss", object_id);
		return false;
	}

	uint32_t cd_time = p_field_boss->get_fight_cd_time(p_role->get_uid());
	proto::client::gc_field_boss_reply reply;
	reply.set_object_id(msg.object_id());
	reply.set_cd_time(cd_time);
	p_role->send_msg_to_client(op_cmd::gc_field_boss_reply, reply);

	return true;
}

bool field_boss_msg_handle_t::handle_cg_field_boss_attack_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_field_boss_attack_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null", uid);
		return false;
	}

	proto::client::gc_field_boss_attack_reply reply;
	if (!field_boss_manager_t::role_attack_field_boss(p_role, common::string_util_t::string_to_uint64(msg.object_id()), reply)) {
		p_role->send_msg_to_client(op_cmd::gc_field_boss_attack_reply, reply);
	}

	return true;
}

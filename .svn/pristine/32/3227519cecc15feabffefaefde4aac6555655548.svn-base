#include "area_msg_handler.hpp"
#include "network.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "hero/hero_manager.hpp"
#include "fight/fight_manager.hpp"
#include "item/item_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::RS_BEGIN, op_cmd::RS_END, area_msg_handler_t::msg_handler_func> area_msg_handler_t::m_area_msg_handle;

bool area_msg_handler_t::init_msg_handler()
{
    bool ret = true;
	// area => game
	ret &= m_area_msg_handle.register_func(op_cmd::rs_syn_area_info_notify, handle_rs_syn_area_info_notify);
	ret &= m_area_msg_handle.register_func(op_cmd::rs_cross_fight_test_notify, handle_rs_cross_fight_test_notify);
	ret &= m_area_msg_handle.register_func(op_cmd::rs_arena_battle_notify, handle_rs_arena_battle_notify);
	ret &= m_area_msg_handle.register_func(op_cmd::rs_arena_battle_ai_notify, handle_rs_arena_battle_ai_notify);

    return ret;
}

bool area_msg_handler_t::handle_rs_syn_area_info_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rs_syn_area_info_notify);
	return true;
}

bool area_msg_handler_t::handle_rs_cross_fight_test_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rs_cross_fight_test_notify);

	role_ptr p_role1 = NULL;
	role_ptr p_role2 = NULL;
	gate_ptr p_gate = gate_manager_t::get_gate_by_id(msg.user1().rd().server().gate(), msg.user1().rd().server().server_id());
	if (NULL != p_gate)
	{
		uint64_t uid1 = string_util_t::string_to_uint64(msg.user1().rd().uid());
		role_manager_t::enter_game(p_gate->get_socket(), uid1);
		p_role1 = role_manager_t::find_role(uid1);
		if (NULL != p_role1)
		{
			p_role1->syn_cross_id(env::server->get_id());
			p_role1->load_cross_data(msg.user1().rd());
			hero_manager_t::load_all_datas(p_role1, msg.user1().hd());
			item_manager_t::load_all_datas(p_role1, msg.user1().item_data());
		}
	}

	p_gate = gate_manager_t::get_gate_by_id(msg.user2().rd().server().gate(), msg.user2().rd().server().server_id());
	if (NULL != p_gate)
	{
		uint64_t uid2 = string_util_t::string_to_uint64(msg.user2().rd().uid());
		role_manager_t::enter_game(p_gate->get_socket(), uid2);
		p_role2 = role_manager_t::find_role(uid2);
		if (NULL != p_role2)
		{
			p_role2->syn_cross_id(env::server->get_id());
			p_role2->load_cross_data(msg.user2().rd());
			hero_manager_t::load_all_datas(p_role2, msg.user2().hd());
			item_manager_t::load_all_datas(p_role2, msg.user2().item_data());
		}
	}
	
	if (NULL != p_role1 && NULL != p_role2)
	{
		fight_manager_t::fight_pvp(p_role1, p_role2, msg.mutable_fight());
	}

	return true;
}

bool area_msg_handler_t::handle_rs_arena_battle_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rs_arena_battle_notify);

	uint64_t uid1 = string_util_t::string_to_uint64(msg.user1().rd().uid());
	uint64_t uid2 = string_util_t::string_to_uint64(msg.user2().rd().uid());

	role_ptr p_role1 = role_manager_t::find_role(uid1);
	role_ptr p_role2 = role_manager_t::find_role(uid2);

	if (p_role1 != NULL )
	{
		log_warn("arena p_role1 != NULL uid[%lu]", uid1);
		return false;
	}
	if (p_role2 != NULL)
	{
		log_warn("arena p_role2 != NULL uid[%lu]", uid2);
		return false;
	}

	gate_ptr p_gate = gate_manager_t::get_gate_by_id(msg.user1().rd().server().gate(), msg.user1().rd().server().server_id());
	if (NULL != p_gate)
	{
		role_manager_t::enter_game(p_gate->get_socket(), uid1);
		p_role1 = role_manager_t::find_role(uid1);
		if (NULL != p_role1)
		{
			p_role1->syn_cross_id(env::server->get_id());
			p_role1->load_cross_data(msg.user1().rd());
			hero_manager_t::load_all_datas(p_role1, msg.user1().hd());
			item_manager_t::load_all_datas(p_role1, msg.user1().item_data());
		}
		else
		{
			log_error("p_role1 NULL uid[%lu]", uid1);
			return false;
		}
	}
	else
	{
		log_error("arena p_gate NULL");
		return false;
	}

	p_gate = gate_manager_t::get_gate_by_id(msg.user2().rd().server().gate(), msg.user2().rd().server().server_id());
	if (NULL != p_gate)
	{
		role_manager_t::enter_game(p_gate->get_socket(), uid2);
		p_role2 = role_manager_t::find_role(uid2);
		if (NULL != p_role2)
		{
			p_role2->syn_cross_id(env::server->get_id());
			p_role2->load_cross_data(msg.user2().rd());
			hero_manager_t::load_all_datas(p_role2, msg.user2().hd());
			item_manager_t::load_all_datas(p_role2, msg.user2().item_data());
		}
		else
		{
			log_error("p_role2 NULL uid[%lu]", uid2);
			return false;
		}
	}
	else
	{
		log_error("arena p_gate NULL");
		return false;
	}

	fight_manager_t::fight_pvp(p_role1, p_role2, msg.mutable_fight());
	

	return true;
}

bool area_msg_handler_t::handle_rs_arena_battle_ai_notify(const network::msg_buf_ptr& msg_buf)
{

	PRE_S2S_MSG(proto::server::rs_arena_battle_ai_notify);

	uint64_t uid1 = string_util_t::string_to_uint64(msg.user1().rd().uid());

	role_ptr p_role1 = role_manager_t::find_role(uid1);

	if (p_role1 != NULL)
	{
		log_warn("arena p_role1 != NULL uid[%lu]", uid1);
		return false;
	}

	gate_ptr p_gate = gate_manager_t::get_gate_by_id(msg.user1().rd().server().gate(), msg.user1().rd().server().server_id());
	if (NULL != p_gate)
	{
		role_manager_t::enter_game(p_gate->get_socket(), uid1);
		p_role1 = role_manager_t::find_role(uid1);
		if (NULL != p_role1)
		{
			p_role1->syn_cross_id(env::server->get_id());
			p_role1->load_cross_data(msg.user1().rd());
			hero_manager_t::load_all_datas(p_role1, msg.user1().hd());
			item_manager_t::load_all_datas(p_role1, msg.user1().item_data());
		}
		else
		{
			log_error("p_role1 NULL uid[%lu]", uid1);
			return false;
		}
	}
	else
	{
		log_error("arena p_gate NULL");
		return false;
	}

	if(msg.npc().has_formation_id())
		fight_manager_t::fight_pve(p_role1, msg.npc().formation_id(), msg.mutable_fight());
	else
		fight_manager_t::fight_pve(p_role1, msg.npc(), msg.mutable_fight());

	return true;
}


#include "global_scene.hpp"
#include "tblh/errcode_enum.hpp"
#include "config_mgr.h"
#include "tblh/MapTable.tbls.h"
#include "user_troop/user_troop_mgr.hpp"

global_scene_t::global_scene_t(uint64_t scene_id, uint32_t map_tid)
	: m_scene_id(scene_id)
	, m_map_tid(map_tid)
{

}

void global_scene_t::set_scene_state(uint32_t set)
{
	if (m_state == set)
	{
		log_warn("scene[%lu] is already state[%u]", m_scene_id, m_state);
		return;
	}

	scene_base::set_scene_state(set);
}

bool global_scene_t::init()
{
	MapTable* p_map_table = GET_CONF(MapTable, m_map_tid);
	if (NULL == p_map_table)
	{
		log_error("center map_table[%u] not find", m_map_tid);
		return false;
	}

	m_scene_type = (proto::server::em_scene_type)p_map_table->get_scene_attr_type();

	return true;
}

uint32_t global_scene_t::check_scene_troop()
{
	switch (m_scene_type)
	{
	case proto::server::em_scene_type_none:
		{
			log_error("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::notice_unknown;
		}
		break;
	case proto::server::em_scene_type_prison:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::user_troop_err_prison;
		}
		break;
	case proto::server::em_scene_type_arena:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::user_troop_err_arena;
		}
		break;
	case proto::server::em_scene_type_dugeon:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::user_troop_err_dungeon;
		}
		break;
	case proto::server::em_scene_type_family_war:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::user_troop_err_family_war_scene;
		}
		break;
	default:
		break;
	}

	return common::errcode_enum::error_ok;
}

uint32_t global_scene_t::check_troop_join()
{
	switch (m_scene_type)
	{
	case proto::server::em_scene_type_none:
		{
			log_error("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::notice_unknown;
		}
		break;
	case proto::server::em_scene_type_prison:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::user_troop_err_prison;
		}
		break;
	case proto::server::em_scene_type_arena:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::user_troop_err_arena;
		}
		break;
	case proto::server::em_scene_type_dugeon:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::user_troop_err_dungeon;
		}
		break;
	case proto::server::em_scene_type_family_war:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return errcode_enum::user_troop_err_family_war_scene;
		}
		break;
	default:
		break;
	}

	return common::errcode_enum::error_ok;
}

void global_scene_t::on_role_enter(const global_user_ptr& p_user)
{
	if (NULL == p_user)
	{
		log_error("p_user NULL error");
		return;
	}

	switch (m_scene_type)
	{
	case proto::server::em_scene_type_prison:
	case proto::server::em_scene_type_arena:
		{
			if (p_user->get_troop_id() != 0)
			{
				proto::client::ec_troop_quit_reply reply;
				uint32_t reply_code = global_troop_manager::quit_troop(p_user);
				reply.set_reply_code(reply_code);
				p_user->send_msg_to_client(op_cmd::ec_troop_quit_reply, reply);
			}
		}
		break;
	default:
		break;
	}
}

bool global_scene_t::is_normal_scene() const
{
	switch (m_scene_type) {
		case proto::server::em_scene_type_emperor:
		case proto::server::em_scene_type_king:
		case proto::server::em_scene_type_normal: {
			return true;
			break;
		}
		default:
			break;
	}

	return false;
}

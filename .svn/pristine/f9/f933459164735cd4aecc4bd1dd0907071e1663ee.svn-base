#include "user_troop_mgr.hpp"
#include "global_id.hpp"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "client.pb.h"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/TroopType.tbls.h"
#include "config_mgr.h"
#include "activity/activity_manager.hpp"
#include "scene/scene_manager.hpp"
#include "tblh/Comprehensive.tbls.h"

game_troop_mgr_t::troop_map game_troop_mgr_t::m_troop_map;

troop_ptr game_troop_mgr_t::get_troop(uint64_t troop_id)
{
	troop_map::iterator itr = m_troop_map.find(troop_id);
	if (itr != m_troop_map.end()) {
		return itr->second;
	}

	return NULL;
}

void game_troop_mgr_t::delete_troop(uint64_t troop_id)
{
	troop_map::iterator itr = m_troop_map.find(troop_id);
	if (itr != m_troop_map.end()) {
		m_troop_map.erase(itr);
	}
}

void game_troop_mgr_t::troop_event(uint64_t uid, proto::server::eg_troop_event_notify& ntf)
{
	if (ntf.event() == proto::common::troop_event_create) {
		troop_ptr p_troop(new troop_t(uid));
		if (p_troop == NULL) {
			log_error("create new_troop null error");
			return;
		}

		std::pair<troop_map::iterator, bool> res = m_troop_map.insert(std::make_pair(uid, p_troop));
		if (!res.second) {
			log_error("user[%lu] create new troop insert error, it is exist", ntf.member_id());
			return;
		}

		p_troop->process_event(ntf.event(), ntf.member_id(), ntf);
	} else {
		troop_map::iterator itr = m_troop_map.find(uid);
		if (itr != m_troop_map.end()) {
			if (itr->second != NULL) {
				itr->second->process_event(ntf.event(), ntf.member_id(), ntf);
			}

			if (ntf.event() == proto::common::troop_event_dissolve) {
				m_troop_map.erase(itr);
			}
		}
	}
}


void game_troop_mgr_t::del_all_troop()
{
	m_troop_map.clear();
}

//////////////////////////////////////////////////////////////////////////
uint32_t game_troop_mgr_t::create_troop(const role_ptr& p_role, const proto::client::cg_troop_create_request& request)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (p_role->get_troop_id() != 0) {
		TROOP_LOG("p_role[%lu] troop id != 0", p_role->get_uid());
		return common::errcode_enum::user_troop_err_code_already_in_troop;
	}

	auto unlock_level = GET_CONF(Comprehensive, comprehensive_common::team_unlock_level);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(unlock_level);
	if (p_role->get_level() < val) {
		TROOP_LOG("p_role[%lu] create troop level[%u] < [%u]", p_role->get_uid(), p_role->get_level(), val);
		return common::errcode_enum::user_troop_create_level_not_enough;
	}

	uint32_t back_code = p_role->can_create_troop();
	if (back_code != common::errcode_enum::error_ok) {
		TROOP_LOG("p_role[%lu] troop state error", p_role->get_uid());
		return back_code;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (p_scene == NULL) {
		log_error("role[%lu] scene not find", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_scene = p_scene->check_scene_troop();
	if (back_scene != common::errcode_enum::error_ok) {
		TROOP_LOG("role[%lu] check scene troop error", p_role->get_uid());
		return back_scene;
	}

	proto::server::ge_troop_create_request ntf;
	ntf.set_type(request.type());
	if (request.has_role_uid()) ntf.set_role_uid(common::string_util_t::string_to_uint64(request.role_uid()));
	env::server->send_msg_to_center(op_cmd::ge_troop_create_request, p_role->get_uid(), ntf);

	return common::errcode_enum::error_ok;
}

uint32_t game_troop_mgr_t::auto_match(const role_ptr& p_role, uint32_t type)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (p_role->get_troop_id() != 0) {
		TROOP_LOG("p_role[%lu] troop id != 0", p_role->get_uid());
		return common::errcode_enum::user_troop_err_code_already_in_troop;
	}

	auto unlock_level = GET_CONF(Comprehensive, comprehensive_common::team_unlock_level);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(unlock_level);
	if (p_role->get_level() < val) {
		TROOP_LOG("p_role[%lu] join troop level[%u] < [%u]", p_role->get_uid(), p_role->get_level(), val);
		return common::errcode_enum::user_troop_join_level_not_enough;
	}

	uint32_t back_code = p_role->can_join_troop();
	if (back_code != common::errcode_enum::error_ok) {
		TROOP_LOG("p_role[%lu] troop state error", p_role->get_uid());
		return back_code;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (p_scene == NULL) {
		log_error("role[%lu] scene not find", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_scene = p_scene->check_scene_troop();
	if (back_scene != common::errcode_enum::error_ok) {
		TROOP_LOG("role[%lu] check scene troop error", p_role->get_uid());
		return back_scene;
	}

	proto::server::ge_troop_auto_match_request request;
	request.set_type(type);
	env::server->send_msg_to_center(op_cmd::ge_troop_auto_match_request, p_role->get_uid(), request);

	return common::errcode_enum::error_ok;
}

uint32_t game_troop_mgr_t::join_troop(const role_ptr& p_role, uint64_t troop_id)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (p_role->get_troop_id() != 0) {
		TROOP_LOG("p_role[%lu] troop id != 0", p_role->get_uid());
		return common::errcode_enum::user_troop_err_code_already_in_troop;
	}

	auto unlock_level = GET_CONF(Comprehensive, comprehensive_common::team_unlock_level);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(unlock_level);
	if (p_role->get_level() < val) {
		TROOP_LOG("p_role[%lu] join troop level[%u] < [%u]", p_role->get_uid(), p_role->get_level(), val);
		return common::errcode_enum::user_troop_join_level_not_enough;
	}

	uint32_t back_code = p_role->can_join_troop();
	if (back_code != common::errcode_enum::error_ok) {
		TROOP_LOG("p_role[%lu] troop state error", p_role->get_uid());
		return back_code;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (p_scene == NULL) {
		log_error("role[%lu] scene not find", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_scene = p_scene->check_scene_troop();
	if (back_scene != common::errcode_enum::error_ok) {
		TROOP_LOG("role[%lu] check scene troop error", p_role->get_uid());
		return back_scene;
	}

	proto::server::ge_troop_join_request request;
	request.set_troop_id(troop_id);
	env::server->send_msg_to_center(op_cmd::ge_troop_join_request, p_role->get_uid(), request);

	return common::errcode_enum::error_ok;
}

uint32_t game_troop_mgr_t::check_process_apply(uint64_t uid)
{
	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] null error", uid);
		return common::errcode_enum::user_troop_err_code_role_offline;
	}

	uint32_t back_code = p_role->can_join_troop();
	if (back_code != common::errcode_enum::error_ok) {
		TROOP_LOG("p_role[%lu] troop state error", p_role->get_uid());
		return back_code;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (p_scene == NULL) {
		log_error("role[%lu] scene not find", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	return p_scene->check_scene_troop();
}

uint32_t game_troop_mgr_t::back_troop(const role_ptr& p_role)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_code = p_role->can_join_troop();
	if (back_code != common::errcode_enum::error_ok) {
		TROOP_LOG("p_role[%lu] troop state error", p_role->get_uid());
		return back_code;
	}

	proto::server::ge_troop_back_request request;
	env::server->send_msg_to_center(op_cmd::ge_troop_back_request, p_role->get_uid(), request);

	return common::errcode_enum::error_ok;
}

void game_troop_mgr_t::process_invite(const role_ptr& p_role, uint64_t troop_id, uint32_t process)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return;
	}

	uint32_t back_code = common::errcode_enum::error_ok;

	do
	{
		if (process != 0) {
			break;
		}

		if (p_role->get_troop_id() != 0) {
			TROOP_LOG("p_role[%lu] troop id != 0", p_role->get_uid());
			back_code = common::errcode_enum::user_troop_err_code_already_in_troop;
			break;
		}

		back_code = p_role->can_join_troop();
		if (back_code != common::errcode_enum::error_ok) {
			TROOP_LOG("p_role[%lu] troop state error", p_role->get_uid());
			break;
		}

		const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
		if (p_scene == NULL) {
			log_error("role[%lu] scene not find", p_role->get_uid());
			back_code = common::errcode_enum::notice_unknown;
			break;
		}

		back_code = p_scene->check_scene_troop();
	} while (0);

	proto::server::ge_troop_process_invite_request request;
	request.set_troop_id(troop_id);
	request.set_process(process);
	request.set_game_code(back_code);
	env::server->send_msg_to_center(op_cmd::ge_troop_process_invite_request, p_role->get_uid(), request);
}

uint32_t game_troop_mgr_t::troop_transmission_reading_end(role_ptr p_role)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	troop_ptr p_troop = get_troop(p_role->get_troop_id());
	if (p_troop == NULL) {
		log_error("role[%lu] troop[%lu] not find", p_role->get_uid(), p_role->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_in_troop;
	}

	return p_troop->do_transmission_reading_end(p_role);
}

uint32_t game_troop_mgr_t::accept_zhuagui_task(const role_ptr& p_role, uint32_t task_id)
{
	if (p_role == NULL) 
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("user:[%lu] task_mgr is null", p_role->get_uid() );
		return common::errcode_enum::notice_unknown;
	}

	task_ptr p_task = p_task_mgr->get_task(task_id);

	proto::server::ge_troop_sync_task_request request;
	request.set_troop_id(p_role->get_troop_id());
	p_task->peek_data(request.mutable_task_info());
	env::server->send_msg_to_center(op_cmd::ge_troop_sync_task_request, p_role->get_uid(), request);
	return common::errcode_enum::error_ok;
}

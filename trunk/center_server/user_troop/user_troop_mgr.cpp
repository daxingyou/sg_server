#include "user_troop_mgr.hpp"
#include "global_id.hpp"
#include "client.pb.h"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/TroopType.tbls.h"
#include "tblh/Activities.tbls.h"
#include "config_mgr.h"
#include "scene/global_scene_manager.hpp"
#include "offline_fight/offline_fight_manager.hpp"
#include "redis/global_user_data_mgr.hpp"

global_troop_manager::troop_map global_troop_manager::m_troop_map;

global_troop_manager::troop_type_wmap global_troop_manager::m_troop_type_wmap;

uint64_t global_troop_manager::get_new_troop_uid()
{
	return g_id_allocator.gen_new_id(ID_ALLOCATOR_TYPE_TROOP);
}

bool global_troop_manager::check_type(uint32_t type)
{
	// 默认类型判断
	if (type == TROOP_TYPE_DEFAULT) return true;

	if (type == ALL_TROOP_TYPE) {
		log_error("troop type[%u] check error", type);
		return false;
	}

	TroopType* conf = GET_CONF(TroopType, type);
	if (conf == NULL) {
		log_error("troop type[%u] not find", type);
		return false;
	}

	return true;
}

uint32_t global_troop_manager::get_type_min_level(uint32_t type)
{
	auto p_troop_type_conf = GET_CONF(TroopType, type);
	if (NULL == p_troop_type_conf) {
		//log_error("NULL == p_troop_type_conf[%d]", type);
		return 1;
	}

	auto p_conf = GET_CONF(Activities, p_troop_type_conf->activity_id());
	if (NULL == p_conf) {
		//log_error("NULL == p_conf[%d]", p_troop_type_conf->activity_id());
		return 1;
	}

	return p_conf->level();
}

uint32_t global_troop_manager::get_max_level(uint32_t type)
{
	auto p_troop_type_conf = GET_CONF(TroopType, type);
	if (NULL == p_troop_type_conf) {
		//log_error("NULL == p_troop_type_conf[%d]", type);
		return 150;
	}

	auto p_conf = GET_CONF(Activities, p_troop_type_conf->activity_id());
	if (NULL == p_conf) {
		//log_error("NULL == p_conf[%d]", p_troop_type_conf->activity_id());
		return 150;
	}

	return p_conf->maxlevel();
}


troop_ptr global_troop_manager::get_troop(uint64_t troop_uid)
{
	troop_map::iterator itr = m_troop_map.find(troop_uid);
	if (itr != m_troop_map.end()) return itr->second;
	return NULL;
}


void global_troop_manager::del_all_troop()
{
	m_troop_type_wmap.clear();
	m_troop_map.clear();
}

uint32_t global_troop_manager::create_new_troop(const global_user_ptr& p_user, uint32_t type)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_unknown;
	}

	if (p_user->get_troop_id() != 0) {
		TROOP_LOG("user[%lu] troop id != 0", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_already_in_troop;
	}

	if (!check_type(type)) {
		log_error("user[%lu] check troop type error", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
	if (NULL == p_scene) {
		log_error("user[%lu] scene not find", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_scene = p_scene->check_scene_troop();
	if (back_scene != common::errcode_enum::error_ok) {
		TROOP_LOG("user[%lu] check scene troop error", p_user->get_uid());
		return back_scene;
	}

	uint64_t new_troop_uid = get_new_troop_uid();
	troop_ptr p_new_troop(new troop_t(new_troop_uid, p_user->get_uid()));
	if (p_new_troop == NULL) {
		log_error("user[%lu] p_new_troop null error", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	p_new_troop->set_type(type);
	p_new_troop->set_level_limit(get_type_min_level(type), get_max_level(type));
	std::pair<troop_map::iterator, bool> res = m_troop_map.insert(std::make_pair(new_troop_uid, p_new_troop));
	if (!res.second) {
		log_error("user[%lu] create new troop insert error, it is exist", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	// 加入队伍
	p_new_troop->add_member(p_user, proto::common::troop_state_normal);

	// 保存一个弱指针
	m_troop_type_wmap[type][new_troop_uid] = p_new_troop;

	//通知消息
	p_new_troop->sync_to_all_game(proto::common::troop_event_create, p_user->get_uid());

	return errcode_enum::error_ok;
}

uint32_t global_troop_manager::join_troop(const global_user_ptr& p_user, uint64_t troop_id)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_unknown;
	}

	if (p_user->get_troop_id() != 0) {
		TROOP_LOG("user[%lu] troop id != 0", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_already_in_troop;
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
	if (NULL == p_scene) {
		log_error("user[%lu] scene not find", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_scene = p_scene->check_scene_troop();
	if (back_scene != common::errcode_enum::error_ok) {
		TROOP_LOG("user[%lu] check scene troop error", p_user->get_uid());
		return back_scene;
	}

	troop_ptr p_troop = get_troop(troop_id);
	if (NULL == p_troop) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_exsit;
	}

	return p_troop->ask_join(p_user);
}

uint32_t global_troop_manager::process_apply(const global_user_ptr& p_user, uint64_t member_uid, uint32_t process)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_unknown;
	}

	troop_ptr p_troop = get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_exsit;
	}

	if (p_troop->get_leader_id() != p_user->get_uid()) {
		log_error("user[%lu] not leader", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_no_right;
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
	if (NULL == p_scene) {
		log_error("user[%lu] scene not find", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_scene = p_scene->check_troop_join();
	if (back_scene != common::errcode_enum::error_ok) {
		TROOP_LOG("user[%lu] scene[%lu] can't join member", p_user->get_uid(), p_user->get_scene_id());
		return back_scene;
	}

	return p_troop->process_apply(member_uid, process);
}

uint32_t global_troop_manager::check_agree_success(const global_user_ptr& p_user, uint64_t member_uid)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_unknown;
	}

	troop_ptr p_troop = get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_exsit;
	}

	if (p_troop->get_leader_id() != p_user->get_uid()) {
		log_error("user[%lu] not leader", p_user->get_uid());
		return errcode_enum::user_troop_err_code_no_right;
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
	if (NULL == p_scene) {
		log_error("user[%lu] scene not find", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_scene = p_scene->check_troop_join();
	if (back_scene != common::errcode_enum::error_ok) {
		TROOP_LOG("user[%lu] scene[%lu] can't join member", p_user->get_uid(), p_user->get_scene_id());
		return back_scene;
	}

	const global_user_ptr& p_member_user = global_user_manager_t::get_global_user_by_uid(member_uid);
	if (NULL == p_member_user) {
		TROOP_LOG("null p_member_user[%lu]", member_uid);
		return common::errcode_enum::user_troop_err_code_role_offline;
	}

	if (p_member_user->get_troop_id() != 0) {
		TROOP_LOG("user[%lu] apply member[%lu] already in troop", p_user->get_uid(), member_uid);
		return common::errcode_enum::user_troop_err_code_already_in_troop;
	}

	return p_troop->agree_apply(p_member_user);
}

uint32_t global_troop_manager::quit_troop(const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_unknown;
	}

	if (p_user->get_troop_id() == 0) { // 没有队伍也算退队成功
		return common::errcode_enum::error_ok;
	}

	troop_map::iterator itr = m_troop_map.find(p_user->get_troop_id());
	if (itr == m_troop_map.end()) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_exsit;
	}

	const troop_ptr& p_troop = itr->second;
	if (NULL == p_troop) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_exsit;
	}

	// do remove
	uint32_t back_code = p_troop->quit_member(p_user);
	if (back_code == common::errcode_enum::error_ok) {
		// 队伍中一个人都没有
		if (p_troop->is_empty()) {
			if (m_troop_type_wmap.find(p_troop->get_type()) != m_troop_type_wmap.end()) {
				m_troop_type_wmap[p_troop->get_type()].erase(p_troop->get_uid());
			}

			m_troop_map.erase(itr);
		}
	}

	return back_code;
}

uint32_t global_troop_manager::kick_troop(const global_user_ptr& p_user, uint64_t member_uid)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_unknown;
	}

	troop_ptr p_troop = get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_exsit;
	}

	if (p_troop->get_leader_id() != p_user->get_uid()) {
		log_error("user[%lu] not troop leader", p_user->get_uid());
		return errcode_enum::user_troop_err_code_no_right;
	}

	if (p_user->get_uid() == member_uid) {
		log_error("user[%lu] can't kick self", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	// do remove
	return p_troop->kick_member(member_uid);
}


void global_troop_manager::auto_match(const global_user_ptr& p_user, uint32_t type)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return;
	}

	if (p_user->get_troop_id() != 0) {
		TROOP_LOG("user[%lu] troop id != 0", p_user->get_uid());
		return;
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
	if (NULL == p_scene) {
		log_error("user[%lu] scene not find", p_user->get_uid());
		return;
	}

	if (p_scene->check_scene_troop() != common::errcode_enum::error_ok) {
		TROOP_LOG("user[%lu] check scene troop error", p_user->get_uid());
		return;
	}

	if (m_troop_type_wmap.find(type) == m_troop_type_wmap.end() || m_troop_type_wmap[type].empty()) return;

	for (troop_wmap::iterator itr = m_troop_type_wmap[type].begin();
		itr != m_troop_type_wmap[type].end(); ++itr) {
		troop_ptr p_troop = itr->second.lock();
		if (p_troop != NULL) {
			p_troop->ask_join(p_user);
			if (p_user->get_troop_id() != 0) {
				TROOP_LOG("user[%lu] auto_match troop", p_user->get_uid());
				return;
			}
		}
	}
}


uint32_t global_troop_manager::modify_troop(const global_user_ptr& p_user, uint32_t new_type, uint32_t level_min, uint32_t level_max)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_role_null;
	}

	troop_ptr p_troop = get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("user[%lu] troop[%lu] not find", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_in_troop;
	}

	if (p_troop->get_leader_id() != p_user->get_uid()) {
		log_error("user[%lu] not leader", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_no_right;
	}

	if (!check_type(new_type)) {
		log_error("user[%lu] troop type[%d] invalid", p_user->get_uid(), new_type);
		return common::errcode_enum::notice_unknown;
	}

	uint32_t real_level_min = level_min;
	uint32_t real_level_max = level_max;
	if (level_min > level_max) {
		real_level_min = level_max;
		real_level_max = level_min;
	}

	uint32_t type_min_level = get_type_min_level(new_type);
	uint32_t max_level = get_max_level(new_type);
	if (real_level_min < type_min_level) real_level_min = type_min_level;
	if (real_level_max > max_level) real_level_max = max_level;

	uint32_t old_type = p_troop->get_type();
	p_troop->set_type(new_type);
	p_troop->set_level_limit(real_level_min, real_level_max);

	if (old_type != new_type) {
		if (m_troop_type_wmap.find(old_type) != m_troop_type_wmap.end()) {
			m_troop_type_wmap[old_type].erase(p_troop->get_uid());
		}
		m_troop_type_wmap[new_type][p_troop->get_uid()] = p_troop;
	}

	p_troop->sync_to_all_game(proto::common::troop_event_modify, p_user->get_uid());
	return common::errcode_enum::error_ok;
}


uint32_t global_troop_manager::dissolve_troop(const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_role_null;
	}

	troop_map::iterator itr = m_troop_map.find(p_user->get_troop_id());
	if (itr == m_troop_map.end()) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_exsit;
	}

	const troop_ptr& p_troop = itr->second;
	if (NULL == p_troop) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_not_exsit;
	}

	if (p_troop->get_leader_id() != p_user->get_uid()) {
		log_error("user[%lu] not leader", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_no_right;
	}

	p_troop->do_dissolve();

	if (m_troop_type_wmap.find(p_troop->get_type()) != m_troop_type_wmap.end()) {
		m_troop_type_wmap[p_troop->get_type()].erase(p_troop->get_uid());
	}

	m_troop_map.erase(itr);

	return common::errcode_enum::error_ok;
}

void global_troop_manager::get_troop_list(const global_user_ptr& p_user, uint32_t type, proto::client::ec_troop_list_reply& reply)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return;
	}

	uint32_t role_level = p_user->get_level();

	if (ALL_TROOP_TYPE == type) { //全部活动
		for (troop_map::const_iterator citr = m_troop_map.begin();
			citr != m_troop_map.end(); ++citr)
		{
			const troop_ptr& p_troop = citr->second;
			if (NULL == p_troop) continue;
			if (TROOP_TYPE_DEFAULT == p_troop->get_type()) continue;
			else if (EXPEDITION_HELP_TROOP_TYPE == p_troop->get_type()) continue;
			
			if (role_level < p_troop->get_level_min() || role_level > p_troop->get_level_max()) continue;
			p_troop->peek_data(reply.add_troops());
		}
	} else { // 特定
		if (m_troop_type_wmap.find(type) != m_troop_type_wmap.end()) {
			for (troop_wmap::iterator itr = m_troop_type_wmap[type].begin();
				itr != m_troop_type_wmap[type].end(); ++itr) {
				troop_ptr p_troop = itr->second.lock();
				if (p_troop != NULL) {
					if (role_level < p_troop->get_level_min() || role_level > p_troop->get_level_max()) continue;
					p_troop->peek_data(reply.add_troops());
				}
			}
		}
	}
}


uint32_t global_troop_manager::troop_invite_member(const global_user_ptr& p_user, uint64_t invite_uid)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_unknown;
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
	if (NULL == p_scene) {
		log_error("user[%lu] scene not find", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	// 场景检测
	uint32_t user_scene_check = p_scene->check_troop_join();
	if (user_scene_check != common::errcode_enum::error_ok) {
		return user_scene_check;
	}

	troop_ptr p_troop = get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::notice_unknown;
	}

	if (p_troop->is_full()) {
		TROOP_LOG("troop[%lu] is full", p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_full;
	}

	const global_user_ptr& p_member_user = global_user_manager_t::get_global_user_by_uid(invite_uid);
	if (NULL == p_member_user) {
		TROOP_LOG("user[%lu] invite_join null user[%lu]", p_user->get_uid(), invite_uid);
		return common::errcode_enum::notice_role_null;
	}

	if (p_member_user->get_troop_id() != 0) {
		TROOP_LOG("user[%lu] invite member failed, invite member[%lu] already in troop", p_user->get_uid(), invite_uid);
		return common::errcode_enum::user_troop_err_code_already_in_troop;
	}

	auto unlock_level = GET_CONF(Comprehensive, comprehensive_common::team_unlock_level);
	uint32_t team_unlock_level = GET_COMPREHENSIVE_VALUE_1(unlock_level);
	if (p_member_user->get_level() < team_unlock_level) {
		TROOP_LOG("user[%lu] level[%d] not enough, need[%d]", p_member_user->get_uid(), p_member_user->get_level(), team_unlock_level);
		return common::errcode_enum::user_troop_invite_level_not_enough;
	}

	if (!p_member_user->is_online()) {
		TROOP_LOG("user[%lu] invite_join user[%lu] not online", p_user->get_uid(), p_member_user->get_uid());
		return common::errcode_enum::user_troop_err_code_role_offline;
	}

	const global_scene_ptr& p_member_scene = global_scene_manager_t::get_scene_by_id(p_member_user->get_scene_id());
	if (NULL == p_member_scene) {
		TROOP_LOG("user[%lu] scene not find", p_member_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t scene_check = p_member_scene->check_troop_join();
	if (p_member_scene->check_troop_join() != common::errcode_enum::error_ok) {
		return scene_check;
	}

	p_troop->invite_join(p_user, p_member_user);

	return common::errcode_enum::error_ok;
}


uint32_t global_troop_manager::troop_invite_shadow(const global_user_ptr& p_user, uint64_t friend_uid)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_unknown;
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
	if (NULL == p_scene) {
		log_error("user[%lu] scene not find", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	// 场景检测
	uint32_t user_scene_check = p_scene->check_troop_join();
	if (user_scene_check != common::errcode_enum::error_ok) {
		return user_scene_check;
	}

	troop_ptr p_troop = get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::notice_unknown;
	}

	if (p_troop->is_full()) {
		TROOP_LOG("user[%lu] troop[%lu] is full", p_user->get_uid(), p_user->get_troop_id());
		return common::errcode_enum::user_troop_err_code_full;
	}

	auto unlock_level = GET_CONF(Comprehensive, comprehensive_common::team_unlock_level);
	uint32_t team_unlock_level = GET_COMPREHENSIVE_VALUE_1(unlock_level);
	uint32_t friend_lv = global_user_data_mgr_t::get_user_level(friend_uid);
	if (friend_lv < team_unlock_level) {
		TROOP_LOG("user[%lu] level[%d] not enough, need[%d]", friend_uid, friend_lv, team_unlock_level);
		return common::errcode_enum::user_troop_invite_level_not_enough;
	}

	if (p_troop->is_shadow_joined(friend_uid)) {
		TROOP_LOG("user[%lu] shadow[%lu] is already joined", p_user->get_uid(), friend_uid);
		return common::errcode_enum::user_troop_shadow_already_in_team;
	}

	uint32_t back_code = p_user->get_friend_mgr().summon_shadow_request(friend_uid);
	if (back_code != common::errcode_enum::error_ok) {
		return back_code;
	}

	if (!p_troop->add_user_shadow(friend_uid)) {
		log_error("user[%lu] add shadow friend[%lu] error", p_user->get_uid(), friend_uid);
		return common::errcode_enum::notice_unknown;
	}

	friend_info_ptr p_friend = p_user->get_friend_mgr().get_friend_by_id(friend_uid);
	proto::server::eg_troop_invite_shadow_notify notify;
	proto::common::friend_member* p_friend_member = notify.mutable_member_info();
	if (p_friend_member) {
		p_friend_member->CopyFrom(p_friend->m_member);
	}
	p_user->send_msg_to_game(op_cmd::eg_troop_invite_shadow_notify, notify);

	return common::errcode_enum::error_ok;
}

void global_troop_manager::troop_invite_join(const global_user_ptr& p_user, const proto::client::ce_troop_invite_join_request& request)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return;
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
	if (NULL == p_scene) {
		log_error("user[%lu] scene not find", p_user->get_uid());
		return;
	}

	// 场景检测
	if (p_scene->check_troop_join() != common::errcode_enum::error_ok) return;

	troop_ptr p_troop = get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return;
	}

	if (request.is_invite_tutor()) { // 有导师优先邀请导师
		p_troop->add_tutor(p_user);
	}

	// 现在队员也能邀请
	//if (p_troop->get_leader_id() != p_user->get_uid())
	//{
	//	log_error("role[%lu] not leader", p_user->get_uid());
	//	return errcode_enum::user_troop_err_code_no_right;
	//}

	if (p_troop->is_full()) {
		TROOP_LOG("troop[%lu] is full", p_user->get_troop_id());
		return;
	}

	auto unlock_level = GET_CONF(Comprehensive, comprehensive_common::team_unlock_level);
	uint32_t team_unlock_level = GET_COMPREHENSIVE_VALUE_1(unlock_level);

	for (int32_t i = 0; i < request.role_id_size(); ++i) {
		uint64_t invite_uid = common::string_util_t::string_to_uint64(request.role_id(i));
		const global_user_ptr& p_member_user = global_user_manager_t::get_global_user_by_uid(invite_uid);
		if (NULL == p_member_user) {
			TROOP_LOG("user[%lu] invite_join null user[%lu]", p_user->get_uid(), invite_uid);
			continue;
		}

		if (p_member_user->get_troop_id() != 0) {
			TROOP_LOG("user[%lu] agree invite member failed, invite member[%lu] already in troop", p_user->get_uid(), invite_uid);
			continue;
		}

		if (p_member_user->get_level() < team_unlock_level) {
			TROOP_LOG("user[%lu] level[%d] not enough, need[%d]", p_member_user->get_uid(), p_member_user->get_level(), team_unlock_level);
			continue;
		}

		if (!p_member_user->is_online()) {
			TROOP_LOG("user[%lu] invite_join user[%lu] not online", p_user->get_uid(), p_member_user->get_uid());
			continue;
		}

		const global_scene_ptr& p_member_scene = global_scene_manager_t::get_scene_by_id(p_member_user->get_scene_id());
		if (NULL == p_member_scene) {
			TROOP_LOG("user[%lu] scene not find", p_member_user->get_uid());
			continue;
		}

		if (p_member_scene->check_troop_join() != common::errcode_enum::error_ok) continue;

		p_troop->invite_join(p_user, p_member_user);
	}
}

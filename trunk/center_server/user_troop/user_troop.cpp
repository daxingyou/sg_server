#include "user_troop.hpp"
#include "user_troop_mgr.hpp"
#include "log_enum.hpp"
#include "tblh/errcode_enum.hpp"
#include "scene/global_scene_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"
#include "game_server/task/task_manager.hpp"
#include "global_id.hpp"
#include "tblh/TutorTable.tbls.h"
#include "redis/global_user_data_mgr.hpp"
#include "family/family_manager.hpp"
#include "look_role_info/simple_data_manager.hpp"

troop_t::troop_t(uint64_t troop_uid, uint64_t leader_uid)
	: m_uid(troop_uid)
	, m_leader_id(leader_uid)
{
	
}


void troop_t::sync_to_all_game(uint32_t event_id, uint64_t member_id)
{
	proto::server::eg_troop_event_notify ntf;
	ntf.set_event(event_id);
	ntf.set_member_id(member_id);
	peek_data(event_id, member_id, ntf.mutable_troop());
	env::server->send_msg_to_all_games(op_cmd::eg_troop_event_notify, m_uid, ntf); // uid是队伍id
}

void troop_t::notify_event(uint32_t event_id, const std::string& member_name)
{
	proto::client::ec_troop_event_notify ntf;
	ntf.set_event(event_id);
	ntf.set_member_name(member_name);
	if (event_id != proto::common::troop_event_dissolve) {
		peek_data(ntf.mutable_troop(), false);
	}
	send_msg_to_all(op_cmd::ec_troop_event_notify, ntf);
}

void troop_t::peek_data(uint32_t event_id, uint64_t member_id, proto::server::troop_server_data* p_data)
{
	if (NULL == p_data) return;

	switch (event_id) {
		case proto::common::troop_event_create: { // 这些需要把全部队员信息下发
			proto::common::troop_data* td = p_data->mutable_base();
			if (!td) return;

			td->set_type(m_type);
			td->set_is_free_join(m_is_free_join);
			td->set_level_min(m_level_min);
			td->set_level_max(m_level_max);

			for (const auto& p_member : m_troop_members) {
				if (NULL != p_member) p_member->peek_data(td->add_members());
			}
			break;
		}
		case proto::common::troop_event_modify: {
			proto::common::troop_data* td = p_data->mutable_base();
			if (!td) return;

			td->set_type(m_type);
			td->set_is_free_join(m_is_free_join);
			td->set_level_min(m_level_min);
			td->set_level_max(m_level_max);
			break;
		}
		case proto::common::troop_event_join: {
			proto::common::troop_data* td = p_data->mutable_base();
			if (!td) return;

			if (m_zg_task_info.id() != 0) peek_zg_task_data(td->mutable_zg_task_info());

			troop_member_ptr p_member = get_troop_member(member_id);
			if (p_member != NULL) {
				p_member->peek_data(td->add_members());

				if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER) {
					p_member->peek_extra_data(p_data->mutable_extra_data());
				}
			}
			break;
		}
		case proto::common::troop_event_back:
		case proto::common::troop_event_leader: {
			proto::common::troop_data* td = p_data->mutable_base();
			if (!td) return;

			if (m_zg_task_info.id() != 0) peek_zg_task_data(td->mutable_zg_task_info());

			troop_member_ptr p_member = get_troop_member(member_id);
			if (p_member != NULL) {
				p_member->peek_data(td->add_members());
			}
			break;
		}
		case proto::common::troop_event_update:
		case proto::common::troop_event_online: { // 只需要发变化的
			proto::common::troop_data* td = p_data->mutable_base();
			if (!td) return;

			troop_member_ptr p_member = get_troop_member(member_id);
			if (p_member != NULL) {
				p_member->peek_data(td->add_members());
			}
			break;
		}
		case proto::common::troop_event_zhuagui_task: {
			proto::common::troop_data* td = p_data->mutable_base();
			if (!td) return;

			if (m_zg_task_info.id() != 0) peek_zg_task_data(td->mutable_zg_task_info());
		}
		default:
			break;
	}
}


void troop_t::peek_data(proto::common::troop_data* p_data, bool is_to_server/* = true*/)
{
	if (NULL == p_data) return;

	p_data->set_id(common::string_util_t::uint64_to_string(m_uid));
	p_data->set_type(m_type);
	p_data->set_is_free_join(m_is_free_join);
	p_data->set_leader(common::string_util_t::uint64_to_string(m_leader_id));
	p_data->set_level_min(m_level_min);
	p_data->set_level_max(m_level_max);

	if (is_to_server && m_zg_task_info.id() != 0) peek_zg_task_data(p_data->mutable_zg_task_info());

	uint32_t fighting = 0;
	for (const auto& p_member : m_troop_members) {
		if (NULL != p_member) {
			p_member->peek_data(p_data->add_members());
			fighting += p_member->get_fighting();
		}
	}
	p_data->set_fighting(fighting);
}

void troop_t::peek_apply_member_data(proto::client::ec_troop_apply_member_reply& reply)
{
	for (const auto& p_member : m_apply_members) {
		if (NULL != p_member) p_member->peek_data(reply.add_apply_list());
	}
}

void troop_t::peek_zg_task_data(proto::common::task_state *p_data)
{
	if (p_data)
		p_data->CopyFrom(m_zg_task_info);
}

bool troop_t::add_member(const global_user_ptr& p_user, proto::common::troop_state_type type)
{
	if (p_user == NULL) {
		log_error("p_user null error");
		return false;
	}

	if (is_full()) {
		log_error("troop[%lu] is full", m_uid);
		return false;
	}

	troop_member_ptr p_member(new troop_member(p_user->get_uid()));
	if (p_member == NULL) {
		log_error("p_member null error");
		return false;
	}

	p_member->set_object_type(proto::common::SCENEOBJECT_USER);
	p_member->set_name(p_user->get_name());
	p_member->set_plugin(p_user->get_hero_plugin());
	p_member->set_fighting(p_user->get_fighting_value());
	p_member->set_level(p_user->get_level());
	p_member->set_state(type);
	m_troop_members.push_back(p_member);

	// 设置人物队伍id
	p_user->set_troop_id(m_uid);

	return true;
}

troop_member_ptr troop_t::get_troop_member(uint64_t member_uid)
{
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_uid() == member_uid) {
			return m_troop_members[i];
		}
	}

	return NULL;
}

uint32_t troop_t::kick_member(uint64_t member_uid)
{
	troop_member_vec::iterator itr = std::find_if(m_troop_members.begin(), m_troop_members.end(),
		[member_uid](const troop_member_ptr& p_member)
	{
		if (NULL != p_member && p_member->get_uid() == member_uid) return true;
		return false;
	});

	if (itr == m_troop_members.end()) {
		log_error("user[%lu] not in troop[%lu]", member_uid, get_uid());
		return common::errcode_enum::user_troop_err_code_not_in_troop;
	}

	troop_member_ptr p_member = *itr;
	// 队伍中删除
	m_troop_members.erase(itr);

	if (p_member && p_member->get_object_type() == proto::common::SCENEOBJECT_USER) {
		global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(member_uid);
		if (p_user) {
			// 设置人物队伍id
			p_user->set_troop_id(0);
		}

		// 刪除宠物
		remove_member_partner(member_uid);
	}

	sync_to_all_game(proto::common::troop_event_kick, member_uid);

	return common::errcode_enum::error_ok;
}

uint32_t troop_t::quit_member(const global_user_ptr& p_member_user)
{
	if (p_member_user == NULL) {
		log_error("p_member_user null error");
		return common::errcode_enum::notice_unknown;
	}

	uint64_t member_uid = p_member_user->get_uid();
	troop_member_vec::iterator itr = std::find_if(m_troop_members.begin(), m_troop_members.end(),
		[member_uid](const troop_member_ptr& p_member)
	{
		if (NULL != p_member && p_member->get_uid() == member_uid) return true;
		return false;
	});

	if (itr == m_troop_members.end()) {
		log_error("user[%lu] not in troop[%lu]", member_uid, get_uid());
		return common::errcode_enum::user_troop_err_code_not_in_troop;
	}

	// 设置人物队伍id
	p_member_user->set_troop_id(0);
	// 队伍中删除
	m_troop_members.erase(itr);
	// 刪除宠物
	remove_member_partner(member_uid);

	if (is_empty()) { //如果队伍没人了就解散
		sync_to_all_game(proto::common::troop_event_dissolve, member_uid);
	} else {
		sync_to_all_game(proto::common::troop_event_quit, member_uid);
		if (member_uid == m_leader_id) { //如果队长退出重新任命
			if (!random_new_leader()) { // 失败强行解散队伍
				m_leader_id = 0;
				do_dissolve();
				m_troop_members.clear();
			}
		}
	}

	return common::errcode_enum::error_ok;
}

void troop_t::remove_member_partner(uint64_t uid)
{
	for (troop_member_vec::iterator itr = m_troop_members.begin(); itr != m_troop_members.end();) {
		const troop_member_ptr& p_member = *itr;
		if (p_member && p_member->get_object_type() != proto::common::SCENEOBJECT_USER && p_member->get_owner_uid() == uid) {
			uint64_t member_uid = p_member->get_uid();
			itr = m_troop_members.erase(itr);
			sync_to_all_game(proto::common::troop_event_quit, member_uid);
		} else {
			++itr;
		}
	}
}


void troop_t::remove_shadow_member(uint64_t member_uid)
{
	troop_member_vec::iterator itr = std::find_if(m_troop_members.begin(), m_troop_members.end(),
		[member_uid](const troop_member_ptr& p_member)
	{
		if (NULL != p_member && p_member->get_uid() == member_uid) return true;
		return false;
	});

	if (itr == m_troop_members.end()) {
		log_error("member[%lu] not in troop[%lu]", member_uid, get_uid());
		return;
	}

	troop_member_ptr p_member = *itr;
	// 队伍中删除
	m_troop_members.erase(itr);

	// 由于可能找不到玩家所以在中心服做通知
	if (p_member)
		notify_event(proto::common::troop_event_quit, p_member->get_name());
	// 同步game
	sync_to_all_game(proto::common::troop_event_shadow_quit, member_uid);
}

uint64_t troop_t::get_member_tutor(uint64_t member_uid)
{
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) { 
		if (m_troop_members[i] != NULL && m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_TUTOR && member_uid == m_troop_members[i]->get_owner_uid()) {
			return m_troop_members[i]->get_uid();
		}
	}

	return 0;
}

bool troop_t::add_tutor(const global_user_ptr& p_user)
{
	if (p_user == NULL) {
		log_error("p_user null error");
		return false;
	}

	if (is_full()) {
		log_error("user[%lu] troop[%lu] is full", p_user->get_uid(), m_uid);
		return false;
	}

	if (get_member_tutor(p_user->get_uid()) > 0) {
		TROOP_LOG("user[%lu] tutor is already joined", p_user->get_uid());
		return false;
	}

	// 暂时默认为1
	TutorTable* p_conf = GET_CONF(TutorTable, 1);
	if (!p_conf) {
		log_error("tutor[%u] not find", 1);
		return false;
	}

	troop_member_ptr p_member(new troop_member(common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_PARTNER)));
	if (p_member == NULL) {
		log_error("p_member null error");
		return false;
	}

	p_member->set_object_type(proto::common::SCENEOBJECT_TUTOR);
	p_member->set_name(common::string_util_t::convert_to_utf8(p_conf->name()));
	p_member->set_plugin((uint32_t)(p_conf->head_id() << 16));
	p_member->set_fighting(p_conf->power());
	p_member->set_level(p_user->get_level());
	p_member->set_state(proto::common::troop_state_normal);
	p_member->set_tutor_data(p_conf->id(), p_user->get_uid());
	m_troop_members.push_back(p_member);

	// 同步新队员加入
	sync_to_all_game(proto::common::troop_event_join, p_member->get_uid());

	return true;
}


bool troop_t::is_shadow_joined(uint64_t uid)
{
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (m_troop_members[i] != NULL && m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USERSHADOW && uid == m_troop_members[i]->get_owner_uid()) {
			return true;
		}
	}

	return false;
}


bool troop_t::add_user_shadow(uint64_t uid)
{
	if (is_full()) {
		log_error("user[%lu] troop[%lu] is full", uid, m_uid);
		return false;
	}

	if (!global_user_data_mgr_t::is_user_key_exsit(uid)) {
		log_error("user[%lu] not find in redis", uid);
		return false;
	}

	troop_member_ptr p_member(new troop_member(common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_PARTNER)));
	if (p_member == NULL) {
		log_error("p_member null error");
		return false;
	}

	p_member->m_shadow_base.set_role_uid(uid);
	if (!simple_data_manager_t::load_simle_info_from_redis(uid, *(p_member->m_shadow_base.mutable_blob()), proto::common::simple_info_type)) {
		log_error("load user[%lu] simple data error", uid);
		return false;
	}

	uint32_t country_id = global_user_data_mgr_t::get_user_country_id(uid);
	if (country_id != 0) {
		p_member->m_shadow_base.set_country(country_id);
		p_member->m_shadow_base.set_country_officer(global_user_data_mgr_t::get_user_country_officer(uid));
	}

	p_member->m_shadow_base.set_plugin_modle(global_user_data_mgr_t::get_user_plugin(uid));
	uint64_t family_id = global_user_data_mgr_t::get_user_family_id(uid);
	if (family_id != 0) {
		family_ptr p_family = family_manager_t::get_family(family_id);
		if (p_family) {
			p_member->m_shadow_base.set_family_id(family_id);
			p_member->m_shadow_base.set_family_name(p_family->get_name());
			p_member->m_shadow_base.set_family_officer(global_user_data_mgr_t::get_user_family_officer(uid));
		}
	}

	p_member->set_object_type(proto::common::SCENEOBJECT_USERSHADOW);
	p_member->set_name(global_user_data_mgr_t::get_name(uid));
	p_member->set_plugin(global_user_data_mgr_t::get_user_hero_plugin(uid));
	p_member->set_fighting(global_user_data_mgr_t::get_user_fighting_value(uid));
	p_member->set_level(global_user_data_mgr_t::get_user_level(uid));
	p_member->set_state(proto::common::troop_state_normal);
	p_member->set_owner_uid(uid);

	m_troop_members.push_back(p_member);

	// 同步新队员加入
	sync_to_all_game(proto::common::troop_event_join, p_member->get_uid());

	return true;
}

void troop_t::add_apply_member(const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return;
	}

	uint64_t user_uid = p_user->get_uid();
	troop_apply_list::iterator itr = std::find_if(m_apply_members.begin(), m_apply_members.end(),
                            [user_uid](const troop_member_ptr& p_member)
                            {
                                if (NULL != p_member && p_member->get_uid() == user_uid) return true;
								return false;
                            });

	if (itr != m_apply_members.end()) {
		troop_member_ptr p_member = *itr;
		// 先删除，然后拿到头部
		m_apply_members.erase(itr);
		m_apply_members.push_front(p_member);
	} else {
		if (m_apply_num >= TROOP_APPLY_MAX_NUM) { // 大于最大数量，删除最老的一个
			m_apply_members.pop_back();
			m_apply_num--;
		}

		// 增加一个
		troop_member_ptr p_member(new troop_member(p_user->get_uid()));
		if (p_member == NULL) {
			log_error("p_member null error");
			return;
		}
		
		p_member->set_name(p_user->get_name());
		p_member->set_plugin(p_user->get_hero_plugin());
		p_member->set_fighting(p_user->get_fighting_value());
		p_member->set_level(p_user->get_level());

		m_apply_members.push_front(p_member);
		m_apply_num++;
	}

	global_user_ptr p_leader = global_user_manager_t::get_global_user_by_uid(m_leader_id);
	if (NULL != p_leader) {
		proto::client::ec_troop_event_notify ntf;
		ntf.set_event(proto::common::troop_event_apply);
		ntf.set_member_name(p_user->get_name());
		p_leader->send_msg_to_client(op_cmd::ec_troop_event_notify, ntf);
	}
}

bool troop_t::random_new_leader()
{
	if (m_troop_members.empty()) { // 没人了无法指定新队长
		log_error("troop[%lu] is empty, can't random new_leader", m_uid);
		return false;
	}

	for (uint32_t i = 0; i < m_troop_members.size(); ++i) { // 先找正常的
		if (m_troop_members[i] != NULL && 
			m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER && 
			m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			set_leader_id(m_troop_members[i]->get_uid());
			if (i != 0) std::swap(m_troop_members[0], m_troop_members[i]); // 队长排在第一位
			sync_to_all_game(proto::common::troop_event_leader, m_leader_id);
			return true;
		}
	}

	for (uint32_t i = 0; i < m_troop_members.size(); ++i) { // 在找暂离的
		if (m_troop_members[i] != NULL && 
			m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER && 
			m_troop_members[i]->get_state() == proto::common::troop_state_leave) {
			set_leader_id(m_troop_members[i]->get_uid());
			m_troop_members[i]->set_state(proto::common::troop_state_normal);
			if (i != 0) std::swap(m_troop_members[0], m_troop_members[i]); // 队长排在第一位
			sync_to_all_game(proto::common::troop_event_leader, m_leader_id);
			return true;
		}
	}

	for (uint32_t i = 0; i < m_troop_members.size(); ++i) { // 找不到说明全部离线强行指定一个能找到的玩家
		if (m_troop_members[i] != NULL && 
			m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
			if (m_troop_members[i]->get_uid() != m_leader_id) {
				set_leader_id(m_troop_members[i]->get_uid());
				if (i != 0) std::swap(m_troop_members[0], m_troop_members[i]); // 队长排在第一位
				sync_to_all_game(proto::common::troop_event_leader, m_leader_id);
			}
			return true;
		}
	}

	return false;
}

bool troop_t::appoint_new_leader(uint64_t uid)
{
	int32_t index = -1;
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (m_troop_members[i] != NULL && m_troop_members[i]->get_uid() == uid) {
			index = (int32_t)i;
			break;
		}
	}

	if (index == -1) {
		log_error("appoint new leader[%lu] error", uid);
		return false;
	}

	set_leader_id(uid);
	m_troop_members[index]->set_state(proto::common::troop_state_normal);
	if (index != 0) std::swap(m_troop_members[0], m_troop_members[index]); // 队长排在第一位
	sync_to_all_game(proto::common::troop_event_leader, m_leader_id);
	return true;
}

void troop_t::update_member_level(uint64_t member_uid, uint32_t level)
{
	troop_member_ptr p_member = get_troop_member(member_uid);
	if (NULL == p_member) {
		log_error("member[%lu] not found", member_uid);
		return;
	}
	p_member->set_level(level);
	sync_to_all_game(proto::common::troop_event_update, member_uid);

	// 查找有没导师
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (m_troop_members[i] != NULL && m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_TUTOR && member_uid == m_troop_members[i]->get_owner_uid()) {
			m_troop_members[i]->set_level(level);
			sync_to_all_game(proto::common::troop_event_update, m_troop_members[i]->get_uid());
			return;
		}
	}
}


void troop_t::update_member_online(const global_user_ptr& p_user, uint32_t online)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return;
	}

	troop_member_ptr p_member = get_troop_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("member[%lu] not in troop[%lu]", p_user->get_uid(), m_uid);
		p_user->set_troop_id(0); // 异常处理
		return;
	}

	if (online == 0) { // 上线
		if (m_leader_id == p_user->get_uid()) { // 离线的队长上线
			p_member->set_state(proto::common::troop_state_normal);
			sync_to_all_game(proto::common::troop_event_online, m_leader_id);
		} else { // 不是队长
			troop_member_ptr p_leader = get_troop_member(m_leader_id);
			if (p_leader == NULL || p_leader->get_state() != proto::common::troop_state_normal) { // 队长异常状态，成为新队长
				appoint_new_leader(p_user->get_uid());
			}
		}

		if (p_user->get_uid() != get_leader_id()) {
			// 先设置为暂离
			p_member->set_state(proto::common::troop_state_leave);
			sync_to_all_game(proto::common::troop_event_online, p_user->get_uid());

			const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
			if (NULL != p_scene) {
				if (p_scene->is_dungeon_scene()) { // 副本自动归队
					back_troop(p_user);
				} else { // 执行提示归队
					proto::client::ec_troop_rejoin_notify ntf;
					p_user->send_msg_to_client(op_cmd::ec_troop_rejoin_notify, ntf);
				}
			}
		}
	} else {
		p_member->set_state(proto::common::troop_state_offline);
		sync_to_all_game(proto::common::troop_event_online, p_user->get_uid());

		if (get_leader_id() == p_user->get_uid()) {
			//如果自己是队长，就产生新队长走产生队长的流程
			random_new_leader();
		}
	}
}


void troop_t::update_member_fighting(uint64_t member_uid, uint32_t fighting)
{
	troop_member_ptr p_member = get_troop_member(member_uid);
	if (NULL == p_member) {
		log_error("member[%lu] not found", member_uid);
		return;
	}

	p_member->set_fighting(fighting);
	sync_to_all_game(proto::common::troop_event_update, member_uid);
}


void troop_t::update_member_head_frame(uint64_t member_uid, uint32_t head_frame)
{
	troop_member_ptr p_member = get_troop_member(member_uid);
	if (NULL == p_member) {
		log_error("member[%lu] not found", member_uid);
		return;
	}
	p_member->set_plugin(head_frame);
	sync_to_all_game(proto::common::troop_event_update, member_uid);
}


void troop_t::update_member_name(uint64_t member_uid, const std::string& name)
{
	troop_member_ptr p_member = get_troop_member(member_uid);
	if (NULL == p_member) {
		log_error("member[%lu] not found", member_uid);
		return;
	}
	p_member->set_name(name);
	sync_to_all_game(proto::common::troop_event_update, member_uid);
}

uint32_t troop_t::ask_join(const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_unknown;
	}

	if (is_full()) {
		TROOP_LOG("troop[%lu] is full role[%lu] can't join", m_uid, p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_full;
	}

	uint32_t user_lv = p_user->get_level();
	if (user_lv < m_level_min || user_lv > m_level_max) {
		TROOP_LOG("troop[%lu] user[%lu] user_lv[%u] less min_lv[%u] or greater level_max[%u]", m_uid, p_user->get_uid(), user_lv, m_level_min, m_level_max);
		return common::errcode_enum::user_troop_err_code_invalid_level;
	}

	const global_user_ptr& p_leader_user = global_user_manager_t::get_global_user_by_uid(m_leader_id);
	if (NULL == p_leader_user) {
		log_error("troop[%lu] NULL == p_leader_user", m_uid);
		return common::errcode_enum::notice_unknown;
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_leader_user->get_scene_id());
	if (NULL == p_scene) {
		log_error("troop[%lu] leader user[%lu] scene not find", m_uid, p_leader_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_scene = p_scene->check_troop_join();
	if (back_scene != common::errcode_enum::error_ok) {
		TROOP_LOG("troop[%lu] leader[%lu] scene check troop join error", m_uid, p_leader_user->get_uid());
		return back_scene;
	}

	if (is_free_join()) { // 设置了自动加入
		if (add_member(p_user, proto::common::troop_state_leave)) {
			// 同步新队员加入
			sync_to_all_game(proto::common::troop_event_join, p_user->get_uid());
		}
	} else { // 加入申请队列通知队长
		add_apply_member(p_user);
		return common::errcode_enum::user_troop_join_send;
	}

	return common::errcode_enum::error_ok;
}


uint32_t troop_t::process_apply(uint64_t uid, uint32_t process)
{
	troop_apply_list::iterator itr = std::find_if(m_apply_members.begin(), m_apply_members.end(),
                            [uid](const troop_member_ptr& p_member)
                            {
                                if (NULL != p_member && p_member->get_uid() == uid) return true;
								return false;
                            });

	if (itr == m_apply_members.end()) {
		log_error("user[%lu] has not apply troop[%lu]", uid, get_uid());
		return common::errcode_enum::user_troop_err_code_not_in_troop;
	}

	// 先删除
	m_apply_members.erase(itr);
	if (m_apply_num > 0) m_apply_num--;

	const global_user_ptr& p_member_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_member_user) {
		TROOP_LOG("p_member_user[%lu] is offline", uid);
		return common::errcode_enum::user_troop_err_code_role_offline;
	}

	if (process == 0) { // 同意
		if (p_member_user->get_troop_id() != 0) {
			TROOP_LOG("user[%lu] agree apply member failed, apply member[%lu] already in troop", m_leader_id, uid);
			return common::errcode_enum::user_troop_err_code_already_in_troop;
		}

		if (is_full()) {
			TROOP_LOG("user[%lu] agree apply member failed, troop[%lu] full", m_leader_id, get_uid());
			return common::errcode_enum::user_troop_err_code_full;
		}

		proto::server::eg_troop_agree_apply_request request;
		request.set_apply_uid(m_leader_id);
		p_member_user->send_msg_to_game(op_cmd::eg_troop_agree_apply_request, request);
	} else {
		// 拒绝处理
	}

	return common::errcode_enum::error_ok;
}


uint32_t troop_t::agree_apply(const global_user_ptr& p_member)
{
	if (NULL == p_member) {
		log_error("NULL == p_member");
		return common::errcode_enum::notice_unknown;
	}

	//同意
	if (is_full()) {
		TROOP_LOG("agree apply member failed, troop[%lu] full", get_uid());
		return common::errcode_enum::user_troop_err_code_full;
	}

	if (add_member(p_member, proto::common::troop_state_leave)) {
		// 同步新队员加入
		sync_to_all_game(proto::common::troop_event_join, p_member->get_uid());
	}

	return common::errcode_enum::error_ok;
}

void troop_t::del_invite(uint64_t uid)
{
	std::map<uint64_t, uint32_t>::iterator itr = m_invite_list.find(uid);
	if (itr != m_invite_list.end()) {
		m_invite_list.erase(itr);
	}
}

void troop_t::invite_join(const global_user_ptr& p_invite_user, const global_user_ptr& p_member)
{
	if (NULL == p_invite_user) {
		log_error("NULL == p_invite_user");
		return;
	}

	if (NULL == p_member) {
		log_error("NULL == p_member");
		return;
	}

	uint32_t now_time = time_util_t::now_time();
	std::map<uint64_t, uint32_t>::iterator itr = m_invite_list.find(p_member->get_uid());
	if (itr == m_invite_list.end()) {
		m_invite_list.insert(std::make_pair(p_member->get_uid(), now_time));
	} else {
		auto invite_time = GET_CONF(Comprehensive, comprehensive_common::troop_invite_time);
		uint32_t val = GET_COMPREHENSIVE_VALUE_1(invite_time);

		if (itr->second + val >= now_time) {
			TROOP_LOG("troop[%lu] invite[%lu] cd", get_uid(), p_member->get_uid());
			return;
		}

		itr->second = now_time;
	}

	proto::client::ec_troop_invite_join_notify ntf;
	ntf.set_leader_name(p_invite_user->get_name());
	ntf.set_leader_level(p_invite_user->get_level());
	ntf.set_troop_id(string_util_t::uint64_to_string(get_uid()));
	ntf.set_type(get_type());
	p_member->send_msg_to_client(op_cmd::ec_troop_invite_join_notify, ntf);
}

uint32_t troop_t::process_invite(const global_user_ptr& p_user, uint32_t process)
{
	if (NULL == p_user) {
		log_error("NULL == p_user");
		return common::errcode_enum::notice_role_null;
	}

	std::map<uint64_t, uint32_t>::iterator itr = m_invite_list.find(p_user->get_uid());
	if (itr == m_invite_list.end()) {
		log_error("user[%lu] not in troop[%lu] invite list", p_user->get_uid(), get_uid());
		return common::errcode_enum::user_troop_err_code_no_right;
	}

	uint32_t invite_time = itr->second;
	// 拒绝不从邀请列表删除,主要是保持cd
	// m_invite_list.erase(itr);

	auto conf = GET_CONF(Comprehensive, comprehensive_common::team_invite_overtime);
	uint32_t troop_invite_time = GET_COMPREHENSIVE_VALUE_1(conf);

	if (common::time_util_t::now_time() > invite_time + troop_invite_time) {
		TROOP_LOG("user[%lu] invite_time too longer", p_user->get_uid());
		return common::errcode_enum::user_troop_err_invite_long_time;
	}

	const global_user_ptr& p_leader_user = global_user_manager_t::get_global_user_by_uid(m_leader_id);
	if (NULL == p_leader_user) {
		log_error("troop[%lu] NULL == p_leader_user[%lu]", m_uid, m_leader_id);
		return common::errcode_enum::notice_unknown;
	}

	if (process == 0) { //同意
		if (is_full()) {
			log_error("user[%lu] agree invite failed, troop[%lu] full", p_user->get_uid(), get_uid());
			return common::errcode_enum::user_troop_err_code_full;
		}

		if (p_user->get_troop_id() > 0) {
			log_error("user[%lu] agree invite failed, already in troop", p_user->get_uid());
			return common::errcode_enum::user_troop_err_code_already_in_troop;
		}

		const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_leader_user->get_scene_id());
		if (NULL == p_scene) {
			log_error("leader user[%lu] scene not find", p_leader_user->get_uid());
			return common::errcode_enum::notice_unknown;
		}

		uint32_t back_scene = p_scene->check_troop_join();
		if (back_scene != common::errcode_enum::error_ok) {
			log_error("leader user[%lu] scene check_troop_join error", p_leader_user->get_uid());
			return back_scene;
		}

		if (add_member(p_user, proto::common::troop_state_leave)) {
			// 同步新队员加入
			sync_to_all_game(proto::common::troop_event_join, p_user->get_uid());
		}
	}

	proto::client::ec_troop_invite_result_notify ntf;
	ntf.set_reply_code(errcode_enum::user_refuse_invitation);
	ntf.set_process(process);
	ntf.set_name(p_user->get_name());
	p_leader_user->send_msg_to_client(op_cmd::ec_troop_invite_result_notify, ntf);

	return common::errcode_enum::error_ok;
}

uint32_t troop_t::toggle_join_flag(uint64_t uid)
{
	if (m_leader_id != uid) {
		log_error("user[%lu] not leader", uid);
		return errcode_enum::user_troop_err_code_no_right;
	}

	m_is_free_join = !m_is_free_join;
	sync_to_all_game(proto::common::troop_event_modify, uid);
	return errcode_enum::error_ok;
}

uint32_t troop_t::set_leave_troop(const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("NULL == p_role");
		return common::errcode_enum::notice_role_null;
	}

	if (m_leader_id == p_user->get_uid()) {
		log_error("user[%lu] is leader", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_no_right;
	}

	troop_member_ptr p_member = get_troop_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("member[%lu] not found", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_not_in_troop;
	}

	if (p_member->get_state() >= proto::common::troop_state_leave) {
		TROOP_LOG("member[%lu] is already state[%u]", p_user->get_uid(), p_member->get_state());
		return common::errcode_enum::notice_unknown;
	}

// 	// 暂离判断
// 	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(p_user->get_scene_id());
// 	if (NULL == p_scene) {
// 		log_error("user[%lu] scene not find", p_user->get_uid());
// 		return common::errcode_enum::notice_unknown;
// 	}
// 
// 	// 副本不能暂离
// 	if (p_scene->is_dungeon_scene()) {
// 		TROOP_LOG("user[%lu] dungeon scene can't leave", p_user->get_uid());
// 		return common::errcode_enum::user_troop_err_temporarily;
// 	}

	p_member->set_state(proto::common::troop_state_leave);
	sync_to_all_game(proto::common::troop_event_leave, p_user->get_uid());

	return common::errcode_enum::error_ok;
}

uint32_t troop_t::back_troop(const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("NULL == p_role");
		return common::errcode_enum::notice_role_null;
	}

	if (m_leader_id == p_user->get_uid()) {
		log_error("user[%lu] is leader", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_no_right;
	}

	troop_member_ptr p_member = get_troop_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("member[%lu] not found", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_not_in_troop;
	}

	if (p_member->get_state() != proto::common::troop_state_leave) {
		log_error("member[%lu] state[%u] error", p_user->get_uid(), p_member->get_state());
		return common::errcode_enum::notice_unknown;
	}

	// 归队成功才设置
	// p_member->set_state(proto::common::troop_state_normal);
	sync_to_all_game(proto::common::troop_event_back, p_member->get_uid());

	return common::errcode_enum::error_ok;
}

uint32_t troop_t::transfer_leader(const global_user_ptr& p_user, uint64_t member_uid)
{
	if (NULL == p_user) {
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	if (m_leader_id != p_user->get_uid()) {
		log_error("user[%lu] not leader", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_no_right;
	}

	if (member_uid == p_user->get_uid()) {
		log_error("user[%lu] set self leader", p_user->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	// 从1开始找
	for (uint32_t i = 1; i < m_troop_members.size(); ++i) { 
		if (m_troop_members[i] != NULL && m_troop_members[i]->get_uid() == member_uid) {
			if (m_troop_members[i]->get_object_type() != proto::common::SCENEOBJECT_USER) {
				log_error("user[%lu] partner can't leader", p_user->get_uid());
				return common::errcode_enum::user_troop_partner_leader_error;
			}

			if (m_troop_members[i]->get_state() != proto::common::troop_state_normal) {
				return common::errcode_enum::user_transfer_captain_erro;
			}

			set_leader_id(m_troop_members[i]->get_uid());
			if (i != 0) std::swap(m_troop_members[0], m_troop_members[i]); // 队长排在第一位
			sync_to_all_game(proto::common::troop_event_leader, m_leader_id);
			return common::errcode_enum::error_ok;
		}
	}

	return errcode_enum::user_troop_err_code_not_in_troop;
}

void troop_t::do_dissolve()
{
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (m_troop_members[i] != NULL && m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
			global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_troop_members[i]->get_uid());
			if (NULL != p_user) p_user->set_troop_id(0);
		}
	}

	sync_to_all_game(proto::common::troop_event_dissolve, m_leader_id);
}

void troop_t::do_transmission_result(uint64_t uid, uint32_t result)
{
	if (result == 0) {
		TROOP_LOG("troop[%lu] member[%lu] transmission failed", m_uid, uid);
		return;
	}

	troop_member_ptr p_member = get_troop_member(uid);
	if (p_member == NULL) {
		log_error("troop[%lu] member[%lu] not find", m_uid, uid);
		return;
	}

	if (p_member->get_state() != proto::common::troop_state_leave) {
		log_error("troop[%lu] member[%lu] state[%u] is not leave when transmission success", m_uid, uid, p_member->get_state());
		return;
	}

	p_member->set_state(proto::common::troop_state_normal);

	sync_to_all_game(proto::common::troop_event_transmission, uid);
}

uint32_t troop_t::sync_task_info( const global_user_ptr& p_user,  const proto::common::task_state &task_info)
{
	if (NULL == p_user) {
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	if (m_leader_id != p_user->get_uid()) {
		log_error("user[%lu] not leader", p_user->get_uid());
		return common::errcode_enum::user_troop_err_code_no_right;
	}

	if (task_info.type() == TASK_TYPE_ZHUOGUI)
	{
		m_zg_task_info.CopyFrom(task_info);
		sync_to_all_game( proto::common::troop_event_zhuagui_task, p_user->get_uid() );
	}

	return common::errcode_enum::error_ok;
}

void troop_t::friend_notify()
{
	if (m_friend_notify > 0)
	{
		return;
	}
	for (const auto& p_member : m_troop_members) 
	{
		if (NULL == p_member)
		{
			continue;
		}
		if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER)
		{
			continue;
		}
		global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(p_member->get_uid());
		if (NULL != p_user)
		{
			p_user->get_friend_mgr().troop_notify(m_troop_members);
		}
	}
	++m_friend_notify;
}

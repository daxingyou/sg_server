#include "field_boss.hpp"

#include "tblh/FightAgainst.tbls.h"
#include "tblh/MonsterHero.tbls.h"
#include "config/config_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "fight/fight_manager.hpp"
#include "scene/scene.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/comprehensive_common.hpp"
#include "achieve/achieve_common.hpp"
#include "tblh/attr_enum.hpp"
#include "monster/monster_manager.hpp"
#include "field_boss_manager.hpp"
#include "tblh/BossPersonalTable.tbls.h"
#include "tblh/BossFamilyTable.tbls.h"

// 最大显示的排行榜数量
const uint32_t TOP_NUM = 3;
// 每秒血量同步
const uint32_t sync_hp_sec = 1;

field_boss_t::field_boss_t()
	: m_sync_sec(sync_hp_sec, common::time_util_t::now_time())
{

}

field_boss_t::~field_boss_t()
{

}

void field_boss_t::del_clear()
{
	monster_t::del_clear();

	m_boss_role_vec.clear();
	m_boss_role_map.clear();
}

bool field_boss_t::init_monster(const monster_common::monster_define_t& define, Monster* monster, uint64_t object_id /*= 0*/)
{
	if (NULL == monster) {
		log_error("npc null error");
		return false;
	}

	// 基类
	if (!monster_t::init_monster(define, monster, object_id)) {
		log_error("field_boss[%u] init null error", define.id);
		return false;
	}

	m_against_form_id = monster->get_fight_id();
	if (m_against_form_id == 0) {
		log_error("field_boss[%u] against_id not find", m_monster_tid);
		return false;
	}

	FightAgainst* against_ids = GET_CONF(FightAgainst, m_against_form_id);
	if (NULL == against_ids) {
		log_error("against_id[%u] null error", m_against_form_id);
		return false;
	}

	for (const auto& tt : against_ids->get_against_ids()) {
		uint32_t id = tt.second;
		MonsterHero* hero = GET_CONF(MonsterHero, id);
		if (NULL == hero) {
			log_error("load MonsterHero[%u] error", id);
			continue;
		}

		uint32_t hp = hero->get_attr(attrtype_enum::max_hp);
		m_hero_map.insert(std::make_pair(tt.first, monster_hero_info(id, hp, hp)));

		m_boss_max_hp += hp;
	}

	m_boss_curr_hp = m_boss_max_hp;

	// 启动定时器
	monster_manager_t::add_special_monster(m_object_id);

	return true;
}

void field_boss_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (NULL == object_info) {
		log_error("object_info null error");
		return;
	}

	monster_t::get_object_info(object_info);

	object_info->set_current_hp(m_boss_curr_hp);
	object_info->set_max_hp(m_boss_max_hp);
}

void field_boss_t::other_object_into_vision(const object_id_type& object)
{
	if (object.second == proto::common::SCENEOBJECT_USER) {
		m_vision_objects.insert(object);
	}
}

void field_boss_t::other_object_out_of_vision(const object_id_type& object)
{
	if (object.second == proto::common::SCENEOBJECT_USER) {
		m_vision_objects.erase(object);
	}
}

void field_boss_t::on_enter_scene(scene_ptr p_scene)
{
	if (NULL == p_scene) {
		log_error("p_scene null error");
		return;
	}

	monster_t::on_enter_scene(p_scene);

	p_scene->add_field_boss(m_object_id);

	// 通知整个场景
	proto::client::gc_field_boss_pos_info_notify notify;
	proto::common::field_boss_pos_info* info = notify.add_infos();
	if (NULL != info) {
		info->set_pos_act(proto::common::FIELD_BOSS_POS_ADD);
		info->set_object_id(common::string_util_t::uint64_to_string(m_object_id));
		info->set_monster_id(get_monster_id());
		info->set_pos_x((uint32_t)get_current_x());
		info->set_pos_y((uint32_t)get_current_y());
	}
	p_scene->send_msg_to_scene(op_cmd::gc_field_boss_pos_info_notify, notify);
}

void field_boss_t::on_leave_scene(scene_ptr p_scene)
{
	if (NULL == p_scene) {
		log_error("p_scene null error");
		return;
	}

	monster_t::on_leave_scene(p_scene);

	p_scene->del_field_boss(m_object_id);

	// 通知整个场景
	proto::client::gc_field_boss_pos_info_notify notify;
	proto::common::field_boss_pos_info* info = notify.add_infos();
	if (NULL != info) {
		info->set_pos_act(proto::common::FIELD_BOSS_POS_DEL);
		info->set_object_id(common::string_util_t::uint64_to_string(m_object_id));
		info->set_monster_id(get_monster_id());
	}

	p_scene->send_msg_to_scene(op_cmd::gc_field_boss_pos_info_notify, notify);
}

uint32_t field_boss_t::get_against_form_id()
{
	return m_against_form_id;
}

void field_boss_t::on_tick(uint64_t msecs)
{
	if (m_sync_sec(msecs / 1000) && m_is_need_sync) {
		m_is_need_sync = false;
		notify_object_common_info_for_nearbies();
	}
}

field_boss_t::boss_role_info_ptr field_boss_t::get_boss_role_info(uint64_t uid)
{
	boss_role_info_map::iterator itr = m_boss_role_map.find(uid);
	if (itr != m_boss_role_map.end()) {
		return itr->second;
	}

	return NULL;
}

field_boss_t::boss_role_info_ptr field_boss_t::add_boss_role_info(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL) {
		log_error("role[%lu] not find", uid);
		return NULL;
	}

	boss_role_info_ptr p_new_info(new boss_role_info());
	if (p_new_info == NULL) {
		log_error("add field boss role null error");
		return NULL;
	}

	p_new_info->role_id = uid;
	p_new_info->role_name = p_role->get_name();

	std::pair<boss_role_info_map::iterator, bool> res = m_boss_role_map.insert(std::make_pair(uid, p_new_info));
	if (!res.second) {
		log_error("role[%lu] create new boss role info insert error, it is exist", uid);
		return NULL;
	}

	// 同时在排序列表中增加
	m_boss_role_vec.push_back(p_new_info);

	return p_new_info;
}

void field_boss_t::fight_role_hurt(uint64_t fight_id,
	const proto::common::fight_camp& self_camp, const proto::common::fight_camp& enemy_camp, 
	const combat_ptr& combat)
{
	if (combat == NULL) {
		log_error("combat null error");
		return;
	}

	// 已经死亡，不走后面的处理
	if (is_die()) {
		return;
	}

	proto::common::hero_state_data hurts_data;
	uint32_t real_hurts = 0;
	for (const proto::common::hero_single& hero : enemy_camp.heroes().hero_list())
	{
		monster_hero_map::iterator itr = m_hero_map.find(hero.fight_attr().pos());
		if (itr != m_hero_map.end() && itr->second.m_id == hero.base_attr().id()) {
			uint32_t round_hurt = combat->get_hero_injured(common::string_util_t::string_to_uint64(hero.base_attr().fight_hero_uid()));
			if (round_hurt == 0) continue;

			if (itr->second.m_curr_hp > round_hurt) {
				real_hurts += round_hurt;
				itr->second.m_curr_hp -= round_hurt;
			} else {
				real_hurts += itr->second.m_curr_hp;
				itr->second.m_curr_hp = 0;
			}

			//记录每个位置的怪物的血量		
			proto::common::hero_state *p_hurt_data = hurts_data.add_states();
			p_hurt_data->set_hero_uid(hero.base_attr().fight_hero_uid());
			p_hurt_data->set_pos(hero.fight_attr().pos());
			p_hurt_data->set_hp( itr->second.m_curr_hp );
		}
	}

	if (real_hurts == 0) {
		return;
	}

	if (m_boss_curr_hp > real_hurts) {
		m_boss_curr_hp -= real_hurts;
	} else {
		m_boss_curr_hp = 0;
	}

	//log_error(" m_boss_curr_hp[%u] hurt_hp-----%u", m_boss_curr_hp, real_hurts  );
	if (m_boss_curr_hp == 0) { // 死亡
		set_object_die();
	}

	uint32_t curr_time = common::time_util_t::now_time();
	for (const proto::common::obj_single& obj : self_camp.objs().obj())
	{
		if (obj.object_type() == proto::common::SCENEOBJECT_USER)
		{
			uint64_t role_uid = string_util_t::string_to_uint64(obj.object_uid());

			boss_role_info_ptr p_boss_role_info = get_boss_role_info(role_uid);
			if (p_boss_role_info == NULL) {
				p_boss_role_info = add_boss_role_info(role_uid);
			}

			if (p_boss_role_info == NULL) {
				log_error("p_boss_role_info[%lu] add error", role_uid);
				continue;
			}

			p_boss_role_info->hurt += real_hurts;
			p_boss_role_info->hurt_time = curr_time;

			role_ptr p_role = role_manager_t::find_role(role_uid);
			if (NULL != p_role && p_role->get_family_id() != 0) {
				m_family_hurt_map[p_role->get_family_id()] += real_hurts;
			}

			// 这里处理击杀boss成就
			if (is_die()) {
				m_killers.push_back(role_uid);
				if (!m_killer_name.empty())
					m_killer_name.append(" ");
				m_killer_name.append(p_boss_role_info->role_name);

				achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_FIELD_BOSS_KILL_NUMBER);
			}
		}
	}

	// 通知其他正在战斗的人血量的变化
	update_all_combat(fight_id);
	
	// 同步血量到各个战斗中,也同步自己
	sync_all_boss_hp( 0, hurts_data );

	if (is_die()) { // 死亡结算
		proto::server::ge_field_boss_killed_notify ntf;
		ntf.set_id(m_id);
		ntf.set_object_id(m_object_id);
		ntf.set_killer_name(m_killer_name);
		env::server->send_msg_to_center(op_cmd::ge_field_boss_killed_notify, 0, ntf);

		set_all_fight_failed(fight_id);
		set_need_clear();
		start_settlement(proto::server::FIELD_BOSS_KILL);
	} else {
		m_is_need_sync = true;
	}
}

void field_boss_t::calc_persion_rank(proto::server::EM_FIELD_MODE mode, proto::server::ge_field_boss_rank_notify& ntf)
{
	if (m_boss_role_vec.empty())
		return;

	std::sort(m_boss_role_vec.begin(), m_boss_role_vec.end());

	const std::vector<uint32_t>& ranks = field_boss_manager_t::get_boss_persion_reward(m_id);
	if (ranks.empty()) {
		log_error("field boss[%u] persion rank reward empty", m_monster_tid);
		return;
	}

	// 奖励发放
	uint32_t index = 0;
	for (std::vector<uint32_t>::const_iterator citr = ranks.begin();
		citr != ranks.end(); ++citr) { // 所有的排名奖励id
		uint32_t id = *citr;
		BossPersonalTable* p_conf = GET_CONF(BossPersonalTable, id);
		proto::server::field_boss_rank* rank_rwd = ntf.add_persion_ranks();
		if (p_conf && rank_rwd) {
			rank_rwd->set_rank_id(id);
			for (uint32_t i = index; i < m_boss_role_vec.size() && i < p_conf->rank(); ++i) {
				if (m_boss_role_vec[i] != NULL)
					rank_rwd->add_uids(m_boss_role_vec[i]->role_id);
			}

			index = p_conf->rank();
		}

		if (index >= m_boss_role_vec.size()) return;
	}
}

void field_boss_t::calc_family_rank(proto::server::EM_FIELD_MODE mode, proto::server::ge_field_boss_rank_notify& ntf)
{
	if (m_family_hurt_map.empty())
		return;

	struct family_hurt_t
	{
		uint64_t family_uid = 0;
		uint32_t hurt = 0;

		family_hurt_t(uint64_t uid, uint32_t val)
			: family_uid(uid), hurt(val)
		{
		}

		bool operator< (const family_hurt_t& item) const
		{
			if (this->hurt > item.hurt) {
				return true;
			}

			if (this->hurt == item.hurt) {
				return this->family_uid < item.family_uid;
			}

			return false;
		}
	};

	std::vector<family_hurt_t> family_hurt_vec;
	for (family_hurt_map::const_iterator citr = m_family_hurt_map.begin();
		citr != m_family_hurt_map.end(); ++citr) {
		family_hurt_vec.push_back(family_hurt_t(citr->first, citr->second));
	}
	std::sort(family_hurt_vec.begin(), family_hurt_vec.end());

	const std::vector<uint32_t>& ranks = field_boss_manager_t::get_boss_family_reward(m_id);
	if (ranks.empty()) {
		log_error("field boss[%u] family rank reward empty", m_monster_tid);
		return;
	}

	// 奖励发放
	uint32_t index = 0;
	for (std::vector<uint32_t>::const_iterator citr = ranks.begin();
		citr != ranks.end(); ++citr) { // 所有的排名奖励id
		uint32_t id = *citr;
		BossFamilyTable* p_conf = GET_CONF(BossFamilyTable, id);
		proto::server::field_boss_rank* rank_rwd = ntf.add_family_ranks();
		if (p_conf && rank_rwd) {
			rank_rwd->set_rank_id(id);
			for (uint32_t i = index; i < family_hurt_vec.size() && i < p_conf->rank(); ++i) {
				rank_rwd->add_uids(family_hurt_vec[i].family_uid);
			}

			index = p_conf->rank();
		}

		if (index >= family_hurt_vec.size()) return;
	}
}

uint32_t field_boss_t::get_fight_cd_time(uint64_t uid)
{
	boss_role_info_ptr p_boss_role_info = get_boss_role_info(uid);
	if (p_boss_role_info == NULL) {
		return 0;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::boss_battle_cd);
	uint32_t cd_time = GET_COMPREHENSIVE_VALUE_1(p_conf);

	uint32_t curr_time = common::time_util_t::now_time();
	if (p_boss_role_info->leave_fight_time + cd_time > curr_time) {
		return p_boss_role_info->leave_fight_time + cd_time - curr_time;
	}

	return 0;
}

void field_boss_t::update_all_combat(uint64_t exception /*= 0*/)
{
	// 通知除自己之外的所有其他和boss战斗的人
	proto::common::fight_state fight_state;
	if (!get_hero_state_data(fight_state.mutable_enemy())) {
		log_error("get boss hero data error");
		return;
	}

	for (auto fight_id : m_field_boss_combat) {
		if (exception != 0 && fight_id == exception) continue;
		fight_manager_t::update_fight(fight_id, fight_state);
	}
}

bool field_boss_t::sync_all_boss_hp( uint64_t fight_id_in, const proto::common::hero_state_data& data)
{
	for (auto fight_id : m_field_boss_combat) {
		fight_manager_t::sync_enemy_hp(fight_id, data);
	}
	return true;
}

bool field_boss_t::get_hero_state_data(proto::common::hero_state_data* data)
{
	if (NULL == data) {
		log_error("hero_state_data null error!");
		return false;
	}

	if (m_hero_map.empty()) {
		log_error("boos[%u] m_hero_map empty", get_monster_id());
		return false;
	}

	for (monster_hero_map::iterator iter = m_hero_map.begin();
		iter != m_hero_map.end(); ++iter) {
		proto::common::hero_state* hero = data->add_states();
		if (NULL != hero) {
			hero->set_pos(iter->first);
			hero->set_hp(iter->second.m_curr_hp);

			//printf("\033[31m pos[%u] curr hp:%u max hp:%u \033[0m\n", iter->first, iter->second.m_curr_hp, iter->second.m_max_hp);
		}
	}

	return true;
}

void field_boss_t::add_combat(uint64_t id)
{
	if (m_is_fight_end) return;

	m_field_boss_combat.insert(id);
}

void field_boss_t::leave_fight(uint64_t fight_id, const proto::common::fight_camp& self_camp)
{
	del_combat(fight_id);

	uint32_t curr_time = common::time_util_t::now_time();
	for (const proto::common::obj_single& obj : self_camp.objs().obj())
	{
		if (obj.object_type() == proto::common::SCENEOBJECT_USER)
		{
			uint64_t role_uid = string_util_t::string_to_uint64(obj.object_uid());

			boss_role_info_ptr p_boss_role_info = get_boss_role_info(role_uid);
			if (p_boss_role_info == NULL) {
				p_boss_role_info = add_boss_role_info(role_uid);
			}

			if (p_boss_role_info == NULL) {
				log_error("p_boss_role_info[%lu] add error", role_uid);
				continue;
			}

			p_boss_role_info->leave_fight_time = curr_time;

			BOSS_LOG("role[%lu] leave boss fight[%lu]", role_uid, fight_id);
		}
	}
}

void field_boss_t::del_combat(uint64_t id)
{
	if (m_is_fight_end) return;

	m_field_boss_combat.erase(id);
}

void field_boss_t::set_all_fight_failed(uint64_t exception /*= 0*/)
{
	if (m_is_fight_end) return;

	m_is_fight_end = true;

	field_boss_combat fight_list = m_field_boss_combat;
	// 所有战斗强制失败
	for (auto fight_id : fight_list) {
		if (exception != 0 && fight_id == exception) continue;
		// 击杀场景内玩家
		combat_ptr p_combat = fight_manager_t::get_combat(fight_id);
		if (p_combat != NULL) {
			p_combat->all_fail();
		}
	}

	m_field_boss_combat.clear();
}

void field_boss_t::set_all_fight_success(uint64_t exception /*= 0*/)
{
	if (m_is_fight_end) return;

	m_is_fight_end = true;

	field_boss_combat fight_list = m_field_boss_combat;
	// 所有战斗强制失败
	for (auto fight_id : fight_list) {
		if (exception != 0 && fight_id == exception) continue;
		// 击杀场景内玩家
		combat_ptr p_combat = fight_manager_t::get_combat(fight_id);
		if (p_combat != NULL) {
			p_combat->all_success();
		}
	}
	m_field_boss_combat.clear();
}


void field_boss_t::field_boss_end(proto::server::EM_FIELD_MODE mode)
{
	// 所有其他战斗结束
	set_all_fight_success();

	set_need_clear();

	start_settlement(mode);
}

void field_boss_t::start_settlement(proto::server::EM_FIELD_MODE mode)
{
	if (mode != proto::server::FIELD_BOSS_NONE) {
		proto::server::ge_field_boss_rank_notify ntf;
		ntf.set_id(m_id);
		ntf.set_monster_id(m_monster_tid);
		ntf.set_mode(mode);
		// 个人伤害奖励发放
		calc_persion_rank(mode, ntf);
		// 个人伤害奖励发放
		calc_family_rank(mode, ntf);
		if (mode == proto::server::FIELD_BOSS_KILL) {
			for (const auto& killer : m_killers) {
				ntf.add_killers(killer);
			}
		}
		env::server->send_msg_to_center(op_cmd::ge_field_boss_rank_notify, 0, ntf);
	}

	BOSS_LOG("field boss[%lu:%u] start settlement, reason:%u", m_object_id, m_monster_tid, mode);
}

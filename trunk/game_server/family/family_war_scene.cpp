#include "family_war_scene.hpp"

#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "config_mgr.h"
#include "tblh/errcode_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/JumpTable.tbls.h"
#include "scene/scene_manager.hpp"
#include "family_war_long.hpp"
#include "monster/monster_manager.hpp"
#include "tblh/CollectTable.tbls.h"
#include "collect/collect_platform.hpp"
#include "hero/hero_manager.hpp"
#include "fight/fight_manager.hpp"
#include "fight/fight_hero.hpp"
#include "family_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "achieve/achieve_common.hpp"
#include "tblh/FamilyWarPersonal.tbls.h"
#include "tblh/FamilyWarRank.tbls.h"

family_war_scene_t::family_war_scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param)
	: scene_t(scene_id, map_id, scene_type, type_param)
{

}

bool family_war_scene_t::init()
{
	// 基类
	if (!scene_t::init())
	{
		log_error("king_war_scene_t::init() error");
		return false;
	}

	// 状态管理初始化
	if (!m_state_controler.init(shared_from_this(), SceneActiveState::None_State))
	{
		log_error("family war scene[%lu] init state_controler error", m_scene_id);
		return false;
	}

	// 设置跳跃点id
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_left_jump_id);
	m_stronghold_side[family_war_side_left].m_jump_id = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_left_battlefield);
	m_stronghold_side[family_war_side_left].m_war_id = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_left_area_id);
	m_stronghold_side[family_war_side_left].m_area_id = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_right_jump_id);
	m_stronghold_side[family_war_side_right].m_jump_id = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_right_battlefield);
	m_stronghold_side[family_war_side_right].m_war_id = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_right_area_id);
	m_stronghold_side[family_war_side_right].m_area_id = GET_COMPREHENSIVE_VALUE_1(p_conf);

	// 龙柱信息
	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_long_area_id);
	m_family_war_long_info.m_area_id = GET_COMPREHENSIVE_VALUE_1(p_conf);

	// 日月台的伤害和攻击间隔
	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_sun_attack_interval);
	m_family_war_platform[family_war_platform_sun].m_interval = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_sun_attack_hurt);
	m_family_war_platform[family_war_platform_sun].m_hurt = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_moon_attack_interval);
	m_family_war_platform[family_war_platform_moon].m_interval = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_moon_attack_hurt);
	m_family_war_platform[family_war_platform_moon].m_hurt = GET_COMPREHENSIVE_VALUE_1(p_conf);

	// 战旗血量
	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_flag_hp);
	uint32_t flag_hp = GET_COMPREHENSIVE_VALUE_1(p_conf);

	proto::client::family_war_fight_info* left_info = m_stronghold_side[family_war_side_left].m_data.mutable_base();
	if (left_info)
	{
		left_info->set_curr_hp(0);
		left_info->set_max_hp(flag_hp);
	}

	proto::client::family_war_fight_info* right_info = m_stronghold_side[family_war_side_right].m_data.mutable_base();
	if (right_info)
	{
		right_info->set_curr_hp(0);
		right_info->set_max_hp(flag_hp);
	}

	// 定时器初始化
	if (NULL == env::server)
	{
		log_error("env::server null error");
		return false;
	}

	m_long_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_long_timer)
	{
		log_error("m_long_timer null error");
		return false;
	}

	for (uint32_t i = 0; i < family_war_platform_max; ++i)
	{
		m_platform_timer[i].reset(new boost::asio::deadline_timer(env::server->get_ios()));
		if (NULL == m_platform_timer[i])
		{
			log_error("m_platform_timer[%u] null error", i);
			return false;
		}
	}

	return true;
}

void family_war_scene_t::clear()
{
	m_family_war_sort_list.clear();
	m_family_war_role_map.clear();

	// 定时器清除
	if (NULL != m_long_timer)
	{
		m_long_timer->cancel();
	}

	for (uint32_t i = 0; i < family_war_platform_max; ++i)
	{
		if (NULL != m_platform_timer[i])
		{
			m_platform_timer[i]->cancel();
		}
	}

	// 基类删除所有怪物
	scene_t::clear();
}

bool family_war_scene_t::start()
{
	set_scene_state(SCENE_STATE_NORMAL);

	uint32_t now_time = common::time_util_t::now_time();

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dungeon_max_time);
	uint32_t max_alive = GET_COMPREHENSIVE_VALUE_1(p_conf);
	m_last_time = now_time + max_alive;

	m_state_controler.change_state(SceneActiveState::Run_State);

	return true;
}

void family_war_scene_t::load_map_collect()
{
	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return;
	}

	// 召唤日月台
	uint32_t index = 0;
	const collect_define_vec& collect_vec = p_map->get_map_collect_points();
	for (collect_define_vec::const_iterator citr = collect_vec.begin();
		citr != collect_vec.end() && index < family_war_platform_max; ++citr)
	{
		const t_collect_define& define = *citr;

		CollectTable* p_collect_config = GET_CONF(CollectTable, define.m_collect_id);
		if (NULL == p_collect_config)
		{
			log_error("scene::load_map_object not found collect [%d]", define.m_collect_id);
			continue;
		}

		if (p_collect_config->type() == proto::common::collect_type_platform)
		{
			family_war_platform_ptr p_family_war_platform(new family_war_platform_t(define.m_collect_id));
			if (p_family_war_platform != NULL && p_family_war_platform->init_collect_point(define.m_x, define.m_y))
			{
				p_family_war_platform->set_platform_type((family_war_platform_type)index);
				p_family_war_platform->set_family_war_family(family_war_side_left, m_stronghold_side[family_war_side_left].m_family_id);
				p_family_war_platform->set_family_war_family(family_war_side_right, m_stronghold_side[family_war_side_right].m_family_id);

				if (init_collect_point(p_family_war_platform))
				{
					m_family_war_platform[index].m_object_id = p_family_war_platform->get_object_id();
					m_family_war_platform[index].m_pos.set_pos_x(define.m_x);
					m_family_war_platform[index].m_pos.set_pos_y(define.m_y);
				}
			}

			index++;
		}
	}

	proto::client::gc_family_platform_info_notify notify;
	proto::common::family_object_pos* sun_pos = notify.mutable_sun_pos();
	if (sun_pos) {
		sun_pos->CopyFrom(m_family_war_platform[family_war_platform_sun].m_pos);
	}

	proto::common::family_object_pos* moon_pos = notify.mutable_moon_pos();
	if (moon_pos) {
		moon_pos->CopyFrom(m_family_war_platform[family_war_platform_moon].m_pos);
	}

	send_msg_to_scene(op_cmd::gc_family_platform_info_notify, notify);
}

sPos family_war_scene_t::get_scene_born_position(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return sPos();
	}

	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type < family_war_side_max)
	{
		JumpTable* p_jump_info = GET_CONF(JumpTable, m_stronghold_side[side_type].m_jump_id);
		if (NULL == p_jump_info)
		{
			log_error("role[%lu] scene[%lu,%u] can not find jump id[%u]", p_role->get_uid(), m_scene_id, m_map_tid, m_stronghold_side[side_type].m_jump_id);
			return sPos();
		}

		return sPos(p_jump_info->to_position_x(), p_jump_info->to_position_y(), p_jump_info->to_angle());
	}

	return scene_t::get_scene_born_position(p_role);
}

void family_war_scene_t::update(uint32_t curr_time)
{
	// 副本秒定时器
	m_state_controler.action(curr_time);

	// 排行3秒更新一次
	if (m_update_rank && common::time_util_t::now_time() > m_next_update_rank)
	{
		rank_all_score();
		m_update_rank = false;
		m_next_update_rank = common::time_util_t::now_time() + 3;
	}
}

void family_war_scene_t::forced_all_leave()
{
	std::set<uint64_t> old_roles = m_roles;

	for (const auto& uid : old_roles)
	{
		role_ptr p_role = role_manager_t::find_role(uid);
		if (p_role != NULL)
		{
			scene_manager_t::role_back_scene(p_role, proto::server::forced_change_scene);
		}
	}
}

void family_war_scene_t::set_family_war_date(const proto::server::family_war_dungeon_data& data)
{
	m_family_war_id = data.id();

	const proto::server::family_war_dungeon_info& left_side = data.left_family();
	m_stronghold_side[family_war_side_left].m_family_id = left_side.family_id();
	proto::client::family_war_fight_info* left_info = m_stronghold_side[family_war_side_left].m_data.mutable_base();
	if (left_info)
	{
		left_info->set_family_id(common::string_util_t::uint64_to_string(left_side.family_id()));
		left_info->set_family_name(left_side.family_name());
	}

	const proto::server::family_war_dungeon_info& right_side = data.right_family();
	m_stronghold_side[family_war_side_right].m_family_id = right_side.family_id();
	proto::client::family_war_fight_info* right_info = m_stronghold_side[family_war_side_right].m_data.mutable_base();
	if (right_info)
	{
		right_info->set_family_id(common::string_util_t::uint64_to_string(right_side.family_id()));
		right_info->set_family_name(right_side.family_name());
	}
}

void family_war_scene_t::prepare_family_war(uint32_t prepare_time)
{
	m_state_type = proto::common::family_war_state_prepare;
	m_state_time = prepare_time;
}

void family_war_scene_t::start_family_war(uint32_t end_time)
{
	m_state_type = proto::common::family_war_state_start;
	m_state_time = end_time;

	// 召唤日月塔
	load_map_collect();

	// 延迟召唤龙柱
	if (NULL != m_long_timer)
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_count_down);
		uint32_t count_down = GET_COMPREHENSIVE_VALUE_1(p_conf);
		m_family_war_long_info.m_refresh_time = common::time_util_t::now_time() + count_down;

		if (count_down > 0)
		{
			m_long_timer->expires_from_now(boost::posix_time::seconds(count_down));
			m_long_timer->async_wait(boost::bind(&family_war_scene_t::on_long_refresh_timer, boost::weak_ptr<family_war_scene_t>(get_family_war_scene_ptr()), boost::asio::placeholders::error));
		}

		BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_104, 0, 0, 0, count_down);

		// 开战后多久刷新
		proto::client::gc_family_long_info_notify notify;
		notify.set_object_id(common::string_util_t::uint64_to_string(m_family_war_long_info.m_object_id));
		notify.set_revive_time(m_family_war_long_info.m_refresh_time);
		send_msg_to_scene(op_cmd::gc_family_long_info_notify, notify);
	}

	notify_scene_war_info();

	FAMILY_LOG("family_war[%u] left_family[%lu] right_family[%lu] start", m_family_war_id, m_stronghold_side[family_war_side_left].m_family_id, m_stronghold_side[family_war_side_right].m_family_id);
}

void family_war_scene_t::end_family_war()
{
	if (m_state_type == proto::common::family_war_state_end)
		return;

	m_state_type = proto::common::family_war_state_end;

	family_war_side_type win_side = settlement_win_family();
	if (win_side >= family_war_side_max)
	{
		log_error("family war[%u] win side error", m_family_war_id);
		return;
	}

	m_win_family = m_stronghold_side[win_side].m_family_id;

	// 最后排序依次
	rank_all_score();

	// 定时器清除
	if (NULL != m_long_timer)
	{
		m_long_timer->cancel();
	}

	for (uint32_t i = 0; i < family_war_platform_max; ++i)
	{
		if (NULL != m_platform_timer[i])
		{
			m_platform_timer[i]->cancel();
		}
	}

	// 设置所有战斗结束
	set_all_combat_failed();

	// 设置pk模式
	std::set<uint64_t> all_roles = m_roles;
	for (std::set<uint64_t>::const_iterator citr = all_roles.begin();
		citr != all_roles.end(); ++citr)
	{
		role_ptr p_role = role_manager_t::find_role(*citr);
		if (p_role) {
			p_role->change_pk_mode(proto::common::PK_PEACE);
		}
	}

	// 设置龙柱死亡状态
	m_family_war_long_info.m_long_state = family_war_long_state_die;
	// 广播图标变化
	m_family_war_long_info.broadcast_icon();
	
 	// 删除龙柱
 	if (m_family_war_long_info.m_object_id != 0)
 	{
 		on_leave_s(object_id_type(m_family_war_long_info.m_object_id, proto::common::SCENEOBJECT_MONSTER));
 
 		monster_manager_t::delete_monster(m_family_war_long_info.m_object_id);
 	}
 
 	// 删除日月台
 	for (uint32_t i = 0; i < family_war_platform_max; ++i)
 	{
 		collect_point_ptr p_collect_point = collect_manager_t::find_collect_point(m_family_war_platform[i].m_object_id);
 		if (NULL != p_collect_point)
 		{
 			p_collect_point->set_object_die();
 			// 消失
 			p_collect_point->leave_scene();
 
 			collect_manager_t::delete_collect_point(m_family_war_platform[i].m_object_id);
 		}
 	}

	notify_scene_war_info();

	m_state_controler.change_state(SceneActiveState::Stop_State);

	BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_112, 0, 0, 0, m_stronghold_side[win_side].get_family_name().c_str());
}

family_war_side_type family_war_scene_t::settlement_win_family()
{
	if (m_stronghold_side[family_war_side_left].m_data.base().curr_hp() < m_stronghold_side[family_war_side_right].m_data.base().curr_hp()) {
		return family_war_side_right;
	} 

	// 积分相同就左方胜利
	return family_war_side_left;
}

void family_war_scene_t::peek_family_war_result(proto::server::family_war_result_data* p_result_data)
{
	if (!p_result_data) {
		return;
	}

	p_result_data->set_id(m_family_war_id);
	p_result_data->set_win_family_id(get_win_family());
	p_result_data->set_left_family_score(get_family_score(family_war_side_left));
	p_result_data->set_right_family_score(get_family_score(family_war_side_right));

	family_war_rank_list::const_iterator rank_list_citr = m_family_war_sort_list.begin();

	// 个人奖励表
	std::map<uint32_t, FamilyWarPersonal*> personal_confs;
	GET_ALL_CONF(FamilyWarPersonal, personal_confs);
	std::map<uint32_t, FamilyWarPersonal*>::iterator personal_conf_itr = personal_confs.begin();

	uint32_t index = 0;
	std::map<uint32_t, FamilyWarRank*> rank_confs;
	GET_ALL_CONF(FamilyWarRank, rank_confs);
	for (std::map<uint32_t, FamilyWarRank*>::iterator itr = rank_confs.begin();
		itr != rank_confs.end(); ++itr) {
		if (!itr->second) continue;

		for (; index < itr->second->rank(); ++index) {
			if (rank_list_citr != m_family_war_sort_list.end()) {
				const family_war_role_info_ptr& p_king_war_role = *rank_list_citr;
				if (!p_king_war_role) {
					log_error("family_war_role_info_ptr null error");
					++rank_list_citr;
					continue;
				}

				proto::server::family_war_personal* p_personal = p_result_data->add_all_roles();
				if (p_personal) {
					p_personal->set_uid(p_king_war_role->m_role_uid);
					p_personal->set_exploit(p_king_war_role->m_data.personal_score());
					p_personal->set_rank(index + 1);
					p_personal->set_rank_level(itr->first);
					p_personal->set_exploit_level(0);
				}

				if (personal_conf_itr != personal_confs.end()) {
					if (!personal_conf_itr->second || p_king_war_role->m_data.personal_score() < personal_conf_itr->second->exploit()) {
						for (++personal_conf_itr; personal_conf_itr != personal_confs.end(); ++personal_conf_itr) {
							if (personal_conf_itr->second && p_king_war_role->m_data.personal_score() >= personal_conf_itr->second->exploit()) {
								break;
							}
						}
					}

					if (personal_conf_itr != personal_confs.end()) {
						if (p_personal) {
							p_personal->set_exploit_level(personal_conf_itr->first);
						}
					}
				}

				++rank_list_citr;
			}

			if (rank_list_citr == m_family_war_sort_list.end()) {
				break;
			}
		}

		if (rank_list_citr == m_family_war_sort_list.end()) {
			break;
		}
	}
}

uint32_t family_war_scene_t::get_family_war_member(family_war_side_type type)
{
	if (type < family_war_side_max)
	{
		return m_stronghold_side[type].m_data.war_member();
	}

	return 0;
}

family_war_side_type family_war_scene_t::get_family_war_side_type(uint64_t family_id)
{
	if (family_id == m_stronghold_side[family_war_side_left].m_family_id)
	{
		return family_war_side_left;
	}
	else if (family_id == m_stronghold_side[family_war_side_right].m_family_id)
	{
		return family_war_side_right;
	}

	return family_war_side_none;
}

uint32_t family_war_scene_t::get_family_score(family_war_side_type type)
{
	if (type >= family_war_side_max)
	{
		log_error("get_family_score type error");
		return 0;
	}

	return m_stronghold_side[type].m_data.base().curr_hp();
}

family_war_side_type family_war_scene_t::get_enemy_side_type(family_war_side_type type)
{
	if (type == family_war_side_left)
	{
		return family_war_side_right;
	}
	else if (type == family_war_side_right)
	{
		return family_war_side_left;
	}

	return family_war_side_none;
}

family_war_scene_t::family_war_role_info_ptr family_war_scene_t::get_family_war_role_info(uint64_t uid)
{
	family_war_role_map::const_iterator citr = m_family_war_role_map.find(uid);
	if (citr != m_family_war_role_map.end())
	{
		return citr->second;
	}

	return family_war_role_info_ptr();
}

family_war_scene_t::family_war_role_info_ptr family_war_scene_t::add_family_war_role_info(uint64_t uid)
{
	family_war_role_info_ptr p_family_war_role(new family_war_role_info_t(uid));
	if (NULL == p_family_war_role)
	{
		log_error("p_family_war_role null error");
		return NULL;
	}

	p_family_war_role->init();

	std::pair<family_war_role_map::iterator, bool> ret = m_family_war_role_map.insert(std::make_pair(uid, p_family_war_role));
	if (!ret.second)
	{
		log_error("role[%lu] insert m_family_war_role_map error", uid);
		return NULL;
	}

	m_family_war_sort_list.push_back(p_family_war_role);

	return p_family_war_role;
}

void family_war_scene_t::peek_family_war_fight_info(proto::client::gc_family_war_fight_info_notify& notify)
{
	notify.set_state(m_state_type);
	notify.set_end_time(m_state_time);

	proto::client::family_war_fight_info* left_side = notify.mutable_left_info();
	if (left_side)
	{
		left_side->CopyFrom(m_stronghold_side[family_war_side_left].m_data.base());
	}

	proto::client::family_war_fight_info* right_side = notify.mutable_right_info();
	if (right_side)
	{
		right_side->CopyFrom(m_stronghold_side[family_war_side_right].m_data.base());
	}
}

void family_war_scene_t::family_war_summon_long(family_war_long_type type)
{
	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return;
	}

	uint32_t monster_tid = get_long_type_monster_tid(type);
	if (monster_tid == 0)
	{
		log_error("long_type[%u] monster_tid not find", type);
		return;
	}

	const monster_define_vec& monster_vec = p_map->get_map_monsters();
	for (monster_define_vec::const_iterator citr = monster_vec.begin();
		citr != monster_vec.end(); ++citr)
	{
		const monster_common::monster_define_t& define = *citr;

		Monster* p_conf = GET_CONF(Monster, define.id);
		if (NULL == p_conf)
		{
			log_error("load monster[%u] error", define.id);
			continue;
		}

		if (p_conf->type() == proto::common::EM_FAMILY_WAR_LONNG)
		{
			family_war_long_ptr p_family_war_long(new family_war_long_t());
			if (p_family_war_long == NULL)
			{
				log_error("p_family_war_long null error");
				return;
			}

			if (!p_family_war_long->init_long(monster_tid, define, p_conf))
			{
				log_error("p_family_war_long[%u] init error", monster_tid);
				return;
			}

			// 初始化龙柱
			p_family_war_long->set_family_war_family(family_war_side_left, m_stronghold_side[family_war_side_left].m_family_id);
			p_family_war_long->set_family_war_family(family_war_side_right, m_stronghold_side[family_war_side_right].m_family_id);

			if (init_object(p_family_war_long, define))
			{
				m_family_war_long_info.m_object_id = p_family_war_long->get_object_id();
				m_family_war_long_info.m_long_state = family_war_long_state_normal;
				m_family_war_long_info.m_refresh_times++;
				// 广播图标变化
				m_family_war_long_info.broadcast_icon();

				uint32_t title_type = 0;
				switch (type)
				{
				case family_war_long_wind:
					{
						title_type = sys_notice_enum::sys_notice_105;
					}
					break;
				case family_war_long_water:
					{
						title_type = sys_notice_enum::sys_notice_106;
					}
					break;
				case family_war_long_fire:
					{
						title_type = sys_notice_enum::sys_notice_107;
					}
					break;
				case family_war_long_gold:
					{
						title_type = sys_notice_enum::sys_notice_108;
					}
					break;
				default:
					break;
				}

				if (title_type != 0)
				{
					BROADCAST_SCENE_NOTICE(get_scene_ptr(), title_type, 0, 0, 0);
				}
			}

			return;
		}
	}
}

void family_war_scene_t::notify_scene_war_info()
{
	proto::client::gc_family_war_fight_info_notify notify;
	peek_family_war_fight_info(notify);
	send_msg_to_scene(op_cmd::gc_family_war_fight_info_notify, notify);
}

void family_war_scene_t::family_occupied_platform(const role_ptr& p_role, family_war_platform_type platform_type, family_war_side_type side_type)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	if (m_state_type != proto::common::family_war_state_start) {
		return;
	}

	if (platform_type >= family_war_platform_max)
	{
		log_error("family_occupied_platform family_war_platform_type[%u] error", platform_type);
		return;
	}

	if (side_type >= family_war_side_max)
	{
		log_error("family_occupied_platform family_war_side_type[%u] error", side_type);
		return;
	}

	switch (side_type)
	{
	case family_war_side_left:
		{
			m_stronghold_side[family_war_side_right].set_platform_state(platform_type, false);	// 设置另一边不占领
		}
		break;
	case family_war_side_right:
		{
			m_stronghold_side[family_war_side_left].set_platform_state(platform_type, false);	// 设置另一边不占领
		}
		break;
	default:
		log_error("family_war_side_type[%u] error", side_type);
		break;
	}

	// 设置占领
	m_stronghold_side[side_type].set_platform_state(platform_type, true);
	// 设置占领家族
	m_family_war_platform[platform_type].m_occupied_side = side_type;

	// 全场景通知，日月台占领
	notify_scene_war_info();

	if (m_platform_timer[platform_type])
	{
		if (m_family_war_platform[platform_type].m_interval != 0)
		{
			m_platform_timer[platform_type]->expires_from_now(boost::posix_time::seconds(m_family_war_platform[platform_type].m_interval));
			m_platform_timer[platform_type]->async_wait(boost::bind(&family_war_scene_t::on_platform_hurt_timer, boost::weak_ptr<family_war_scene_t>(get_family_war_scene_ptr()), platform_type, boost::asio::placeholders::error));
		}
	}

	// 个人战功增加
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_attack_sun_moon);
	uint32_t attack_score = GET_COMPREHENSIVE_VALUE_1(p_conf);

	const family_war_scene_t::family_war_role_info_ptr& p_role_info = get_family_war_role_info(p_role->get_uid());
	if (NULL != p_role_info)
	{
		p_role_info->add_score(p_role, attack_score);

		m_update_rank = true;
	}

	switch (platform_type)
	{
	case family_war_platform_sun:
		{
			BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_110, 0, 0, 0,
				p_role->get_family_name().c_str(), p_role->get_name().c_str());
		}
		break;
	case family_war_platform_moon:
		{
			BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_111, 0, 0, 0,
				p_role->get_family_name().c_str(), p_role->get_name().c_str());
		}
		break;
	default:
		break;
	}

	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_FAMILY_WAR_OCCUPIED_SUN_OR_MOON_NUMBER);
}

bool family_war_scene_t::add_family_score(family_war_side_type type, uint32_t score)
{
	if (type >= family_war_side_max) {
		log_error("family_war_side_type[%u] error", type);
		return false;
	}

	m_stronghold_side[type].add_score(score);

	notify_scene_war_info();

	if (m_stronghold_side[type].is_early_win()) {
		family_manager_t::family_war_early_end(get_family_war_scene_ptr());
		return true;
	}

	return false;
}

void family_war_scene_t::rank_all_score()
{
	if (m_family_war_sort_list.empty())
		return;

	m_family_war_sort_list.sort([](const family_war_role_info_ptr& h1, const family_war_role_info_ptr& h2) {
		if (NULL == h1 || NULL == h2) {
			return false;
		}

		if (h1->m_data.personal_score() != h2->m_data.personal_score())
		{
			return h1->m_data.personal_score() > h2->m_data.personal_score();
		}

		return h1->m_role_uid < h2->m_role_uid;
	});

	uint32_t index = 0;
	for (family_war_rank_list::const_iterator citr = m_family_war_sort_list.begin();
		citr != m_family_war_sort_list.end(); ++citr, ++index)
	{
		const family_war_role_info_ptr& p_info = *citr;
		if (NULL != p_info) {
			p_info->update_rank(index + 1);
		}
	}
}

uint32_t family_war_scene_t::attack_long(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type >= family_war_side_max)
	{
		// 不是家族战成员，却进了家族战副本
		log_error("role[%lu] not family war member", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (m_state_type != proto::common::family_war_state_start)
	{
		log_error("role[%lu] family war is end", p_role->get_uid());
		return common::errcode_enum::family_err_war_state_error;
	}

	if (m_family_war_long_info.m_long_state != family_war_long_state_normal)
	{
		log_error("role[%lu] family war long state error", p_role->get_uid());
		return common::errcode_enum::family_err_war_long_state_error;
	}

	if (!p_role->have_hero_live())
	{
		log_error("role[%lu] have no hero live", p_role->get_uid());
		return common::errcode_enum::family_err_war_have_no_hero;
	}

	family_war_side_type enemy_side = get_enemy_side_type(side_type);
	if (enemy_side == family_war_side_none)
	{
		log_error("role[%lu] get_enemy_side_type error", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (m_family_war_long_info.long_wei(p_role, enemy_side))
	{
		return common::errcode_enum::error_ok;
	}

	monster_ptr p_monster = monster_manager_t::find_monster(m_family_war_long_info.m_object_id);
	if (NULL == p_monster)
	{
		log_error("can not find long[%lu]", m_family_war_long_info.m_object_id);
		return common::errcode_enum::sys_notice_object_not_exsit;
	}

	family_war_long_ptr p_long = p_monster->get_family_war_long();
	if (NULL == p_long)
	{
		log_error("monster[%lu] not family long", m_family_war_long_info.m_object_id);
		return common::errcode_enum::notice_unknown;
	}

	if (p_long->is_die())
	{
		log_error("long[%lu] is already die", m_family_war_long_info.m_object_id);
		return common::errcode_enum::notice_unknown;
	}

	proto::common::fight_param fp;
	fp.set_type(proto::common::fight_type_family_war);
	//fp.mutable_light()->set_monster_id(p_long->get_monster_id());
	fp.mutable_light()->set_object_id(common::string_util_t::uint64_to_string(m_scene_id));	// 场景id
	if (!p_long->get_hero_state_data(fp.mutable_start()->mutable_enemy()))
	{
		log_error("role[%lu] get family_long[%u] hero data error", p_role->get_uid(), p_long->get_monster_id());
		return common::errcode_enum::notice_unknown;
	}

	uint64_t fight_id = fight_manager_t::fight_pve(p_role, p_long->get_against_form_id(), &fp);
	if (fight_id == 0)
	{
		log_error("role[%lu] fight_pve boss[%u] error", p_role->get_uid(), p_long->get_monster_id());
		return common::errcode_enum::notice_unknown;
	}

	p_long->add_combat(fight_id);

	return common::errcode_enum::error_ok;
}

void family_war_scene_t::attack_long_round(uint64_t uid, uint64_t fight_id, const proto::common::fight_camp& enemy_camp, const combat_ptr& p_combat)
{
	if (m_state_type != proto::common::family_war_state_start) {
		return;
	}

	if (p_combat == NULL)
	{
		log_error("p_combat == NULL");
		return;
	}

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("role[%lu] null error", uid);
		return;
	}

	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type >= family_war_side_max)
	{
		// 不是家族战成员，却进了家族战副本
		log_error("role[%lu] not family war member", uid);
		return;
	}

	monster_ptr p_monster = monster_manager_t::find_monster(m_family_war_long_info.m_object_id);
	if (NULL == p_monster)
	{
		log_error("family war long[%lu] not find", m_family_war_long_info.m_object_id);
		return;
	}

	family_war_long_ptr p_family_war_long = p_monster->get_family_war_long();
	if (NULL == p_family_war_long)
	{
		log_error("monster[%lu] not family war long", m_family_war_long_info.m_object_id);
		return;
	}

	if (p_family_war_long->is_die()) {
		return;
	}

	if (p_family_war_long->is_family_occupied()) { // 防止已经在结算在进入下面的函数结算
		return;
	}

	uint32_t hurts = p_family_war_long->update_hero_hp(shared_from_this(), side_type, fight_id, enemy_camp, p_combat);
	if (hurts == 0)	// 正常处理
	{
		return;
	}

	const family_war_scene_t::family_war_role_info_ptr& p_role_info = get_family_war_role_info(uid);
	if (NULL == p_role_info)
	{
		log_error("role[%lu] family_war_role_info_ptr not find", uid);
		return;
	}

	p_role_info->m_all_hurts += hurts;

	if (m_stronghold_side[side_type].m_hurt_max_uid == uid)
	{
		m_stronghold_side[side_type].m_hurt_max_cnt = p_role_info->m_all_hurts;
		m_stronghold_side[side_type].m_data.set_hurt_max_value(p_role_info->m_all_hurts);
	}
	else
	{
		if (p_role_info->m_all_hurts > m_stronghold_side[side_type].m_hurt_max_cnt)
		{
			m_stronghold_side[side_type].m_hurt_max_uid = uid;
			m_stronghold_side[side_type].m_hurt_max_cnt = p_role_info->m_all_hurts;

			m_stronghold_side[side_type].m_data.set_hurt_max_member(p_role->get_name());
			m_stronghold_side[side_type].m_data.set_hurt_max_value(p_role_info->m_all_hurts);
		}
	}

	achieve_common_t::notify_progress_state(uid, proto::common::ACHIEVE_FAMILY_WAR_DRAGON_HARM, 0, hurts);
}

void family_war_scene_t::attack_long_end(uint64_t uid, uint64_t fight_id, const combat_ptr& p_combat, bool is_win)
{
	if (m_state_type != proto::common::family_war_state_start) {
		return;
	}

	if (p_combat == NULL)
	{
		log_error("p_combat == NULL");
		return;
	}

	monster_ptr p_monster = monster_manager_t::find_monster(m_family_war_long_info.m_object_id);
	if (NULL == p_monster)
	{
		log_error("family war long[%lu] not find", m_family_war_long_info.m_object_id);
		return;
	}

	family_war_long_ptr p_family_war_long = p_monster->get_family_war_long();
	if (NULL == p_family_war_long)
	{
		log_error("monster[%lu] not family war long", m_family_war_long_info.m_object_id);
		return;
	}

	if (!p_family_war_long->is_die() && !p_family_war_long->is_family_occupied()) {
		p_family_war_long->del_combat(fight_id);
	}

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("role[%lu] null error", uid);
		return;
	}

	const family_war_scene_t::family_war_role_info_ptr& p_role_info = get_family_war_role_info(uid);
	if (NULL == p_role_info)
	{
		log_error("role[%lu] family_war_role_info_ptr not find", uid);
		return;
	}

	// 判断是否被龙柱打死
	//bool is_all_hero_die = true;
	//for (const auto& heropair : p_combat->get_all_heros())
	//{
	//	if (heropair.second == NULL)
	//		continue;

	//	if (heropair.second->get_camp() == proto::common::combat_camp_1)
	//	{
	//		if (heropair.second->is_dead())
	//		{
	//			p_role_info->role_hero_die(p_role, heropair.second->get_unique_id());
	//		}
	//		else
	//		{
	//			is_all_hero_die = false;
	//		}
	//	}
	//}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_attack_long);
	uint32_t attack_long_score = GET_COMPREHENSIVE_VALUE_1(p_conf);

	// 个人战功
	p_role_info->add_score(p_role, attack_long_score);
	m_update_rank = true;

	achieve_common_t::notify_progress_state(uid, proto::common::ACHIEVE_FAMILY_WAR_NUMBER);

	// 设置死亡
	if (!is_win /*|| is_all_hero_die*/)
	{
		on_other_kill_me(p_role, p_family_war_long);
	}
}

void family_war_scene_t::family_war_long_death(uint64_t fight_id)
{
	if (m_state_type != proto::common::family_war_state_start) {
		return;
	}

	monster_ptr p_monster = monster_manager_t::find_monster(m_family_war_long_info.m_object_id);
	if (NULL == p_monster)
	{
		log_error("family war long[%lu] not find", m_family_war_long_info.m_object_id);
		return;
	}

	family_war_long_ptr p_family_war_long = p_monster->get_family_war_long();
	if (NULL == p_family_war_long)
	{
		log_error("monster[%lu] not family war long", m_family_war_long_info.m_object_id);
		return;
	}

	if (p_family_war_long->is_die()) {
		return;
	}

	if (p_family_war_long->is_family_occupied()) { // 防止已经在结算在进入下面的函数结算
		return;
	}

	family_war_side_type side_type = p_family_war_long->get_win_side();
	if (side_type == family_war_side_none)
	{
		log_error("family war get_win_side type error");
		return;
	}

	family_war_side_type enemy_side = get_enemy_side_type(side_type);
	if (enemy_side == family_war_side_none)
	{
		log_error("get_enemy_side_type error");
		return;
	}

	// 设置死亡
	p_family_war_long->set_object_die(m_stronghold_side[side_type].m_family_id);
	// 设置龙柱死亡状态
	m_family_war_long_info.m_long_state = family_war_long_state_die;
	// 广播图标变化
	m_family_war_long_info.broadcast_icon();

	BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_109, 0, 0, 0, m_stronghold_side[side_type].get_family_name().c_str(), m_stronghold_side[enemy_side].get_family_name().c_str());
	
	p_family_war_long->set_all_fight_success(fight_id);

	m_stronghold_side[side_type].add_occupied_long();

	// 计算伤害（目前改为伤害就是积分）
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_long_hurt);
	uint32_t base_hurt = GET_COMPREHENSIVE_VALUE_1(p_conf);
	uint32_t real_hurt = base_hurt * p_family_war_long->get_hurt_pre(side_type);

	// 日月之力加成
	if (m_family_war_platform[family_war_platform_sun].m_occupied_side == side_type && m_family_war_platform[family_war_platform_moon].m_occupied_side == side_type)
	{
		p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_sun_moon_increase);
		uint32_t increase = GET_COMPREHENSIVE_VALUE_1(p_conf);

		real_hurt = (increase / 100.f + 1) * real_hurt;
	}

	if (add_family_score(side_type, real_hurt))
	{
		return;
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_long_revive_time);
	uint32_t count_down = GET_COMPREHENSIVE_VALUE_1(p_conf);

	if (NULL != m_long_timer)
	{
		if (count_down > 0)
		{
			m_long_timer->expires_from_now(boost::posix_time::seconds(count_down));
			m_long_timer->async_wait(boost::bind(&family_war_scene_t::on_long_refresh_timer, boost::weak_ptr<family_war_scene_t>(get_family_war_scene_ptr()), boost::asio::placeholders::error));
		}

		BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_104, 0, 0, 0, count_down);
	}
}

std::pair<family_war_kill_title, uint32_t> family_war_scene_t::get_kill_title(uint32_t kill_num)
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_kill_title4);
	uint32_t need_num = GET_COMPREHENSIVE_VALUE_1(p_conf);
	uint32_t title = GET_COMPREHENSIVE_VALUE_3(p_conf);
	if (kill_num >= need_num)
	{
		return std::make_pair(family_war_kill_title_4, title);
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_kill_title3);
	need_num = GET_COMPREHENSIVE_VALUE_1(p_conf);
	title = GET_COMPREHENSIVE_VALUE_3(p_conf);
	if (kill_num >= need_num)
	{
		return std::make_pair(family_war_kill_title_3, title);
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_kill_title2);
	need_num = GET_COMPREHENSIVE_VALUE_1(p_conf);
	title = GET_COMPREHENSIVE_VALUE_3(p_conf);
	if (kill_num >= need_num)
	{
		return std::make_pair(family_war_kill_title_2, title);
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_kill_title1);
	need_num = GET_COMPREHENSIVE_VALUE_1(p_conf);
	title = GET_COMPREHENSIVE_VALUE_3(p_conf);
	if (kill_num >= need_num)
	{
		return std::make_pair(family_war_kill_title_1, title);
	}

	return std::make_pair(family_war_kill_title_0, 0);
}

uint32_t family_war_scene_t::get_reward_by_self_title(family_war_kill_title title)
{
	uint32_t add_score = 0;
	switch (title)
	{
	case family_war_kill_title_0:
		break;
	case family_war_kill_title_1:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_kill_score1);
		add_score = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	case family_war_kill_title_2:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_kill_score2);
		add_score = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	case family_war_kill_title_3:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_kill_score3);
		add_score = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	case family_war_kill_title_4:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_kill_score4);
		add_score = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	default:
	{
		log_error("title[%u] not find", title);
	}
	break;
	}

	return add_score;
}

uint32_t family_war_scene_t::get_reward_by_other_title(family_war_kill_title title)
{
	uint32_t add_score = 0;
	switch (title)
	{
	case family_war_kill_title_0:
		break;
	case family_war_kill_title_1:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_end_kill_score1);
		add_score = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	case family_war_kill_title_2:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_end_kill_score2);
		add_score = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	case family_war_kill_title_3:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_end_kill_score3);
		add_score = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	case family_war_kill_title_4:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_end_kill_score4);
		add_score = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	break;
	default:
	{
		log_error("title[%u] not find", title);
	}
	break;
	}

	return add_score;
}

std::string family_war_scene_t::get_title_name(family_war_kill_title title)
{
	switch (title)
	{
	case family_war_kill_title_0:
		break;
	case family_war_kill_title_1:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_kill_title1);
		if (p_conf)
		{
			return p_conf->parameter1();
		}
	}
	break;
	case family_war_kill_title_2:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_kill_title2);
		if (p_conf)
		{
			return p_conf->parameter1();
		}
	}
	break;
	case family_war_kill_title_3:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_kill_title3);
		if (p_conf)
		{
			return p_conf->parameter1();
		}
	}
	break;
	case family_war_kill_title_4:
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_kill_title4);
		if (p_conf)
		{
			return p_conf->parameter1();
		}
	}
	break;
	default:
	{
		log_error("title[%u] not find", title);
	}
	break;
	}

	return "";
}

uint32_t family_war_scene_t::get_long_type_monster_tid(family_war_long_type type)
{
	uint32_t monster_tid = 0;
	switch (type)
	{
	case family_war_long_wind:
		{
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_monster_wind);
			monster_tid = GET_COMPREHENSIVE_VALUE_1(p_conf);
		}
		break;
	case family_war_long_water:
		{
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_monster_water);
			monster_tid = GET_COMPREHENSIVE_VALUE_1(p_conf);
		}
		break;
	case family_war_long_fire:
		{
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_monster_fire);
			monster_tid = GET_COMPREHENSIVE_VALUE_1(p_conf);
		}
		break;
	case family_war_long_gold:
		{
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_monster_gold);
			monster_tid = GET_COMPREHENSIVE_VALUE_1(p_conf);
		}
		break;
	default:
		log_error("family_war_long_type[%u] error", type);
		break;
	}

	return monster_tid;
}

void family_war_scene_t::peek_family_war_fight_data(uint64_t uid, proto::client::gc_family_war_fight_data_reply& reply)
{
	const family_war_scene_t::family_war_role_info_ptr& p_role_info = get_family_war_role_info(uid);
	if (NULL != p_role_info)
	{
		proto::client::family_war_role_data* p_role_data = reply.mutable_role_data();
		if (p_role_data)
		{
			p_role_data->CopyFrom(p_role_info->m_data);
		}
	}

	proto::client::family_war_fight_data* left_data = reply.mutable_left_family();
	if (left_data)
	{
		left_data->CopyFrom(m_stronghold_side[family_war_side_left].m_data);
	}

	proto::client::family_war_fight_data* right_data = reply.mutable_right_family();
	if (right_data)
	{
		right_data->CopyFrom(m_stronghold_side[family_war_side_right].m_data);
	}
}

uint32_t family_war_scene_t::role_enter_battlefield(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type >= family_war_side_max)
	{
		// 不是家族战成员，却进了家族战副本
		log_error("role[%lu] not family war member", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (m_state_type == proto::common::family_war_state_prepare)
	{
		FAMILY_LOG("role[%lu] family war is prepare", p_role->get_uid());
		return common::errcode_enum::family_err_war_state_not_start;
	}

	if (m_state_type == proto::common::family_war_state_end)
	{
		FAMILY_LOG("role[%lu] family war is end", p_role->get_uid());
		return common::errcode_enum::family_err_war_state_error;
	}

	family_war_role_info_ptr p_role_info = get_family_war_role_info(p_role->get_uid());
	if (NULL == p_role_info)
	{
		log_error("role[%lu] can't find role info", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (p_role->get_role_area_id() != m_stronghold_side[side_type].m_area_id)
	{
		log_error("p_role[%lu] area error", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (p_role_info->m_data.action_point() == 0)
	{
		FAMILY_LOG("p_role[%lu] action_point == 0", p_role->get_uid());
		return common::errcode_enum::family_err_war_action_point_zero;
	}

	if (!p_role->have_hero_live())
	{
		FAMILY_LOG("role[%lu] have no hero live", p_role->get_uid());
		return common::errcode_enum::family_err_war_have_no_hero;
	}

	// 传送到据点
	scene_manager_t::role_jump_point(p_role, m_stronghold_side[side_type].m_war_id);

	return common::errcode_enum::error_ok;
}

void family_war_scene_t::get_family_war_result(uint64_t uid, proto::client::gc_family_war_result_info_reply& reply)
{
	if (m_state_type != proto::common::family_war_state_end)
	{
		log_error("role[%lu] family war is not end", uid);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const family_war_scene_t::family_war_role_info_ptr& p_role_info = get_family_war_role_info(uid);
	if (NULL != p_role_info)
	{
		proto::client::family_war_role_data* p_role_data = reply.mutable_role_data();
		if (p_role_data)
		{
			p_role_data->CopyFrom(p_role_info->m_data);
		}
	}

	proto::client::family_war_fight_data* left_data = reply.mutable_left_family();
	if (left_data)
	{
		left_data->CopyFrom(m_stronghold_side[family_war_side_left].m_data);
	}

	proto::client::family_war_fight_data* right_data = reply.mutable_right_family();
	if (right_data)
	{
		right_data->CopyFrom(m_stronghold_side[family_war_side_right].m_data);
	}

	reply.set_win_family_id(common::string_util_t::uint64_to_string(m_win_family));

	reply.set_reply_code(common::errcode_enum::error_ok);
}

void family_war_scene_t::on_role_enter(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type >= family_war_side_max)
	{
		// 不是家族战成员，却进了家族战副本
		log_error("role[%lu] enter family war scene[%lu], not family war member", p_role->get_uid(), m_scene_id);
		p_role->change_pk_mode(proto::common::PK_PEACE);
		return;
	}

	family_war_role_info_ptr p_role_info = get_family_war_role_info(p_role->get_uid());
	if (NULL == p_role_info)
	{
		p_role_info = add_family_war_role_info(p_role->get_uid());
	}

	if (NULL == p_role_info)
	{
		log_error("role[%lu] add family_war_role_info_ptr null error", p_role->get_uid());
		p_role->change_pk_mode(proto::common::PK_PEACE);
		return;
	}

	// 改变pk模式的时候一起同步
	p_role->change_temporary_title(p_role_info->m_kill_title.second, false);
	p_role->change_pk_mode(proto::common::PK_FAMILY, side_type);

	// 初始化人物状态
	p_role->set_in_family_war(true);
	p_role_info->init_role_heros(p_role);

	// 更新家族人数
	m_stronghold_side[side_type].add_war_member(p_role->get_uid());

	// 武将信息
	proto::client::gc_formations_info_notify formations_notify;
	p_role_info->peek_hero_data(formations_notify);
	p_role->send_msg_to_client(op_cmd::gc_formations_info_notify, formations_notify);

	// 战斗信息 
	proto::client::gc_family_war_fight_info_notify notify;
	peek_family_war_fight_info(notify);
	p_role->send_msg_to_client(op_cmd::gc_family_war_fight_info_notify, notify);

	if (m_state_type == proto::common::family_war_state_start) {
		// 龙柱信息
		if (m_family_war_long_info.m_object_id != 0) {
			monster_ptr p_monster = monster_manager_t::find_monster(m_family_war_long_info.m_object_id);
			if (p_monster) {
				family_war_long_ptr p_long = p_monster->get_family_war_long();
				if (p_long) {
					proto::client::gc_family_long_info_notify long_info_notify;
					p_long->peek_hp_info(long_info_notify);
					p_role->send_msg_to_client(op_cmd::gc_family_long_info_notify, long_info_notify);
				} else {
					log_error("monster[%lu] not family long", m_family_war_long_info.m_object_id);
				}
			} else {
				log_error("monster[%lu] not family long", m_family_war_long_info.m_object_id);
			}
		} else {
			// 开战后多久刷新
			proto::client::gc_family_long_info_notify long_info_notify;
			long_info_notify.set_object_id(common::string_util_t::uint64_to_string(m_family_war_long_info.m_object_id));
			long_info_notify.set_revive_time(m_family_war_long_info.m_refresh_time);
			p_role->send_msg_to_client(op_cmd::gc_family_long_info_notify, long_info_notify);
		}

		// 日月台信息
		proto::client::gc_family_platform_info_notify platform_info_notify;
		proto::common::family_object_pos* sun_pos = platform_info_notify.mutable_sun_pos();
		if (sun_pos) {
			sun_pos->CopyFrom(m_family_war_platform[family_war_platform_sun].m_pos);
		}

		proto::common::family_object_pos* moon_pos = platform_info_notify.mutable_moon_pos();
		if (moon_pos) {
			moon_pos->CopyFrom(m_family_war_platform[family_war_platform_moon].m_pos);
		}
		p_role->send_msg_to_client(op_cmd::gc_family_platform_info_notify, platform_info_notify);
	}

	if (p_role->get_role_area_id() != m_stronghold_side[side_type].m_area_id)
	{
		scene_manager_t::role_jump_point(p_role, m_stronghold_side[side_type].m_jump_id);
	}
}

void family_war_scene_t::on_role_leave(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	p_role->set_in_family_war(false);

	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type < family_war_side_max)
	{
		m_stronghold_side[side_type].del_war_member(p_role->get_uid());

		if (m_family_war_long_info.m_area_id == p_role->get_role_area_id())
		{
			m_family_war_long_info.del_nearby_role(p_role->get_uid(), side_type);
		}
	}
}

void family_war_scene_t::role_leave_area(role_ptr p_role, uint32_t area_id)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return;
	}

	if (m_family_war_long_info.m_area_id == area_id) {
		family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
		if (side_type < family_war_side_max) {
			m_family_war_long_info.del_nearby_role(p_role->get_uid(), side_type);
			if (m_state_type == proto::common::family_war_state_start && m_family_war_long_info.m_long_state == family_war_long_state::family_war_long_state_normal) {
				proto::client::gc_attack_icon_notify ntf;
				ntf.set_type(0);
				ntf.set_long_uid(common::string_util_t::uint64_to_string(m_family_war_long_info.m_object_id));
				p_role->send_msg_to_client(op_cmd::gc_attack_icon_notify, ntf);
			}
		}
	}
}

void family_war_scene_t::role_enter_area(role_ptr p_role, uint32_t area_id)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return;
	}

	if (m_family_war_long_info.m_area_id == area_id) {
		family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
		if (side_type < family_war_side_max) {
			m_family_war_long_info.add_nearby_role(p_role->get_uid(), side_type);
			if (m_state_type == proto::common::family_war_state_start && m_family_war_long_info.m_long_state == family_war_long_state::family_war_long_state_normal) {
				proto::client::gc_attack_icon_notify ntf;
				ntf.set_type(1);
				ntf.set_long_uid(common::string_util_t::uint64_to_string(m_family_war_long_info.m_object_id));
				p_role->send_msg_to_client(op_cmd::gc_attack_icon_notify, ntf);
			}
		}
	}
}

void family_war_scene_t::on_revive_role(role_ptr p_role)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type < family_war_side_max)
	{
		// 复活点复活
		scene_manager_t::role_jump_point(p_role, m_stronghold_side[side_type].m_jump_id);
	}
	else
	{
		// 基类调用
		scene_t::on_revive_role(p_role);
	}
}

void family_war_scene_t::on_role_kill_other(role_ptr p_role, object_base_ptr p_enemy)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	if (NULL == p_enemy)
	{
		log_error("p_enemy null error");
		return;
	}

	if (m_state_type != proto::common::family_war_state_start) {
		return;
	}

	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type >= family_war_side_max)
	{
		log_error("role[%lu] family_war_side_type[%u] error", p_role->get_uid(), (uint32_t)side_type);
		return;
	}

	if (p_enemy->is_role())
	{
		role_ptr p_role_enemy = p_enemy->get_role();
		if (NULL == p_role_enemy)
		{
			log_error("p_enemy get role error");
			return;
		}

		const family_war_scene_t::family_war_role_info_ptr& p_role_info = get_family_war_role_info(p_role->get_uid());
		if (NULL == p_role_info)
		{
			log_error("role[%lu] family_war_role_info_ptr not find", p_role->get_uid());
			return;
		}

		const combat_ptr& p_combat = fight_manager_t::get_combat(p_role->get_fight_uid());
		if (NULL != p_combat)
		{
			std::set<uint64_t> death_hero_uids;
			p_combat->get_self_death_hero(p_role->get_uid(), death_hero_uids);

			for (std::set<uint64_t>::const_iterator citr = death_hero_uids.begin();
				citr != death_hero_uids.end(); ++citr)
			{
				p_role_info->role_hero_die(p_role, *citr);
			}
		}

		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_win_score);
		uint32_t win_score = GET_COMPREHENSIVE_VALUE_1(p_conf);
		p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_scorelimit1);
		uint32_t fight_scorelimit_max = GET_COMPREHENSIVE_VALUE_1(p_conf);
		float fight_scorelimit_max_precent = fight_scorelimit_max / 10000.0f;
		p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_scorelimit2);
		uint32_t fight_scorelimit_min = GET_COMPREHENSIVE_VALUE_1(p_conf);
		float fight_scorelimit_min_precent = fight_scorelimit_min / 10000.0f;

		float fight_precent = fight_scorelimit_min_precent;
		if (p_role->get_fighting() != 0) {
			fight_precent = (float)p_role_enemy->get_fighting() / (float)p_role->get_fighting();

			if (fight_precent > fight_scorelimit_max_precent) {
				fight_precent = fight_scorelimit_max_precent;
			} else if (fight_precent < fight_scorelimit_min_precent) {
				fight_precent = fight_scorelimit_min_precent;
			}
		}

		uint32_t add_score = win_score * fight_precent;

		p_role_info->add_one_kill();

		p_role_info->m_kill_title = get_kill_title(p_role_info->m_kills);
		p_role->change_temporary_title(p_role_info->m_kill_title.second); // 更新称号
		add_score += get_reward_by_self_title(p_role_info->m_kill_title.first);

		family_war_kill_title enemy_title = family_war_kill_title_0;
		const family_war_scene_t::family_war_role_info_ptr& p_enemy_info = get_family_war_role_info(p_role_enemy->get_uid());
		if (NULL != p_enemy_info)
		{
			enemy_title = p_enemy_info->m_kill_title.first;

			if (enemy_title != family_war_kill_title_0)
			{
				p_enemy_info->m_kill_title = std::make_pair(family_war_kill_title_0, 0);
				p_role_enemy->change_temporary_title(p_enemy_info->m_kill_title.second);// 更新称号
			}
		}
		add_score += get_reward_by_other_title(enemy_title);

		// 个人战功
		p_role_info->add_score(p_role, add_score);
		m_update_rank = true;

		m_stronghold_side[side_type].add_fight_win_cnt();

		if (m_stronghold_side[side_type].m_kill_max_uid == p_role->get_uid())
		{
			m_stronghold_side[side_type].m_kill_max_cnt = p_role_info->m_kill_count;
			m_stronghold_side[side_type].m_data.set_kill_max_value(p_role_info->m_kill_count);
		}
		else if (p_role_info->m_kill_count > m_stronghold_side[side_type].m_kill_max_cnt)
		{
			m_stronghold_side[side_type].m_kill_max_uid = p_role->get_uid();
			m_stronghold_side[side_type].m_kill_max_cnt = p_role_info->m_kill_count;

			m_stronghold_side[side_type].m_data.set_kill_max_member(p_role->get_name());
			m_stronghold_side[side_type].m_data.set_kill_max_value(p_role_info->m_kill_count);
		}

		BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_101, 0, 0, 0,
			p_role->get_family_name().c_str(), p_role->get_name().c_str(),
			p_role_enemy->get_family_name().c_str(), p_role_enemy->get_name().c_str());

		if (p_role_info->m_kill_title.first != family_war_kill_title_0)
		{
			BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_102, 0, 0, 0,
				p_role->get_family_name().c_str(), p_role->get_name().c_str(),
				common::string_util_t::convert_to_utf8(get_title_name(p_role_info->m_kill_title.first)).c_str());
		}

		if (enemy_title != family_war_kill_title_0)
		{
			BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_102, 0, 0, 0,
				p_role_enemy->get_family_name().c_str(), p_role_enemy->get_name().c_str(),
				common::string_util_t::convert_to_utf8(get_title_name(enemy_title)).c_str());
		}

		// 家族积分
		if (add_family_score(side_type, add_score))
		{
			return;
		}
	}

	if (!p_role->have_hero_live())
	{
		// 传送到据点
		scene_manager_t::role_jump_point(p_role, m_stronghold_side[side_type].m_jump_id);
	}
}

void family_war_scene_t::on_other_kill_me(role_ptr p_role, object_base_ptr p_enemy)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	if (p_enemy == NULL)
	{
		log_error("p_enemy null error");
		return;
	}

	if (m_state_type != proto::common::family_war_state_start) {
		return;
	}

	p_role->set_object_die(p_enemy->get_object_id());

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_revive_time);
	uint32_t revive_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	p_role->set_revive_time(revive_time);

	show_revive_ui(p_role);

	if (p_enemy->is_role())
	{
		role_ptr p_role_enemy = p_enemy->get_role();
		if (NULL == p_role_enemy)
		{
			log_error("p_enemy get role error");
			return;
		}

		const family_war_scene_t::family_war_role_info_ptr& p_role_info = get_family_war_role_info(p_role->get_uid());
		if (NULL == p_role_info)
		{
			log_error("role[%lu] family_war_role_info_ptr not find", p_role->get_uid());
			return;
		}

		const combat_ptr& p_combat = fight_manager_t::find_combat(p_role->get_fight_uid());
		if (NULL != p_combat)
		{
			std::set<uint64_t> death_hero_uids;
			p_combat->get_self_death_hero(p_role->get_uid(), death_hero_uids);

			for (std::set<uint64_t>::const_iterator citr = death_hero_uids.begin();
				citr != death_hero_uids.end(); ++citr)
			{
				p_role_info->role_hero_die(p_role, *citr);
			}
		}

		p_role_info->add_self_death();

		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_dead_point);
		uint32_t dead_point = GET_COMPREHENSIVE_VALUE_1(p_conf);

		p_role_info->less_action_point(p_role, dead_point, false);

		p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_failed_score);
		uint32_t failed_score = GET_COMPREHENSIVE_VALUE_1(p_conf);

		p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_scorelimit1);
		uint32_t fight_scorelimit_max = GET_COMPREHENSIVE_VALUE_1(p_conf);
		float fight_scorelimit_max_precent = fight_scorelimit_max / 10000.0f;
		p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_fight_scorelimit2);
		uint32_t fight_scorelimit_min = GET_COMPREHENSIVE_VALUE_1(p_conf);
		float fight_scorelimit_min_precent = fight_scorelimit_min / 10000.0f;

		float fight_precent = fight_scorelimit_min_precent;
		if (p_role->get_fighting() != 0) {
			fight_precent = (float)p_role_enemy->get_fighting() / (float)p_role->get_fighting();

			if (fight_precent > fight_scorelimit_max_precent) {
				fight_precent = fight_scorelimit_max_precent;
			} else if (fight_precent < fight_scorelimit_min_precent) {
				fight_precent = fight_scorelimit_min_precent;
			}
		}

		uint32_t add_score = failed_score * fight_precent;

		// 个人战功
		p_role_info->add_score(p_role, add_score);
		m_update_rank = true;

		family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
		if (side_type < family_war_side_max)
		{
			// 家族积分
			if (add_family_score(side_type, add_score))
			{
				return;
			}
		}
	}
}

void family_war_scene_t::show_revive_ui(role_ptr p_role)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	uint32_t time_now = common::time_util_t::now_time();

	proto::client::gc_object_death_notify notify;
	notify.set_revive_type(proto::common::object_revive_family_war);

	if (p_role->get_revive_time() > time_now)
	{
		notify.set_revive_time(p_role->get_revive_time() - time_now);
	}
	else
	{
		notify.set_revive_time(0);
	}

	p_role->send_msg_to_client(op_cmd::gc_object_death_notify, notify);
}

void family_war_scene_t::on_login_die(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("role null error");
		return;
	}

	// 没死亡
	if (!p_role->is_die())
	{
		log_error("role[%lu] not die", p_role->get_uid());
		return;
	}

	// 弹出复活界面
	show_revive_ui(p_role);
}

family_war_scene_t::family_war_role_info_t::family_war_role_info_t(uint64_t uid) : m_role_uid(uid), m_kill_title(family_war_kill_title::family_war_kill_title_0, 0)
{

}

void family_war_scene_t::family_war_role_info_t::init()
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_action_point);
	uint32_t point = GET_COMPREHENSIVE_VALUE_1(p_conf);

	m_data.set_action_point(point);
	m_data.set_personal_score(0);
	m_data.set_score_rank(0);
	m_data.set_kill_count(0);
	m_data.set_death_count(0);
	m_data.set_kills(0);
}

void family_war_scene_t::family_war_role_info_t::peek_hero_data(proto::client::gc_formations_info_notify& notify, bool peek_hero /*= true*/)
{
	proto::client::family_war_role_data* p_role_data = notify.mutable_role_data();
	if (p_role_data)
	{
		p_role_data->CopyFrom(m_data);
	}

	if (peek_hero)
	{
		for (const auto& hero : m_heros)
		{
			proto::common::family_war_hero_data* data = notify.add_data();
			if (data)
			{
				data->set_hero_uid(common::string_util_t::uint64_to_string(hero.first));
				data->set_type(hero.second);
			}
		}

		m_update_hero = false;
	}
}

void family_war_scene_t::family_war_role_info_t::peek_hero_data(proto::client::gc_ask_formations_info_reply& notify)
{
	for (const auto& hero : m_heros)
	{
		proto::common::family_war_hero_data* data = notify.add_data();
		if (data)
		{
			data->set_hero_uid(common::string_util_t::uint64_to_string(hero.first));
			data->set_type(hero.second);
		}
	}

	m_update_hero = false;
}

void family_war_scene_t::family_war_role_info_t::peek_hero_data(proto::client::gc_formations_hero_update_reply& notify)
{
	for (const auto& hero : m_heros)
	{
		proto::common::family_war_hero_data* data = notify.add_data();
		if (data)
		{
			data->set_hero_uid(common::string_util_t::uint64_to_string(hero.first));
			data->set_type(hero.second);
		}
	}

	m_update_hero = false;
}

bool family_war_scene_t::family_war_role_info_t::have_hero_live()
{
	for (const auto& hero : m_heros)
	{
		if (hero.second == proto::common::family_war_hero_normal)
		{
			return true;
		}
	}

	return false;
}

void family_war_scene_t::family_war_role_info_t::init_role_heros(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	std::set<uint64_t> hero_map;

	for (const auto& hero : m_heros)
	{
		if (hero.second == proto::common::family_war_hero_normal)
		{
			hero_map.insert(hero.first);
		}
	}

	p_role->set_family_war_heros(hero_map);
}

void family_war_scene_t::family_war_role_info_t::notify_role_info(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	proto::client::gc_formations_info_notify notify;
	peek_hero_data(notify, m_update_hero);
	p_role->send_msg_to_client(op_cmd::gc_formations_info_notify, notify);

	// printf("\033[31m role[%s] action_point[%u] personal_score[%u] score_rank[%u] \033[0m\n", p_role->get_name().c_str(), m_data.action_point(), m_data.personal_score(), m_data.score_rank());
}

uint32_t family_war_scene_t::family_war_role_info_t::role_hero_up(const role_ptr& p_role, uint64_t hero_uid)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	// 主公武将自动上阵不能布阵
	if (p_role->get_main_hero_uid() == hero_uid)
	{
		log_error("p_role[%lu] main hero auto up", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_hero_max);
	uint32_t hero_max = GET_COMPREHENSIVE_VALUE_1(p_conf);

	if (m_heros.size() >= hero_max)
	{
		log_error("role[%lu] hero size[%u] >= %u", p_role->get_uid(), (uint32_t)m_heros.size(), hero_max);
		return common::errcode_enum::family_err_war_hero_max_cnt;
	}

	std::map<uint64_t, proto::common::family_war_hero_type>::iterator itr = m_heros.find(hero_uid);
	if (itr != m_heros.end())
	{
		log_error("role[%lu] hero_uid[%lu] is already up", p_role->get_uid(), hero_uid);
		return common::errcode_enum::family_err_war_hero_already_in_list;
	}

	hero_ptr p_hero = p_role->get_hero_by_unique_id(hero_uid);
	if (NULL == p_hero)
	{
		log_error("role[%lu] hero_uid[%lu] not find", p_role->get_uid(), hero_uid);
		return common::errcode_enum::family_err_war_no_hero;
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_hero_level);
	uint32_t hero_level = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (p_hero->get_current_level() < hero_level)
	{
		log_error("role[%lu] hero_uid[%lu] level < %u", p_role->get_uid(), hero_uid, hero_level);
		return common::errcode_enum::family_err_war_hero_level;
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_hero_star);
	uint32_t hero_star = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (p_hero->get_star() < hero_star)
	{
		log_error("role[%lu] hero_uid[%lu] star < %u", p_role->get_uid(), hero_uid, hero_star);
		return common::errcode_enum::family_err_war_hero_star;
	}

	m_heros[hero_uid] = proto::common::family_war_hero_normal;

	p_role->add_family_war_hero(hero_uid);

	m_update_hero = true;

	return common::errcode_enum::error_ok;
}

uint32_t family_war_scene_t::family_war_role_info_t::role_hero_down(const role_ptr& p_role, uint64_t hero_uid)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	std::map<uint64_t, proto::common::family_war_hero_type>::iterator itr = m_heros.find(hero_uid);
	if (itr == m_heros.end())
	{
		log_error("role[%lu] hero_uid[%lu] is not find", p_role->get_uid(), hero_uid);
		return common::errcode_enum::family_err_war_hero_not_in_list;
	}

	m_heros.erase(itr);

	p_role->del_family_war_hero(hero_uid);

	m_update_hero = true;

	return common::errcode_enum::error_ok;
}

uint32_t family_war_scene_t::family_war_role_info_t::role_hero_revive(const role_ptr& p_role, uint64_t hero_uid)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	std::map<uint64_t, proto::common::family_war_hero_type>::iterator itr = m_heros.find(hero_uid);
	if (itr == m_heros.end())
	{
		log_error("role[%lu] hero_uid[%lu] is not in list", p_role->get_uid(), hero_uid);
		return common::errcode_enum::family_err_war_hero_not_in_list;
	}

	if (itr->second == proto::common::family_war_hero_normal)
	{
		log_error("role[%lu] hero_uid[%lu] not die", p_role->get_uid(), hero_uid);
		return common::errcode_enum::family_err_war_hero_not_die;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_hero_revive);
	uint32_t hero_revive_point = GET_COMPREHENSIVE_VALUE_1(p_conf);

	uint32_t point = m_data.action_point();
	if (point < hero_revive_point)
	{
		log_error("role[%lu] action[%u] point insufficient", p_role->get_uid(), point);
		return common::errcode_enum::family_err_war_action_point_insufficient;
	}

	m_data.set_action_point(point - hero_revive_point);

	itr->second = proto::common::family_war_hero_normal;

	p_role->add_family_war_hero(hero_uid);

	m_update_hero = true;

	return common::errcode_enum::error_ok;
}

void family_war_scene_t::family_war_role_info_t::role_hero_die(const role_ptr& p_role, uint64_t hero_uid)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	// 主公武将不死
	if (p_role->get_main_hero_uid() == hero_uid)
	{
		return;
	}

	std::map<uint64_t, proto::common::family_war_hero_type>::iterator itr = m_heros.find(hero_uid);
	if (itr == m_heros.end())
	{
		log_error("role[%lu] hero_uid[%lu] is not in list", m_role_uid, hero_uid);
		return;
	}

	itr->second = proto::common::family_war_hero_death;

	p_role->del_family_war_hero(hero_uid);

	m_update_hero = true;
}

void family_war_scene_t::family_war_role_info_t::update_rank(uint32_t rank)
{
	if (rank != m_data.score_rank())
	{
		m_data.set_score_rank(rank);

		const role_ptr& p_role = role_manager_t::find_role(m_role_uid);
		if (NULL != p_role)
		{
			notify_role_info(p_role);
		}
	}
}

void family_war_scene_t::family_war_role_info_t::add_score(const role_ptr& p_role, uint32_t score)
{
	m_data.set_personal_score(m_data.personal_score() + score);

	if (NULL != p_role)
	{
		notify_role_info(p_role);
	}
}

void family_war_scene_t::family_war_role_info_t::less_action_point(const role_ptr& p_role, uint32_t val, bool is_update /*= true*/)
{
	if (val < m_data.action_point())
	{
		m_data.set_action_point(m_data.action_point() - val);
	}
	else
	{
		m_data.set_action_point(0);
	}

	if (NULL != p_role && is_update)
	{
		notify_role_info(p_role);
	}
}

void family_war_scene_t::family_war_role_info_t::add_one_kill()
{
	m_kill_count++;
	m_kills++;

	m_data.set_kill_count(m_kill_count);

	if (m_kills > m_data.kills())
	{
		m_data.set_kills(m_kills);
	}
	achieve_common_t::notify_progress_state(m_role_uid, proto::common::ACHIEVE_FAMILY_WAR_KILL_PLAYER_NUMBER);
}

void family_war_scene_t::family_war_role_info_t::add_self_death()
{
	m_death_count++;
	m_kills = 0;

	m_data.set_death_count(m_death_count);
}

void family_war_scene_t::on_long_refresh_timer(const boost::weak_ptr<family_war_scene_t>& p_wlong, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	family_war_scene_ptr p_family_war_scene(p_wlong.lock());
	if (NULL == p_family_war_scene)
	{
		log_error("p_family_war_scene null error");
		return;
	}

	p_family_war_scene->on_update_long_state();
}

void family_war_scene_t::on_long_refresh()
{
	uint32_t prec = 0;
	if (m_family_war_long_info.m_refresh_times == 0)
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_call_gold_probability_1);
		prec = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	else if (m_family_war_long_info.m_refresh_times == 1)
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_call_gold_probability_2);
		prec = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}
	else
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_call_gold_probability_3);
		prec = GET_COMPREHENSIVE_VALUE_1(p_conf);
	}

	family_war_long_type long_type;
	if (random_util_t::selectByPercent(prec))
	{
		long_type = family_war_long_gold;
	}
	else
	{
		long_type = (family_war_long_type)random_util_t::randBetween(family_war_long_wind, family_war_long_fire);
	}

	if (m_family_war_long_info.m_object_id != 0)
	{
		monster_ptr p_monster = monster_manager_t::find_monster(m_family_war_long_info.m_object_id);
		if (NULL != p_monster)
		{
			p_monster->leave_scene();

			monster_manager_t::delete_monster(m_family_war_long_info.m_object_id);
		}

		m_family_war_long_info.m_object_id = 0;
	}

	family_war_summon_long(long_type);
}

void family_war_scene_t::on_update_long_state()
{
	switch (m_family_war_long_info.m_long_state)
	{
	case family_war_long_state_die:
		{
			on_long_refresh();
		}
		break;
	default:
		log_error("family war long state[%u] error", m_family_war_long_info.m_long_state);
		break;
	}
}

void family_war_scene_t::on_platform_hurt_timer(const boost::weak_ptr<family_war_scene_t>& p_wlong, family_war_platform_type type, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	family_war_scene_ptr p_family_war_scene(p_wlong.lock());
	if (NULL == p_family_war_scene)
	{
		log_error("p_family_war_scene null error");
		return;
	}

	p_family_war_scene->on_platform_hurt(type);
}

void family_war_scene_t::on_platform_hurt(family_war_platform_type type)
{
	if (type >= family_war_platform_max)
	{
		log_error("on_platform_hurt family_war_platform_type[%u] error", type);
		return;
	}

	if (m_state_type != proto::common::family_war_state_start)
	{
		return;
	}

	if (m_family_war_platform[type].m_occupied_side >= family_war_side_max)
	{
		log_error("on_platform_hurt hurt family_war_side_type[%u] error", m_family_war_platform[type].m_occupied_side);
		return;
	}

	if (add_family_score(m_family_war_platform[type].m_occupied_side, m_family_war_platform[type].m_hurt))
	{
		return;
	}

	if (m_platform_timer[type])
	{
		if (m_family_war_platform[type].m_interval != 0)
		{
			m_platform_timer[type]->expires_from_now(boost::posix_time::seconds(m_family_war_platform[type].m_interval));
			m_platform_timer[type]->async_wait(boost::bind(&family_war_scene_t::on_platform_hurt_timer, boost::weak_ptr<family_war_scene_t>(get_family_war_scene_ptr()), type, boost::asio::placeholders::error));
		}
	}
}

void family_war_scene_t::family_war_stronghold::add_war_member(uint64_t uid)
{
	m_family_roles.insert(uid);
	m_data.set_war_member(m_family_roles.size());
}

void family_war_scene_t::family_war_stronghold::del_war_member(uint64_t uid)
{
	m_family_roles.erase(uid);
	m_data.set_war_member(m_family_roles.size());
}

bool family_war_scene_t::family_war_stronghold::is_early_win() const
{
	return m_data.base().curr_hp() >= m_data.base().max_hp();
}

void family_war_scene_t::family_war_stronghold::add_score(uint32_t val)
{
	proto::client::family_war_fight_info* info = m_data.mutable_base();
	if (info) {
		info->set_curr_hp(info->curr_hp() + val);
	}
}

void family_war_scene_t::family_war_stronghold::set_platform_state(family_war_platform_type type, bool is_occupied)
{
	proto::client::family_war_fight_info* info = m_data.mutable_base();
	if (info)
	{
		switch (type)
		{
		case family_war_platform_sun:
			{
				info->set_is_occupied_sun(is_occupied);
			}
			break;
		case family_war_platform_moon:
			{
				info->set_is_occupied_moon(is_occupied);
			}
			break;
		default:
			log_error("family_war_platform_type[%u] error", type);
			break;
		}
	}
}

void family_war_scene_t::family_war_stronghold::add_occupied_long()
{
	m_occupied_cnt++;

	m_data.set_occupied_cnt(m_occupied_cnt);
}

void family_war_scene_t::family_war_stronghold::add_fight_win_cnt()
{
	m_fight_win_cnt++;

	m_data.set_fight_win_cnt(m_fight_win_cnt);
}

const std::string& family_war_scene_t::family_war_stronghold::get_family_name() const
{
	return m_data.base().family_name();
}

void family_war_scene_t::family_war_long_info::add_nearby_role(uint64_t uid, family_war_side_type type)
{
	if (type < family_war_side_max)
	{
		m_near_roles[type].insert(uid);
	}
}

void family_war_scene_t::family_war_long_info::del_nearby_role(uint64_t uid, family_war_side_type type)
{
	if (type < family_war_side_max)
	{
		m_near_roles[type].erase(uid);
	}
}

bool family_war_scene_t::family_war_long_info::long_wei(const role_ptr& p_role, family_war_side_type type)
{
    if (NULL == p_role)
    {
        log_error("NULL == p_role");
        return false;
    }

	if (type >= family_war_side_max)
	{
		log_error("family_war_side_type[%u] error", type);
		return false;
	}

	role_ptr p_fight_role = NULL;
	uint32_t dis = 0;
	for (std::set<uint64_t>::const_iterator citr = m_near_roles[type].begin();
		citr != m_near_roles[type].end(); ++citr)
	{
		const role_ptr& p_area_role = role_manager_t::find_role(*citr);
		if (p_area_role)
		{
			// 不在区域
			if (p_area_role->get_role_area_id() != m_area_id)
			{
				continue;
			}

			// 死亡
			if (p_area_role->is_die())
			{
				continue;
			}

			// 没有可用武将
			if (!p_area_role->have_hero_live())
			{
				continue;
			}

			if (!p_area_role->can_enter_fight(false))
			{
				continue;
			}

			if (NULL == p_fight_role)
			{
				p_fight_role = p_area_role;
				dis = std::abs(int(p_area_role->get_fighting() - p_role->get_fighting()));
			}
			else
			{
				uint32_t cur_dis = std::abs(int(p_area_role->get_fighting() - p_role->get_fighting()));
				if (cur_dis < dis)
				{
					p_fight_role = p_area_role;
					dis = cur_dis;
				}
			}
		}
	}

	if (p_fight_role != NULL)
	{
		proto::common::fight_param param;
		param.set_type(proto::common::fight_type_family_pk);
		if (fight_manager_t::fight_pvp(p_role, p_fight_role, &param) != 0)
		{
			// 遭遇战提示
			proto::client::gc_attack_platform_fight_notify ntf;
			p_role->send_msg_to_client(op_cmd::gc_attack_platform_fight_notify, ntf);
			p_fight_role->send_msg_to_client(op_cmd::gc_attack_platform_fight_notify, ntf);

			FAMILY_LOG("role[%lu] fight with area_role[%lu]", p_role->get_uid(), p_fight_role->get_uid());
			return true;
		}
		else
		{
			// 这个错误要注意，是什么原因没进入战斗
			log_error("----------------role[%lu] area_role[%lu] enter fight error-------------------", p_role->get_uid(), p_fight_role->get_uid());
			return false;
		}
	}

	return false;
}

void family_war_scene_t::family_war_long_info::broadcast_icon()
{
	// 刷新区域内攻击图标
	proto::server::ga_broadcast_notify msg_to_gate;

	for (uint32_t i = 0; i < family_war_side_max; ++i) {
		for (std::set<uint64_t>::const_iterator citr = m_near_roles[i].begin();
			citr != m_near_roles[i].end(); ++citr) {
			msg_to_gate.add_role_list(*citr);
		}
	}

	// 给gate转发
	if (msg_to_gate.role_list_size() > 0) {
		proto::client::gc_attack_icon_notify ntf;
		if (m_long_state == family_war_long_state_normal) {
			ntf.set_type(1);
		} else {
			ntf.set_type(0);
		}
		ntf.set_long_uid(common::string_util_t::uint64_to_string(m_object_id));

		msg_to_gate.set_cmd(op_cmd::gc_attack_icon_notify);

		std::string cmd_data;
		network::wrap_msg(cmd_data, ntf);
		msg_to_gate.set_data(cmd_data);

		env::server->send_msg_to_all_gate(op_cmd::ga_broadcast_notify, 0, msg_to_gate);
	}
}

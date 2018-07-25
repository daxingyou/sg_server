#include "dungeon_scene.hpp"

#include "tblh/DungeonTable.tbls.h"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"
#include "user_troop/user_troop_mgr.hpp"
#include "task/task_manager.hpp"
#include "script/script_dungeon.h"
#include "log/log_wrapper.hpp"
#include "tblh/errcode_enum.hpp"
#include "item/drop_manager.hpp"
#include "scene/scene_manager.hpp"
#include "achieve/achieve_common.hpp"
#include "monster/monster_manager.hpp"
#include "role/money_manager.hpp"
#include "dungeon_manager.hpp"

dungeon_scene_t::dungeon_scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param)
	: scene_t(scene_id, map_id, scene_type, type_param)
	, m_script_dungeon(new script_dungeon_t(this))
{

}

dungeon_scene_t::~dungeon_scene_t()
{

}

bool dungeon_scene_t::init()
{
	// 基类
	if (!scene_t::init()) {
		log_error("dungeon_scene_t::init() error");
		return false;
	}

	if (!m_state_controler.init(get_dungeon_scene_ptr(), proto::common::dungeon_state_none)) {
		log_error("dungeon scene[%lu] init state_controler error", m_scene_id);
		return false;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dungeon_wait_time);
	m_wait_time = GET_COMPREHENSIVE_VALUE_1(p_conf);

	return true;
}

void dungeon_scene_t::clear()
{
	if (NULL != m_script_dungeon) {
		m_script_dungeon->dispose();
		m_script_dungeon = NULL;
	}

	// 基类删除所有怪物
	scene_t::clear();
}

bool dungeon_scene_t::start()
{
	// 开启只是设置副本状态，召唤怪物放在后面
	set_scene_state(SCENE_STATE_NORMAL);

	uint32_t now_time = common::time_util_t::now_time();

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dungeon_max_time);
	uint32_t max_alive = GET_COMPREHENSIVE_VALUE_1(p_conf);
	m_last_time = now_time + max_alive;

	m_state_controler.change_state(proto::common::dungeon_state_wait);

	m_dungeon_result = proto::common::dungeon_result_none;

	return true;
}

void dungeon_scene_t::load_map_object()
{
	map_ptr p_map = get_map_ptr();
	if (NULL == p_map) {
		log_error("map[%u] null error", m_map_tid);
		return;
	}

	// 加载所有定时召唤怪物
	const dungeon_summon_delay_vec& delay_vec = p_map->get_map_summon_delay();
	if (!delay_vec.empty()) {
		m_delay_vec.assign(delay_vec.begin(), delay_vec.end());
	}

	const dungeon_summon_dynamic_vec& dynamic_vec = p_map->get_map_summon_dynamic();
	if (!dynamic_vec.empty()) {
		m_dynamic_vec.assign(dynamic_vec.begin(), dynamic_vec.end());
	}

	// 加载所有副本怪物
	const monster_define_vec& monster_vec = p_map->get_map_monsters();
	for (monster_define_vec::const_iterator itr = monster_vec.begin();
		itr != monster_vec.end(); ++itr) {
		const monster_common::monster_define_t& define = *itr;

		Monster* monster = GET_CONF(Monster, define.id);
		if (NULL == monster) {
			log_error("dungeon_scene_t::load_map_object not found monster [%u]", define.id);
			continue;
		}

		monster_ptr p_monster(new monster_t());
		if (p_monster != NULL && p_monster->init_monster(define, monster) && init_object(p_monster, define))
		{
			// 记录进度怪物
			if (define.type == monster_common::mob_monster_schedule) {
				add_schedule_monster(p_monster);
			}
		}
	}
}

bool dungeon_scene_t::init_dugeon(uint32_t dungeon_id)
{
	DungeonTable* conf = GET_CONF(DungeonTable, dungeon_id);
	if (NULL == conf) {
		log_error("dungeon_id[%u] not exist", dungeon_id);
		return false;
	}

	m_dungeon_id = dungeon_id;
	m_dungeon_type = (dungeon::dungeon_type_enum)conf->dungeon_type();
	m_pass_time_limit = conf->time_limit();
	m_revive_limit = conf->revieve_limit();
	m_pass_type = (dungeon::pass_type_enum)conf->pass_type();

	m_schedule_value = conf->pass_schedule();

	return true;
}

void dungeon_scene_t::set_create_date(const proto::server::dungeon_create_data& data)
{
	m_dugenon_create.m_create_type = data.type();
	m_dugenon_create.m_id = data.dungeon_create_id();
}

bool dungeon_scene_t::is_can_restart()
{
	if (m_restart_count > MAX_DUNGEON_FIGHT_TIME) {
		log_error("dungeon[%u] restart_count > %u", m_dungeon_id, m_restart_count);
		return false;
	}

	if (m_state_controler.get_state_type() != proto::common::dungeon_state_stop) {
		log_error("dungeon[%u] state[%u] error", m_dungeon_id, m_state_controler.get_state_type());
		return false;
	}

	// 检查玩家是否有在战斗的
	if (is_have_fight()) {
		// 有人在战斗就不能重新开启
		log_error("somebody has fight!");
		return false;
	}

	return true;
}

void dungeon_scene_t::restart_dungeon()
{
	// 清理所有怪物
	clear_all_monster();

	// 初始化一些数据
	set_scene_state(SCENE_STATE_NORMAL);

	uint32_t now_time = common::time_util_t::now_time();

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dungeon_max_time);
	uint32_t max_alive = GET_COMPREHENSIVE_VALUE_1(p_conf);
	m_last_time = now_time + max_alive;

	m_start_time = common::time_util_t::now_time();
	if (m_pass_time_limit != 0)
		m_end_time = m_start_time + m_pass_time_limit;

	m_idle_time = 0;

	m_curr_revive_time = 0;

	m_eventList.clear();

	m_death_count.clear();
	m_schedule_monster_vec.clear();

	// 召唤怪物
	load_map_object();

	m_dungeon_result = proto::common::dungeon_result_none;

	m_state_controler.change_state(proto::common::dungeon_state_start);

	m_restart_count++;

	// 广播副本状态
	notify_dungeon_schedule();

	// 拉人
	map_ptr p_jump_map = get_map_ptr();
	if (NULL == p_jump_map)
	{
		log_error("scene[%lu] can not find map[%u] when restart", m_scene_id, get_map_tid());
		return;
	}

	DungeonTable* p_dungeon_table = GET_CONF(DungeonTable, m_dungeon_id);
	if (NULL == p_dungeon_table)
	{
		log_error("dungeon_table id[%u] not find", m_dungeon_id);
		return;
	}

	std::set<uint64_t> old_roles = m_roles;
	for (const auto& uid : old_roles)
	{
		role_ptr p_role = role_manager_t::find_role(uid);
		if (p_role != NULL)
		{
			// 扣除精力
			money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_VIGOUR, p_dungeon_table->cost_vigour(), log_enum::source_type_vigour_dungeon_reduce, 0);
			// 单独通知精力变化
			proto::client::gc_vigour_info_notify ntf;
			proto::common::role_change_data* p_rcd = ntf.mutable_rcd();
			if (p_rcd) {
				p_role->get_personal_info(p_rcd->mutable_per_info());
			}
			p_role->send_msg_to_client(op_cmd::gc_vigour_info_notify, ntf);

			scene_manager_t::role_jump_point(p_role, p_jump_map->get_born_point(p_role->get_level()));
		}
	}
}

void dungeon_scene_t::update(uint32_t curr_time)
{
	// 副本秒定时器
	if (m_state_controler.get_state_type() != proto::common::dungeon_state_none) {
		m_state_controler.action(curr_time);
	}
}

void dungeon_scene_t::on_monster_fight_result(monster_ptr p_monster, bool is_win, uint32_t fight_time, uint32_t death_value)
{
	if (p_monster == NULL) {
		log_error("p_monster null error");
		return;
	}

	// 已经停止
	if (m_state_controler.get_state_type() != proto::common::dungeon_state_start) {
		return;
	}

	// 已经结算过
	if (m_dungeon_result != proto::common::dungeon_result_none) {
		return;
	}

	// 是否开始结算副本
	do {
		// 怪物赢了，让场景内所有玩家死亡
		if (is_win) {
			// 副本时间到了
			if (reach_end_time(common::time_util_t::now_time())) {
				m_dungeon_result = proto::common::dungeon_result_failed_time_long;
				break;
			}

			if (m_revive_limit != -1) {
				if (m_curr_revive_time >= (uint32_t)m_revive_limit) {
					m_dungeon_result = proto::common::dungeon_result_failed_fight;
					break;
				}

				std::set<uint64_t> role_list = m_roles;
				for (const auto& uid : role_list) {
					const role_ptr& p_role = role_manager_t::find_role(uid);
					if (p_role != NULL) {
						on_other_kill_me(p_role, p_monster);
					}
				}
			}
		} else {
			m_death_count[p_monster->get_monster_id()] += 1;

			schedule_monster_ptr p_schedule_monster = get_schedule_monster(p_monster->get_monster_id());
			if (p_schedule_monster != NULL) {
				p_schedule_monster->add_schedule_count(p_monster->get_object_id());

				p_schedule_monster->m_all_round_time += fight_time;
				p_schedule_monster->m_all_death_value += death_value;

				if (NULL != m_script_dungeon) {
					p_schedule_monster->m_all_death_score += m_script_dungeon->calc_death_score(m_dungeon_id, death_value);
				}

				notify_dungeon_schedule();
			}

			if (NULL != m_script_dungeon) {
				// 先触发脚本事件
				m_script_dungeon->on_monster_die(m_dungeon_id, p_monster->get_monster_id());
			}

			// 还没有通关，通过检测
			if (m_dungeon_result == proto::common::dungeon_result_none) check_key_monster();
		}
	} while (0);

	// 开始结算
	if (m_dungeon_result != proto::common::dungeon_result_none) start_settlement();
}

void dungeon_scene_t::on_role_enter(role_ptr p_role)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return;
	}

	// pk模式修改 目前默认安全区
	p_role->change_pk_mode(proto::common::PK_PEACE);

	log_wrapper_t::send_dungeon_log(p_role->get_object_id(), m_dungeon_id, m_dugenon_create.m_create_type, m_dugenon_create.m_id, log_enum::source_type_dungeon_enter, m_state_controler.get_state_type());

	// 已经停止
	if (m_state_controler.get_state_type() == proto::common::dungeon_state_none) {
		log_error("dungeon[%u] is already state[dungeon_state_none]", m_dungeon_id);
		notify_dungeon_schedule(p_role);
		return;
	}

	if (m_state_controler.get_state_type() == proto::common::dungeon_state_start && is_idle_wait()) {
		clear_idle_time();
	}

	switch (m_dugenon_create.m_create_type) {
		case proto::server::dungeon_create_single: {
			if (m_state_controler.get_state_type() == proto::common::dungeon_state_wait) {
				start_fight();
			} else {
				notify_dungeon_schedule(p_role);
			}

			break;
		}
		case proto::server::dungeon_create_team: {
			troop_ptr p_troop = game_troop_mgr_t::get_troop(m_dugenon_create.m_id);
			if (p_troop == NULL) {
				log_error("dungeon[%u] creater is team[%lu], is already not exist", m_dungeon_id, m_dugenon_create.m_id);
				notify_dungeon_schedule(p_role);
				return;
			}

			// 队长进入副本
			if (p_role->get_object_id() == p_troop->get_leader_id() && m_state_controler.get_state_type() == proto::common::dungeon_state_wait) {
				start_fight();
			} else {
				notify_dungeon_schedule(p_role);
			}

			break;
		}
		default:
			break;
	}
}

void dungeon_scene_t::on_role_leave(role_ptr p_role)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return;
	}

	if (m_state_controler.get_state_type() == proto::common::dungeon_state_start && get_role_size() == 0) {
		// 进入空闲等待
		start_idle_time(common::time_util_t::now_time());
	}

	log_wrapper_t::send_dungeon_log(p_role->get_object_id(), m_dungeon_id, m_dugenon_create.m_create_type, m_dugenon_create.m_id, log_enum::source_type_dungeon_leave, m_state_controler.get_state_type());
}

void dungeon_scene_t::notify_dungeon_schedule(role_ptr p_role /*= NULL*/)
{
	proto::client::gc_dungeon_schedule_data_notify notify;

	notify.set_dungeon_id(m_dungeon_id);
	notify.set_state_type(m_state_controler.get_state_type());

	for (std::vector<schedule_monster_ptr>::const_iterator citr = m_schedule_monster_vec.begin();
		citr != m_schedule_monster_vec.end(); ++citr) {
		const schedule_monster_ptr& p_schedule_monster = *citr;
		if (p_schedule_monster != NULL) {
			p_schedule_monster->peek_data(notify.add_datas());
		}
	}

	if (m_end_time != 0) notify.set_left_time(m_end_time);

	if (p_role != NULL) {
		notify.set_pass_time(p_role->get_dungeon_data().get_dungeon_pass_time(m_dungeon_id));
		p_role->send_msg_to_client(op_cmd::gc_dungeon_schedule_data_notify, notify);
	} else {
		for (std::set<uint64_t>::const_iterator citr = m_roles.begin();
			citr != m_roles.end(); ++citr) {
			role_ptr p_member = role_manager_t::find_role(*citr);
			if (p_member != NULL) {
				notify.set_pass_time(p_member->get_dungeon_data().get_dungeon_pass_time(m_dungeon_id));
				p_member->send_msg_to_client(op_cmd::gc_dungeon_schedule_data_notify, notify);
			}
		}
	}
}

void dungeon_scene_t::on_other_kill_me(role_ptr p_role, object_base_ptr p_enemy)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return;
	}

	if (p_enemy == NULL) {
		log_error("p_enemy null error");
		return;
	}

	p_role->set_object_die(p_enemy->get_object_id());

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dungeon_revive_time);
	uint32_t revive_time = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_role->set_revive_time(revive_time);

	show_revive_ui(p_role);
}

void dungeon_scene_t::show_revive_ui(role_ptr p_role)
{
	if (NULL == p_role) {
		log_error("role null error");
		return;
	}

	uint32_t time_now = common::time_util_t::now_time();

	proto::client::gc_object_death_notify notify;
	notify.set_revive_type(proto::common::object_revive_dungeon);

	if (p_role->get_revive_time() > time_now) {
		notify.set_revive_time(p_role->get_revive_time() - time_now);
	} else {
		notify.set_revive_time(0);
	}

	if (m_revive_limit != -1) {
		notify.set_curr_revive_times(m_curr_revive_time);
		notify.set_max_revive_times(m_revive_limit);
	}

	p_role->send_msg_to_client(op_cmd::gc_object_death_notify, notify);
}

void dungeon_scene_t::on_login_die(role_ptr p_role)
{
	if (NULL == p_role) {
		log_error("role null error");
		return;
	}

	// 没死亡
	if (!p_role->is_die()) {
		log_error("role[%lu] not die", p_role->get_uid());
		return;
	}

	// 弹出复活界面
	show_revive_ui(p_role);
}

void dungeon_scene_t::on_revive_role(role_ptr p_role)
{
	// 副本场景复活不用拉人
}

void dungeon_scene_t::revive_all_role()
{
	// 复活的时候把场景的都复活
	std::set<uint64_t> role_list = m_roles;
	for (const auto& uid : role_list) {
		role_ptr p_other_role = role_manager_t::find_role(uid);
		if (p_other_role != NULL && p_other_role->is_die()) {
			p_other_role->set_object_revive(0);

			proto::client::gc_object_revive_reply reply;
			reply.set_reply_code(common::errcode_enum::error_ok);
			p_other_role->send_msg_to_client(op_cmd::gc_object_revive_reply, reply);
		}
	}

	m_curr_revive_time += 1;
}

void dungeon_scene_t::start_fight()
{
	// 为0表示没有时间限制
	m_start_time = common::time_util_t::now_time();
	if (m_pass_time_limit != 0)
		m_end_time = m_start_time + m_pass_time_limit;

	clear_idle_time();

	m_state_controler.change_state(proto::common::dungeon_state_start);

	// 召唤怪物
	load_map_object();

	// 广播副本状态
	notify_dungeon_schedule();
}

void dungeon_scene_t::start_settlement()
{
	m_state_controler.change_state(proto::common::dungeon_state_stop);

	proto::client::gc_notify_dungeon_result_notify notify;
	notify.set_dungeon_id(m_dungeon_id);
	notify.set_type(m_dungeon_result);

	// 副本公用数据组织
	// 只有通关才结算
	if (m_dungeon_result == proto::common::dungeon_result_success) {
		// 计算得分
		uint32_t all_round_time = 0;
		uint32_t all_death_score = 0;
		uint32_t all_death_value = 0;
		uint32_t curr_value = 0;

		for (std::vector<schedule_monster_ptr>::const_iterator citr = m_schedule_monster_vec.begin();
			citr != m_schedule_monster_vec.end(); ++citr) {
			const schedule_monster_ptr& p_schedule_monster = *citr;
			if (p_schedule_monster != NULL && p_schedule_monster->m_monster_type != proto::common::EM_MONSTER_NPC) {
				all_round_time += p_schedule_monster->m_all_round_time;
				all_death_score += p_schedule_monster->m_all_death_score;
				all_death_value += p_schedule_monster->m_all_death_value;
				curr_value += p_schedule_monster->m_data.curr_num();
			}
		}

		// 对于那些击杀数为0的情况
		if (curr_value == 0) curr_value = 1;

		uint32_t all_round_score = 0;
		if (NULL != m_script_dungeon) {
			all_round_score = m_script_dungeon->calc_time_score(m_dungeon_id, all_round_time);
		}

		uint32_t average_death_score = ceil((float)all_death_score / curr_value / 5) * 5;
		uint32_t average_death_value = ceil((float)all_death_value / curr_value);

		notify.set_all_score(all_round_score + average_death_score + 10);
		notify.set_all_round_time(all_round_time);
		notify.set_round_score(all_round_score);
		notify.set_average_death(average_death_value);
		notify.set_death_score(average_death_score);
	}

	if (m_dungeon_result == proto::common::dungeon_result_success) {	// 对于通关的处理，每个人都不一样
		// 分数评价
		uint32_t score_level = 0;
		if (NULL != m_script_dungeon) {
			score_level = m_script_dungeon->calc_score_level(m_dungeon_id, notify.all_score());
		}

		// 通关奖励
		DungeonTable* p_dungeon_table = GET_CONF(DungeonTable, m_dungeon_id);
		if (NULL == p_dungeon_table) {
			log_error("dungeon[%u] config null error", m_dungeon_id);
			return;
		}

		uint32_t pass_reward = p_dungeon_table->get_pass_reward(score_level);

		// 副本中所有玩家通关副本
		std::set<uint64_t> role_list = m_roles;
		for (const auto& uid : role_list) {
			const role_ptr& p_role = role_manager_t::find_role(uid);
			if (p_role == NULL) {
				log_error("role[%lu] null not find", uid);
				continue;
			}

			bool is_reward_limit = false;
			uint32_t reply_code = common::errcode_enum::error_ok;
			do {
				if (!dungeon_manager_t::check_fight_times(p_role, m_dungeon_id))
				{
					DUNGEON_LOG("role[%lu] dungeon[%u] fight_times error", p_role->get_uid(), m_dungeon_id);
					reply_code = common::errcode_enum::dungeon_reward_limit_fight_times;
					is_reward_limit = true;
					break;
				}

				uint32_t vigour = p_role->get_vigour();
				if (vigour < p_dungeon_table->cost_vigour()) {
					DUNGEON_LOG("role[%lu] dungeon[%u] vigour[%u] < %u", p_role->get_uid(), m_dungeon_id, vigour, p_dungeon_table->cost_vigour());
					reply_code = common::errcode_enum::dungeon_reward_limit_vigour;
					is_reward_limit = true;
					break;
				}
			} while (0);

			// 先清理下
			notify.clear_rcd();

			// 首次通关处理
			if (!is_reward_limit) {

				// 增加挑战次数
				p_role->get_dungeon_data().add_fight_times(p_dungeon_table->base_id());
				// 扣除精力
				money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_VIGOUR, p_dungeon_table->cost_vigour(), log_enum::source_type_vigour_dungeon_reduce, 0);

				if (0 != pass_reward) {
					if (!drop_manager_t::check_bag_num(p_role, pass_reward)) {
						reply_code = errcode_enum::task_bag_full_mail;
					}

					if (!drop_manager_t::drop(p_role, pass_reward, log_enum::source_type_dungeon_pass,
						proto::common::drop_sys_type_dugeon, m_dungeon_id, notify.mutable_rcd())) {
						log_error("role[%lu] give dungeon[%u] pass reward[%u] error", p_role->get_uid(), m_dungeon_id, pass_reward);
					}
				}

				if (p_role->get_dungeon_data().get_dungeon_pass_time(m_dungeon_id) == 0) {
					// 先增加通关次数
					p_role->get_dungeon_data().add_dungeon_pass(m_dungeon_id);

					p_role->get_dungeon_data().set_best_pass_time(m_dungeon_id, notify.all_round_time());
					p_role->get_dungeon_data().set_best_death_value(m_dungeon_id, notify.average_death());

					// 当前就是最佳纪录
					notify.set_history_round_time(notify.all_round_time());
					notify.set_history_death(notify.average_death());

					if (0 != p_dungeon_table->first_pass_reward()) {
						if (!drop_manager_t::check_bag_num(p_role, p_dungeon_table->first_pass_reward())) {
							reply_code = errcode_enum::task_bag_full_mail;
						}

						if (!drop_manager_t::drop(p_role, p_dungeon_table->first_pass_reward(), log_enum::source_type_dungeon_first_pass,
							proto::common::drop_sys_type_dugeon, m_dungeon_id, notify.mutable_rcd())) {
							log_error("role[%lu] give dungeon[%u] first pass reward[%u] error", p_role->get_uid(), m_dungeon_id, p_dungeon_table->first_pass_reward());
						}
					}
				} else {
					// 增加通关次数
					p_role->get_dungeon_data().add_dungeon_pass(m_dungeon_id);

					uint32_t best_pass_time = p_role->get_dungeon_data().get_best_pass_time(m_dungeon_id);
					if (best_pass_time == 0 || notify.all_round_time() < best_pass_time) {
						p_role->get_dungeon_data().set_best_pass_time(m_dungeon_id, notify.all_round_time());

						notify.set_history_round_time(notify.all_round_time());
					} else {
						notify.set_history_round_time(best_pass_time);
					}

					uint32_t best_death_value = p_role->get_dungeon_data().get_best_death_value(m_dungeon_id);
					if (best_death_value == 0 || notify.average_death() < best_death_value) {
						p_role->get_dungeon_data().set_best_death_value(m_dungeon_id, notify.average_death());

						notify.set_history_death(notify.average_death());
					} else {
						notify.set_history_death(best_death_value);
					}
				}

				task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
				if (NULL != p_task_mgr) {
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_DUNGEON, *notify.mutable_rcd(), m_dungeon_id, 1);
					p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_DUNGEON, *notify.mutable_rcd(), 0, 1);
				}

				proto::common::role_change_data* rcd = notify.mutable_rcd();
				if (rcd) p_role->get_personal_info(rcd->mutable_per_info());
			}

			notify.set_reply_code(reply_code);
			p_role->send_msg_to_client(op_cmd::gc_notify_dungeon_result_notify, notify);

			achieve_common_t::notify_progress_state(uid, proto::common::ACHIEVE_GRADE_PASS_DUNGEON_NUMBER,score_level);
			achieve_common_t::notify_progress_state(uid, proto::common::ACHIEVE_DUNGEON_TOTAL_PASS,0);
			achieve_common_t::notify_progress_state(uid, proto::common::ACHIEVE_BASE_PASS_DUNGEON_NUMBER, p_dungeon_table->base_id());
		
			if (m_dugenon_create.m_create_type == proto::server::dungeon_create_team) 
			{
				proto::server::ge_troop_friend_notify ntf;
				ntf.set_troop_id(m_dugenon_create.m_id);
				env::server->send_msg_to_center(op_cmd::ge_troop_friend_notify, 0, ntf);
			}

		}
	} else { // 失败的话都是一样的处理
		send_msg_to_scene(op_cmd::gc_notify_dungeon_result_notify, notify);
	}

	DUNGEON_LOG("dungeon[%u] end, reason:[%u]", m_dungeon_id, m_dungeon_result);
}

void dungeon_scene_t::check_key_monster()
{
	bool is_all_complete = true;

	switch (m_pass_type) {
		case dungeon::pass_type_kill_any: {
			uint32_t all_curr_value = 0;
			for (std::vector<schedule_monster_ptr>::const_iterator citr = m_schedule_monster_vec.begin();
				citr != m_schedule_monster_vec.end(); ++citr) {
				const schedule_monster_ptr& p_schedule_monster = *citr;
				if (p_schedule_monster != NULL && p_schedule_monster->m_monster_type != proto::common::EM_MONSTER_NPC) {
					all_curr_value += p_schedule_monster->m_data.curr_num();
				}
			}

			is_all_complete = all_curr_value >= m_schedule_value;
			break;
		}
		case dungeon::pass_type_kill_all: {
			for (std::vector<schedule_monster_ptr>::const_iterator citr = m_schedule_monster_vec.begin();
				citr != m_schedule_monster_vec.end(); ++citr) {
				const schedule_monster_ptr& p_schedule_monster = *citr;
				if (p_schedule_monster != NULL && p_schedule_monster->m_data.curr_num() < p_schedule_monster->m_data.max_num()) {
					is_all_complete = false;
					break;
				}
			}
			break;
		}
		default:
			break;
	}

	// 完成的情况，直接进入结算
	if (is_all_complete) {
		m_dungeon_result = proto::common::dungeon_result_success;
		return;
	}

	// 时间到没完成的情况
	if (reach_end_time(common::time_util_t::now_time())){
		m_dungeon_result = proto::common::dungeon_result_failed_time_long;
		return;
	}
}

void dungeon_scene_t::failed_time_long()
{
	if (m_dungeon_result == proto::common::dungeon_result_none)
	{
		m_dungeon_result = proto::common::dungeon_result_failed_time_long;
		start_settlement();
	}
}

void dungeon_scene_t::summon_dynamic_monster(uint32_t tid, uint32_t num)
{
	for (uint32_t i = 0; i < num; ++i) {
		dungeon_summon_dynamic_vec::iterator itr = std::find_if(m_dynamic_vec.begin(), m_dynamic_vec.end(),
			[tid](const monster_common::dungeon_summon_dynamic& def)
		{
			if (def.id == tid) return true;
			return false;
		});

		if (itr == m_dynamic_vec.end()) {
			log_error("dungeon[%u] summon[%u] not find", m_map_tid, tid);
			return;
		}

		const monster_common::dungeon_summon_dynamic& summon_dynamic_define = *itr;
		Monster* p_conf = GET_CONF(Monster, summon_dynamic_define.id);
		if (NULL == p_conf) {
			log_error("summon_dynamic_monster not found monster config [%u]", summon_dynamic_define.id);
			return;
		}

		monster_ptr p_monster(new monster_t());
		if (p_monster != NULL && p_monster->init_monster(summon_dynamic_define, p_conf) && init_object(p_monster, summon_dynamic_define)) {
			// 记录进度怪物
			if (summon_dynamic_define.type == monster_common::mob_monster_schedule) {
				add_schedule_monster(p_monster);
			}
		}

		m_dynamic_vec.erase(itr);
	}
}

void dungeon_scene_t::check_daley_summon(uint32_t curr_time)
{
	if (m_delay_vec.empty()) return;

	uint32_t diffSec = curr_time - m_start_time;
	uint32_t count = 0;
	for (dungeon_summon_delay_vec::iterator itr = m_delay_vec.begin();
		itr != m_delay_vec.end(); /*++itr*/) {
		const monster_common::dungeon_summon_delay& define = (*itr);
		if (diffSec >= define.delay) {
			Monster* p_conf = GET_CONF(Monster, define.id);
			if (NULL == p_conf) {
				log_error("check_daley_summon not found monster config [%u]", define.id);
				return;
			}

			monster_ptr p_monster(new monster_t());
			if (p_monster != NULL && p_monster->init_monster(define, p_conf) && init_object(p_monster, define)) {
				// 记录进度怪物
				if (define.type == monster_common::mob_monster_schedule) {
					add_schedule_monster(p_monster);
					count++;
				}
			}

			itr = m_delay_vec.erase(itr);
		} else {
			++itr;
		}
	}

	// 攒起来一次通知
	if (count > 0) notify_dungeon_schedule();
}

uint32_t dungeon_scene_t::get_death_count(uint32_t tid)
{
	std::map<uint32_t, uint32_t>::const_iterator citr = m_death_count.find(tid);
	if (citr != m_death_count.end()) {
		return citr->second;
	}

	return 0;
}

void dungeon_scene_t::forced_all_leave()
{
	switch (m_dugenon_create.m_create_type) {
		case proto::server::dungeon_create_single: { // 单人副本
			role_ptr p_role = role_manager_t::find_role(m_dugenon_create.m_id);
			if (p_role != NULL) {
				scene_manager_t::role_back_scene(p_role, proto::server::forced_change_scene);
			}

			break;
		}
		case proto::server::dungeon_create_team: { // 组队把队长拉走
			troop_ptr p_troop = game_troop_mgr_t::get_troop(m_dugenon_create.m_id);
			if (NULL != p_troop) {
				role_ptr p_role = role_manager_t::find_role(p_troop->get_leader_id());
				if (p_role != NULL) {
					scene_manager_t::role_back_scene(p_role, proto::server::forced_change_scene);
				}
			}

			break;
		}
		default: {
			log_error("scene[%lu] dungeon[%u] dungeon_create_type[%u] error", m_scene_id, m_dungeon_id, m_dugenon_create.m_create_type);
		}
	}
}

dungeon_scene_t::schedule_monster_ptr dungeon_scene_t::get_schedule_monster(uint32_t tid)
{
	std::vector<schedule_monster_ptr>::iterator itr = std::find_if(m_schedule_monster_vec.begin(), m_schedule_monster_vec.end(),
                            [tid](const schedule_monster_ptr& p_schedule_monster)
                            {
                                if (NULL != p_schedule_monster && p_schedule_monster->m_monster_tid == tid) return true;
								return false;
                            });

	if (itr != m_schedule_monster_vec.end()) {
		return *itr;
	}

	return NULL;
}

void dungeon_scene_t::add_schedule_monster(const monster_ptr& p_monster)
{
	if (NULL == p_monster) {
		log_error("p_monster null error");
		return;
	}

	schedule_monster_ptr p_schedule_monster = get_schedule_monster(p_monster->get_monster_id());
	if (p_schedule_monster == NULL) {
		p_schedule_monster.reset(new schedule_monster_t(p_monster->get_monster_id(), p_monster->get_monster_type()));
		m_schedule_monster_vec.push_back(p_schedule_monster);
	}

	if (p_schedule_monster == NULL) {
		log_error("p_schedule_monster null error");
		return;
	}

	p_schedule_monster->add_schedule_monster(p_monster);
}

bool dungeon_scene_t::is_event_done(uint32_t id) const
{
	return m_eventList.find(id) != m_eventList.end();
}

void dungeon_scene_t::add_event_done(uint32_t id)
{
	if (!is_event_done(id)) {
		m_eventList.insert(id);
		DUNGEON_LOG("dungeon[%u:%u] add event[%u]", m_dungeon_id, m_map_tid, id);
	}
}

void dungeon_scene_t::ask_do_event(uint32_t id)
{
	if (is_event_done(id)) {
		log_error("dungeon[%u:%u] event[%u] is done", m_dungeon_id, m_map_tid, id);
		return;
	}

	if (NULL != m_script_dungeon) {
		// 先触发脚本事件
		m_script_dungeon->do_user_event(m_dungeon_id, id);
	}
}

void dungeon_scene_t::add_schedule_count(uint32_t monster_tid, bool is_del)
{
	schedule_monster_ptr p_schedule_monster = get_schedule_monster(monster_tid);
	if (p_schedule_monster != NULL) {
		p_schedule_monster->add_schedule_count(is_del);

		notify_dungeon_schedule();
	}
}

dungeon_scene_t::schedule_monster_t::schedule_monster_t(uint32_t tid, proto::common::EM_MONSTER_TYPE type)
	: m_monster_tid(tid)
	, m_monster_type(type)
{
	m_data.set_monster_tid(tid);
	m_data.set_curr_num(0);
	m_data.set_max_num(0);
}

void dungeon_scene_t::schedule_monster_t::add_schedule_monster(const monster_ptr& p_monster)
{
	if (NULL == p_monster) {
		log_error("p_monster null error");
		return;
	}

	m_data.set_max_num(m_data.max_num() + 1);

	if (m_monster_list.empty()) {
		m_data.set_pos_x(p_monster->get_current_x());
		m_data.set_pos_y(p_monster->get_current_y());
	}

	m_monster_list.push_back(p_monster->get_object_id());
}

void dungeon_scene_t::schedule_monster_t::add_schedule_count(uint64_t object_id)
{
	m_data.set_curr_num(m_data.curr_num() + 1);

	std::list<uint64_t>::iterator itr = std::find(m_monster_list.begin(), m_monster_list.end(), object_id);
	if (itr != m_monster_list.end()) {
		m_monster_list.erase(itr);
	}

	if (!m_monster_list.empty()) {
		monster_ptr p_monster = monster_manager_t::find_monster(m_monster_list.front());
		if (p_monster != NULL) {
			m_data.set_pos_x(p_monster->get_current_x());
			m_data.set_pos_y(p_monster->get_current_y());
		}
	}
}

void dungeon_scene_t::schedule_monster_t::add_schedule_count(bool is_del /*= false*/)
{
	m_data.set_curr_num(m_data.curr_num() + 1);

	if (!m_monster_list.empty()) {
		if (is_del) {
			monster_ptr p_monster = monster_manager_t::find_monster(m_monster_list.front());
			if (p_monster != NULL) {
				p_monster->leave_scene();
				monster_manager_t::delete_monster(p_monster->get_object_id());
			}
		}

		m_monster_list.pop_front();
	}

	if (!m_monster_list.empty()) {
		monster_ptr p_monster = monster_manager_t::find_monster(m_monster_list.front());
		if (p_monster != NULL) {
			m_data.set_pos_x(p_monster->get_current_x());
			m_data.set_pos_y(p_monster->get_current_y());
		}
	}
}

void dungeon_scene_t::schedule_monster_t::peek_data(proto::common::dungeon_schedule_data* p_data)
{
	if (p_data) {
		p_data->CopyFrom(m_data);
	}
}

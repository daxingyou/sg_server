#include "user_troop.hpp"
#include "formation/formation.hpp"
#include "common.pb.h"
#include "user_troop_mgr.hpp"
#include "hero/hero_manager.hpp"
#include "log_enum.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Treasure.tbls.h"
#include "tblh/TreasureMake.tbls.h"
#include "treasure/treasure_manager.hpp"
#include "task/task_manager.hpp"
#include "mail/mail_manager.hpp"
#include "item/item_manager.hpp"
#include "scene/scene.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/comprehensive_common.hpp"
#include "config/config_manager.hpp"
#include "dungeon/dungeon_manager.hpp"
#include "scene/scene_manager.hpp"
#include "fight/fight_manager.hpp"
#include "tblh/TutorTable.tbls.h"
#include "partner/partner_manager.hpp"
#include "partner/partner_tutor.hpp"
#include "partner/partner_shadow.hpp"
#include "friend/friendliness.h"

const int32_t WALK_ADJUST[9][2] = { { 0, -2 },{ 2, -2 },{ 2, 0 },{ 2, 2 },{ 0, 2 },{ -2, 2 },{ -2, 0 },{ -2, -2 },{ 0, 0 } };

void troop_t::troop_member::load_data(proto::common::user_troop_member* p_data)
{
	if (p_data) m_base.Swap(p_data);
	m_uid = common::string_util_t::string_to_uint64(m_base.id());
}


void troop_t::troop_member::load_extra(proto::server::troop_extra_info* p_data)
{
	if (p_data) m_extra_info.Swap(p_data);
}

troop_t::troop_t(uint64_t uid)
	: m_uid(uid)
	, m_formation_mgr(uid)
{
	m_formation_mgr.init_formation();

	m_moves.resize(TROOP_MEMBER_MAX_COUNT);
}


void troop_t::get_normal_members(troop_member_vec& members)
{
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			members.push_back(m_troop_members[i]);
		}
	}
}


bool troop_t::get_role_size() const
{
	uint32_t num = 0;
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
			num++;
		}
	}

	return num;
}

troop_t::troop_member_ptr troop_t::get_member(uint64_t member_uid)
{
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_uid() == member_uid) {
			return m_troop_members[i];
		}
	}

	return NULL;
}


troop_t::troop_member_index troop_t::get_member_index(uint64_t member_uid)
{
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_uid() == member_uid) {
			return troop_member_index(m_troop_members[i], i);
		}
	}

	return troop_member_index(NULL, -1);
}

void troop_t::update_troop_modify(proto::common::troop_data* td)
{
	if (!td) {
		log_error("troop_data null error");
		return;
	}

	m_is_free_join = td->is_free_join();
	m_type = td->type();
	m_level_min = td->level_min();
	m_level_max = td->level_max();
}

void troop_t::process_event(uint32_t event_id, uint64_t member_id, proto::server::eg_troop_event_notify& ntf)
{
	switch (event_id) {
		case proto::common::troop_event_create:
			do_create_troop(member_id, ntf.mutable_troop());
			break;
		case proto::common::troop_event_join:
			do_member_join(member_id, ntf.mutable_troop());
			break;
		case proto::common::troop_event_quit:
			do_member_quit(member_id, true);
			break;
		case proto::common::troop_event_shadow_quit:
			do_member_quit(member_id, false);
			break;
		case proto::common::troop_event_kick:
			do_member_kick(member_id);
			break;
		case proto::common::troop_event_leave:
			do_member_leave(member_id);
			break;
		case proto::common::troop_event_leader:
			do_change_leader(member_id, ntf.mutable_troop());
			break;
		case proto::common::troop_event_modify:
			do_troop_modify(member_id, ntf.mutable_troop());
			break;
		case proto::common::troop_event_back:
			do_member_back(member_id, ntf.mutable_troop());
			break;
		case proto::common::troop_event_update:
			do_member_update(member_id, ntf.mutable_troop());
			break;
		case proto::common::troop_event_online:
			do_member_online(member_id, ntf.mutable_troop());
			break;
		case proto::common::troop_event_dissolve:
			do_troop_dissolve(member_id);
			break;
		case proto::common::troop_event_transmission:
			do_member_transmission(member_id);
			break;
		case proto::common::troop_event_zhuagui_task:
			do_member_zhuagui_task(member_id, ntf.mutable_troop());
			break;
		default: {
			log_error("troop event[%u] not define", event_id);
			break;
		}
	}
}

void troop_t::notify_event(uint32_t event_id, const std::string& member_name)
{
	proto::client::gc_troop_event_notify ntf;
	ntf.set_event(event_id);
	ntf.set_member_name(member_name);
	if (event_id != proto::common::troop_event_dissolve) {
		peek_data(ntf.mutable_troop());
	}
	send_msg_to_all(op_cmd::gc_troop_event_notify, ntf);
}

void troop_t::peek_data(proto::common::troop_data* p_data)
{
	if (NULL == p_data) return;

	p_data->set_id(common::string_util_t::uint64_to_string(m_uid));
	p_data->set_type(m_type);
	p_data->set_is_free_join(m_is_free_join);
	p_data->set_leader(common::string_util_t::uint64_to_string(m_leader_uid));
	p_data->set_level_min(m_level_min);
	p_data->set_level_max(m_level_max);

	uint32_t fighting = 0;
	for (const auto& p_member : m_troop_members) {
		if (NULL != p_member) {
			p_member->peek_data(p_data->add_members());
			fighting += p_member->get_fighting();
		}
	}

	p_data->set_fighting(fighting);
}

// 更新一些基础的
void troop_t::cross_server_init(const role_ptr& p_role)
{
	if (p_role == NULL) {
		log_error("p_role == NULL");
		return;
	}

	troop_member_ptr p_member = get_member(p_role->get_uid());
	if (p_member == NULL) {
		log_error("role[%lu] not troop[%lu] member", p_role->get_uid(), m_uid);
		return;
	}

	p_member->set_is_trade(p_role->get_mode_state() == proto::common::role_mode_state_trade);
	p_role->update_troop_info(m_uid, p_member->get_state(), m_leader_uid == p_role->get_uid());
}

void troop_t::cross_server_update(const role_ptr& p_role, proto::server::scene_change_type type)
{
	if (p_role == NULL) {
		log_error("p_role == NULL");
		return;
	}

	troop_member_index p_member_index = get_member_index(p_role->get_uid());
	if (p_member_index.second == -1 || p_member_index.first == NULL) {
		log_error("troop[%lu] member_id[%lu] null", m_uid, p_role->get_uid());
		return;
	}

	if (m_leader_uid == p_role->get_uid()) { // 队长
		m_troop_mode = p_role->get_mode_state();
		reset_move(p_role->get_current_x(), p_role->get_current_y(), p_role->get_direction()); // 重置寻路点
		m_troop_speed = p_role->get_speed_by_type(is_troop_trade() ? proto::common::role_mode_state_trade : m_troop_mode);
		// 更新队伍速度
		for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
			if (NULL != m_troop_members[i] && 
				m_troop_members[i]->get_state() == proto::common::troop_state_normal && 
				m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
				role_ptr p_member_role = role_manager_t::find_role(m_troop_members[i]->get_uid());
				if (p_member_role != NULL) p_member_role->update_troop_speed(m_troop_speed);
			}
		}

		// 队长跨场景完成，加载队伍伙伴
		if (type != proto::server::relogin_server_scene && m_leader_uid == p_role->get_uid()) {
			for (uint32_t i = 1; i < m_troop_members.size(); ++i) {
				if (NULL != m_troop_members[i] && m_troop_members[i]->get_object_type() != proto::common::SCENEOBJECT_USER) {
					do_create_partner(m_troop_members[i], p_role->get_scene_id(), i);
				}
			}
		}

		//fill_member_hero_to_formation();
	} else {
		if (type == proto::server::user_troop_jump) {
			role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
			if (p_leader != NULL) {
				if (p_member_index.second >= TROOP_MEMBER_MAX_COUNT) {
					log_error("troop[%lu] member[%lu] index[%u] error", m_uid, p_role->get_uid(), p_member_index.second);
					return;
				}

				p_member_index.first->set_state(proto::common::troop_state_normal);
				p_role->update_troop_info(m_uid, proto::common::troop_state_normal, m_leader_uid == p_role->get_uid());
				// 直接传送(只针对本服务器的玩家)
				scene_manager_t::gm_jump_scene(p_role, p_leader->get_scene_id(), m_moves[p_member_index.second].m_x, m_moves[p_member_index.second].m_y, m_moves[p_member_index.second].m_dir);

				if (p_leader->get_fight_uid() != 0) { // 检测队长是否在战斗，在战斗就观战
					fight_manager_t::watch_combat(p_role, m_leader_uid, true);
				}

				if (p_leader->get_object_state() != proto::common::object_state_fighting) { // 不在战斗清理组队布阵
					//fill_member_hero_to_formation();
				}

				if (p_role->get_mode_state() == proto::common::role_mode_state_trade) { // 加入的人是跑商
					uint32_t new_troop_speed = p_leader->get_speed_by_type(proto::common::role_mode_state_trade);
					if (new_troop_speed != m_troop_speed) { // 队伍速度变化，全队更新
						update_troop_speed(new_troop_speed);
					} else {
						p_role->update_troop_speed(m_troop_speed); // 否则只更新自己的
					}
				} else { // 不是跑商
					if (m_troop_mode == proto::common::role_mode_state_trade) {
						p_role->update_troop_mount(proto::common::role_mode_state_normal, m_troop_speed);
					} else {
						p_role->update_troop_mount(m_troop_mode, m_troop_speed);
					}
				}

				// 队长在巡逻设置队员巡逻
				if (p_leader->is_patrol()) {
					p_role->troop_patrol(true);
				}

				// 通知中心服
				proto::server::ge_troop_transmission_notify center_ntf;
				center_ntf.set_result(1);
				env::server->send_msg_to_center(op_cmd::ge_troop_transmission_notify, p_role->get_uid(), center_ntf);

				notify_event(proto::common::troop_event_back, p_role->get_name());
				TROOP_LOG("troop[%lu] role[%lu] back", m_uid, p_role->get_uid());
			}
		} else {
			if (p_member_index.first->get_state() == proto::common::troop_state_normal) {
				role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
				if (p_leader != NULL) { // 队长
					if (p_role->get_mode_state() == proto::common::role_mode_state_trade) { // 加入的人是跑商
						uint32_t new_troop_speed = p_leader->get_speed_by_type(proto::common::role_mode_state_trade);
						if (new_troop_speed != m_troop_speed) { // 队伍速度变化，全队更新
							update_troop_speed(new_troop_speed);
						} else {
							p_role->update_troop_speed(m_troop_speed);
						}
					} else { // 不是跑商
						p_role->update_troop_speed(m_troop_speed);
					}
				}
			}
		}
	}
}

void troop_t::do_create_troop(uint64_t member_id, proto::server::troop_server_data* p_server_data)
{
	if (!p_server_data) {
		log_error("troop_server_data null error");
		return;
	}

	proto::common::troop_data* td = p_server_data->mutable_base();
	if (!td) {
		log_error("troop_data null error");
		return;
	}

	if (td->members_size() < 1) {
		log_error("troop[%lu] leader[%lu] create_troop member size == 0", m_uid, member_id);
		return;
	}

	m_leader_uid = member_id;

	update_troop_modify(td);

	troop_member_ptr p_member(new troop_member());
	if (p_member == NULL) {
		log_error("p_member null error");
		return;
	}

	p_member->load_data(td->mutable_members(0));
	m_troop_members.push_back(p_member);

	notify_event(proto::common::troop_event_create, p_member->get_name());

	// 创建队伍一定是玩家
	role_ptr p_leader_role = role_manager_t::find_role(m_leader_uid);
	if (NULL != p_leader_role) { // 设置队伍状态和速度
		// 设置跑商状态
		p_member->set_is_trade(p_leader_role->get_mode_state() == proto::common::role_mode_state_trade);
		p_leader_role->update_troop_info(m_uid, p_member->get_state(), true);
		m_troop_mode = p_leader_role->get_mode_state();
		m_troop_speed = p_leader_role->get_real_speed();
		// 重置寻路点
		reset_move(p_leader_role->get_current_x(), p_leader_role->get_current_y(), p_leader_role->get_direction()); 
	}

	TROOP_LOG("role[%lu] create new troop[%lu]", member_id, m_uid);
}


void troop_t::do_change_leader(uint64_t member_id, proto::server::troop_server_data* p_server_data)
{
	if (!p_server_data) {
		log_error("troop_server_data null error");
		return;
	}

	proto::common::troop_data* td = p_server_data->mutable_base();
	if (!td) {
		log_error("troop_data null error");
		return;
	}

	if (td->members_size() < 1) {
		log_error("troop[%lu] do_change_leader size == 0", m_uid);
		return;
	}

	int32_t index = -1; // 第几个
	troop_member_ptr p_new_leader_member = NULL;
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_uid() == member_id) {
			p_new_leader_member = m_troop_members[i];
			index = i;
			break;
		}
	}

	if (p_new_leader_member == NULL || index == -1) {
		log_error("troop[%lu] new leader[%lu] not find", m_uid, member_id);
		return;
	}

	if (p_new_leader_member->get_object_type() != proto::common::SCENEOBJECT_USER) {
		log_error("troop[%lu] new leader[%lu] not user", m_uid, member_id);
		return;
	}

	p_new_leader_member->load_data(td->mutable_members(0));

	if (index != 0) std::swap(m_troop_members[0], m_troop_members[index]); // 交换位置

	uint64_t old_leader_uid = m_leader_uid;
	m_leader_uid = member_id;

	role_ptr p_leader_role = role_manager_t::find_role(m_leader_uid);
	if (NULL != p_leader_role) {
		if (td->has_zg_task_info()) { // 有抓鬼接任务
			proto::common::role_change_data rcd;
			p_leader_role->accept_sync_task(td->zg_task_info(), rcd);
		}

		if (p_leader_role->is_troo_transmission())
			p_leader_role->do_break_transmission();

		// 设置跑商状态
		p_new_leader_member->set_is_trade(p_leader_role->get_mode_state() == proto::common::role_mode_state_trade);
		p_leader_role->update_troop_info(m_uid, p_new_leader_member->get_state(), true);
		m_troop_mode = p_leader_role->get_mode_state();
		uint32_t new_speed = p_leader_role->get_speed_by_type(is_troop_trade() ? proto::common::role_mode_state_trade : m_troop_mode);
		update_troop_speed(new_speed);
		// 重置寻路点
		reset_move(p_leader_role->get_current_x(), p_leader_role->get_current_y(), p_leader_role->get_direction());
	}

	role_ptr p_old_leader = role_manager_t::find_role(old_leader_uid);
	if (NULL != p_old_leader) {
		if (p_old_leader->get_object_state() != proto::common::object_state_fighting) { // 队员变动就直接清空组队布阵数据(战斗中不清空)
			//fill_member_hero_to_formation();
		}

		troop_member_ptr p_old_leader_member = get_member(old_leader_uid); // 队长退队的话就是找不到,找不到不用处理说明已经处理过了
		if (p_old_leader_member != NULL) p_old_leader->update_troop_info(m_uid, p_old_leader_member->get_state(), false);
	} else {
		//fill_member_hero_to_formation();
	}

	notify_event(proto::common::troop_event_leader, p_new_leader_member->get_name());

	TROOP_LOG("troop[%lu] change leader from role[%lu] to role[%lu]", m_uid, old_leader_uid, m_leader_uid);
}

void troop_t::do_member_join(uint64_t member_id, proto::server::troop_server_data* p_server_data)
{
	if (!p_server_data) {
		log_error("troop_server_data null error");
		return;
	}

	proto::common::troop_data* td = p_server_data->mutable_base();
	if (!td) {
		log_error("troop_data null error");
		return;
	}

	if (td->members_size() < 1) {
		log_error("troop[%lu] member_join size == 0", m_uid);
		return;
	}

	if (is_full()) {
		log_error("troop[%lu] is full", m_uid);
		return;
	}

	troop_member_ptr p_member(new troop_member());
	if (p_member == NULL) {
		log_error("p_member null error");
		return;
	}
	p_member->load_data(td->mutable_members(0));
	m_troop_members.push_back(p_member);

	role_ptr p_leader = role_manager_t::find_role(m_leader_uid);

	if (p_member->get_object_type() == proto::common::SCENEOBJECT_USER) {
		role_ptr p_join_member = role_manager_t::find_role(member_id);
		if (p_join_member != NULL) { // 找到玩家
			if (td->has_zg_task_info()) { // 有抓鬼接任务
				proto::common::role_change_data rcd;
				p_join_member->accept_sync_task(td->zg_task_info(), rcd);
			}

			if (p_join_member->get_pk_mode() != proto::common::PK_PEACE) { // 玩家不能直接传送
				notify_event(proto::common::troop_event_join, p_member->get_name());
				p_join_member->update_troop_info(m_uid, p_member->get_state(), member_id == m_leader_uid);
				if (p_join_member->start_transmission(proto::common::transmission_type_troop) == common::errcode_enum::error_ok) { // 组队读条
					proto::client::gc_jump_reading_start_notify ntf;
					ntf.set_type(proto::common::transmission_type_troop);
					ntf.set_reply_code(common::errcode_enum::error_ok);
					p_join_member->send_msg_to_client(op_cmd::gc_jump_reading_start_notify, ntf);
				}
			} else {
				if (p_leader != NULL) { // 同个服务器的处理
					int32_t index = (int32_t)m_troop_members.size() - 1;
					if (index >= TROOP_MEMBER_MAX_COUNT) {
						log_error("troop[%lu] member[%lu] size[%u] error", m_uid, member_id, (uint32_t)m_troop_members.size());
						return;
					}

					p_member->set_state(proto::common::troop_state_normal);
					p_member->set_is_trade(p_join_member->get_mode_state() == proto::common::role_mode_state_trade);
					notify_event(proto::common::troop_event_join, p_member->get_name());
					p_join_member->update_troop_info(m_uid, proto::common::troop_state_normal, member_id == m_leader_uid);
					// 直接传送(只针对本服务器的玩家)
					scene_manager_t::gm_jump_scene(p_join_member, p_leader->get_scene_id(), m_moves[index].m_x, m_moves[index].m_y, m_moves[index].m_dir);

					if (p_join_member->get_fight_uid() == 0 && p_leader->get_fight_uid() != 0) { // 检测队长是否在战斗，在战斗就观战
						fight_manager_t::watch_combat(p_join_member, m_leader_uid, true);
					}

					if (p_leader->get_object_state() != proto::common::object_state_fighting) { // 不在战斗清理组队布阵

					}

					// 队长在巡逻设置队员巡逻
					if (p_leader->is_patrol()) {
						p_join_member->troop_patrol(true);
					}

					if (p_join_member->get_mode_state() == proto::common::role_mode_state_trade) { // 加入的人是跑商
						uint32_t new_troop_speed = p_leader->get_speed_by_type(proto::common::role_mode_state_trade);
						if (new_troop_speed != m_troop_speed) { // 队伍速度变化，全队更新
							update_troop_speed(new_troop_speed);
						} else {
							p_join_member->update_troop_speed(m_troop_speed); // 否则只更新自己的
						}
					} else { // 不是跑商
						if (m_troop_mode == proto::common::role_mode_state_trade) {
							p_join_member->update_troop_mount(proto::common::role_mode_state_normal, m_troop_speed);
						} else {
							p_join_member->update_troop_mount(m_troop_mode, m_troop_speed);
						}
					}

					// 这种情况直接通知中心服
					proto::server::ge_troop_transmission_notify center_ntf;
					center_ntf.set_result(1);
					env::server->send_msg_to_center(op_cmd::ge_troop_transmission_notify, p_join_member->get_uid(), center_ntf);
				} else { // 不在同一个服务器传送
					scene_manager_t::troop_do_cross_server(p_join_member);
				}
			}
		} else { // 不在这个服务器
			notify_event(proto::common::troop_event_join, p_member->get_name());
		}
	} else {
		// 对于伙伴把附加数据保存，用于创建对象
		p_member->load_extra(p_server_data->mutable_extra_data());

		notify_event(proto::common::troop_event_join, p_member->get_name());

		if (p_leader != NULL) { // 在队长所在服务器招伙伴
			int32_t index = (int32_t)m_troop_members.size() - 1;
			if (index >= TROOP_MEMBER_MAX_COUNT) {
				log_error("troop[%lu] member[%lu] size[%u] error", m_uid, member_id, (uint32_t)m_troop_members.size());
				return;
			}

			do_create_partner(p_member, p_leader->get_scene_id(), index);
		}
	}
	//fill_member_hero_to_formation();

	TROOP_LOG("member[%lu] join troop[%lu]", member_id, m_uid);
}

void troop_t::do_member_quit(uint64_t member_id, bool is_notify)
{
	troop_member_vec::iterator itr = std::find_if(m_troop_members.begin(), m_troop_members.end(),
		[member_id](const troop_member_ptr& p_member)
	{
		if (NULL != p_member && p_member->get_uid() == member_id) return true;
		return false;
	});

	if (itr == m_troop_members.end()) {
		log_error("user[%lu] not in troop[%lu], when member quit", member_id, m_uid);
		return;
	}

	role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
	if (NULL == p_leader || p_leader->get_object_state() != proto::common::object_state_fighting) { // 队员变动就直接清空组队布阵数据(战斗中不清空)
		//fill_member_hero_to_formation();
	}

	// 智能指针先拷贝然后删除
	troop_member_ptr p_member = *itr;
	m_troop_members.erase(itr);

	if (p_member == NULL) {
		log_error("troop[%lu] member_id[%lu] null error", m_uid, member_id);
		return;
	}

	if (is_notify)
		notify_event(proto::common::troop_event_quit, p_member->get_name());

	if (p_member->get_object_type() == proto::common::SCENEOBJECT_USER) {
		role_ptr p_quit_member = role_manager_t::find_role(member_id);
		if (p_quit_member != NULL) {
			// 放弃抓鬼任务如果有的话
			p_quit_member->drop_circle_task(TASK_TYPE_ZHUOGUI);

			if (p_quit_member->is_troo_transmission())
				p_quit_member->do_break_transmission();
			p_quit_member->update_troop_info(0);
			p_quit_member->change_real_speed(); // 更新下自己的速度
			p_quit_member->notify_object_common_info_for_nearbies();
			if (p_leader != NULL &&
				m_leader_uid != member_id &&
				p_member->get_state() == proto::common::troop_state_normal) { // 不是队长退队，同个服务器未暂离和离线的处理
				uint32_t new_troop_speed = p_leader->get_speed_by_type(is_troop_trade() ? proto::common::role_mode_state_trade : m_troop_mode);
				update_troop_speed(new_troop_speed);
			}

			// 退队推出巡逻状态
			if (p_quit_member->is_patrol()) {
				p_quit_member->troop_patrol(false);
			}

			drop_treasure(member_id);

			if (p_quit_member->is_in_dungeon()) {
				scene_manager_t::role_back_scene(p_quit_member, proto::server::forced_change_scene);
			}
		}
	} else {
		partner_manager_t::delete_partner(member_id);
	}

	TROOP_LOG("role[%lu] quit troop[%lu]", member_id, m_uid);
}

void troop_t::do_member_kick(uint64_t member_id)
{
	troop_member_vec::iterator itr = std::find_if(m_troop_members.begin(), m_troop_members.end(),
		[member_id](const troop_member_ptr& p_member)
	{
		if (NULL != p_member && p_member->get_uid() == member_id) return true;
		return false;
	});

	if (itr == m_troop_members.end()) {
		log_error("user[%lu] not in troop[%lu], when member quit", member_id, m_uid);
		return;
	}

	role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
	if (NULL == p_leader || p_leader->get_object_state() != proto::common::object_state_fighting) { // 队员变动就直接清空组队布阵数据(战斗中不清空)
		//fill_member_hero_to_formation();
	}

	// 智能指针先拷贝然后删除
	troop_member_ptr p_member = *itr;
	m_troop_members.erase(itr);

	if (p_member == NULL) {
		log_error("troop[%lu] member_id[%lu] null error", m_uid, member_id);
		return;
	}

	notify_event(proto::common::troop_event_kick, p_member->get_name());

	if (p_member->get_object_type() == proto::common::SCENEOBJECT_USER) {
		role_ptr p_kick_member = role_manager_t::find_role(member_id);
		if (p_kick_member != NULL) {
			// 放弃抓鬼任务如果有的话
			p_kick_member->drop_circle_task(TASK_TYPE_ZHUOGUI);

			if (p_kick_member->is_troo_transmission())
				p_kick_member->do_break_transmission();
			p_kick_member->update_troop_info(0);
			p_kick_member->change_real_speed(); // 更新下自己的速度
			p_kick_member->notify_object_common_info_for_nearbies();
			if (p_leader != NULL && p_member->get_state() == proto::common::troop_state_normal) { // 同个服务器的处理
				uint32_t new_troop_speed = p_leader->get_speed_by_type(is_troop_trade() ? proto::common::role_mode_state_trade : m_troop_mode);
				update_troop_speed(new_troop_speed);
			}

			// 退队推出巡逻状态
			if (p_kick_member->is_patrol()) {
				p_kick_member->troop_patrol(false);
			}

			drop_treasure(member_id);

			proto::client::gc_troop_event_notify ntf;
			ntf.set_event(proto::common::troop_event_kick);
			ntf.set_member_name(p_member->get_name());
			p_kick_member->send_msg_to_client(op_cmd::gc_troop_event_notify, ntf);

			if (p_kick_member->is_in_dungeon()) {
				scene_manager_t::role_back_scene(p_kick_member, proto::server::forced_change_scene);
			}
		}
	} else {
		partner_manager_t::delete_partner(member_id);
	}

	TROOP_LOG("troop[%lu] kick role[%lu]", m_uid, member_id);
}


void troop_t::do_member_leave(uint64_t member_id)
{
	troop_member_ptr p_member = get_member(member_id);
	if (p_member == NULL) 
	{
		log_error("troop[%lu] member_id[%lu] null", m_uid, member_id);
		return;
	}

	p_member->set_state(proto::common::troop_state_leave);

	if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER) 
	{
		log_error("troop[%lu] leave member[%lu] not user", m_uid, member_id);
		return;
	}

	role_ptr p_leave_member = role_manager_t::find_role(member_id);
	if (p_leave_member != NULL) 
	{
		if (p_leave_member->is_troo_transmission())
		{
			p_leave_member->do_break_transmission();
		}
		p_leave_member->update_troop_info(m_uid, proto::common::troop_state_leave, m_leader_uid == member_id);
		p_leave_member->change_real_speed(); // 更新下自己的速度
		p_leave_member->notify_object_common_info_for_nearbies();
		// 退队推出巡逻状态
		if (p_leave_member->is_patrol()) 
		{
			p_leave_member->troop_patrol(false);
		}

		role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
		if (p_leader != NULL) { // 同个服务器的处理
			uint32_t new_troop_speed = p_leader->get_speed_by_type(is_troop_trade() ? proto::common::role_mode_state_trade : m_troop_mode);
			update_troop_speed(new_troop_speed);
		}
	}

	notify_event(proto::common::troop_event_leave, p_member->get_name());

	TROOP_LOG("troop[%lu] role[%lu] leave", m_uid, member_id);
}


void troop_t::do_member_back(uint64_t member_id, proto::server::troop_server_data* p_server_data)
{
	if (!p_server_data) {
		log_error("troop_server_data null error");
		return;
	}

	proto::common::troop_data* td = p_server_data->mutable_base();
	if (!td) {
		log_error("troop_data null error");
		return;
	}

	troop_member_index p_member_index = get_member_index(member_id);
	if (p_member_index.second == -1 || p_member_index.first == NULL) {
		log_error("troop[%lu] member_id[%lu] null", m_uid, member_id);
		return;
	}

	if (p_member_index.first->get_object_type() != proto::common::SCENEOBJECT_USER) {
		log_error("troop[%lu] back member[%lu] not user", m_uid, member_id);
		return;
	}

	role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
	if (NULL == p_leader) { // 
		//fill_member_hero_to_formation();
	}

	role_ptr p_back_member = role_manager_t::find_role(member_id);
	if (p_back_member != NULL) {
		if (td->has_zg_task_info()) { // 有抓鬼接任务
			proto::common::role_change_data rcd;
			p_back_member->accept_sync_task(td->zg_task_info(), rcd);
		}

		p_back_member->update_troop_info(m_uid, p_member_index.first->get_state(), member_id == m_leader_uid);

		if (p_back_member->get_pk_mode() != proto::common::PK_PEACE) { // 玩家不能直接传送
			if (p_back_member->start_transmission(proto::common::transmission_type_troop) == common::errcode_enum::error_ok) { // 组队读条
				proto::client::gc_jump_reading_start_notify ntf;
				ntf.set_type(proto::common::transmission_type_troop);
				ntf.set_reply_code(common::errcode_enum::error_ok);
				p_back_member->send_msg_to_client(op_cmd::gc_jump_reading_start_notify, ntf);
			}

			TROOP_LOG("troop[%lu] role[%lu] back transmission", m_uid, member_id);
		} else {
			if (p_leader != NULL) { // 同个服务器的处理
				if (p_member_index.second >= TROOP_MEMBER_MAX_COUNT) {
					log_error("troop[%lu] member[%lu] index[%u] error", m_uid, member_id, p_member_index.second);
					return;
				}

				p_member_index.first->set_state(proto::common::troop_state_normal);
				p_member_index.first->set_is_trade(p_back_member->get_mode_state() == proto::common::role_mode_state_trade);
				p_back_member->update_troop_info(m_uid, proto::common::troop_state_normal, member_id == m_leader_uid);
				// 直接传送(只针对本服务器的玩家)
				scene_manager_t::gm_jump_scene(p_back_member, p_leader->get_scene_id(), m_moves[p_member_index.second].m_x, m_moves[p_member_index.second].m_y, m_moves[p_member_index.second].m_dir);

				if (p_back_member->get_fight_uid() == 0 && (p_leader->get_fight_uid() != 0 || p_leader->get_watching_fight_uid() != 0))
				{ // 检测队长是否在战斗/观战，在就观战
					fight_manager_t::watch_combat(p_back_member, m_leader_uid, true);
				}

				if (p_leader->get_object_state() != proto::common::object_state_fighting) { // 不在战斗清理组队布阵
					//fill_member_hero_to_formation();
				}

				// 队长在巡逻设置队员巡逻
				if (p_leader->is_patrol()) {
					p_back_member->troop_patrol(true);
				}

				if (p_back_member->get_mode_state() == proto::common::role_mode_state_trade) { // 加入的人是跑商
					uint32_t new_troop_speed = p_leader->get_speed_by_type(proto::common::role_mode_state_trade);
					if (new_troop_speed != m_troop_speed) { // 队伍速度变化，全队更新
						update_troop_speed(new_troop_speed);
					} else {
						p_back_member->update_troop_speed(m_troop_speed); // 否则只更新自己的
					}
				} else { // 不是跑商
					if (m_troop_mode == proto::common::role_mode_state_trade) {
						p_back_member->update_troop_mount(proto::common::role_mode_state_normal, m_troop_speed);
					} else {
						p_back_member->update_troop_mount(m_troop_mode, m_troop_speed);
					}
				}

				// 这种情况直接通知中心服
				proto::server::ge_troop_transmission_notify center_ntf;
				center_ntf.set_result(1);
				env::server->send_msg_to_center(op_cmd::ge_troop_transmission_notify, p_back_member->get_uid(), center_ntf);

				notify_event(proto::common::troop_event_back, p_back_member->get_name());
				TROOP_LOG("troop[%lu] role[%lu] back", m_uid, member_id);
			} else { // 不在同一个服务器传送
				scene_manager_t::troop_do_cross_server(p_back_member);
				TROOP_LOG("troop[%lu] role[%lu] back cross_server", m_uid, member_id);
			}
		}
	}
}

void troop_t::do_troop_modify(uint64_t member_id, proto::server::troop_server_data* p_server_data)
{
	if (!p_server_data) {
		log_error("troop_server_data null error");
		return;
	}

	proto::common::troop_data* td = p_server_data->mutable_base();
	if (!td) {
		log_error("troop_data null error");
		return;
	}

	update_troop_modify(td);

	troop_member_ptr p_member = get_member(member_id);
	if (p_member == NULL) {
		log_error("p_member[%lu] null error", member_id);
		return;
	}

	notify_event(proto::common::troop_event_modify, p_member->get_name());

	TROOP_LOG("troop[%lu] role[%lu] leave", m_uid, member_id);
}


void troop_t::do_member_update(uint64_t member_id, proto::server::troop_server_data* p_server_data)
{
	if (!p_server_data) {
		log_error("troop_server_data null error");
		return;
	}

	proto::common::troop_data* td = p_server_data->mutable_base();
	if (!td) {
		log_error("troop_data null error");
		return;
	}

	if (td->members_size() < 1) {
		log_error("troop[%lu] member_join size == 0", m_uid);
		return;
	}

	troop_member_ptr p_member = get_member(member_id);
	if (p_member == NULL) {
		log_error("troop[%lu] member_id[%lu] null", m_uid, member_id);
		return;
	}
	p_member->load_data(td->mutable_members(0));

	notify_event(proto::common::troop_event_update, p_member->get_name());

	TROOP_LOG("troop[%lu] role[%lu] update", m_uid, member_id);
}


void troop_t::do_member_online(uint64_t member_id, proto::server::troop_server_data* p_server_data)
{
	if (!p_server_data) {
		log_error("troop_server_data null error");
		return;
	}

	proto::common::troop_data* td = p_server_data->mutable_base();
	if (!td) {
		log_error("troop_data null error");
		return;
	}

	if (td->members_size() < 1) {
		log_error("troop[%lu] member_join size == 0", m_uid);
		return;
	}

	troop_member_ptr p_member = get_member(member_id);
	if (p_member == NULL) {
		log_error("troop[%lu] member_id[%lu] null", m_uid, member_id);
		return;
	}
	p_member->load_data(td->mutable_members(0));

	role_ptr p_online_member = role_manager_t::find_role(member_id);
	if (p_online_member != NULL) {
		p_online_member->update_troop_info(m_uid, p_member->get_state(), m_leader_uid == member_id);
		p_member->set_is_trade(p_online_member->get_mode_state() == proto::common::role_mode_state_trade);

		switch (p_member->get_state()) {
			case proto::common::troop_state_normal: { // 一般是队长上线
				break;
			}
			case proto::common::troop_state_leave: 
			case proto::common::troop_state_offline: {
				p_online_member->change_real_speed(); // 更新下自己的速度
				p_online_member->notify_object_common_info_for_nearbies();
				role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
				if (p_leader != NULL) { // 同个服务器的处理
					uint32_t new_troop_speed = p_leader->get_speed_by_type(is_troop_trade() ? proto::common::role_mode_state_trade : m_troop_mode);
					update_troop_speed(new_troop_speed);
				}
				break;
			}
			default: {
				log_error("role[%lu] troop state[%u] not define", member_id, p_member->get_state());
			}
		}
	}

	notify_event(proto::common::troop_event_online, p_member->get_name());

	TROOP_LOG("troop[%lu] role[%lu] online[%u]", m_uid, member_id, p_member->get_state());
}


void troop_t::do_troop_dissolve(uint64_t member_id)
{
	troop_member_ptr p_dissolve_member = get_member(member_id);
	if (p_dissolve_member == NULL) {
		log_error("troop[%lu] member_id[%lu] null when do_troop_dissolve", m_uid, member_id);
	}

	for (const auto& p_member : m_troop_members) {
		if (NULL == p_member) continue;
		if (p_member->get_object_type() == proto::common::SCENEOBJECT_USER) {
			role_ptr p_role = role_manager_t::find_role(p_member->get_uid());
			if (NULL != p_role) {
				p_role->drop_circle_task(TASK_TYPE_ZHUOGUI);

				if (p_role->is_troo_transmission())
					p_role->do_break_transmission();
				p_role->update_troop_info(0);
				p_role->change_real_speed(); // 更新下自己的速度
				p_role->notify_object_common_info_for_nearbies();
				// 退队推出巡逻状态
				if (p_role->is_patrol()) {
					p_role->troop_patrol(false);
				}

				proto::client::gc_troop_event_notify ntf;
				ntf.set_event(proto::common::troop_event_dissolve);
				if (p_dissolve_member != NULL) {
					ntf.set_member_name(p_dissolve_member->get_name());
				}
				p_role->send_msg_to_client(op_cmd::gc_troop_event_notify, ntf);

				if (p_role->is_in_dungeon()) {
					scene_manager_t::role_back_scene(p_role, proto::server::forced_change_scene);
				}
			}
		} else {
			partner_manager_t::delete_partner(p_member->get_uid());
		}
	}

	TROOP_LOG("troop[%lu] role[%lu] dissolve", m_uid, member_id);
}


void troop_t::do_member_transmission(uint64_t member_id)
{
	troop_member_ptr p_member = get_member(member_id);
	if (p_member == NULL) {
		log_error("troop[%lu] member_id[%lu] null", m_uid, member_id);
		return;
	}

	if (p_member->get_state() == proto::common::troop_state_leave) {
		p_member->set_state(proto::common::troop_state_normal);

		notify_event(proto::common::troop_event_back, p_member->get_name());
		TROOP_LOG("troop[%lu] role[%lu] back", m_uid, member_id);
	}
}


uint32_t troop_t::do_transmission_reading_end(role_ptr p_role)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	troop_member_index p_member_index = get_member_index(p_role->get_uid());
	if (p_member_index.second == -1 || p_member_index.first == NULL) {
		log_error("troop[%lu] member_id[%lu] null", m_uid, p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (p_role->get_transmission_type() != proto::common::transmission_type_troop) {
		log_error("role[%lu] troop transmission reading error", p_role->get_uid());
		return errcode_enum::notice_pk_mode_reading_error;
	}

	uint32_t reading_time = scene_manager_t::get_pk_jump_time(p_role->get_pk_mode());
	uint32_t start_time = p_role->get_object_state_param();
	// 模糊判断
	if (start_time + reading_time - 2 > common::time_util_t::now_time() || common::time_util_t::now_time() > start_time + reading_time + 2) {
		log_error("role[%lu] troop transmission reading time error", p_role->get_uid());
		// 重置状态
		p_role->cancel_transmission();
		return errcode_enum::notice_pk_mode_reading_error;
	}

	// 重置状态
	p_role->cancel_transmission();

	role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
	if (p_leader != NULL) { // 同个服务器的处理
		if (p_member_index.second >= TROOP_MEMBER_MAX_COUNT) {
			log_error("troop[%lu] member[%lu] index[%u] error", m_uid, p_role->get_uid(), p_member_index.second);
			return common::errcode_enum::notice_unknown;
		}

		p_member_index.first->set_state(proto::common::troop_state_normal);
		p_member_index.first->set_is_trade(p_role->get_mode_state() == proto::common::role_mode_state_trade);
		p_role->update_troop_info(m_uid, proto::common::troop_state_normal, p_role->get_uid() == m_leader_uid);
		// 直接传送(只针对本服务器的玩家)
		scene_manager_t::gm_jump_scene(p_role, p_leader->get_scene_id(), m_moves[p_member_index.second].m_x, m_moves[p_member_index.second].m_y, m_moves[p_member_index.second].m_dir);

		if (p_role->get_fight_uid() == 0 && p_leader->get_fight_uid() != 0) { // 检测队长是否在战斗，在战斗就观战
			fight_manager_t::watch_combat(p_role, m_leader_uid, true);
		}

		if (p_leader->get_object_state() != proto::common::object_state_fighting) { // 不在战斗清理组队布阵
			//fill_member_hero_to_formation();
		}

		// 队长在巡逻设置队员巡逻
		if (p_leader->is_patrol()) {
			p_role->troop_patrol(true);
		}

		if (p_role->get_mode_state() == proto::common::role_mode_state_trade) { // 加入的人是跑商
			uint32_t new_troop_speed = p_leader->get_speed_by_type(proto::common::role_mode_state_trade);
			if (new_troop_speed != m_troop_speed) { // 队伍速度变化，全队更新
				update_troop_speed(new_troop_speed);
			} else {
				p_role->update_troop_speed(m_troop_speed); // 否则只更新自己的
			}
		} else { // 不是跑商
			if (m_troop_mode == proto::common::role_mode_state_trade) {
				p_role->update_troop_mount(proto::common::role_mode_state_normal, m_troop_speed);
			} else {
				p_role->update_troop_mount(m_troop_mode, m_troop_speed);
			}
		}

		// 通知中心服
		proto::server::ge_troop_transmission_notify center_ntf;
		center_ntf.set_result(1);
		env::server->send_msg_to_center(op_cmd::ge_troop_transmission_notify, p_role->get_uid(), center_ntf);

		notify_event(proto::common::troop_event_back, p_role->get_name());
		TROOP_LOG("troop[%lu] role[%lu] back", m_uid, p_role->get_uid());
	} else { // 不在同一个服务器传送
		scene_manager_t::troop_do_cross_server(p_role);
		TROOP_LOG("troop[%lu] role[%lu] back cross_server", m_uid, p_role->get_uid());
	}

	return common::errcode_enum::error_ok;
}

void troop_t::leader_move(const scene_ptr& p_scene, uint32_t speed, float x, float y, float direction, uint32_t high)
{
	if (NULL == p_scene) {
		log_warn("nullptr == pScene");
		return;
	}

	if (speed > 10) {
		int32_t troop_dir = map_t::get_comp_dir(x, y, m_moves[0].m_x, m_moves[0].m_y);
		for (int32_t i = 0; i < TROOP_MEMBER_MAX_COUNT; ++i) {
			m_moves[i].m_x = x + WALK_ADJUST[troop_dir][0] * i;
			m_moves[i].m_y = y + WALK_ADJUST[troop_dir][1] * i;
			m_moves[i].m_dir = direction;
			m_moves[i].m_high = 0;
		}
	} else {
		m_moves.pop_back();
		m_moves.push_front(troop_move(x, y, direction, high));
	}

	for (uint32_t i = 1, index = 1; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			object_base_ptr p_object = object_manager_t::find_object(m_troop_members[i]->get_object_id_type());
			if (NULL != p_object && index < TROOP_MEMBER_MAX_COUNT) {
				p_scene->on_move_s(p_object, m_moves[index].m_x, m_moves[index].m_y, m_moves[index].m_dir, 0, m_moves[index].m_high);
				++index;
			}
		}
	}
}


void troop_t::member_fast_move(const scene_ptr& p_scene, uint32_t move_type, float x, float y, float dir)
{
	if (NULL == p_scene) {
		log_warn("nullptr == pScene");
		return;
	}

	// 重置跟随点
	reset_move(x, y, dir);

	for (uint32_t i = 1; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			if (m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
				role_ptr p_role = role_manager_t::find_role(m_troop_members[i]->get_uid());
				if (NULL != p_role) {
					// 对于不是瞬移的广播9屏
					proto::client::gc_object_scene_jump_notify notify;
					notify.set_uid(common::string_util_t::uint64_to_string(p_role->get_object_id()));
					notify.set_object_type(p_role->get_object_type());
					notify.set_move_type(move_type);
					notify.set_dest_x(x);
					notify.set_dest_y(y);
					notify.set_dest_dir(dir);

					if (move_type == 0) {
						p_role->send_msg_to_client(op_cmd::gc_object_scene_jump_notify, notify);
					} else {
						p_role->send_msg_to_nine(op_cmd::gc_object_scene_jump_notify, notify);
					}

					p_scene->on_move_s(p_role, x, y, dir, 0, 0);
				}
			} else {
				partner_ptr p_partner = partner_manager_t::find_partner(m_troop_members[i]->get_uid());
				if (p_partner) {
					p_scene->on_move_s(p_partner, x, y, dir, 0, 0);
				}
			}
		}
	}
}


void troop_t::reset_move(float x, float y, float dir)
{
	for (troop_move_deque::iterator itr = m_moves.begin();
		itr != m_moves.end(); ++itr) {
		itr->m_x = x;
		itr->m_y = y;
		itr->m_dir = dir;
		itr->m_high = 0;
	}
}


void troop_t::follow_jump_scene(proto::server::scene_change_type change_type, uint64_t scene_id, float des_x /*= -1*/, float des_y /*= -1*/, float des_dir /*= 0.f*/)
{
	for (uint32_t i = 1; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			if (m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
				role_ptr p_role = role_manager_t::find_role(m_troop_members[i]->get_uid());
				if (NULL != p_role) {
					scene_manager_t::do_jump_scene(p_role, change_type, scene_id, des_x, des_y, des_dir);
				}
			} else {
				const scene_ptr& p_scene = scene_manager_t::find_scene(scene_id);
				if (p_scene) {
					partner_ptr p_partner = partner_manager_t::find_partner(m_troop_members[i]->get_uid());
					if (p_partner) {
						p_partner->leave_scene();
						p_scene->on_enter_s(p_partner, des_x, des_y, des_dir);
					}
				} else {
					partner_manager_t::delete_partner(m_troop_members[i]->get_uid());
				}
			}
		}
	}
}


void troop_t::follow_cross_server(const proto::server::cross_server_data& data)
{
	// 队员先过去
	for (uint32_t i = 1; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			if (m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
				role_ptr p_role = role_manager_t::find_role(m_troop_members[i]->get_uid());
				if (NULL != p_role) {
					role_manager_t::del_role_by_cross_server(p_role->get_uid(), data.type());

					proto::server::ga_user_cross_server_notify notify;
					proto::server::cross_server_data* p_data = notify.mutable_data();
					if (p_data) p_data->CopyFrom(data);
					p_role->send_msg_to_gate(op_cmd::ga_user_cross_server_notify, notify);

					SCENE_LOG("role[%lu] cross server jump scene[%lu], change type[%u]!", p_role->get_uid(), data.scene_id(), data.type());
				}
			} else {
				partner_manager_t::delete_partner(m_troop_members[i]->get_uid());
			}
		}
	}
}


bool troop_t::is_member_trade()
{
	for (uint32_t i = 1; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && 
			m_troop_members[i]->get_state() == proto::common::troop_state_normal && 
			m_troop_members[i]->is_trade_state()) {
			return true;
		}
	}

	return false;
}


bool troop_t::is_troop_trade()
{
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] &&
			m_troop_members[i]->get_state() == proto::common::troop_state_normal &&
			m_troop_members[i]->is_trade_state()) {
			return true;
		}
	}

	return false;
}


uint32_t troop_t::member_scene_check(const scene_ptr& p_scene)
{
	if (p_scene == NULL) {
		log_error("p_scene null error");
		return common::errcode_enum::notice_unknown;
	}

	for (uint32_t i = 1; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] &&
			m_troop_members[i]->get_state() == proto::common::troop_state_normal &&
			m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
			role_ptr p_member_role = role_manager_t::find_role(m_troop_members[i]->get_uid());
			if (NULL != p_member_role) {
				uint32_t scene_check = p_scene->scene_enter_check(p_member_role);
				if (scene_check != common::errcode_enum::error_ok) {
					// SCENE_LOG("troop role[%lu] scene[%lu] check error:%u", p_member_role->get_uid(), p_scene->get_scene_id(), scene_check);
					return scene_check;
				}
			}
		}
	}

	return common::errcode_enum::error_ok;
}

void troop_t::update_mount_state(proto::common::role_mode_state state, uint32_t speed)
{
	m_troop_mode = state;
	m_troop_speed = speed;

	for (uint32_t i = 1; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] &&
			m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			if (m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
				role_ptr p_member_role = role_manager_t::find_role(m_troop_members[i]->get_uid());
				if (NULL != p_member_role) {
					p_member_role->update_troop_mount(m_troop_mode, m_troop_speed);
				}
			} else {
				partner_ptr p_partner = partner_manager_t::find_partner(m_troop_members[i]->get_uid());
				if (p_partner) {
					p_partner->set_real_speed(m_troop_speed);
				}
			}
		}
	}
}


bool troop_t::update_trade_state(const role_ptr& p_role, proto::common::role_mode_state state)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return false;
	}

	troop_member_ptr p_member = get_member(p_role->get_uid());
	if (p_member == NULL) {
		log_error("p_member[%lu] null error", p_role->get_uid());
		return false;
	}

	// 设置跑商状态
	p_member->set_is_trade(state == proto::common::role_mode_state_trade);

	if (m_leader_uid == p_role->get_uid()) { // 队长的处理
		m_troop_mode = state;
	}

	role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
	if (p_leader == NULL) {
		log_error("p_leader[%lu] null error", m_leader_uid);
		return false;
	}

	// 计算队伍速度 
	m_troop_speed = p_leader->get_speed_by_type(is_troop_trade() ? proto::common::role_mode_state_trade : m_troop_mode);

	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && 
			m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			if (m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
				if (m_troop_members[i]->get_uid() != p_role->get_uid()) { // 设置的人自己更新
					role_ptr p_member_role = role_manager_t::find_role(m_troop_members[i]->get_uid());
					if (p_member_role != NULL) {
						p_member_role->update_troop_speed(m_troop_speed);
					}
				}
			} else {
				partner_ptr p_partner = partner_manager_t::find_partner(m_troop_members[i]->get_uid());
				if (p_partner) {
					p_partner->set_real_speed(m_troop_speed);
				}
			}
		}
	}

	return true;
}


void troop_t::update_troop_speed(uint32_t new_speed)
{
	if (new_speed == m_troop_speed)
		return;
	m_troop_speed = new_speed;
	// 更新队伍速度
	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			if (m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER) {
				role_ptr p_member_role = role_manager_t::find_role(m_troop_members[i]->get_uid());
				if (p_member_role != NULL) p_member_role->update_troop_speed(m_troop_speed);
			} else {
				partner_ptr p_partner = partner_manager_t::find_partner(m_troop_members[i]->get_uid());
				if (p_partner) {
					p_partner->set_real_speed(m_troop_speed);
				}
			}
		}
	}
}


void troop_t::update_member_patrol(uint64_t uid, bool is_patrol)
{
	if (uid != m_leader_uid) {
		log_error("role[%lu] not troop leader, can't change member state", uid);
		return;
	}

	// 更新队伍成员巡逻状态
	for (uint32_t i = 1; i < m_troop_members.size(); ++i) {
		if (NULL != m_troop_members[i] && 
			m_troop_members[i]->get_object_type() == proto::common::SCENEOBJECT_USER && 
			m_troop_members[i]->get_state() == proto::common::troop_state_normal) {
			role_ptr p_member_role = role_manager_t::find_role(m_troop_members[i]->get_uid());
			if (p_member_role != NULL) p_member_role->troop_patrol(is_patrol);
		}
	}
}

void troop_t::start_team_check(uint32_t dungeon_id)
{
	m_dungeon_id = dungeon_id;

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dungeon_team_check_time);
	uint32_t team_check_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	m_team_check_end = common::time_util_t::now_time() + team_check_time;

	proto::client::gc_dugeon_team_check_request request;
	request.set_dungeon_id(dungeon_id);
	request.set_leader_id(common::string_util_t::uint64_to_string(m_leader_uid));

	for (const auto& p_member : m_troop_members) {
		if (NULL == p_member) {
			log_error("p_member null error");
			continue;
		}

		if (p_member->get_uid() == m_leader_uid) { // 队长默认确认
			p_member->set_team_check(proto::common::team_check_type_confirm);

			// 队长直接确认
			proto::common::teamcheck_data* p_data = request.add_datas();
			if (p_data != NULL) {
				p_data->set_uid(common::string_util_t::uint64_to_string(m_leader_uid));
				p_data->set_type(proto::common::team_check_type_confirm);
			}

		} else if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER) { // 伙伴默认确认
			p_member->set_team_check(proto::common::team_check_type_confirm);

			// 伙伴直接确认
			proto::common::teamcheck_data* p_data = request.add_datas();
			if (p_data != NULL) {
				p_data->set_uid(common::string_util_t::uint64_to_string(p_member->get_uid()));
				p_data->set_type(proto::common::team_check_type_confirm);
			}
		} else {
			p_member->set_team_check(proto::common::team_check_type_none);
		}

		if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER) {
			continue;
		}

		// 暂离和离线不发送
		if (p_member->get_state() != proto::common::troop_state_normal) {
			continue;
		}

		role_ptr p_role = role_manager_t::find_role(p_member->get_uid());
		if (NULL == p_role) {
			log_error("team[%lu] role[%lu] not find", m_uid, p_member->get_uid());
			continue;
		}
		p_role->send_msg_to_client(op_cmd::gc_dugeon_team_check_request, request);
	}
}


void troop_t::role_team_check(uint64_t uid, proto::common::team_check_type type)
{
	if (common::time_util_t::now_time() > m_team_check_end) {
		DUNGEON_LOG("role[%lu] team check is time out", uid);
		return;
	}

	if (type == proto::common::team_check_type_none) {
		log_error("role[%lu] check type[%u] error", uid, type);
		return;
	}

	troop_member_ptr p_member = get_member(uid);
	if (p_member == NULL) {
		log_error("role[%lu] not in team[%lu]", uid, m_uid);
		return;
	}

	if (p_member->get_team_check() != proto::common::team_check_type_none) {
		log_error("role[%lu] can't check repeate", uid);
		return;
	}

	p_member->set_team_check(type);

	// 通知所有队员情况
	proto::client::gc_sync_team_check_notify notify;
	notify.set_dungeon_id(m_dungeon_id);
	// 只发送改变数据
	proto::common::teamcheck_data* p_data = notify.add_datas();
	if (p_data != NULL) {
		p_data->set_uid(common::string_util_t::uint64_to_string(uid));
		p_data->set_type(p_member->get_team_check());
	}
	send_msg_to_all(op_cmd::gc_sync_team_check_notify, notify);

	check_team_all();
}


void troop_t::check_team_all()
{
	bool is_all_check = true;

	for (const auto& p_member : m_troop_members) {
		if (NULL == p_member) {
			log_error("p_member null error");
			is_all_check = false;
			break;
		}

		if (p_member->get_team_check() != proto::common::team_check_type_confirm) {
			is_all_check = false;
			break;
		}
	}

	// 所有人都确认
	if (is_all_check) {
		proto::client::gc_ask_team_check_reply reply;
		reply.set_reply_code(common::errcode_enum::error_ok);
		send_msg_to_all(op_cmd::gc_ask_team_check_reply, reply);

		// 开启副本
		dungeon_manager_t::open_dungeon_by_team(m_leader_uid, m_uid, m_dungeon_id);

		clear_team_check();
	}
}


void troop_t::clear_team_check()
{
	m_dungeon_id = 0;
	m_team_check_end = 0;
}


void troop_t::get_no_confirm_member(proto::client::gc_ask_team_check_reply& reply)
{
	if (m_team_check_end == 0) {
		log_error("team[%lu] not in team check", m_uid);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (common::time_util_t::now_time() < m_team_check_end) {
		reply.set_reply_code(common::errcode_enum::dungeon_team_check_time_not_in);
		return;
	}

	bool find_uncheck_member = false;

	for (const auto& p_member : m_troop_members) {
		if (NULL == p_member) {
			log_error("p_member null error");
			continue;
		}

		if (p_member->get_team_check() != proto::common::team_check_type_confirm) {
			find_uncheck_member = true;

			reply.set_reply_code(common::errcode_enum::dungeon_team_check_error);
			// 谁没确认
			proto::common::teamcheck_data* p_data = reply.add_datas();
			if (p_data != NULL) {
				p_data->set_uid(common::string_util_t::uint64_to_string(p_member->get_uid()));
				p_data->set_type(p_member->get_team_check());
			}

			break;
		}
	}

	if (!find_uncheck_member) {
		reply.set_reply_code(common::errcode_enum::error_ok);
	}

	clear_team_check();
}

//////////////////////////////////////////////////////////////////////////
void troop_t::notify_formation()
{
	//role_ptr p_leader = role_manager_t::find_role(m_leader_uid);
	//if (p_leader != NULL && p_leader->get_object_state() != proto::common::object_state_fighting) 
	{ // 不在战斗清理组队布阵
		fill_member_hero_to_formation();
	}
	formation_ptr form = m_formation_mgr.get_cur_formation();
	if (NULL == form)
	{
		log_error("user_troop form NULL");
		return;
	}
	proto::client::gc_troop_formation_ntf ntf;
	form->peek_data(ntf.mutable_form());
	send_msg_to_all(op_cmd::gc_troop_formation_ntf, ntf);
}

bool troop_t::fill_member_hero_to_formation()
{
	m_formation_mgr.get_cur_formation()->clear_heroes();
	object_base_ptr p_role = object_base_ptr();
	//object_base_ptr p_leader = object_base_ptr();

	troop_member_ptr p_member = troop_member_ptr();
	formation_ptr p_formation = m_formation_mgr.get_cur_formation();
	std::map<uint32_t, PosHeroData> hero_pos;

	troop_t::troop_member_vec member_list;
	get_normal_members(member_list);
	uint32_t hero_fight_pos = 0;
	uint32_t count = fight_manager_t::get_hero_count_by_member_size(member_list.size());
	for (uint32_t i = 0; i < member_list.size(); ++i)
	{
		p_member = member_list[i];
		if (NULL == p_member)
		{
			log_error("user_troop fill_member_hero_to_formation p_member NULL");
			continue;
		}

		p_role = object_manager_t::find_object(p_member->get_object_id_type());
		if (NULL == p_role)
		{
			//log_error("user_troop fill_member_hero_to_formation p_role[%lu] NULL", p_member->get_uid());
			continue;
		}
		hero_ptr p_hero = hero_ptr();

		std::vector<uint64_t> hero_list;
		p_role->get_hero_list(proto::common::fight_type_none, member_list.size(), hero_list);
		
		uint32_t index = 0;
		for (; index < hero_list.size(); ++index)
		{
			hero_pos.insert(std::make_pair(++hero_fight_pos, PosHeroData(p_member->get_object_id_type(), hero_list[index])));
		}
		for (; index < count; ++index)
		{
			hero_pos.insert(std::make_pair(++hero_fight_pos, PosHeroData(p_member->get_object_id_type(), 0)));
		}
		
	}
	m_formation_mgr.set_troop_member_num(member_list.size());
	m_formation_mgr.update_formation(NULL, m_formation_mgr.get_cur_formation_id(), proto::common::fight_type_none, hero_pos, 0);
	return true;
}

//////////////////////////////////////////////////////////////////////////
void troop_t::peek_treasure_data(proto::common::treasure_data* p_treasure_data)
{
	for (auto it : m_treasure_item)
	{
		proto::common::treasure_single* p_single = p_treasure_data->add_treasure_list();
		p_single->set_uid(string_util_t::uint64_to_string(it.first));
		p_single->set_item_tid(it.second);
	}
	p_treasure_data->set_treasure_id(m_treasure_id);
}

bool troop_t::use_treasure_item(uint64_t role_uid, uint32_t item_tid)
{
	if (m_treasure_id > 0)
	{
		log_error("role[%lu] already treasure", role_uid);
		return false;
	}
	auto iter = m_treasure_item.find(role_uid);
	if (iter != m_treasure_item.end())
	{
		iter->second = item_tid;
	} else
	{
		m_treasure_item.insert(std::make_pair(role_uid, item_tid));
	}

	//广播消息给队员
	proto::client::gc_treasure_data_reply reply;
	//尝试开启藏宝图
	try_open_treasure();
	TREASURE_LOG("role[%lu] try_open_treasure:%d", role_uid, m_treasure_id);
	peek_treasure_data(reply.mutable_treasure_data());
	send_msg_to_all(op_cmd::gc_treasure_data_reply, reply);
	if (m_treasure_id > 0)
	{
		for (auto it : m_treasure_item)
		{
			uint64_t role_uid = it.first;
			uint32_t item_tid = it.second;
			role_ptr p_role = role_manager_t::find_role(role_uid);
			if (NULL == p_role)
			{
				continue;
			}

			//任务开启
			task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
			if (NULL == p_task_mgr)
			{
				continue;
			}
			proto::common::role_change_data rcd;
			item_manager_t::remove_item(p_role, item_tid, 1, log_enum::source_type_role_use_treasure_item, 0, &rcd,
				proto::common::package_type_main);
			p_task_mgr->active_treasure_task(m_treasure_id, rcd);
		}
	}

	return true;
}

void troop_t::open_treasure(uint64_t role_uid)
{
	if (m_troop_members.size() <= 1)
	{
		log_error("role[%lu] open_treasure failed, member size not enough", role_uid);
		return;
	}
	if (role_uid != m_leader_uid)
	{
		log_error("role[%lu] open_treasure failed, not leader", role_uid);
		return;
	}
	for (const auto& p_member : m_troop_members)
	{
		if (NULL == p_member)
		{
			log_error("NULL == p_member");
			return;
		}

		if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER) {
			continue;
		}

		uint64_t member_uid = p_member->get_uid();
		role_ptr p_role = role_manager_t::find_role(member_uid);
		if (NULL == p_role)
		{
			log_error("role[%lu] open_treasure failed, not found", member_uid);
			return;
		}
		activity_mgr_ptr p_act_mgr = p_role->get_activity_mgr();
		if (NULL == p_act_mgr)
		{
			log_error("role[%lu] open_treasure failed, NULL == p_act_mgr", member_uid);
			return;
		}
		if (!p_act_mgr->check_open(activity_treasure))
		{
			log_error("role[%lu] open_treasure failed, activity_treasure check_open failed", member_uid);
			return;
		}
	}
	//广播消息给队员
	proto::client::gc_treasure_data_reply reply;
	peek_treasure_data(reply.mutable_treasure_data());
	send_msg_to_all(op_cmd::gc_treasure_data_reply, reply);
}

void troop_t::drop_treasure(uint64_t role_uid)
{
	if (m_treasure_id == 0) {
		////还没生成藏宝任务 所有玩家都退出
		//for (auto it : m_treasure_item)
		//{
		//	uint64_t member_uid = it.first;
		//	uint32_t item_tid = it.second;
		//	std::map<uint32_t, uint32_t> items_id_count;
		//	items_id_count[item_tid] = 1;
		//	//返还寻宝道具
		//	mail_manager_t::send_mail(member_uid, proto::common::MAIL_TYPE_SYS, 7, items_id_count);
		//}
		m_treasure_item.clear();
	} else {
		//已生成藏宝任务，只有自己退出
		auto it = m_treasure_item.find(role_uid);
		if (it == m_treasure_item.end())
		{
			log_error("drop_treasure error, role[%lu] not found", role_uid);
			return;
		}
		uint32_t item_tid = it->second;
		m_treasure_item.erase(it);
		std::map<uint32_t, uint32_t> items_id_count;
		items_id_count[item_tid] = 1;
		//返还寻宝道具
		mail_manager_t::send_mail(role_uid, proto::common::MAIL_TYPE_SYS, get_treasure_mail_id(), items_id_count);
		//放弃任务
		role_ptr p_self = role_manager_t::find_role(role_uid);
		if (NULL != p_self)
		{
			task_mgr_ptr p_task_mgr = p_self->get_task_mgr();
			if (NULL != p_task_mgr)
			{
				p_task_mgr->drop_treasure_task();
			}
		}
	}

	//广播消息给队员
	proto::client::gc_treasure_drop_reply reply;
	reply.set_uid(string_util_t::uint64_to_string(role_uid));
	send_msg_to_all(op_cmd::gc_treasure_drop_reply, reply);
}

void troop_t::gather_treasure(uint64_t role_uid)
{
	if (role_uid != m_leader_uid)
	{
		log_error("role[%lu] gather_treasure failed, not leader", role_uid);
		return;
	}
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] gather_treasure failed, not found", role_uid);
		return;
	}
	if (m_treasure_id == 0)
	{
		log_error("role[%lu] gather_treasure failed, not treasure", role_uid);
		return;
	}
	auto p_make_conf = GET_CONF(TreasureMake, m_treasure_id);
	if (NULL == p_make_conf)
	{
		log_error("role[%lu] gather_treasure failed, treasure[%d] invalid", role_uid, m_treasure_id);
		return;
	}
	if (p_role->get_map_tid() != p_make_conf->map())
	{
		log_error("role[%lu] p_role->get_map_tid()[%d] != p_make_conf->map()[%d]", role_uid, p_role->get_map_tid(), p_make_conf->map());
		return;
	}
	if (!p_role->is_nearby_point(p_make_conf->pos(), 3))
	{
		log_error("role[%lu] is_nearby_point error", role_uid);
		return;
	}
	uint32_t rand_max = 0;
	for (auto prob : p_make_conf->event_prob())
	{
		rand_max += prob;
	}
	if (0 == rand_max)
	{
		log_error("TreasureMake[%d] 0 == rand_max", m_treasure_id);
		return;
	}
	int32_t rand_val = random_util_t::randMin(0, rand_max);
	if (-1 == rand_val)
	{
		log_error("TreasureMake[%d] rand_max invalid", m_treasure_id);
		return;
	}
	uint32_t rand_index = 0;
	for (auto prob : p_make_conf->event_prob())
	{
		if ((uint32_t)rand_val < prob)
		{
			break;
		}
		rand_val -= prob;
		++rand_index;
	}
	if (rand_index >= p_make_conf->event_id().size())
	{
		log_error("p_make_conf[%d] error", m_treasure_id);
		return;
	}
	uint32_t treasure_event_id = p_make_conf->event_id().at(rand_index);
	std::vector<uint64_t> team;
	for (auto it : m_treasure_item)
	{
		team.push_back(it.first);
	}
	friendliness_t::add_friendliness_when_gather_treasure(team);
	for (auto it : m_treasure_item)
	{
		uint64_t member_uid = it.first;
		role_ptr p_member = role_manager_t::find_role(member_uid);
		if (NULL == p_member)
		{
			continue;
		}

		treasure_manager_t::gather_treasure(p_member, treasure_event_id, it.second);
		task_mgr_ptr p_task_mgr = p_member->get_task_mgr();
		if (NULL != p_task_mgr)
		{
			proto::common::role_change_data rcd;
			p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_TREASURE, rcd, 1, 1);
		}
		activity_mgr_ptr p_activity_mgr = p_member->get_activity_mgr();
		if (NULL != p_activity_mgr)
		{
			p_activity_mgr->activity_done(activity_treasure);
		}
	}
	m_treasure_item.clear();
	m_treasure_id = 0;
}

uint32_t troop_t::get_treasure_mail_id()
{
	auto treasure_mail_id = GET_CONF(Comprehensive, comprehensive_common::treasure_abandon_email_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(treasure_mail_id);
	return val;
}

void troop_t::try_open_treasure()
{
	if (m_troop_members.size() <= 1)
	{
		return;
	}
	if (m_treasure_item.size() < m_troop_members.size())
	{
		return;
	}
	Treasure* p_conf = GET_CONF(Treasure, treasure_type_troop);
	if (NULL == p_conf)
	{
		log_error("treasure conf error");
		return;
	}
	uint32_t rand_max = 0;
	for (auto prob : p_conf->treasure_make_prob())
	{
		rand_max += prob;
	}
	if (0 == rand_max)
	{
		log_error("Treasure[%d] 0 == rand_max", treasure_type_troop);
		return;
	}
	int32_t rand_val = random_util_t::randMin(0, rand_max);
	if (-1 == rand_val)
	{
		log_error("Treasure[%d] rand_max invalid", treasure_type_troop);
		return;
	}
	uint32_t rand_index = 0;
	for (auto prob : p_conf->treasure_make_prob())
	{
		if ((uint32_t)rand_val < prob)
		{
			break;
		}
		rand_val -= prob;
		++rand_index;
	}
	if (rand_index >= p_conf->treasure_make_id().size())
	{
		log_error("treasure conf error");
		return;
	}
	m_treasure_id = p_conf->treasure_make_id().at(rand_index);
}

bool troop_t::check_family(uint64_t family_id)
{
	for (uint32_t i = 1; i < m_troop_members.size(); ++i)
	{
		const troop_member_ptr& p_member = m_troop_members[i];
		if (NULL == p_member)
		{
			continue;
		}
		if (p_member->get_state() != 0)
		{
			continue;
		}

		if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER) {
			continue;
		}

		role_ptr p_role = role_manager_t::find_role(p_member->get_uid());
		if (NULL == p_role)
		{
			continue;
		}

		if (p_role->get_family_id() != family_id)
		{
			return false;
		}
	}
	return true;
}

void troop_t::do_member_zhuagui_task(uint64_t member_id, proto::server::troop_server_data* p_server_data)
{
	if (!p_server_data) {
		log_error("troop_server_data null error");
		return;
	}

	proto::common::troop_data* td = p_server_data->mutable_base();

	for (uint32_t i = 0; i < m_troop_members.size(); ++i) {
		const troop_member_ptr& p_member = m_troop_members[i];
		if (NULL == p_member) continue;

		if (p_member->get_uid() == member_id) continue;

		if (p_member->get_state() != 0) continue;

		if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER) continue;

		role_ptr p_role = role_manager_t::find_role(p_member->get_uid());
		if (NULL == p_role) continue;

		if (NULL == td) {
			p_role->drop_circle_task(TASK_TYPE_ZHUOGUI);
			continue;
		}

		if ( !td->has_zg_task_info() ) {
			log_error(" sync zhuagui task faild, has no zg_task_info ");
			return;
		}
	
		//接取任务
		proto::common::role_change_data rcd;
		p_role->accept_sync_task(td->zg_task_info(), rcd);
	}
}

//////////////////////////////////////////////////////////////////////////
void troop_t::do_create_partner(const troop_member_ptr& p_member, uint64_t scene_uid, uint32_t index)
{
	if (!p_member) {
		log_error("p_member null error");
		return;
	}

	if (index >= TROOP_MEMBER_MAX_COUNT) {
		log_error("do create partner index[%d] error", index);
		return;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(scene_uid);
	if (NULL == p_scene) {
		log_error("do create partner scene[%lu] not find", scene_uid);
		return;
	}

	switch (p_member->get_object_type()) {
		case proto::common::SCENEOBJECT_TUTOR: {
			const proto::server::troop_extra_info& extra = p_member->get_member_extra();
			uint32_t tutor_id = extra.tutor_tid();
			TutorTable* p_conf = GET_CONF(TutorTable, tutor_id);
			if (!p_conf) {
				log_error("tutor[%u] not find", tutor_id);
				return;
			}

			uint64_t owner_uid = p_member->get_owner_uid();
			troop_member_ptr p_owner_member = get_member(owner_uid);
			if (!p_owner_member) {
				log_error("tutor[%u] owner[%lu] not in troop", tutor_id, owner_uid);
				return;
			}

			std::string tutor_name;
			StringTool::format(tutor_name, common::string_util_t::convert_to_utf8(p_conf->title()).c_str(), p_owner_member->get_name().c_str());

			partner_tutor_ptr p_tutor(new partner_tutor_t(p_member->get_uid()));
			if ( p_tutor != NULL && p_tutor->init_tutor(p_conf, tutor_name, p_owner_member->get_level()) ) {
				if (!partner_manager_t::add_partner(p_tutor)) {
					log_error("partner[%lu] add monster_manager error", p_tutor->get_object_id());
					return;
				}

				p_tutor->set_object_born();
				p_tutor->set_real_speed(m_troop_speed);
				p_tutor->set_troop_uid(m_uid);

				if (!p_scene->on_enter_s(p_tutor, m_moves[index].m_x, m_moves[index].m_y, m_moves[index].m_dir))
				{
					log_error("partner[%lu] enter scene[%lu] map[%u] error", p_tutor->get_object_id(), p_scene->get_scene_id(), p_scene->get_map_tid());
					partner_manager_t::delete_partner(p_tutor->get_object_id());
					return;
				}

				TROOP_LOG("troop[%lu] create partner[%lu] type[%u]", m_uid, p_member->get_uid(), p_member->get_object_type());
			}
			break;
		}
		case proto::common::SCENEOBJECT_USERSHADOW: {
			if (p_scene->get_scene_type() == proto::server::em_scene_type_dugeon) {
				partner_shadow_ptr p_shadow(new partner_shadow_t(p_member->get_uid()));
				if (p_shadow != NULL && p_shadow->init_shadow(p_member)) {
					if (!partner_manager_t::add_partner(p_shadow)) {
						log_error("partner[%lu] add monster_manager error", p_shadow->get_object_id());
						return;
					}

					p_shadow->set_object_born();
					p_shadow->set_real_speed(m_troop_speed);
					p_shadow->set_troop_uid(m_uid);

					if (!p_scene->on_enter_s(p_shadow, m_moves[index].m_x, m_moves[index].m_y, m_moves[index].m_dir))
					{
						log_error("partner[%lu] enter scene[%lu] map[%u] error", p_shadow->get_object_id(), p_scene->get_scene_id(), p_scene->get_map_tid());
						partner_manager_t::delete_partner(p_shadow->get_object_id());
						return;
					}

					TROOP_LOG("troop[%lu] create partner[%lu] type[%u]", m_uid, p_member->get_uid(), p_member->get_object_type());
				}
			}
			break;
		}
		default: {
			log_error("do create partner object_type[%u] error", p_member->get_object_type());
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
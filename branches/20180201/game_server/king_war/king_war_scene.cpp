#include "king_war_scene.hpp"
#include "tblh/Country.tbls.h"
#include "config_mgr.h"
#include "long_pulse.hpp"
#include "monster/monster_manager.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "tblh/errcode_enum.hpp"
#include "scene/scene_manager.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "king_war_manager.hpp"
#include "collect/collect_manager.hpp"
#include "fight/fight_manager.hpp"
#include "tblh/CollectTable.tbls.h"
#include "sys_notice/sys_notice_manager.hpp"

king_war_scene_t::king_war_scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param)
	: scene_t(scene_id, map_id, scene_type, type_param)
{
	// 初始化王城战家族
	m_family[king_war_side_attack] = 0;
	m_family[king_war_side_defence] = 0;
    m_artifacts_info.reset();
}

king_war_scene_t::~king_war_scene_t()
{

}

bool king_war_scene_t::init()
{
	// 基类
	if (!scene_t::init())
	{
		log_error("king_war_scene_t::init() error");
		return false;
	}

	switch (m_scene_type)
	{
	case proto::server::em_scene_type_king:
		{
			m_country_id = m_type_param;
		}
		break;
	default:
		{
			log_error("map[%u] scene[%lu] scene_type[%u] error", m_map_tid,  m_scene_id, m_scene_type);
			return false;
		}
		break;
	}

	if (!king_war_manager_t::add_country_scene(m_country_id, m_scene_id))
	{
		log_error("country[%u] add scene error", m_country_id);
		return false;
	}

	Country* conf = GET_CONF(Country, m_country_id);
	if (NULL == conf)
	{
		log_error("country[%u] not exist", m_country_id);
		return false;
	}

	// 初始化一些场景信息
	m_relive_side[king_war_side_attack].m_area_id = conf->att_area_id();
	m_relive_side[king_war_side_attack].m_jump_id = conf->att_jump_id();

	m_relive_side[king_war_side_defence].m_area_id = conf->def_area_id();
	m_relive_side[king_war_side_defence].m_jump_id = conf->def_jump_id();

	// 定时器初始化
	if (NULL == env::server)
	{
		log_error("env::server null error");
		return false;
	}

	m_ready_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_ready_timer)
	{
		log_error("m_ready_timer null error");
		return false;
	}

	m_refresh_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_refresh_timer)
	{
		log_error("m_refresh_timer null error");
		return false;
	}

	m_artifacts_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_artifacts_timer)
	{
		log_error("m_artifacts_timer null error");
		return false;
	}

	return true;
}

void king_war_scene_t::clear()
{
	// 场景删除
	for (uint32_t i = 0; i < king_war_side_max; ++i)
	{
		m_family_roles[i].clear();
		m_family[i] = 0;
	}

	// 场景删除怪物,基类会清理
	m_long_pulse_map.clear();

	// 定时器清除
	if (NULL != m_ready_timer)
	{
		m_ready_timer->cancel();
	}

	// 定时器清除
	if (NULL != m_refresh_timer)
	{
		m_refresh_timer->cancel();
	}

	// 定时器清除
	if (NULL != m_artifacts_timer)
	{
		m_artifacts_timer->cancel();
	}

	// 状态
	m_king_war_state = proto::common::king_war_state_end;

	m_artifacts_info.reset();

	// 基类删除所有怪物
	scene_t::clear();
}

void king_war_scene_t::role_leave_area(role_ptr p_role, uint32_t area_id)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	// 不在开战期间
	if (m_king_war_state < proto::common::king_war_state_prepare)
		return;

	// 没有战意值不处理
	if (p_role->get_king_war_warfare() == 0)
	{
		return;
	}

	// 不是参战方
	king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
	if (side_type == king_war_side_none)
	{
		return;
	}

	king_war_long_pulse_ptr p_long_area = get_king_war_long_pulse_by_area_id(area_id);
	if (p_long_area != NULL && p_long_area->m_object_id != 0)
	{
		monster_ptr p_monster = monster_manager_t::find_monster(p_long_area->m_object_id);
		if (NULL == p_monster)
		{
			log_error("can not find p_monster[%lu]", p_long_area->m_object_id);
			return;
		}

		proto::client::gc_king_war_icon_notify notify;
		notify.set_state(0);
		p_role->send_msg_to_client(op_cmd::gc_king_war_icon_notify, notify);

		//log_warn("role[%lu] leave area[%u]", p_role->get_object_id(), area_id);
	}
}

void king_war_scene_t::role_enter_area(role_ptr p_role, uint32_t area_id)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	// 不在开战期间
	if (m_king_war_state < proto::common::king_war_state_prepare)
		return;

	// 没有战意值不处理
	if (p_role->get_king_war_warfare() == 0)
	{
		return;
	}

	// 不是参战方
	king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
	if (side_type == king_war_side_none)
	{
		return;
	}

	king_war_long_pulse_ptr p_long_area = get_king_war_long_pulse_by_area_id(area_id);
	if (p_long_area != NULL && p_long_area->m_object_id != 0)
	{
		monster_ptr p_monster = monster_manager_t::find_monster(p_long_area->m_object_id);
		if (NULL == p_monster)
		{
			log_error("can not find p_monster[%lu]", p_long_area->m_object_id);
			return;
		}

		if (!p_monster->is_long_pulse())
		{
			log_error("object[%lu] is not long", p_long_area->m_object_id);
			return;
		}

		long_pulse_ptr p_long_monster = p_monster->get_long_pulse();
		if (NULL == p_long_monster)
		{
			log_error("object[%lu] is not long", p_long_area->m_object_id);
			return;
		}

		proto::client::gc_king_war_icon_notify notify;
		notify.set_state(1);
		notify.set_family_belong(common::string_util_t::uint64_to_string(p_long_monster->get_occupy_family()));
		notify.set_object_id(common::string_util_t::uint64_to_string(p_long_area->m_object_id));
		p_role->send_msg_to_client(op_cmd::gc_king_war_icon_notify, notify);

		//log_warn("role[%lu] in area[%u] object_id[%lu]", p_role->get_object_id(), area_id, p_long_area->m_object_id);
	}
}

void king_war_scene_t::on_revive_role(role_ptr p_role)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	// 开战期间
	if (m_king_war_state == proto::common::king_war_state_start)
	{
		king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
		// 参战玩家,加入列表
		if (side_type < king_war_side_max && p_role->get_king_war_warfare() != 0)
		{
			// 复活点复活
			scene_manager_t::role_jump_point(p_role, m_relive_side[side_type].m_jump_id);
		}
		else
		{
			// 基类调用
			scene_t::on_revive_role(p_role);
		}
	}
	else
	{
		// 基类调用
		scene_t::on_revive_role(p_role);
	}
}

void king_war_scene_t::update_long_info(proto::client::gc_update_long_info_notify& notify)
{
	for (king_war_long_pulse_map::iterator itr = m_long_pulse_map.begin();
		itr != m_long_pulse_map.end(); ++itr)
	{
		king_war_long_pulse_ptr p_long_area = itr->second;
		if (p_long_area != NULL && p_long_area->m_object_id != 0)
		{
			monster_ptr p_monster = monster_manager_t::find_monster(p_long_area->m_object_id);
			if (NULL == p_monster)
			{
				log_error("can not find p_monster[%lu]", p_long_area->m_object_id);
				continue;
			}

			if (!p_monster->is_long_pulse())
			{
				log_error("object[%lu] is not long", p_long_area->m_object_id);
				continue;
			}

			long_pulse_ptr p_long_monster = p_monster->get_long_pulse();
			if (NULL == p_long_monster)
			{
				log_error("object[%lu] is not long", p_long_area->m_object_id);
				continue;
			}

			proto::common::king_war_long_info* info = notify.add_infos();
			if (info != NULL)
			{
				info->set_object_id(common::string_util_t::uint64_to_string(p_long_monster->get_object_id()));
				info->set_monster_tid(p_long_monster->get_monster_id());
				info->set_curr_x((uint32_t)p_long_monster->get_current_x());
				info->set_curr_y((uint32_t)p_long_monster->get_current_y());
				info->set_curr_hp(p_long_monster->get_current_hp());
				info->set_max_hp(p_long_monster->get_max_hp());
				info->set_family_belong(common::string_util_t::uint64_to_string(p_long_monster->get_occupy_family()));
			}
		}
	}
}

void king_war_scene_t::notify_scene_long_info()
{
	// 场景中的玩家通知龙脉信息
	proto::client::gc_update_long_info_notify notify;
	if (m_king_war_state == proto::common::king_war_state_start)
	{
		update_long_info(notify);
		notify.set_state(1);
	}
	else
	{
		notify.set_state(0);
	}

	struct king_war_select : public scene_select_t
	{
		explicit king_war_select(uint64_t att_family_id, uint64_t def_family_id) : m_att_family_id(att_family_id), m_def_family_id(def_family_id) {};
		bool exec(const role_ptr& p_role) const
		{
			if (p_role->get_family_id() == m_att_family_id || p_role->get_family_id() == m_def_family_id)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		uint64_t m_att_family_id = 0;
		uint64_t m_def_family_id = 0;
	}king_war_select(m_family[king_war_side_attack], m_family[king_war_side_defence]);

	send_msg_to_scene(op_cmd::gc_update_long_info_notify, notify, &king_war_select);
}

uint32_t king_war_scene_t::is_can_collect_artifacts(role_ptr p_role)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	// 不在开战期间
	if (m_king_war_state < proto::common::king_war_state_start)
	{
		log_error("p_role[%lu] m_king_war_state[%u] error", p_role->get_object_id(), m_king_war_state);
		return common::errcode_enum::notice_king_war_error11;
	}

	// 是否是参战家族
	king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
	if (side_type == king_war_side_none)
	{
		log_error("p_role[%lu] is not king war family", p_role->get_object_id(), m_king_war_state);
		return common::errcode_enum::notice_king_war_error12;
	}

	// 战意值为0
	if (p_role->get_king_war_warfare() == 0)
	{
		log_error("p_role[%lu] king_war_warfare[%u] == 0", p_role->get_object_id(), p_role->get_king_war_warfare());
		return common::errcode_enum::notice_king_war_error14;
	}

	if (m_artifacts_info.m_artifacts_id == 0 || m_artifacts_info.m_state != em_artifacts_appear)
	{
		log_error("artifacts id[%lu] || m_artifacts_info.m_state[%u] != em_artifacts_appear error", m_artifacts_info.m_artifacts_id, m_artifacts_info.m_state);
		return common::errcode_enum::notice_unknown;
	}

	return common::errcode_enum::error_ok;
}

void king_war_scene_t::role_get_artifacts(role_ptr p_role, uint32_t tid)
{
	// 删除神器
	if (m_artifacts_info.m_artifacts_id != 0 && m_artifacts_info.m_state == em_artifacts_appear)
	{
		collect_point_ptr p_collect_point = collect_manager_t::find_collect_point(m_artifacts_info.m_artifacts_id);
		if (NULL != p_collect_point)
		{
			p_collect_point->set_object_die();

			p_collect_point->leave_scene();

			collect_manager_t::delete_collect_point(m_artifacts_info.m_artifacts_id);
		}

		// id清0
		m_artifacts_info.m_artifacts_id = 0;
		m_artifacts_info.m_role_artifacts = p_role->get_object_id();
		m_artifacts_info.m_state = em_artifacts_collected;

		// 清理到时间自动消失的定时器
		if (NULL != m_artifacts_timer)
		{
			m_artifacts_timer->cancel();
		}

		// 通知客户端选择神器
		proto::client::gc_artifact_choice_notify notify;
		p_role->send_msg_to_client(op_cmd::gc_artifact_choice_notify, notify);

		KINGWAR_LOG("role[%lu] get artifacts[%lu:%u]", p_role->get_object_id(), m_artifacts_info.m_artifacts_id, m_artifacts_info.m_artifacts_tid);
	}
}

bool king_war_scene_t::role_user_artifacts(role_ptr p_role, uint32_t index)
{
	if (m_artifacts_info.m_state != em_artifacts_collected)
	{
		log_error("role[%lu] m_artifacts_info.m_state[%u] != em_artifacts_collected", p_role->get_object_id(), m_artifacts_info.m_state);
		return false;
	}

	if (m_artifacts_info.m_role_artifacts != p_role->get_object_id())
	{
		log_error("role[%lu] not artifacts_role[%lu]", p_role->get_object_id(), m_artifacts_info.m_role_artifacts);
		return false;
	}

	king_war_long_pulse_ptr p_long_area = NULL;
	for (king_war_long_pulse_map::iterator itr = m_long_pulse_map.begin();
		itr != m_long_pulse_map.end(); ++itr)
	{
		if (itr->second != NULL && itr->second->m_object_id != 0 && itr->second->m_long_type == index)
		{
			p_long_area = itr->second;
			break;
		}
	}

	if (p_long_area == NULL)
	{
		log_error("role[%lu] not find this index[%u]", p_role->get_object_id(), index);
		return false;
	}

	m_artifacts_info.m_area_id = p_long_area->m_area_id;
	m_artifacts_info.m_state = em_artifacts_start;

	// 10 秒后发动神器
	m_artifacts_timer->expires_from_now(boost::posix_time::seconds(10));
	m_artifacts_timer->async_wait(boost::bind(&king_war_scene_t::on_artifacts_call_back, this, boost::asio::placeholders::error));

	// 公告
	BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_23, 0, 0, m_country_id, p_role->get_family_name().c_str(), p_role->get_name().c_str());

	// 通知本场景参战家族10秒后发动神器
	struct king_war_select : public scene_select_t
	{
		explicit king_war_select(uint64_t att_family_id, uint64_t def_family_id) : m_att_family_id(att_family_id), m_def_family_id(def_family_id) {};
		bool exec(const role_ptr& p_role) const
		{
			if (p_role != NULL && (p_role->get_family_id() == m_att_family_id || p_role->get_family_id() == m_def_family_id))
			{
				return true;
			}

			return false;
		}

		uint64_t m_att_family_id = 0;
		uint64_t m_def_family_id = 0;
	}king_war_select(m_family[king_war_side_attack], m_family[king_war_side_defence]);

	proto::client::gc_artifact_left_time_notify notify;
	notify.set_time_left(10);
	send_msg_to_scene(op_cmd::gc_artifact_left_time_notify, notify, &king_war_select);

	KINGWAR_LOG("role[%lu] use artifacts[%lu:%u]", p_role->get_object_id(), m_artifacts_info.m_artifacts_id, m_artifacts_info.m_artifacts_tid);

	return true;
}

void king_war_scene_t::on_role_kill_other(role_ptr p_role, object_position_ptr p_enemy)
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

	if (p_role->is_in_king_war() && p_enemy->is_role())
	{
		king_war_manager_t::role_kill_other(p_role, p_enemy->get_role());
	}
}

void king_war_scene_t::on_other_kill_me(role_ptr p_role, object_position_ptr p_enemy)
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

	// 不在进行王城战（包括不是参战家族和筋疲力尽的玩家）,或者不是被人击杀的
	if (!p_role->is_in_king_war() || !p_enemy->is_role())
	{
		scene_t::on_other_kill_me(p_role, p_enemy);
		return;
	}
	
	p_role->set_object_die(p_enemy->get_object_id());

	king_war_manager_t::role_kill_other(p_role, p_enemy->get_role());

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_revive_time);
	uint32_t revive_time = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_role->set_revive_time(revive_time);

	show_revive_ui(p_role);
}

void king_war_scene_t::show_revive_ui(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("role null error");
		return;
	}

	// 不在进行王城战（包括不是参战家族和筋疲力尽的玩家）
	if (!p_role->is_in_king_war())
	{
		scene_t::show_revive_ui(p_role);
		return;
	}

	uint32_t time_now = common::time_util_t::now_time();

	proto::client::gc_object_death_notify notify;
	notify.set_revive_type(proto::common::object_revive_king_war);

	if (p_role->get_revive_time() > time_now)
	{
		notify.set_revive_time(p_role->get_revive_time() - time_now);
	}
	else
	{
		notify.set_revive_time(0);
	}

	king_war_role_ptr p_king_war_role = king_war_manager_t::get_king_war_role(p_role->get_country_id(), p_role->get_object_id());
	if (p_king_war_role != NULL)
	{
		notify.set_death_count(p_king_war_role->m_death_count);
	}

	p_role->send_msg_to_client(op_cmd::gc_object_death_notify, notify);
}

void king_war_scene_t::on_login_die(role_ptr p_role)
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

	// 不在进行王城战（包括不是参战家族和筋疲力尽的玩家）
	if (!p_role->is_in_king_war())
	{
		scene_t::on_login_die(p_role);
		return;
	}

	// 弹出复活界面
	show_revive_ui(p_role);
}

void king_war_scene_t::ready_king_war()
{
	m_king_war_state = proto::common::king_war_state_prepare;

	// 开战清理下参战玩家
	for (uint32_t i = 0; i < king_war_side_max; ++i)
	{
		m_family_roles[i].clear();
	}

	// 场景删除龙脉
	for (king_war_long_pulse_map::iterator itr = m_long_pulse_map.begin();
		itr != m_long_pulse_map.end(); ++itr)
	{
		king_war_long_pulse_ptr p_long_area = itr->second;
		if (p_long_area != NULL && p_long_area->m_object_id != 0)
		{
			on_leave_s(object_id_type(p_long_area->m_object_id, proto::common::SCENEOBJECT_MONSTER));

			monster_manager_t::delete_monster(p_long_area->m_object_id);
		}
	}

	m_long_pulse_map.clear();

	// 先统计本场景中两方玩家
	for (std::set<uint64_t>::iterator itr = m_roles.begin();
		itr != m_roles.end(); ++itr)
	{
		role_ptr p_role = role_manager_t::find_role(*itr);
		if (NULL != p_role)
		{
			king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
			// 参战玩家
			if (side_type < king_war_side_max)
			{
				m_family_roles[side_type].insert(*itr);
			}
		}
	}

	KINGWAR_LOG("scene[%lu] country[%u] ready king war", m_scene_id, m_country_id);
}

void king_war_scene_t::start_king_war()
{
	m_king_war_state = proto::common::king_war_state_start;

	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return;
	}

	// 召唤怪物
	const king_war_define_vec& long_pulse_vec = p_map->get_map_king_war();
	for (king_war_define_vec::const_iterator itr = long_pulse_vec.begin();
		itr != long_pulse_vec.end(); ++itr)
	{
		const monster_common::king_war_monster& define = *itr;

		if (define.king_war_type >= king_war_obj_type_max)
		{
			log_error("king_war_type[%u] type error", define.king_war_type);
			continue;
		}

		Monster* monster = GET_CONF(Monster, define.id);
		if (NULL == monster)
		{
			log_error("scene::load_map_object not found monster [%u]", define.id);
			continue;
		}

		long_pulse_ptr p_long_pulse = long_pulse_ptr(new long_pulse_t());
		if (p_long_pulse == NULL)
		{
			log_error("long_pulse[%u] null error", define.id);
			continue;
		}

		if (!p_long_pulse->init_long_pulse(define, monster))
		{
			log_error("long_pulse[%u] init_long_pulse error", define.id);
			continue;
		}

		if (init_object(p_long_pulse, define))
		{
			// 设置这个国家id
			p_long_pulse->set_country_id(m_country_id);
			// 设置参战家族
			p_long_pulse->set_king_war_family(king_war_side_attack, m_family[king_war_side_attack]);
			p_long_pulse->set_king_war_family(king_war_side_defence, m_family[king_war_side_defence]);

			king_war_long_pulse_ptr p_long_area = king_war_long_pulse_ptr(new king_war_long_pulse());
			if (p_long_area != NULL)
			{
				p_long_area->m_object_id = p_long_pulse->get_object_id();
				p_long_area->m_area_id = define.area_id;
				p_long_area->m_long_type = (king_war_obj_type)define.king_war_type;

				m_long_pulse_map.insert(std::make_pair(define.area_id, p_long_area));
			}
		}
	}

	// 通知全场景3秒后拉人
	proto::client::gc_king_war_scene_start_notify notify;
	notify.set_def_family_id(common::string_util_t::uint64_to_string(m_family[king_war_side_defence]));
	notify.set_att_family_id(common::string_util_t::uint64_to_string(m_family[king_war_side_attack]));
	send_msg_to_scene(op_cmd::gc_king_war_scene_start_notify, notify);

	// 定时器，开战拉人
	if (m_ready_timer != NULL)
	{
		m_ready_timer->expires_from_now(boost::posix_time::seconds(3));
		m_ready_timer->async_wait(boost::bind(&king_war_scene_t::on_ready_timer_call_back, this, boost::asio::placeholders::error));
	}

	// 全场景通知龙脉信息
	notify_scene_long_info();

	// 定时器，刷新场景龙脉信息
	if (m_refresh_timer != NULL)
	{
		m_refresh_timer->expires_from_now(boost::posix_time::seconds(5));
		m_refresh_timer->async_wait(boost::bind(&king_war_scene_t::on_refresh_info_call_back, this, boost::asio::placeholders::error));
	}

	// 定时器，4分钟后刷新场景神器出世信息
	m_artifacts_info.reset();
	if (m_artifacts_timer != NULL)
	{
		m_artifacts_timer->expires_from_now(boost::posix_time::seconds(4 * 60));
		m_artifacts_timer->async_wait(boost::bind(&king_war_scene_t::on_artifacts_call_back, this, boost::asio::placeholders::error));
		m_artifacts_info.m_state = em_artifacts_ready;
	}

	// 设置场景中的玩家的pk模式
	for (std::set<uint64_t>::iterator itr = m_roles.begin();
		itr != m_roles.end(); ++itr)
	{
		role_ptr p_role = role_manager_t::find_role(*itr);
		if (NULL != p_role)
		{
			king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
			// 参战玩家
			if (side_type < king_war_side_max)
			{
				proto::client::gc_role_battle_value_notify battle_value_notify;
				king_war_role_ptr p_king_war_role = king_war_manager_t::get_king_war_role(m_country_id, p_role->get_object_id());
				if (p_king_war_role != NULL)
				{
					p_role->set_king_war_warfare(p_king_war_role->m_warfare);
					battle_value_notify.set_value(p_king_war_role->m_warfare);
				}
				else
				{
					p_role->set_king_war_warfare(WARFARE_DEFAULT);
					battle_value_notify.set_value(WARFARE_DEFAULT);
				}
				battle_value_notify.set_state(1);
				p_role->send_msg_to_client(op_cmd::gc_role_battle_value_notify, battle_value_notify);

				// 没有战意值不能pk
				if (p_role->get_king_war_warfare() == 0)
				{
					scene_change_pk_mode(p_role, proto::common::PK_PEACE);
				}
				else
				{
					scene_change_pk_mode(p_role, proto::common::PK_FAMILY);
				}
			}
		}
	}

	KINGWAR_LOG("scene[%lu] country[%u] start king war", m_scene_id, m_country_id);
}

void king_war_scene_t::end_king_war()
{
	m_king_war_state = proto::common::king_war_state_end;

	// 战斗结束清理
	for (uint32_t i = 0; i < king_war_side_max; ++i)
	{
		m_family_roles[i].clear();
		m_family[i] = 0;
	}


	for (king_war_long_pulse_map::iterator itr = m_long_pulse_map.begin();
		itr != m_long_pulse_map.end(); ++itr)
	{
		king_war_long_pulse_ptr p_long_area = itr->second;
		if (p_long_area != NULL && p_long_area->m_object_id != 0)
		{
			on_leave_s(object_id_type(p_long_area->m_object_id, proto::common::SCENEOBJECT_MONSTER));

			monster_manager_t::delete_monster(p_long_area->m_object_id);
		}
	}

	m_long_pulse_map.clear();

	// 全场景通知龙脉信息，放到结束的消息
	//notify_scene_long_info();

	// 删除神器
	if (m_artifacts_info.m_artifacts_id != 0)
	{
		collect_point_ptr p_collect_point = collect_manager_t::find_collect_point(m_artifacts_info.m_artifacts_id);
		if (NULL != p_collect_point)
		{
			p_collect_point->set_object_die();
			// 消失
			p_collect_point->leave_scene();

			collect_manager_t::delete_collect_point(m_artifacts_info.m_artifacts_id);
		}
	}

	m_artifacts_info.reset();

	// 结束时玩家的一些处理
	for (std::set<uint64_t>::iterator itr = m_roles.begin();
		itr != m_roles.end(); ++itr)
	{
		role_ptr p_role = role_manager_t::find_role(*itr);
		if (NULL != p_role)
		{
			king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
			// 参战玩家
			if (side_type < king_war_side_max)
			{
				// 结束时候还死亡直接复活
				if (p_role->is_die())
				{
					p_role->set_object_revive(0);
				}

				// 改变一下pk模式
				scene_change_pk_mode(p_role, proto::common::PK_PEACE);

				// 设置玩家状态
				p_role->set_king_war_warfare(0);

				// 当时正在攻击状态的改变状态
				if (p_role->get_object_state() == proto::common::object_state_attack)
				{
					p_role->change_object_state(proto::common::object_state_normal);
				}
			}
		}
	}

	// 定时器清除
	if (NULL != m_ready_timer)
	{
		m_ready_timer->cancel();
	}

	if (NULL != m_refresh_timer)
	{
		m_refresh_timer->cancel();
	}

	if (NULL != m_artifacts_timer)
	{
		m_artifacts_timer->cancel();
	}

	// 全场景通知战意志,放到结束的消息
	//proto::client::gc_role_battle_value_notify battle_value_notify;
	//battle_value_notify.set_state(0);
	//send_msg_to_scene(op_cmd::gc_role_battle_value_notify, battle_value_notify);

	KINGWAR_LOG("scene[%lu] country[%u] ending king war", m_scene_id, m_country_id);
}

void king_war_scene_t::set_king_war_family(king_war_side_type type, uint64_t family_id)
{
	if (type >= king_war_side_max)
	{
		log_error("family type[%u] error", type);
		return;
	}

	m_family[type] = family_id;
}

uint32_t king_war_scene_t::get_scene_family_num(king_war_side_type type)
{
	if (type < king_war_side_max)
	{
		return m_family_roles[type].size();
	}

	log_error("get_scene_family_num type[%u] error", type);
	return 0;
}

uint32_t king_war_scene_t::king_war_scene_process(role_ptr p_role)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return errcode_enum::notice_unknown;
	}

	king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());

	// 已经在本场景了
	if (p_role->get_scene_id() == m_scene_id)
	{
		// 参战玩家
		if (side_type < king_war_side_max)
		{
			// 不在自己方的传送点
			if (p_role->get_role_area_id() != m_relive_side[side_type].m_area_id)
			{
				scene_manager_t::role_jump_point(p_role, m_relive_side[side_type].m_jump_id);
			}
			else
			{
				//KINGWAR_LOG("p_role[%lu] is already in area", p_role->get_object_id());
				return errcode_enum::notice_king_war_error13;
			}
		}
		else // 不是参战玩家
		{
			// 在复活点周围，拉走
			for (uint32_t i = 0; i < king_war_side_max; ++i)
			{
				if (p_role->get_role_area_id() == m_relive_side[i].m_area_id)
				{
					scene_manager_t::role_jump_point(p_role, get_revive_point());

					break;
				}
			}
		}
	}
	else
	{
		// 参战玩家
		if (side_type < king_war_side_max)
		{
			return scene_manager_t::role_jump_point(p_role, m_relive_side[side_type].m_jump_id);
		}
		else
		{
			return errcode_enum::notice_unknown;
		}
	}

	return errcode_enum::error_ok;
}

king_war_side_type king_war_scene_t::get_king_war_side_type(uint64_t family_id)
{
	if (family_id == m_family[king_war_side_attack])
	{
		return king_war_side_attack;
	}
	else if (family_id == m_family[king_war_side_defence])
	{
		return king_war_side_defence;
	}

	return king_war_side_none;
}

void king_war_scene_t::on_role_enter(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	// 不在参战期间
	if (m_king_war_state < proto::common::king_war_state_prepare)
	{
		scene_change_pk_mode(p_role, proto::common::PK_PEACE);
	}
	else
	{
		king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
		// 参战玩家,加入列表
		if (side_type < king_war_side_max)
		{
			m_family_roles[side_type].insert(p_role->get_object_id());

			if (m_king_war_state == proto::common::king_war_state_start)
			{
				// 开战期间设置人的状态
				proto::client::gc_role_battle_value_notify battle_value_notify;
				king_war_role_ptr p_king_war_role = king_war_manager_t::get_king_war_role(m_country_id, p_role->get_object_id());
				if (p_king_war_role != NULL)
				{
					p_role->set_king_war_warfare(p_king_war_role->m_warfare);
					battle_value_notify.set_value(p_king_war_role->m_warfare);
				}
				else
				{
					p_role->set_king_war_warfare(WARFARE_DEFAULT);
					battle_value_notify.set_value(WARFARE_DEFAULT);
				}
				battle_value_notify.set_state(1);
				p_role->send_msg_to_client(op_cmd::gc_role_battle_value_notify, battle_value_notify);

				// 没有战意值不能pk
				if (p_role->get_king_war_warfare() == 0)
				{
					scene_change_pk_mode(p_role, proto::common::PK_PEACE);
				}
				else
				{
					scene_change_pk_mode(p_role, proto::common::PK_FAMILY);
				}


				// 场景中的玩家通知龙脉信息
				proto::client::gc_update_long_info_notify notify;
				update_long_info(notify);
				notify.set_state(1);
				p_role->send_msg_to_client(op_cmd::gc_update_long_info_notify, notify);

				// 不在自己方的传送点,传送到复活点
				if (p_role->get_role_area_id() != m_relive_side[side_type].m_area_id)
				{
					scene_manager_t::role_jump_point(p_role, m_relive_side[side_type].m_jump_id);
				}
			}
			else
			{
				scene_change_pk_mode(p_role, proto::common::PK_PEACE);
			}
		}
		else
		{
			scene_change_pk_mode(p_role, proto::common::PK_PEACE);
		}
	}
}

void king_war_scene_t::on_role_leave(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	// 不在开战期间
	if (m_king_war_state < proto::common::king_war_state_prepare)
		return;

	king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
	// 参战玩家,加入列表
	if (side_type < king_war_side_max)
	{
		m_family_roles[side_type].erase(p_role->get_object_id());
	}

	if (m_king_war_state == proto::common::king_war_state_start)
	{
		// 开战期间离开设置状态
		p_role->set_king_war_warfare(0);
	}
}

void king_war_scene_t::on_ready_timer_call_back(const boost::system::error_code& ec)
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

	// 时间到了拉人
    for (std::set<uint64_t>::iterator itr = m_roles.begin();
    itr != m_roles.end(); ++itr)
    {
        role_ptr p_role = role_manager_t::find_role(*itr);
        if (NULL != p_role)
        {
            king_war_scene_process(p_role);
        }
    }
}

void king_war_scene_t::on_refresh_info_call_back(const boost::system::error_code& ec)
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

	if (m_refresh_timer != NULL)
	{
		// 全场景通知龙脉信息
		notify_scene_long_info();

		// 定时器，刷新场景龙脉信息
		m_refresh_timer->expires_from_now(boost::posix_time::seconds(5));
		m_refresh_timer->async_wait(boost::bind(&king_war_scene_t::on_refresh_info_call_back, this, boost::asio::placeholders::error));
	}
}

void king_war_scene_t::on_artifacts_call_back(const boost::system::error_code& ec)
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

	if (m_artifacts_timer == NULL)
	{
		log_error("m_artifacts_timer null error!");
		return;
	}

	switch (m_artifacts_info.m_state)
	{
	case em_artifacts_ready:
		{
			m_artifacts_info.m_state = em_artifacts_pre;
			// 1分钟后出现神器
			m_artifacts_timer->expires_from_now(boost::posix_time::seconds(60));
			m_artifacts_timer->async_wait(boost::bind(&king_war_scene_t::on_artifacts_call_back, this, boost::asio::placeholders::error));

			// 公告
			BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_22, 0, 0, m_country_id);
		}
		break;
	case em_artifacts_pre:
		{
			map_ptr p_map = get_map_ptr();
			if (NULL == p_map)
			{
				log_error("map[%u] null error", m_map_tid);
				m_artifacts_info.m_state = em_artifacts_none;
				return;
			}

			const collect_define_vec& collect_vec = p_map->get_map_collect_points();
			int32_t index = random_util_t::randBetween(0, (int32_t)collect_vec.size() - 1);
			if (index == -1)
			{
				log_error("rand from 0 to [%d] error", (int32_t)collect_vec.size() - 1);
				return;
			}

			const t_collect_define& define = collect_vec.at(index);

			CollectTable* p_collect_config = GET_CONF(CollectTable, define.m_collect_id);
			if (NULL == p_collect_config)
			{
				log_error("scene::load_map_object not found collect [%d]", define.m_collect_id);
				m_artifacts_info.m_state = em_artifacts_none;
				return;
			}

			collect_point_ptr p_collect_point = collect_point_ptr(new collect_point_t(define.m_collect_id));
			if (p_collect_point != NULL && p_collect_point->init_collect_point(define.m_x, define.m_y))
			{
				init_collect_point(p_collect_point);
			}

			if (p_collect_config->type() == proto::common::collect_type_artifacts)
			{
				// 只有一个神器
				m_artifacts_info.m_artifacts_tid = define.m_collect_id;
				m_artifacts_info.m_artifacts_id = p_collect_point->get_object_id();
			}

			if (m_artifacts_info.m_artifacts_id == 0)
			{
				log_error("have no one artifacts");
				m_artifacts_info.m_state = em_artifacts_none;
				return;
			}

			// 5分钟后如果没给采集，就刷新掉
			m_artifacts_timer->expires_from_now(boost::posix_time::seconds(5 * 60));
			m_artifacts_timer->async_wait(boost::bind(&king_war_scene_t::on_artifacts_call_back, this, boost::asio::placeholders::error));

			m_artifacts_info.m_state = em_artifacts_appear;
			m_artifacts_info.m_role_artifacts = 0;

			// 定时器，刷新场景神器信息
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_weapon_refresh_upper);
			uint32_t max_refresh = GET_COMPREHENSIVE_VALUE_1(p_conf);

			p_conf = GET_CONF(Comprehensive, comprehensive_common::king_weapon_refresh_lower);
			uint32_t min_refresh = GET_COMPREHENSIVE_VALUE_1(p_conf);

			// 下一个神器刷新时间 = 这个神器的持续时间 + 间隔时间（随机）
			int32_t refresh_time = random_util_t::randBetween((int32_t)min_refresh, (int32_t)max_refresh);
			if (refresh_time == -1)
			{
				log_error("rand from [%d] to [%d] error", (int32_t)min_refresh, (int32_t)max_refresh);
				return;
			}

			// 5分钟
			refresh_time += 5;

			// 下个神器的刷新前一分钟
			if (refresh_time > 1)
			{
				// 下一个神器的刷新时间
				m_artifacts_info.m_next_time = common::time_util_t::now_time() + (refresh_time - 1) * 60;
			}
		}
		break;
	case em_artifacts_appear:
		{
			// 删除神器
			if (m_artifacts_info.m_artifacts_id != 0)
			{
				collect_point_ptr p_collect_point = collect_manager_t::find_collect_point(m_artifacts_info.m_artifacts_id);
				if (NULL != p_collect_point)
				{
					p_collect_point->set_object_die();
					// 消失
					p_collect_point->leave_scene();

					collect_manager_t::delete_collect_point(m_artifacts_info.m_artifacts_id);
				}
			}

			uint32_t time_now = common::time_util_t::now_time();

			if (m_artifacts_info.m_next_time <= time_now)
			{
				log_error("scene[%lu] kingwar get next artifacts refresh time error", m_scene_id);
				return;
			}

			uint32_t interval_time = m_artifacts_info.m_next_time - time_now;

			m_artifacts_info.reset();
			m_artifacts_info.m_state = em_artifacts_ready;

			m_artifacts_timer->expires_from_now(boost::posix_time::seconds(interval_time));
			m_artifacts_timer->async_wait(boost::bind(&king_war_scene_t::on_artifacts_call_back, this, boost::asio::placeholders::error));
		}
		break;
	case em_artifacts_start:
		{
			// 10 秒到了发动神器
			// 击杀区域内的人
			// 先统计本场景中两方玩家
			do 
			{
				king_war_long_pulse_ptr p_long_pulse = get_king_war_long_pulse_by_area_id(m_artifacts_info.m_area_id);
				if (p_long_pulse == NULL)
				{
					log_error("scene[%lu] area_id[%u] error", m_scene_id, m_artifacts_info.m_area_id);
					break;
				}

				std::set<uint64_t> fights;
				for (std::set<uint64_t>::iterator itr = m_roles.begin();
					itr != m_roles.end(); ++itr)
				{
					role_ptr p_role = role_manager_t::find_role(*itr);
					if (NULL != p_role && p_role->get_role_area_id() == m_artifacts_info.m_area_id)
					{
						if (p_role->get_fight_uid() != 0)
						{
							fights.insert(p_role->get_fight_uid());
						}
					}
				}

				// 击杀场景内玩家
				for (std::set<uint64_t>::const_iterator citr = fights.begin();
					citr != fights.end(); ++citr)
				{
					// 击杀场景内玩家
					combat_ptr p_combat = fight_manager_t::get_combat(*citr);
					if (p_combat != NULL)
					{
						p_combat->all_fail();
					}
				}

				monster_ptr p_monster = monster_manager_t::find_monster(p_long_pulse->m_object_id);
				if (NULL == p_monster)
				{
					log_error("scene[%lu] can not find p_monster[%lu]", m_scene_id, p_long_pulse->m_object_id);
					break;
				}

				if (!p_monster->is_long_pulse())
				{
					log_error("scene[%lu] object[%lu] is not long", m_scene_id, p_long_pulse->m_object_id);
					break;
				}

				long_pulse_ptr p_long_monster = p_monster->get_long_pulse();
				if (NULL == p_long_monster)
				{
					log_error("scene[%lu] object[%lu] is not long", m_scene_id, p_long_pulse->m_object_id);
					break;
				}

				if (p_long_monster->is_die())
				{
					KINGWAR_LOG("scene[%lu] object[%lu] is already die", m_scene_id, p_long_pulse->m_object_id);
				}
				else
				{
					p_long_monster->set_object_die(0);
				}

			} while (0);

			uint32_t time_now = common::time_util_t::now_time();

			if (m_artifacts_info.m_next_time <= time_now)
			{
				log_error("scene[%lu] kingwar get next artifacts refresh time error", m_scene_id);
				return;
			}

			uint32_t interval_time = m_artifacts_info.m_next_time - time_now;

			m_artifacts_info.reset();

			m_artifacts_info.m_state = em_artifacts_ready;

			m_artifacts_timer->expires_from_now(boost::posix_time::seconds(interval_time));
			m_artifacts_timer->async_wait(boost::bind(&king_war_scene_t::on_artifacts_call_back, this, boost::asio::placeholders::error));
		}
		break;
	default:
		{
			log_error("scene[%lu] m_artifacts_info.m_state[%u] error", m_artifacts_info.m_state);
		}
		break;
	}
}

king_war_scene_t::king_war_long_pulse_ptr king_war_scene_t::get_king_war_long_pulse_by_area_id(uint32_t area_id)
{
	king_war_long_pulse_map::iterator itr = m_long_pulse_map.find(area_id);
	if (itr != m_long_pulse_map.end())
	{
		return itr->second;
	}

	return NULL;
}


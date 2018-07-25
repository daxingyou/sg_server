#include "long_pulse.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "role/role_manager.hpp"
#include "fight/fight_manager.hpp"
#include "king_war_manager.hpp"
#include "king_war_scene.hpp"
#include "country/country_manager.hpp"

long_pulse_t::~long_pulse_t()
{

}

void long_pulse_t::del_clear()
{
	monster_t::del_clear();

	// 定时器清除
	if (NULL != m_revive_timer)
	{
		m_revive_timer->cancel();
	}

	if (NULL != m_occupied_timer)
	{
		m_occupied_timer->cancel();
	}
}

void long_pulse_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (NULL == object_info)
	{
		log_error("object_info null error");
		return;
	}

	monster_t::get_object_info(object_info);

	object_info->set_current_hp(m_curr_hp);
	object_info->set_max_hp(m_max_hp);
	object_info->set_family_id(string_util_t::uint64_to_string(m_occupy_family));
	if (m_country_id != 0) {
		country_manager_t::peek_country_common(m_country_id, object_info->mutable_country());
	}
}

void long_pulse_t::other_object_into_vision(const object_id_type& object)
{
	if (object.second == proto::common::SCENEOBJECT_USER)
	{
		m_vision_objects.insert(object);
	}
}

void long_pulse_t::other_object_out_of_vision(const object_id_type& object)
{
	if (object.second == proto::common::SCENEOBJECT_USER)
	{
		m_vision_objects.erase(object);
	}
}

bool long_pulse_t::init_long_pulse(const monster_common::king_war_monster& define, Monster* monster)
{
	if (NULL == monster)
	{
		log_error("npc null error");
		return false;
	}

	// 基类
	if (!monster_t::init_monster(define, monster))
	{
		log_error("long_pulse[%u] null error", define.id);
		return false;
	}

	if (define.king_war_type >= king_war_obj_type_max)
	{
		log_error("king_war_type type error");
		return false;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_dragon_attack_interval);
	m_round_time = GET_COMPREHENSIVE_VALUE_1(p_conf);

	m_obj_type = (king_war_obj_type)define.king_war_type;

	m_max_hp = define.hp;
	m_curr_hp = m_max_hp;

	m_area_id = define.area_id;

	m_war_family[king_war_side_attack] = 0;
	m_war_family[king_war_side_defence] = 0;

	m_occupy_family = 0;

	m_defence_list.clear();
	m_attack_list.clear();

	m_family_hurt[king_war_side_attack] = 0;
	m_family_hurt[king_war_side_defence] = 0;

	// 定时器初始化
	if (NULL == env::server)
	{
		log_error("env::server null error");
		return false;
	}

	m_revive_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_revive_timer)
	{
		log_error("m_revive_timer null error");
		return false;
	}

	m_occupied_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_occupied_timer)
	{
		log_error("m_occupied_timer null error");
		return false;
	}

	return true;
}

void long_pulse_t::set_king_war_family(king_war_side_type type, uint64_t family_id)
{
	if (type >= king_war_side_max)
	{
		log_error("family type[%u] error", type);
		return;
	}

	m_war_family[type] = family_id;
}

uint32_t long_pulse_t::ask_add_defence_list(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return errcode_enum::notice_unknown;
	}

	// 死亡提醒
	if (is_die())
	{
		log_warn("object[%lu] is dead", get_object_id());
		return errcode_enum::sys_notice_object_dead;
	}

	// 不在怪物区域
	if (p_role->get_scene_id() != m_scene_id ||  p_role->get_role_area_id() != m_area_id)
	{
		log_error("role[%lu] area[%u] != [%u]", p_role->get_object_id(), p_role->get_role_area_id(), m_area_id);
		return errcode_enum::notice_unknown;
	}

	// 不是占领家族
	if (m_occupy_family != p_role->get_family_id())
	{
		log_error("role[%lu] not war defence family", p_role->get_object_id());
		return errcode_enum::notice_unknown;
	}

	m_defence_list.insert(p_role->get_object_id());

	return errcode_enum::error_ok;
}

uint32_t long_pulse_t::ask_add_attack_list(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return errcode_enum::notice_unknown;
	}

	// 死亡提醒
	if (is_die())
	{
		log_warn("object[%lu] is dead", get_object_id());
		return errcode_enum::sys_notice_object_dead;
	}

	// 不在怪物区域
	if (p_role->get_scene_id() != m_scene_id || p_role->get_role_area_id() != m_area_id)
	{
		log_error("role[%lu] area[%u] != [%u]", p_role->get_object_id(), p_role->get_role_area_id(), m_area_id);
		return errcode_enum::notice_unknown;
	}

	// 不是参战玩家
	if (p_role->get_family_id() != m_war_family[king_war_side_attack] && p_role->get_family_id() != m_war_family[king_war_side_defence])
	{
		log_error("role[%lu] not war family", p_role->get_object_id());
		return errcode_enum::notice_unknown;
	}

	// 是占领家族
	if (m_occupy_family == p_role->get_family_id())
	{
		log_error("role[%lu] is war defence family", p_role->get_object_id());
		return errcode_enum::notice_unknown;
	}

	if (p_role->change_object_state(proto::common::object_state_attack, m_object_id) != common::errcode_enum::error_ok)
	{
		log_error("role[%lu] can not change state to [role_sys_state_attack]", p_role->get_object_id());
		return errcode_enum::notice_king_war_error_state;
	}

	uint32_t time_now = common::time_util_t::now_time();
	m_attack_list[p_role->get_object_id()] = time_now;

	// log_warn("role[%lu] add attack time[%u]", p_role->get_object_id(), time_now);
	
	return errcode_enum::error_ok;
}

void long_pulse_t::ask_end_attack(uint64_t role_uid)
{
	std::map<uint64_t, uint32_t>::iterator itr = m_attack_list.find(role_uid);
	if (itr != m_attack_list.end())
	{
		itr->second = 0;

		//log_warn("role[%lu] end attack", role_uid);
	}
}

void long_pulse_t::ask_hurt_long(role_ptr p_role, proto::client::gc_king_war_attack_end_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 死亡提醒
	if (is_die())
	{
		log_warn("object[%lu] is dead", get_object_id());
		reply.set_reply_code(errcode_enum::sys_notice_object_dead);
		return;
	}

	// 不在怪物区域
	if (p_role->get_scene_id() != m_scene_id || p_role->get_role_area_id() != m_area_id)
	{
		log_error("role[%lu] area[%u] != [%u]", p_role->get_object_id(), p_role->get_role_area_id(), m_area_id);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 是否在攻击列表中
	std::map<uint64_t, uint32_t>::iterator att_atr = m_attack_list.find(p_role->get_object_id());
	if (att_atr == m_attack_list.end())
	{
		log_error("role[%lu] not in attack list", p_role->get_object_id());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint32_t time_now = common::time_util_t::now_time();

	if (att_atr->second + m_round_time > time_now)
	{
		log_error("role[%lu] not time on", p_role->get_object_id());
		reply.set_reply_code(errcode_enum::notice_king_war_time_limit);
		reply.set_left_time(att_atr->second + m_round_time - time_now);
		return;
	}

	bool is_in_fight = false;

	// 防守队列找人
	for (std::set<uint64_t>::iterator itr = m_defence_list.begin();
		itr != m_defence_list.end();)
	{
		// 角色已经删除
		role_ptr p_def_role = role_manager_t::find_role(*itr);
		if (p_def_role == NULL)
		{
			m_defence_list.erase(itr++);
			continue;
		}

		// 防守队员不在本场景或场景区域
		if (p_def_role->get_scene_id() != m_scene_id || p_def_role->get_role_area_id() != m_area_id)
		{
			m_defence_list.erase(itr++);
			continue;
		}

		// 角色是否已经死亡
		if (p_def_role->is_die())
		{
			m_defence_list.erase(itr++);
			continue;
		}

		// pk模式必须是家族
		if (p_def_role->get_pk_mode() != proto::common::PK_FAMILY)
		{
			m_defence_list.erase(itr++);
			continue;
		}

		// 没战意不能战斗
		if (p_def_role->get_king_war_warfare() == 0)
		{
			m_defence_list.erase(itr++);
			continue;
		}

		// 防守队员能不能进战斗
		if (!p_def_role->can_enter_fight())
		{
			m_defence_list.erase(itr++);
			continue;
		}

		// 到此防守队友没问题，找到了防守队员

		// 把他们拉进战斗
		proto::common::fight_param param;
		param.set_type(proto::common::fight_type_pk);
		if (fight_manager_t::fight_pvp(p_role, p_def_role, &param) != 0)
		{
			// 进入战斗成功
			m_defence_list.erase(itr++);

			is_in_fight = true;
		}
		else
		{
			// 这个错误要注意，是什么原因没进入战斗
			log_error("----------------role[%lu] def_role[%u] enter fight error-------------------", p_role->get_object_id(), p_def_role->get_object_id());
		}

		break;
	}

	reply.set_reply_code(errcode_enum::error_ok);
	reply.set_is_fight(is_in_fight);

	// 是否在战斗中
	if (is_in_fight)
	{
		// 进战斗把时间设置为0
		att_atr->second = 0;

		//log_warn("role[%lu] attack long and enter fight time[%u]", p_role->get_object_id(), time_now);
	}
	else
	{
		king_war_manager_t::attack_long_add(p_role);

		// 时间更新要放在造成伤害上面，造成伤害会清列表，所以先设置时间，防止宕机
		att_atr->second = time_now;	// 更新时间

		// TODO:计算造成的伤害，目前为50
		hurt_by_family(100, p_role->get_object_id(), p_role->get_family_id());

		KINGWAR_LOG("role[%lu] attack long and long hurt time[%u]", p_role->get_object_id(), time_now);
	}
}

king_war_side_type long_pulse_t::get_king_war_side_type(uint64_t family_id)
{
	if (family_id == m_war_family[king_war_side_attack])
	{
		return king_war_side_attack;
	}
	else if (family_id == m_war_family[king_war_side_defence])
	{
		return king_war_side_defence;
	}

	return king_war_side_none;
}

void long_pulse_t::update_nearby_icon()
{
	for (const auto& object : m_vision_objects)
	{
		if (object.second == proto::common::SCENEOBJECT_USER)
		{
			role_ptr p_role = role_manager_t::find_role(object.first);
			if (NULL == p_role)
			{
				log_error("role[%u] error null", object.first);
				continue;
			}

			// 不是对战双方
			king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
			if (side_type == king_war_side_none)
			{
				continue;
			}

			if (p_role->get_scene_id() != m_scene_id || p_role->get_role_area_id() != m_area_id)
			{
				continue;
			}

			proto::client::gc_king_war_icon_notify notify;
			notify.set_state(1);
			notify.set_family_belong(common::string_util_t::uint64_to_string(m_occupy_family));
			notify.set_object_id(common::string_util_t::uint64_to_string(m_object_id));
			p_role->send_msg_to_client(op_cmd::gc_king_war_icon_notify, notify);
		}
	}
}

void long_pulse_t::hurt_by_family(uint32_t damage, uint64_t role_uid, uint64_t family_id)
{
	king_war_side_type side_type = get_king_war_side_type(family_id);
	if (side_type >= king_war_side_max)
	{
		log_error("role[%lu] family[%lu] not in king war, hurt value error", role_uid, family_id);
		return;
	}

	// 统计龙脉受到的伤害
	m_family_hurt[side_type] += damage;
	m_curr_hp = m_curr_hp > damage ? m_curr_hp - damage : 0;

	// 受伤同步9屏
	proto::client::gc_object_hp_change_notify ntf;
	ntf.set_object_id(common::string_util_t::uint64_to_string(m_object_id));
	ntf.set_change_type(proto::common::obj_hp_change_type_minus);
	ntf.set_change_value(damage);
	ntf.set_hurt_id(common::string_util_t::uint64_to_string(role_uid));
	send_msg_to_nine(op_cmd::gc_object_hp_change_notify, ntf);

	//死亡
	if (m_curr_hp == 0)
	{
		// 哪个家族照成的伤害多
		if (m_family_hurt[king_war_side_attack] > m_family_hurt[king_war_side_defence])
		{
			set_object_die(m_war_family[king_war_side_attack]);
		}
		else
		{
			set_object_die(m_war_family[king_war_side_defence]);
		}
	}
}

void long_pulse_t::on_object_born()
{
	// 直接调用基类
	object_position_t::on_object_born();

	// 设置周围人图标
	update_nearby_icon();
}

void long_pulse_t::set_object_die(uint64_t obj_killer /*= 0*/)
{
	// 设置占领家族
	m_occupy_family = obj_killer;

	// 直接调用基类
	object_position_t::set_object_die(obj_killer);
}

void long_pulse_t::on_object_die()
{
	monster_t::on_object_die();

	// 通知周围人状态变化
	for (const auto& object : m_vision_objects)
	{
		if (object.second == proto::common::SCENEOBJECT_USER)
		{
			role_ptr p_role = role_manager_t::find_role(object.first);
			if (NULL == p_role)
			{
				log_error("role[%u] error null", object.first);
				continue;
			}

			// 不是对战双方
			king_war_side_type side_type = get_king_war_side_type(p_role->get_family_id());
			if (side_type == king_war_side_none)
			{
				continue;
			}

			// 区域
			if (p_role->get_scene_id() != m_scene_id || p_role->get_role_area_id() != m_area_id)
			{
				continue;
			}

			// 如果正在攻击设置状态
			if (p_role->get_object_state() == proto::common::object_state_attack)
			{
				p_role->change_object_state(proto::common::object_state_normal);
			}

			// 通知消失图标
			proto::client::gc_king_war_icon_notify notify;
			notify.set_state(0);
			p_role->send_msg_to_client(op_cmd::gc_king_war_icon_notify, notify);
		}
	}

	// 重置队列
	m_defence_list.clear();
	m_attack_list.clear();

	// 重置伤害
	m_family_hurt[king_war_side_attack] = 0;
	m_family_hurt[king_war_side_defence] = 0;

	// 定时增加龙气取消
	if (NULL != m_occupied_timer)
	{
		m_occupied_timer->cancel();
	}

	// 10秒后复活
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_dragon_revive_time);
	uint32_t revive_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (revive_time == 0)
	{
		log_error("revive_time == 0 error");
		return;
	}

	if (m_revive_timer != NULL)
	{
		m_revive_timer->expires_from_now(boost::posix_time::seconds(revive_time));
		m_revive_timer->async_wait(boost::bind(&king_war_manager_t::on_long_revive_timer_call_back, boost::asio::placeholders::error, m_object_id));
	}

	// 全场景更新龙脉状态
	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
	if (NULL == p_scene)
	{
		log_error("p_scene[%lu] null error", m_scene_id);
		return;
	}

	if (!p_scene->is_king_war_city())
	{
		log_error("scene[%lu] is not king country", m_scene_id);
		return;
	}

	king_war_scene_ptr p_king_scene = p_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL)
	{
		log_error("scene[%lu]is not king country", m_scene_id);
		return;
	}

	p_king_scene->notify_scene_long_info();
}

void long_pulse_t::on_object_revive()
{
	m_curr_hp = m_max_hp;

	// 直接调用基类
	monster_t::on_object_revive();

	// 设置周围人图标
	update_nearby_icon();

	// 全场景更新龙脉状态
	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
	if (NULL == p_scene)
	{
		log_error("p_scene[%lu] null error", m_scene_id);
		return;
	}

	if (!p_scene->is_king_war_city())
	{
		log_error("scene[%lu] is not king country", m_scene_id);
		return;
	}

	king_war_scene_ptr p_king_scene = p_scene->get_king_war_scene_ptr();
	if (p_king_scene == NULL)
	{
		log_error("scene[%lu]is not king country", m_scene_id);
		return;
	}

	p_king_scene->notify_scene_long_info();
}

void long_pulse_t::on_revive_timer_call_back()
{
	// 时间到了复活
    // 复活通知周围的人
    set_object_revive(0);

	// 有占领家族 每秒给这个家族加龙气
	if (m_occupied_timer != NULL && m_occupy_family != 0)
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_dragon_score_interval);
		uint32_t interval_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
		if (interval_time == 0)
		{
			log_error("interval_time == 0 error");
			return;
		}

		m_occupied_timer->expires_from_now(boost::posix_time::seconds(interval_time));
		m_occupied_timer->async_wait(boost::bind(&king_war_manager_t::on_long_occupied_timer_call_back, boost::asio::placeholders::error, m_object_id));
	}
}

void long_pulse_t::on_occupied_timer_call_back()
{
	// 有占领家族 每秒给这个家族加龙气
	if (m_occupied_timer != NULL && m_occupy_family != 0)
	{
		add_family_long_gas();

		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_dragon_score_interval);
		uint32_t interval_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
		if (interval_time == 0)
		{
			log_error("interval_time == 0 error");
			return;
		}

		m_occupied_timer->expires_from_now(boost::posix_time::seconds(interval_time));
		m_occupied_timer->async_wait(boost::bind(&king_war_manager_t::on_long_occupied_timer_call_back, boost::asio::placeholders::error, m_object_id));
	}
}

void long_pulse_t::add_family_long_gas()
{
	uint32_t group_id = 0;

	switch (m_obj_type)
	{
	case king_war_obj_long:
		{
			group_id = comprehensive_common::king_dragon_score_add1;
		}
		break;
	case king_war_obj_long1:
		{
			group_id = comprehensive_common::king_dragon_score_add2;
		}
		break;
	case king_war_obj_long2:
		{
			group_id = comprehensive_common::king_dragon_score_add3;
		}
		break;
	default:
		{
			log_error("long obj type[%u] error", m_obj_type);
			return;
		}
		break;
	}

	auto p_conf = GET_CONF(Comprehensive, group_id);
	uint32_t add_value = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (add_value == 0)
	{
		log_error("long gas add_value == 0 error");
		return;
	}

	king_war_manager_t::add_long_gas(m_country_id, m_occupy_family, add_value);
}

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
#include "tblh/JumpTable.tbls.h"
#include "user_troop/user_troop_mgr.hpp"
#include "achieve/achieve_common.hpp"

king_war_scene_t::king_war_scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param)
	: scene_t(scene_id, map_id, scene_type, type_param)
{
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
	m_relive_side[king_war_side_attack].m_scene_id = conf->att_jump_pos();

	m_relive_side[king_war_side_defence].m_area_id = conf->def_area_id();
	m_relive_side[king_war_side_defence].m_jump_id = conf->def_jump_id();
	m_relive_side[king_war_side_defence].m_scene_id = conf->def_jump_pos();

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
		m_fight_family[i].reset();
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
	uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
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
	uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
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
		notify.set_occupy_camp(p_long_monster->get_occupy_side());
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
		uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
		// 参战玩家,加入列表
		if (side_type < king_war_side_max)
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
				info->set_occupy_family(common::string_util_t::uint64_to_string(p_long_monster->get_occupy_family()));
				info->set_occupy_help(common::string_util_t::uint64_to_string(p_long_monster->get_occupy_help_family()));
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

// 	struct king_war_select : public scene_select_t
// 	{
// 		explicit king_war_select(uint64_t att_family_id, uint64_t def_family_id) : m_att_family_id(att_family_id), m_def_family_id(def_family_id) {};
// 		bool exec(const role_ptr& p_role) const
// 		{
// 			if (p_role->get_family_id() == m_att_family_id || p_role->get_family_id() == m_def_family_id)
// 			{
// 				return true;
// 			}
// 			else
// 			{
// 				return false;
// 			}
// 		}
// 
// 		uint64_t m_att_family_id = 0;
// 		uint64_t m_def_family_id = 0;
// 	}king_war_select(m_family[king_war_side_attack], m_family[king_war_side_defence]);

	send_msg_to_scene(op_cmd::gc_update_long_info_notify, notify);
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
	uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
	if (side_type == king_war_side_none)
	{
		log_error("p_role[%lu] is not king war family", p_role->get_object_id());
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

void king_war_scene_t::role_get_artifacts(const role_ptr& p_role, uint64_t artifacts_uid, uint32_t buff_tid)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return;
	}

	// 删除神器
	if (m_artifacts_info.m_artifacts_id == artifacts_uid && m_artifacts_info.m_state == em_artifacts_appear)
	{
		// id清0
		m_artifacts_info.m_artifacts_id = 0;
		m_artifacts_info.m_state = em_artifacts_collected;

		// 获得自己的信息，没有就增加
		king_war_role_ptr p_king_war_role = king_war_manager_t::get_king_war_role_ex(m_country_id, p_role);
		if (p_king_war_role != NULL)
		{
			auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_start_weapon_merit);
			uint32_t add_val = GET_COMPREHENSIVE_VALUE_1(p_conf);
			p_king_war_role->add_exploit(add_val);

			KINGWAR_LOG("role[%lu] user artifacts, add exploit[%u]", p_role->get_uid(), add_val);
		}

		achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_KING_WAR_START_WEAPON);

		m_artifacts_info.m_buff_tid = buff_tid;
		m_artifacts_info.m_king_war_side = get_king_war_side_type(p_role->get_family_id());
		m_artifacts_info.m_state = em_artifacts_start;

		// 1 秒后启动神器效果
        if (NULL != m_artifacts_timer)
        {
            m_artifacts_timer->expires_from_now(boost::posix_time::seconds(1));
            m_artifacts_timer->async_wait(boost::bind(&king_war_scene_t::on_artifacts_call_back, this, boost::asio::placeholders::error));
        }

		// 	// 通知本场景参战家族10秒后发动神器
		// 	struct king_war_select : public scene_select_t
		// 	{
		// 		explicit king_war_select(uint64_t att_family_id, uint64_t def_family_id) : m_att_family_id(att_family_id), m_def_family_id(def_family_id) {};
		// 		bool exec(const role_ptr& p_role) const
		// 		{
		// 			if (p_role != NULL && (p_role->get_family_id() == m_att_family_id || p_role->get_family_id() == m_def_family_id))
		// 			{
		// 				return true;
		// 			}
		// 
		// 			return false;
		// 		}
		// 
		// 		uint64_t m_att_family_id = 0;
		// 		uint64_t m_def_family_id = 0;
		// 	}king_war_select(m_family[king_war_side_attack], m_family[king_war_side_defence]);

		proto::client::gc_artifact_left_time_notify notify;
		notify.set_family_name(p_role->get_family_name());
		notify.set_role_name(p_role->get_name());
		notify.set_tid(buff_tid);
		send_msg_to_scene(op_cmd::gc_artifact_left_time_notify, notify);

		KINGWAR_LOG("role[%lu] use artifacts[%u]", p_role->get_object_id(), m_artifacts_info.m_artifacts_tid);
	}
}

void king_war_scene_t::on_role_kill_other(role_ptr p_role, object_base_ptr p_enemy)
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
		king_war_manager_t::role_kill_other(get_king_war_scene_ptr(), p_role, p_enemy->get_role());
	}
}

void king_war_scene_t::on_other_kill_me(role_ptr p_role, object_base_ptr p_enemy)
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

	king_war_manager_t::other_kill_self(get_king_war_scene_ptr(), p_role, p_enemy->get_role());

	p_role->set_object_die(p_enemy->get_object_id());

	if (m_king_war_state == proto::common::king_war_state_start) {
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_revive_time);
		uint32_t revive_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
		p_role->set_revive_time(revive_time);
	}

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
	if (m_king_war_state != proto::common::king_war_state_start)
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

	king_war_role_ptr p_king_war_role = king_war_manager_t::get_king_war_role(m_country_id, p_role->get_object_id());
	if (p_king_war_role != NULL)
	{
		notify.set_death_count(p_king_war_role->get_death_count());
		notify.set_king_war_free(p_king_war_role->get_free_revive_count());
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
	if (m_king_war_state != proto::common::king_war_state_start)
	{
		scene_t::on_login_die(p_role);
		return;
	}

	// 弹出复活界面
	show_revive_ui(p_role);
}

void king_war_scene_t::update_center_role_num()
{
	proto::server::ge_king_war_scene_user_num_notify ntf;
	ntf.set_country_id(m_country_id);
	ntf.set_att_num(m_family_roles[king_war_side_attack].size());
	ntf.set_def_num(m_family_roles[king_war_side_defence].size());
	env::server->send_msg_to_center(op_cmd::ge_king_war_scene_user_num_notify, 0, ntf);
}

uint32_t king_war_scene_t::ask_king_war_enter_scene(const role_ptr& p_role)
{
	if (!p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	switch (m_king_war_state) {
		case proto::common::king_war_state_end:
		case proto::common::king_war_state_sign_up:
		case proto::common::king_war_state_pre: {
			// 拉人
			map_ptr p_jump_map = get_map_ptr();
			if (NULL != p_jump_map) {
				return scene_manager_t::role_jump_point(p_role, p_jump_map->get_born_point(p_role->get_level()));
			}
			break;
		}
		case proto::common::king_war_state_prepare: {
			log_error("role[%lu] state == proto::common::king_war_state_prepare, can't enter scene", p_role->get_uid());
			return common::errcode_enum::king_war_error_not_start_fight;
			break;
		}
		case proto::common::king_war_state_start: {
			uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
			if (side_type == king_war_side_none) {
				log_error("role[%lu] not king war side", p_role->get_uid());
				return common::errcode_enum::king_war_error_not_king_war_family;
			}

			if (p_role->get_king_war_warfare() == 0) {
				KINGWAR_LOG("p_role[%lu] king_war_warfare == 0", p_role->get_uid());
				return common::errcode_enum::notice_king_war_error14;
			}

			// 传送到据点
			return scene_manager_t::role_jump_point(p_role, m_relive_side[side_type].m_scene_id);
			break;
		}
		default:
			log_error("country[%u] king_war_state[%u] error", m_country_id, m_king_war_state);
			break;
	}

	return common::errcode_enum::notice_unknown;
}

void king_war_scene_t::load_map_collect()
{
	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return;
	}

	const collect_define_vec& collect_vec = p_map->get_map_collect_points();
	for (collect_define_vec::const_iterator itr = collect_vec.begin();
		itr != collect_vec.end(); ++itr)
	{
		const t_collect_define& define = *itr;

		CollectTable* p_collect_config = GET_CONF(CollectTable, define.m_collect_id);
		if (NULL == p_collect_config)
		{
			log_error("scene::load_map_object not found collect [%d]", define.m_collect_id);
			continue;
		}

		// 神器不加载
		if (p_collect_config->type() == proto::common::collect_type_artifacts)
		{
			m_artifacts_vec.push_back(define);
			continue;
		}

		collect_point_ptr p_collect_point(new collect_point_t(define.m_collect_id));
		if (p_collect_point != NULL && p_collect_point->init_collect_point(define.m_x, define.m_y))
		{
			init_collect_point(p_collect_point);
		}
	}
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
			uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
			// 参战玩家
			if (side_type < king_war_side_max)
			{
				m_family_roles[side_type].insert(*itr);
			}
		}
	}

	update_center_role_num();

	// 通知全场景3秒后拉人
	proto::client::gc_king_war_scene_start_notify notify;
	notify.set_def_family_id(common::string_util_t::uint64_to_string(m_fight_family[king_war_side_defence].m_family_id));
	notify.set_def_help_family(common::string_util_t::uint64_to_string(m_fight_family[king_war_side_defence].m_help_family));
	notify.set_att_family_id(common::string_util_t::uint64_to_string(m_fight_family[king_war_side_attack].m_family_id));
	notify.set_att_help_family(common::string_util_t::uint64_to_string(m_fight_family[king_war_side_attack].m_help_family));
	send_msg_to_scene(op_cmd::gc_king_war_scene_start_notify, notify);

	// 定时器，开战拉人
	if (m_ready_timer != NULL)
	{
		m_ready_timer->expires_from_now(boost::posix_time::seconds(3));
		m_ready_timer->async_wait(boost::bind(&king_war_scene_t::on_ready_timer_call_back, this, boost::asio::placeholders::error));
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
			p_long_pulse->set_king_war_family(king_war_side_attack, m_fight_family[king_war_side_attack]);
			p_long_pulse->set_king_war_family(king_war_side_defence, m_fight_family[king_war_side_defence]);

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

	// 全场景通知龙脉信息
	notify_scene_long_info();

	// 定时器，刷新场景龙脉信息
	if (m_refresh_timer != NULL)
	{
		m_refresh_timer->expires_from_now(boost::posix_time::seconds(5));
		m_refresh_timer->async_wait(boost::bind(&king_war_scene_t::on_refresh_info_call_back, this, boost::asio::placeholders::error));
	}

	// 定时器，神器刷新前提醒
	m_artifacts_info.reset();
	if (m_artifacts_timer != NULL)
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::king_weapon_refresh_initial);
		uint32_t refresh_initial = GET_COMPREHENSIVE_VALUE_1(p_conf);
		
		// 如果第一次刷新时间小于1分钟 直接进入预告期
		if (refresh_initial > 1) {
			m_artifacts_timer->expires_from_now(boost::posix_time::seconds((refresh_initial - 1) * 60));
		} else {
			m_artifacts_timer->expires_from_now(boost::posix_time::seconds(1));
		}
		m_artifacts_timer->async_wait(boost::bind(&king_war_scene_t::on_artifacts_call_back, this, boost::asio::placeholders::error));
		m_artifacts_info.m_state = em_artifacts_ready;
	}

	// 设置场景中的玩家的pk模式
	std::set<uint64_t> role_list = m_roles;
	for (std::set<uint64_t>::iterator itr = role_list.begin();
		itr != role_list.end(); ++itr)
	{
		role_ptr p_role = role_manager_t::find_role(*itr);
		if (NULL != p_role)
		{
			uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
			// 参战玩家
			if (side_type < king_war_side_max)
			{
				auto p_king_fight = GET_CONF(Comprehensive, comprehensive_common::king_fight_number);
				uint32_t king_fight_number = GET_COMPREHENSIVE_VALUE_1(p_king_fight);

				p_role->set_king_war_warfare(king_fight_number);
				p_role->change_pk_mode(proto::common::PK_CAMP, side_type);

				proto::client::gc_role_battle_value_notify battle_value_notify;
				battle_value_notify.set_value(p_role->get_king_war_warfare());
				battle_value_notify.set_state(1);
				p_role->send_msg_to_client(op_cmd::gc_role_battle_value_notify, battle_value_notify);
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
		m_fight_family[i].reset();
		m_king_war_role[i].Clear();
		m_king_war_warfare[i] = 0;
	}

	m_roles_pos_time = 0;

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

	// 全场景通知龙脉信息
	notify_scene_long_info();

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
	std::set<uint64_t> role_list = m_roles;
	for (std::set<uint64_t>::iterator itr = role_list.begin();
		itr != role_list.end(); ++itr)
	{
		role_ptr p_role = role_manager_t::find_role(*itr);
		if (NULL != p_role && p_role->get_pk_mode() == proto::common::PK_CAMP)
		{
			// 结束时候还死亡直接复活
			if (p_role->is_die())
			{
				p_role->set_object_revive(0);
			}

			// 改变一下pk模式
			p_role->change_pk_mode(proto::common::PK_PEACE);

			// 设置玩家状态
			p_role->set_king_war_warfare(0);

			// 当时正在攻击状态的改变状态
			if (p_role->get_object_state() == proto::common::object_state_attack)
			{
				p_role->change_object_state(proto::common::object_state_normal);
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

void king_war_scene_t::set_king_war_family(king_war_side_type type, const king_war_family& family_info)
{
	if (type >= king_war_side_max)
	{
		log_error("family type[%u] error", type);
		return;
	}

	m_fight_family[type] = family_info;
}

void king_war_scene_t::king_war_scene_process(role_ptr p_role)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return;
	}

	uint32_t side_type = king_war_side_none;

	if (p_role->get_troop_id() != 0) {
		if (p_role->is_leader()) {
			side_type = get_king_war_side_type(p_role->get_family_id());
			if (side_type != king_war_side_none) {
				troop_ptr p_troop = game_troop_mgr_t::get_troop(p_role->get_troop_id());
				if (p_troop && p_troop->member_scene_check(get_scene_ptr()) != common::errcode_enum::error_ok) { // 队伍中有人不能进这个场景
					side_type = king_war_side_none;
				}
			}
		} else {
			if (p_role->get_troop_state() == 0) { // 有队伍未暂离不处理
				return;
			}

			side_type = get_king_war_side_type(p_role->get_family_id());
		}
	} else {
		side_type = get_king_war_side_type(p_role->get_family_id());
	}

	if (side_type == king_war_side_none) {
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::imperial_city_id);
		uint32_t king_city = GET_COMPREHENSIVE_VALUE_1(p_conf);
		scene_manager_t::role_jump_scene(p_role, proto::server::user_change_scene, king_city);
	} else {
		if (p_role->get_role_area_id() != m_relive_side[side_type].m_area_id) {
			scene_manager_t::role_jump_point(p_role, m_relive_side[side_type].m_jump_id);
		}
	}
}

uint32_t king_war_scene_t::get_king_war_side_type(uint64_t family_id)
{
	if (family_id != 0) {
		for (uint32_t i = king_war_side_attack; i < king_war_side_max; ++i) {
			if (family_id == m_fight_family[i].m_family_id || family_id == m_fight_family[i].m_help_family) {
				return i;
			}
		}
	}

	return king_war_side_none;
}

sPos king_war_scene_t::get_scene_born_position(const role_ptr& p_role)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return sPos();
	}

	if (m_king_war_state >= proto::common::king_war_state_prepare) {
		uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
		if (side_type < king_war_side_max) {
			JumpTable* p_jump_info = GET_CONF(JumpTable, m_relive_side[side_type].m_jump_id);
			if (NULL != p_jump_info) {	
				return sPos(p_jump_info->to_position_x(), p_jump_info->to_position_y(), p_jump_info->to_angle());
			} else {
				log_error("role[%lu] scene[%lu,%u] can not find jump id[%u]", p_role->get_uid(), m_scene_id, m_map_tid, m_relive_side[side_type].m_jump_id);
			}
		}
	}

	return scene_t::get_scene_born_position(p_role);
}

uint32_t king_war_scene_t::scene_enter_check(const role_ptr& p_role)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	if (m_king_war_state >= proto::common::king_war_state_prepare) {
		uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
		if (side_type == king_war_side_none) {
			return common::errcode_enum::king_war_error_can_not_king_war_scene;
		} else {
			return common::errcode_enum::error_ok;
		}
	}

	return scene_t::scene_enter_check(p_role);
}

uint32_t king_war_scene_t::check_scene_troop()
{
	if (m_king_war_state >= proto::common::king_war_state_prepare) {
		TROOP_LOG("king war scene can't troop when m_king_war_state >= proto::common::king_war_state_prepare");
		return common::errcode_enum::king_war_error_no_troop;
	}

	return common::errcode_enum::error_ok;
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
		p_role->change_pk_mode(proto::common::PK_PEACE);
	}
	else
	{
		uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
		// 参战玩家,加入列表
		if (side_type < king_war_side_max)
		{
			m_family_roles[side_type].insert(p_role->get_object_id());

			update_center_role_num();

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
					auto p_king_fight = GET_CONF(Comprehensive, comprehensive_common::king_fight_number);
					uint32_t king_fight_number = GET_COMPREHENSIVE_VALUE_1(p_king_fight);

					p_role->set_king_war_warfare(king_fight_number);
					battle_value_notify.set_value(king_fight_number);
				}
				battle_value_notify.set_state(1);
				p_role->send_msg_to_client(op_cmd::gc_role_battle_value_notify, battle_value_notify);

				// 没有战意值不能pk
				if (p_role->get_king_war_warfare() == 0)
				{
					p_role->change_pk_mode(proto::common::PK_PEACE);
				}
				else
				{
					p_role->change_pk_mode(proto::common::PK_CAMP, side_type);
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
				p_role->change_pk_mode(proto::common::PK_PEACE);
			}
		}
		else
		{
			p_role->change_pk_mode(proto::common::PK_PEACE);
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

	uint32_t side_type = get_king_war_side_type(p_role->get_family_id());
	// 参战玩家,加入列表
	if (side_type < king_war_side_max)
	{
		m_family_roles[side_type].erase(p_role->get_object_id());

		update_center_role_num();
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
	std::set<uint64_t> role_list = m_roles;
	for (const auto& uid : role_list) {
		const role_ptr& p_role = role_manager_t::find_role(uid);
		if (p_role) {
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
			m_artifacts_info.m_start_left = common::time_util_t::now_time() + 60;
			// 1分钟后出现神器
			m_artifacts_timer->expires_from_now(boost::posix_time::seconds(60));
			m_artifacts_timer->async_wait(boost::bind(&king_war_scene_t::on_artifacts_call_back, this, boost::asio::placeholders::error));

			// 公告
			BROADCAST_SCENE_NOTICE(get_scene_ptr(), sys_notice_enum::sys_notice_22, 0, 0, m_country_id);
		}
		break;
	case em_artifacts_pre:
		{
			if (m_artifacts_vec.empty()) {
				log_error("country[%u] king war artifacts empty", m_country_id);
				return;
			}

			int32_t index = random_util_t::randBetween(0, (int32_t)m_artifacts_vec.size() - 1);
			if (index == -1)
			{
				log_error("rand from 0 to [%d] error", (int32_t)m_artifacts_vec.size() - 1);
				return;
			}

			const t_collect_define& define = m_artifacts_vec.at(index);

			CollectTable* p_collect_config = GET_CONF(CollectTable, define.m_collect_id);
			if (NULL == p_collect_config)
			{
				log_error("scene::load_map_object not found collect [%d]", define.m_collect_id);
				m_artifacts_info.m_state = em_artifacts_none;
				return;
			}

			collect_point_ptr p_collect_point = collect_point_ptr(new collect_point_t(define.m_collect_id));
			if (!p_collect_point)
			{
				log_error("p_collect_point null error");
				return;
			}
			
			if (!p_collect_point->init_collect_point(define.m_x, define.m_y))
			{
				log_error("p_collect_point init_collect_point error");
				return;
			}
			
			if (!init_collect_point(p_collect_point))
			{
				log_error("p_collect_point init_collect_point error");
				return;
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
			// 加buff
			if (m_artifacts_info.m_king_war_side != king_war_side_none) {
				for (std::set<uint64_t>::iterator itr = m_roles.begin();
					itr != m_roles.end(); ++itr)
				{
					role_ptr p_role = role_manager_t::find_role(*itr);
					if (NULL != p_role && p_role->get_king_war_warfare() > 0)
					{
						uint32_t side = get_king_war_side_type(p_role->get_family_id());
						if (side == m_artifacts_info.m_king_war_side) {
							role_buff_mgr_ptr p_role_buff_mgr = p_role->get_role_buff_mgr();
							if (NULL != p_role_buff_mgr)
							{
								p_role_buff_mgr->add_role_buff(m_artifacts_info.m_buff_tid, log_enum::source_type_role_buff_king_war, m_country_id);
								p_role_buff_mgr->notify_to_client();
							}
						}
					}
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
	default:
		{
			log_error("scene[%lu] m_artifacts_info.m_state[%u] error", m_scene_id, m_artifacts_info.m_state);
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

void king_war_scene_t::get_king_war_roles_pos(uint32_t side, proto::common::king_war_side_roles* p_roles)
{
	if (p_roles == NULL) {
		log_error("p_roles null error");
		return;
	}

	if (side == king_war_side_none) {
		return;
	}

	uint32_t curr_time = common::time_util_t::now_time();
	if (curr_time > m_roles_pos_time) { // 每3秒同步一次所有人的位置信息
		m_king_war_role[king_war_side_attack].Clear();
		m_king_war_role[king_war_side_defence].Clear();
		m_king_war_warfare[king_war_side_attack] = 0;
		m_king_war_warfare[king_war_side_defence] = 0;

		for (std::set<uint64_t>::const_iterator citr = m_roles.begin();
			citr != m_roles.end(); ++citr) {
			const role_ptr& p_role = role_manager_t::find_role(*citr);
			if (NULL != p_role && p_role->get_pk_mode() == proto::common::PK_CAMP) {
				// 参战玩家
				uint32_t side = p_role->get_pk_mode_ex();
				if (side < king_war_side_max) {
					proto::common::king_war_role_pos* p_role_pos = m_king_war_role[side].add_roles();
					if (p_role_pos) {
						p_role_pos->set_pos_x(p_role->get_current_x());
						p_role_pos->set_pos_y(p_role->get_current_y());
					}

					m_king_war_warfare[side] += p_role->get_king_war_warfare();
				}
			}
		}

		m_roles_pos_time = curr_time + 3;
	}

	p_roles->CopyFrom(m_king_war_role[side]);
}

uint32_t king_war_scene_t::get_king_war_warfare(uint32_t side_type) const
{
	if (side_type < king_war_side_max) {
		return m_king_war_warfare[side_type];
	}

	return 0;
}

uint32_t king_war_scene_t::get_artifacts_start_left() const
{
	if (m_artifacts_info.m_state == em_artifacts_pre) {
		return m_artifacts_info.m_start_left;
	}

	return 0;
}


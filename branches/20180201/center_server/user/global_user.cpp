#include "global_user.hpp"
#include "main/center_server.hpp"

#include "utility.hpp"
#include "redis_client.hpp"
#include "country/country_mgr.hpp"
#include "offline_friend_msg_mgr/offline_friend_msg_mgr.hpp"
#include "challenge/challenge_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"
#include "redis/global_user_data_mgr.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "global_user_msg_handle.hpp"
#include "scene/global_scene_manager.hpp"
#include "tblh/MapTable.tbls.h"
#include "family/family_manager.hpp"
#include "red_point/red_point_manager.hpp"
#include "log/log_wrapper.hpp"
#include "offline/offline_role_manager.hpp"
#include "offline_arena/offline_arena_manager.hpp"
#include "common/time_manager.hpp"
#include "tblh/time_type_enum.hpp"
#include "mail/global_mail_manager.hpp"

#define WRITE_SCENE_LOG_TO_DB(uid, old_map, new_map, type) \
    log_wrapper_t::send_scene_log(uid, old_map, new_map, type);

USING_NS_NETWORK;
USING_NS_COMMON;

global_user_t::global_user_t(uint64_t uid)
	: m_uid(uid)
	, m_friend_mgr(uid)
	, m_mail_data(uid)
	, m_shop_goods_data(uid)
{

}

global_user_t::~global_user_t()
{

}

void global_user_t::set_name(const std::string& val)
{
	if (m_common_info.m_name != val)
	{
		m_common_info.m_name = val;

		global_user_data_mgr_t::set_name(m_uid, val);
	}
}

void global_user_t::set_country_id(uint32_t val)
{
	m_common_info.m_country_id = val;
	m_common_info.m_country_officer = 0;

	global_user_data_mgr_t::set_user_country_id(m_uid, val);
	global_user_data_mgr_t::set_user_country_officer(m_uid, 0);
}

void global_user_t::set_country_officer(uint32_t val)
{
	if (m_common_info.m_country_officer != val)
	{
		m_common_info.m_country_officer = val;

		global_user_data_mgr_t::set_user_country_officer(m_uid, val);

		// 同步game
		global_user_msg_handle_t::sync_data_to_game(m_uid, proto::server::data_sync_type_country_officer, val, true);
	}
}

void global_user_t::join_family(uint64_t family_id, uint32_t officer, uint32_t country_id)
{
	m_common_info.m_family_id = family_id;
	m_common_info.m_family_officer = officer;
	m_common_info.m_country_id = country_id;
	m_common_info.m_country_officer = 0;

	global_user_data_mgr_t::set_user_family_id(m_uid, family_id);
	global_user_data_mgr_t::set_user_family_officer(m_uid, officer);

	global_user_data_mgr_t::set_user_country_id(m_uid, country_id);
	global_user_data_mgr_t::set_user_country_officer(m_uid, 0);
}

void global_user_t::leave_family()
{
	m_common_info.m_family_id = 0;
	m_common_info.m_family_officer = 0;
	m_common_info.m_country_id = 0;
	m_common_info.m_country_officer = 0;

	global_user_data_mgr_t::set_user_family_id(m_uid, 0);
	global_user_data_mgr_t::set_user_family_officer(m_uid, 0);

	global_user_data_mgr_t::set_user_country_id(m_uid, 0);
	global_user_data_mgr_t::set_user_country_officer(m_uid, 0);
}

void global_user_t::set_family_officer(uint32_t val)
{
	if (m_common_info.m_family_officer != val)
	{
		m_common_info.m_family_officer = val;

		global_user_data_mgr_t::set_user_family_officer(m_uid, val);

		// 同步game
		global_user_msg_handle_t::sync_data_to_game(m_uid, proto::server::data_sync_type_family_officer, m_common_info.m_family_officer, true);
	}
}
    
void global_user_t::set_last_logout_time(uint32_t val)
{
	if (m_common_info.m_last_logout_time != val)
	{
		m_common_info.m_last_logout_time = val;

		global_user_data_mgr_t::set_last_logout_time(m_uid, val);
	}
}

void global_user_t::set_level(uint32_t val)
{
	if (m_common_info.m_level != val)
	{
		m_common_info.m_level = val;

		global_user_data_mgr_t::set_user_level(m_uid, val);

		//如果有队伍，同步数据到队伍
		if (get_troop_id() > 0)
		{
			troop_ptr p_troop = global_troop_manager::get_troop(get_troop_id());
			if (NULL != p_troop)
			{
				p_troop->update_member_level(m_uid, val);
			}
		}
		//离线竞技场决定是否要晋级
		offline_arena_manager_t::update_role_level(m_uid, val);
		update_attr();
	}
}

void global_user_t::set_fighting_value(uint32_t val)
{
	if (m_common_info.m_fighting_value != val)
	{
		m_common_info.m_fighting_value = val;

		global_user_data_mgr_t::set_user_fighting_value(m_uid, val);

		//如果有队伍，同步数据到队伍
		if (get_troop_id() > 0)
		{
			troop_ptr p_troop = global_troop_manager::get_troop(get_troop_id());
			if (NULL != p_troop)
			{
				p_troop->update_member_fighting(m_uid, val);
			}
		}

		update_attr();
	}
}

void global_user_t::set_hero_plugin(uint32_t val)
{
	if (m_common_info.m_hero_plugin != val)
	{
		m_common_info.m_hero_plugin = val;

		global_user_data_mgr_t::set_user_hero_plugin(m_uid, val);

		update_attr();
	}
}

bool global_user_t::reg_user(uint32_t gate_id)
{
	// 设置gate_id
	set_gate_id(gate_id);

	// 默认是下线，只有在场景服务器登陆时才算上线
	m_online_state = common::ROLE_ONLINE_STATE_LOGOUT;

	// 加载基本信息
	global_user_data_mgr_t::load_global_user_common(m_uid, m_common_info);

	// 判断登陆场景
	// 场景是否还存在
	global_scene_ptr p_scene = NULL;
	do 
	{
		// 下线场景存在不
		p_scene = global_scene_manager_t::get_scene_by_id(m_common_info.m_scene_id);
		if (NULL != p_scene /*&& p_scene->get_scene_state() == SCENE_STATE_NORMAL*/)
			break;

		// 最后静态场景存在不
		p_scene = global_scene_manager_t::get_scene_by_id(m_common_info.m_last_map_tid);
		if (NULL != p_scene /*&& p_scene->get_scene_state() == SCENE_STATE_NORMAL*/)
		{
			set_scene_id(m_common_info.m_last_map_tid, m_common_info.m_last_map_tid);
			break;
		}

		// 都没有，只能去出生地图
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::born_map_tid);
		uint32_t born_map_tid = GET_COMPREHENSIVE_VALUE_1(p_conf);
		p_scene = global_scene_manager_t::get_scene_by_id(born_map_tid);
		if (NULL != p_scene /*&& p_scene->get_scene_state() == SCENE_STATE_NORMAL*/)
		{
			set_scene_id(born_map_tid, born_map_tid);
			break;
		}

	} while (0);

	if (NULL == p_scene)
	{
		log_error("user[%lu] can no find scene can reg!!!", m_uid);
		return false;
	}

	set_game_id(p_scene->get_game_id());

	// 通知场景玩家注册
	proto::server::eg_reg_user_scene_request reginscene;
	reginscene.set_uid(m_uid);
	reginscene.set_gate_id(gate_id);

	// 场景数据
	proto::server::cross_server_data* p_server_data = reginscene.mutable_scene_data();
	if (p_server_data == NULL)
	{
		log_error("user[%lu] p_server_data null error", m_uid);
		return false;
	}

	p_server_data->set_type(proto::server::login_server_scene);
	p_server_data->set_scene_id(m_common_info.m_scene_id);

	//家族 国家
	{
		reginscene.set_family_id(m_common_info.m_family_id);
		reginscene.set_family_officer(get_family_officer());
		if (m_common_info.m_family_id != 0)
		{
			const family_ptr& p_family = family_manager_t::get_family(m_common_info.m_family_id);
			if (NULL != p_family)
			{
				reginscene.set_family_name(p_family->get_name());
			}
			else
			{
				log_error("family[%lu] find name error", m_common_info.m_family_id);
				reginscene.set_family_name("");
			}
		}
		else
		{
			reginscene.set_family_name("");
		}

		reginscene.set_country_id(m_common_info.m_country_id);
		reginscene.set_country_officer(m_common_info.m_country_officer);
	}

	send_msg_to_game(op_cmd::eg_reg_user_scene_request, reginscene);

	return true;
}

bool global_user_t::relogin(uint32_t gate_id)
{
	// 设置gate_id
	if (m_gate_id != gate_id)
	{
		log_error("user[%lu] m_gate_id[%u] != gate_id[%u]", m_uid, m_gate_id, gate_id);
		return false;
	}

	proto::server::eg_relogin_scene_request reginscene;
	reginscene.set_uid(m_uid);
	reginscene.set_gate_id(gate_id);

	send_msg_to_game(op_cmd::eg_relogin_scene_request, reginscene);

	return true;
}

bool global_user_t::reg_cross(const proto::server::cross_server_data& data)
{
	// 组织玩家跨服信息
	proto::server::eg_reg_user_scene_request reginscene;
	reginscene.set_uid(m_uid);
	reginscene.set_gate_id(get_gate_id());

	// 场景数据
	proto::server::cross_server_data* p_server_data = reginscene.mutable_scene_data();
	if (p_server_data == NULL)
	{
		log_error("user[%lu] p_server_data null error", m_uid);
		return false;
	}

	switch (data.type())
	{
	case proto::server::user_change_scene:
	case proto::server::user_enter_dungeon:
	case proto::server::forced_change_scene:
	case proto::server::center_change_scene:
		{
			global_scene_ptr p_to_scene = global_scene_manager_t::get_scene_by_id(data.scene_id());
			if (NULL == p_to_scene)
			{
				log_error("user[%lu] cross server scene[%lu] not find", m_uid, data.scene_id());
				return false;
			}

			set_game_id(p_to_scene->get_game_id());

			p_server_data->CopyFrom(data);
		}
		break;
	case proto::server::user_troop_jump:
		{
			troop_ptr p_troop = global_troop_manager::get_troop(get_troop_id());
			if (p_troop == NULL)
			{
				log_error("user[%lu] troop[%lu] can't find", m_uid, get_troop_id());
				return false;
			}

			// 应该是队长拉人,不能自己拉自己
			if (p_troop->get_leader_id() == m_uid)
			{
				log_error("user[%lu] cross_server type[%u] leader_uid[%lu] == self", m_uid, data.type(), p_troop->get_leader_id());
				return false;
			}

			const global_user_ptr& p_leader = global_user_manager_t::get_global_user_by_uid(p_troop->get_leader_id());
			if (p_leader == NULL)
			{
				log_error("user[%lu] troop[%lu] learder[%lu] can't find", m_uid, get_troop_id(), p_troop->get_leader_id());
				return false;
			}

			global_scene_ptr p_to_scene = global_scene_manager_t::get_scene_by_id(p_leader->get_scene_id());
			if (NULL == p_to_scene)
			{
				log_error("user[%lu] cross server scene[%lu] not find", m_uid, p_to_scene->get_scene_id());
				return false;
			}

			set_game_id(p_to_scene->get_game_id());

			p_server_data->set_type(proto::server::user_troop_jump);
			p_server_data->set_scene_id(p_leader->get_scene_id());
		}
		break;
	default:
		{
			log_error("user[%lu] cross server type[%u] error", m_uid, data.type());
			return false;
		}
		break;
	}

	//家族 国家
	{
		reginscene.set_family_id(m_common_info.m_family_id);
		reginscene.set_family_officer(get_family_officer());
		if (m_common_info.m_family_id != 0)
		{
			const family_ptr& p_family = family_manager_t::get_family(m_common_info.m_family_id);
			if (NULL != p_family)
			{
				reginscene.set_family_name(p_family->get_name());
			}
			else
			{
				log_error("family[%lu] find name error", m_common_info.m_family_id);
				reginscene.set_family_name("");
			}
		}
		else
		{
			reginscene.set_family_name("");
		}

		reginscene.set_country_id(m_common_info.m_country_id);
		reginscene.set_country_officer(m_common_info.m_country_officer);
	}

	p_server_data->set_troop_id(get_troop_id());

	send_msg_to_game(op_cmd::eg_reg_user_scene_request, reginscene);

	return true;
}

//发送给客户端准备登陆场景时需要的数据
void global_user_t::ready_enter_scene()
{
    // 离线私聊
    if (offline_friend_msg_mgr_t::is_have_offile_chat_msg(m_uid))
    {
        proto::client::ec_batch_personal_chat_notify msg;

        offline_friend_msg_mgr_t::get_offline_chat_and_delete(m_uid, msg);

        send_msg_to_client(op_cmd::ec_batch_personal_chat_notify, msg);
    }

    // 约战
    {
        challenge_manager_t::notice_single_challenge(m_uid);
    }

	//检查全局邮件
	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid( m_uid );
	if (p_user)
		global_mail_manager_t::check_user_new(p_user);

	if (m_mail_data.is_have_new_mail())
	{
		red_point_manager_t::red_point_notify(m_uid, proto::common::RED_POINT_TYPE::RED_POINT_MAIL);
	}

	red_point_manager_t::check_goods_point(m_uid);

	//离线数据下发
	offline_role_manager_t::online(m_uid);

	// 国家预告下发
	if (country_mgr_t::get_country_period() != proto::common::country_period_hegemony) {
		proto::client::ec_country_open_notify ntf;
		ntf.set_period_type(country_mgr_t::get_country_period());
		ntf.set_start_time(country_mgr_t::get_create_countdown());
		send_msg_to_client(op_cmd::ec_country_open_notify, ntf);
	}
}

void global_user_t::on_logout()
{
	// 已经是下线状态的不用重复设
	if (m_online_state == common::ROLE_ONLINE_STATE_LOGOUT)
	{
		return;
	}

	ROLE_ONLINE_STATE old_state = m_online_state;
	// 设置离线
	m_online_state = common::ROLE_ONLINE_STATE_LOGOUT;

	// 直接退出的处理
	if (old_state == ROLE_ONLINE_STATE_ONLINE)
	{
		uint32_t logout_time = common::time_util_t::now_time();
		set_last_logout_time(logout_time);

		update_online_state();
	}

	if (get_troop_id() != 0) global_troop_manager::quit_troop(shared_from_this());

	clear_data();
}

bool global_user_t::is_in_king_city()
{
	auto king_city = GET_CONF(Comprehensive, comprehensive_common::imperial_city_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(king_city);
	
	return m_common_info.m_map_tid == val;
}

void global_user_t::clear_data()
{
	m_friend_mgr.clear_all();
	m_mail_data.clear();
	m_shop_goods_data.clear();
}

void global_user_t::set_scene_id(uint64_t scene_id, uint32_t map_tid)
{
	MapTable* p_last_map = GET_CONF(MapTable, m_common_info.m_map_tid);
	if (NULL != p_last_map && p_last_map->scene_type() == proto::server::map_type_static && m_common_info.m_last_map_tid != m_common_info.m_map_tid)
	{
		m_common_info.m_last_map_tid = m_common_info.m_map_tid;

		// 场景变化的一些操作
		global_user_data_mgr_t::set_last_map_tid(m_uid, m_common_info.m_last_map_tid);
	}

	if (m_common_info.m_scene_id != scene_id)
	{
		WRITE_SCENE_LOG_TO_DB(m_uid, m_common_info.m_map_tid, map_tid, common::log_enum::source_type_scene_jump);

		m_common_info.m_scene_id = scene_id;
		m_common_info.m_map_tid = map_tid;

		// 场景变化的一些操作
		global_user_data_mgr_t::set_scene_id(m_uid, m_common_info.m_scene_id, m_common_info.m_map_tid);
	}
}

void global_user_t::sync_scene_info(uint64_t scene_id, uint32_t map_tid)
{
	set_scene_id(scene_id, map_tid);

	if (is_in_king_city())
	{
		challenge_manager_t::check_ready(m_uid);
	}

	const global_scene_ptr& p_scene = global_scene_manager_t::get_scene_by_id(scene_id);
	if (NULL != p_scene)
	{
		p_scene->on_role_enter(shared_from_this());
	}
}

void global_user_t::online(uint32_t online)
{
	if (common::ROLE_ONLINE_STATE_ONLINE == online)
	{
		if (common::ROLE_ONLINE_STATE_OFFLINE == m_online_state)
		{
			if (get_troop_id() > 0)
			{
				troop_ptr troop = global_troop_manager::get_troop(get_troop_id());
				if (NULL != troop)
				{
					troop->update_member_online(shared_from_this(), online);
				}
			}
		}
		//上线
		m_online_state = common::ROLE_ONLINE_STATE_ONLINE;
	}
	else
	{
		set_last_logout_time(common::time_util_t::now_time());

		//离线
		m_online_state = common::ROLE_ONLINE_STATE_OFFLINE;

		if (get_troop_id() > 0)
		{
			troop_ptr troop = global_troop_manager::get_troop(get_troop_id());
			if (NULL != troop)
			{
				troop->update_member_online(shared_from_this(), online);
			}
		}
	}

	update_online_state();
}

void global_user_t::sync_role_date(const proto::server::global_user_data& data)
{
	set_level(data.level());
	set_fighting_value(data.fighting());
	set_hero_plugin(data.hero_plugin());
	set_family_contribution(data.family_contribution());

	switch (data.type())
	{
	case proto::server::login_server_scene:
		{
			on_login();
		}
		break;
	case proto::server::relogin_server_scene:
		{
			online(common::ROLE_ONLINE_STATE_ONLINE);
		}
		break;
	case proto::server::user_change_scene:
	case proto::server::user_enter_dungeon:
	case proto::server::user_troop_jump:
	case proto::server::forced_change_scene:
	case proto::server::center_change_scene:
		{
			sync_scene_info(data.scene_id(), data.map_tid());
		}
		break;
	default:
		log_warn("user[%lu] sync_role_date type[%u] error", m_uid, data.type());
		break;
	}
}

void global_user_t::on_login()
{
	// 加载邮件信息
	m_mail_data.on_login_load();
	// 加载好友
	m_friend_mgr.load_all_relations();
	// 加载商店
	m_shop_goods_data.on_login_load();

	m_online_state = common::ROLE_ONLINE_STATE_ONLINE;

	if (is_in_king_city())
	{
		challenge_manager_t::check_ready(m_uid);
	}

	update_online_state();

	// 每日更新操作
	if (!time_manager_t::check_same_period(common::time_type_enum::time_one_day, m_common_info.m_update_time)) {
		one_day(common::time_util_t::now_time());
	}
}

void global_user_t::set_family_contribution(uint32_t val)
{
	if (m_common_info.m_family_contribution != val) {
		uint32_t old_contribution = m_common_info.m_family_contribution;

		m_common_info.m_family_contribution = val;

		global_user_data_mgr_t::set_family_contribution(m_uid, val);

		if (val > old_contribution && m_common_info.m_family_id != 0) {
			const family_ptr& p_family = family_manager_t::get_family(m_common_info.m_family_id);
			if (NULL != p_family) {
				p_family->add_family_contribution(m_uid, val - old_contribution);
			}
		}
	}
}

void global_user_t::update_online_state()
{
	// 有帮会，通知上线状态
	if (m_common_info.m_family_id != 0)
	{
		const family_ptr& p_family = family_manager_t::get_family(m_common_info.m_family_id);
		if (NULL != p_family)
		{
			p_family->update_member_online_state(shared_from_this());
		}
	}
}

void global_user_t::update_attr()
{
	// 有帮会，通知上线状态
	if (m_common_info.m_family_id != 0)
	{
		const family_ptr& p_family = family_manager_t::get_family(m_common_info.m_family_id);
		if (NULL != p_family)
		{
			p_family->update_member_attr(shared_from_this());
		}
	}

	if (m_common_info.m_country_id != 0 && m_common_info.m_country_officer != 0) {
		country_ptr p_country = country_mgr_t::get_country(m_common_info.m_country_id);
		if (p_country) {
			p_country->update_officer_attr(m_common_info.m_country_officer, shared_from_this());
		}
	}
}

void global_user_t::one_day(uint32_t curr_time)
{
	// 每日更新
	m_common_info.m_update_time = curr_time;

	global_user_data_mgr_t::set_update_time(m_uid, curr_time);
}


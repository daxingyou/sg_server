#include "role_data_manager.hpp"
#include "main/db_server.hpp"
#include "main/db_server_fwd.hpp"
#include "redis_client.hpp"
#include "config/config_manager.hpp"
#include "common/global_id.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Hero.tbls.h"
#include "tblh/HeroJobTransfer.tbls.h"
#include "utility.hpp"
#include <boost/regex.hpp>
#include "utility.hpp"
#include "redis_data_manager.hpp"

std::set<std::string> role_data_manager::m_temp_name_occupancy;

role_data_manager::role_data_manager()
{
}

role_data_manager::~role_data_manager()
{
}

bool role_data_manager::save_role_data_to_redis(uint64_t uid, const proto::common::role_data& ud)
{
	REDIS_BATCH_START
	redis_client_t::set_uint32("role", "gm", uid, ud.gm());
	redis_client_t::set_uint32("role", "last_login_time", uid, ud.last_login_time());
    redis_client_t::set_uint32("role", "last_logout_time", uid, ud.last_logout_time());
    redis_client_t::set_uint32("role", "continue_login_days", uid, ud.continue_login_days());
    redis_client_t::set_uint32("role", "cumulative_login_days", uid, ud.cumulative_login_days());
	redis_client_t::set_uint32("role", "update_day_time", uid, ud.update_day_time());
	redis_client_t::set_uint32("role", "update_week_time", uid, ud.update_week_time());

	redis_client_t::set_uint32("role", "map_tid", uid, ud.map_tid());
	const proto::common::object_common_info& common_info = ud.obj_info();
	redis_client_t::set_float("role", "pos_x", uid, common_info.current_x());
	redis_client_t::set_float("role", "pos_y", uid, common_info.current_y());
	redis_client_t::set_float("role", "dir", uid, common_info.direction());
	const proto::common::last_static_map& last_map = ud.last_map();
	redis_client_t::set_uint32("role", "last_map_tid", uid, last_map.last_map_tid());
	redis_client_t::set_float("role", "last_pos_x", uid, last_map.last_pos_x());
	redis_client_t::set_float("role", "last_pos_y", uid, last_map.last_pos_y());
	redis_client_t::set_float("role", "last_dir", uid, last_map.last_dir());

	const proto::common::personal_info& per_info = ud.per_info();
	redis_client_t::set_uint32("role", "fighting", uid, ud.fighting());
	redis_client_t::set_uint32("role", "city_id", uid, per_info.city_id());
	redis_client_t::set_uint32("role", "garrison_city", uid, per_info.garrison_city());
	redis_client_t::set_uint32("role", "offline_reward_calc_time", uid, per_info.offline_reward_calc_time());
	redis_client_t::set_uint32("role", "luck_gift_gold", uid, per_info.luck_gift_gold());
	redis_client_t::set_uint64("role", "hero_uid", uid, string_util_t::string_to_uint64(per_info.hero_uid()));

	const proto::common::task_type_data& ttd = ud.ttd();
	std::ostringstream ss;
	for (auto ttc : ttd.type_count())
	{
		ss << ttc.type() << ":" << ttc.count() << ";";
	}
	std::string task_type_info = ss.str().substr(0, ss.str().length() - 1);
	redis_client_t::set_string("role", "task_type_data", uid, task_type_info);
	redis_client_t::set_uint32("role", "exp", uid, ud.exp());
	redis_client_t::set_uint32("role", "level", uid, ud.level());
	redis_client_t::set_uint32("role", "plugin", uid, per_info.plugin());
	redis_client_t::set_uint64("role", "function_code", uid, common::string_util_t::string_2_to_uint64(ud.function_code()));
	redis_client_t::set_uint32("role", "npc_code", uid, ud.npc_code());
	redis_client_t::set_uint32("role", "collect_code", uid, ud.collect_code());
	redis_client_t::set_uint32("role", "head_frame", uid, per_info.head_frame());
	REDIS_BATCH_DONE
	// role_ex
	save_role_ex_to_redis(uid, ud);
	// role_money
	save_role_money_to_redis(uid, ud);
	// role_fight
	save_role_fight_to_redis(uid, ud);

	// interior
	//save_interior_data_to_redis(uid, ud.interior());
	return true;
}

bool role_data_manager::load_role_data_from_redis(uint64_t uid, proto::common::role_data* ud)
{
	if (ud == NULL)
	{
		log_error("load_role_data_from_redis [%lu] ud == null", uid);
		return false;
	}

	std::string name = redis_client_t::get_string("role", "name", uid);
	if (name.empty())
		return false;

	ud->set_uid(string_util_t::uint64_to_string(uid));
	ud->set_name(name);
	ud->set_gm(redis_client_t::get_uint32("role", "gm", uid));
	ud->set_last_login_time(redis_client_t::get_uint32("role", "last_login_time", uid));
    ud->set_last_logout_time(redis_client_t::get_uint32("role", "last_logout_time", uid));
    ud->set_continue_login_days(redis_client_t::get_uint32("role", "continue_login_days", uid));
    ud->set_cumulative_login_days(redis_client_t::get_uint32("role", "cumulative_login_days", uid));
	ud->set_update_day_time(redis_client_t::get_uint32("role", "update_day_time", uid));
	ud->set_update_week_time(redis_client_t::get_uint32("role", "update_week_time", uid));
	ud->set_role_type(redis_client_t::get_uint32("role", "role_type", uid));
	ud->set_channel_id(redis_client_t::get_uint32("role", "channel_id", uid));

	ud->set_map_tid(redis_client_t::get_uint32("role", "map_tid", uid));
	proto::common::object_common_info* common_info = ud->mutable_obj_info();
	if (NULL != common_info)
	{
		common_info->set_current_x(redis_client_t::get_float("role", "pos_x", uid));
		common_info->set_current_y(redis_client_t::get_float("role", "pos_y", uid));
		common_info->set_direction(redis_client_t::get_float("role", "dir", uid));
	}

	proto::common::last_static_map* last_map = ud->mutable_last_map();
	if (NULL != last_map)
	{
		last_map->set_last_map_tid(redis_client_t::get_uint32("role", "last_map_tid", uid));
		last_map->set_last_pos_x(redis_client_t::get_float("role", "last_pos_x", uid));
		last_map->set_last_pos_y(redis_client_t::get_float("role", "last_pos_y", uid));
		last_map->set_last_dir(redis_client_t::get_float("role", "last_dir", uid));
	}

    ud->set_create_time(redis_client_t::get_uint32("role", "create_time", uid));
    ud->set_fighting(redis_client_t::get_uint32("role", "fighting", uid));
    ud->set_exp(redis_client_t::get_uint32("role", "exp", uid));
    ud->set_level(redis_client_t::get_uint32("role", "level", uid));

	uint64_t function_code = redis_client_t::get_uint64("role", "function_code", uid);
	ud->set_function_code(string_util_t::uint64_to_2_string( function_code ));

	ud->set_npc_code(redis_client_t::get_uint32("role", "npc_code", uid));
	ud->set_collect_code(redis_client_t::get_uint32("role", "collect_code", uid));

	proto::common::personal_info* per_info = ud->mutable_per_info();

	per_info->set_city_id(redis_client_t::get_uint32("role", "city_id", uid));
	per_info->set_garrison_city(redis_client_t::get_uint32("role", "garrison_city", uid));
	per_info->set_offline_reward_calc_time(redis_client_t::get_uint32("role", "offline_reward_calc_time", uid));
	per_info->set_luck_gift_gold(redis_client_t::get_uint32("role", "luck_gift_gold", uid));
	per_info->set_hero_uid(string_util_t::uint64_to_string(redis_client_t::get_uint64("role", "hero_uid", uid)));
	per_info->set_head_frame(redis_client_t::get_uint32("role", "head_frame", uid));
	std::string task_type_info = redis_client_t::get_string("role", "task_type_data", uid);
	std::vector<int32_t> task_type_info_vec;
	string_util_t::split<int32_t>(task_type_info, task_type_info_vec, ":;");
	if (task_type_info_vec.size() % 2 != 0)
	{
		log_error("user[%lu] load task type data error task_type_info is wrong", uid);
	}
	else
	{
		for (size_t i = 0; i < task_type_info_vec.size();)
		{
			proto::common::task_type_count* ttc = ud->mutable_ttd()->add_type_count();
			if (NULL == ttc)
			{
				continue;
			}

			ttc->set_type(task_type_info_vec[i++]);
			ttc->set_count(task_type_info_vec[i++]);
		}
	}
	
	// role_ex
	load_role_ex_from_redis(uid, ud);
	// role_money
	load_role_money_from_redis(uid, ud);
	// role_fight
	load_role_fight_from_redis(uid, ud);

	// interior
	//load_interior_data_from_redis(uid, ud->mutable_interior());
	return true;
}

uint32_t role_data_manager::check_new_role(uint64_t uid, uint32_t gate_id, const std::string& name, uint32_t role_type, int32_t create_time)
{
    if (redis_client_t::is_hash_exist("role", uid))
    {
        log_error("create role error: already has the role[%lu]", uid);
        return common::errcode_enum::notice_account_is_already_have_role;
    }

	if (role_type < 1 || role_type > 4)
	{
		log_error("user[%lu] role_type[%d] invalid", uid, role_type);
		return common::errcode_enum::notice_account_role_type_error;
	}

	uint32_t ec = role_data_manager::check_nickname_valid(name);
	if (ec > 0)
	{
		log_error("user[%lu] check_nickname_valid[%s] invalid", uid, name.c_str());
		return ec;
	}

	// 临时占坑
	m_temp_name_occupancy.insert(name);

	// 去account注册
	proto::server::dn_create_role_request req;
	req.set_account_id(user_uid_generator::get_account_id(uid));
	req.set_server_id(env::xml->srv_group.group_id);
	req.set_gate_id(gate_id);
	proto::common::account_role_data *p_role_data = req.mutable_role_data();
	p_role_data->set_name(name);
	p_role_data->set_species(role_type);
	p_role_data->set_create_time(create_time);
	p_role_data->set_level(1);
	p_role_data->set_plugin(get_plugin(role_type));
	env::server->send_msg_to_account(op_cmd::dn_create_role_request, uid, req);

	return common::errcode_enum::error_ok;
}

void role_data_manager::create_new_role(uint64_t uid, const proto::common::account_role_data& data)
{
	REDIS_BATCH_START
		redis_client_t::set_string("role", "name", uid, data.name());
#ifdef OPEN_GM_OP
		redis_client_t::set_uint32("role", "gm", uid, 1);
#else
		redis_client_t::set_uint32("role", "gm", uid, 0);
#endif
		redis_client_t::set_uint32("role", "create_time", uid, data.create_time());
		redis_client_t::set_uint32("role", "level", uid, data.level());
		redis_client_t::set_uint32("role", "role_type", uid, data.species());
		redis_client_t::set_uint32("role", "channel_id", uid, data.channel_id());

		redis_client_t::set_string("global_user", "name", uid, data.name());
		redis_client_t::set_uint32("global_user", "create_time", uid, data.create_time());
		redis_client_t::set_uint32("global_user", "level", uid, 1);
		redis_client_t::set_uint32("global_user", "fighting", uid, 0);
		redis_client_t::set_uint32("global_user", "update_time", uid, common::time_util_t::now_time());
		redis_client_t::set_uint32("global_user", "channel_id", uid, data.channel_id());

		std::string did_str = user_did_generator::gen_user_did(uid);
		uint32_t did = string_util_t::string_to_uint32(did_str);
		redis_client_t::set_uint32("global_user", "did", uid, did);
		redis_client_t::set_uint32("global_user", "rt", uid, common::time_util_t::now_time()); // 设置数据时间
	REDIS_BATCH_DONE

	del_temp_name(data.name());
}

void role_data_manager::del_temp_name(const std::string& temp_name)
{
	// 删除临时占位的名字
	std::set<std::string>::iterator citr = m_temp_name_occupancy.find(temp_name);
	if (citr != m_temp_name_occupancy.end())
	{
		m_temp_name_occupancy.erase(citr);
	}
}

uint32_t role_data_manager::get_plugin(uint32_t role_type)
{
	auto tansfer = GET_CONF(HeroJobTransfer, role_type);
	if (tansfer == NULL)
	{
		log_error("tansfer == NULL role_type[%u]!", role_type);
		return 100;
	}
	uint32_t hero_id = tansfer->initial_id();
	auto p_hero_conf = GET_CONF(Hero, hero_id);
	if (NULL == p_hero_conf)
	{
		log_error("NULL == p_hero_conf[%d]", hero_id);
		return 100;
	}
	return p_hero_conf->default_model();
}

void role_data_manager::change_name(uint64_t uid, const proto::server::gd_change_name_request& msg, proto::server::dg_change_name_reply& reply)
{
	std::string name = msg.new_name();
	uint32_t reply_code = check_nickname_valid(name);
	if (reply_code == errcode_enum::error_ok)
	{
		//名字检测合法 改名
		redis_client_t::set_string("role", "name", uid, name);
		
		//通知account
		redis_data_manager_t::handle_syn_role_data(uid);
		//通知center
		proto::server::de_change_name_notify ntf;
		ntf.set_new_name(name);
		env::server->send_msg_to_center(op_cmd::de_change_name_notify, uid, ntf);
	}
	reply.set_new_name(name);
	reply.set_reply_code(reply_code);
	reply.set_cost(msg.cost());
}

uint32_t role_data_manager::check_nickname_valid(const std::string& name)
{
	// 检查特殊字符
	if (!common::string_util_t::is_no_special_char(name))
	{
		log_error("nickname[%s] has special char!", name.c_str());
		return errcode_enum::sys_notice_not_chinese_word;
	}

	uint32_t name_size = common::string_util_t::get_char_size(name);

	// 检查长度
	if (name_size < 2)
	{
		log_error("nickname[%s] length[%u] error!", name.c_str(), name_size);
		return errcode_enum::notice_name_number_min;
	}

	if (name_size > 6)
	{
		log_error("nickname[%s] length[%d] error!", name.c_str(), name_size);
		return errcode_enum::notice_name_number_max;
	}

	// 检测临时占位的名字
	std::set<std::string>::iterator itr = m_temp_name_occupancy.find(name);
	if (itr != m_temp_name_occupancy.end())
	{
		ROLE_LOG("nickname[%s] is repeated!", name.c_str());
		return errcode_enum::notice_name_repeat;
	}

	// 检查重命名
	if (redis_client_t::is_set_exist("global_user", "name", name))
	{
        ROLE_LOG("nickname[%s] is repeated!", name.c_str());
		return errcode_enum::notice_name_repeat;
	}

	// 检查敏感字
	/*for (size_t i = 0;i < dirtyname_config_manager_t::get_size(); ++i)
	{
	if (std::string::npos != name.find(dirtyname_config_manager_t::m_name_config_list[i].dirtyname.c_str()))
	{
	log_error("nickname[%s] has sensitive word!", name.c_str());
	return 4;
	}
	}*/

	return 0;
}

//bool role_data_manager::save_interior_data_to_redis(uint64_t uid, const proto::common::interior_info& ii)
//{
//	redis_client_t::set_uint32("interior", "level", uid, ii.level());
//	redis_client_t::set_uint32("interior", "task_id", uid, ii.task_id());
//	redis_client_t::set_uint32("interior", "star_level", uid, ii.star_level());
//	redis_client_t::set_uint32("interior", "start_time", uid, ii.start_time());
//	redis_client_t::set_uint32("interior", "span_time", uid, ii.span_time());
//	redis_client_t::set_uint32("interior", "last_reward_time", uid, ii.last_reward_time());
//	std::ostringstream ss;
//	for (int i = 0; i < ii.hero_id_list_size(); ++i)
//	{
//		ss << ii.hero_id_list(i) << "$";
//	}
//	std::string heros = ss.str().substr(0, ss.str().length() - 1);
//	redis_client_t::set_string("interior", "hero_id_list", uid, heros);
//	redis_client_t::set_uint32("interior", "left_times", uid, ii.left_times());
//	redis_client_t::set_uint32("interior", "is_start", uid, ii.is_start());
//
//	return true;
//}
//
//bool role_data_manager::load_interior_data_from_redis(uint64_t uid, proto::common::interior_info* ii)
//{
//	if (ii == NULL)
//	{
//		log_error("load_interior_data_from_redis [%lu] ud == null", uid);
//		return false;
//	}
//
//	ii->set_level(redis_client_t::get_uint32("interior", "level", uid));
//	ii->set_task_id(redis_client_t::get_uint32("interior", "task_id", uid));
//	ii->set_star_level(redis_client_t::get_uint32("interior", "star_level", uid));
//	ii->set_start_time(redis_client_t::get_uint32("interior", "start_time", uid));
//	ii->set_span_time(redis_client_t::get_uint32("interior", "span_time", uid));
//	ii->set_last_reward_time(redis_client_t::get_uint32("interior", "last_reward_time", uid));
//	std::string heros = redis_client_t::get_string("interior", "hero_id_list", uid);
//	std::vector<uint32_t> vec_hero_id;
//	string_util_t::ParseIds(heros, vec_hero_id, '$');
//	for (auto hero_id : vec_hero_id)
//	{
//		ii->add_hero_id_list(hero_id);
//	}
//	ii->set_left_times(redis_client_t::get_uint32("interior", "left_times", uid));
//	ii->set_is_start(redis_client_t::get_uint32("interior", "is_start", uid));
//
//	return true;
//}

bool role_data_manager::is_role_exist(uint64_t uid)
{
	return redis_client_t::is_hash_exist("role", uid);
}

bool role_data_manager::save_role_ex_to_redis(uint64_t uid, const proto::common::role_data& ud)
{
	REDIS_BATCH_START
	const proto::common::personal_info& per_info = ud.per_info();
	redis_client_t::set_uint32("role_ex", "cur_form_id", uid, per_info.cur_form_id());
	redis_client_t::set_uint32("role_ex", "equip_smelt_value", uid, per_info.equip_smelt_value());
	std::ostringstream sss;
	for (auto box_id : per_info.equip_smelt_box())
	{
		if (!sss.str().empty())
		{
			sss << "$";
		}

		sss << box_id;
	}

	redis_client_t::set_string("role_ex", "equip_smelt_box", uid, sss.str());
	const proto::common::formation_data& fd = ud.formation();
	std::ostringstream ss;
	for (const proto::common::formation_single& fs : fd.formation_list())
	{
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << fs.formation_index() << "|";

		std::ostringstream oss;
		for (const proto::common::formation_pair& fp : fs.formations())
		{
			if (!oss.str().empty())
			{
				oss << ";";
			}

			oss << fp.pos() << ":" << fp.unique_id();
		}

		ss << oss.str();

		ss << "|" << fs.tactic_id();
	}

	redis_client_t::set_string("role_ex", "formation", uid, ss.str());

    const proto::common::new_role_guide_data& nrgd = ud.guide_data();
    std::string guide_list_str = "";
    for (const proto::common::new_role_guide_single& nrgs : nrgd.guide_list())
    {
        std::string guide_str = boost::lexical_cast<std::string>(nrgs.tid()) + ':' + 
								boost::lexical_cast<std::string>(nrgs.status()) + ':' + 
								boost::lexical_cast<std::string>(nrgs.event_type());
        string_util_t::combine(guide_str, guide_list_str, "|");
    }
    redis_client_t::set_string("role_ex", "guide", uid, guide_list_str);

	std::ostringstream ss_tactic;
	for (auto ts : ud.tactic().tactics())
	{
		ss_tactic << ts.tactic_id() << ":" << ts.tactic_level() << "$";
	}
	std::string tactic_info = ss_tactic.str().substr(0, ss_tactic.str().length() - 1);
	redis_client_t::set_string("role_ex", "tactic", uid, tactic_info);

	std::ostringstream ss_role_buff;
	for (auto rb : ud.role_buff().role_buffs())
	{
		ss_role_buff << rb.buff_id() << ":" << rb.remain_time() << "$";
	}
	std::string role_buff_info = ss_role_buff.str().substr(0, ss_role_buff.str().length() - 1);
	redis_client_t::set_string("role_ex", "buff", uid, role_buff_info);

	const proto::common::role_trade_info& trade_info = ud.trade_info();
	redis_client_t::set_uint32("role_ex", "trade_point", uid, trade_info.trade_point());
	redis_client_t::set_uint32("role_ex", "trade_last_reset_buy_num_time", uid, trade_info.last_reset_buy_num_time());
	redis_client_t::set_uint32("role_ex", "trade_week_total_gain", uid, trade_info.total_week_gain());
	
	const proto::common::page_data& page_data = ud.page_list();
	std::string page_list_str = "" , page_task_list_str = "",page_elite_list_str = ""; 
	for (int i =0; i < page_data.pass_page_list_size(); ++i)
	{
		string_util_t::combine(page_data.pass_page_list(i), page_list_str, "|");
	}
	redis_client_t::set_string("role_ex", "page_pass_list", uid, page_list_str);

	for (const proto::common::page_task_single& it : page_data.task_info())
	{
		std::string page_str = boost::lexical_cast<std::string>(it.id()) + ':' +
			boost::lexical_cast<std::string>(it.status());
		string_util_t::combine(page_str, page_task_list_str, "|");
	}
	redis_client_t::set_string("role_ex", "page_task_pass_list", uid, page_task_list_str);

	const proto::common::page_elite_data& elite_list = page_data.elite_data();
	for (const proto::common::page_elite_single& it : elite_list.pass_elite_list())
	{
		std::string page_str = boost::lexical_cast<std::string>(it.id()) + ':' +
			boost::lexical_cast<std::string>(it.status());
		string_util_t::combine(page_str, page_elite_list_str, "|");
	}
	redis_client_t::set_string("role_ex", "page_elite_pass_list", uid, page_elite_list_str);
	
	std::string page_quick_fight_str;
	for (const proto::common::page_quick_fight_data& it : page_data.quick_data())
	{
		std::string page_str = boost::lexical_cast<std::string>(it.id()) + ':' +
			boost::lexical_cast<std::string>(it.num());
		string_util_t::combine(page_str, page_quick_fight_str, "|");
	}
	redis_client_t::set_string("role_ex", "quick_fight_list", uid, page_quick_fight_str);

	std::string page_reset_fight_str;
	for (const proto::common::page_reset_quick_fight_data& it : page_data.reset_data())
	{
		std::string page_str = boost::lexical_cast<std::string>(it.id()) + ':' +
			boost::lexical_cast<std::string>(it.num());
		string_util_t::combine(page_str, page_reset_fight_str, "|");
	}
	redis_client_t::set_string("role_ex", "reset_quick_fight_list", uid, page_reset_fight_str);

	const proto::common::active_reward_task_data& active_reward_task_list = ud.task_list();
	std::string active_reward_task_list_str = "";
	for (int i = 0; i < active_reward_task_list.task_list_size(); ++i)
	{
		uint32_t task_id = active_reward_task_list.task_list(i);
		if (0 == task_id)
			continue;
		string_util_t::combine(task_id, active_reward_task_list_str, "|");
	}
	redis_client_t::set_string("role_ex", "active_reward_task_list", uid, active_reward_task_list_str);
	redis_client_t::set_uint32("role_ex", "patrol_count", uid, per_info.patrol_count());
	redis_client_t::set_uint32("role_ex", "equip_lucky", uid, per_info.equip_lucky());

	const proto::common::multi_time_data& multi_time_list = ud.multi_time();


	std::string multi_time_list_str = "";
	for (const proto::common::public_struct& time : multi_time_list.time_long_list())
	{
		std::string time_str = boost::lexical_cast<std::string>(time.type()) + ':' +
			boost::lexical_cast<std::string>(time.param());
		string_util_t::combine(time_str, multi_time_list_str, "|");
	}
	redis_client_t::set_string("role_ex", "time_long_multi_list", uid, multi_time_list_str);
	redis_client_t::set_uint32("role_ex", "multi_state", uid, ud.multi_time().state());
	redis_client_t::set_uint32("role_ex", "mode_state", uid, ud.mode_state());

	redis_client_t::set_uint32("role_ex", "lifeforce_label", uid, ud.life_data().label());


	const proto::common::head_data& head = ud.head_frame_info();
	std::string head_list_str = "",head_str = "",frame_str = "";
	
	for (int i = 0; i < head.head_list_size();i++)
	{
		head_str += boost::lexical_cast<std::string>(head.head_list(i)) + ':';
	}
	string_util_t::combine(head_str, head_list_str, "|");

	for (int i = 0; i < head.frame_list_size(); i++)
	{
		frame_str += boost::lexical_cast<std::string>(head.frame_list(i)) + ':';
	}
	head_list_str += frame_str;
	redis_client_t::set_string("role_ex", "head_frame_list", uid, head_list_str);
	redis_client_t::set_uint32("role_ex", "wusheng_score", uid, per_info.wusheng_score());
	redis_client_t::set_uint32("role_ex", "wusheng_score_today", uid, per_info.wusheng_score_today());
	redis_client_t::set_uint32("role_ex", "wusheng_lucky", uid, per_info.wusheng_lucky());
	redis_client_t::set_uint32("role_ex", "wusheng_today_buy", uid, per_info.wusheng_today_buy());
	std::ostringstream oss_career;
	for (auto career_id : per_info.career_id())
	{
		if (!oss_career.str().empty())
		{
			oss_career << "$";
		}

		oss_career << career_id;
	}
	redis_client_t::set_string("role_ex", "career_id", uid, oss_career.str());
	redis_client_t::set_uint32("role_ex", "change_name", uid, per_info.change_name());

	std::ostringstream oss_title;
	for (auto title_id : per_info.title_id())
	{
		if (!oss_title.str().empty())
		{
			oss_title << "$";
		}

		oss_title << title_id;
	}
	redis_client_t::set_string("role_ex", "title_id", uid, oss_title.str());
	redis_client_t::set_uint32("role_ex", "cur_title", uid, per_info.cur_title());

	std::ostringstream oss_handbook;
	for (auto handbook : per_info.handbook())
	{
		if (!oss_handbook.str().empty())
		{
			oss_handbook << "$";
		}

		oss_handbook << handbook;
	}
	redis_client_t::set_string("role_ex", "handbook", uid, oss_handbook.str());
	REDIS_BATCH_DONE
	
	return true;
}

bool role_data_manager::load_role_ex_from_redis(uint64_t uid, proto::common::role_data* ud)
{
	if (NULL == ud)
	{
		return false;
	}

	proto::common::personal_info* per_info = ud->mutable_per_info();
	per_info->set_cur_form_id(redis_client_t::get_uint32("role_ex", "cur_form_id", uid));
	per_info->set_equip_smelt_value(redis_client_t::get_uint32("role_ex", "equip_smelt_value", uid));
	std::string box_info = redis_client_t::get_string("role_ex", "equip_smelt_box", uid);
	if (!box_info.empty())
	{
		std::vector<std::string> boxes;
		string_util_t::split<std::string>(box_info, boxes, "$");
		for (auto box : boxes)
		{
			uint32_t equip_smelt_value = boost::lexical_cast<uint32_t>(box);
			per_info->add_equip_smelt_box(equip_smelt_value);
		}
	}

	std::string formations_info = redis_client_t::get_string("role_ex", "formation", uid);
	if (!formations_info.empty())
	{
		std::vector<std::string> formations;
		string_util_t::split<std::string>(formations_info, formations, "$");
		for (auto formation : formations)
		{
			std::vector<std::string> formation_vec;
			string_util_t::split<std::string>(formation, formation_vec, "|");
			if (formation_vec.size() < 2)
			{
				log_error("load formation fail uid[%lu] data[%s]", uid, formation.c_str());
				continue;
			}

			proto::common::formation_data* fd = ud->mutable_formation();
			proto::common::formation_single* fs = fd->add_formation_list();
			if (NULL == fs)
			{
				continue;
			}

			fs->set_formation_index(boost::lexical_cast<uint32_t>(formation_vec[0]));
			std::vector<std::string> formation_pair_vec;
			string_util_t::split<std::string>(formation_vec[1], formation_pair_vec, ":;");
			if (formation_pair_vec.size() % 2 != 0)
			{
				log_error("load formation pair fail uid[%lu] data[%s]", uid, formation_vec[1].c_str());
				continue;
			}

			for (size_t i = 0; i < formation_pair_vec.size();)
			{
				proto::common::formation_pair* fp = fs->add_formations();
				if (NULL == fp)
				{
					continue;
				}

				fp->set_pos(boost::lexical_cast<uint32_t>(formation_pair_vec[i++]));
				fp->set_unique_id(formation_pair_vec[i++]);
			}

			if (formation_vec.size() > 2)
			{
				fs->set_tactic_id(boost::lexical_cast<uint32_t>(formation_vec[2]));
			}
		}
	}


    std::string guide_list_str = redis_client_t::get_string("role_ex", "guide", uid);
    if (!guide_list_str.empty())
    {
        proto::common::new_role_guide_data* nrgd = ud->mutable_guide_data();
        std::vector<std::string> guide_list;
        string_util_t::split<std::string>(guide_list_str, guide_list, "|");
		std::vector<uint32_t> param_list;
        for (uint32_t i = 0; i < (uint32_t)guide_list.size(); ++i)
        {

			string_util_t::split<uint32_t>(guide_list[i], param_list, ":");

			if (param_list.empty())
				continue;
			uint32_t tid = param_list.at(0);
			uint32_t status = param_list.at(1);
			uint32_t event_type = param_list.at(2);
			proto::common::new_role_guide_single* nrgs = nrgd->add_guide_list();
			nrgs->set_tid(tid);
			nrgs->set_status(status);
			nrgs->set_event_type(event_type);
			param_list.clear();
			/* std::string::size_type pos = guide_list[i].find_first_of(':');
			 if (pos != std::string::npos)
			 {
				 uint32_t tid = string_util_t::string_to_uint32(guide_list[i].substr(0, pos));
				 uint32_t status = string_util_t::string_to_uint32(guide_list[i].substr(pos + 1));
				 proto::common::new_role_guide_single* nrgs = nrgd->add_guide_list();
				 nrgs->set_tid(tid);
				 nrgs->set_status(status);
			 }*/
        }
    }
	std::string tactic_info = redis_client_t::get_string("role_ex", "tactic", uid);
	std::vector<int32_t> tactic_info_vec;
	string_util_t::split<int32_t>(tactic_info, tactic_info_vec, ":$");
	if (tactic_info_vec.size() % 2 != 0)
	{
		log_error("user[%lu] load tactic info error tactic_info is wrong", uid);
		return false;
	}
	for (size_t i = 0; i < tactic_info_vec.size();)
	{
		proto::common::tactic_single* ts = ud->mutable_tactic()->add_tactics();
		ts->set_tactic_id(tactic_info_vec[i++]);
		ts->set_tactic_level(tactic_info_vec[i++]);
	}

	std::string role_buff_info = redis_client_t::get_string("role_ex", "buff", uid);
	std::vector<int32_t> role_buff_info_vec;
	string_util_t::split<int32_t>(role_buff_info, role_buff_info_vec, ":$");
	if (role_buff_info_vec.size() % 2 != 0)
	{
		log_error("user[%lu] load buff info error role_buff_info is wrong", uid);
		return false;
	}
	for (size_t i = 0; i < role_buff_info_vec.size();)
	{
		proto::common::role_buff_single* rbs = ud->mutable_role_buff()->add_role_buffs();
		rbs->set_buff_id(role_buff_info_vec[i++]);
		rbs->set_remain_time(role_buff_info_vec[i++]);
	}

	proto::common::role_trade_info* trade_info = ud->mutable_trade_info();
	trade_info->set_trade_point(redis_client_t::get_uint32("role_ex", "trade_point", uid));
	trade_info->set_last_reset_buy_num_time(redis_client_t::get_uint32("role_ex", "trade_last_reset_buy_num_time", uid));
	trade_info->set_total_week_gain(redis_client_t::get_uint32("role_ex", "trade_week_total_gain", uid));

	std::string page_pass_list_str = redis_client_t::get_string("role_ex", "page_pass_list", uid);
	std::string page_task_pass_list_str = redis_client_t::get_string("role_ex", "page_task_pass_list", uid);
	std::string page_elite_pass_list_str = redis_client_t::get_string("role_ex", "page_elite_pass_list", uid);
	std::string page_quick_fight_list_str = redis_client_t::get_string("role_ex", "quick_fight_list", uid);
	std::string page_reset_fight_list_str = redis_client_t::get_string("role_ex", "reset_quick_fight_list", uid);

	if (!page_pass_list_str.empty())
	{
		proto::common::page_data* page_data = ud->mutable_page_list();
		std::vector<uint32_t> page_list;
		string_util_t::split<uint32_t>(page_pass_list_str, page_list, "|");
		for (auto& it : page_list)
		{
			page_data->add_pass_page_list(it);
		}
	}

	if (!page_task_pass_list_str.empty())
	{
		proto::common::page_data* page_data = ud->mutable_page_list();
		std::vector<std::string> page_list;
		string_util_t::split<std::string>(page_task_pass_list_str, page_list, "|");
		std::vector<uint32_t> param_list;
		for (uint32_t i = 0; i < (uint32_t)page_list.size(); ++i)
		{
			string_util_t::split<uint32_t>(page_list[i], param_list, ":");
			if (param_list.empty())
				continue;
			const uint32_t tid = param_list.at(0);
			const uint32_t status = param_list.at(1);
			proto::common::page_task_single* single = page_data->add_task_info();
			single->set_id(tid);
			single->set_status(status);
			param_list.clear();
		}
	}

	if (!page_elite_pass_list_str.empty())
	{
		proto::common::page_data* page_data = ud->mutable_page_list();
		proto::common::page_elite_data* page_elite = page_data->mutable_elite_data();
		std::vector<std::string> page_list;
		string_util_t::split<std::string>(page_elite_pass_list_str, page_list, "|");
		std::vector<uint32_t> param_list;
		for (uint32_t i = 0; i < (uint32_t)page_list.size(); ++i)
		{
			string_util_t::split<uint32_t>(page_list[i], param_list, ":");
			if (param_list.empty())
				continue;
			const uint32_t tid = param_list.at(0);
			const uint32_t status = param_list.at(1);
			proto::common::page_elite_single* single = page_elite->add_pass_elite_list();
			single->set_id(tid);
			single->set_status(status);
			param_list.clear();
		}
	}

	if (!page_quick_fight_list_str.empty())
	{
		proto::common::page_data* page_data = ud->mutable_page_list();
		std::vector<std::string> page_list;
		string_util_t::split<std::string>(page_quick_fight_list_str, page_list, "|");
		std::vector<uint32_t> param_list;
		for (uint32_t i = 0; i < (uint32_t)page_list.size(); ++i)
		{
			string_util_t::split<uint32_t>(page_list[i], param_list, ":");
			if (param_list.empty())
				continue;
			const uint32_t tid = param_list.at(0);
			const uint32_t num = param_list.at(1);
			proto::common::page_quick_fight_data* single = page_data->add_quick_data();
			single->set_id(tid);
			single->set_num(num);
			param_list.clear();
		}
	}

	if (!page_reset_fight_list_str.empty())
	{
		proto::common::page_data* page_data = ud->mutable_page_list();
		std::vector<std::string> page_list;
		string_util_t::split<std::string>(page_reset_fight_list_str, page_list, "|");
		std::vector<uint32_t> param_list;
		for (uint32_t i = 0; i < (uint32_t)page_list.size(); ++i)
		{
			string_util_t::split<uint32_t>(page_list[i], param_list, ":");
			if (param_list.empty())
				continue;
			const uint32_t tid = param_list.at(0);
			const uint32_t num = param_list.at(1);
			proto::common::page_reset_quick_fight_data* single = page_data->add_reset_data();
			single->set_id(tid);
			single->set_num(num);
			param_list.clear();
		}
	}

	std::string active_reward_task_list_str = redis_client_t::get_string("role_ex", "active_reward_task_list", uid);
	if (!active_reward_task_list_str.empty())
	{
		std::vector<uint32_t> task_list;
		string_util_t::split<uint32_t>(active_reward_task_list_str, task_list, "|");
		proto::common::active_reward_task_data* p_active_reward_task_data = ud->mutable_task_list();
		for (auto& it : task_list)
		{
			p_active_reward_task_data->add_task_list(it);
		}
	}
	per_info->set_patrol_count(redis_client_t::get_uint32("role_ex", "patrol_count", uid));
	per_info->set_equip_lucky(redis_client_t::get_uint32("role_ex", "equip_lucky", uid));
	proto::common::multi_time_data* p_multi_time_data = ud->mutable_multi_time();
	std::string multi_time_list_str = redis_client_t::get_string("role_ex", "time_long_multi_list", uid);
	if (!multi_time_list_str.empty())
	{
		std::vector<std::string> multi_str_list;
		string_util_t::split<std::string>(multi_time_list_str, multi_str_list, "|");
		std::vector<uint32_t> multi_list;
		for (uint32_t i = 0; i < (uint32_t)multi_str_list.size(); ++i)
		{
			string_util_t::split<uint32_t>(multi_str_list[i], multi_list, ":");
			if (multi_list.empty())
				continue;
			const uint32_t type = multi_list.at(0);
			const uint32_t param = multi_list.at(1);
			proto::common::public_struct* p_struct = p_multi_time_data->add_time_long_list();
			p_struct->set_type(type);
			p_struct->set_param(param);
			multi_list.clear();
		}	
	}
	p_multi_time_data->set_state(redis_client_t::get_uint32("role_ex", "multi_state", uid));
	uint32_t mode_state = redis_client_t::get_uint32("role_ex", "mode_state", uid);
	ud->set_mode_state((mode_state == 0 ? 1 : mode_state));

	proto::common::life_info* p_life_info = ud->mutable_life_data();
	p_life_info->set_label(redis_client_t::get_uint32("role_ex", "lifeforce_label", uid));
	per_info->set_wusheng_score(redis_client_t::get_uint32("role_ex", "wusheng_score", uid));
	per_info->set_wusheng_score_today(redis_client_t::get_uint32("role_ex", "wusheng_score_today", uid));
	per_info->set_wusheng_lucky(redis_client_t::get_uint32("role_ex", "wusheng_lucky", uid));
	per_info->set_wusheng_today_buy(redis_client_t::get_uint32("role_ex", "wusheng_today_buy", uid));

	std::string head_frame_list_str = redis_client_t::get_string("role_ex", "head_frame_list", uid);
	if (!head_frame_list_str.empty())
	{
		proto::common::head_data* p_head = ud->mutable_head_frame_info();
		std::vector<std::string> head_frame_list;
		string_util_t::split<std::string>(head_frame_list_str, head_frame_list, "|");
		std::vector<uint32_t> param_list;

		for (uint32_t i = 0; i < (uint32_t)head_frame_list.size(); ++i)
		{
			string_util_t::split<uint32_t>(head_frame_list[i], param_list, ":");

			if (param_list.empty())
				continue;
			if (0 == i)
			{
				for (auto& it : param_list)
				{
					p_head->add_head_list(it);
				}
			} 
			else if(1 == i)
			{
				for (auto& it : param_list)
				{
					p_head->add_frame_list(it);
				}
			}
			
			param_list.clear();
		}
	}
	std::string career_str = redis_client_t::get_string("role_ex", "career_id", uid);
	if (!career_str.empty())
	{
		std::vector<uint32_t> career_ids;
		string_util_t::split<uint32_t>(career_str, career_ids, "$");
		for (auto career_id : career_ids)
		{
			per_info->add_career_id(career_id);
		}
	}
	per_info->set_change_name(redis_client_t::get_uint32("role_ex", "change_name", uid));

	std::string title_str = redis_client_t::get_string("role_ex", "title_id", uid);
	if (!title_str.empty())
	{
		std::vector<uint32_t> title_ids;
		string_util_t::split<uint32_t>(title_str, title_ids, "$");
		for (auto title_id : title_ids)
		{
			per_info->add_title_id(title_id);
		}
	}
	per_info->set_cur_title(redis_client_t::get_uint32("role_ex", "cur_title", uid));

	std::string handbook_str = redis_client_t::get_string("role_ex", "handbook", uid);
	if (!handbook_str.empty())
	{
		std::vector<uint32_t> handbook_ids;
		string_util_t::split<uint32_t>(handbook_str, handbook_ids, "$");
		for (auto handbook : handbook_ids)
		{
			per_info->add_handbook(handbook);
		}
	}
	return true;
}

bool role_data_manager::save_role_money_to_redis(uint64_t uid, const proto::common::role_data& ud)
{
	REDIS_BATCH_START
	const proto::common::personal_info& per_info = ud.per_info();
	redis_client_t::set_uint32("role_money", "yuanbao", uid, per_info.yuanbao());
	redis_client_t::set_uint32("role_money", "silver", uid, per_info.silver());
	redis_client_t::set_uint32("role_money", "copper", uid, per_info.copper());
	redis_client_t::set_uint32("role_money", "errantry", uid, per_info.errantry());
	redis_client_t::set_uint32("role_money", "energy", uid, per_info.energy());
	redis_client_t::set_uint32("role_money", "prestige", uid, per_info.prestige());
	redis_client_t::set_uint32("role_money", "prestige_total", uid, per_info.prestige_total());
	redis_client_t::set_uint32("role_money", "prestige_level", uid, per_info.prestige_level());
	redis_client_t::set_uint32("role_money", "prestige_today", uid, per_info.prestige_today());
	redis_client_t::set_uint32("role_money", "achieve_level", uid, per_info.achieve_level());
	redis_client_t::set_uint32("role_money", "achieve_point", uid, per_info.achieve_point());
	redis_client_t::set_uint32("role_money", "achieve_recv_reward_level", uid, per_info.achieve_recv_reward_level());
	redis_client_t::set_uint32("role_money", "lifeforce", uid, per_info.lifeforce());
	redis_client_t::set_uint32("role_money", "lifestar_level", uid, per_info.lifestar_level());
	redis_client_t::set_uint32("role_money", "lifestate_level", uid, per_info.lifestate_level());
	redis_client_t::set_uint32("role_money", "life_stone", uid, per_info.life_stone());
	redis_client_t::set_uint32("role_money", "herosoul", uid, per_info.herosoul());
	redis_client_t::set_uint32("role_money", "honor", uid, per_info.honor());
	redis_client_t::set_uint32("role_money", "honor_week", uid, per_info.honor_week());
	redis_client_t::set_uint32("role_money", "honor_limit", uid, per_info.honor_limit());
	redis_client_t::set_uint32("role_money", "contribution", uid, per_info.contribution());
	redis_client_t::set_uint32("role_money", "multi_point", uid, per_info.multi_point());
	redis_client_t::set_uint32("role_money", "role_unlock", uid, per_info.role_unlock());
	redis_client_t::set_uint32("role_money", "vigour", uid, per_info.vigour());
	redis_client_t::set_uint32("role_money", "vigour_buy", uid, per_info.vigour_buy());
	redis_client_t::set_uint32("role_money", "vigour_time", uid, per_info.vigour_time());
	redis_client_t::set_uint32("role_money", "friendship_point", uid, per_info.friendship_point());
	REDIS_BATCH_DONE
	return true;
}

bool role_data_manager::load_role_money_from_redis(uint64_t uid, proto::common::role_data* ud)
{
	proto::common::personal_info* per_info = ud->mutable_per_info();
	per_info->set_yuanbao(redis_client_t::get_uint32("role_money", "yuanbao", uid));
	per_info->set_silver(redis_client_t::get_uint32("role_money", "silver", uid));
	per_info->set_copper(redis_client_t::get_uint32("role_money", "copper", uid));
	per_info->set_energy(redis_client_t::get_uint32("role_money", "energy", uid));
	per_info->set_errantry(redis_client_t::get_uint32("role_money", "errantry", uid));
	per_info->set_prestige(redis_client_t::get_uint32("role_money", "prestige", uid));
	per_info->set_prestige_total(redis_client_t::get_uint32("role_money", "prestige_total", uid));
	per_info->set_prestige_level(redis_client_t::get_uint32("role_money", "prestige_level", uid));
	per_info->set_prestige_today(redis_client_t::get_uint32("role_money", "prestige_today", uid));
	per_info->set_achieve_point(redis_client_t::get_uint32("role_money", "achieve_point", uid));
	per_info->set_achieve_level(redis_client_t::get_uint32("role_money", "achieve_level", uid));
	per_info->set_achieve_recv_reward_level(redis_client_t::get_uint32("role_money", "achieve_recv_reward_level", uid));
	per_info->set_lifeforce(redis_client_t::get_uint32("role_money", "lifeforce", uid));
	per_info->set_lifestar_level(redis_client_t::get_uint32("role_money", "lifestar_level", uid));
	per_info->set_lifestate_level(redis_client_t::get_uint32("role_money", "lifestate_level", uid));
	per_info->set_life_stone(redis_client_t::get_uint32("role_money", "life_stone", uid));
	per_info->set_herosoul(redis_client_t::get_uint32("role_money", "herosoul", uid));
	per_info->set_honor(redis_client_t::get_uint32("role_money", "honor", uid));
	per_info->set_honor_week(redis_client_t::get_uint32("role_money", "honor_week", uid));
	per_info->set_honor_limit(redis_client_t::get_uint32("role_money", "honor_limit", uid));
	per_info->set_contribution(redis_client_t::get_uint32("role_money", "contribution", uid));
	per_info->set_multi_point(redis_client_t::get_uint32("role_money", "multi_point", uid));
	per_info->set_role_unlock(redis_client_t::get_uint32("role_money", "role_unlock", uid));
	per_info->set_vigour(redis_client_t::get_uint32("role_money", "vigour", uid));
	per_info->set_vigour_buy(redis_client_t::get_uint32("role_money", "vigour_buy", uid));
	per_info->set_vigour_time(redis_client_t::get_uint32("role_money", "vigour_time", uid));
	per_info->set_friendship_point(redis_client_t::get_uint32("role_money", "friendship_point", uid));
	return true;
}

bool role_data_manager::save_role_fight_to_redis(uint64_t uid, const proto::common::role_data& ud)
{
	REDIS_BATCH_START
	const proto::common::personal_info& per_info = ud.per_info();
	redis_client_t::set_uint32("role_fight", "pk_value_reduce_time", uid, per_info.pk_value_reduce_time());
	redis_client_t::set_uint32("role_fight", "pk_value", uid, per_info.pk_value());
	redis_client_t::set_uint32("role_fight", "pk_count", uid, per_info.pk_count());
	redis_client_t::set_uint32("role_fight", "target_count", uid, per_info.target_count());
	redis_client_t::set_uint32("role_fight", "pk_punishment", uid, per_info.pk_punishment());
	redis_client_t::set_uint64("role_fight", "fight_uid", uid, ud.fight().fight_uid());
	redis_client_t::set_uint32("role_fight", "auto_fight_flag", uid, ud.fight().auto_fight_flag());
	redis_client_t::set_uint32("role_fight", "ignore_lineup_flag", uid, ud.fight().ignore_lineup_flag());

	const proto::common::fight_study_data& fsd = ud.fight().study();
	redis_client_t::set_uint32("role_fight", "fight_study_step", uid, fsd.step());
	redis_client_t::set_uint32("role_fight", "fight_study_step_time", uid, fsd.step_time());
	redis_client_t::set_uint64("role_fight", "fight_study_target", uid, fsd.target());
	redis_client_t::set_uint64("role_fight", "fight_study_attacker", uid, fsd.attacker());
	redis_client_t::set_uint32("role_fight", "fight_study_time", uid, fsd.study_time());
	REDIS_BATCH_DONE
	return true;
}

bool role_data_manager::load_role_fight_from_redis(uint64_t uid, proto::common::role_data* ud)
{
	proto::common::personal_info* per_info = ud->mutable_per_info();
	per_info->set_target_count(redis_client_t::get_uint32("role_fight", "target_count", uid));
	per_info->set_pk_count(redis_client_t::get_uint32("role_fight", "pk_count", uid));
	per_info->set_pk_value(redis_client_t::get_uint32("role_fight", "pk_value", uid));
	per_info->set_pk_value_reduce_time(redis_client_t::get_uint32("role_fight", "pk_value_reduce_time", uid));
	per_info->set_pk_punishment(redis_client_t::get_uint32("role_fight", "pk_punishment", uid));

	ud->mutable_fight()->set_fight_uid(redis_client_t::get_uint64("role_fight", "fight_uid", uid));
	ud->mutable_fight()->set_auto_fight_flag(redis_client_t::get_uint32("role_fight", "auto_fight_flag", uid));
	ud->mutable_fight()->set_ignore_lineup_flag(redis_client_t::get_uint32("role_fight", "ignore_lineup_flag", uid));

	proto::common::fight_study_data* fsd = ud->mutable_fight()->mutable_study();
	fsd->set_step(redis_client_t::get_uint32("role_fight", "fight_study_step", uid));
	fsd->set_step_time(redis_client_t::get_uint32("role_fight", "fight_study_step_time", uid));
	fsd->set_target(redis_client_t::get_uint64("role_fight", "fight_study_target", uid));
	fsd->set_attacker(redis_client_t::get_uint64("role_fight", "fight_study_attacker", uid));
	fsd->set_study_time(redis_client_t::get_uint32("role_fight", "fight_study_time", uid));
	return true;
}

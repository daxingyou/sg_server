#include "family_data_mgr.hpp"
#include "log.hpp"
#include "redis_client.hpp"
#include "main/center_server_fwd.hpp"
#include "tblh/family_enum.hpp"
#include "global_user_data_mgr.hpp"
#include "utility.hpp"
#include "common.pb.h"
#include "country/country_mgr.hpp"

bool family_data_mgr_t::load_all_datas(family_manager_t::family_map& familys)
{
	familys.clear();

	std::vector<uint64_t> all_keys;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>("family", all_keys);

	uint32_t success_num = 0;
	for (const auto& key : all_keys)
	{
		uint32_t invalid = redis_client_t::get_uint32("family", "invalid", key);

		// 对于已经无效的数据
		if (invalid != 0)
		{
			continue;
		}

		family_ptr p_family(new family(key));
		if (NULL == p_family)
		{
			log_error("NULL == p_family");
			return false;
		}

		if (!load_family_data(p_family))
		{
			log_error("family[%lu] load data error", key);
			return false;
		}

		if (!load_family_shop(p_family)) {
			log_error("family[%lu] load shop error", key);
			return false;
		}

		if (!load_all_build(p_family))
		{
			log_error("family[%lu] load build error", key);
			return false;
		}

		if (!load_all_member(p_family))
		{
			log_error("family[%lu] load member error", key);
			return false;
		}

		std::pair<family_manager_t::family_map::iterator, bool> ret = familys.insert(family_manager_t::family_map::value_type(key, p_family));
		if (!ret.second)
		{
			log_error("family[%lu] insert error, it is exist", key);
			continue;
		}

		if (p_family->get_country_id() != 0) {
			country_ptr p_country = country_mgr_t::get_country(p_family->get_country_id());
			if (p_country) p_country->add_family(p_family);
		}

		// 正在弹劾，起服查看时间到了没
		if (p_family->m_impeacher != 0)
		{
			p_family->update_impeacher();
		}

		success_num++;
	}

	FAMILY_LOG("family data load end, success[%u] failed[%u]", success_num, all_keys.size() - success_num);

	return true;
}

bool family_data_mgr_t::load_all_war_data(uint32_t cond, family_manager_t::against_data_map& against_map, family_manager_t::family_against_id_map& id_map)
{
	against_map.clear();
	id_map.clear();

	std::vector<std::string> keys;
	redis_client_t::get_keys_by_field_condition<std::string, uint32_t, std::vector<std::string>>("family_war", "week", cond, keys);

	for (const auto& key : keys)
	{
		uint32_t id = redis_client_t::get_uint32("family_war", "id", key);
		if (id == 0)
		{
			log_error("family war rk[%s] id == 0", key.c_str());
			continue;
		}

		uint32_t index = id >> 16;
		uint32_t side = id & 0x0000FFFF;
		proto::common::family_war_data* data = NULL;

		switch (side)
		{
		case family_war_side::family_war_side_left:
			{
				data = against_map[index].m_data.mutable_left_side();
			}
			break;
		case family_war_side::family_war_side_right:
			{
				data = against_map[index].m_data.mutable_right_side();
			}
			break;
		default:
			log_error("family war id[%s] side error", key.c_str());
			return false;
		}

		uint64_t family_id = redis_client_t::get_uint64("family_war", "family_id", key);
		if (family_id == 0)
		{
			log_error("family war rk[%s] family_id == 0", key.c_str());
			continue;
		}

		id_map.insert(std::make_pair(family_id, index));

		if (NULL != data)
		{
			data->set_family_id(common::string_util_t::uint64_to_string(family_id));
			data->set_family_name(redis_client_t::get_string("family_war", "family_name", key));
			data->set_family_prestige(redis_client_t::get_uint32("family_war", "family_prestige", key));
			data->set_family_score(redis_client_t::get_uint32("family_war", "family_score", key));
			data->set_family_rank(redis_client_t::get_uint32("family_war", "family_rank", key));
			data->set_result((proto::common::family_war_result)redis_client_t::get_uint32("family_war", "result", key));
		}
	}

	return true;
}

bool family_data_mgr_t::load_all_war_history_data(uint32_t cond, proto::common::family_war_history_data& history_data)
{
	std::vector<std::string> keys;
	redis_client_t::get_keys_by_field_condition<std::string, uint32_t, std::vector<std::string>>("family_war", "week", cond, keys);

	if (keys.empty())
	{
		return false;
	}

	for (const auto& key : keys)
	{
		proto::common::family_war_data* data = history_data.add_data();

		if (NULL != data)
		{
			data->set_family_id(common::string_util_t::uint64_to_string(redis_client_t::get_uint64("family_war", "family_id", key)));
			data->set_family_name(redis_client_t::get_string("family_war", "family_name", key));
			data->set_family_prestige(redis_client_t::get_uint32("family_war", "family_prestige", key));
			data->set_family_score(redis_client_t::get_uint32("family_war", "family_score", key));
			data->set_family_rank(redis_client_t::get_uint32("family_war", "family_rank", key));
			data->set_result((proto::common::family_war_result)redis_client_t::get_uint32("family_war", "result", key));
		}
	}

	return true;
}

bool family_data_mgr_t::is_family_key_exsit(uint64_t family_id)
{
	return redis_client_t::is_key_exist("family", family_id);
}

bool family_data_mgr_t::is_family_name_exsit(const std::string& name)
{
	std::vector<uint64_t> families;
	redis_client_t::get_keys_by_field_condition<uint64_t, std::string, std::vector<uint64_t>>("family", "name", name, families);

	return !families.empty();
}

std::string family_data_mgr_t::get_family_name(uint64_t family_id)
{
	if (!family_data_mgr_t::is_family_key_exsit(family_id))
	{
		log_error("family[%lu] not find", family_id);
		return std::string("");
	}

	return redis_client_t::get_string("family", "name", family_id);
}

bool family_data_mgr_t::is_member_key_exsit(uint64_t member_id)
{
	return redis_client_t::is_key_exist("family_member", member_id);
}

bool family_data_mgr_t::is_build_key_exsit(const std::string& key)
{
	return redis_client_t::is_key_exist("family_build", key);
}

bool family_data_mgr_t::load_all_member(const family_ptr& p_family) 
{
	if (NULL == p_family) {
		log_error("NULL == p_family");
		return false;
	}

	std::vector<uint64_t> all_members;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint64_t, std::vector<uint64_t>>("family_member", "family_id", p_family->m_family_id, all_members);

	for (const auto& member_id : all_members) {
		if (!global_user_data_mgr_t::is_user_key_exsit(member_id)) {
			// 家族成员id不存在，走删除
			log_error("family[%lu] member[%lu] is not exsit", p_family->m_family_id, member_id);
			del_family_member(member_id);
			continue;
		}

		uint32_t officer_type = redis_client_t::get_uint32("family_member", "family_officer_type", member_id);
		if (family_officer_type::family_officer_type_none == officer_type || officer_type > family_officer_type::family_officer_type_newbie) {
			log_error("family[%lu] member[%lu] officer_type[%u] error", p_family->m_family_id, member_id, officer_type);
			continue;
		}

		family_member_ptr p_family_member(new family_member(member_id, p_family->m_family_id));
		if (NULL == p_family_member) {
			log_error("NULL == p_family_member");
			return false;
		}

		p_family_member->m_base.set_role_name(global_user_data_mgr_t::get_name(member_id));
		p_family_member->m_base.set_officer(officer_type);
		p_family_member->m_base.set_level(global_user_data_mgr_t::get_user_level(member_id));
		p_family_member->m_base.set_fighting(global_user_data_mgr_t::get_user_fighting_value(member_id));
		p_family_member->m_base.set_last_offline_time(global_user_data_mgr_t::get_last_logout_time(member_id));
		p_family_member->m_base.set_total_family_contribution(redis_client_t::get_uint32("family_member", "total_family_contribution", member_id));
		p_family_member->m_base.set_left_family_contribution(global_user_data_mgr_t::get_family_contribution(member_id));
		p_family_member->m_base.set_plugin(global_user_data_mgr_t::get_user_hero_plugin(member_id));
		p_family_member->m_base.set_prayer_contribution(redis_client_t::get_uint32("family_member", "prayer_contribution", member_id));

		p_family_member->m_join_time = redis_client_t::get_uint32("family_member", "join_family_time", member_id);
		p_family_member->m_is_impeach_ack = (family_impeach_type)redis_client_t::get_uint32("family_member", "impeach_ack", member_id);

		p_family_member->m_last_prayer_day_time = redis_client_t::get_uint32("family_member", "last_prayer_day_time", p_family_member->m_uid);
		p_family_member->m_prayer_times = redis_client_t::get_uint32("family_member", "prayer_times", p_family_member->m_uid);
		p_family_member->m_prayer_rwd_times = redis_client_t::get_uint32("family_member", "prayer_rwd_times", p_family_member->m_uid);
		p_family_member->m_last_prayer_week_time = redis_client_t::get_uint32("family_member", "last_prayer_week_time", p_family_member->m_uid);
		p_family_member->m_prayer_gift_times = redis_client_t::get_uint32("family_member", "prayer_gift_times", p_family_member->m_uid);

		p_family_member->m_prayer_hero_tid = redis_client_t::get_uint32("family_member", "prayer_hero_tid", member_id);
		p_family_member->m_prayer_hero_num = redis_client_t::get_uint32("family_member", "prayer_hero_num", member_id);
		p_family_member->m_prayer_hero_max = redis_client_t::get_uint32("family_member", "prayer_hero_max", member_id);
		p_family_member->m_prayer_start_time = redis_client_t::get_uint32("family_member", "prayer_start_time", member_id);
		p_family_member->m_prayer_complete_time = redis_client_t::get_uint32("family_member", "prayer_complete_time", member_id);

		std::string gift_log_str = redis_client_t::get_string("family_member", "prayer_gift_log", member_id);
		if (!gift_log_str.empty()) {
			std::vector<std::string> log_vec;
			std::vector<std::string> log_items;
			common::string_util_t::split<std::string>(gift_log_str, log_vec, "$");

			for (const auto& one_log : log_vec) { 
				common::string_util_t::split<std::string>(one_log, log_items, ":");
				if (log_items.size() != 2) {
					log_error("role[%lu] load prayer_gift_log error", member_id);
					continue;
				}

				proto::common::family_gift_data* p_gift_data = p_family_member->m_gift_list.add_gift_datas();
				if (p_gift_data != NULL) {
					uint64_t uid = common::string_util_t::string_to_uint64(log_items[0]);
					uint32_t times = common::string_util_t::string_to_uint32(log_items[1]);
					p_gift_data->set_role_id(log_items[0]);
					p_gift_data->set_gift_times(times);
					p_family_member->m_gift_list_map[uid] = times;
				}
			}
		}

		redis_client_t::get_protobuf("family_member", "prayer_log", member_id, p_family_member->m_record_list);

		// 加载个人商店信息
		load_member_shop_info(p_family_member);

		std::pair<family::family_member_map::iterator, bool> ret = p_family->m_member_map.insert(family::family_member_map::value_type(member_id, p_family_member));
		if (!ret.second) {
			log_error("family[%lu] member[%lu] insert error, it is exist", p_family->m_family_id, member_id);
			continue;
		}

		// 分类
		p_family->m_officer_map[officer_type].insert(member_id);

		// 设置族长id
		if (officer_type == family_officer_type::family_officer_type_general) {
			p_family->m_data.set_patriarch(common::string_util_t::uint64_to_string(member_id));
		}
	}

	return true;
}

bool family_data_mgr_t::load_all_build(const family_ptr& p_family)
{
	if (NULL == p_family)
	{
		log_error("NULL == p_family");
		return false;
	}

	// 考虑到可能增加新建筑，走配置
	for (uint32_t i = proto::common::family_build_type_MIN; i < proto::common::family_build_type_ARRAYSIZE; ++i)
	{
		family_build_ptr p_family_build(new family_build((proto::common::family_build_type)i, p_family->m_family_id));
		if (NULL == p_family_build)
		{
			log_error("NULL == p_family_build");
			return false;
		}

		if (is_build_key_exsit(p_family_build->m_key))
		{
			p_family_build->m_base.set_level(redis_client_t::get_uint32("family_build", "level", p_family_build->m_key));
			p_family_build->m_base.set_cd_time(redis_client_t::get_uint32("family_build", "cd_time", p_family_build->m_key));
		}
		else
		{
			p_family_build->init();

			// 反存
			redis_client_t::set_uint64("family_build", "id", p_family_build->m_key, p_family_build->m_family_id);
			redis_client_t::set_uint32("family_build", "type", p_family_build->m_key, (uint32_t)p_family_build->m_type);
			redis_client_t::set_uint32("family_build", "level", p_family_build->m_key, p_family_build->m_base.level());
			redis_client_t::set_uint32("family_build", "cd_time", p_family_build->m_key, p_family_build->m_base.cd_time());
		}

		std::pair<family::family_build_map::iterator, bool> ret = p_family->m_build_map.insert(family::family_build_map::value_type(p_family_build->m_type, p_family_build));
		if (!ret.second)
		{
			log_error("family[%lu] build[%u] insert error, it is exist", p_family->m_family_id, p_family_build->m_type);
			continue;
		}
	}

	// 更新和家族建筑等级相关的数据
	p_family->update_member_size();
	p_family->update_max_money();

	return true;
}

void family_data_mgr_t::del_family_member(uint64_t key)
{
	// 不存在直接return
	if (!is_member_key_exsit(key))
	{
		log_error("family member_id[%lu] is not exsit", key);
		return;
	}

	redis_client_t::set_uint64("family_member", "family_id", key, 0);
	redis_client_t::set_uint32("family_member", "family_officer_type", key, 0);
	redis_client_t::set_uint32("family_member", "total_family_contribution", key, 0);
	redis_client_t::set_uint32("family_member", "join_family_time", key, 0);
	redis_client_t::set_uint32("family_member", "impeach_ack", key, 0);
	redis_client_t::set_uint32("family_member", "prayer_contribution", key, 0);

	// 退家族不删除祈愿次数相关数据

	redis_client_t::set_uint32("family_member", "prayer_hero_tid", key, 0);
	redis_client_t::set_uint32("family_member", "prayer_hero_num", key, 0);
	redis_client_t::set_uint32("family_member", "prayer_hero_max", key, 0);
	redis_client_t::set_uint32("family_member", "prayer_start_time", key, 0);
	redis_client_t::set_uint32("family_member", "prayer_complete_time", key, 0);
	redis_client_t::set_string("family_member", "prayer_gift_log", key, "");

	// 限购信息不清除
	// redis_client_t::set_uint32("family_member", "shop_refresh", key, 0);
	// redis_client_t::set_string("family_member", "random_shop_limit", key, "");
}

bool family_data_mgr_t::load_family_data(const family_ptr& p_family)
{
	if (NULL == p_family)
	{
		log_error("NULL == p_family");
		return false;
	}

	uint64_t family_id = p_family->m_family_id;

	p_family->m_data.set_name(redis_client_t::get_string("family", "name", family_id));
	p_family->m_data.set_level(redis_client_t::get_uint32("family", "level", family_id));
	p_family->m_create_time = redis_client_t::get_uint32("family", "create_time", family_id);
	p_family->m_data.set_country(redis_client_t::get_uint32("family", "country", family_id));
	p_family->m_data.set_loyalty(redis_client_t::get_uint32("family", "loyalty", family_id));
	p_family->m_data.set_creator(redis_client_t::get_string("family", "creator", family_id));
	p_family->m_data.set_money(redis_client_t::get_uint32("family", "money", family_id));
	p_family->m_data.set_declaration(redis_client_t::get_string("family", "declaration", family_id));
	p_family->m_data.set_notice(redis_client_t::get_string("family", "notice", family_id));
	p_family->m_data.set_prosperity(redis_client_t::get_uint32("family", "prosperity", family_id));
	p_family->m_data.set_prestige(redis_client_t::get_uint32("family", "prestige", family_id));
	p_family->m_data.set_territory(redis_client_t::get_uint32("family", "territory", family_id));
	p_family->m_data.set_auto_newbie(redis_client_t::get_uint32("family", "auto_newbie", family_id) != 0);
	p_family->m_data.set_matain_cost(redis_client_t::get_uint32("family", "matain_cost", family_id));

	p_family->m_is_low_maintenance = redis_client_t::get_uint32("family", "is_low_maintenance", family_id) != 0;

	Misc::Parse2SeperatorDataInfo(redis_client_t::get_string("family", "apply_family", family_id), p_family->m_apply_map);

	p_family->m_online_prosperity_add_daily = redis_client_t::get_uint32("family", "online_prosperity_add_daily", family_id);
	p_family->m_chat_prosperity_add_daily = redis_client_t::get_uint32("family", "chat_prosperity_add_daily", family_id);
	p_family->m_task_prosperity_add_daily = redis_client_t::get_uint32("family", "task_prosperity_add_daily", family_id);

	p_family->m_prestige_add_daily = redis_client_t::get_uint32("family", "prestige_add_daily", family_id);

	p_family->m_impeacher = redis_client_t::get_uint64("family", "impeacher", family_id);
	p_family->m_impeach_time = redis_client_t::get_uint32("family", "impeach_time", family_id);
	p_family->m_impeacher_cnt = redis_client_t::get_uint32("family", "impeacher_cnt", family_id);

	p_family->m_quit_time = redis_client_t::get_uint32("family", "quit_time", family_id);
	p_family->m_quit_count = redis_client_t::get_uint32("family", "quit_count", family_id);

	return true;
}

void family_data_mgr_t::save_all(const family_ptr& p_family)
{
	if (NULL == p_family)
	{
		log_error("NULL == p_family");
		return;
	}

	uint64_t family_id = p_family->m_family_id;

	redis_client_t::set_uint32("family", "level", family_id, p_family->m_data.level());
	redis_client_t::set_uint32("family", "create_time", family_id, p_family->m_create_time);
	redis_client_t::set_uint32("family", "country", family_id, p_family->m_data.country());
	redis_client_t::set_uint32("family", "loyalty", family_id, p_family->m_data.loyalty());
	redis_client_t::set_uint32("family", "money", family_id, p_family->m_data.money());
	redis_client_t::set_uint32("family", "auto_newbie", family_id, p_family->m_data.auto_newbie() ? 1 : 0);
	redis_client_t::set_string("family", "name", family_id, p_family->m_data.name());
	redis_client_t::set_string("family", "declaration", family_id, p_family->m_data.declaration());
	redis_client_t::set_string("family", "notice", family_id, p_family->m_data.notice());
	redis_client_t::set_uint32("family", "prosperity", family_id, p_family->m_data.prosperity());
	redis_client_t::set_uint32("family", "prestige", family_id, p_family->m_data.prestige());
	redis_client_t::set_uint32("family", "territory", family_id, p_family->m_data.territory());
	redis_client_t::set_string("family", "creator", family_id, p_family->m_data.creator());

	redis_client_t::set_uint64("family", "impeacher", family_id, p_family->m_impeacher);
	redis_client_t::set_uint32("family", "impeach_time", family_id, p_family->m_impeach_time);
	redis_client_t::set_uint32("family", "impeacher_cnt", family_id, p_family->m_impeacher_cnt);

	redis_client_t::set_uint32("family", "online_prosperity_add_daily", family_id, p_family->m_online_prosperity_add_daily);
	redis_client_t::set_uint32("family", "chat_prosperity_add_daily", family_id, p_family->m_chat_prosperity_add_daily);
	redis_client_t::set_uint32("family", "task_prosperity_add_daily", family_id, p_family->m_task_prosperity_add_daily);

	redis_client_t::set_uint32("family", "prestige_add_daily", family_id, p_family->m_prestige_add_daily);
	redis_client_t::set_uint32("family", "matain_cost", family_id, p_family->m_data.matain_cost());
	redis_client_t::set_uint32("family", "is_low_maintenance", family_id, p_family->m_is_low_maintenance ? 1 : 0);

	redis_client_t::set_uint32("family", "quit_time", family_id, p_family->m_quit_time);
	redis_client_t::set_uint32("family", "quit_count", family_id, p_family->m_quit_count);

	save_family_shop(p_family);

	save_apply_list(p_family);

	save_all_build(p_family);

	save_all_member(p_family);
}

void family_data_mgr_t::save_apply_list(const family_ptr& p_family)
{
	if (NULL == p_family)
	{
		log_error("NULL == p_family");
		return;
	}

	if (p_family->m_apply_map.empty())
	{
		redis_client_t::set_string("family", "apply_family", p_family->m_family_id, "");
	}
	else
	{
		std::ostringstream oss;
		for (const auto& item : p_family->m_apply_map)
		{
			if (!oss.str().empty())
			{
				oss << ";";
			}

			oss << item.first << ":" << item.second;
		}

		redis_client_t::set_string("family", "apply_family", p_family->m_family_id, oss.str());
	}
}

void family_data_mgr_t::save_all_build(const family_ptr& p_family)
{
	if (NULL == p_family)
	{
		log_error("NULL == p_family");
		return;
	}

	for (const auto& item : p_family->m_build_map)
	{
		const family_build_ptr& p_family_build = item.second;

		if (NULL != p_family_build)
		{
			redis_client_t::set_uint64("family_build", "id", p_family_build->m_key, p_family_build->m_family_id);
			redis_client_t::set_uint32("family_build", "type", p_family_build->m_key, (uint32_t)p_family_build->m_type);
			redis_client_t::set_uint32("family_build", "level", p_family_build->m_key, p_family_build->m_base.level());
			redis_client_t::set_uint32("family_build", "cd_time", p_family_build->m_key, p_family_build->m_base.cd_time());
		}
	}
}

void family_data_mgr_t::save_all_member(const family_ptr& p_family)
{
	if (NULL == p_family)
	{
		log_error("NULL == p_family");
		return;
	}

	for (const auto& item : p_family->m_member_map)
	{
		save_one_member(item.second);
	}
}

void family_data_mgr_t::save_one_member(const family_member_ptr& p_family_member) 
{
	if (NULL == p_family_member) {
		log_error("NULL == p_family_member");
		return;
	}

	redis_client_t::set_uint64("family_member", "family_id", p_family_member->m_uid, p_family_member->m_family_id);
	redis_client_t::set_uint32("family_member", "family_officer_type", p_family_member->m_uid, p_family_member->m_base.officer());
	redis_client_t::set_uint32("family_member", "total_family_contribution", p_family_member->m_uid, p_family_member->m_base.total_family_contribution());
	redis_client_t::set_uint32("family_member", "join_family_time", p_family_member->m_uid, p_family_member->m_join_time);
	redis_client_t::set_uint32("family_member", "impeach_ack", p_family_member->m_uid, p_family_member->m_is_impeach_ack);
	redis_client_t::set_uint32("family_member", "prayer_contribution", p_family_member->m_uid, p_family_member->m_base.prayer_contribution());
	redis_client_t::set_uint32("family_member", "prayer_hero_tid", p_family_member->m_uid, p_family_member->m_prayer_hero_tid);
	redis_client_t::set_uint32("family_member", "prayer_hero_num", p_family_member->m_uid, p_family_member->m_prayer_hero_num);
	redis_client_t::set_uint32("family_member", "prayer_hero_max", p_family_member->m_uid, p_family_member->m_prayer_hero_max);
	redis_client_t::set_uint32("family_member", "prayer_start_time", p_family_member->m_uid, p_family_member->m_prayer_start_time);
	redis_client_t::set_uint32("family_member", "prayer_complete_time", p_family_member->m_uid, p_family_member->m_prayer_complete_time);

	redis_client_t::set_uint32("family_member", "last_prayer_day_time", p_family_member->m_uid, p_family_member->m_last_prayer_day_time);
	redis_client_t::set_uint32("family_member", "prayer_times", p_family_member->m_uid, p_family_member->m_prayer_times);
	redis_client_t::set_uint32("family_member", "prayer_rwd_times", p_family_member->m_uid, p_family_member->m_prayer_rwd_times);
	redis_client_t::set_uint32("family_member", "last_prayer_week_time", p_family_member->m_uid, p_family_member->m_last_prayer_week_time);
	redis_client_t::set_uint32("family_member", "prayer_gift_times", p_family_member->m_uid, p_family_member->m_prayer_gift_times);

	std::ostringstream ss;
	for (std::map<uint64_t, uint32_t>::const_iterator citr = p_family_member->m_gift_list_map.begin();
		citr != p_family_member->m_gift_list_map.end(); ++citr) {
		if (!ss.str().empty()) {
			ss << "$";
		}
		ss << citr->first << ":" << citr->second;
	}

	redis_client_t::set_string("family_member", "prayer_gift_log", p_family_member->m_uid, ss.str());
	redis_client_t::set_protobuf("family_member", "prayer_log", p_family_member->m_uid, p_family_member->m_record_list);

	// 保持商店信息
	save_family_shop_personal(p_family_member->m_uid, p_family_member->m_shop_personal_data);
}

void family_data_mgr_t::save_one_build(const family_build_ptr& p_family_build)
{
	if (NULL == p_family_build)
	{
		log_error("NULL == p_family_build");
		return;
	}

	redis_client_t::set_uint64("family_build", "id", p_family_build->m_key, p_family_build->m_family_id);
	redis_client_t::set_uint32("family_build", "type", p_family_build->m_key, (uint32_t)p_family_build->m_type);
	redis_client_t::set_uint32("family_build", "level", p_family_build->m_key, p_family_build->m_base.level());
	redis_client_t::set_uint32("family_build", "cd_time", p_family_build->m_key, p_family_build->m_base.cd_time());
}

void family_data_mgr_t::del_family(uint64_t family_id)
{
	if (!family_data_mgr_t::is_family_key_exsit(family_id))
	{
		log_error("family[%lu] not find", family_id);
		return;
	}

	// 家族成员在删除家族前就应该全部删除

	// 删除家族建筑
	std::vector<std::string> family_builds;
	redis_client_t::get_keys_by_field_condition<std::string, uint64_t, std::vector<std::string>>("family_build", "id", family_id, family_builds);
	for (const auto& build_key : family_builds)
	{
		redis_client_t::set_uint64("family_build", "id", build_key, 0);
		redis_client_t::set_uint32("family_build", "type", build_key, 0);
		redis_client_t::set_uint32("family_build", "level", build_key, 0);
		redis_client_t::set_uint32("family_build", "cd_time", build_key, 0);
	}

	// 删除家族信息
	redis_client_t::set_uint32("family", "invalid", family_id, 1);	// 设置无效
	redis_client_t::set_uint32("family", "level", family_id, 0);
	redis_client_t::set_uint32("family", "create_time", family_id, 0);
	redis_client_t::set_uint32("family", "country", family_id, 0);
	redis_client_t::set_uint32("family", "loyalty", family_id, 0);
	redis_client_t::set_uint32("family", "money", family_id, 0);
	redis_client_t::set_uint32("family", "auto_newbie", family_id, 0);
	redis_client_t::set_string("family", "name", family_id, "");
	redis_client_t::set_string("family", "declaration", family_id, "");
	redis_client_t::set_string("family", "notice", family_id, "");
	redis_client_t::set_uint32("family", "prosperity", family_id, 0);
	redis_client_t::set_uint32("family", "prestige", family_id, 0);
	redis_client_t::set_uint32("family", "territory", family_id, 0);
	//redis_client_t::set_string("family", "creator", family_id, p_family->m_data.creator());

	redis_client_t::set_uint64("family", "impeacher", family_id, 0);
	redis_client_t::set_uint32("family", "impeach_time", family_id, 0);
	redis_client_t::set_uint32("family", "impeacher_cnt", family_id, 0);

	redis_client_t::set_uint32("family", "online_prosperity_add_daily", family_id, 0);
	redis_client_t::set_uint32("family", "chat_prosperity_add_daily", family_id, 0);
	redis_client_t::set_uint32("family", "task_prosperity_add_daily", family_id, 0);

	redis_client_t::set_string("family", "apply_family", family_id, "");

	redis_client_t::set_uint32("family", "prestige_add_daily", family_id, 0);
	redis_client_t::set_uint32("family", "matain_cost", family_id, 0);
	redis_client_t::set_uint32("family", "is_low_maintenance", family_id, 0);

	redis_client_t::set_uint32("family", "quit_time", family_id, 0);
	redis_client_t::set_uint32("family", "quit_count", family_id, 0);

	redis_client_t::set_uint32("family", "shop_refresh", family_id, 0);
	redis_client_t::set_string("family", "random_shop", family_id, "");
}

void family_data_mgr_t::save_all_war_data(uint32_t week_id, const family_manager_t::against_data_map& against_map)
{
	for (family_manager_t::against_data_map::const_iterator citr = against_map.begin();
		citr != against_map.end(); ++citr)
	{
		uint32_t index = citr->first;
		const family_manager_t::family_against_data_t& war_data = citr->second;

		// 左方
		{
			const proto::common::family_war_data& left_side_data = war_data.m_data.left_side();

			std::string left_key = left_side_data.family_id() + common::string_util_t::uint32_to_string(week_id);

			uint32_t id = (index << 16) + family_war_side::family_war_side_left;
			redis_client_t::set_uint32("family_war", "id", left_key, id);
			redis_client_t::set_uint32("family_war", "week", left_key, week_id);
			redis_client_t::set_uint64("family_war", "family_id", left_key, common::string_util_t::string_to_uint64(left_side_data.family_id()));
			redis_client_t::set_string("family_war", "family_name", left_key, left_side_data.family_name());
			redis_client_t::set_uint32("family_war", "family_prestige", left_key, left_side_data.family_prestige());
			redis_client_t::set_uint32("family_war", "family_score", left_key, left_side_data.family_score());
			redis_client_t::set_uint32("family_war", "family_rank", left_key, left_side_data.family_rank());
			redis_client_t::set_uint32("family_war", "result", left_key, (uint32_t)left_side_data.result());
		}

		// 右方
		{
			const proto::common::family_war_data& right_side_data = war_data.m_data.right_side();

			std::string right_key = right_side_data.family_id() + common::string_util_t::uint32_to_string(week_id);

			uint32_t id = (index << 16) + family_war_side::family_war_side_right;
			redis_client_t::set_uint32("family_war", "id", right_key, id);
			redis_client_t::set_uint32("family_war", "week", right_key, week_id);
			redis_client_t::set_uint64("family_war", "family_id", right_key, common::string_util_t::string_to_uint64(right_side_data.family_id()));
			redis_client_t::set_string("family_war", "family_name", right_key, right_side_data.family_name());
			redis_client_t::set_uint32("family_war", "family_prestige", right_key, right_side_data.family_prestige());
			redis_client_t::set_uint32("family_war", "family_score", right_key, right_side_data.family_score());
			redis_client_t::set_uint32("family_war", "family_rank", right_key, right_side_data.family_rank());
			redis_client_t::set_uint32("family_war", "result", right_key, (uint32_t)right_side_data.result());
		}
	}
}

//////////////////////////////////////////////////////////////////////////

#define FAMILY_KEY_CHECK(family_id) \
	if (!family_data_mgr_t::is_family_key_exsit(family_id)) \
	{ \
		log_error("family[%lu] not find", family_id); \
		return; \
	}

void family_data_mgr_t::update_family_money(uint64_t family_id, uint32_t val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "money", family_id, val);
}

void family_data_mgr_t::update_family_maintain(uint64_t family_id, uint32_t val, bool is_low)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "matain_cost", family_id, val);
	redis_client_t::set_uint32("family", "is_low_maintenance", family_id, is_low ? 1 : 0);
}

void family_data_mgr_t::update_family_prosperity(uint64_t family_id, uint32_t val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "prosperity", family_id, val);

	// 同时清空加成
	redis_client_t::set_uint32("family", "online_prosperity_add_daily", family_id, 0);
	redis_client_t::set_uint32("family", "chat_prosperity_add_daily", family_id, 0);
	redis_client_t::set_uint32("family", "task_prosperity_add_daily", family_id, 0);
}

void family_data_mgr_t::update_online_prosperity_add_daily(uint64_t family_id, uint32_t val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "online_prosperity_add_daily", family_id, val);
}

void family_data_mgr_t::update_chat_prosperity_add_daily(uint64_t family_id, uint32_t val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "chat_prosperity_add_daily", family_id, val);
}

void family_data_mgr_t::update_task_prosperity_add_daily(uint64_t family_id, uint32_t val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "task_prosperity_add_daily", family_id, val);
}

void family_data_mgr_t::update_family_prestige(uint64_t family_id, uint32_t val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "prestige", family_id, val);

	// 同时清空加成
	redis_client_t::set_uint32("family", "prestige_add_daily", family_id, 0);
}

void family_data_mgr_t::update_family_auto_newbie(uint64_t family_id, bool val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "auto_newbie", family_id, val ? 1 : 0);
}

void family_data_mgr_t::update_family_declaration(uint64_t family_id, const std::string& val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_string("family", "declaration", family_id, val);
}

void family_data_mgr_t::update_family_notification(uint64_t family_id, const std::string& val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_string("family", "notice", family_id, val);
}

void family_data_mgr_t::update_family_level(uint64_t family_id, uint32_t val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "level", family_id, val);
}

void family_data_mgr_t::update_family_country(uint64_t family_id, uint32_t val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "country", family_id, val);
}

void family_data_mgr_t::update_family_loyalty(uint64_t family_id, uint32_t val)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "loyalty", family_id, val);
}

void family_data_mgr_t::update_quit_country(uint64_t family_id, uint32_t quit_time, uint32_t quit_count)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "quit_time", family_id, quit_time);
	redis_client_t::set_uint32("family", "quit_count", family_id, quit_count);
}

//////////////////////////////////////////////////////////////////////////
#define FAMILY_MEMBER_KEY_CHECK(member_uid) \
	if (!family_data_mgr_t::is_member_key_exsit(member_uid)) \
	{ \
		log_error("family member_id[%lu] is not exsit", member_uid); \
		return; \
	}

void family_data_mgr_t::update_family_member_officer(uint64_t uid, uint32_t val) 
{
	FAMILY_MEMBER_KEY_CHECK(uid);

	redis_client_t::set_uint32("family_member", "family_officer_type", uid, val);
}

void family_data_mgr_t::update_total_family_contribution(uint64_t uid, uint32_t val)
{
	FAMILY_MEMBER_KEY_CHECK(uid);

	redis_client_t::set_uint32("family_member", "total_family_contribution", uid, val);
}

void family_data_mgr_t::update_prayer_contribution(uint64_t uid, uint32_t val)
{
	FAMILY_MEMBER_KEY_CHECK(uid);

	redis_client_t::set_uint32("family_member", "prayer_contribution", uid, val);
}

void family_data_mgr_t::load_prayer_info(const family_member_ptr& p_family_member)
{
	if (!is_member_key_exsit(p_family_member->m_uid)) {
		// 没有可能是新号
		return;
	}

	p_family_member->m_last_prayer_day_time = redis_client_t::get_uint32("family_member", "last_prayer_day_time", p_family_member->m_uid);
	p_family_member->m_prayer_times = redis_client_t::get_uint32("family_member", "prayer_times", p_family_member->m_uid);
	p_family_member->m_prayer_rwd_times = redis_client_t::get_uint32("family_member", "prayer_rwd_times", p_family_member->m_uid);
	p_family_member->m_last_prayer_week_time = redis_client_t::get_uint32("family_member", "last_prayer_week_time", p_family_member->m_uid);
	p_family_member->m_prayer_gift_times = redis_client_t::get_uint32("family_member", "prayer_gift_times", p_family_member->m_uid);
}

void family_data_mgr_t::update_day_prayer_info(const family_member_ptr& p_family_member)
{
	FAMILY_MEMBER_KEY_CHECK(p_family_member->m_uid);

	redis_client_t::set_uint32("family_member", "last_prayer_day_time", p_family_member->m_uid, p_family_member->m_last_prayer_day_time);
	redis_client_t::set_uint32("family_member", "prayer_times", p_family_member->m_uid, p_family_member->m_prayer_times);
	redis_client_t::set_uint32("family_member", "prayer_rwd_times", p_family_member->m_uid, p_family_member->m_prayer_rwd_times);
}

void family_data_mgr_t::update_prayer_times(uint64_t uid, uint32_t val)
{
	FAMILY_MEMBER_KEY_CHECK(uid);

	redis_client_t::set_uint32("family_member", "prayer_times", uid, val);
}

void family_data_mgr_t::update_prayer_rwd_times(uint64_t uid, uint32_t val)
{
	FAMILY_MEMBER_KEY_CHECK(uid);

	redis_client_t::set_uint32("family_member", "prayer_rwd_times", uid, val);
}

void family_data_mgr_t::update_week_prayer_info(const family_member_ptr& p_family_member)
{
	FAMILY_MEMBER_KEY_CHECK(p_family_member->m_uid);

	redis_client_t::set_uint32("family_member", "last_prayer_week_time", p_family_member->m_uid, p_family_member->m_last_prayer_week_time);
	redis_client_t::set_uint32("family_member", "prayer_gift_times", p_family_member->m_uid, p_family_member->m_prayer_gift_times);

	redis_client_t::set_protobuf("family_member", "prayer_log", p_family_member->m_uid, p_family_member->m_record_list);
}

void family_data_mgr_t::update_prayer_hero_info(const family_member_ptr& p_family_member)
{
	FAMILY_MEMBER_KEY_CHECK(p_family_member->m_uid);

	redis_client_t::set_uint32("family_member", "prayer_hero_tid", p_family_member->m_uid, p_family_member->m_prayer_hero_tid);
	redis_client_t::set_uint32("family_member", "prayer_hero_num", p_family_member->m_uid, p_family_member->m_prayer_hero_num);
	redis_client_t::set_uint32("family_member", "prayer_hero_max", p_family_member->m_uid, p_family_member->m_prayer_hero_max);
	redis_client_t::set_uint32("family_member", "prayer_start_time", p_family_member->m_uid, p_family_member->m_prayer_start_time);
	redis_client_t::set_uint32("family_member", "prayer_complete_time", p_family_member->m_uid, p_family_member->m_prayer_complete_time);

	std::ostringstream ss;
	for (std::map<uint64_t, uint32_t>::const_iterator citr = p_family_member->m_gift_list_map.begin();
		citr != p_family_member->m_gift_list_map.end(); ++citr) {
		if (!ss.str().empty()) {
			ss << "$";
		}
		ss << citr->first << ":" << citr->second;
	}
	redis_client_t::set_string("family_member", "prayer_gift_log", p_family_member->m_uid, ss.str());
}

void family_data_mgr_t::save_prayer_record(uint64_t uid, const proto::common::family_prayer_record_list& record_list)
{
	FAMILY_MEMBER_KEY_CHECK(uid);

	redis_client_t::set_protobuf("family_member", "prayer_log", uid, record_list);
}

void family_data_mgr_t::update_prayer_gift_times(uint64_t uid, uint32_t val)
{
	FAMILY_MEMBER_KEY_CHECK(uid);

	redis_client_t::set_uint32("family_member", "prayer_gift_times", uid, val);
}

void family_data_mgr_t::load_member_shop_info(const family_member_ptr& p_family_member)
{
	if (!is_member_key_exsit(p_family_member->m_uid)) {
		// 没有可能是新号
		return;
	}

	p_family_member->m_shop_personal_data.set_next_refresh_time(redis_client_t::get_uint32("family_member", "shop_refresh", p_family_member->m_uid));
	std::string shop_items = redis_client_t::get_string("family_member", "random_shop_limit", p_family_member->m_uid);
	if (!shop_items.empty()) {
		std::vector<std::string> items_vec;
		common::string_util_t::split<std::string>(shop_items, items_vec, "$");

		std::vector<uint32_t> item_vec;
		for (const auto& item : items_vec) {
			common::string_util_t::split<uint32_t>(item, item_vec, ":");
			if (item_vec.size() != 2) {
				log_error("family[%lu] member[%lu] load shop error", p_family_member->m_family_id, p_family_member->m_uid);
				return;
			}

			proto::common::family_shop_personal_item* p_item = p_family_member->m_shop_personal_data.add_items();
			if (p_item) {
				p_item->set_shop_index(item_vec[0]);
				p_item->set_buy_count(item_vec[1]);
			}
		}
	}
}

bool family_data_mgr_t::load_family_shop(const family_ptr& p_family)
{
	if (NULL == p_family) {
		log_error("NULL == p_family");
		return false;
	}

	uint64_t family_id = p_family->m_family_id;
	p_family->m_family_shop.m_shop_data.set_next_refresh_time(redis_client_t::get_uint32("family", "shop_refresh", family_id));
	std::string shop_items = redis_client_t::get_string("family", "random_shop", family_id);
	if (!shop_items.empty()) {
		std::vector<std::string> items_vec;
		common::string_util_t::split<std::string>(shop_items, items_vec, "$");

		std::vector<uint32_t> item_vec;
		for (const auto& item : items_vec) {
			common::string_util_t::split<uint32_t>(item, item_vec, ":");
			if (item_vec.size() != 2) {
				log_error("family[%lu] load shop error", family_id);
				return false;
			}

			proto::common::family_shop_item* p_item = p_family->m_family_shop.m_shop_data.add_items();
			if (p_item) {
				p_item->set_shop_item(item_vec[0]);
				p_item->set_sold_count(item_vec[1]);
			}
		}
	}

	return true;
}

void family_data_mgr_t::save_family_shop(uint64_t family_id, const proto::common::family_shop_data& data)
{
	FAMILY_KEY_CHECK(family_id);

	redis_client_t::set_uint32("family", "shop_refresh", family_id, data.next_refresh_time());

	std::ostringstream ss;
	for (int32_t i = 0; i < data.items_size(); ++i)
	{
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << data.items(i).shop_item() << ":" << (uint32_t)data.items(i).sold_count();
	}

	redis_client_t::set_string("family", "random_shop", family_id, ss.str());
}

void family_data_mgr_t::save_family_shop(const family_ptr& p_family)
{
	if (NULL == p_family) {
		log_error("NULL == p_family");
		return;
	}

	save_family_shop(p_family->m_family_id, p_family->m_family_shop.m_shop_data);
}

void family_data_mgr_t::save_family_shop_personal(uint64_t uid, const proto::common::family_shop_personal_data& data)
{
	FAMILY_MEMBER_KEY_CHECK(uid);

	redis_client_t::set_uint32("family_member", "shop_refresh", uid, data.next_refresh_time());

	std::ostringstream ss;
	for (int32_t i = 0; i < data.items_size(); ++i)
	{
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << data.items(i).shop_index() << ":" << (uint32_t)data.items(i).buy_count();
	}

	redis_client_t::set_string("family_member", "random_shop_limit", uid, ss.str());
}

void family_data_mgr_t::clear_family_shop_personal(uint64_t uid)
{
	FAMILY_MEMBER_KEY_CHECK(uid);

	redis_client_t::set_uint32("family_member", "shop_refresh", uid, 0);
	redis_client_t::set_string("family_member", "random_shop_limit", uid, "");
}


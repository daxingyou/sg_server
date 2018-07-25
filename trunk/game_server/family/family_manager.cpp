#include "family_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "role/money_manager.hpp"
#include "tblh/MapTable.tbls.h"
#include "family_war_scene.hpp"
#include "dungeon/dungeon_manager.hpp"
#include "scene/scene_manager.hpp"
#include "monster/monster_manager.hpp"
#include "family_war_long.hpp"
#include "config_mgr.h"
#include "tblh/Hero.tbls.h"
#include "item/item_manager.hpp"
#include "hero/hero_manager.hpp"
#include "item/drop_manager.hpp"
#include "tblh/ShopItemTable.tbls.h"
#include "log/log_wrapper.hpp"
#include "family_war_scene.hpp"

family_manager_t::family_war_info_map family_manager_t::m_family_war_info_map;

uint32_t family_manager_t::create_family_request(const role_ptr& p_role, const std::string& name, const std::string& declaration)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_create_lv);
	uint32_t need_level = GET_COMPREHENSIVE_VALUE_1(conf);
	if (p_role->get_level() < need_level)
	{
		FAMILY_LOG("role[%lu] level not enough!! can't create family", p_role->get_uid());
		return common::errcode_enum::error_level_not_enough;
	}

    /*if (0 == p_role->get_country_id())
    {
        FAMILY_LOG("role[%lu] not join country yet", p_role->get_uid());
        return common::errcode_enum::country_error_not_in_country;
    }*/

	if (0 != p_role->get_family_id())
	{
		FAMILY_LOG("user[%lu]already join other family!! can't create family", p_role->get_uid());
		return common::errcode_enum::family_err_code_already_join_other_family;
	}

	uint32_t money = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_YUANBAO);
	conf = GET_CONF(Comprehensive, comprehensive_common::family_create_cost);
	uint32_t create_family_cost = GET_COMPREHENSIVE_VALUE_1(conf);
	if (money < create_family_cost)
	{
		FAMILY_LOG("role[%lu]create family not enough money: money[%u] request[%u]", p_role->get_uid(), money, create_family_cost);
		return common::errcode_enum::notice_gold_money_not_enough;
	}

	// 扣钱
	money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, create_family_cost, log_enum::source_type_family_create_cost, 0);

	proto::server::ge_create_family_request request;
	request.set_name(name);
	request.set_declaration(declaration);
	env::server->send_msg_to_center(op_cmd::ge_create_family_request, p_role->get_uid(), request);

	return common::errcode_enum::error_ok;
}

void family_manager_t::prepare_family_war_request(const proto::server::eg_prepare_family_war_request& request)
{
	m_family_war_info_map.clear();

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_war_dungeon);
	uint32_t family_war_map_tid = GET_COMPREHENSIVE_VALUE_1(conf);

	MapTable* p_map_table = GET_CONF(MapTable, family_war_map_tid);
	if (NULL == p_map_table)
	{
		log_error("map_table[%u] null error when create family_war scene", family_war_map_tid);
		return;
	}

	if (p_map_table->scene_type() != proto::server::map_type_dynamic)
	{
		log_error("family war map_table[%u] scene_type[%u] error", family_war_map_tid, p_map_table->scene_type());
		return;
	}

	if (p_map_table->get_scene_attr_type() != proto::server::em_scene_type_family_war)
	{
		log_error("map_table[%u] scene_attr_type[%u] is not family_war when create family war scene", family_war_map_tid, p_map_table->get_scene_attr_type());
		return;
	}

	proto::server::ge_prepare_family_war_reply reply;

	for (int32_t i = 0; i < request.family_war_data_size(); ++i)
	{
		const proto::server::family_war_dungeon_data& data = request.family_war_data(i);

		family_war_scene_ptr p_family_war_scene = dungeon_manager_t::create_family_war_scene(family_war_map_tid, data);
		if (NULL == p_family_war_scene)
		{
			log_error("family war[%u] create family war scene error", data.id());
			continue;
		}

		p_family_war_scene->prepare_family_war(request.start_time());

		// 保存
		m_family_war_info_map[data.id()] = p_family_war_scene->get_scene_id();

		proto::server::family_war_dungeon_back* back_info = reply.add_dungeons();
		if (back_info)
		{
			back_info->set_id(data.id());

			proto::server::game_scene_reg_info* p_reg_info = back_info->mutable_info();
			if (p_reg_info != NULL)
			{
				p_reg_info->set_scene_id(p_family_war_scene->get_scene_id());
				p_reg_info->set_map_tid(p_family_war_scene->get_map_tid());
			}
		}
	}

	env::server->send_msg_to_center(op_cmd::ge_prepare_family_war_reply, 0, reply);
}

void family_manager_t::family_war_start(uint32_t end_time)
{
	for (family_war_info_map::const_iterator citr = m_family_war_info_map.begin();
		citr != m_family_war_info_map.end(); ++citr)
	{
		const scene_ptr& p_scene = scene_manager_t::find_scene(citr->second);
		if (NULL == p_scene)
		{
			continue;
		}

		family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
		if (NULL == p_family_war_scene)
		{
			log_error("scene[%lu] not family_war_scene", p_scene->get_scene_id());
			continue;
		}

		p_family_war_scene->start_family_war(end_time);
	}
}

void family_manager_t::family_war_end()
{
	proto::server::ge_family_war_result_notify notify;

	for (family_war_info_map::const_iterator citr = m_family_war_info_map.begin();
		citr != m_family_war_info_map.end(); ++citr)
	{
		const scene_ptr& p_scene = scene_manager_t::find_scene(citr->second);
		if (NULL == p_scene)
		{
			continue;
		}

		family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
		if (NULL == p_family_war_scene)
		{
			log_error("scene[%lu] not family_war_scene", p_scene->get_scene_id());
			continue;
		}

		p_family_war_scene->end_family_war();

		p_family_war_scene->peek_family_war_result(notify.add_datas());
	}

	m_family_war_info_map.clear();

	env::server->send_msg_to_center(op_cmd::ge_family_war_result_notify, 0, notify);
}

void family_manager_t::get_family_war_member(const role_ptr& p_role, proto::client::gc_family_war_member_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("p_role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
	if (NULL == p_family_war_scene)
	{
		log_error("p_role[%lu] scene[%lu] not family_war_scene", p_role->get_uid(), p_role->get_scene_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	reply.set_left_family(p_family_war_scene->get_family_war_member(family_war_side_left));
	reply.set_right_family(p_family_war_scene->get_family_war_member(family_war_side_right));
	reply.set_reply_code(common::errcode_enum::error_ok);
}

void family_manager_t::get_role_formations_info(const role_ptr& p_role, proto::client::gc_ask_formations_info_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("p_role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
	if (NULL == p_family_war_scene)
	{
		log_error("p_role[%lu] scene[%lu] not family_war_scene", p_role->get_uid(), p_role->get_scene_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const family_war_scene_t::family_war_role_info_ptr& p_role_info = p_family_war_scene->get_family_war_role_info(p_role->get_uid());
	if (NULL == p_role_info)
	{
		log_error("role[%lu] scene[%lu] not have role info", p_role->get_uid(), p_role->get_scene_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	p_role_info->peek_hero_data(reply);
	reply.set_reply_code(common::errcode_enum::error_ok);
}

uint32_t family_manager_t::ask_formations_hero_update(const role_ptr& p_role, const proto::client::cg_formations_hero_update_request& request, proto::client::gc_formations_hero_update_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("p_role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
	if (NULL == p_family_war_scene)
	{
		log_error("p_role[%lu] scene[%lu] not family_war_scene", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	if (p_family_war_scene->get_war_state() < proto::common::family_war_state_prepare)
	{
		FAMILY_LOG("p_role[%lu] scene[%lu] not in family_war_state_prepare", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::family_err_war_state_not_prepare;
	}

	const family_war_scene_t::family_war_role_info_ptr& p_role_info = p_family_war_scene->get_family_war_role_info(p_role->get_uid());
	if (NULL == p_role_info)
	{
		log_error("role[%lu] scene[%lu] not have role info", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_code = common::errcode_enum::error_ok;
	do {
		for (int32_t i = 0; i < request.up_hero_uid_size(); ++i)
		{
			back_code = p_role_info->role_hero_up(p_role, common::string_util_t::string_to_uint64(request.up_hero_uid(i)));

			if (back_code != common::errcode_enum::error_ok)
			{
				break;
			}
		}

		if (back_code != common::errcode_enum::error_ok) {
			break;
		}

		for (int32_t i = 0; i < request.down_hero_uid_size(); ++i)
		{
			back_code = p_role_info->role_hero_down(p_role, common::string_util_t::string_to_uint64(request.down_hero_uid(i)));

			if (back_code != common::errcode_enum::error_ok)
			{
				break;
			}
		}

	} while (0);

	p_role_info->peek_hero_data(reply);

	return back_code;
}

uint32_t family_manager_t::ask_formations_revive(const role_ptr& p_role, const proto::client::cg_formations_hero_revive_request& request)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("p_role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
	if (NULL == p_family_war_scene)
	{
		log_error("p_role[%lu] scene[%lu] not family_war_scene", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	const family_war_scene_t::family_war_role_info_ptr& p_role_info = p_family_war_scene->get_family_war_role_info(p_role->get_uid());
	if (NULL == p_role_info)
	{
		log_error("role[%lu] scene[%lu] not have role info", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t back_code = common::errcode_enum::error_ok;

	for (int32_t i = 0; i < request.hero_uid_size(); ++i)
	{
		back_code = p_role_info->role_hero_revive(p_role, common::string_util_t::string_to_uint64(request.hero_uid(i)));

		if (back_code != common::errcode_enum::error_ok)
		{
			break;
		}
	}

	p_role_info->notify_role_info(p_role);

	return back_code;
}

uint32_t family_manager_t::ask_attack_long(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("p_role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
	if (NULL == p_family_war_scene)
	{
		log_error("p_role[%lu] scene[%lu] not family_war_scene", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	return p_family_war_scene->attack_long(p_role);
}

void family_manager_t::family_war_early_end(const family_war_scene_ptr& p_war_scene)
{
	if (!p_war_scene) {
		log_error("p_war_scene null error");
		return;
	}

	p_war_scene->end_family_war();

	proto::server::ge_family_war_early_result_notify notify;
	p_war_scene->peek_family_war_result(notify.mutable_data());
	env::server->send_msg_to_center(op_cmd::ge_family_war_early_result_notify, 0, notify);

	family_war_info_map::iterator itr = m_family_war_info_map.find(p_war_scene->get_family_war_id());
	if (itr != m_family_war_info_map.end())
	{
		m_family_war_info_map.erase(itr);
	}
}

void family_manager_t::get_family_war_fight_data(const role_ptr& p_role, proto::client::gc_family_war_fight_data_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("p_role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		return;
	}

	family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
	if (NULL == p_family_war_scene)
	{
		log_error("p_role[%lu] scene[%lu] not family_war_scene", p_role->get_uid(), p_role->get_scene_id());
		return;
	}

	p_family_war_scene->peek_family_war_fight_data(p_role->get_uid(), reply);
}

uint32_t family_manager_t::ask_enter_battlefield(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("p_role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
	if (NULL == p_family_war_scene)
	{
		log_error("p_role[%lu] scene[%lu] not family_war_scene", p_role->get_uid(), p_role->get_scene_id());
		return common::errcode_enum::notice_unknown;
	}

	return p_family_war_scene->role_enter_battlefield(p_role);
}

void family_manager_t::get_long_hurt_info(const role_ptr& p_role, uint64_t object_id)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	monster_ptr p_monster = monster_manager_t::find_monster(object_id);
	if (NULL == p_monster)
	{
		log_error("can not find long[%lu]", object_id);
		return;
	}

	family_war_long_ptr p_long = p_monster->get_family_war_long();
	if (NULL == p_long)
	{
		log_error("monster[%lu] not family long", object_id);
		return;
	}

	proto::client::gc_long_hurt_info_reply reply;
	p_long->peek_long_hurt_info(reply);
	p_role->send_msg_to_client(op_cmd::gc_long_hurt_info_reply, reply);
}

void family_manager_t::get_family_war_result(const role_ptr& p_role, proto::client::gc_family_war_result_info_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (NULL == p_scene)
	{
		log_error("p_role[%lu] scene[%lu] not find", p_role->get_uid(), p_role->get_scene_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
	if (NULL == p_family_war_scene)
	{
		log_error("p_role[%lu] scene[%lu] not family_war_scene", p_role->get_uid(), p_role->get_scene_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	p_family_war_scene->get_family_war_result(p_role->get_uid(), reply);
}

uint32_t family_manager_t::ask_family_prayer_request(const role_ptr& p_role, const proto::client::cg_family_prayer_request& request)
{
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	Hero* hero_conf = NULL;
	if (request.has_hero_uid() && !request.hero_uid().empty()) {
		hero_ptr p_hero = p_role->get_hero_by_unique_id(common::string_util_t::string_to_uint64(request.hero_uid()));
		if (NULL == p_hero) {
			log_error("role[%lu] not has hero[%s]", p_role->get_uid(), request.hero_uid().c_str());
			return common::errcode_enum::notice_hero_null;
		}

		hero_conf = GET_CONF(Hero, p_hero->get_tid());
		if (hero_conf == NULL) {
			log_error("role[%lu] hero[%u] config not exists", p_role->get_uid(), p_hero->get_tid());
			return common::errcode_enum::notice_unknown;
		}
	} else if (request.hero_tid() > 0){
		hero_conf = GET_CONF(Hero, request.hero_tid());
		if (hero_conf == NULL) {
			log_error("role[%lu] Hero[%u] not exists", p_role->get_uid(), request.hero_tid());
			return common::errcode_enum::notice_unknown;
		}

		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_condition2);
		uint32_t condition = GET_COMPREHENSIVE_VALUE_1(conf);

		uint32_t need_num = hero_conf->compose_count() * ((float)condition / 10000);

		if (!item_manager_t::has_item(p_role, hero_conf->material_id(), need_num)) {
			FAMILY_LOG("role[%lu] material_id[%u] less need_num[%u]", p_role->get_uid(), hero_conf->material_id(), need_num);
			return common::errcode_enum::family_error_prayer_item_limit;
		}
	} else {
		log_error("role[%lu] ask_family_prayer_request hero_uid[%s] hero_tid[%u] error", p_role->get_uid(), request.hero_uid().c_str(), request.hero_tid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t max_prayer_num = 0;
	switch (hero_conf->rarity()) {
		case proto::common::hero_rarity_type_normal: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_hero_cnt0);
			max_prayer_num = GET_COMPREHENSIVE_VALUE_1(conf);
		}
		break;
		case proto::common::hero_rarity_type_rare: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_hero_cnt1);
			max_prayer_num = GET_COMPREHENSIVE_VALUE_1(conf);
		}
		break;
		case proto::common::hero_rarity_type_super_rare: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_hero_cnt2);
			max_prayer_num = GET_COMPREHENSIVE_VALUE_1(conf);
		}
		break;
		case proto::common::hero_rarity_type_super_super_rare: {
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_hero_cnt3);
			max_prayer_num = GET_COMPREHENSIVE_VALUE_1(conf);
		}
		break;
		default: {
			log_error("role[%lu] hero[%u] rarity[%u] type not define", p_role->get_uid(), hero_conf->id(), hero_conf->rarity());
			return common::errcode_enum::notice_unknown;
		}
		break;
	}

// 	if (request.hero_num() > max_prayer_num) {
// 		FAMILY_LOG("role[%lu] hero[%u] num[%u] > max_prayer_num[%u]", p_role->get_uid(), hero_conf->id(), request.hero_num(), max_prayer_num);
// 		return common::errcode_enum::family_error_prayer_item_limit;
// 	}

	proto::server::ge_family_prayer_request ge_request;
	ge_request.set_hero_tid(hero_conf->id());
	ge_request.set_hero_num(max_prayer_num);
	env::server->send_msg_to_center(op_cmd::ge_family_prayer_request, p_role->get_uid(), ge_request);

	return common::errcode_enum::error_ok;
}

uint32_t family_manager_t::family_prayer_gift_request(const role_ptr& p_role, const proto::client::cg_family_prayer_gift_request& request)
{
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	Hero* hero_conf = GET_CONF(Hero, request.hero_tid());
	if (hero_conf == NULL) {
		log_error("role[%lu] hero[%u] config not exists", p_role->get_uid(), request.hero_tid());
		return common::errcode_enum::notice_unknown;
	}
	
	if (!item_manager_t::has_item(p_role, hero_conf->material_id(), 1)) {
		FAMILY_LOG("role[%lu] material_id[%u] less then [%u]", p_role->get_uid(), hero_conf->material_id(), 1);
		return common::errcode_enum::family_error_prayer_gift_once_to_much;
	}

	proto::server::ge_family_prayer_gift_request ge_request;
	// 先扣除，失败返还
	item_manager_t::remove_item(p_role, hero_conf->material_id(), 1, log_enum::source_type_family_prayer_gift, 0, ge_request.mutable_change_data());

	ge_request.set_role_id(common::string_util_t::string_to_uint64(request.role_id()));
	ge_request.set_hero_tid(request.hero_tid());
	ge_request.set_hero_num(1);
	ge_request.set_hero_rarity(hero_conf->rarity());
	ge_request.set_hero_material_id(hero_conf->material_id());
	env::server->send_msg_to_center(op_cmd::ge_family_prayer_gift_request, p_role->get_uid(), ge_request);

	return common::errcode_enum::error_ok;
}

void family_manager_t::family_prayer_gift_reply(const role_ptr& p_role, const proto::server::eg_family_prayer_gift_failed_notify& notify, proto::client::gc_family_prayer_gift_reply& reply)
{
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return;
	}

	reply.set_reply_code(notify.reply_code());

	if (notify.reply_code() != common::errcode_enum::error_ok) {
		// 失败返还道具
		item_manager_t::add_item(p_role, notify.hero_material_id(), notify.hero_num(), log_enum::source_type_family_prayer_gift_back, 0);
		return;
	}

	reply.set_role_id(common::string_util_t::uint64_to_string(notify.role_id()));
	reply.set_hero_tid(notify.hero_tid());
	reply.set_hero_num(notify.hero_num());

	proto::common::role_change_data* p_role_change_data = reply.mutable_change_data();
	if (p_role_change_data) {
		p_role_change_data->CopyFrom(notify.change_data());
		p_role->get_personal_info(p_role_change_data->mutable_per_info());
	}

	if (notify.has_rwd_item()) {
		if (!drop_manager_t::drop(p_role, notify.rwd_item(), log_enum::source_type_family_prayer_gift_rwd,
			proto::common::drop_sys_type_family_prayer, 0, p_role_change_data))
		{
			log_error("role[%lu] give family prayer reward[%u] error", p_role->get_uid(), notify.rwd_item());
			return;
		}
	}
}

uint32_t family_manager_t::family_shop_buy_request(const role_ptr& p_role, uint32_t index, uint32_t shop_id, uint32_t num)
{
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return common::errcode_enum::notice_unknown;
	}

	if (p_role->get_family_id() == 0) {
		log_error("role[%lu] not join family", p_role->get_uid());
		return common::errcode_enum::notice_no_family;
	}

	ShopItemTable* p_ShopItem = GET_CONF(ShopItemTable, shop_id);
	if (!p_ShopItem) {
		log_error("role[%lu] family_shop[%u] not find", p_role->get_uid(), shop_id);
		return common::errcode_enum::random_shop_error_item_not_find;
	}

	if (num == 0) {
		log_error("role[%lu] family shop buy count == 0", p_role->get_uid());
		return common::errcode_enum::random_shop_error_count_zero;
	}

	if (p_ShopItem->personal_level() != 0 && p_role->get_level() < p_ShopItem->personal_level()) {
		log_error("role[%lu] level[%u] < shop_item[%u] personal_level[%u]", p_role->get_uid(), p_role->get_level(), shop_id, p_ShopItem->personal_level());
		return common::errcode_enum::family_shop_personal_level_limit;
	}

	money_ptr p_money = p_role->get_money();
	if (p_money == NULL) {
		log_error("role[%lu] p_money null error", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t money = p_money->get_money((proto::common::MONEY_TYPE)p_ShopItem->money_type());
	uint32_t cost = p_ShopItem->price() * num;
	if (money < cost) {
		FAMILY_LOG("role[%lu] money_type[%u] money_value[%u] family_shop_item_id[%u] money[%u] insufficient",
			p_role->get_uid(), p_ShopItem->money_type(), money, shop_id, cost);

		switch (p_ShopItem->money_type()) {
			case proto::common::MONEY_TYPE_YUANBAO: {
				return common::errcode_enum::notice_shop_error2;
				break;
			}
			case proto::common::MONEY_TYPE_SILVER: {
				return common::errcode_enum::notice_shop_error3;
				break;
			}
			case proto::common::MONEY_TYPE_COPPER: {
				return common::errcode_enum::notice_shop_error4;
				break;
			}
			default: {
				return common::errcode_enum::notice_shop_error5;
			}
		}
	}

	std::map<uint32_t, uint32_t> item_map;
	item_map.insert(std::make_pair(p_ShopItem->item_id(), p_ShopItem->item_val() * num));

	if (!item_manager_t::check_add_items(p_role, item_map)) {
		SHOP_LOG("role[%lu] backpack is not enough", p_role->get_uid());
		return common::errcode_enum::notice_shop_error8;
	}

	proto::server::ge_family_shop_buy_request req;
	req.set_index(index);
	req.set_shop_item(shop_id);
	req.set_num(num);
	env::server->send_msg_to_center(op_cmd::ge_family_shop_buy_request, p_role->get_uid(), req);

	return common::errcode_enum::error_ok;
}

void family_manager_t::family_shop_center_check_back(const role_ptr& p_role, 
	const proto::server::eg_family_shop_buy_reply& center_reply, 
	proto::client::gc_family_shop_buy_reply& reply)
{
	if (NULL == p_role) {
		log_error("NULL == p_role");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	ShopItemTable* p_ShopItem = GET_CONF(ShopItemTable, center_reply.shop_item());
	if (!p_ShopItem) {
		log_error("role[%lu] family_shop_item[%u] not find", p_role->get_uid(), center_reply.shop_item());
		reply.set_reply_code(common::errcode_enum::random_shop_error_item_not_find);
		return;
	}

	money_ptr p_money = p_role->get_money();
	if (p_money == NULL) {
		log_error("role[%lu] p_money null error", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	uint32_t money = p_money->get_money((proto::common::MONEY_TYPE)p_ShopItem->money_type());
	uint32_t cost = p_ShopItem->price() * center_reply.num();
	if (money < cost) {
		FAMILY_LOG("role[%lu] money_type[%u] money_value[%u] family_shop_item_id[%u] money[%u] insufficient",
			p_role->get_uid(), p_ShopItem->money_type(), money, center_reply.shop_item(), cost);

		switch (p_ShopItem->money_type()) {
			case proto::common::MONEY_TYPE_YUANBAO: {
				reply.set_reply_code(common::errcode_enum::notice_shop_error2);
				return;
				break;
			}
			case proto::common::MONEY_TYPE_SILVER: {
				reply.set_reply_code(common::errcode_enum::notice_shop_error3);
				return;
				break;
			}
			case proto::common::MONEY_TYPE_COPPER: {
				reply.set_reply_code(common::errcode_enum::notice_shop_error4);
				return;
				break;
			}
			default: {
				reply.set_reply_code(common::errcode_enum::notice_shop_error5);
				return;
			}
		}
	}

	std::map<uint32_t, uint32_t> item_map;
	uint32_t item_num = p_ShopItem->item_val() * center_reply.num();
	item_map.insert(std::make_pair(p_ShopItem->item_id(), item_num));

	if (!item_manager_t::check_add_items(p_role, item_map)) { // 这里不返回只输出，放不下自动发邮件
		SHOP_LOG("role[%lu] backpack is not enough", p_role->get_uid());
	}

	// 直接扣钱给东西
	if (!money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)p_ShopItem->money_type(), cost, log_enum::source_type_family_shop_buy,
		center_reply.shop_item(), false, reply.mutable_change_data()))
	{
		log_error("role[%lu] use_money money error", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_shop_error5);
		return;
	}

	item_manager_t::add_items(p_role, item_map, log_enum::source_type_family_shop_item, center_reply.shop_item(), reply.mutable_change_data());

	if (reply.mutable_change_data()) p_role->get_personal_info(reply.mutable_change_data()->mutable_per_info());

	reply.set_reply_code(common::errcode_enum::error_ok);
	proto::common::family_shop_data* p_shop_data = reply.mutable_shop_data();
	if (p_shop_data) p_shop_data->CopyFrom(center_reply.shop_data());
	proto::common::family_shop_personal_data* p_personal_data = reply.mutable_personal_data();
	if (p_personal_data) p_personal_data->CopyFrom(center_reply.personal_data());

	log_wrapper_t::send_random_shop_log(p_role->get_uid(), 0, center_reply.shop_item(), p_ShopItem->money_type(), cost, p_ShopItem->item_id(), item_num, 0, 0, log_enum::source_type_family_shop_buy);
}

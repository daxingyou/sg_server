#include "patrol_hire_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "tblh/PatrolHire.tbls.h"
#include "tblh/PatrolLevel.tbls.h"
#include "tblh/Patrolstar.tbls.h"
#include "utility.hpp"
#include "common/config_mgr.h"
#include "role/money_manager.hpp"
#include "item/item_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/comprehensive_common.hpp"
#include "item/drop_manager.hpp"
#include "log/log_wrapper.hpp"

uint32_t patrol_hire_manager_t::get_patrol_hire_reward(const role_ptr& p_role, proto::client::patrol_hire_rwd* p_rwd)
{
	if (!p_role) {
		log_error("p_role == NULL");
		return common::errcode_enum::notice_unknown;
	}

	if (!p_rwd) {
		log_error("p_role == NULL");
		return common::errcode_enum::notice_unknown;
	}

	const patrol_hire_ptr& p_patrol_hire = p_role->get_patrol_hire();
	if (!p_patrol_hire) {
		log_error("role[%lu] p_patrol_hire == NULL", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	if (p_patrol_hire->get_map_id() == 0) {
		PATROL_HIRE_LOG("role[%lu] map == 0", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	PatrolHire* p_conf_PatrolHire = GET_CONF(PatrolHire, p_patrol_hire->get_map_id());
	if (NULL == p_conf_PatrolHire) {
		log_error("role[%lu] map[%u] == NULL", p_role->get_uid(), p_patrol_hire->get_map_id());
		return common::errcode_enum::notice_unknown;
	}

	PatrolLevel* p_conf_PatrolLevel = GET_CONF(PatrolLevel, p_role->get_privilege_level());
	if (NULL == p_conf_PatrolLevel) {
		log_error("role[%lu] PatrolLevel[%u] == NULL", p_role->get_uid(), p_role->get_privilege_level());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t patrolhire_cd = p_conf_PatrolLevel->time();
	if (patrolhire_cd == 0) {
		log_error("role[%lu] PatrolLevel[%u] cd == 0", p_role->get_uid(), p_role->get_privilege_level());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t curr_time = common::time_util_t::now_time();
	if (p_patrol_hire->get_begin_time() > curr_time) {
		log_error("role[%lu] begin_time > curr_time", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::patrol_max_time);
	uint32_t patrol_max_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	uint32_t intervals_times = curr_time - p_patrol_hire->get_begin_time() > patrol_max_time ? patrol_max_time : curr_time - p_patrol_hire->get_begin_time();
	uint32_t reward_times = intervals_times / patrolhire_cd;
	if (reward_times == 0) {
		PATROL_HIRE_LOG("role[%lu] patrol_hire reward time less", p_role->get_uid());
		return common::errcode_enum::auto_patrol_error_1;
	}

	float star_add = (float)get_star_add(p_role->get_all_hero_star()) / 10000;
	uint32_t count = reward_times * (1 + star_add);
	if (count > 0) {
		send_patrol_hire_reward(p_role, p_conf_PatrolHire, count, p_rwd->mutable_rcd());
	}

	p_rwd->set_map_id(p_patrol_hire->get_map_id());
	p_rwd->set_duration(intervals_times);

	PATROL_HIRE_LOG("role[%lu] get patrol_hire reward count[%d]", p_role->get_uid(), count);
	log_wrapper_t::send_patrol_hire_log(p_role->get_uid(), p_patrol_hire->get_begin_time(), curr_time, count, 0, 0, p_patrol_hire->get_map_id(), log_enum::source_type_patrol_hire_cal);

	p_patrol_hire->reset_begin_time();
	return common::errcode_enum::error_ok;
}

void patrol_hire_manager_t::fast_patrol_hire(const role_ptr& p_role, uint32_t map_id, proto::client::gc_fast_patrol_hire_reply& reply)
{
	if (!p_role) {
		log_error("p_role == NULL");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const patrol_hire_ptr& p_patrol_hire = p_role->get_patrol_hire();
	if (!p_patrol_hire) {
		log_error("role[%lu] p_patrol_hire == NULL", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (map_id == 0) {
		PATROL_HIRE_LOG("role[%lu] fast_patrol_hire map == 0", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	PatrolHire* p_conf_PatrolHire = GET_CONF(PatrolHire, map_id);
	if (NULL == p_conf_PatrolHire) {
		log_error("role[%lu] fast_patrol_hire map[%u] == NULL", p_role->get_uid(), map_id);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (p_role->get_level() < p_conf_PatrolHire->level()) {
		log_error("role[%lu] map[%u] need level[%u]", p_role->get_uid(), map_id, p_conf_PatrolHire->level());
		reply.set_reply_code(common::errcode_enum::auto_patrol_error_2);
		return;
	}

	PatrolLevel* p_conf_PatrolLevel = GET_CONF(PatrolLevel, p_role->get_privilege_level());
	if (NULL == p_conf_PatrolLevel) {
		log_error("role[%lu] PatrolLevel[%u] == NULL", p_role->get_uid(), p_role->get_privilege_level());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	uint32_t patrolhire_cd = p_conf_PatrolLevel->time();
	if (patrolhire_cd == 0) {
		log_error("role[%lu] PatrolLevel[%u] cd == 0", p_role->get_uid(), p_role->get_privilege_level());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::fast_partol_number);
	uint32_t fast_partol_number = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (p_patrol_hire->get_fast_patrol_count() >= fast_partol_number) {
		log_error("role[%lu] fast_patrol_count[%u] > %u", p_role->get_uid(), p_patrol_hire->get_fast_patrol_count(), fast_partol_number);
		reply.set_reply_code(common::errcode_enum::auto_patrol_error_4);
		return;
	}

	money_ptr p_money = p_role->get_money();
	if (p_money == NULL) {
		log_error("user[%lu] p_money null error", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	uint32_t money = p_money->get_money(proto::common::MONEY_TYPE_YUANBAO);
	p_conf = GET_CONF(Comprehensive, comprehensive_common::fast_partol_money);
	uint32_t cost = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (money < cost) {
		PATROL_HIRE_LOG("role[%lu] fast_patrol_hire money_type[%u] money_value[%u] < %u",
			p_role->get_uid(), proto::common::MONEY_TYPE_YUANBAO, money, cost);
		reply.set_reply_code(common::errcode_enum::auto_patrol_error_3);
		return;
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::fast_partol_time);
	uint32_t fast_partol_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	uint32_t reward_times = fast_partol_time / patrolhire_cd;
	if (reward_times == 0) {
		log_error("role[%lu] fast_partol_time / patrolhire_cd == 0", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	proto::client::patrol_hire_rwd* p_rwd = reply.mutable_reward();
	if (!p_rwd) {
		log_error("role[%lu] p_rwd == NULL", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, cost, log_enum::source_type_patrol_hire_cal,
		0, false, p_rwd->mutable_rcd())) {
		log_error("role[%lu] use_money money error", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	p_rwd->set_map_id(map_id);
	p_rwd->set_duration(fast_partol_time);

	float star_add = (float)get_star_add(p_role->get_all_hero_star()) / 10000;
	uint32_t count = reward_times * (1 + star_add);
	if (count > 0) {
		send_patrol_hire_reward(p_role, p_conf_PatrolHire, count, p_rwd->mutable_rcd());
	}

	PATROL_HIRE_LOG("role[%lu] fast_patrol_hire count[%d]", p_role->get_uid(), count);
	log_wrapper_t::send_patrol_hire_log(p_role->get_uid(), 0, 0, count, 0, 0, map_id, log_enum::source_type_patrol_point_exchange);

	p_patrol_hire->add_fast_patrol_count();
	p_patrol_hire->peek_data(reply.mutable_info());
	reply.set_reply_code(common::errcode_enum::error_ok);
}

void patrol_hire_manager_t::change_patrol_hire_map(const role_ptr& p_role, uint32_t map_id, proto::client::gc_change_patrol_hire_reply& reply)
{
	if (!p_role) {
		log_error("p_role == NULL");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const patrol_hire_ptr& p_patrol_hire = p_role->get_patrol_hire();
	if (!p_patrol_hire) {
		log_error("role[%lu] p_patrol_hire == NULL", p_role->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	PatrolHire* p_conf_PatrolHire = GET_CONF(PatrolHire, map_id);
	if (NULL == p_conf_PatrolHire) {
		log_error("role[%lu] map[%u] == NULL", p_role->get_uid(), map_id);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (p_role->get_level() < p_conf_PatrolHire->level()) {
		log_error("role[%lu] map[%u] need level[%u]", p_role->get_uid(), map_id, p_conf_PatrolHire->level());
		reply.set_reply_code(common::errcode_enum::auto_patrol_error_2);
		return;
	}

	if (p_patrol_hire->get_map_id() != 0) {
		patrol_hire_manager_t::get_patrol_hire_reward(p_role, reply.mutable_reward());
	}

	PATROL_HIRE_LOG("role[%lu] change patrol_hire map[%u]", p_role->get_uid(), map_id);
	log_wrapper_t::send_patrol_hire_log(p_role->get_uid(), common::time_util_t::now_time(), 0, 0, 0, 0, map_id, log_enum::source_type_patrol_hire_enter_auto_patrol);

	p_patrol_hire->set_map_id(map_id);
	p_patrol_hire->peek_data(reply.mutable_info());
	reply.set_reply_code(common::errcode_enum::error_ok);
}

uint32_t patrol_hire_manager_t::get_star_add(uint32_t star)
{
	struct ExecStar : public execEntry< Patrolstar >
	{
		uint32_t star_num;
		uint32_t star_add;
		ExecStar(uint32_t star) : star_num(star), star_add(0) {};

		bool exec(Patrolstar* entry)
		{
			if (NULL == entry)
			{
				return true;
			}

			if (star_num < entry->id()) {
				return false;
			}

			star_add = entry->drop_add();

			return true;
		}

	}exec(star);

	if (NULL == PatrolstarManager::getInstancePtr())
	{
		log_error("NULL == PatrolstarManager::getInstancePtr()");
		return 0;
	}

	PatrolstarManager::getInstancePtr()->execEveryData(exec);

	return exec.star_add;
}

void patrol_hire_manager_t::send_patrol_hire_reward(const role_ptr& p_role, PatrolHire* p_conf, uint32_t count, proto::common::role_change_data* p_rcd)
{
	if (!p_role) {
		log_error("p_role == NULL");
		return;
	}

	if (!p_conf) {
		log_error("p_conf == NULL");
		return;
	}

	std::map<uint32_t, uint32_t> drop_ids;
	uint32_t drop_thousand_num = count / 1000;
	if (drop_thousand_num > 0) {
		drop_ids.insert(std::make_pair(p_conf->drop_thousand(), drop_thousand_num));
	}

	count = count % 1000;
	uint32_t drop_hundred_num = count / 100;
	if (drop_hundred_num > 0) {
		drop_ids.insert(std::make_pair(p_conf->drop_hundred(), drop_hundred_num));
	}

	count = count % 100;
	uint32_t drop_ten_num = count / 10;
	if (drop_ten_num > 0) {
		drop_ids.insert(std::make_pair(p_conf->drop_ten(), drop_ten_num));
	}

	uint32_t drop_one_num = count % 10;
	if (drop_one_num > 0) {
		drop_ids.insert(std::make_pair(p_conf->drop_one(), drop_one_num));
	}

	drop_manager_t::drop_ex(p_role, drop_ids, log_enum::source_type_patrol_hire_cal, proto::common::drop_sys_type_patrol_hire, 0, p_rcd, true);
}

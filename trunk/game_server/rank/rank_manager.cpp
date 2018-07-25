#include "rank_manager.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "tblh/rank_common.hpp"

void rank_manager_t::update_criminal(uint64_t uid, uint32_t pk_value)
{
	proto::server::ge_update_rank_notify ntf;
	ntf.set_rank_type(common::rank_common::criminal_rank_0);
	ntf.set_owner(uid);
	ntf.set_key(pk_value);
	env::server->send_msg_to_center(op_cmd::ge_update_rank_notify, uid, ntf);
}

void rank_manager_t::update_achievement(uint64_t uid, uint32_t value, uint32_t level)
{
	proto::server::ge_update_rank_notify ntf;
	ntf.set_rank_type(common::rank_common::achievement_rank_0);
	ntf.set_owner(uid);
	ntf.set_key(value);
	proto::common::rank_data_extra* p_extra = ntf.mutable_extra();
	if (p_extra) {
		proto::common::rank_score_achievement* p_achieve = p_extra->mutable_achieve();
		if (p_achieve) {
			p_achieve->set_level(level);
		}
	}
	env::server->send_msg_to_center(op_cmd::ge_update_rank_notify, uid, ntf);
}

void rank_manager_t::update_tower(uint64_t uid, uint32_t cur_tower, uint32_t cost_day, uint32_t level)
{
	proto::server::ge_update_rank_notify ntf;
	ntf.set_rank_type(common::rank_common::towers_rank_0);
	ntf.set_owner(uid);
	ntf.set_key(cur_tower);
	proto::common::rank_data_extra* p_extra = ntf.mutable_extra();
	if (p_extra) {
		proto::common::rank_score_tower* p_tower = p_extra->mutable_tower();
		if (p_tower) {
			p_tower->set_cost_day(cost_day);
			p_tower->set_level(level);
		}
	}
	env::server->send_msg_to_center(op_cmd::ge_update_rank_notify, uid, ntf);
}

void rank_manager_t::update_recruit(uint64_t uid, uint32_t val)
{
	proto::server::ge_update_rank_notify ntf;
	ntf.set_rank_type(common::rank_common::recruit_rank_0);
	ntf.set_owner(uid);
	ntf.set_key(val);
	env::server->send_msg_to_center(op_cmd::ge_update_rank_notify, uid, ntf);
}

void rank_manager_t::update_copper(uint64_t uid, uint32_t val)
{
	proto::server::ge_update_rank_notify ntf;
	ntf.set_rank_type(common::rank_common::consume_copper_rank_0);
	ntf.set_owner(uid);
	ntf.set_key(val);
	env::server->send_msg_to_center(op_cmd::ge_update_rank_notify, uid, ntf);
}

void rank_manager_t::update_gold(uint64_t uid, uint32_t val)
{
	proto::server::ge_update_rank_notify ntf;
	ntf.set_rank_type(common::rank_common::consume_golden_rank_0);
	ntf.set_owner(uid);
	ntf.set_key(val);
	env::server->send_msg_to_center(op_cmd::ge_update_rank_notify, uid, ntf);
}

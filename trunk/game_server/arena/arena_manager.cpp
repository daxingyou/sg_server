#include "arena/arena_manager.hpp"
#include "activity/activity_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/ArenaClassTable.tbls.h"
#include "tblh/ArenaBattleAwardTable.tbls.h"
#include "tblh/ArenaWeekAwardTable.tbls.h"
#include "tblh/Activities.tbls.h"
#include "role/money_manager.hpp"
#include "item/drop_manager.hpp"
#include "item/item_manager.hpp"
#include "mail/mail_manager.hpp"
#include "time_manager.hpp"
#include "role/role_manager.hpp"
#include "scene/scene_manager.hpp"
#include "achieve/achieve_common.hpp"

bool arena_manager_t::init()
{
	//读取活动表
	auto reset_time_id = GET_CONF(Comprehensive, comprehensive_common::arena_reset_time_id);
	uint32_t time_id = GET_COMPREHENSIVE_VALUE_1(reset_time_id);
	if (0 == time_id)
	{
		log_error("arena reset time_id invalid");
		return false;
	}
	time_manager_t::register_func(time_id, arena_manager_t::on_season);

	return true;
}

void arena_manager_t::on_season(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		//重置玩家数据
		role_manager_t::reset_arena();
		role_manager_t::award_arena();
	}
}

uint32_t arena_manager_t::enter_arena(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role NULL error");
		return common::errcode_enum::notice_unknown;
	}

	if (p_role->get_troop_id() > 0)
	{
		ARENA_LOG("role[%lu] troop can not enter arena scene", p_role->get_uid());
		return common::errcode_enum::user_troop_can_not_enter_arena;
	}

	if (p_role->get_mode_state() == proto::common::role_mode_state_trade)
	{
		ARENA_LOG("role[%lu] trade can not enter arena scene", p_role->get_uid());
		return common::errcode_enum::user_trade_can_not_enter_arena;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::arena_map_id);
	uint32_t map_id = GET_COMPREHENSIVE_VALUE_1(p_conf);
	return scene_manager_t::role_jump_scene(p_role, proto::server::user_change_scene, map_id);
}

void arena_manager_t::leave_arena(const role_ptr& p_role)
{
	if (NULL == p_role) {
		log_error("p_role NULL error");
		return;
	}

	do
	{
		uint32_t last_map_tid = p_role->get_role_scene_info().get_last_map_tid();
		if (last_map_tid == 0) {
			break;
		}

		if (last_map_tid == p_role->get_scene_id()) {
			break;
		}

		if (errcode_enum::error_ok == scene_manager_t::role_jump_scene(p_role, proto::server::user_change_scene, last_map_tid, p_role->get_role_scene_info().get_last_pos_x(), p_role->get_role_scene_info().get_last_pos_y(), p_role->get_role_scene_info().get_last_dir())) {
			return;
		}
	} while (0);

	// 发送任何错误去默认地图
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::imperial_city_id);
	uint32_t born_map_tid = GET_COMPREHENSIVE_VALUE_1(p_conf);
	scene_manager_t::role_jump_scene(p_role, proto::server::user_change_scene, born_map_tid);
}

uint32_t arena_manager_t::join_arena(role_ptr p_role, proto::common::role_change_data *p_data)
{
	if (NULL == p_role)
	{
		log_error("arena p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == p_data)
	{
		log_error("arena p_data NULL");
		return errcode_enum::notice_unknown;
	}
	if (p_role->get_arena().is_start())
	{
		log_error("role[%lu] join_arena has start", p_role->get_uid());
		return errcode_enum::notice_arena_has_start;
	}

	// 检查活动有没有开启
	activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
	if (NULL == p_activity_mgr)
	{
		log_error("role[%lu] join_arena NULL == p_activity_mgr", p_role->get_uid());
		return errcode_enum::notice_unknown;
	}
	if (!p_activity_mgr->check_open(activity_arena))
	{
		log_error("role[%lu] join_arena activity_arena not open", p_role->get_uid());
		return errcode_enum::notice_activity_not_open;
	}
	// 门票次数判断
	if (errcode_enum::error_ok != item_manager_t::remove_item(p_role, get_ticket_item(), 1, log_enum::source_type_arena_begin, 0, p_data))
	{
		log_error("role[%lu] join_arena has no ticket", p_role->get_uid());
		return errcode_enum::notice_arena_no_ticket;
	}

	// 记录开始标记
	p_role->get_arena().set_start(true);
	p_role->get_arena().save_self();
	return errcode_enum::error_ok;
}

uint32_t arena_manager_t::buy_ticket(role_ptr p_role, proto::common::role_change_data *p_data)
{
	if (NULL == p_role)
	{
		log_error("arena p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (NULL == p_data)
	{
		log_error("p_data NULL");
		return errcode_enum::notice_unknown;
	}

	//auto max_num_conf = GET_CONF(Comprehensive, comprehensive_common::arena_ticket_max_num);
	//uint32_t max_num = GET_COMPREHENSIVE_VALUE_1(max_num_conf);
	//// 门票可叠加 但不可超过一定上限
	//uint32_t tickets = p_role->get_arena().get_arena_tickets();
	//if (tickets + 1 >= max_num)
	//{
	//	return errcode_enum::notice_arena_ticket_max_num;
	//}

	auto cost_conf = GET_CONF(Comprehensive, comprehensive_common::arena_ticket_cost);
	uint32_t cost = GET_COMPREHENSIVE_VALUE_1(cost_conf);

	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, cost, log_enum::source_type_arena_buy_ticket,
									0, false, p_data))
	{
		log_error("role[%lu] buy_ticket money_not_enough", p_role->get_uid());
		return errcode_enum::notice_gold_money_not_enough;
	}
	
	//p_role->get_arena().set_arena_tickets(tickets + 1);


	return errcode_enum::error_ok;
}

void arena_manager_t::do_next_day(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("arena p_role NULL");
		return;
	}

	//auto max_num_conf = GET_CONF(Comprehensive, comprehensive_common::arena_ticket_max_num);
	//uint32_t max_num = GET_COMPREHENSIVE_VALUE_1(max_num_conf);

	// 没分数并且没段数 就认为没参加过竞技场
	if (p_role->get_arena().get_score() == 0 && p_role->get_arena().get_level() == 0)
		return;

// 	// 每天奖励
// 	std::map<uint32_t, ArenaDailyAwardTable*> confs;
// 	GET_ALL_CONF(ArenaDailyAwardTable, confs);
// 	for (auto conf : confs)
// 	{
// 		ArenaDailyAwardTable* p_conf = conf.second;
// 		if (NULL != p_conf)
// 		{
// 			if (p_role->get_arena().get_score() >= p_conf->score_min() && p_role->get_arena().get_score() < p_conf->score_max())
// 			{
// 				// todo 全局邮件功能 钱海江实现
// 				mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, p_conf->mail_id(), 
// 										  proto::common::MAIL_GET_TYPE_ARENA);
// 				break;
// 			}
// 		}
// 	}
}

uint32_t arena_manager_t::match_rank_for_battle(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("arena p_role NULL");
		return errcode_enum::notice_unknown;
	}
	if (!p_role->get_arena().is_start())
	{
		log_error("arena role[%lu] not join but match", p_role->get_uid());
		return errcode_enum::notice_arena_not_join;
	}
	if (p_role->get_arena().is_fighting())
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::arena_battle_time_max);
		uint32_t max_fight_time = GET_COMPREHENSIVE_VALUE_1(conf);
		
		// 防止状态卡死的时间判断
		if (common::time_util_t::now_time() - p_role->get_arena().get_last_fight_time() >= max_fight_time)
		{
			p_role->get_arena().set_is_fighting(false);
			p_role->get_arena().set_last_fight_time(0);
			p_role->get_arena().save_self();
			p_role->change_object_state(proto::common::object_state_normal);
		}
		else
		{
			log_error("arena role[%lu] is fighting", p_role->get_uid());
			return errcode_enum::notice_is_fighting;
		}
	}
	// 判断活动时间
	//if (!p_activity_mgr->check_open(activity_arena))
	//{
	//	return errcode_enum::notice_activity_not_open;
	//}

	if (p_role->get_object_state() != proto::common::object_state_arena_match)
	{
		p_role->change_object_state(proto::common::object_state_arena_match);
	}

	proto::server::gr_arena_match_request msg;
	msg.set_score(p_role->get_arena().get_score());
	msg.set_arena_level(p_role->get_arena().get_level());
	p_role->peek_cross_data(msg.mutable_user(), proto::common::role_cross_type_arena);

	env::server->send_msg_to_area(op_cmd::gr_arena_match_request, p_role->get_uid(), msg);
	return errcode_enum::error_ok;
}

void arena_manager_t::cancel_match(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("arena p_role NULL");
		return;
	}
	p_role->get_arena().set_is_fighting(false);
	p_role->get_arena().set_last_fight_time(0);
	p_role->get_arena().save_self();
	p_role->change_object_state(proto::common::object_state_normal);

	proto::client::gc_arena_cancel_match_reply reply;
	reply.set_reply_code(0);
	p_role->send_msg_to_client(op_cmd::gc_arena_cancel_match_reply, reply);
}

void arena_manager_t::deal_match_rank_result(role_ptr p_role, proto::server::rg_arena_match_reply& msg)
{
	if (NULL == p_role)
	{
		log_error("arena p_role NULL");
		return;
	}
	// 匹配成功就进入战斗了 修改战斗标记
	if (msg.reply_code() == errcode_enum::error_ok)
	{
		p_role->get_arena().set_is_fighting(true);
		p_role->get_arena().set_last_fight_time(common::time_util_t::now_time());
		p_role->get_arena().save_self();
		p_role->change_object_state(proto::common::object_state_fighting);
	}

	proto::client::gc_arena_match_reply msg_reply;
	msg_reply.set_reply_code(msg.reply_code());
	msg_reply.set_target_score(msg.target_score());
	msg_reply.set_target_arena_level(msg.target_arena_level());
	msg_reply.mutable_target_user()->CopyFrom(msg.target_user());
	p_role->send_msg_to_client(op_cmd::gc_arena_match_reply, msg_reply);
}

void arena_manager_t::deal_arena_rank_battle_result(role_ptr p_role, proto::server::rg_arena_battle_result_notify& msg)
{
	if (NULL == p_role)
	{
		log_error("arena p_role NULL");
		return;
	}
	arena_t& arena_data = p_role->get_arena();
	uint32_t own_score = arena_data.get_score();
	uint32_t tar_score = msg.target_score();
	auto p_arena_class_conf = GET_CONF(ArenaClassTable, arena_data.get_level());
	if (NULL == p_arena_class_conf)
	{
		log_error("NULL == p_arena_class_conf[%d]", arena_data.get_level());
		return;
	}
	
	proto::client::gc_arena_battle_result_notify msg_reply;

	bool is_end = false;

	if (msg.is_win())
	{
		auto max_win_times_conf = GET_CONF(Comprehensive, comprehensive_common::arena_win_max_times);
		uint32_t max_win_times = GET_COMPREHENSIVE_VALUE_1(max_win_times_conf);

		arena_data.set_win_times(arena_data.get_win_times() + 1);
		arena_data.set_total_win_times(arena_data.get_total_win_times() + 1);
		if (arena_data.get_win_times() >= max_win_times)
		{
			arena_data.set_start(false);
			is_end = true;
		}
		if (arena_data.get_win_times() > arena_data.get_best_win_times())
		{
			arena_data.set_best_win_times(arena_data.get_win_times());
		}
		money_manager_t::add_honor(p_role, p_arena_class_conf->win_honor(),
			log_enum::source_type_arena_battle_win, 0, msg_reply.mutable_rcd());

		achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_ARENA_BATTLE_SINGLE_WIN_NUMBER, arena_data.get_win_times());
		achieve_common_t::notify_progress_state(p_role->get_uid(),proto::common::ACHIEVE_ARENA_BATTLE_TOTAL_WIN_NUMBER, 0);
	}
	else
	{
		auto max_lose_times_conf = GET_CONF(Comprehensive, comprehensive_common::arena_lose_max_times);
		uint32_t max_lose_times = GET_COMPREHENSIVE_VALUE_1(max_lose_times_conf);
		arena_data.set_lose_times(arena_data.get_lose_times() + 1);
		if (arena_data.get_lose_times() >= max_lose_times)
		{
			arena_data.set_start(false);
			is_end = true;
		}
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_HONOR, p_arena_class_conf->lose_honor(),
			log_enum::source_type_arena_battle_lose, 0, true, msg_reply.mutable_rcd());
	}

	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_ARENA_BATTLE_TOTAL_NUMBER);
	
	uint32_t win_times = arena_data.get_win_times();
	// 结束
	if(is_end)
	{
		ArenaBattleAwardTable *p_award_table = GET_CONF(ArenaBattleAwardTable, win_times);
		if (NULL == p_award_table)
		{
			log_error("arena p_award_table NULL win_times[%d]", win_times);
		}
		else
		{
			drop_manager_t::drop(p_role, p_award_table->drop_id(), log_enum::source_type_arena_battle_win,
								 proto::common::drop_sys_type_arena, win_times, msg_reply.mutable_rcd());
		}
	}

	arena_data.set_is_fighting(false);
	arena_data.set_last_fight_time(0);
	p_role->change_object_state(proto::common::object_state_normal);

	// 获得奖励分
	int32_t reward_score = get_battle_reward_score(p_role->get_uid(), own_score, tar_score, msg.is_win());
	arena_data.set_score(arena_data.get_score() + reward_score);
	// 获得最新段位
	uint32_t new_arena_level = get_level_by_score(msg.is_win(), arena_data.get_score(), arena_data.get_level());
	arena_data.set_level(new_arena_level);
	arena_data.set_total_fight_times(arena_data.get_total_fight_times() + 1);
	
	//通知center
	proto::server::ge_arena_fight_data_notify msg_to_center;
	msg_to_center.set_fight_value(p_role->get_fighting());
	p_role->peek_fight_data(msg_to_center.mutable_fight_data());
	env::server->send_msg_to_center(op_cmd::ge_arena_fight_data_notify, p_role->get_uid(), msg_to_center);
	
	msg_reply.set_is_win(msg.is_win());
	arena_data.peek_data(msg_reply.mutable_arena_data());
//	msg_reply.mutable_target_user()->CopyFrom(msg.target());
	msg_reply.set_own_rank_up(msg.own_rank_up());
	msg_reply.set_own_new_rank(msg.own_new_rank());
	p_role->get_personal_info(msg_reply.mutable_rcd()->mutable_per_info());
	p_role->send_msg_to_client(op_cmd::gc_arena_battle_result_notify, msg_reply);
	// 结束
	if (is_end)
	{
		//客户端要求发完消息之后再重置数据
		arena_data.set_lose_times(0);
		arena_data.set_win_times(0);
	}
	arena_data.save_self();
}

void arena_manager_t::arena_season_end()
{
	// todo 全局邮件功能 钱海江实现
	// 赛季奖励
// 	std::map<uint32_t, ArenaSeasonAwardTable*> confs;
// 	GET_ALL_CONF(ArenaSeasonAwardTable, confs);
// 	for (auto conf : confs)
// 	{
// 		ArenaSeasonAwardTable* p_conf = conf.second;
// 		if (NULL != p_conf)
// 		{
// 			mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, p_conf->mail_id(), proto::common::MAIL_GET_TYPE_ARENA);
// 			
// 		}
// 	}
}

uint32_t arena_manager_t::get_level_by_score(bool is_win, uint32_t score, uint32_t arena_level)
{
	// 如果在1段位并且输了 就不用重算段位了
	if (arena_level == 1 && !is_win)
		return arena_level;
	
	// 如果在最高段位 并且赢了 也不用重算段位了
	if (NULL == ArenaClassTableManager::getInstancePtr())
	{
		log_error("NULL == ArenaClassTableManager::getInstancePtr()");
		return arena_level;
	}
	uint32_t size = ArenaClassTableManager::getInstancePtr()->getSize();
	if (arena_level == size && is_win)
		return arena_level;

	// 赢了判断向上段位 输了判断向下段位
	if (is_win)
	{
		ArenaClassTable *p_config = GET_CONF(ArenaClassTable, arena_level + 1);
		if (NULL == p_config)
		{
			log_error("arena p_config NULL tid[%d]", arena_level + 1);
			return arena_level;
		}
		if (score >= p_config->score())
		{
			return arena_level + 1;
		}
	}
	else
	{
		ArenaClassTable *p_config = GET_CONF(ArenaClassTable, arena_level - 1);
		if (NULL == p_config)
		{
			log_error("arena p_config NULL tid[%d]", arena_level - 1);
			return arena_level;
		}
		if (score < p_config->score())
		{
			return arena_level - 1;
		}
	}
	return arena_level;
}

uint32_t arena_manager_t::get_ticket_item()
{
	auto arena_ticket_item_id = GET_CONF(Comprehensive, comprehensive_common::arena_ticket_item_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(arena_ticket_item_id);
	return val;
}

int32_t arena_manager_t::get_battle_reward_score(uint64_t role_uid, uint32_t own_score, uint32_t target_score, bool is_win)
{
	if (own_score == 0)
	{
		log_error("arena own_score = 0 role[%lu]", role_uid);
		return 0;
	}
	auto score_basic_conf = GET_CONF(Comprehensive, comprehensive_common::arena_basic_score);
	auto score_turn_rank_conf = GET_CONF(Comprehensive, comprehensive_common::arena_score_turn_rank);
	auto score_turn_range_conf = GET_CONF(Comprehensive, comprehensive_common::arena_score_turn_range);
	auto score_turn_max_conf = GET_CONF(Comprehensive, comprehensive_common::arena_score_turn_max);
	auto score_turn_min_conf = GET_CONF(Comprehensive, comprehensive_common::arena_score_turn_min);
	auto conf_min = GET_CONF(ArenaClassTable, 1);
    if (NULL == conf_min)
    {
        log_error("NULL == conf_min role[%lu]", role_uid);
        return 0;
    }
	int32_t basic_score = GET_COMPREHENSIVE_VALUE_1(score_basic_conf);
	int32_t score_turn_rank = GET_COMPREHENSIVE_VALUE_1(score_turn_rank_conf);
	int32_t score_turn_range = GET_COMPREHENSIVE_VALUE_1(score_turn_range_conf);
	int32_t score_turn_max = GET_COMPREHENSIVE_VALUE_1(score_turn_max_conf);
	int32_t score_turn_min = GET_COMPREHENSIVE_VALUE_1(score_turn_min_conf);
	uint32_t min_score = conf_min->score();
	
	// 算自己和目标的分数差
	int32_t score_dec = target_score * 100 / own_score;
	int32_t scode_add_rate = 0;

	// 判断是否在下行区间
	if (score_dec < 100 - score_turn_rank)
	{
		for (int32_t i = 100 - score_turn_rank; i > score_turn_min; i -= score_turn_rank)
		{
			if (score_dec < i)
			{
				scode_add_rate -= score_turn_range;
			}
		}

	}
	// 判断是否在上行区间
	else if (score_dec > 100 + score_turn_rank)
	{
		for (int32_t i = 100 + score_turn_rank; i <= score_turn_max; i += score_turn_rank)
		{
			if (score_dec > i)
			{
				scode_add_rate += score_turn_range;
			}
		}
	}

	int32_t	final_award_score = basic_score + basic_score * scode_add_rate / 100;
	final_award_score = is_win ? final_award_score : -final_award_score;

	// 不得低于最低分数
	if (final_award_score < 0 && own_score + final_award_score < min_score)
	{
		final_award_score = -(own_score - min_score);
	}

	return final_award_score;
}

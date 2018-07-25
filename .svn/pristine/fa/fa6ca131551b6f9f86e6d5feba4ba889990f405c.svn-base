#include "role_trade_manager.hpp"
#include "log/log_wrapper.hpp"
#include "config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "city/city_manager.hpp"
#include "trade/trade_manager.hpp"
#include "item/item_manager.hpp"
#include "role/money_manager.hpp"
#include "tblh/Item.tbls.h"
#include "tblh/Trade.tbls.h"
#include "mount/mount.hpp"
#include "activity/activity_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"
#include "trade/trade.hpp"
#include "item/drop_manager.hpp"
#include "trade/role_trade.hpp"
#include "achieve/achieve_common.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

role_trade_manager_t::role_trade_manager_t()
{
}

role_trade_manager_t::~role_trade_manager_t()
{
}

uint32_t role_trade_manager_t::trade_buy_all_items(role_ptr role, const proto::client::cg_trade_buy_item_request &msg_req,
	proto::client::gc_trade_buy_item_reply &msg_reply)
{
	proto::common::role_change_data change_data;
	proto::client::gc_trade_info_notify msg;
	uint32_t reply_code = errcode_enum::error_ok;
	bool has_buy_success = true;
	do 
	{
		if (NULL == role)
		{
			log_error("trade role NULL");
			reply_code = errcode_enum::notice_role_not_exist;
			break;
		}
		trade_ptr p_trade = trade_manager_t::get_trade(msg_req.city_id());
		if (NULL == p_trade)
		{
			log_error("NULL == p_trade trade_id[%d]", msg_req.city_id());
			reply_code = errcode_enum::notice_city_not_find;
			break;
		}

		for (int i = 0; i < msg_req.item_list_size(); ++i)
		{
			if (msg_req.item_list(i).item_num() == 0)
				continue;

			reply_code = role_trade_manager_t::trade_buy_item(p_trade, role, msg_req.item_list(i).item_id(),
				msg_req.item_list(i).item_num(), &change_data);
			if (reply_code != errcode_enum::error_ok)
			{
				has_buy_success = false;
				break;
			}	
		}
	} while (false);

	/// 存盘 组包
	if (has_buy_success)
	{
		///change_data.mutable_per_info()->set_copper(money_manager_t::get_money(role, proto::common::MONEY_TYPE_COPPER));
		role->get_personal_info(change_data.mutable_per_info());
		change_data.mutable_trade_info()->set_trade_bag_weight(role->get_trade_info().get_trade_bag_weight());
		change_data.mutable_trade_info()->set_trade_point(role->get_trade_info().get_trade_point());
		change_data.mutable_trade_info()->set_total_week_gain(role->get_trade_info().get_trade_total_week_gain());
		role->save_self();
		role->get_trade_info().save_trade_item_data(true, role->get_uid());
		TRADE_LOG("role[%lu] trade buy update copper[%d] bag_weight[%d] trade_point[%d] week_gain[%d]", role->get_uid(),change_data.per_info().copper(),
			change_data.trade_info().trade_bag_weight(),change_data.trade_info().trade_point(), change_data.trade_info().total_week_gain());
	}

	msg_reply.mutable_change_data()->CopyFrom(change_data);
	role->get_trade_info().notify_trade_item_info(msg,true);
	role->send_msg_to_client(op_cmd::gc_trade_info_notify, msg);
	return reply_code;
}

uint32_t role_trade_manager_t::trade_sell_all_items(role_ptr role, const proto::client::cg_trade_sell_item_request &msg_req,
	proto::client::gc_trade_sell_item_reply &msg_reply)
{
	uint32_t reply_code = errcode_enum::error_ok;
	proto::common::role_change_data change_data;
	proto::client::gc_trade_info_notify msg;
	bool has_sell_success = true;
	uint32_t total_gain = 0;
	do 
	{
		if (NULL == role)
		{
			log_error("[wys] trade role NULL");
			reply_code = errcode_enum::notice_role_not_exist;
			break;
		}
		trade_ptr p_trade = trade_manager_t::get_trade(msg_req.city_id());
		if (NULL == p_trade)
		{
			log_error("[wys] trade [%d] NULL", msg_req.city_id());
			reply_code =  errcode_enum::notice_city_not_find;
			break;
		}

		auto conf_sell_tax = GET_CONF(Comprehensive, comprehensive_common::trade_tax_rate);
		uint32_t trade_sell_tax = GET_COMPREHENSIVE_VALUE_1(conf_sell_tax);
	
		for (int i = 0; i < msg_req.item_list_size(); ++i)
		{
			reply_code = role_trade_manager_t::trade_sell_item(p_trade, role, msg_req.item_list(i).item_id(),
				msg_req.item_list(i).item_num(), trade_sell_tax,
				&change_data, total_gain);

			if (reply_code != errcode_enum::error_ok)
			{
				has_sell_success = false;
				break;
			}
		}
	} while (false);

	/// 存盘 组包
	if (has_sell_success)
	{
		role->get_trade_info().add_trade_week_total_gain(total_gain);					///每周累计利润
		role->get_trade_info().save_trade_item_data(true, role->get_uid());
		role->save_self();

		///同步更新玩家跑商信息
		///change_data.mutable_per_info()->set_copper(money_manager_t::get_money(role, proto::common::MONEY_TYPE_COPPER));
		role->get_personal_info(change_data.mutable_per_info());
		change_data.mutable_trade_info()->set_trade_bag_weight(role->get_trade_info().get_trade_bag_weight());
		change_data.mutable_trade_info()->set_trade_point(role->get_trade_info().get_trade_point());
		change_data.mutable_trade_info()->set_total_week_gain(role->get_trade_info().get_trade_total_week_gain());
		msg_reply.set_gain(total_gain);
		TRADE_LOG("role[%lu] trade sell update total_gain[%d] copper[%d] bag_weight[%d] trade_point[%d] week_gain[%d]", role->get_uid(), total_gain, 
			change_data.per_info().copper(),change_data.trade_info().trade_bag_weight(), change_data.trade_info().trade_point(),
			change_data.trade_info().total_week_gain());
	}

	msg_reply.mutable_change_data()->CopyFrom(change_data);
	role->get_trade_info().notify_trade_item_info(msg, true);
	role->send_msg_to_client(op_cmd::gc_trade_info_notify, msg);
	achieve_common_t::notify_progress_state(role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_TRADE_NUMBER);
	achieve_common_t::notify_progress_state(role->get_uid(), proto::common::ACHIEVE_TRADE_SINGLE_GAIN, 0,total_gain);
	return reply_code;
}

uint32_t role_trade_manager_t::trade_buy_item(trade_ptr p_trade, role_ptr role, uint32_t item_tid,
	uint32_t item_num, proto::common::role_change_data *p_data)
{
	uint32_t errcode = errcode_enum::error_ok;	///错误码
	trade_item_ptr p_trade_item = trade_item_ptr();
	city_ptr p_city = city_ptr();
	uint32_t total_price = 0;					///总价
	do 
	{
		if (NULL == role)
		{
			log_error("NULL == role");
			errcode = errcode_enum::notice_role_not_exist;
			break;
		}

		if (NULL == p_trade)
		{
			log_error("NULL == p_trade role[%lu]",role->get_uid());
			errcode = errcode_enum::notice_city_not_find;
			break;
		}
	
		if (NULL == p_data)
		{
			log_error("trade proto::common::role_change_data NULL");
			errcode = errcode_enum::notice_unknown;
			break;
		}
		if (0 == item_tid)
		{
			log_error("[wys] trade item_tid = 0");
			errcode = errcode_enum::notice_trade_item_not_find;
			break;
		}
		p_trade_item = p_trade->get_trade_item(item_tid);
		if (NULL == p_trade_item)
		{
			log_error("trade p_trade_item[%d] not find", item_tid);
			errcode = errcode_enum::notice_trade_item_not_find;
			break;
		}
		p_city = city_manager_t::get_city(role->get_map_tid());
		if (NULL == p_city)
		{
			log_error("NULL == p_city role[%lu] map_tid[%d]",role->get_uid() ,role->get_map_tid());
			errcode = errcode_enum::notice_city_not_find;
			break;
		}
		///是否可购买
		if (!p_trade_item->is_can_buy())
		{
			log_error("role[%lu] buy item[%d] is not buy",role->get_uid(),item_tid);
			errcode = errcode_enum::notice_trade_item_can_not_buy;
			break;
		}
		///检查可购买数量
		role_trade_item_info_ptr p_role_trade_item = role->get_trade_info().get_role_trade_item_info(item_tid);
		if (NULL == p_role_trade_item)
		{
			if (item_num > p_trade_item->get_can_buy_num())
			{
				TRADE_LOG("role[%lu] buy item[%d] item_num[%d] more than cur_num[%d]", role->get_uid(), item_tid,item_num, p_trade_item->get_can_buy_num());
				errcode = errcode_enum::notice_trade_item_buy_num_max;
				break;
			}
		}
		else
		{
			if (!p_role_trade_item->can_buy(item_num))
			{
				errcode = errcode_enum::notice_trade_item_buy_num_max;
				break;
			}
		}
		///负重判断 
		const uint32_t trade_bag_max_weight = role->get_trade_info().get_trade_max_weight();
		if (trade_bag_max_weight - role->get_trade_info().get_trade_bag_weight() < item_num)
		{
			errcode = errcode_enum::notice_trade_item_buy_bag_max;
			break;
		}
		///背包格子检测
		if (!item_manager_t::check_add_item(role, item_tid, item_num, proto::common::package_type_trade))
		{
			errcode = errcode_enum::notice_trade_item_buy_bag_max;
			break;
		}
		/*///计算税收
		auto conf_buy_tax = GET_CONF(Comprehensive, comprehensive_common::trade_tax_rate);
		const uint32_t trade_buy_tax = GET_COMPREHENSIVE_VALUE_1(conf_buy_tax);
		uint32_t tax_price = 0;*/

		///不在自己国家交易收税
		/*if (0 == role->get_country_id() ||
			p_city->get_capture_country() != role->get_country_id())
		{
			tax_price = p_trade_item->get_buy_price() * item_num * trade_buy_tax / 10000;
		}*/
		///总价
		total_price = p_trade_item->get_buy_price() * item_num /*+ tax_price*/;
		const uint32_t cur_copper = money_manager_t::get_money(role, proto::common::MONEY_TYPE_COPPER);
		if (cur_copper < total_price)
		{
			log_error("role[%lu] buy item[%d] item_num[%d] need_price[%d] cur_money[%d]",role->get_uid(),item_tid,item_num, total_price, cur_copper);
			errcode = errcode_enum::notice_copper_money_not_enough;
			break;
		}
		///添加道具
		item_manager_t::add_item(role, item_tid, item_num, log_enum::source_type_trade_buy_item, 0, p_data, proto::common::package_type_trade);
		///扣钱
		money_manager_t::use_money(role, proto::common::MONEY_TYPE_COPPER, total_price, log_enum::source_type_trade_buy_item, item_tid);
		///添加购买记录
		proto::common::role_trade_item_single *p_change_item_single = p_data->mutable_trade_item()->add_trade_item_list();
		role->get_trade_info().buy_trade_item(item_tid, total_price, item_num,
			p_trade_item->get_can_buy_num(), p_change_item_single);

		if (role->get_trade_info().get_trade_bag_weight() > 0) { // 服务器切换跑商状态
			role->toggle_trade_state(proto::common::role_mode_state_trade);
		}

		///添加行为日志
		role_trade_item_info_ptr p_role_item = role->get_trade_info().get_role_trade_item_info(item_tid);
		if (p_role_item)
		{
			log_wrapper_t::send_trade_log(role->get_uid(), item_tid, p_trade_item->get_buy_price(), item_num, 0,p_role_item->get_item_buy_num(),
				p_role_item->get_item_buy_price(), 0, p_trade->get_id(), log_enum::opt_type_add, log_enum::source_type_trade_buy_item);
		}
		
	} while (false);

	return errcode;
}


uint32_t role_trade_manager_t::trade_sell_item(trade_ptr p_trade, role_ptr role, uint32_t item_tid,
	uint32_t item_num, uint32_t trade_sell_tax,
	proto::common::role_change_data *p_data, uint32_t& total_gain)
{
	uint32_t errcode = errcode_enum::error_ok;	///错误码
	role_trade_item_info_ptr p_role_trade_item = role_trade_item_info_ptr();
	trade_item_ptr p_trade_item = trade_item_ptr();
	city_ptr p_city = city_ptr();
	do 
	{
		if (NULL == role)
		{
			log_error("trade role NULL");
			errcode = errcode_enum::notice_role_not_exist;
			break;
		}
		if (NULL == p_trade)
		{
			log_error("trade city NULL");
			errcode = errcode_enum::notice_city_not_find;
			break;
		}
		if (NULL == p_data)
		{
			log_error("trade proto::common::role_change_data NULL");
			errcode = errcode_enum::notice_unknown;
			break;
		}
		if (0 == item_tid)
		{
			log_error("trade item_tid = 0");
			errcode = errcode_enum::notice_trade_item_not_find;
			break;
		}

		/// 检查已购买数量和出售数量是否相符
		p_role_trade_item = role->get_trade_info().get_role_trade_item_info(item_tid);
		if (NULL == p_role_trade_item)
		{
			log_error("trade role_trade_item[%d] not find", item_tid);
			errcode = errcode_enum::notice_trade_item_not_find;
			break;
		}

		p_city = city_manager_t::get_city(role->get_map_tid());
		if (NULL == p_city)
		{
			log_error("NULL == p_city map_tid[%d]", role->get_map_tid());
			errcode = errcode_enum::notice_city_not_find;
			break;
		}

		if (item_num > p_role_trade_item->get_item_buy_num())
		{
			log_error("trade sell item item_sell_num[%d] error not match item_num[%d]", item_num, p_role_trade_item->get_item_buy_num());
			errcode = errcode_enum::notice_trade_item_not_find;
			break;
		}

		p_trade_item = p_trade->get_trade_item(item_tid);
		if (NULL == p_trade_item)
		{
			log_error("trade p_city_trade_item[%d] not find", item_tid);
			errcode = errcode_enum::notice_trade_item_type_not_find;
			break;
		}

		/// 扣除背包中的贸易物品
		errcode = item_manager_t::remove_item(role, item_tid, item_num, log_enum::source_type_trade_sell_item, 0,
			p_data, proto::common::package_type_trade);

	} while (false);
	
	if (errcode != errcode_enum::error_ok)
	{
		return errcode;
	}
	
	///流程开始
	/////////////////////////////////////////////////////////////////////////////////////////////////
	troop_ptr p_troop = game_troop_mgr_t::get_troop(role->get_troop_id());
	uint32_t team_num = 1;													///组队人数 默认1
	if (p_troop)
	{
		const troop_t::troop_member_vec& member_list = p_troop->get_members();
		team_num = member_list.size() > 1 ? member_list.size() : 1;
	}
	
	auto conf_1 = GET_CONF(Comprehensive, comprehensive_common::trade_single_point_add_rate);
	auto conf_2 = GET_CONF(Comprehensive, comprehensive_common::trade_team_point_add_rate);
	auto conf_3 = GET_CONF(Comprehensive, comprehensive_common::trade_team_add_rate);
	auto conf_4 = GET_CONF(Comprehensive, comprehensive_common::trade_prosperity_add_rate);
	const uint32_t rate_point1 = GET_COMPREHENSIVE_VALUE_1(conf_1);			///单人贸易点系数
	const uint32_t rate_point2 = GET_COMPREHENSIVE_VALUE_1(conf_2);			///组队贸易点系数
	const uint32_t team_add_rate = GET_COMPREHENSIVE_VALUE_1(conf_3);		///组队跑商加成系数
	const uint32_t prosperity_rate = GET_COMPREHENSIVE_VALUE_1(conf_4);		///繁荣度系数

	uint32_t total_price = 0, tax_price = 0;												///加的钱  税
	int32_t gain = 0;																		///利润
	float base_gain = 0.0f;																	///利润基础值
	const uint32_t cur_trade_point = role->get_trade_info().get_trade_point();				///当前贸易点数
	const uint32_t item_buy_price = p_role_trade_item->get_item_buy_price();				///该物品当前购买总价
	const uint32_t item_buy_num = p_role_trade_item->get_item_buy_num();					///该物品当前购买总数
	const uint32_t single_buy_price = uint32_t((float)item_buy_price / (float)item_buy_num);///该物品当前单价
	const uint32_t single_sell_price = p_trade_item->get_sell_price();						///该物品在当前跑商的当前出售单价
	const uint32_t sell_part_cost = single_buy_price * item_num;							///出售部分的成本

	///判断贸易事件有没有结束
	if (!p_trade->check_trade_event_is_end())
	{
		switch (p_trade->get_event_type())
		{
		case proto::common::trade_event_type_popular:
		{
			/*auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_event2_market_add);
			uint32_t event_popular_market_add = GET_COMPREHENSIVE_VALUE_1(conf);
			market = (uint32_t)((float)(p_trade_item->get_market() * event_popular_market_add) / 10000);*/
		}break;
		case proto::common::trade_event_type_black:
		{
			uint32_t add_item_num = 0;													///增加黑市宝箱数量
			auto conf_prob = GET_CONF(Comprehensive, comprehensive_common::trade_event1_chest_prob);
			auto conf_drop_id = GET_CONF(Comprehensive, comprehensive_common::trade_event1_chest_id);
			const uint32_t config_prob = GET_COMPREHENSIVE_VALUE_1(conf_prob);			///获得黑市宝箱概率
			const uint32_t drop_id = GET_COMPREHENSIVE_VALUE_1(conf_drop_id);			///黑市宝箱掉落ID
			for (uint32_t i = 0; i < item_num; ++i)
			{
				///uint32_t add_box_prob = rate_util_t::ranged_random(1, 10000);
				uint32_t add_box_prob = random_util_t::randBetween(1, 10000);
				if (add_box_prob > config_prob)
					continue;
				++add_item_num;
			}
			TRADE_LOG("role[%lu] trade black event sell add black box_num[%d] drop_id[%d]",role->get_uid(),add_item_num,drop_id);
			for (uint32_t i = 0; i < add_item_num; ++i)
			{
				///添加黑市宝箱奖励
				drop_manager_t::drop(role, drop_id, log_enum::source_type_trade_sell_item, proto::common::drop_sys_type_trade, item_tid, p_data);
			}
		}break;
		default:
			break;
		}
	}
	///不消耗 / 消耗贸易点的出售价 原价 获得的钱 税
	bool is_original_price_sell = false;							///是否原价出售
	if (p_trade_item->is_can_buy())
	{										
		///此处没有计算利润 原价卖出
		total_price = sell_part_cost;						///如果城市商店出售列表里有这个物品 原价出售	
		is_original_price_sell = true;
	}
	else
	{
		///是否有税收
		float tax_base_part = 0;
		if (role->get_country_id() == 0 ||
			p_city->get_capture_country() != role->get_country_id())
		{
			tax_base_part = (float)trade_sell_tax / 10000;
		}

		uint32_t single_base_gain = single_sell_price - single_buy_price;				///单一利润
		float fix_part = (float)single_base_gain * (1 + float(p_city->get_prosperity_level() * prosperity_rate) / 10000 +
			float((team_num - 1) * team_add_rate) / 10000);								///公式固定的统一部分 不包括数量 
		float team_add_part = float(team_num > 1 ? rate_point2 : rate_point1) / 10000;	///贸易点加成基础部分
		TRADE_LOG("role[%lu] single_base_gain[%d] city_pro_level[%d] pro_rate[%d] team_num[%d] team_rate[%d] team_add_part[%f] tax_base_part[%f] fix_part[%f]",
			role->get_uid(), single_base_gain, p_city->get_prosperity_level(), prosperity_rate,team_num,team_add_rate,team_add_part, tax_base_part, fix_part);
		if (item_num <= cur_trade_point)
		{
			///gain = fix_part * team_add_part * (1 - tax_base_part) * item_num;						///利润
			base_gain = fix_part * (1 + team_add_part ) *  item_num;									///利润
			///tax_price = fix_part * team_add_part * tax_base_part * item_num;							///税
		} 
		else
		{
			///gain = fix_part * (1 - tax_base_part) * (team_add_part * cur_trade_point + (item_num - cur_trade_point));		///利润
			base_gain = fix_part *  (( 1 + team_add_part) * cur_trade_point + (item_num - cur_trade_point));					//基础利润
			///tax_price = fix_part * tax_base_part * (team_add_part * cur_trade_point + (item_num - cur_trade_point));			///税	
		}
		gain = uint32_t(base_gain *(1 - tax_base_part));														///利润
		tax_price = uint32_t(base_gain * (1 - tax_base_part) * tax_base_part);															///税
		total_price = gain + sell_part_cost;														///加的钱
		TRADE_LOG("role[%lu] sell_item[%d] item_num[%d] cur_point[%d] base_gain[%f] gain[%d] tax_price[%d] total_price[%d]",
			role->get_uid(), item_tid, item_num, cur_trade_point,base_gain, gain, tax_price, total_price);

		///计算消耗贸易点
		if (gain > 0)
		{
			///贸易点消耗改为出售的物品数
			const uint32_t cost_trade_point = (cur_trade_point > item_num ? item_num : cur_trade_point);									
			role->get_trade_info().set_trade_point(cur_trade_point - cost_trade_point, log_enum::source_type_trade_sell_item);
			///role->get_trade_info().add_trade_week_total_gain(gain);					每周累计利润 外层出售循环统一统计
			total_gain += gain;
		}
	}
	TRADE_LOG("role[%lu] single_sell_price[%d] single_buy_price[%d] item[%d] item_num[%d] is_original_price[%d]",role->get_uid(), single_sell_price, single_buy_price, item_tid, item_num, is_original_price_sell);
	///修改角色的贸易数据
	proto::common::role_trade_item_single *p_change_item_single = p_data->mutable_trade_item()->add_trade_item_list();
	role->get_trade_info().sell_trade_item(item_tid, item_num, p_change_item_single);

	if (role->get_trade_info().get_trade_bag_weight() == 0) { // 服务器切换跑商状态
		role->toggle_trade_state(proto::common::role_mode_state_normal);
	}

	///修改城市的贸易数据
	p_trade->trade_sell_item(p_trade_item, item_num);

	///加钱
	money_manager_t::add_money(role, proto::common::MONEY_TYPE_COPPER, total_price, log_enum::source_type_trade_sell_item, item_tid);

	///添加行为日志
	log_wrapper_t::send_trade_log(role->get_uid(), item_tid, single_sell_price, item_num, tax_price, p_role_trade_item->get_item_buy_num(),
		p_role_trade_item->get_item_buy_price(), gain, p_trade->get_id(), log_enum::opt_type_del, log_enum::source_type_trade_sell_item);

	///成就通知
	achieve_common_t::notify_progress_state(role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_TRADE_TOTAL_GAIN_NUMBER, 1, gain);
	return errcode;
}

uint32_t role_trade_manager_t::get_city_trade_shop_info(uint32_t trade_id, role_ptr role, proto::client::gc_get_city_trade_shop_reply &reply)
{
	if (NULL == role)
	{
		log_error("[wys] trade role NULL");
		return errcode_enum::notice_role_not_exist;
	}

	uint32_t reply_code = errcode_enum::error_ok;
	const trade_manager_t::trade_map& temp_trade_map = trade_manager_t::get_all_trade();
	proto::common::city_trade_shop_info *p_city_data = NULL;
	bool b_is_need_reset_buy_num = false;						///是否重置购买数量
	if (trade_id == 0)											///获取所有城市
	{
		for (auto& it : temp_trade_map)
		{
			p_city_data = reply.add_city_shop_info_list();
			if (NULL == p_city_data)
			{
				log_error("[wys] trade reply msg city_shop_info_list add fail");
				return errcode_enum::notice_unknown;
			}

			reply_code = get_one_city_trade_shop_info(it.first, role, p_city_data, b_is_need_reset_buy_num, true);
			if (reply_code != errcode_enum::error_ok)
				return reply_code;		
		}
	}
	else /// 获取单一城市
	{
		p_city_data = reply.add_city_shop_info_list();
		if (NULL == p_city_data)
		{
			log_error("[wys] trade reply msg city_shop_info_list add fail");
			return errcode_enum::notice_unknown;
		}
		reply_code = get_one_city_trade_shop_info(trade_id, role, p_city_data, b_is_need_reset_buy_num);
		if (reply_code != errcode_enum::error_ok)
			return reply_code;
	}
	auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_goods_add_time2);
	uint32_t buy_num_refresh_time = GET_COMPREHENSIVE_VALUE_1(conf);
	uint32_t cur_time = common::time_util_t::now_time();
	uint32_t send_time = std::abs(trade_manager_t::get_buy_num_reset_time() + buy_num_refresh_time * 60 - cur_time);
	reply.set_item_buy_num_reset_time(send_time);

	/// 重置购买数量
	if (b_is_need_reset_buy_num)
	{
		role->get_trade_info().save_trade_item_data(true, role->get_uid());
	}
	
	return reply_code;
}

uint32_t role_trade_manager_t::trade_supplement_item(role_ptr role, uint32_t trade_id, uint64_t use_item_uid,
	proto::common::role_change_data *change_data,
	proto::common::city_trade_shop_info *p_data)
{
	uint32_t reply_code = errcode_enum::error_ok;
	do 
	{
		if (NULL == role)
		{
			log_error("trade role NULL");
			reply_code = errcode_enum::notice_role_not_exist;
			break;
		}
		if (0 == trade_id)
		{
			log_error("trade trade_id = 0");
			reply_code = errcode_enum::notice_city_not_find;
			break;
		}
		if (NULL == change_data)
		{
			log_error("trade proto::common::role_change_data NULL");
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		if (NULL == p_data)
		{
			log_error("trade proto::common::city_trade_shop_info NULL");
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		trade_ptr p_trade = trade_manager_t::get_trade(trade_id);
		if (NULL == p_trade)
		{
			log_error("NULL == p_trade", trade_id);
			reply_code = errcode_enum::notice_city_not_find;
			break;
		}
		/// 查找背包中的道具
		item_ptr p_role_trade_use_item = item_manager_t::get_item_by_uid(role, use_item_uid);
		if (NULL == p_role_trade_use_item)
		{
			log_error("[wys] trade role[%lu] not have item uid[%lu]", role->get_uid(), use_item_uid);
			reply_code = errcode_enum::notice_item_not_enough;
			break;
		}
		/// 查找配置表 验证物品类型
		Item *p_use_item_config = GET_CONF(Item, p_role_trade_use_item->get_tid());
		if (NULL == p_use_item_config || p_use_item_config->use_type() != proto::common::item_use_type_trade_buy)
		{
			log_error("[wys] trade not find item config tid[%d]", p_role_trade_use_item->get_tid());
			reply_code = errcode_enum::notice_item_null;
			break;
		}

		/// 扣除
		reply_code = item_manager_t::remove_item(role, p_role_trade_use_item->get_tid(), 1,
			log_enum::source_type_trade_supplement_item, 0, change_data);
		if (reply_code != errcode_enum::error_ok)
			break;

		proto::common::city_trade_shop_item_info *p_trade_shop_item_info = NULL;
		role_trade_item_info_ptr p_role_trade_item = role_trade_item_info_ptr();
		trade_item_ptr p_trade_item = trade_item_ptr();
		troop_ptr p_troop = troop_ptr();
		/// 遍历城市商店中所有物品 补充符合条件的物品购买数量
		const trade_item_map&  item_list = p_trade->get_trade_item_list();
		if (0 != role->get_troop_id())
		{
			p_troop = game_troop_mgr_t::get_troop(role->get_troop_id());
			if (NULL == p_troop)
			{
				log_error("NULL == p_troop [%d]",role->get_troop_id());
				reply_code = errcode_enum::user_troop_err_code_not_exsit;
				break;
			}
		}
		Item *p_item_config = NULL;
		std::vector<trade_item_ptr> add_item_list;					///补货物品列表
		for (auto&it : item_list)	
		{
			p_trade_item = it.second;
			if (NULL == p_trade_item)
			{
				log_error("NULL == p_trade_item");
				reply_code = errcode_enum::notice_unknown;
				break;
			}
			if (!p_trade_item->is_can_buy())
			{
				continue;
			}
			/// 判断品质是否相符
			p_item_config = GET_CONF(Item, p_trade_item->get_id());
			if (NULL == p_item_config)
			{
				log_error("trade item config not find tid[%d]", p_trade_item->get_id());
				continue;
			}
			if (p_item_config->quality() != p_use_item_config->quality())
			{
				continue;
			}
			add_item_list.push_back(p_trade_item);	
		}

		/// 补货
		if (p_troop)
		{
			const troop_t::troop_member_vec& member_list = p_troop->get_members();
			for (auto& member : member_list)
			{
				role_ptr p_role = role_manager_t::find_role(member->get_uid());
				if (NULL == p_role)
				{
					log_error("NULL == p_role troop role[%d]", member->get_uid());
					continue;
				}
				bool is_own = false;
				if (role->get_uid() == member->get_uid())
					is_own = true;
				proto::client::gc_trade_supplement_notify notify;
				notify.set_role_uid(role->get_uid());
				notify.set_use_item_id(p_role_trade_use_item->get_tid());
				for (auto& item : add_item_list)
				{
					if (NULL == item)
					{
						log_error("NULL == item");
						continue;
					}
					p_role->get_trade_info().supplement_trade_item(item->get_id(), item->get_can_buy_num(), item->get_can_buy_num());

					/// 获取最新数据
					p_role_trade_item = p_role->get_trade_info().get_role_trade_item_info(item->get_id());
					if (NULL == p_role_trade_item)
					{
						log_error("[wys] trade p_role_trade_item NULL");
						reply_code = errcode_enum::notice_unknown;
						break;
					}
					proto::common::trade_supplement_info* p_supplement_item = notify.add_item_list();
					p_supplement_item->set_item_tid(item->get_id());
					p_supplement_item->set_item_num(item->get_can_buy_num());
					if (is_own)
					{
						p_trade_shop_item_info = p_data->add_item_list();
						if (NULL == p_trade_shop_item_info)
						{
							log_error("[wys] trade add_item_list fail");
							reply_code = errcode_enum::notice_unknown;
							break;
						}
						/// 组包
						p_trade_shop_item_info->set_item_tid(item->get_id());
						p_trade_shop_item_info->set_can_buy_max_num(p_role_trade_item->get_item_can_buy_num());
						TRADE_LOG("trade_id[%d] item[%d] can_buy_max_num[%d]", trade_id, p_trade_shop_item_info->item_tid(), p_trade_shop_item_info->can_buy_max_num());
					}
					else
					{
						proto::common::city_trade_shop_info* p_shop_info = notify.mutable_city_shop_info();
						p_trade_shop_item_info = p_shop_info->add_item_list();
						if (NULL == p_trade_shop_item_info)
						{
							log_error("[wys] trade add_item_list fail");
							reply_code = errcode_enum::notice_unknown;
							break;
						}
						/// 组包
						p_trade_shop_item_info->set_item_tid(item->get_id());
						p_trade_shop_item_info->set_can_buy_max_num(p_role_trade_item->get_item_can_buy_num());
						TRADE_LOG("trade_id[%d] item[%d] can_buy_max_num[%d]", trade_id, p_trade_shop_item_info->item_tid(), p_trade_shop_item_info->can_buy_max_num());
					}	
				}
				p_role->get_trade_info().save_trade_item_data(true, role->get_uid());
				p_role->send_msg_to_client(op_cmd::gc_trade_supplement_notify, notify);
			}
		}
		else
		{
			for (auto& item : add_item_list)
			{
				if (NULL == item)
				{
					log_error("NULL == item");
					continue;
				}
				role->get_trade_info().supplement_trade_item(item->get_id(), item->get_can_buy_num(), item->get_can_buy_num());
				/// 获取最新数据
				p_role_trade_item = role->get_trade_info().get_role_trade_item_info(item->get_id());
				if (NULL == p_role_trade_item)
				{
					log_error("[wys] trade p_role_trade_item NULL");
					reply_code = errcode_enum::notice_unknown;
					break;
				}
				p_trade_shop_item_info = p_data->add_item_list();
				if (NULL == p_trade_shop_item_info)
				{
					log_error("[wys] trade add_item_list fail");
					reply_code = errcode_enum::notice_unknown;
					break;
				}
				/// 组包
				p_trade_shop_item_info->set_item_tid(item->get_id());
				p_trade_shop_item_info->set_can_buy_max_num(p_role_trade_item->get_item_can_buy_num());
			}
			
			TRADE_LOG("trade_id[%d] item[%d] can_buy_max_num[%d]", trade_id, p_trade_shop_item_info->item_tid(), p_trade_shop_item_info->can_buy_max_num());
			role->get_trade_info().save_trade_item_data(true, role->get_uid());
		}
		
	} while (false);

	return reply_code;
}

uint32_t role_trade_manager_t::get_all_city_trade_event(proto::client::gc_trade_event_reply &msg_reply,uint32_t type /* = 0 */)
{
	const trade_manager_t::trade_map&  trade_list = trade_manager_t::get_all_trade();
	trade_ptr p_trade = trade_ptr();
	
	if (0 == type)
	{
		std::vector<trade_ptr> trade_popular;
		std::vector<trade_ptr> trade_black;

		for (auto& it : trade_list)
		{
			p_trade = it.second;
			if (NULL == p_trade)
			{
				log_error("NULL == p_trade");
				continue;
			}
			if (p_trade->get_event_type() == proto::common::trade_event_type_popular)
			{
				trade_popular.push_back(p_trade);
			}
			else if(p_trade->get_event_type() == proto::common::trade_event_type_black)
			{
				trade_black.push_back(p_trade);
			}
		}
		uint32_t popular_num = trade_popular.size();
		uint32_t black_num = trade_black.size();
		TRADE_LOG("popular_num[%d] black_num[%d]", popular_num, black_num);
		if (0 != popular_num)
		{
			///uint32_t rand_popular = rate_util_t::ranged_random(0, popular_num - 1);
			int32_t rand_popular = random_util_t::randBetween(0, popular_num - 1);
			if (-1 == rand_popular)
			{
				log_error("popular event  min[%d]   max[%d]",0,popular_num - 1);
				rand_popular = 0;
			}
	
			p_trade = trade_popular[rand_popular];
			if (p_trade)
			{
				proto::common::city_trade_event_info* p_city_info = msg_reply.add_city_event_list();
				p_city_info->set_city_tid(p_trade->get_id());
				p_city_info->set_event_type((proto::common::trade_event_type)p_trade->get_event_type());
				p_city_info->set_trade_event_param(p_trade->get_event_param());
				const uint32_t item_tid = p_trade->get_event_param();
				for (auto& it : trade_list)
				{
					trade_ptr p_temp_trade = it.second;
					if (NULL == p_temp_trade)
					{
						log_error("NULL == p_trade");
						continue;
					}
					const trade_item_map&  item_list = p_temp_trade->get_trade_item_list();
					auto iter =  item_list.find(item_tid);
					if (iter == item_list.end())
						continue;
					trade_item_ptr p_item = iter->second;
					if (NULL == p_item)
						continue;
					if (!p_item->is_can_buy())
						continue;
					p_city_info->set_trade_event_param2(it.first);
					break;
				}
			}
				}

		if ( 0 != black_num)
		{
			uint32_t item_tid = 0;
			///uint32_t rand_black = rate_util_t::ranged_random(0, black_num - 1);
			int32_t rand_black = random_util_t::randBetween(0, black_num - 1);
			if (-1 == rand_black)
			{
				log_error("black event  min[%d]   max[%d]", 0, black_num - 1);
				rand_black = 0;
			}
			
			p_trade = trade_black[rand_black];
			if (p_trade)
			{
				const trade_item_map& item_list = p_trade->get_trade_item_list();
				for (auto& it : item_list)
				{
					if (!it.second)
						continue;
					if(!it.second->is_can_buy())
						continue;
					item_tid = it.first;
					break;
				}
				proto::common::city_trade_event_info* p_city_info = msg_reply.add_city_event_list();
				p_city_info->set_city_tid(p_trade->get_id());
				p_city_info->set_event_type((proto::common::trade_event_type)p_trade->get_event_type());
				p_city_info->set_trade_event_param(item_tid);
			}
		}

		uint32_t trade_id = 0,item_tid = 0,sell_trade_id = 0;
		trade_manager_t::get_suggest_trade_info(trade_id, item_tid, sell_trade_id);
		if (0 != trade_id && 0 != item_tid && 0 != sell_trade_id)
		{
			proto::common::city_trade_event_info* p_city_info = msg_reply.add_city_event_list();
			p_city_info->set_city_tid(trade_id);
			p_city_info->set_event_type(proto::common::trade_event_type::trade_event_type_normal);
			p_city_info->set_trade_event_param(item_tid);
			p_city_info->set_trade_event_param2(sell_trade_id);
			TRADE_LOG("suggest trade_id[%d] item_tid[%d]", trade_id, item_tid);
		}
	}
	else
	{
		for (auto& it : trade_list)
		{
			p_trade = it.second;
			if (NULL == p_trade)
			{
				log_error("NULL == p_trade");
				continue;
			}
			if (p_trade->get_event_type() == proto::common::trade_event_type_normal)
			{
				continue;
			}
			proto::common::city_trade_event_info* p_city_info = msg_reply.add_city_event_list();
			p_city_info->set_city_tid(p_trade->get_id());
			p_city_info->set_event_type((proto::common::trade_event_type)p_trade->get_event_type());
			p_city_info->set_trade_event_param(p_trade->get_event_param());
			TRADE_LOG("trade[%d] event_type[%d] event_param[%d]", p_trade->get_id(), p_trade->get_event_type(), p_trade->get_event_param());
		}
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_event_time2);
	uint32_t event_refresh_time = GET_COMPREHENSIVE_VALUE_1(conf);
	msg_reply.set_event_reset_time(std::abs(trade_manager_t::get_event_reset_time() + event_refresh_time * 60 - common::time_util_t::now_time()));
	return errcode_enum::error_ok;
}

uint32_t role_trade_manager_t::get_one_city_trade_shop_info(uint32_t trade_id, role_ptr role, proto::common::city_trade_shop_info *p_data,
	bool &b_is_need_reset_buy_num, bool b_is_need_send_all_item)
{
	uint32_t reply_code = errcode_enum::error_ok;
	do 
	{
		if (NULL == role)
		{
			log_error("[wys] trade role NULL");
			reply_code = errcode_enum::notice_role_not_exist;
			break;
		}
		if (NULL == p_data)
		{
			log_error("[wys] trade proto::common::city_trade_shop_info NULL");
			reply_code = errcode_enum::notice_unknown;
			break;
		}
		if (0 == trade_id)
		{
			log_error("[wys] trade city_id = 0");
			reply_code = errcode_enum::notice_city_not_find;
			break;
		}
		trade_ptr p_trade = trade_manager_t::get_trade(trade_id);
		if (NULL == p_trade)
		{
			log_error("[wys] trade city[%d] NULL", trade_id);
			reply_code = errcode_enum::notice_city_not_find;
			break;
		}

		Trade* p_config = GET_CONF(Trade, trade_id);
		if (NULL == p_config)
		{
			log_error("NULL == p_config city_id[%d]", trade_id);
			reply_code = errcode_enum::notice_city_not_find;
			break;
		}
		city_ptr p_city = city_manager_t::get_city(p_config->scene());
		if (NULL == p_city)
		{
			log_error("[wys] city[%d] NULL", p_config->scene());
			reply_code = errcode_enum::notice_city_not_find;
			break;
		}
		bool is_need_reset_buy_num = false;
		/// 申请数据时验证玩家是否到了重置购买数量的刷新时间
		if (role->get_trade_info().get_last_reset_buy_num_time() < trade_manager_t::get_buy_num_reset_time())
		{
			is_need_reset_buy_num = true;
			role->get_trade_info().reset_buy_num();
			//TRADE_LOG("trade reset trade_id[%d] last_reset_time[%d] buy_num_reset_time[%lu]", trade_id, role->get_trade_info().get_last_reset_buy_num_time(), trade_manager_t::get_buy_num_reset_time());
		}

		b_is_need_reset_buy_num = is_need_reset_buy_num;

		/// 组包
		p_data->mutable_city_info()->set_city_id(p_config->scene());
		p_data->mutable_city_info()->set_prosperity_value(p_city->get_prosperity_value());
		p_data->mutable_city_info()->set_city_hold_value(p_city->get_hold_value());
		p_data->mutable_city_info()->set_prosperity_level(p_city->get_prosperity_level());
		p_data->set_trade_id(trade_id);
		p_data->set_event_item_id(p_trade->get_event_param());
		proto::common::city_trade_shop_item_info *p_trade_shop_item_info = NULL;
		role_trade_item_info_ptr p_role_trade_item = role_trade_item_info_ptr();
		trade_item_ptr p_trade_item = trade_item_ptr();
		uint32_t trade_item_can_buy_max_num = 0;
		uint32_t trade_item_buy_price = 0;

		const trade_item_map &temp_item_list = p_trade->get_trade_item_list();

		/// 遍历城市商店物品 组包
		for (auto& it : temp_item_list)
		{
			trade_item_can_buy_max_num = 0;
			trade_item_buy_price = 0;
			p_trade_item = it.second;
			if (NULL == p_trade_item)
			{
				log_error("[wys] trade p_trade_item NULL");
				reply_code = errcode_enum::notice_unknown;
				break;
			}

			if (!p_trade_item->is_can_buy() && !item_manager_t::has_item(role, it.first, 1, proto::common::package_type_trade))
			{
				continue;
			}

			if (p_trade_item->is_can_buy())
			{
				trade_item_buy_price = p_trade_item->get_buy_price();
			}
			/*else
			{
				///做个优化 只发送在背包里的出售给商店的物品数据
				if (!b_is_need_send_all_item && !item_manager_t::has_item(role, it.first, 1, proto::common::package_type_trade))
					continue;
			}*/

			///玩家身上存在该物品就从玩家身上获取可购买数量 不存在以该城市商店物品可购买最大数量为准
			p_role_trade_item = role->get_trade_info().get_role_trade_item_info(p_trade_item->get_id());
			if (p_role_trade_item)
			{
				/*if (is_need_reset_buy_num)
				{
					p_role_trade_item->set_item_max_buy_num(p_trade_item->get_can_buy_num());
				}*/
				trade_item_can_buy_max_num = p_role_trade_item->get_item_can_buy_num();
			}
			else
			{
				trade_item_can_buy_max_num = p_trade_item->get_can_buy_num();
			}

			p_trade_shop_item_info = p_data->add_item_list();
			if (NULL == p_trade_shop_item_info)
			{
				log_error("[wys] trade add_item_list fail");
				reply_code = errcode_enum::notice_unknown;
				break;
			}
			uint32_t new_market = p_trade_item->get_market();
			if (p_trade->get_event_param() == p_trade_item->get_id())
			{
				uint32_t event_market_add = 10000;
				auto conf = GET_CONF(Comprehensive, comprehensive_common::trade_event2_market_add);
				event_market_add = GET_COMPREHENSIVE_VALUE_1(conf);
				new_market = (uint32_t)((float)p_trade_item->get_market() * ((float)event_market_add / 10000));
				TRADE_LOG("role[%lu] trade bag item[%d] new_market[%d]", role->get_uid(), p_trade_item->get_id(), new_market);
			}
			p_trade_shop_item_info->set_item_tid(p_trade_item->get_id());
			p_trade_shop_item_info->set_can_buy_max_num(trade_item_can_buy_max_num);
			p_trade_shop_item_info->set_item_buy_price(trade_item_buy_price);
			p_trade_shop_item_info->set_item_sell_price(p_trade_item->get_sell_price());
			p_trade_shop_item_info->set_trade_market(p_trade_item->get_market() - p_trade_item->get_last_market());
			p_trade_shop_item_info->set_new_trade_market(new_market);
			p_trade_shop_item_info->set_city_buy_max_num(p_trade_item->get_can_buy_num());
			/*log_warn("trade_id[%d] id[%d] can_buy_max_num[%d] buy_price[%d] sell_price[%d] new_market[%d] get_can_buy_num[%d]",
				trade_id, p_trade_item->get_id(), trade_item_can_buy_max_num, trade_item_buy_price, p_trade_item->get_sell_price(), p_trade_item->get_market(), p_trade_item->get_can_buy_num());*/

		}
	} while (false);
	
	return reply_code;
}


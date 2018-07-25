#include "business_act_data_manager.hpp"
#include "utility.hpp"
#include "redis_client.hpp"
USING_NS_COMMON;

bool business_act_data_manager_t::load_business_act_data_to_redis(uint64_t uid, proto::common::bussiness_act_data* p_list)
{
	//Ò¡Ç®Ê÷¶Áµµ
	proto::common::money_tree_data *p_money_tree = p_list->mutable_money_tree_info();
	std::string money_tree_str = redis_client_t::get_string("business_act", "money_tree", uid);
	if (!money_tree_str.empty())
	{
		std::vector<uint32_t> tmp_vec;
		common::string_util_t::split<uint32_t>(money_tree_str, tmp_vec, "$");
		if (tmp_vec.size() == 2)
		{
			p_money_tree->set_level( tmp_vec[0]);
			p_money_tree->set_count( tmp_vec[1]);
		}
	}

	//ÆßÈÕµÇÂ¼¶Áµµ
	uint32_t sevenday_login_day = redis_client_t::get_uint32("business_act", "sevenday_login_day", uid);
	uint32_t sevenday_login_flag = redis_client_t::get_uint32("business_act", "sevenday_login_flag", uid);
	std::string sevenday_login_data_str = redis_client_t::get_string("business_act", "sevenday_login_data", uid);

	proto::common::sevenday_login_save_data *p_login_save_data = p_list->mutable_sevenday_data();
	p_login_save_data->set_day(sevenday_login_day);
	p_login_save_data->set_login_flag(sevenday_login_flag);
	if (!sevenday_login_data_str.empty())
	{
		std::vector<uint32_t> data_list;
		common::string_util_t::split<uint32_t>(sevenday_login_data_str, data_list, "$");
		for (auto& it : data_list)
		{
			p_login_save_data->add_days(it);
		}
	}
	
	//ÔÂ¿¨¶Áµµ
	proto::common::month_card_save_data *p_month_card = p_list->mutable_month_card_data();
	std::string month_card_str = redis_client_t::get_string("business_act", "month_card", uid);
	if (!month_card_str.empty())
	{
		std::vector<std::string> data_list;
		common::string_util_t::split<std::string>(month_card_str, data_list, "$");
		for ( auto it : data_list)
		{ 
			std::vector<uint32_t> data_vec;
			common::string_util_t::split<uint32_t>(it, data_vec, ":");
			if (data_vec.size() == 3)
			{
				proto::common::month_card_info *p_conf = p_month_card->add_card_info();
				p_conf->set_month_card_id(data_vec[0]);
				p_conf->set_end_time(data_vec[1]);
				p_conf->set_ed_prize(data_vec[2]);
			}
		}
	}

	//»ù½ð¶Áµµ
	uint32_t level_fund_buy_flag = redis_client_t::get_uint32("business_act", "level_fund_buy_flag", uid);
	std::string level_fund_prize_str = redis_client_t::get_string("business_act", "level_fund_prize", uid);

	proto::common::level_fund_save_info *p_level_fund = p_list->mutable_level_fund_data();
	p_level_fund->set_buy_flag(level_fund_buy_flag);
	if (!level_fund_prize_str.empty())
	{
		std::vector<uint32_t> data_list;
		common::string_util_t::split<uint32_t>(level_fund_prize_str, data_list, "$");
		for (auto& it : data_list)
		{
			p_level_fund->add_id(it);
		}
	}

	//ÏÞ¹º¶Áµµ
	uint32_t limit_buy_day = redis_client_t::get_uint32("business_act", "limit_buy_day", uid);
	uint32_t red = redis_client_t::get_uint32("business_act", "limit_buy_red", uid);
	std::string limit_buy_data_str = redis_client_t::get_string("business_act", "limit_buy_data", uid);

	proto::common::limit_buy_save_data *p_limit_buy = p_list->mutable_limit_buy_data();
	p_limit_buy->set_day(limit_buy_day);
	p_limit_buy->set_red(red);

	if (!limit_buy_data_str.empty())
	{
		std::vector<std::string> data_list;
		common::string_util_t::split<std::string>(limit_buy_data_str, data_list, "$");
		for (auto it : data_list)
		{
			std::vector<uint32_t> data_vec;
			common::string_util_t::split<uint32_t>(it, data_vec, ":");
			if (data_vec.size() == 2)
			{
				proto::common::limit_buy_save_info *p_info = p_limit_buy->add_info();
				p_info->set_id(data_vec[0]);
				p_info->set_num(data_vec[1]);
			}
		}
	}


	//¶Ò»»¶Áµµ
	uint32_t exchange_item_start_time = redis_client_t::get_uint32("business_act", "exchange_item_start_time", uid);
	std::string exchange_item_data_str = redis_client_t::get_string("business_act", "exchange_item_data", uid);

	proto::common::exchange_item_save_data *p_exchange_item_data= p_list->mutable_exchange_data();
	p_exchange_item_data->set_start_time( exchange_item_start_time );

	if (!exchange_item_data_str.empty())
	{
		std::vector<std::string> data_list;
		common::string_util_t::split<std::string>(exchange_item_data_str, data_list, "$");
		for (auto it : data_list)
		{
			std::vector<uint32_t> data_vec;
			common::string_util_t::split<uint32_t>(it, data_vec, ":");
			if (data_vec.size() == 2)
			{
				proto::common::exchange_item_save_info *p_info = p_exchange_item_data->add_info();
				p_info->set_id(data_vec[0]);
				p_info->set_cur_num(data_vec[1]);
			}
		}
	}
	
	//Í³¼ÆÊý¾Ý¶Áµµ
	std::string statistical_data_str = redis_client_t::get_string("business_act", "statistical_data", uid);
	proto::common::statistical_save_data *p_statistical_data= p_list->mutable_statistical_data();
	if (!statistical_data_str.empty())
	{
		std::vector<std::string> data_list;
		common::string_util_t::split<std::string>(statistical_data_str, data_list, "$");
		for (auto it : data_list)
		{
			std::vector<uint32_t> data_vec;
			common::string_util_t::split<uint32_t>(it, data_vec, ":");
			if (data_vec.size() == 2)
			{
				proto::common::statistical_save_info *p_info = p_statistical_data->add_info();
				p_info->set_id(data_vec[0]);
				p_info->set_num(data_vec[1]);
			}
		}
	}

	//ÆßÈÕÄ¿±ê¶Áµµo
	uint32_t day = redis_client_t::get_uint32("business_act", "sd_target_day", uid);
	std::string sd_target_data_str = redis_client_t::get_string("business_act", "sd_target_data", uid);
	proto::common::sd_target_save_data *p_sd_target_data = p_list->mutable_sevenday_target_data();
	p_sd_target_data->set_day(day);
	if (!sd_target_data_str.empty())
	{
		std::vector<std::uint32_t> data_list;
		common::string_util_t::split<std::uint32_t>(sd_target_data_str, data_list, "$");
		for (auto it : data_list)
		{
			p_sd_target_data->add_info(it);
		}
	}

	//ÀÛ³ä½±Àø
	std::string recharge_total_str = redis_client_t::get_string("business_act", "recharge_total", uid);
	if (!recharge_total_str.empty())
	{
		proto::common::recharge_total_data *p_data = p_list->mutable_recharge_total();
		std::vector<uint32_t> recharge_total_ids;
		string_util_t::split<uint32_t>(recharge_total_str, recharge_total_ids, "$");
		for (auto recharge_total_id : recharge_total_ids)
		{
			p_data->add_recharge_total_id(recharge_total_id);
		}
	}

	//»Æ½ðÊ®Á¬
	std::string gold_luckydraw_str = redis_client_t::get_string("business_act", "gold_luckydraw_data", uid);
	if (!gold_luckydraw_str.empty())
	{
		proto::common::gold_luckydraw_data *p_data = p_list->mutable_gold_luckydraw();
		std::vector<uint32_t> ids;
		string_util_t::split<uint32_t>(gold_luckydraw_str, ids, "$");
		for (auto id : ids)
		{
			p_data->add_gold_luckydraw_id(id);
		}
	}
	return true;
}

bool business_act_data_manager_t::save_money_tree_data_to_redis(uint64_t uid, const proto::common::money_tree_data &data )
{
	std::ostringstream sss;
	sss << data.level() << "$" << data.count();
	redis_client_t::set_string("business_act", "money_tree", uid, sss.str());
	return true;
}

bool business_act_data_manager_t::save_sevenday_login_data_to_redis(uint64_t uid, const proto::common::sevenday_login_save_data & list)
{
	redis_client_t::set_uint32("business_act", "sevenday_login_day", uid, list.day() );
	redis_client_t::set_uint32("business_act", "sevenday_login_flag", uid, list.login_flag() );

	std::ostringstream sss;
	for (int32_t i = 0; i < list.days_size(); ++i)
	{
		if (!sss.str().empty())
			sss << "$";
		sss << list.days(i);
	}
	redis_client_t::set_string("business_act", "sevenday_login_data", uid, sss.str());
	return true;
}

bool business_act_data_manager_t::save_month_card_data_to_redis(uint64_t uid, const proto::common::month_card_save_data &list)
{
	std::ostringstream sss;
	for (int32_t i = 0; i < list.card_info_size(); ++i)
	{
		const proto::common::month_card_info &card = list.card_info(i);
		if (!sss.str().empty())
			sss << "$";
		sss << card.month_card_id() << ":" << card.end_time() << ":" << card.ed_prize();
	}
	redis_client_t::set_string("business_act", "month_card", uid, sss.str());
	return true;
}

//»ù½ð´æ´¢
bool business_act_data_manager_t::save_level_fund_data_to_redis(uint64_t uid, const proto::common::level_fund_save_info  &list)
{
	redis_client_t::set_uint32("business_act", "level_fund_buy_flag", uid, list.buy_flag() );
	std::ostringstream sss;
	for (int32_t i = 0; i < list.id_size(); ++i)
	{
		if (!sss.str().empty())
			sss << "$";
		sss << list.id(i);
	}
	redis_client_t::set_string("business_act", "level_fund_prize", uid, sss.str());
	return true;
}


//ÏÞ¹º
bool business_act_data_manager_t::save_limit_buy_data_to_redis(uint64_t uid, const proto::common::limit_buy_save_data  &list)
{
	redis_client_t::set_uint32("business_act", "limit_buy_day", uid, list.day() );
	redis_client_t::set_uint32("business_act", "limit_buy_red", uid, list.red() );
	std::ostringstream sss;
	for (int32_t i = 0; i < list.info_size(); ++i)
	{
		const proto::common::limit_buy_save_info &pinfo = list.info(i);
		if (!sss.str().empty())
			sss << "$";
		sss << pinfo.id() << ":" << pinfo.num();
	}
	redis_client_t::set_string("business_act", "limit_buy_data", uid, sss.str());
	return true;
}

//¶Ò»»
bool business_act_data_manager_t::save_exchange_item_data_to_redis(uint64_t uid, const proto::common::exchange_item_save_data  &list)
{
	redis_client_t::set_uint32("business_act", "exchange_item_start_time", uid, list.start_time());

	std::ostringstream sss;
	for (int32_t i = 0; i < list.info_size(); ++i)
	{
		const proto::common::exchange_item_save_info &pinfo = list.info(i);
			if (!sss.str().empty())
				sss << "$";
		sss << pinfo.id() << ":" << pinfo.cur_num();
	}
	redis_client_t::set_string("business_act", "exchange_item_data", uid, sss.str());
	return true;
}

//ÆßÈÕÄ¿±ê´æ´¢
bool business_act_data_manager_t::save_sevenday_target_data_to_redis(uint64_t uid, const proto::common::sd_target_save_data &list)
{
	redis_client_t::set_uint32("business_act", "sd_target_day", uid, list.day());
	std::ostringstream sss;
	for (int32_t i = 0; i < list.info_size(); ++i)
	{
		if (!sss.str().empty())
			sss << "$";
		sss << list.info(i);
	}
	redis_client_t::set_string("business_act", "sd_target_data", uid, sss.str());
	return true;
}


//Í³¼ÆÊý¾Ý
bool business_act_data_manager_t::save_statistical_data_to_redis(uint64_t uid, const proto::common::statistical_save_data &list)
{
	std::ostringstream sss;
	for (int32_t i = 0; i < list.info_size(); ++i)
	{
		const proto::common::statistical_save_info &pinfo = list.info(i);
		if (!sss.str().empty())
			sss << "$";
		sss << pinfo.id() << ":" << pinfo.num();
	}
	redis_client_t::set_string("business_act", "statistical_data", uid, sss.str());
	return true;
}

bool business_act_data_manager_t::save_recharger_total_data_to_redis(uint64_t uid, const proto::common::recharge_total_data &list)
{
	std::ostringstream oss_recharge_total;
	for (auto recharge_total_id : list.recharge_total_id())
	{
		if (!oss_recharge_total.str().empty())
		{
			oss_recharge_total << "$";
		}

		oss_recharge_total << recharge_total_id;
	}
	redis_client_t::set_string("business_act", "recharge_total", uid, oss_recharge_total.str());
	return true;
}

bool business_act_data_manager_t::save_gold_luckydraw_data_to_redis(uint64_t uid, const proto::common::gold_luckydraw_data &list)
{
	std::ostringstream oss_gold_luckydraw;
	for (auto gold_luckydraw_id : list.gold_luckydraw_id())
	{
		if (!oss_gold_luckydraw.str().empty())
		{
			oss_gold_luckydraw << "$";
		}

		oss_gold_luckydraw << gold_luckydraw_id;
	}
	redis_client_t::set_string("business_act", "gold_luckydraw_data", uid, oss_gold_luckydraw.str());
	return true;
}


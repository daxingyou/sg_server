#include "business_act_data_manager.hpp"
#include "utility.hpp"
#include "common/redis_client.hpp"


bool business_act_data_manager_t::load_business_act_data_to_redis(uint64_t uid, proto::common::bussiness_act_data* p_list)
{
	//Ò¡Ç®Ê÷¶Áµµ
	proto::common::money_tree_data *p_money_tree = p_list->mutable_money_tree_info();
	std::string money_tree_str = redis_client_t::get_string("role_ex", "money_tree", uid);
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
	uint32_t sevenday_login_day = redis_client_t::get_uint32("role_ex", "sevenday_login_day", uid);
	uint32_t sevenday_login_flag = redis_client_t::get_uint32("role_ex", "sevenday_login_flag", uid);
	std::string sevenday_login_data_str = redis_client_t::get_string("role_ex", "sevenday_login_data", uid);

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
	return true;
}

bool business_act_data_manager_t::save_money_tree_data_to_redis(uint64_t uid, const proto::common::money_tree_data &data )
{
	std::ostringstream sss;
	sss << data.level() << "$" << data.count();
	redis_client_t::set_string("role_ex", "money_tree", uid, sss.str());
	return true;
}

bool business_act_data_manager_t::save_sevenday_login_data_to_redis(uint64_t uid, const proto::common::sevenday_login_save_data & list)
{
	redis_client_t::set_uint32("role_ex", "sevenday_login_day", uid, list.day() );
	redis_client_t::set_uint32("role_ex", "sevenday_login_flag", uid, list.login_flag() );

	std::ostringstream sss;
	for (int32_t i = 0; i < list.days_size(); ++i)
	{
		if (!sss.str().empty())
			sss << "$";
		sss << list.days(i);
	}
	redis_client_t::set_string("role_ex", "sevenday_login_data", uid, sss.str());
	return true;
}

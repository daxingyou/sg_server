#include "luckydraw_mgr.hpp"
#include "tblh/LuckydrawWish.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "common/config_mgr.h"
#include "tblh/Country.tbls.h"
#include "redis/country_data_mgr.hpp"
#include "utility.hpp"
#include "mail/mail_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/comprehensive_common.hpp"
#include "tblh/MailTable.tbls.h"
#include "mail/mail_manager.hpp"

wish_summon_map luckydraw_mgr_t::sys_wish_summon_map;
std::map<uint32_t, wish_summon_map>  luckydraw_mgr_t::king_wish_summon_map;
std::map<uint32_t, uint32_t> luckydraw_mgr_t::king_money_map;

bool luckydraw_mgr_t::init() 
{
	luckydraw_mgr_t::king_wish_summon_map.clear();
	luckydraw_mgr_t::sys_wish_summon_map.clear();
	luckydraw_mgr_t::king_money_map.clear();

	//初始化系统类型的祈福礼包
	std::map<uint32_t, LuckydrawWish*> config_map;
	GET_ALL_CONF(LuckydrawWish, config_map);
	for (auto iter : config_map) 
	{
		auto p_conf = iter.second;
		if (NULL == p_conf) 
			continue;

		if (p_conf->type() != proto::common::luckydraw_type::luckydraw_type_wish || p_conf->default_open() != 1)
			continue;
		
		proto::common::wish_summon_info wish_info;
		wish_info.set_wish_summon_id(p_conf->get_id());
		wish_info.set_open_flag(0);
		wish_info.set_time_left(0);
		wish_info.set_buy_count(0);
		luckydraw_mgr_t::sys_wish_summon_map.insert(std::make_pair(p_conf->get_id(), wish_info));
	}
	
	std::map<uint32_t, Country*> country_map;
	GET_ALL_CONF(Country, country_map);
	for (auto it : country_map)
	{
		wish_summon_map wish_summon_list;
		wish_summon_list.clear();
		for (auto iter : config_map)
		{
			auto p_conf = iter.second;
			if (NULL == p_conf)
				continue;

			if (p_conf->type() != proto::common::luckydraw_type::luckydraw_type_wish || p_conf->default_open() != 2)
				continue;

			proto::common::wish_summon_info wish_info;
			wish_info.set_wish_summon_id(p_conf->get_id());
			wish_info.set_open_flag(0);
			wish_info.set_time_left(0);
			wish_info.set_buy_count(0);
			wish_summon_list.insert(std::make_pair(p_conf->get_id(), wish_info));
		}
		
		//读档
		std::string king_wish_info = country_data_mgr_t::load_king_luckydraw_info(it.first);
		std::vector<std::string> info_list;
		string_util_t::split<std::string>(king_wish_info, info_list, ";");
		for (auto info_str : info_list)
		{
			std::vector<uint32_t> info;
			string_util_t::split<uint32_t>(info_str,info, "$");
			if (info.size() != 4)
			{
				log_error("load king_wish_data  error");
				return false;
			}

			proto::common::wish_summon_info wish_info;
			wish_info.set_wish_summon_id(info[0]);
			wish_info.set_open_flag(info[1]);
			wish_info.set_time_left(info[2]);
			wish_info.set_buy_count(info[3]);
			wish_summon_list[info[0]] = wish_info;
		}
		luckydraw_mgr_t::king_wish_summon_map.insert(std::make_pair(it.first, wish_summon_list));

		std::uint32_t money = country_data_mgr_t::load_king_luckydraw_money(it.first);
		luckydraw_mgr_t::king_money_map.insert(std::make_pair(it.first, money));
		luckydraw_mgr_t::save_wish_info( it.first );
		luckydraw_mgr_t::save_king_money( it.first, money );
	}


	return true;
}

//开启国王礼包
uint32_t luckydraw_mgr_t::open_king_wish_summon_req(const global_user_ptr &p_user, uint32_t summon_id)
{
	if (NULL == p_user)
	{
		log_error("open_king_wish_summon p_user is NULL");
		return errcode_enum::notice_unknown;
	}

	if (p_user->get_country_officer() != officer_type::officer_type_king)
	{
		log_error("user[%lu] is not king", p_user->get_uid());
		return errcode_enum::notice_unknown;
	}

	uint32_t country_id = p_user->get_country_id();

	auto iter_f = luckydraw_mgr_t::king_wish_summon_map.find(country_id);
	if (iter_f == luckydraw_mgr_t::king_wish_summon_map.end())
	{
		log_error("role[%lu] country_id[%u] not found", p_user->get_uid(), country_id);
		return errcode_enum::notice_unknown;
	}

	auto &wish_summon_list = iter_f->second;
	auto iter_summon_f = wish_summon_list.find(summon_id);
	if (iter_summon_f == wish_summon_list.end())
	{
		log_error("role[%lu] summon_id[%u] not found", p_user->get_uid(), summon_id);
		return errcode_enum::notice_unknown;
	}

	//检查礼包是否已经开启过
	uint32_t now_time = time_util_t::now_time();
	auto wish_info = iter_summon_f->second;
	if ( wish_info.time_left() > now_time || wish_info.open_flag() == 2 )
	{
		log_error("role[%lu] summon_id[%u] has opend", p_user->get_uid(), summon_id);
		return errcode_enum::notice_unknown;
	}

	//检查礼包是否可以开启
	LuckydrawWish* p_conf = GET_CONF(LuckydrawWish, summon_id);
	if (NULL == p_conf) {
		log_error("role[%lu] LuckydrawWish[%u] not find", p_user->get_uid(), summon_id);
		return errcode_enum::notice_unknown;
	}

	if (p_conf->type() != proto::common::luckydraw_type::luckydraw_type_wish || p_conf->default_open() != 2)
	{
		log_error("role[%lu] try open not king wish summon[%u]", p_user->get_uid(), summon_id);
		return errcode_enum::notice_unknown;
	}

	//用来计数开启次数
	wish_info.set_open_flag(2);

	//TODO判断次数之类的,暂时没有需求
	return errcode_enum::error_ok;
}

//购买祈福礼包
uint32_t luckydraw_mgr_t::buy_wish_summon_req(const global_user_ptr &p_user, uint32_t summon_id)
{
	if (NULL == p_user)
	{
		log_error("p_role == NULL");
		return errcode_enum::notice_unknown;
	}

	auto iter_f = luckydraw_mgr_t::sys_wish_summon_map.find(summon_id);
	if (iter_f != luckydraw_mgr_t::sys_wish_summon_map.end())
	{//系统礼包
		return errcode_enum::error_ok;
	}
	else 
	{//国王礼包
		auto iter_f = luckydraw_mgr_t::king_wish_summon_map.find(p_user->get_country_id());
		if (iter_f == luckydraw_mgr_t::king_wish_summon_map.end())
		{
			log_error("user[%lu] buy country wish not found", p_user->get_uid());
			return errcode_enum::notice_unknown;
		}

		auto &wish_summon_list = iter_f->second;
		auto iter_summon_f = wish_summon_list.find(summon_id);
		if (iter_summon_f == wish_summon_list.end())
		{
			log_error("user[%lu] buy wish summon[%u] not exist", p_user->get_uid(), summon_id);
			return errcode_enum::notice_unknown;
		}

		uint32_t now_time = time_util_t::now_time();
		auto &wish_info = iter_summon_f->second;
		if ( now_time > wish_info.time_left() )
		{
			log_error("role[%lu], country[%u], summon_id[%u] has closed", p_user->get_uid(), p_user->get_country_id(), summon_id);
			return errcode_enum::card_time_out;
		}
		return errcode_enum::error_ok;
	}
	return errcode_enum::notice_unknown;
}

//开启礼包
uint32_t luckydraw_mgr_t::on_open_wish_summon(const global_user_ptr &p_user, uint32_t summon_id, proto::client::ec_open_summon_wish_reply &msg)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return errcode_enum::notice_unknown;
	}

	LuckydrawWish* p_conf = GET_CONF(LuckydrawWish, summon_id);
	if (NULL == p_conf) {
		log_error("LuckydrawWish[%u] not find",summon_id);
		return errcode_enum::notice_unknown;
	}
	
	uint32_t country_id = p_user->get_country_id();
	uint32_t now_time = time_util_t::now_time();
	auto iter_f = luckydraw_mgr_t::king_wish_summon_map.find(country_id);
	if (iter_f == luckydraw_mgr_t::king_wish_summon_map.end())
	{
		log_error("role[%lu] country_id[%u] not found", p_user->get_uid(), country_id);
		return errcode_enum::notice_unknown;
	}

	auto &wish_summon_list = iter_f->second;
	auto iter_summon_f = wish_summon_list.find(summon_id);
	if (iter_summon_f == wish_summon_list.end())
	{
		log_error("role[%lu] summon_id[%u] not found", p_user->get_uid(), summon_id);
		return errcode_enum::notice_unknown;
	}

	auto &wish_info = iter_summon_f->second;
	wish_info.set_wish_summon_id(summon_id);
	wish_info.set_open_flag(1);
	wish_info.set_time_left( now_time + p_conf->open_time() * 3600 );
	wish_info.set_buy_count(0);
	luckydraw_mgr_t::save_wish_info(p_user->get_country_id());

	auto *p_wish_info = msg.mutable_info();
	p_wish_info->CopyFrom(wish_info);
	p_wish_info->set_time_left(p_conf->open_time() * 3600);

	return errcode_enum::error_ok;
}

//购买国王礼包返回
uint32_t luckydraw_mgr_t::on_buy_wish_summon(const global_user_ptr &p_user, uint32_t summon_id)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return errcode_enum::notice_unknown;
	}

	LuckydrawWish* p_conf = GET_CONF(LuckydrawWish, summon_id);
	if (NULL == p_conf) {
		log_error("role[%lu] LuckydrawWish[%u] not find", p_user->get_uid(), summon_id);
		return errcode_enum::notice_unknown;
	}

	if (p_conf->type() == proto::common::luckydraw_type::luckydraw_type_wish && p_conf->default_open() == 1)
	{
		auto iter_summon_f = luckydraw_mgr_t::sys_wish_summon_map.find(summon_id);
		if (iter_summon_f == luckydraw_mgr_t::sys_wish_summon_map.end())
		{
			log_error("user[%lu] buy wish summon[%u] not exist", p_user->get_uid(), summon_id );
			return errcode_enum::notice_unknown;
		}
		auto &wish_info = iter_summon_f->second;
		uint32_t count = wish_info.buy_count() + 1;
		wish_info.set_buy_count(count);
		luckydraw_mgr_t::save_wish_info(p_user->get_country_id());
	}
	else
	{
		auto iter_f = luckydraw_mgr_t::king_wish_summon_map.find(p_user->get_country_id());
		if (iter_f == luckydraw_mgr_t::king_wish_summon_map.end())
		{
			log_error("user[%lu] buy country wish not found", p_user->get_uid());
			return errcode_enum::notice_unknown;
		}

		auto &wish_summon_list = iter_f->second;
		auto iter_summon_f = wish_summon_list.find(summon_id);
		if (iter_summon_f == wish_summon_list.end())
		{
			log_error("user[%lu] buy wish summon[%u] not exist", p_user->get_uid(), summon_id );
			return errcode_enum::notice_unknown;
		}

		auto &wish_info = iter_summon_f->second;
		uint32_t count = wish_info.buy_count() + 1;
		wish_info.set_buy_count(count);
	
		auto iter_money_f = luckydraw_mgr_t::king_money_map.find(p_user->get_country_id());
		if ( iter_money_f == luckydraw_mgr_t::king_money_map.end() )
		{
			log_error("user[%lu] add king[%u] money  not exist", p_user->get_uid(), p_user->get_country_id() );
			return errcode_enum::notice_unknown;
		}
			
		auto money_per_str = GET_CONF(Comprehensive, comprehensive_common::draw_money_scale);
		uint32_t money_per = GET_COMPREHENSIVE_VALUE_1(money_per_str);
		uint32_t money = p_conf->cost_resource.num * (money_per / 10000.0f);

		auto &king_money = iter_money_f->second;
		king_money += money;

		luckydraw_mgr_t::save_wish_info(p_user->get_country_id());
		luckydraw_mgr_t::save_king_money(p_user->get_country_id(), king_money);
	}
	return errcode_enum::error_ok;
}

uint32_t luckydraw_mgr_t::get_wish_summon_info_request(const global_user_ptr &p_user, proto::client::ec_get_wish_summon_info_reply &reply)
{
	if (NULL == p_user)
	{
		log_error(" NULL == p_user ");
		return errcode_enum::error_ok;
	}

	//打包系统数据	
	for (auto it : luckydraw_mgr_t::sys_wish_summon_map)
	{
		auto *p_info = reply.add_ws_list();
		p_info->CopyFrom(it.second);
	}

	uint32_t now_time = time_util_t::now_time();
	uint32_t country_id = p_user->get_country_id();
	//打包国家数据
	if (country_id != 0)
	{
		auto iter_f = luckydraw_mgr_t::king_wish_summon_map.find(country_id);
		if (iter_f == luckydraw_mgr_t::king_wish_summon_map.end())
		{
			log_error("country wish info not found");
			return errcode_enum::notice_unknown;
		}

		auto &wish_map = iter_f->second;
		for (auto it : wish_map)
		{
			auto wish_info = it.second;
			auto *p_info = reply.add_ws_list();
			p_info->CopyFrom(it.second);
			uint32_t time_left = now_time > wish_info.time_left() ? 0 : wish_info.time_left() - now_time;
			p_info->set_time_left( time_left );
		}

		auto iter_money_f = luckydraw_mgr_t::king_money_map.find(country_id);
		if (iter_money_f == luckydraw_mgr_t::king_money_map.end())
		{
			log_error("king money info not found");
			return errcode_enum::notice_unknown;
		}
		reply.set_king_moeny( iter_money_f->second );
	}
	return errcode_enum::error_ok;
}

bool luckydraw_mgr_t::save_wish_info(uint32_t country_id)
{
	auto iter_f = luckydraw_mgr_t::king_wish_summon_map.find(country_id);
	if (iter_f == luckydraw_mgr_t::king_wish_summon_map.end())
	{
		log_error("save country info not found country[%u]", country_id);
		return false;
	}
	
	std::ostringstream sss;
	auto &wish_info_map = iter_f->second;
	for (auto it : wish_info_map)
	{
		auto &wish_info = it.second;
		if (!sss.str().empty())
		{
			sss << ";";
		}
		sss << wish_info.wish_summon_id() << "$";
		sss << wish_info.open_flag() << "$";
		sss << wish_info.time_left() << "$";
		sss << wish_info.buy_count();
	}
	std::string var = sss.str();
	country_data_mgr_t::update_king_luckydraw_info(country_id, var);

	return true;
}

bool luckydraw_mgr_t::save_king_money(uint32_t country_id, uint32_t money)
{
	country_data_mgr_t::update_king_luckydraw_money(country_id, money);
	return true;
}

uint32_t luckydraw_mgr_t::get_king_money(const global_user_ptr &p_user, bool force )
{
	if (NULL == p_user)
	{
		log_error(" NULL == p_user ");
		return errcode_enum::notice_unknown;
	}

	if (p_user->get_country_officer() != officer_type::officer_type_king && force == false )
	{
		log_error("user[%lu] is not king", p_user->get_uid());
		return errcode_enum::notice_unknown;
	}

	uint32_t country_id = p_user->get_country_id();
	auto iter_f = luckydraw_mgr_t::king_money_map.find(country_id);
	if (iter_f == luckydraw_mgr_t::king_money_map.end())
	{
		log_error("user[%lu] no country[%u] money", p_user->get_uid(), country_id);
		return errcode_enum::notice_unknown;
	}

	uint32_t money = iter_f->second;
	iter_f->second = 0;
	luckydraw_mgr_t::save_king_money(country_id, 0);

	//发送邮件
	auto comp_king_mail_conf = GET_CONF(Comprehensive, comprehensive_common::draw_mail_id);
	uint32_t mail_id = GET_COMPREHENSIVE_VALUE_1(comp_king_mail_conf);

	MailTable* p_mail = GET_CONF(MailTable, mail_id); 
	if (NULL == p_mail) 
	{ 
		log_error("user[%lu] p_mail[%u] error null", p_user->get_uid(), mail_id); 
		return errcode_enum::notice_unknown;
	}

	std::string content_text; 
	StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_mail->content()).c_str(), money ); 

	std::map<uint32_t, uint32_t> items_id_count;
	items_id_count[100001] = money;
	mail_manager_t::send_mail(p_user->get_uid(), proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_mail->from_name()),
		common::string_util_t::convert_to_utf8(p_mail->title()), content_text,  items_id_count, proto::common::MAIL_GET_TYPE_LUCKYDRAW_KING_MONEY);
	return errcode_enum::error_ok;
}


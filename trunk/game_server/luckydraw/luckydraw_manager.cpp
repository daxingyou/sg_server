#include "luckydraw_manager.hpp"
#include "luckydraw.hpp"
#include "log/log_wrapper.hpp"
#include "item/item_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "hero/hero_manager.hpp"
#include "item/drop_manager.hpp"
#include "tblh/mailcode_enum.hpp"
#include "mail/mail_manager.hpp"
#include "role/money_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "tblh/LuckydrawSeq.tbls.h"
#include "tblh/LuckydrawWish.tbls.h"
#include "tblh/LuckydrawType.tbls.h"
#include "tblh/Hero.tbls.h"
#include "role/role.hpp"
#include "general_event/general_info_manager.hpp"
#include "rank/rank_manager.hpp"

USING_NS_COMMON;

luckydraw_pool luckydraw_manager_t::s_luckydraw_pool;

bool luckydraw_manager_t::make_luckydraw_pool()
{
	s_luckydraw_pool.clear();
	std::map<uint32_t, Luckydraw*> luckydraw_map;
	GET_ALL_CONF(Luckydraw, luckydraw_map);
	
	for (auto it : luckydraw_map)
	{
		auto p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		uint32_t type = p_conf->type();
		s_luckydraw_pool[type].push_back(p_conf);
	}
	return luckydraw_t::init_senoir_seq();
}

void luckydraw_manager_t::clear_luckydraw_pool()
{
	s_luckydraw_pool.clear();
}

uint32_t luckydraw_manager_t::luckydraw(role_ptr p_role, uint32_t type, uint32_t count, proto::client::gc_luckydraw_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_unknown;
	}

	if (type == proto::common::luckydraw_type_junior)
	{
		return luckydraw_junior(p_role, count, reply);
	}
	else if(type == proto::common::luckydraw_type_senior)
	{
		luckydraw_ptr p_luckydraw = p_role->get_luckydraw();
		if (NULL == p_luckydraw)
		{
			log_error("NULL == p_luckydraw");
			return errcode_enum::notice_unknown;
		}
		if (p_luckydraw->lucky_full())
		{
			return luckydraw_gold(p_role, reply);
		}
		else
		{
			return luckydraw_senior(p_role, count, reply);
		}
	}
	else if (type == proto::common::luckydraw_type_general)
	{
		return luckydraw_general(p_role, 1, reply);
	}
	return errcode_enum::notice_unknown;
}

void luckydraw_manager_t::get_luckydraw_data(role_ptr p_role, proto::common::luckydraw_client_data* p_data)
{
	//if (!is_today_limit())
	//{
	//	return;
	//}
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	auto p_luckydraw = p_role->get_luckydraw();
	if (NULL == p_luckydraw)
	{
		log_error("NULL == p_luckydraw");
		return;
	}
	p_luckydraw->peek_client_data(p_data);
	uint32_t et = business_activity_manager_t::get_gold_luckydraw_expired_time(p_role);
	p_data->set_gold_luckydraw_expired_time(et);
}

uint32_t luckydraw_manager_t::luckydraw_junior(role_ptr p_role, uint32_t count, proto::client::gc_luckydraw_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	auto p_luckydraw = p_role->get_luckydraw();
	if (NULL == p_luckydraw)
	{
		log_error("NULL == p_luckydraw");
		return errcode_enum::notice_role_null;
	}
	if (is_today_limit() && p_luckydraw->get_today_count(proto::common::luckydraw_type_junior) + count > get_today_draw_limit_number())
	{
		log_error("p_role[%lu] luckydraw_junior today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	if (is_today_limit() && p_luckydraw->get_today_count() + count > get_today_draw_total_limit_number())
	{
		log_error("p_role[%lu] luckydraw_junior today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	LuckydrawType* p_conf = GET_CONF(LuckydrawType, proto::common::luckydraw_type_junior);
	if (NULL == p_conf) {
		log_error("LuckydrawType[%u] not find", proto::common::luckydraw_type_junior);
		return errcode_enum::notice_unknown;
	}
	uint32_t card_index = general_info_manager_t::get_card_index();
	if (card_index >= p_conf->card().size())
	{
		log_error("LuckydrawType[%u] card_size error", proto::common::luckydraw_type_junior);
		return errcode_enum::notice_unknown;
	}
	auto it = s_luckydraw_pool.find(p_conf->card().at(card_index));
	if (it == s_luckydraw_pool.end())
	{
		log_error("s_luckydraw_pool error");
		return errcode_enum::notice_unknown;
	}
	const std::vector<Luckydraw*> luckydraw_vec = it->second;
	if (luckydraw_vec.empty())
	{
		log_error("s_luckydraw_pool error");
		return errcode_enum::notice_unknown;
	}
	uint32_t rand_max = 0;
	for (auto luckydraw_conf : luckydraw_vec)
	{
		if (NULL == luckydraw_conf)
		{
			continue;
		}
		rand_max += luckydraw_conf->prob();
	}
	if (0 == rand_max)
	{
		log_error("0 == rand_max");
		return errcode_enum::notice_unknown;
	}
	if (0 == count)
	{
		log_error("0 == count");
		return errcode_enum::notice_unknown;
	}
	if (count > get_normal_draw_limit_number())
	{
		count = get_normal_draw_limit_number();
	}


	if (!item_manager_t::has_item(p_role, p_conf->material().first, p_conf->material().second))
	{
		log_error("role[%lu] item not enough", p_role->get_uid());
		return errcode_enum::notice_item_not_enough;
	}

	for (uint32_t i = 0; i < count; ++i)
	{
		if (errcode_enum::error_ok != item_manager_t::remove_item(p_role, p_conf->material().first, p_conf->material().second, log_enum::source_type_hero_luckydraw, 0, reply.mutable_rcd()))
		{
			count = i;
			break;
		}

		int32_t rand_val = random_util_t::randMin(0, rand_max);
		if (-1 == rand_val)
		{
			log_error("rand_max invalid");
			return errcode_enum::notice_unknown;
		}
		
		for (auto luckydraw_conf : luckydraw_vec)
		{
			if (NULL == luckydraw_conf)
			{
				continue;
			}
			uint32_t prob = luckydraw_conf->prob();
			if ((uint32_t)rand_val < prob)
			{
				uint32_t reply_code = errcode_enum::error_ok;
				if (!luckydraw_award(p_role, proto::common::luckydraw_type_junior, luckydraw_conf, reply, reply_code))
				{
					return reply_code;
				}
				else
				{
					p_luckydraw->set_today_count(proto::common::luckydraw_type_junior, p_luckydraw->get_today_count(proto::common::luckydraw_type_junior) + 1);
				}
				break;
			}
			else
			{
				rand_val -= prob;
			}
		}
	}
	p_luckydraw->save_self();
	rank_manager_t::update_recruit(p_role->get_uid(), count);
	return errcode_enum::error_ok;
}

uint32_t luckydraw_manager_t::luckydraw_senior(role_ptr p_role, uint32_t count, proto::client::gc_luckydraw_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	uint32_t draw_type = proto::common::luckydraw_type_senior;

	auto p_luckydraw = p_role->get_luckydraw();
	if (NULL == p_luckydraw)
	{
		log_error("NULL == p_luckydraw");
		return errcode_enum::notice_role_null;
	}
	if (is_today_limit() && p_luckydraw->get_today_count(proto::common::luckydraw_type_senior) + count > get_today_draw_limit_number())
	{
		log_error("p_role[%lu] luckydraw_senior today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	if (is_today_limit() && p_luckydraw->get_today_count() + count > get_today_draw_total_limit_number())
	{
		log_error("p_role[%lu] luckydraw_senior today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	if (count > get_normal_draw_limit_number())
	{
		count = get_normal_draw_limit_number();
	}

	//目前没有反射表
	LuckydrawType* p_conf_type = GET_CONF(LuckydrawType, proto::common::luckydraw_type_senior);
	if (NULL == p_conf_type) {
		log_error("LuckydrawType[%u] not find", proto::common::luckydraw_type_senior);
		return errcode_enum::notice_unknown;
	}

	//消耗资源	
	if (errcode_enum::error_ok != item_manager_t::remove_item(p_role, p_conf_type->material().first, p_conf_type->material().second * count, log_enum::source_type_hero_luckydraw, 0, reply.mutable_rcd()))
	{
		log_error("role[%lu] item not enough", p_role->get_uid());
		return errcode_enum::notice_item_not_enough;
	}

	//元宝抽卡，多抽一次
	if (count == get_normal_draw_limit_number() )
	{
		count = 11;
	}

	for (uint32_t i = 0; i < count; ++i )
	{
		uint32_t senior_count = p_luckydraw->get_senior_count(draw_type);
		if (0 == senior_count) //首抽
		{
			uint32_t first_draw_id = get_first_draw_id();
			auto p_conf = GET_CONF(Luckydraw, first_draw_id);
			if (NULL == p_conf)
			{
				log_error("NULL == p_conf[%d]", first_draw_id);
				log_error("role[%lu], draw_error need_draw[%d], drawing[%d]", p_role->get_uid(), count, i);
				return errcode_enum::notice_unknown;
			}

			uint32_t reply_code = errcode_enum::error_ok;
			if (!luckydraw_award(p_role, proto::common::luckydraw_type_senior, p_conf, reply, reply_code))
			{
				log_error("role[%lu], draw_error need_draw[%d], drawing[%d]", p_role->get_uid(), count, i);
				return reply_code;
			}
		}
		else if (senior_count <= 10) //1-10的特殊抽卡逻辑
		{
			//特殊事件ID 999
			uint32_t draw_id = 999;
			uint32_t level = p_luckydraw->get_senior_level(draw_id, senior_count-1);
			if (0 == level)
			{
				log_error("0 == level  role[%lu], need_draw[%d], drawing[%d]", p_role->get_uid(), count, i);
				return errcode_enum::notice_unknown;
			}

			uint32_t reply_code = luckydraw_manager_t::doluckydraw(p_role, proto::common::luckydraw_type_senior, level, draw_id, reply);
			if (reply_code != errcode_enum::error_ok)
			{
				log_error("role[%lu], draw_error need_draw[%d], drawing[%d]", p_role->get_uid(), count, i);
				return reply_code;
			}
		}
		else //超100次需要打乱一次抽卡seq
		{ 
			//uint32_t index = (senior_count - 1) % s_senior_count;	
			uint32_t index = (senior_count - 1) % p_luckydraw->get_luckydraw_pool_size( draw_type );
			if (index == 0)
			{
				p_luckydraw->make_seq(draw_type);
			}
	
			uint32_t level = p_luckydraw->get_senior_level(draw_type, index);
			if (0 == level)
			{
				log_error("0 == level  role[%lu], need_draw[%d], drawing[%d]", p_role->get_uid(), count, i);
				return errcode_enum::notice_unknown;
			}
			uint32_t card_index = general_info_manager_t::get_card_index();
			if (card_index >= p_conf_type->card().size())
			{
				log_error("LuckydrawType[%u] card_size error", proto::common::luckydraw_type_senior);
				return errcode_enum::notice_unknown;
			}
			uint32_t reply_code = luckydraw_manager_t::doluckydraw(p_role, proto::common::luckydraw_type_senior, level, p_conf_type->card().at(card_index), reply);
			if (reply_code != errcode_enum::error_ok)
			{
				log_error("role[%lu], draw_error level[%d], card[%d]", p_role->get_uid(), level, p_conf_type->card().at(card_index));
				return reply_code;
			}
		}
		p_luckydraw->set_senior_count( draw_type, senior_count + 1);
		p_luckydraw->set_today_count(proto::common::luckydraw_type_senior, p_luckydraw->get_today_count(proto::common::luckydraw_type_senior) + 1);
	}
	p_luckydraw->save_self();
	rank_manager_t::update_recruit(p_role->get_uid(), count);
	return errcode_enum::error_ok;
}


uint32_t luckydraw_manager_t::luckydraw_general(role_ptr p_role, uint32_t count, proto::client::gc_luckydraw_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	auto p_luckydraw = p_role->get_luckydraw();
	if (NULL == p_luckydraw)
	{
		log_error("NULL == p_luckydraw");
		return errcode_enum::notice_role_null;
	}
	if (is_today_limit() && p_luckydraw->get_today_count(proto::common::luckydraw_type_general) + count > get_today_draw_limit_number())
	{
		log_error("p_role[%lu] luckydraw_general today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	if (is_today_limit() && p_luckydraw->get_today_count() + count > get_today_draw_total_limit_number())
	{
		log_error("p_role[%lu] luckydraw_general today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	LuckydrawType* p_conf = GET_CONF(LuckydrawType, proto::common::luckydraw_type_general);
	if (NULL == p_conf) {
		log_error("LuckydrawType[%u] not find", proto::common::luckydraw_type_general);
		return errcode_enum::notice_unknown;
	}
	uint32_t remain_time = general_info_manager_t::get_general_card_remain_time();
	if (0 == remain_time)
	{
		log_error("p_role[%lu] luckydraw_general closed", p_role->get_uid());
		return errcode_enum::notice_unknown;
	}
	uint32_t card_index = general_info_manager_t::get_general_card_index();
	if (0 == card_index || card_index > p_conf->card().size())
	{
		log_error("LuckydrawType[%u] card_size error", proto::common::luckydraw_type_general);
		return errcode_enum::notice_unknown;
	}
	auto it = s_luckydraw_pool.find(p_conf->card().at(card_index-1));
	if (it == s_luckydraw_pool.end())
	{
		log_error("s_luckydraw_pool error");
		return errcode_enum::notice_unknown;
	}
	const std::vector<Luckydraw*> luckydraw_vec = it->second;
	if (luckydraw_vec.empty())
	{
		log_error("s_luckydraw_pool error");
		return errcode_enum::notice_unknown;
	}
	uint32_t rand_max = 0;
	for (auto luckydraw_conf : luckydraw_vec)
	{
		if (NULL == luckydraw_conf)
		{
			continue;
		}
		rand_max += luckydraw_conf->prob();
	}
	if (0 == rand_max)
	{
		log_error("0 == rand_max");
		return errcode_enum::notice_unknown;
	}
	if (0 == count)
	{
		log_error("0 == count");
		return errcode_enum::notice_unknown;
	}
	if (count > get_normal_draw_limit_number())
	{
		count = get_normal_draw_limit_number();
	}

	if (money_manager_t::get_money(p_role, (proto::common::MONEY_TYPE)p_conf->money().first) < p_conf->money().second)
	{
		log_error("role[%lu] money not enough", p_role->get_uid());
		return errcode_enum::notice_gold_money_not_enough;
	}

	for (uint32_t i = 0; i < count; ++i)
	{
		if (!money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)p_conf->money().first, p_conf->money().second, log_enum::source_type_hero_luckydraw, 0, reply.mutable_rcd()))
		{
			count = i;
			break;
		}

		int32_t rand_val = random_util_t::randMin(0, rand_max);
		if (-1 == rand_val)
		{
			log_error("rand_max invalid");
			return errcode_enum::notice_unknown;
		}

		for (auto luckydraw_conf : luckydraw_vec)
		{
			if (NULL == luckydraw_conf)
			{
				continue;
			}
			uint32_t prob = luckydraw_conf->prob();
			if ((uint32_t)rand_val < prob)
			{
				uint32_t reply_code = errcode_enum::error_ok;
				if (!luckydraw_award(p_role, proto::common::luckydraw_type_general, luckydraw_conf, reply, reply_code))
				{
					return reply_code;
				}
				else
				{
					p_luckydraw->set_today_count(proto::common::luckydraw_type_general, p_luckydraw->get_today_count(proto::common::luckydraw_type_general) + 1);
				}
				break;
			}
			else
			{
				rand_val -= prob;
			}
		}
	}
	p_role->get_personal_info(reply.mutable_rcd()->mutable_per_info());
	p_luckydraw->save_self();
	rank_manager_t::update_recruit(p_role->get_uid(), count);
	return errcode_enum::error_ok;
}


uint32_t luckydraw_manager_t::luckydraw_gold(role_ptr p_role, proto::client::gc_luckydraw_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	uint32_t draw_type = proto::common::luckydraw_type_gold;

	auto p_luckydraw = p_role->get_luckydraw();
	if (NULL == p_luckydraw)
	{
		log_error("NULL == p_luckydraw");
		return errcode_enum::notice_role_null;
	}
	if (is_today_limit() && p_luckydraw->get_today_count(proto::common::luckydraw_type_gold) + 10 > get_today_draw_limit_number())
	{
		log_error("p_role[%lu] luckydraw_type_gold today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	if (is_today_limit() && p_luckydraw->get_today_count() + 10 > get_today_draw_total_limit_number())
	{
		log_error("p_role[%lu] luckydraw_type_gold today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	//目前没有反射表
	LuckydrawType* p_conf_type = GET_CONF(LuckydrawType, proto::common::luckydraw_type_gold);
	if (NULL == p_conf_type) {
		log_error("LuckydrawType[%u] not find", proto::common::luckydraw_type_gold);
		return errcode_enum::notice_unknown;
	}

	//消耗资源	
	if (errcode_enum::error_ok != item_manager_t::remove_item(p_role, p_conf_type->material().first, p_conf_type->material().second*10, log_enum::source_type_hero_luckydraw, 0, reply.mutable_rcd()))
	{
		log_error("role[%lu] item not enough", p_role->get_uid());
		return errcode_enum::notice_item_not_enough;
	}
	p_luckydraw->use_lucky();
	for (uint32_t i = 0; i < 11; ++i)
	{
		uint32_t gold_count = p_luckydraw->get_senior_count(draw_type);
		uint32_t index = gold_count % p_luckydraw->get_luckydraw_pool_size(draw_type);
		if (index == 0)
		{
			p_luckydraw->make_seq(draw_type);
		}

		uint32_t level = p_luckydraw->get_senior_level(draw_type, index);
		if (0 == level)
		{
			log_error("luckydraw_type_gold 0 == level  role[%lu]", p_role->get_uid());
			return errcode_enum::notice_unknown;
		}
		uint32_t card_index = general_info_manager_t::get_card_index();
		if (card_index >= p_conf_type->card().size())
		{
			log_error("LuckydrawType[%u] card_size error", proto::common::luckydraw_type_gold);
			return errcode_enum::notice_unknown;
		}
		uint32_t reply_code = luckydraw_manager_t::doluckydraw(p_role, proto::common::luckydraw_type_gold, level, p_conf_type->card().at(card_index), reply);
		if (reply_code != errcode_enum::error_ok)
		{
			log_error("role[%lu], draw_error level[%d], card[%d]", p_role->get_uid(), level, p_conf_type->card().at(card_index));
			return reply_code;
		}
		p_luckydraw->set_senior_count(draw_type, gold_count + 1);
		p_luckydraw->set_today_count(proto::common::luckydraw_type_gold, p_luckydraw->get_today_count(proto::common::luckydraw_type_gold) + 1);
	}
	p_luckydraw->save_self();
	rank_manager_t::update_recruit(p_role->get_uid(), 11);
	return errcode_enum::error_ok;
}

bool luckydraw_manager_t::luckydraw_award(role_ptr p_role, uint32_t type, Luckydraw* p_conf, proto::client::gc_luckydraw_reply& reply, uint32_t& reply_code)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		reply_code = errcode_enum::notice_unknown;
		return false;
	}
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf");
		reply_code = errcode_enum::notice_config_error;
		return false;
	}
	
	uint32_t award_id = p_conf->award_id();
	if (p_conf->award_type() == 1)
	{
		auto p_hero_conf = GET_CONF(Hero, award_id);
		if (NULL == p_hero_conf)
		{
			log_error("NULL == p_hero_conf[%d]", award_id);
			return false;
		}
		if (type == proto::common::luckydraw_type_senior)
		{
			log_warn("luckydraw_type_senior award[%d] rarity[%d]", award_id, p_hero_conf->rarity());
			//抽到稀有度为史诗以上的武将扣除气运值
			if (p_hero_conf->rarity() >= 3)
			{
				luckydraw_ptr p_luckydraw = p_role->get_luckydraw();
				if (NULL != p_luckydraw)
				{
					p_luckydraw->use_lucky();
				}
			}
			else
			{
				LuckydrawType* p_type_conf = GET_CONF(LuckydrawType, proto::common::luckydraw_type_senior);
				if (NULL == p_type_conf) {
					log_error("LuckydrawType[%u] not find", proto::common::luckydraw_type_general);
					return errcode_enum::notice_unknown;
				}
				luckydraw_ptr p_luckydraw = p_role->get_luckydraw();
				if (NULL != p_luckydraw)
				{
					p_luckydraw->add_lucky(p_type_conf->add_lucky());
				}
			}
		}
		auto rarity_conf = GET_CONF(Comprehensive, comprehensive_common::recruiting_announcement_1);
		uint32_t rarity = GET_COMPREHENSIVE_VALUE_1(rarity_conf);
		hero_ptr p_hero = hero_manager_t::get_hero(p_role, award_id);
		if ( NULL == p_hero && p_hero_conf->rarity() >= rarity)
		{
			///抽卡抽到稀有以上的武将广播
			SysNoticeTable* p_notice = GET_CONF(SysNoticeTable, award_id);
			if (NULL != p_notice && !p_notice->text().empty())
			{
				BROADCAST_SYS_NOTICE(award_id, p_role->get_uid(), 0, 0, p_role->get_name().c_str());
			}
		}
		hero_manager_t::add_hero(p_role, award_id, log_enum::source_type_hero_luckydraw, reply.mutable_rcd(), 1, 0, true, reply.add_award_list());
	}
	else if (p_conf->award_type() == 2)
	{
		/*if (!drop_manager_t::drop(p_role, award_id, log_enum::source_type_hero_luckydraw, proto::common::drop_sys_type_hero_luckydraw, 0, reply.mutable_rcd()))
		{
			log_error("role[%lu] luckydraw_award drop fail drop_id[%d]",p_role->get_uid(),award_id);
			reply_code = errcode_enum::notice_unknown;
			return false;
		}*/
		if (item_manager_t::check_add_item(p_role, award_id, p_conf->count(), proto::common::package_type_main))
		{
			item_manager_t::add_item(p_role, award_id, p_conf->count(), log_enum::source_type_hero_luckydraw, 0, reply.mutable_rcd());
		}
		else
		{
			std::map<uint32_t, uint32_t> mail_item_info;
			mail_item_info.insert(std::make_pair(award_id, p_conf->count()));
			mail_manager_t::send_mail(p_role->get_uid(), proto::common::MAIL_TYPE_SYS, mailcode_enum::sys_mail_collect,
				mail_item_info, proto::common::MAIL_GET_TYPE_SYS);
		}

		proto::common::luckydraw_award *la = reply.add_award_list();
		la->set_hero_tid(0);
		la->set_item_tid(award_id);
		la->set_item_count(p_conf->count());
	}
	return true;
}

uint32_t luckydraw_manager_t::get_normal_draw_limit_number()
{
	auto normal_draw_limit_number = GET_CONF(Comprehensive, comprehensive_common::normal_draw_limit_number);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(normal_draw_limit_number);
	return val;
}


uint32_t luckydraw_manager_t::get_today_draw_limit_number()
{
	auto today_draw_limit_number = GET_CONF(Comprehensive, comprehensive_common::draw_card_limit_time);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(today_draw_limit_number);
	return val;
}


uint32_t luckydraw_manager_t::get_today_draw_total_limit_number()
{
	auto today_draw_limit_number = GET_CONF(Comprehensive, comprehensive_common::draw_card_limit_times);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(today_draw_limit_number);
	return val;
}

bool luckydraw_manager_t::is_today_limit()
{
	auto is_today_limit = GET_CONF(Comprehensive, comprehensive_common::draw_card_limit_switch);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(is_today_limit);
	return val > 0;
}


uint32_t luckydraw_manager_t::get_card_id(uint32_t type)
{
	if (type == proto::common::luckydraw_type_wish)
	{
		return 0;
	}
	LuckydrawType* p_conf = GET_CONF(LuckydrawType, type);
	if (NULL == p_conf) {
		log_error("LuckydrawType[%u] not find", type);
		return 0;
	}
	uint32_t card_index = 0;
	switch (type)
	{
	case proto::common::luckydraw_type_junior:
		card_index = general_info_manager_t::get_card_index();
		break;
	case proto::common::luckydraw_type_senior:
		card_index = general_info_manager_t::get_card_index();
		break;
	case proto::common::luckydraw_type_general:
		card_index = general_info_manager_t::get_general_card_index();
		break;
	}
	if (card_index >= p_conf->card().size())
	{
		log_error("LuckydrawType[%u] card_size error", proto::common::luckydraw_type_junior);
		return 0;
	}
	if (type == proto::common::luckydraw_type_general && card_index > 0)
	{
		card_index -= 1;
	}
	return p_conf->card().at(card_index);
}

uint32_t luckydraw_manager_t::get_first_draw_id()
{
	auto first_draw_id = GET_CONF(Comprehensive, comprehensive_common::first_draw_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(first_draw_id);
	return val;
}

uint32_t luckydraw_manager_t::get_senior_yuanbao_consume()
{
	auto senior_yuanbao_consume = GET_CONF(Comprehensive, comprehensive_common::senior_draw_consume);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(senior_yuanbao_consume);
	return val;
}

uint32_t luckydraw_manager_t::doluckydraw(role_ptr p_role, uint32_t type, uint32_t level, uint32_t draw_type, proto::client::gc_luckydraw_reply& reply )
{
	if (NULL == p_role)
	{
		log_error("p_role is NULL");
		return errcode_enum::notice_unknown;
	}

	auto it = s_luckydraw_pool.find(draw_type);
	if (it == s_luckydraw_pool.end())
	{
		log_error("s_luckydraw_pool error");
		return errcode_enum::notice_unknown;
	}
	const std::vector<Luckydraw*> luckydraw_vec = it->second;
	if (luckydraw_vec.empty())
	{
		log_error("s_luckydraw_pool error");
		return errcode_enum::notice_unknown;
	}
	uint32_t rand_max = 0;
	for (auto luckydraw_conf : luckydraw_vec)
	{
		if (NULL == luckydraw_conf)
		{
			continue;
		}
		if (luckydraw_conf->level() != level)
		{
			continue;
		}
		rand_max += luckydraw_conf->prob();
	}
	if (0 == rand_max)
	{
		log_error("0 == rand_max");
		return errcode_enum::notice_unknown;
	}

	int32_t rand_val = random_util_t::randMin(0, rand_max);
	if (-1 == rand_val)
	{
		log_error("rand_max invalid");
		return errcode_enum::notice_unknown;
	}

	for (auto luckydraw_conf : luckydraw_vec)
	{
		if (NULL == luckydraw_conf)
		{
			continue;
		}
		if (luckydraw_conf->level() != level)
		{
			continue;
		}
		uint32_t prob = luckydraw_conf->prob();
		if ((uint32_t)rand_val < prob)
		{
			uint32_t reply_code = errcode_enum::error_ok;
			if (!luckydraw_award(p_role, type, luckydraw_conf, reply, reply_code))
				return reply_code;

			break;
		}
		else
		{
			rand_val -= prob;
		}
	}
	return errcode_enum::error_ok;
}

uint32_t luckydraw_manager_t::buy_wish_summon(role_ptr p_role, uint32_t summon_id, proto::client::gc_buy_wish_summon_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role ");
		return errcode_enum::notice_unknown;
	}

	auto p_luckydraw = p_role->get_luckydraw();
	if (NULL == p_luckydraw)
	{
		log_error("NULL == p_luckydraw");
		return errcode_enum::notice_role_null;
	}

	if (is_today_limit() && p_luckydraw->get_today_count(proto::common::luckydraw_type_wish) >= get_today_draw_limit_number())
	{
		log_error("p_role[%lu] luckydraw_wish today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	if (is_today_limit() && p_luckydraw->get_today_count() >= get_today_draw_total_limit_number())
	{
		log_error("p_role[%lu] luckydraw_wish today_count limit", p_role->get_uid());
		return errcode_enum::draw_card_limit_error;
	}
	uint32_t draw_type = proto::common::luckydraw_type_wish;
	LuckydrawType* p_conf_type = GET_CONF(LuckydrawType, draw_type);
	if (NULL == p_conf_type) {
		log_error("LuckydrawWish[%u] not find", draw_type);
		return errcode_enum::notice_unknown;
	}

	//消耗资源	
	proto::client::gc_luckydraw_reply reply_out;
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE(p_conf_type->money().first), p_conf_type->money().second, log_enum::source_type_hero_buy_luckydraw, 0, true, reply_out.mutable_rcd()))
	{
		log_error("role[%lu] money not enough", p_role->get_uid());
		return errcode_enum::notice_gold_money_not_enough;
	}

	uint32_t senior_count = p_luckydraw->get_senior_count(draw_type);

	//抽卡
	uint32_t index = (senior_count - 1) % p_luckydraw->get_luckydraw_pool_size(draw_type);
	if (index == 0)
	{
		p_luckydraw->make_seq(draw_type);
	}

	uint32_t level = p_luckydraw->get_senior_level(draw_type, index);
	if (0 == level)
	{
		log_error("0 == level  role[%lu], summon_id[%d]", p_role->get_uid(), summon_id);
		return errcode_enum::notice_unknown;
	}

	uint32_t reply_code = luckydraw_manager_t::doluckydraw(p_role, proto::common::luckydraw_type_wish, level, summon_id, reply_out);
	if (reply_code != errcode_enum::error_ok)
	{
		log_error("role[%lu], draw_error summon_id[%d]", p_role->get_uid(), summon_id);
		return reply_code;
	}

	reply.mutable_rcd()->CopyFrom(reply_out.rcd());
	reply.mutable_award_list()->CopyFrom(reply_out.award_list());

	p_luckydraw->set_senior_count(draw_type, senior_count + 1);
	p_luckydraw->set_today_count(proto::common::luckydraw_type_wish, p_luckydraw->get_today_count(proto::common::luckydraw_type_wish) + 1);
	p_luckydraw->save_self();

	//通知center
	proto::server::ge_buy_wish_summon_reply msg;
	msg.set_summon_id(summon_id);
	msg.set_reply_code(errcode_enum::error_ok);
	env::server->send_msg_to_center(op_cmd::ge_buy_wish_summon_reply, p_role->get_uid(), msg);
	rank_manager_t::update_recruit(p_role->get_uid(), 1);
	return errcode_enum::error_ok;
}

uint32_t luckydraw_manager_t::open_king_wish_summon(role_ptr p_role, uint32_t summon_id)
{
	uint32_t err_code = errcode_enum::error_ok;
	proto::server::ge_king_open_wish_summon_reply msg;
	do {
		if (NULL == p_role)
		{
			log_error("NULL == p_role ");
			err_code = errcode_enum::notice_unknown;
			break;
		}

		LuckydrawWish* p_conf = GET_CONF(LuckydrawWish, summon_id);
		if (NULL == p_conf) {
			log_error("LuckydrawWish[%u] not find", summon_id);
			err_code = errcode_enum::notice_unknown;
			break;
		}

		//扣费
		if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE(p_conf->open_resource().first), p_conf->open_resource().second, log_enum::source_type_hero_open_luckydraw, 0, true, msg.mutable_rcd()))
		{
			log_error("role[%lu] money not enough", p_role->get_uid());
			err_code = errcode_enum::notice_gold_money_not_enough;
			break;
		}
	} while (false);

	proto::common::role_change_data* p_rcd = msg.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}

	//通知center
	msg.set_summon_id(summon_id);
	msg.set_reply_code(err_code);
	env::server->send_msg_to_center(op_cmd::ge_king_open_wish_summon_reply, p_role->get_uid(), msg);

	return errcode_enum::error_ok;
}


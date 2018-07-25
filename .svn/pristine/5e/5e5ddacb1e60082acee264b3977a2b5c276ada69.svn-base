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
#include "tblh/Hero.tbls.h"
#include "role/role.hpp"

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
		return errcode_enum::notice_unknown;
	}

	if (type == proto::common::luckydraw_type_junior)
	{
		return luckydraw_junior(p_role, count, reply);
	}
	else if(type == proto::common::luckydraw_type_senior)
	{
		return luckydraw_senior(p_role, count , reply);
	}
	return errcode_enum::notice_unknown;
}

uint32_t luckydraw_manager_t::luckydraw_junior(role_ptr p_role, uint32_t count, proto::client::gc_luckydraw_reply& reply)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}

	auto it = s_luckydraw_pool.find(proto::common::luckydraw_type_junior);
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

	LuckydrawWish* p_conf = GET_CONF(LuckydrawWish, 1);
	if (NULL == p_conf) {
		log_error("LuckydrawWish[%u] not find", 1);
		return errcode_enum::notice_unknown;
	}

	if (!item_manager_t::has_item(p_role, p_conf->item_resource.id, 1))
	{
		log_error("role[%lu] item not enough", p_role->get_uid());
		return errcode_enum::notice_item_not_enough;
	}

	for (uint32_t i = 0; i < count; ++i)
	{
		if (errcode_enum::error_ok != item_manager_t::remove_item(p_role, p_conf->item_resource.id, p_conf->item_resource.num, log_enum::source_type_hero_luckydraw, 0, reply.mutable_rcd()))
		{
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
				if (!luckydraw_award(p_role, luckydraw_conf, reply, reply_code))
					return reply_code;
				break;
			}
			else
			{
				rand_val -= prob;
			}
		}
	}
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

	if (count > get_normal_draw_limit_number())
	{
		count = get_normal_draw_limit_number();
	}

	//目前没有反射表
	LuckydrawWish* p_conf_wish = GET_CONF(LuckydrawWish, 2);
	if (NULL == p_conf_wish) {
		log_error("LuckydrawWish[%u] not find", 2);
		return errcode_enum::notice_unknown;
	}

	//消耗资源	
	if (p_conf_wish->cost_resource.id != 0
		&& !money_manager_t::use_money(p_role, proto::common::MONEY_TYPE(p_conf_wish->cost_resource.id), p_conf_wish->cost_resource.num * count, log_enum::source_type_hero_luckydraw, 0, true, reply.mutable_rcd())
		)
	{
		log_error("role[%lu] money[%d]not enough", p_role->get_uid(), p_conf_wish->cost_resource.id);
		return errcode_enum::notice_gold_money_not_enough;
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
			if (!luckydraw_award(p_role, p_conf, reply, reply_code))
			{
				log_error("role[%lu], draw_error need_draw[%d], drawing[%d]", p_role->get_uid(), count, i);
				return reply_code;
			}
		}
		else if (senior_count <= 10) //1-10的特殊抽卡逻辑
		{
			uint32_t level = p_luckydraw->get_senior_level(proto::common::luckydraw_type_special_10, senior_count-1);
			if (0 == level)
			{
				log_error("0 == level  role[%lu], need_draw[%d], drawing[%d]", p_role->get_uid(), count, i);
				return errcode_enum::notice_unknown;
			}

			auto p_first_10_conf = GET_CONF(Comprehensive, comprehensive_common::first_10continuous_card_id);
			if (NULL == p_first_10_conf)
			{
				log_error("role[%lu], first_10_draw draw_id is NULL ", p_role->get_uid());
				return errcode_enum::notice_unknown;
			}
			uint32_t draw_id = GET_COMPREHENSIVE_VALUE_1(p_first_10_conf);
			
			uint32_t reply_code = luckydraw_manager_t::doluckydraw(p_role, level, draw_id, reply);
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
	
			uint32_t reply_code = luckydraw_manager_t::doluckydraw(p_role, level, draw_type, reply);
			if (reply_code != errcode_enum::error_ok)
			{
				log_error("role[%lu], draw_error need_draw[%d], drawing[%d]", p_role->get_uid(), count, i);
				return reply_code;
			}
		}
		p_luckydraw->set_senior_count( draw_type, senior_count + 1);
		p_luckydraw->save_self();
	}
	return errcode_enum::error_ok;
}


bool luckydraw_manager_t::luckydraw_award(role_ptr p_role, Luckydraw* p_conf, proto::client::gc_luckydraw_reply& reply,uint32_t& reply_code)
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
		hero_ptr p_hero = hero_manager_t::get_hero(p_role, award_id);
		if ( NULL == p_hero && p_hero_conf->star() >= 3 )
		{
			std::vector<std::string> param_str_list;
			param_str_list.push_back(p_role->get_name());
			param_str_list.push_back(common::string_util_t::uint32_to_string(award_id));
			sys_notice_manager_t::sys_notice_notify(p_role->get_uid(), proto::common::sys_notify_type_draw_hero, 0, param_str_list);

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

uint32_t luckydraw_manager_t::get_first_draw_id()
{
	auto first_draw_id = GET_CONF(Comprehensive, comprehensive_common::first_draw_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(first_draw_id);
	return val;
}

uint32_t luckydraw_manager_t::get_junior_item_id()
{
	auto normal_draw_itemid = GET_CONF(Comprehensive, comprehensive_common::normal_draw_itemid);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(normal_draw_itemid);
	return val;
}

uint32_t luckydraw_manager_t::get_senior_yuanbao_consume()
{
	auto senior_yuanbao_consume = GET_CONF(Comprehensive, comprehensive_common::senior_draw_consume);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(senior_yuanbao_consume);
	return val;
}

uint32_t luckydraw_manager_t::doluckydraw(role_ptr p_role, uint32_t level, uint32_t draw_type, proto::client::gc_luckydraw_reply& reply )
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
			if (!luckydraw_award(p_role, luckydraw_conf, reply, reply_code))
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

	LuckydrawWish* p_conf = GET_CONF(LuckydrawWish, summon_id);
	if (NULL == p_conf) {
		log_error("LuckydrawWish[%u] not find", summon_id);
		return errcode_enum::notice_unknown;
	}
	
	//消耗资源	
	proto::client::gc_luckydraw_reply reply_out;
	if (p_conf->cost_resource.id != 0 
		&& !money_manager_t::use_money(p_role, proto::common::MONEY_TYPE(p_conf->cost_resource.id), p_conf->cost_resource.num , log_enum::source_type_hero_buy_luckydraw, 0, true, reply_out.mutable_rcd())
		)
	{
		log_error("role[%lu] money[%d]not enough", p_role->get_uid(), p_conf->cost_resource.id );
		return errcode_enum::notice_gold_money_not_enough;
	}

	auto p_luckydraw = p_role->get_luckydraw();
	if (NULL == p_luckydraw)
	{
		log_error("NULL == p_luckydraw");
		return errcode_enum::notice_role_null;
	}

	uint32_t draw_type = p_conf->type();
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

	uint32_t reply_code = luckydraw_manager_t::doluckydraw(p_role, level, draw_type, reply_out);
	if (reply_code != errcode_enum::error_ok)
	{
		log_error("role[%lu], draw_error summon_id[%d]", p_role->get_uid(), summon_id);
		return reply_code;
	}

	reply.mutable_rcd()->CopyFrom(reply_out.rcd());
	reply.mutable_award_list()->CopyFrom(reply_out.award_list());

	p_luckydraw->set_senior_count(draw_type, senior_count + 1);
	p_luckydraw->save_self();

	//通知center
	proto::server::ge_buy_wish_summon_reply msg;
	msg.set_summon_id(summon_id);
	msg.set_reply_code(errcode_enum::error_ok);
	env::server->send_msg_to_center(op_cmd::ge_buy_wish_summon_reply, p_role->get_uid(), msg);
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
		if (p_conf->open_resource.id != 0
			&& !money_manager_t::use_money(p_role, proto::common::MONEY_TYPE(p_conf->open_resource.id), p_conf->open_resource.num, log_enum::source_type_hero_open_luckydraw, 0, true, msg.mutable_rcd())
			)
		{
			log_error("role[%lu] money[%d]not enough", p_role->get_uid(), p_conf->open_resource.id);
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


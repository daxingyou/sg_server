#include "redbag_manager.hpp"

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "user/global_user_manager.hpp"
#include "client.pb.h"
#include "global_id.hpp"
#include "common/redis_client.hpp"
#include "config_mgr.h"
#include "utility.hpp"
#include "tblh/errcode_enum.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "family/family_manager.hpp"
#include "mail/mail_manager.hpp"
#include "tblh/mailcode_enum.hpp"
#include "log/log_wrapper.hpp"

USING_NS_COMMON;

redbag_map redbag_manager_t::m_redbag_map;

uint64_t redbag_manager_t::get_new_redbag_uid()
{
	return g_id_allocator.gen_new_id(ID_ALLOCATOR_TYPE_REDBAG);
}

void redbag_manager_t::load_data()
{
	//读取所有红包
	std::vector<uint64_t> redbag_uids;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>("redbag", redbag_uids);

	uint32_t now_time = time_util_t::now_time();
	for (auto redbag_uid : redbag_uids)
	{
		uint64_t sender_uid = redis_client_t::get_uint64("redbag", "sender_uid", redbag_uid);
		uint32_t send_time = redis_client_t::get_uint32("redbag", "send_time", redbag_uid);
		uint32_t rest_count = redis_client_t::get_uint32("redbag", "rest_count", redbag_uid);
		uint32_t rest_money = redis_client_t::get_uint32("redbag", "rest_money", redbag_uid);
		uint32_t total_count = redis_client_t::get_uint32("redbag", "send_count", redbag_uid);
		uint32_t total_money = redis_client_t::get_uint32("redbag", "money", redbag_uid);
		const std::string& sender_name = redis_client_t::get_string("redbag", "sender_name", redbag_uid);
		if (send_time + redbag_t::get_expired_time() >= now_time)
		{
			if (rest_count > 0)
			{
				//发送邮件
				redis_client_t::set_uint32("redbag", "rest_count", redbag_uid, 0);
				redis_client_t::set_uint32("redbag", "rest_money", redbag_uid, 0);

				std::map<uint32_t, uint32_t> items_id_count;
				items_id_count[redbag_t::get_back_item()] = rest_money;
				mail_manager_t::send_mail(sender_uid, proto::common::MAIL_TYPE_SYS, mailcode_enum::sys_mail_red_envelope_1, items_id_count);
				REDBAG_LOG("redbag[%lu] expired back_money[%d] sender[%lu]", redbag_uid, rest_money, sender_uid);
				log_wrapper_t::send_redbag_log(sender_uid, sender_name, redbag_uid, total_count, total_money, 0,
					rest_count, rest_money, log_enum::source_type_role_luck_gift_return_surplus);
			}
			else
			{
				continue;
			}
			
		}
		
		redbag_ptr p_redbag = redbag_ptr(new redbag_t);
		p_redbag->set_uid(redbag_uid);
		p_redbag->set_sender_uid(sender_uid);
		p_redbag->set_sender_name(sender_name);
		p_redbag->set_count(total_count);
		p_redbag->set_money(total_money);
		p_redbag->set_rest_count(rest_count);
		p_redbag->set_rest_money(rest_money);
		p_redbag->set_send_time(send_time);
		p_redbag->set_finish_time(redis_client_t::get_uint32("redbag", "finish_time", redbag_uid));
		proto::common::draw_data dd;
		redis_client_t::get_protobuf("redbag", "draw_data", redbag_uid, dd);
		p_redbag->load_draw_data(dd);
		add_redbag(p_redbag);
	}
}

void redbag_manager_t::redbag_timeout(uint64_t redbag_uid, const boost::system::error_code& ec)
{
	//REDBAG_LOG("redbag[%lu] timeout", redbag_uid);
	if (ec == boost::asio::error::operation_aborted)
	{
		return;
	}
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}
	redbag_ptr p_redbag = get_redbag_by_uid(redbag_uid);
	if (NULL == p_redbag)
	{
		log_error("NULL == p_redbag[%lu]", redbag_uid);
		return;
	}
	if (p_redbag->get_rest_money() > 0)
	{
		std::map<uint32_t, uint32_t> items_id_count;
		items_id_count[redbag_t::get_back_item()] = p_redbag->get_rest_money();
		mail_manager_t::send_mail(p_redbag->get_sender_uid(), proto::common::MAIL_TYPE_SYS, mailcode_enum::sys_mail_red_envelope_1, items_id_count);
		REDBAG_LOG("redbag[%lu] expired back_money[%d] sender[%lu]", redbag_uid, p_redbag->get_rest_money(), p_redbag->get_sender_uid());
		log_wrapper_t::send_redbag_log(p_redbag->get_sender_uid(), p_redbag->get_sender_name(), redbag_uid, p_redbag->get_count(), 
			p_redbag->get_money(), 0, p_redbag->get_rest_count(), p_redbag->get_rest_money(), log_enum::source_type_role_luck_gift_return_surplus);
	}
//	p_redbag->save_self();
	del_redbag(p_redbag);
}

void redbag_manager_t::add_redbag(redbag_ptr p_redbag)
{
	if (NULL == p_redbag)
	{
		log_error("NULL == p_redbag");
		return;
	}
	m_redbag_map.insert(std::make_pair(p_redbag->get_uid(), p_redbag));
	p_redbag->start_timer();
}

void redbag_manager_t::del_redbag(redbag_ptr p_redbag)
{
	if (NULL == p_redbag)
	{
		log_error("NULL == p_redbag");
		return;
	}
}

redbag_ptr redbag_manager_t::get_redbag_by_uid(uint64_t redbag_uid)
{
	auto it = m_redbag_map.find(redbag_uid);
	if (it == m_redbag_map.end())
	{
		return redbag_ptr(NULL);
	}
	return it->second;
}

void redbag_manager_t::stop_timer()
{
	for (auto it : m_redbag_map)
	{
		redbag_ptr p_redbag = it.second;
		if (NULL != p_redbag)
		{
			p_redbag->stop_timer();
		}
	}
}

uint32_t redbag_manager_t::draw(uint64_t redbag_uid, global_user_ptr p_role, proto::common::redbag_data* p_data)
{
	redbag_ptr p_redbag = get_redbag_by_uid(redbag_uid);
	if (NULL == p_redbag)
	{
		log_error("NULL == p_redbag[%lu]", redbag_uid);
		return errcode_enum::notice_unknown;
	}
	uint32_t reply_code = p_redbag->draw(p_role, p_data);
	p_redbag->peek_data(p_data);
	return reply_code;
}

void redbag_manager_t::send(global_user_ptr p_role, uint32_t count, uint32_t money)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	family_ptr p_family = family_manager_t::get_family(p_role->get_family_id());
	if (NULL == p_family)
	{
		log_error("NULL == p_family[%lu]", p_role->get_family_id());
		return;
	}
	redbag_ptr p_redbag = redbag_ptr(new redbag_t);
	p_redbag->set_uid(get_new_redbag_uid());
	p_redbag->set_sender_uid(p_role->get_uid());
	p_redbag->set_sender_name(p_role->get_name());
	p_redbag->set_count(count);
	p_redbag->set_money(money);
	p_redbag->set_rest_count(count);
	p_redbag->set_rest_money(money);
	uint32_t now_time = time_util_t::now_time();
	p_redbag->set_send_time(now_time);
	p_redbag->set_finish_time(0);
	add_redbag(p_redbag);
	REDBAG_LOG("role[%lu] send redbag[%lu] count[%d] money[%d]", p_redbag->get_sender_uid(), p_redbag->get_uid(), count, money);
	log_wrapper_t::send_redbag_log(p_redbag->get_sender_uid(), p_redbag->get_sender_name(), p_redbag->get_uid(), p_redbag->get_count(),
		p_redbag->get_money(), 0, p_redbag->get_rest_count(), p_redbag->get_rest_money(), log_enum::source_type_role_luck_gift_send);

	proto::client::ec_redbag_notify ntf;
	ntf.set_redbag_uid(string_util_t::uint64_to_string(p_redbag->get_uid()));
	ntf.set_role_uid(string_util_t::uint64_to_string(p_role->get_uid()));
	ntf.set_role_name(p_role->get_name());
	ntf.set_role_level(p_role->get_level());
	ntf.set_plugin(p_role->get_hero_plugin());
	p_family->send_msg_to_family(op_cmd::ec_redbag_notify, ntf);
}

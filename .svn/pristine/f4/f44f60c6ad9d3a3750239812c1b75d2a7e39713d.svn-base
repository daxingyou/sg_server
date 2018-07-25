#include "redbag.hpp"
#include "log.hpp"
#include "utility.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "redbag_manager.hpp"
#include "common/redis_client.hpp"
#include "tblh/errcode_enum.hpp"
#include "user/global_user.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/comprehensive_common.hpp"
#include "config_mgr.h"
#include "log/log_wrapper.hpp"
USING_NS_COMMON;

void redbag_t::peek_data(proto::common::redbag_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_uid(string_util_t::uint64_to_string(m_uid));
	p_data->set_sender_uid(string_util_t::uint64_to_string(m_sender_uid));
	p_data->set_sender_name(m_sender_name);
	p_data->set_count(m_count);
	p_data->set_money(m_money);
	p_data->set_rest_count(m_rest_count);
	p_data->set_rest_money(m_rest_money);
	p_data->set_send_time(m_send_time);
	p_data->set_finish_time(m_finish_time);
	proto::common::draw_data* p_draw_data = p_data->mutable_draw_data();
	for (auto it : m_draw_data)
	{
		proto::common::draw_single * p_single = p_draw_data->add_draw_info();
		p_single->set_role_uid(string_util_t::uint64_to_string(it.first));
		p_single->set_role_name(it.second.second);
		p_single->set_money(it.second.first);
	}
}

void redbag_t::load_draw_data(const proto::common::draw_data& dd)
{
	for (int32_t i = 0; i < dd.draw_info_size(); ++i)
	{
		const proto::common::draw_single& single = dd.draw_info(i);
		uint64_t uid = string_util_t::string_to_uint64(single.role_uid());
		m_draw_data.insert(std::make_pair(uid, std::make_pair(single.money(), single.role_name())));
	}
}

uint32_t redbag_t::get_expired_time()
{
	auto expired_time = GET_CONF(Comprehensive, comprehensive_common::red_envelope_effective_time);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(expired_time);
	return val * 3600;
}

uint32_t redbag_t::get_draw_min()
{
	auto draw_min = GET_CONF(Comprehensive, comprehensive_common::red_envelope_min);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(draw_min);
	return val;
}

uint32_t redbag_t::get_draw_max()
{
	auto draw_max = GET_CONF(Comprehensive, comprehensive_common::red_envelope_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(draw_max);
	return val;
}

uint32_t redbag_t::get_back_item()
{
	auto back_item = GET_CONF(Comprehensive, comprehensive_common::red_envelope_currency);
	uint32_t val = GET_COMPREHENSIVE_VALUE_2(back_item);
	return val;
}

void redbag_t::start_timer()
{
	uint32_t now_time = common::time_util_t::now_time();
	if (now_time >= m_send_time + get_expired_time())
	{
		return;
	}
	if (NULL == m_timer)
	{
		m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_timer->cancel();
	m_timer->expires_from_now(boost::posix_time::seconds(m_send_time + get_expired_time() - now_time));
	m_timer->async_wait(boost::bind(&redbag_manager_t::redbag_timeout, get_uid(), boost::asio::placeholders::error));
}

void redbag_t::save_self()
{
	redis_client_t::set_uint64("redbag", "sender_uid", m_uid, m_sender_uid);
	redis_client_t::set_string("redbag", "sender_name", m_uid, m_sender_name);
	redis_client_t::set_uint32("redbag", "send_count", m_uid, m_count);
	redis_client_t::set_uint32("redbag", "money", m_uid, m_money);
	redis_client_t::set_uint32("redbag", "rest_count", m_uid, m_rest_count);
	redis_client_t::set_uint32("redbag", "rest_money", m_uid, m_rest_money);
	redis_client_t::set_uint32("redbag", "send_time", m_uid, m_send_time);
	redis_client_t::set_uint32("redbag", "finish_time", m_uid, m_finish_time);
	
	proto::common::draw_data draw_data;
	for (auto it : m_draw_data)
	{
		proto::common::draw_single * p_single = draw_data.add_draw_info();
		p_single->set_role_uid(string_util_t::uint64_to_string(it.first));
		p_single->set_role_name(it.second.second);
		p_single->set_money(it.second.first);
	}
	redis_client_t::set_protobuf("redbag", "draw_data", m_uid, draw_data);
}

void redbag_t::stop_timer()
{
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}
}

uint32_t redbag_t::draw(global_user_ptr p_role, proto::common::redbag_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (m_draw_data.find(p_role->get_uid()) != m_draw_data.end())
	{
		return errcode_enum::notice_luck_gift_has_grab;
	}
	if (0 == m_rest_count)
	{
		return errcode_enum::notice_luck_gift_not_exist;
	}
	uint32_t draw_money = 0;
	if (1 == m_rest_count)
	{
		//只有一个包了
		draw_money = m_rest_money;
	}
	else
	{
		//还有超过一个包 最大值最小值算法比较绕
		uint32_t draw_min = get_draw_min();
		
		if (m_rest_money < draw_min * m_rest_count)
		{
			log_error("redbag[%lu] rest_money[%d] rest_count[%d]", get_uid(), m_rest_money, m_rest_count);
			return errcode_enum::notice_unknown;
		}
		uint32_t draw_max = m_rest_money - draw_min * (m_rest_count - 1);
		if (m_rest_money > get_draw_max() * (m_rest_count - 1) )
		{
			draw_min = m_rest_money - get_draw_max() * (m_rest_count - 1);
		}
		if (draw_max > get_draw_max())
		{
			draw_max = get_draw_max();
		}
		draw_money = random_util_t::randBetween(draw_min, draw_max);
	}
	m_rest_money -= draw_money;
	m_rest_count -= 1;
	m_draw_data.insert(std::make_pair(p_role->get_uid(), std::make_pair(draw_money, p_role->get_name())));
	REDBAG_LOG("role[%lu] draw redbag[%lu] money[%d] reset_count[%d] rest_money[%d]", p_role->get_uid(), 
		get_uid(), draw_money, m_rest_count, m_rest_money);
	if (0 == m_rest_count)
	{
		//领完了，设置时间
		m_finish_time = time_util_t::now_time() - m_send_time;
		stop_timer();
	}
	save_self();
	log_wrapper_t::send_redbag_log(p_role->get_uid(), p_role->get_name(), get_uid(), get_count(), get_money(), 
		draw_money, get_rest_count(), get_rest_money(), log_enum::source_type_role_luck_gift_grab);

	proto::server::eg_redbag_draw_notify ntf;
	ntf.set_money(draw_money);
	p_role->send_msg_to_game(op_cmd::eg_redbag_draw_notify, ntf);
	return errcode_enum::error_ok;
}



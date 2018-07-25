#include "patrol_hire.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/errcode_enum.hpp"
#include "common/config_mgr.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/comprehensive_common.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "red_point/red_point_manager.hpp"
#include "role/role_manager.hpp"
#include "mail/mail_manager.hpp"
#include "tblh/mailcode_enum.hpp"
#include "tblh/MailTable.tbls.h"

patrol_hire_t::patrol_hire_t(uint64_t role_uid)
	:m_role_uid(role_uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::patrol_hire);
	m_over_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
}

void patrol_hire_t::del_timer()
{
	if (m_over_timer) {
		m_over_timer->cancel();
	}
}


void patrol_hire_t::on_over_timer_callback(uint64_t user_id, const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
	{
		return;
	} 
	else if (ec)
	{
		log_error("Timer error: %s", ec.message().c_str());
		return;
	}

	role_ptr p_role = role_manager_t::find_role(user_id);
	if (NULL == p_role) {
		log_error("user:[%lu] p_role is NULL", user_id);
		return;
	}

	patrol_hire_ptr p_patrol_hire_ptr = p_role->get_patrol_hire();
	if (p_patrol_hire_ptr) {
		p_patrol_hire_ptr->on_over_time();
	}
}

void patrol_hire_t::load_data(const proto::common::patrol_hire_data& data, uint32_t last_login_out)
{
	uint32_t curr_time = common::time_util_t::now_time();
	m_map_id = data.map_id();
	m_begin_time = data.begin_time() > curr_time ? curr_time : data.begin_time(); // 对于调时间的处理
	m_fast_patrol_count = data.fast_patrol_count();

	if (m_map_id != 0) {
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::patrol_max_time);
		uint32_t patrol_max_time = GET_COMPREHENSIVE_VALUE_1(p_conf);

		p_conf = GET_CONF(Comprehensive, comprehensive_common::patrol_mail_time);
		uint32_t patrol_mail_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
		if (patrol_mail_time == 0) {
			log_error("patrol_mail_time == 0");
			return;
		}

		if (last_login_out > m_begin_time + patrol_max_time) { // 下线时间已经大于最大时间
			return;
		}

		if (m_over_timer) { // 计算下次发邮件时间
			uint32_t next_mail_time = ((curr_time - m_begin_time) / patrol_mail_time + 1) * patrol_mail_time + m_begin_time;
			m_over_timer->expires_from_now(boost::posix_time::seconds(next_mail_time - curr_time));
			m_over_timer->async_wait(boost::bind(&patrol_hire_t::on_over_timer_callback, m_role_uid, boost::asio::placeholders::error));
		}

		if (last_login_out < m_begin_time) {
			return;
		}

		MailTable* p_conf_mail = GET_CONF(MailTable, common::mailcode_enum::partol_mail);
		if (!p_conf_mail) {
			log_error("mail[%u] not find", common::mailcode_enum::partol_mail);
			return;
		}

		uint32_t cur_count = (last_login_out - m_begin_time) / patrol_mail_time;
		uint32_t intervals_times = curr_time - m_begin_time > patrol_max_time ? patrol_max_time : curr_time - m_begin_time;
		uint32_t send_count = intervals_times / patrol_mail_time;

		std::map<uint32_t, uint32_t> item_map;
		for (uint32_t i = cur_count + 1; i <= send_count; ++i) {
			std::string content_text;
			StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf_mail->content()).c_str(), i * patrol_mail_time);

			mail_manager_t::send_mail(m_role_uid, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_conf_mail->from_name()), common::string_util_t::convert_to_utf8(p_conf_mail->title()), content_text,
				item_map);
		}
	}
}

void patrol_hire_t::peek_data(proto::common::patrol_hire_data* p_data)
{
	if (p_data) {
		p_data->set_map_id(m_map_id);
		p_data->set_begin_time(m_begin_time);
		p_data->set_fast_patrol_count(m_fast_patrol_count);
	}
}

void patrol_hire_t::save_self()
{
	proto::common::patrol_hire_data data;
	peek_data(&data);
	role_unify_save::add_task(m_role_uid, m_key, data);
}

void patrol_hire_t::set_map_id(uint32_t map_id)
{
	m_map_id = map_id;
	reset_begin_time();
}


void patrol_hire_t::reset_begin_time()
{
	m_begin_time = common::time_util_t::now_time();
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::patrol_mail_time);
	uint32_t patrol_mail_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (patrol_mail_time == 0) {
		log_error("patrol_mail_time == 0");
		return;
	}

	if (m_over_timer) { // 计算下次发邮件时间
		m_over_timer->expires_from_now(boost::posix_time::seconds(patrol_mail_time));
		m_over_timer->async_wait(boost::bind(&patrol_hire_t::on_over_timer_callback, m_role_uid, boost::asio::placeholders::error));
	}
	save_self();
}


void patrol_hire_t::add_fast_patrol_count()
{
	m_fast_patrol_count += 1;
	save_self();
}

void patrol_hire_t::one_day()
{
	m_fast_patrol_count = 0;
	save_self();
	PATROL_HIRE_LOG("role[%lu] one_day cal hire patrol", m_role_uid);
}

void patrol_hire_t::on_login()
{
	if (m_map_id != 0) {
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::patrol_max_time);
		uint32_t patrol_max_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
		if (common::time_util_t::now_time() >= m_begin_time + patrol_max_time) {
			red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE_PATROL_HIRE, 1);
		}
	}
}

void patrol_hire_t::on_over_time()
{
	if (m_map_id == 0) {
		log_error("m_map_id == 0");
		return;
	}

	// 发邮件
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::patrol_mail_time);
	uint32_t patrol_mail_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (patrol_mail_time == 0) {
		log_error("patrol_mail_time == 0");
		return;
	}

	MailTable* p_conf_mail = GET_CONF(MailTable, common::mailcode_enum::partol_mail);
	if (!p_conf_mail) {
		log_error("mail[%u] not find", common::mailcode_enum::partol_mail);
		return;
	}

	std::map<uint32_t, uint32_t> item_map;
	std::string content_text;
	StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf_mail->content()).c_str(), (common::time_util_t::now_time() - m_begin_time) / patrol_mail_time * patrol_mail_time);
	mail_manager_t::send_mail(m_role_uid, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_conf_mail->from_name()), common::string_util_t::convert_to_utf8(p_conf_mail->title()), content_text,
		item_map);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::patrol_max_time);
	uint32_t patrol_max_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	uint32_t curr_time = common::time_util_t::now_time();
	if (curr_time >= m_begin_time + patrol_max_time) {
		red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE_PATROL_HIRE, 1);
		PATROL_HIRE_LOG("role[%lu] patrol_hire max time", m_role_uid);
		return;
	}

	if (m_over_timer) { // 计算下次发邮件时间
		uint32_t next_mail_time = ((curr_time - m_begin_time) / patrol_mail_time + 1) * patrol_mail_time + m_begin_time;
		m_over_timer->expires_from_now(boost::posix_time::seconds(next_mail_time - curr_time));
		m_over_timer->async_wait(boost::bind(&patrol_hire_t::on_over_timer_callback, m_role_uid, boost::asio::placeholders::error));
	}
}


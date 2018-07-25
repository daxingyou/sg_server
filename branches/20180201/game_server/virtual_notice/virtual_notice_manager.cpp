#include "virtual_notice_manager.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "config_mgr.h"
#include "tblh/comprehensive_common.hpp"
#include "tblh/VirtualNoticeTable.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "StringTool.h"
#include "utility.hpp"
#include "name_manager.hpp"

boost::shared_ptr<boost::asio::deadline_timer> virtual_notice_manager_t::m_timer = NULL;

#define FORMAT_VIRTUAL_PARAM(notice_text, text, args...) StringTool::format(notice_text, common::string_util_t::convert_to_utf8(text).c_str(), ##args)

bool virtual_notice_manager_t::init()
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::virtual_notice_interval);
	uint32_t interval = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (0 == interval)
	{
		return true;
	}

	if (NULL == VirtualNoticeTableManager::getInstancePtr())
	{
		log_error("NULL == VirtualNoticeTableManager::getInstancePtr()");
		return false;
	}
	// 如果配置表个数是0不通知
	if (VirtualNoticeTableManager::getInstancePtr()->getSize() == 0)
	{
		return true;
	}

	// 定时器初始化
	if (NULL == env::server)
	{
		log_error("env::server null error");
		return false;
	}

	m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	if (NULL == m_timer)
	{
		log_error("m_timer null error");
		return false;
	}

	m_timer->expires_from_now(boost::posix_time::seconds(interval));
	m_timer->async_wait(boost::bind(&virtual_notice_manager_t::on_timer, boost::asio::placeholders::error));

	return true;
}

void virtual_notice_manager_t::close()
{
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}
}

void virtual_notice_manager_t::on_timer(const boost::system::error_code& ec)
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

	if (m_timer != NULL)
	{
 		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::virtual_notice_interval);
 		uint32_t interval = GET_COMPREHENSIVE_VALUE_1(p_conf);

		if (interval != 0) {
			m_timer->expires_from_now(boost::posix_time::seconds(interval));
			m_timer->async_wait(boost::bind(&virtual_notice_manager_t::on_timer, boost::asio::placeholders::error));

			broadcast_virtual_notice();
		}
	}
}

void virtual_notice_manager_t::broadcast_virtual_notice()
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::virtual_notice_min);
	uint32_t min_count = GET_COMPREHENSIVE_VALUE_1(p_conf);

	p_conf = GET_CONF(Comprehensive, comprehensive_common::virtual_notice_max);
	uint32_t max_count = GET_COMPREHENSIVE_VALUE_1(p_conf);

	int32_t count = random_util_t::randBetween(min_count, max_count);
	if (count == -1)
	{
		log_error("broadcast_virtual_notice rand from [%u] to [%u] error", min_count, max_count);
		return;
	}
	
	proto::client::gc_sys_notice_notify ntf;
	for (int32_t i = 0; i < count; ++i)
	{
		random_virtual_notice(ntf.add_notices());
	}

	if (NULL != env::server)
	{
		std::string cmd_data;
		network::wrap_msg(cmd_data, ntf);
		proto::server::ga_broadcast_notify msg_to_gate;
		msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
		msg_to_gate.set_data(cmd_data);

		env::server->send_msg_to_all_gate(op_cmd::ga_broadcast_notify, 0, msg_to_gate);
	}
}

void virtual_notice_manager_t::random_virtual_notice(proto::common::sys_notice_data* data)
{
	if (data == NULL)
	{
		log_error("data null error");
		return;
	}
	if (NULL == VirtualNoticeTableManager::getInstancePtr())
	{
		log_error("NULL == VirtualNoticeTableManager::getInstancePtr()");
		return;
	}
	uint32_t random_id = random_util_t::randBetween(1, VirtualNoticeTableManager::getInstancePtr()->getSize());
	VirtualNoticeTable* p_notice_conf = GET_CONF(VirtualNoticeTable, random_id);
	if (p_notice_conf == NULL)
	{
		log_error("VirtualNoticeTable[%u] null error", random_id);
		return;
	}

	std::string notice_text;

	switch (p_notice_conf->params())
	{
	case 0:
		{
			FORMAT_VIRTUAL_PARAM(notice_text, p_notice_conf->text());
		}
		break;
	case 1:
		{
			FORMAT_VIRTUAL_PARAM(notice_text, p_notice_conf->text(), name_manager_t::create_new_name().c_str());
		}
		break;
	case 2:
		{
			FORMAT_VIRTUAL_PARAM(notice_text, p_notice_conf->text(), name_manager_t::create_new_name().c_str(), name_manager_t::create_new_name().c_str());
		}
		break;
	case 3:
		{
			FORMAT_VIRTUAL_PARAM(notice_text, p_notice_conf->text(), name_manager_t::create_new_name().c_str(), name_manager_t::create_new_name().c_str(), name_manager_t::create_new_name().c_str());
		}
		break;
	default:
		log_error("VirtualNoticeTable[%u] param[%u] error", random_id, p_notice_conf->params());
		break;
	}

	data->set_type((proto::common::system_notice_type)p_notice_conf->notice_type());
	data->set_priority(p_notice_conf->priority());
	data->set_text(notice_text);
}

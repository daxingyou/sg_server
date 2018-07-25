#include "user_msg_checker.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"

user_msg_checker_t::msg_id_set user_msg_checker_t::m_masked_msg_gm;
user_msg_checker_t::msg_id_set user_msg_checker_t::m_masked_msg_config;

bool user_msg_checker_t::init()
{
	load_config();
	return true;
}

void user_msg_checker_t::add_masked_msg(uint16_t cmd)
{
	m_masked_msg_gm.insert(cmd);
	log_info("add masked %u", cmd);
}

void user_msg_checker_t::remove_masked_msg(uint16_t cmd)
{
	m_masked_msg_gm.erase(cmd);
	m_masked_msg_config.erase(cmd);

	log_info("remove masked %u", cmd);
}

bool user_msg_checker_t::check_masked_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr msg_buf, bool notify)
{
	bool ret = true;

	if (m_masked_msg_gm.find(cmd) != m_masked_msg_gm.end()
		|| m_masked_msg_config.find(cmd) != m_masked_msg_config.end())
	{
		ret = false;
	}

	if( !ret && notify ) // 返回客户端消息
	{
		proto::client::ac_masked_msg_notify notify;
		notify.add_cmds(cmd);
		env::server->send_msg_to_client(s, 0, op_cmd::ac_masked_msg_notify, notify);
	}

	return ret;
}

void user_msg_checker_t::load_config()
{
	m_masked_msg_config.clear();

	auto masked_conf = GET_CONF(Comprehensive, comprehensive_common::masked_msg);
	if (NULL != masked_conf && !masked_conf->parameter1().empty())
	{
        log_info("check client cmd: %s", masked_conf->parameter1().c_str());
		std::vector<uint32_t> cmds;
		string_util_t::ParseIds(masked_conf->parameter1(), cmds, ';');

		for (auto cmd : cmds)
		{
			m_masked_msg_config.insert(cmd);
		}
	}
}


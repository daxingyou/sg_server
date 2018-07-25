#include "sys_notice_manager.hpp"

#include "log.hpp"
#include "server.pb.h"
#include "client.pb.h"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "op_cmd.hpp"
#include "protobuf.hpp"
#include "user/global_user_manager.hpp"
#include "family/family_manager.hpp"


boost::shared_ptr<boost::asio::deadline_timer> sys_notice_manager_t::s_send_notice_timer;	//扫荡结束定时器
std::map<uint32_t, proto::gm_common::gm_notice_single> sys_notice_manager_t::s_notice_map;


void sys_notice_manager_t::init()
{
	s_send_notice_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	s_send_notice_timer->expires_from_now(boost::posix_time::seconds(10));
	s_send_notice_timer->async_wait(boost::bind(&sys_notice_manager_t::on_send_notice_callback, boost::asio::placeholders::error));
}

void sys_notice_manager_t::close_clear()
{
	if (s_send_notice_timer != NULL) {
		s_send_notice_timer->cancel();
	}
}

void sys_notice_manager_t::broadcast_system_notice(SysNoticeTable* p_table, uint64_t uid, uint64_t family_id, uint64_t country_id, const std::string& text)
{
	if (NULL == p_table)
	{
		log_error("SysNoticeTable null error");
		return;
	}

	proto::client::gc_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();

	if (NULL != data)
	{
		data->set_type((proto::common::system_notice_type)p_table->notice_type());
		data->set_priority(p_table->priority());
		data->set_text(text);

		if (uid != 0)
			data->set_role_id(common::string_util_t::uint64_to_string(uid));

		if (family_id != 0)
			data->set_family_id(common::string_util_t::uint64_to_string(family_id));

		if (country_id != 0)
			data->set_country_id(common::string_util_t::uint64_to_string(country_id));
	}

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	std::string cmd_data;
	network::wrap_msg(cmd_data, ntf);
	proto::server::ea_broadcast_notify msg_to_gate;
	msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
	msg_to_gate.set_data(cmd_data);

	env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
}

void sys_notice_manager_t::broadcast_system_notice(SysNoticeTable* p_table, const std::vector<uint64_t>& to_uids, uint64_t uid, uint64_t family_id, uint64_t country_id, const std::string& text)
{
	if (NULL == p_table)
	{
		log_error("SysNoticeTable null error");
		return;
	}

	if (to_uids.empty()) {
		return;
	}

	proto::client::gc_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();

	if (NULL != data)
	{
		data->set_type((proto::common::system_notice_type)p_table->notice_type());
		data->set_priority(p_table->priority());
		data->set_text(text);

		if (uid != 0)
			data->set_role_id(common::string_util_t::uint64_to_string(uid));

		if (family_id != 0)
			data->set_family_id(common::string_util_t::uint64_to_string(family_id));

		if (country_id != 0)
			data->set_country_id(common::string_util_t::uint64_to_string(country_id));
	}

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	std::string cmd_data;
	network::wrap_msg(cmd_data, ntf);
	proto::server::ea_broadcast_notify msg_to_gate;
	for (std::vector<uint64_t>::const_iterator citr = to_uids.begin();
		citr != to_uids.end(); ++citr) {
		msg_to_gate.add_role_list(*citr);
	}

	msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
	msg_to_gate.set_data(cmd_data);

	env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
}

//////////////////////////////////////////////////////////////////////////

void sys_notice_manager_t::broadcast_system_notice(SysNoticeTable* p_table, uint64_t uid, const std::string& text, uint32_t label)
{
	if (NULL == p_table)
	{
		log_error("SysNoticeTable null error");
		return;
	}

	proto::client::gc_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();

	if (NULL != data)
	{
		data->set_type((proto::common::system_notice_type)p_table->notice_type());
		data->set_priority(p_table->priority());
		data->set_text(text);
		data->set_label(label);

		if (uid != 0)
			data->set_role_id(common::string_util_t::uint64_to_string(uid));
	}

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	std::string cmd_data;
	network::wrap_msg(cmd_data, ntf);
	proto::server::ea_broadcast_notify msg_to_gate;
	msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
	msg_to_gate.set_data(cmd_data);

	env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
}

void sys_notice_manager_t::broadcast_system_notice(uint64_t uid, const std::string& text, uint32_t label)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return;
	}

	if (text.empty())
	{
		log_error("text empty error");
		return;
	}

	proto::client::ec_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();

	if (NULL != data)
	{
		data->set_type(proto::common::system_notice_type::SYS_NOTICE_CHANNEL);
		data->set_text(text);
		data->set_label(label);
		data->set_role_id(common::string_util_t::uint64_to_string(uid));		
	}

	p_user->send_msg_to_client(op_cmd::ec_sys_notice_notify, ntf);
}

void sys_notice_manager_t::broadcast_gm_system_notice(uint32_t type, const std::string &text)
{
	proto::client::gc_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();
	data->set_type(proto::common::system_notice_type(type) );
	data->set_text(text);
	data->set_label(0);

	std::string cmd_data;
	network::wrap_msg(cmd_data, ntf);
	proto::server::ea_broadcast_notify msg_to_gate;
	msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
	msg_to_gate.set_data(cmd_data);

	env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
}

bool sys_notice_manager_t::update_limit_count(const proto::common::sys_notity_single& single,bool is_from_game /* = true */)
{
	SysChannel* p_config = GET_CONF(SysChannel, hash_util_t::hash_to_uint32(single.type(), single.param()));
	if (NULL == p_config)
	{
		log_error("NULL == p_config,type[%d],param[%d]", single.type(), single.param());
		return false;
	}

	sys_notice_map::iterator iter = m_sys_notice_map.find(hash_util_t::hash_to_uint32(single.type(), single.param()));
	if (iter == m_sys_notice_map.end())
	{
		sys_notice_ptr p_sys_notice = sys_notice_ptr(new sys_notice_t());
		if (NULL == p_sys_notice)
			return false;
		p_sys_notice->load_data(single);
		p_sys_notice->set_count(1);
		m_sys_notice_map.insert(std::make_pair(hash_util_t::hash_to_uint32(single.type(), single.param()), p_sys_notice));
	}
	else
	{
		sys_notice_ptr p_sys_notice = iter->second;
		if (NULL == p_sys_notice)
		{
			sys_notice_ptr p_notice = sys_notice_ptr(new sys_notice_t());
			if (NULL == p_notice)
				return false;
			p_notice->load_data(single);
			p_notice->set_count(1);
			m_sys_notice_map.insert(std::make_pair(hash_util_t::hash_to_uint32(single.type(), single.param()), p_notice));
		}
		else
		{
			uint32_t cur_count = p_sys_notice->get_count();
			///log_warn("cur_count[%d] is_from_game[%d] single.count[%d]", cur_count, is_from_game, single.count());
			if (is_from_game)
			{
				p_sys_notice->set_count((single.count() > cur_count ? single.count() : cur_count));
			}
			else
			{
				p_sys_notice->set_count(single.count() + cur_count);
			}
			///log_warn("type[%d] param[%d] count[%d]", single.type(), single.param(), p_sys_notice->get_count());
		}
	}
	
	return true;

}

void sys_notice_manager_t::broadcast_system_notice_to_family(uint64_t uid, const std::string& text, uint32_t label /*= 0*/)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return;
	}

	if (text.empty())
	{
		log_error("text empty error");
		return;
	}
	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id)
	{
		return;
	}

	proto::client::ec_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();

	if (NULL != data)
	{
		data->set_type(proto::common::system_notice_type::SYS_NOTICE_CHANNEL);
		data->set_text(text);
		data->set_label(label);
		data->set_role_id(common::string_util_t::uint64_to_string(uid));
	}
	p_user->send_msg_to_client(op_cmd::ec_sys_notice_notify, ntf);
	const family_ptr& p_family = family_manager_t::get_family(family_id);
	const family::family_member_map& member_list =  p_family->get_all_member();
	for (auto& it : member_list)
	{
		if (uid == it.first)
		{
			continue;
		}
		global_user_ptr p_user_member = global_user_manager_t::get_global_user_by_uid(it.first);

		if (NULL == p_user_member)
		{
			log_error("user[%lu] null error", it.first);
			continue;
		}
		p_user_member->send_msg_to_client(op_cmd::ec_sys_notice_notify, ntf);
	}
}

sys_notice_ptr sys_notice_manager_t::get_sys_notice(uint32_t type, uint32_t param)
{
	sys_notice_map::iterator iter = m_sys_notice_map.find(hash_util_t::hash_to_uint32(type, param));
	if (iter != m_sys_notice_map.end())
	{
		sys_notice_ptr p_sys_notice = iter->second;
		if (NULL != p_sys_notice)
		{
			return p_sys_notice;
		}
	}
	///log_warn("NULL != p_sys_notice type[%d] param[%d]", type, param);
	return sys_notice_ptr();
}

bool sys_notice_manager_t::handle_ge_get_sys_notice_list_request(const network::tcp_socket_ptr& s)
{
	proto::server::eg_syn_sys_notice_notify msg;
	proto::common::sys_notity_data* p_data = msg.mutable_sys_notice_list();
	sys_notice_ptr p_sys_notice = sys_notice_ptr();

	for (auto& it : m_sys_notice_map)
	{
		p_sys_notice = it.second;
		if (NULL == p_sys_notice)
		{
			log_error("NULL == p_sys_notice");
			return false;
		}

		if (0 == p_sys_notice->get_type() ||
			0 == p_sys_notice->get_tid())
		{
			continue;
		}

		proto::common::sys_notity_single* p_single = p_data->add_sys_notity_data_list();
		p_sys_notice->peek_data(p_single);
	}
	env::server->send_msg_to_game(s, op_cmd::eg_syn_sys_notice_notify, 0, msg);
	return true;
}

void sys_notice_manager_t::update_broadcast_count(uint32_t type, uint32_t param)
{
	proto::common::sys_notity_single single;
	single.set_type(type);
	single.set_param(param);
	single.set_count(1);

	if (update_limit_count(single,false))
	{
		sys_notice_ptr p_sys_notice = get_sys_notice(type, param);
		if (NULL == p_sys_notice)
		{
			log_error("NULL == p_sys_notice type[%d] param[%d]",type,param);
			return;
		}
		proto::server::eg_check_sys_notice_reply msg_reply;
		proto::common::sys_notity_data* p_data = msg_reply.mutable_sys_notice_list();
		proto::common::sys_notity_single* p_single = p_data->add_sys_notity_data_list();
		p_sys_notice->peek_data(p_single);
		env::server->send_msg_to_all_games(op_cmd::eg_check_sys_notice_reply, 0, msg_reply);
	}
}

uint32_t sys_notice_manager_t::get_broadcast_count(uint32_t type, uint32_t param)
{
	sys_notice_map::iterator iter = m_sys_notice_map.find(hash_util_t::hash_to_uint32(type, param));
	if (iter == m_sys_notice_map.end())
	{
		return 0;
	}
	else
	{
		sys_notice_ptr p_sys_notice = iter->second;
		if (NULL == p_sys_notice)
		{
			return 0;
		}
		return p_sys_notice->get_count();
	}
}

void sys_notice_manager_t::clear_notice_list()
{
	m_sys_notice_map.clear();
}


void sys_notice_manager_t::on_send_notice_callback(const boost::system::error_code& ec)
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

	uint32_t now_time = time_util_t::now_time();
	std::vector<uint32_t> del_list;
	for (auto &it :sys_notice_manager_t::s_notice_map)
	{
		proto::gm_common::gm_notice_single &single = it.second;
		if (single.start_time() > now_time)
			continue;

		if (now_time >= single.last_send_time() + single.tick())
		{
			//发送公告	
			sys_notice_manager_t::broadcast_gm_system_notice(uint32_t(single.type()), single.notice() );
			single.set_last_send_time(now_time);
		}
		if (now_time >= single.end_time())
		{
			del_list.push_back(it.first);
			continue;
		}
	}

	for (auto it : del_list)
	{
		sys_notice_manager_t::s_notice_map.erase(it);
	}

	//重启定时器
	if (s_send_notice_timer != NULL)
	{
		s_send_notice_timer->expires_from_now( boost::posix_time::seconds(10) );
		s_send_notice_timer->async_wait(boost::bind(&sys_notice_manager_t::on_send_notice_callback, boost::asio::placeholders::error));
	}
}


void sys_notice_manager_t::add_gm_system_notice( const proto::gm_common::gm_notice_single &single)
{
	proto::gm_common::gm_notice_single data;
	data.CopyFrom( single);
	s_notice_map[1] = data;
	//sys_notice_manager_t::broadcast_gm_system_notice(uint32_t(proto::common::system_notice_type::SYS_NOTICE_LANTERN), single.notice());
}


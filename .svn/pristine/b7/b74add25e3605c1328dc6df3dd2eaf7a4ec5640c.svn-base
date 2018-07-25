#include "challenge_manager.hpp"

#include "user/global_user_manager.hpp"
#include "client.pb.h"
#include "global_id.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/sys_notice_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "redis/challenge_data_mgr.hpp"
#include "time_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "tblh/time_type_enum.hpp"

USING_NS_COMMON;

challenge_map challenge_manager_t::m_today_challenge_map;
challenge_map challenge_manager_t::m_tomorrow_challenge_map;
challenge_map challenge_manager_t::m_day_after_tomorrow_challenge_map;
challenge_record challenge_manager_t::m_challenge_record;
boost::shared_ptr<boost::asio::deadline_timer> challenge_manager_t::m_timer;

void challenge_t::start_process_timer()
{
	if (NULL == m_process_timer)
	{
		m_process_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_process_timer->cancel();
	m_process_timer->expires_from_now(boost::posix_time::seconds(challenge_manager_t::get_process_time()));
	m_process_timer->async_wait(boost::bind(&challenge_manager_t::challenge_timeout, get_uid(), boost::asio::placeholders::error));
	
}

void challenge_t::stop_process_timer()
{
	if (NULL != m_process_timer)
	{
		m_process_timer->cancel();
	}
}

void challenge_t::start_notice_timer()
{
	if (NULL == m_process_timer)
	{
		m_process_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_process_timer->cancel();
	m_process_timer->expires_from_now(boost::posix_time::seconds(challenge_manager_t::get_notice_cd()));
	m_process_timer->async_wait(boost::bind(&challenge_manager_t::on_notice_timer, get_uid(), boost::asio::placeholders::error));
}

void challenge_t::stop_notice_timer()
{
	if (NULL != m_process_timer)
	{
		m_process_timer->cancel();
	}
}

void challenge_t::peek_data(proto::common::challenge_info* p_info)
{
	if (NULL == p_info)
	{
		return;
	}
	p_info->set_challenge_uid(string_util_t::uint64_to_string(get_uid()));
	p_info->set_challenger_name(get_challenger_name());
	p_info->set_target_name(get_target_name());
	p_info->set_challenge_time(time_util_t::get_day_str(get_challenge_time()));
	p_info->set_result(get_state());
}

uint64_t challenge_manager_t::get_new_challenge_uid()
{
	return g_id_allocator.gen_new_id(ID_ALLOCATOR_TYPE_CHALLENGE);
}

void challenge_manager_t::load_data()
{
	uint32_t today = time_util_t::get_today();
	challenge_data_mgr_t::load_challenge_by_day(today, m_today_challenge_map);
	PK_LOG("today[%d] m_today_challenge_map size[%d]", today, m_today_challenge_map.size());

	uint32_t tomorrow = time_util_t::get_tomorrow();
	challenge_data_mgr_t::load_challenge_by_day(tomorrow, m_tomorrow_challenge_map);
	PK_LOG("tomorrow[%d] m_tomorrow_challenge_map size[%d]", tomorrow, m_tomorrow_challenge_map.size());

	uint32_t day_after_tomorrow = time_util_t::get_day_after_tommrrow();
	challenge_data_mgr_t::load_challenge_by_day(day_after_tomorrow, m_day_after_tomorrow_challenge_map);
	PK_LOG("day_after_tomorrow[%d] m_day_after_tomorrow_challenge_map size[%d]", day_after_tomorrow, m_day_after_tomorrow_challenge_map.size());

	challenge_data_mgr_t::load_challenge_record(get_pass_day(), get_record_num(), m_challenge_record);

	time_manager_t::register_func(time_type_enum::time_pk_battle1, challenge_manager_t::notice_challenge);
	time_manager_t::register_func(time_type_enum::time_pk_battle2, challenge_manager_t::ready_challenge);
}

void challenge_manager_t::peek_challenge_record(proto::client::ec_challenge_record_reply& reply)
{
	challenge_record::reverse_iterator it_rbegin = m_challenge_record.rbegin();

	for (auto it = it_rbegin; it != m_challenge_record.rend(); ++it)
	{
		challenge_ptr p_challenge = *it;
		if (NULL == p_challenge)
		{
			continue;
		}
		proto::common::challenge_info* p_info = reply.add_challenge_done();
		p_challenge->peek_data(p_info);
	}
	for (auto it : m_today_challenge_map)
	{
		challenge_ptr p_challenge = it.second;
		if (NULL == p_challenge)
		{
			continue;
		}
		if (p_challenge->get_state() == proto::common::challenge_state_win || p_challenge->get_state() == proto::common::challenge_state_lose)
		{
			proto::common::challenge_info* p_info = reply.add_challenge_done();
			p_challenge->peek_data(p_info);
		}
		else if (p_challenge->get_state() == proto::common::challenge_state_accept)
		{
			proto::common::challenge_info* p_info = reply.add_challenge_coming();
			p_challenge->peek_data(p_info);
		}
	}
	
	for (auto it : m_tomorrow_challenge_map)
	{
		challenge_ptr p_challenge = it.second;
		if (NULL == p_challenge)
		{
			continue;
		}
		if (p_challenge->get_state() != proto::common::challenge_state_accept)
		{
			continue;
		}
		proto::common::challenge_info* p_info = reply.add_challenge_coming();
		p_challenge->peek_data(p_info);
	}
	for (auto it : m_day_after_tomorrow_challenge_map)
	{
		challenge_ptr p_challenge = it.second;
		if (NULL == p_challenge)
		{
			continue;
		}
		proto::common::challenge_info* p_info = reply.add_challenge_coming();
		p_challenge->peek_data(p_info);
	}
}

uint32_t challenge_manager_t::challenge_request(global_user_ptr p_role, uint64_t target_uid)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (p_role->get_uid() == target_uid)
	{
		log_error("role[%lu] challenge self", target_uid);
		return errcode_enum::notice_unknown;
	}
	global_user_ptr p_target = global_user_manager_t::get_global_user_by_uid(target_uid);
	if (NULL == p_target)
	{
		log_error("NULL == p_target[%lu]", target_uid);
		return errcode_enum::pk_battle_cue_2;
	}
	if (global_user_manager_t::get_delta_level(p_role, p_target) > get_delta_level())
	{
		log_error("delta_level error");
		return errcode_enum::pk_battle_cue_1;
	}
	challenge_ptr p_challenge = get_day_after_tomorrow_challenge(p_role->get_uid());
	if (NULL != p_challenge)
	{
		log_error("role[%lu] has challenge already", p_role->get_uid());
		return errcode_enum::pk_cue_7;
	}
	p_challenge = get_day_after_tomorrow_challenge(p_target->get_uid());
	if (NULL != p_challenge)
	{
		log_error("target[%lu] has challenge already", p_target->get_uid());
		return errcode_enum::pk_cue_8;
	}
	challenge_ptr p_new_challenge = challenge_ptr(new challenge_t);
	p_new_challenge->set_uid(get_new_challenge_uid());
	p_new_challenge->set_challenger_uid(p_role->get_uid());
	p_new_challenge->set_target_uid(p_target->get_uid());
	p_new_challenge->set_challenger_name(p_role->get_name());
	p_new_challenge->set_target_name(p_target->get_name());
	uint32_t day_after_tomorrow = time_util_t::get_day_after_tommrrow();
	p_new_challenge->set_challenge_time(day_after_tomorrow);
	p_new_challenge->start_process_timer();
	m_day_after_tomorrow_challenge_map.insert(std::make_pair(p_new_challenge->get_uid(), p_new_challenge));
	//存储
	challenge_data_mgr_t::add_challenge(p_new_challenge);

	PK_LOG("add_challenge[%llu] challenger[%llu] target[%llu]", p_new_challenge->get_uid(), p_role->get_uid(), p_target->get_uid());
	//通知约战目标
	proto::client::ec_challenge_invite_notify ntf;
	ntf.set_challenger_name(p_role->get_name());
	p_target->send_msg_to_client(op_cmd::ec_challenge_invite_notify, ntf);
	return errcode_enum::error_ok;
}

uint32_t challenge_manager_t::challenge_process_request(global_user_ptr p_role, uint32_t process)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	challenge_ptr p_challenge = get_day_after_tomorrow_challenge(p_role->get_uid());
	if (NULL == p_challenge)
	{
		log_error("target[%lu] has not challenge", p_role->get_uid());
		return errcode_enum::notice_no_match_role;
	}
	if (p_role->get_uid() != p_challenge->get_target_uid())
	{
		log_error("target[%lu] has not challenge", p_role->get_uid());
		return errcode_enum::notice_no_match_role;
	}
	if (p_challenge->get_state() != proto::common::challenge_state_process)
	{
		log_error("target[%lu] challenge[%lu] state[%d] error not[%d]", p_role->get_uid(), p_challenge->get_uid(), p_challenge->get_state(), proto::common::challenge_state_process);
		return errcode_enum::notice_no_match_role;
	}
	p_challenge->stop_process_timer();

	if (0 == process)
	{
		PK_LOG("role[%lu] accepted challenge[%llu]", p_role->get_uid(), p_challenge->get_uid());
		//应战
		p_challenge->set_state(proto::common::challenge_state_accept);
		//存储
		challenge_data_mgr_t::update_challenge_state(p_challenge->get_uid(), p_challenge->get_state());
		//接受约战的公告
		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_12, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
		p_challenge->set_can_notice(true);
	}
	else
	{
		PK_LOG("role[%lu] refused challenge[%llu]", p_role->get_uid(), p_challenge->get_uid());
		p_challenge->set_state(proto::common::challenge_state_refuse);
		//存储
		challenge_data_mgr_t::update_challenge_state(p_challenge->get_uid(), p_challenge->get_state());
		//拒绝
		auto it = m_day_after_tomorrow_challenge_map.find(p_challenge->get_uid());
		if (it != m_day_after_tomorrow_challenge_map.end())
		{
			m_day_after_tomorrow_challenge_map.erase(it);
		}
		//拒绝约战的公告
		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_11, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str())
	}
	//通知双方结果
	proto::client::ec_challenge_result_notify ntf;
	ntf.set_challenger_name(p_challenge->get_challenger_name());
	ntf.set_target_name(p_challenge->get_target_name());
	ntf.set_result(p_challenge->get_state());
	p_role->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
	global_user_ptr p_challenger = global_user_manager_t::get_global_user_by_uid(p_challenge->get_challenger_uid());
	if (NULL != p_challenger)
	{
		p_challenger->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
	}
	return errcode_enum::error_ok;
}

void challenge_manager_t::challenge_timeout(uint64_t challenger_uid, const boost::system::error_code& ec)
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
	auto iter = m_day_after_tomorrow_challenge_map.find(challenger_uid);
	if (iter == m_day_after_tomorrow_challenge_map.end())
	{
		log_error("NULL == p_challenge[%lu]", challenger_uid);
		return;
	}
	challenge_ptr p_challenge = iter->second;
	if (NULL == p_challenge)
	{
		log_error("NULL == p_challenge[%lu]", challenger_uid);
		return;
	}
	p_challenge->stop_process_timer();
	PK_LOG("challenge[%llu] refused, respond time_out", p_challenge->get_uid());
	p_challenge->set_state(proto::common::challenge_state_refuse);
	challenge_data_mgr_t::update_challenge_state(p_challenge->get_uid(), p_challenge->get_state());
	//拒绝约战的公告
	BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_11, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
	//通知双方结果
	proto::client::ec_challenge_result_notify ntf;
	ntf.set_challenger_name(p_challenge->get_challenger_name());
	ntf.set_target_name(p_challenge->get_target_name());
	ntf.set_result(proto::common::challenge_state_refuse);
	global_user_ptr p_challenger = global_user_manager_t::get_global_user_by_uid(p_challenge->get_challenger_uid());
	if (NULL != p_challenger)
	{
		p_challenger->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
	}
	global_user_ptr p_target = global_user_manager_t::get_global_user_by_uid(p_challenge->get_target_uid());
	if (NULL != p_target)
	{
		p_target->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
	}

	auto it = m_day_after_tomorrow_challenge_map.find(p_challenge->get_uid());
	if (it != m_day_after_tomorrow_challenge_map.end())
	{
		m_day_after_tomorrow_challenge_map.erase(it);
	}
}

void challenge_manager_t::challenge_fight_result(uint64_t challenger_uid, uint32_t result)
{
	challenge_ptr p_challenge = get_today_challenge(challenger_uid);
	if (NULL == p_challenge)
	{
		log_error("NULL == p_challenge, challenger_uid[%lu]", challenger_uid);
		return;
	}
	if (p_challenge->get_challenger_uid() != challenger_uid)
	{
		log_error("NULL == p_challenge, challenger_uid[%lu]", challenger_uid);
		return;
	}
	PK_LOG("challenge[%llu] fight result[%d]", p_challenge->get_uid(), result);
	p_challenge->set_state(result);
	challenge_done(p_challenge);
	challenge_data_mgr_t::update_challenge_state(p_challenge->get_uid(), p_challenge->get_state());
	proto::client::ec_challenge_result_notify ntf;
	ntf.set_challenger_name(p_challenge->get_challenger_name());
	ntf.set_target_name(p_challenge->get_target_name());
	ntf.set_result(p_challenge->get_state());
	global_user_ptr p_challenger = global_user_manager_t::get_global_user_by_uid(p_challenge->get_challenger_uid());
	if (NULL != p_challenger)
	{
		p_challenger->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
	}
	global_user_ptr p_target = global_user_manager_t::get_global_user_by_uid(p_challenge->get_target_uid());
	if (NULL != p_target)
	{
		p_target->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
	}
	if (result == proto::common::challenge_state_win)
	{
		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_16, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
	}
	else if (result == proto::common::challenge_state_lose)
	{
		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_17, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
	}
	else if (result == proto::common::challenge_state_draw)
	{
		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_18, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
	}
	else if (result == proto::common::challenge_state_win_without_fight)
	{
		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_15, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_challenger_name().c_str());
	}
	else if (result == proto::common::challenge_state_lose_without_fight)
	{
		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_15, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
	}
}

void challenge_manager_t::one_day()
{
	PK_LOG("challenge_manager_t::one_day");
	m_today_challenge_map.clear();
	m_today_challenge_map = m_tomorrow_challenge_map;
	PK_LOG("m_today_challenge_map size[%d]", m_today_challenge_map.size());
	m_tomorrow_challenge_map.clear();
	m_tomorrow_challenge_map = m_day_after_tomorrow_challenge_map;
	PK_LOG("m_tomorrow_challenge_map size[%d]", m_tomorrow_challenge_map.size());
	m_day_after_tomorrow_challenge_map.clear();
}

void challenge_manager_t::notice_challenge(uint32_t id, bool is_in_time)
{
	PK_LOG("notice_challenge");
	if (!is_in_time)
	{
		return;
	}
	for (auto it : m_today_challenge_map)
	{
		challenge_ptr p_challenge = it.second;
		if (NULL == p_challenge)
		{
			continue;
		}
		if (p_challenge->get_state() != proto::common::challenge_state_accept)
		{
			continue;
		}
		p_challenge->stop_notice_timer();
		p_challenge->set_can_notice(false);
		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_14, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
	}
}

void challenge_manager_t::notice_single_challenge(uint64_t role_uid)
{
	//按照今天明天后天的优先级查找最近的一次已经应战的约战
	challenge_ptr p_challenge = get_today_challenge(role_uid);
	if (NULL == p_challenge || p_challenge->get_state() != proto::common::challenge_state_accept)
	{
		p_challenge = get_tomorrow_challenge(role_uid);
	}
	if (NULL == p_challenge || p_challenge->get_state() != proto::common::challenge_state_accept)
	{
		p_challenge = get_day_after_tomorrow_challenge(role_uid);
	}
	if (NULL == p_challenge || p_challenge->get_state() != proto::common::challenge_state_accept)
	{
		return;
	}
	global_user_ptr p_challenger = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL != p_challenger)
	{
		proto::client::ec_challenge_coming_notify ntf;
		p_challenge->peek_data(ntf.mutable_coming_challenge());
		p_challenger->send_msg_to_client(op_cmd::ec_challenge_coming_notify, ntf);
	}
	if (!p_challenge->can_notice())
	{
		return;
	}
	p_challenge->set_can_notice(false);
	p_challenge->start_notice_timer();
	//成功找到 开始装逼
	BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_13, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str(), time_util_t::get_day_str(p_challenge->get_challenge_time()).c_str());
	
}

void challenge_manager_t::on_notice_timer(uint64_t challenger_uid, const boost::system::error_code& ec)
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
	auto it = m_today_challenge_map.find(challenger_uid);
	if (it == m_today_challenge_map.end())
	{
		log_error("NULL == p_challenge[%lu]", challenger_uid);
		return;
	}
	challenge_ptr p_challenge = it->second;
	if (NULL == p_challenge)
	{
		log_error("NULL == p_challenge[%lu]", challenger_uid);
		return;
	}
	p_challenge->stop_notice_timer();
	p_challenge->set_can_notice(true);
}

void challenge_manager_t::ready_challenge(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		PK_LOG("ready_challenge size[%d]", m_today_challenge_map.size());
		//进入战前倒计时
		for (auto it : m_today_challenge_map)
		{
			challenge_ptr p_challenge = it.second;
			if (NULL == p_challenge)
			{
				continue;
			}
			if (p_challenge->get_state() != proto::common::challenge_state_accept)
			{
				continue;
			}
			global_user_ptr p_challenger = global_user_manager_t::get_global_user_by_uid(p_challenge->get_challenger_uid());
			if (NULL != p_challenger && p_challenger->is_in_king_city())
			{
				proto::client::ec_challenge_ready_notify ntf;
				ntf.set_challenger_name(p_challenge->get_target_name());
				p_challenger->send_msg_to_client(op_cmd::ec_challenge_ready_notify, ntf);
				//通知game传送
				proto::server::eg_challenge_transfer_notify ntf_s;
				p_challenger->send_msg_to_game(op_cmd::eg_challenge_transfer_notify, ntf_s);
			}

			global_user_ptr p_target = global_user_manager_t::get_global_user_by_uid(p_challenge->get_target_uid());
			if (NULL != p_target && p_target->is_in_king_city())
			{
				proto::client::ec_challenge_ready_notify ntf;
				ntf.set_challenger_name(p_challenge->get_challenger_name());
				p_target->send_msg_to_client(op_cmd::ec_challenge_ready_notify, ntf);
				//通知game传送
				proto::server::eg_challenge_transfer_notify ntf_s;
				p_target->send_msg_to_game(op_cmd::eg_challenge_transfer_notify, ntf_s);
			}
		}
	}
	else
	{
		PK_LOG("begin_challenge ready size[%d]", m_today_challenge_map.size());
		for (auto it : m_today_challenge_map)
		{
			challenge_ptr p_challenge = it.second;
			if (NULL == p_challenge)
			{
				continue;
			}
			if (p_challenge->get_state() != proto::common::challenge_state_accept)
			{
				continue;
			}
			bool challenger_exist = true;
			global_user_ptr p_challenger = global_user_manager_t::get_global_user_by_uid(p_challenge->get_challenger_uid());
			if (NULL == p_challenger)
			{
				PK_LOG("NULL == p_challenger[%llu]", p_challenge->get_challenger_uid());
				challenger_exist = false;
			}
			else if (!p_challenger->is_in_king_city())
			{
				PK_LOG("p_challenger[%llu] is not in king_city", p_challenge->get_challenger_uid());
				challenger_exist = false;
			}
			bool target_exist = true;
			global_user_ptr p_target = global_user_manager_t::get_global_user_by_uid(p_challenge->get_target_uid());
			if (NULL == p_target)
			{
				PK_LOG("NULL == p_target[%llu]", p_challenge->get_target_uid());
				target_exist = false;
			}
			else if (!p_target->is_in_king_city())
			{
				PK_LOG("p_target[%llu] is not in king_city", p_challenge->get_target_uid());
				target_exist = false;
			}
			if (challenger_exist)
			{
				if (target_exist)
				{
					//通知双方倒计时
					proto::client::ec_challenge_fight_start_notify ntf;
					p_challenger->send_msg_to_client(op_cmd::ec_challenge_fight_start_notify, ntf);
					p_target->send_msg_to_client(op_cmd::ec_challenge_fight_start_notify, ntf);

					//通知game传送
					proto::server::eg_challenge_transfer_notify ntf_s;
					p_challenger->send_msg_to_game(op_cmd::eg_challenge_transfer_notify, ntf_s);
					p_target->send_msg_to_game(op_cmd::eg_challenge_transfer_notify, ntf_s);
				}
				else
				{
					//发起方不战而胜
					p_challenge->set_state(proto::common::challenge_state_win_without_fight);
					BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_15, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_challenger_name().c_str());
					proto::client::ec_challenge_win_without_fight_notify ntf;
					ntf.set_challenger_name(p_challenge->get_target_name());
					p_challenger->send_msg_to_client(op_cmd::ec_challenge_win_without_fight_notify, ntf);
				}
			}
			else
			{
				if (target_exist)
				{
					//应战方不战而胜 约战方不战而败
					p_challenge->set_state(proto::common::challenge_state_lose_without_fight);
					BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_15, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
					proto::client::ec_challenge_win_without_fight_notify ntf;
					ntf.set_challenger_name(p_challenge->get_challenger_name());
					p_target->send_msg_to_client(op_cmd::ec_challenge_win_without_fight_notify, ntf);
				}
				else
				{
					//双方都没到场
					p_challenge->set_state(proto::common::challenge_state_draw);
					BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_18, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
				}
			}
			challenge_data_mgr_t::update_challenge_state(p_challenge->get_uid(), p_challenge->get_state());
			//proto::client::ec_challenge_result_notify ntf;
			//ntf.set_challenger_name(p_challenge->get_challenger_name());
			//ntf.set_target_name(p_challenge->get_target_name());
			//ntf.set_result(p_challenge->get_state());
			//if (NULL != p_challenger)
			//{
			//	p_challenger->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
			//}
			//if (NULL != p_target)
			//{
			//	p_target->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
			//}
		}
		if (NULL == m_timer)
		{
			m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
		}
		m_timer->expires_from_now(boost::posix_time::seconds(get_fight_start_time()));
		m_timer->async_wait(boost::bind(&challenge_manager_t::begin_challenge, boost::asio::placeholders::error));
	}
}

void challenge_manager_t::begin_challenge(const boost::system::error_code& ec)
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
	PK_LOG("begin_challenge size[%d]", m_today_challenge_map.size());
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}
	
	for (auto it : m_today_challenge_map)
	{
		challenge_ptr p_challenge = it.second;
		if (NULL == p_challenge)
		{
			continue;
		}
		if (p_challenge->get_state() != proto::common::challenge_state_accept)
		{
			continue;
		}
		bool challenger_exist = false;
		global_user_ptr p_challenger = global_user_manager_t::get_global_user_by_uid(p_challenge->get_challenger_uid());
		if (NULL != p_challenger && p_challenger->is_in_king_city())
		{
			challenger_exist = true;
		}
		bool target_exist = false;
		global_user_ptr p_target = global_user_manager_t::get_global_user_by_uid(p_challenge->get_target_uid());
		if (NULL != p_target && p_target->is_in_king_city())
		{
			target_exist = true;
		}
		if (challenger_exist)
		{
			if (target_exist)
			{
				//开打 等结果再改变状态通知客户端
				proto::server::eg_challenge_fight_request req;
				req.set_target_uid(p_challenge->get_target_uid());
				p_challenger->send_msg_to_game(op_cmd::eg_challenge_fight_request, req);
				return;
			}
			else
			{
				//发起方不战而胜
				p_challenge->set_state(proto::common::challenge_state_win_without_fight);
				BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_15, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_challenger_name().c_str());
			}
		}
		else
		{
			if (target_exist)
			{
				//应战方不战而胜 发起方不战而败
				p_challenge->set_state(proto::common::challenge_state_lose_without_fight);
				BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_15, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str(), p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
			}
			else
			{
				//双方都没到场
				p_challenge->set_state(proto::common::challenge_state_draw);
				BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_18, 0, 0, 0, p_challenge->get_challenger_name().c_str(), p_challenge->get_target_name().c_str());
			}
		}
		challenge_data_mgr_t::update_challenge_state(p_challenge->get_uid(), p_challenge->get_state());
		proto::client::ec_challenge_result_notify ntf;
		ntf.set_challenger_name(p_challenge->get_challenger_name());
		ntf.set_target_name(p_challenge->get_target_name());
		ntf.set_result(p_challenge->get_state());
		if (NULL != p_challenger)
		{
			p_challenger->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
		}
		if (NULL != p_target)
		{
			p_target->send_msg_to_client(op_cmd::ec_challenge_result_notify, ntf);
		}
	}
}

void challenge_manager_t::check_ready(uint64_t role_uid)
{
	if (!time_manager_t::is_in_time(time_type_enum::time_pk_battle2))
	{
		return;
	}
	challenge_ptr p_challenge = get_today_challenge(role_uid);
	if (NULL == p_challenge || p_challenge->get_state() != proto::common::challenge_state_accept)
	{
		return;
	}
	global_user_ptr p_challenger = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL != p_challenger)
	{
		proto::client::ec_challenge_ready_notify ntf;
		if (role_uid == p_challenge->get_challenger_uid())
		{
			ntf.set_challenger_name(p_challenge->get_target_name());
		}
		else
		{
			ntf.set_challenger_name(p_challenge->get_challenger_name());
		}
		p_challenger->send_msg_to_client(op_cmd::ec_challenge_ready_notify, ntf);
		//通知game传送
		proto::server::eg_challenge_transfer_notify ntf_s;
		p_challenger->send_msg_to_game(op_cmd::eg_challenge_transfer_notify, ntf_s);
	}
}

challenge_ptr challenge_manager_t::get_today_challenge(uint64_t role_uid)
{
	for (auto it : m_today_challenge_map)
	{
		challenge_ptr p_challenge = it.second;
		if (NULL == p_challenge)
		{
			continue;
		}
		if (p_challenge->get_challenger_uid() == role_uid || p_challenge->get_target_uid() == role_uid)
		{
			return p_challenge;
		}
	}
	return challenge_ptr(NULL);
}

challenge_ptr challenge_manager_t::get_tomorrow_challenge(uint64_t role_uid)
{
	for (auto it : m_tomorrow_challenge_map)
	{
		challenge_ptr p_challenge = it.second;
		if (NULL == p_challenge)
		{
			continue;
		}
		if (p_challenge->get_challenger_uid() == role_uid || p_challenge->get_target_uid() == role_uid)
		{
			return p_challenge;
		}
	}
	return challenge_ptr(NULL);
}

challenge_ptr challenge_manager_t::get_day_after_tomorrow_challenge(uint64_t role_uid)
{
	for (auto it : m_day_after_tomorrow_challenge_map)
	{
		challenge_ptr p_challenge = it.second;
		if (NULL == p_challenge)
		{
			continue;
		}
		if (p_challenge->get_challenger_uid() == role_uid || p_challenge->get_target_uid() == role_uid)
		{
			return p_challenge;
		}
	}
	return challenge_ptr(NULL);
}

void challenge_manager_t::challenge_done(challenge_ptr p_challenge)
{
	if (NULL == p_challenge)
	{
		return;
	}
	auto it = m_today_challenge_map.find(p_challenge->get_uid());
	if (it == m_today_challenge_map.end())
	{
		return;
	}
	if (p_challenge->get_state() == proto::common::challenge_state_win || p_challenge->get_state() == proto::common::challenge_state_lose)
	{
		if (m_challenge_record.size() >= get_record_num())
		{
			m_challenge_record.pop_front();
		}
		m_challenge_record.push_back(p_challenge);
	}
	
	m_today_challenge_map.erase(it);
}

uint32_t challenge_manager_t::get_delta_level()
{
	auto delta_level = GET_CONF(Comprehensive, comprehensive_common::pk_battle_valid_lv);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(delta_level);
	return val;
}

uint32_t challenge_manager_t::get_notice_cd()
{
	auto notice_cd = GET_CONF(Comprehensive, comprehensive_common::pk_battle_notice_time);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(notice_cd);
	return val;
}

uint32_t challenge_manager_t::get_fight_start_time()
{
	auto fight_start_time = GET_CONF(Comprehensive, comprehensive_common::pk_battle_fight_start_count_down);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(fight_start_time);
	return val;
}

uint32_t challenge_manager_t::get_process_time()
{
	auto process_time = GET_CONF(Comprehensive, comprehensive_common::pk_battle_auto_refuse_count_down);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(process_time);
	return val;
}

uint32_t challenge_manager_t::get_pass_day()
{
	auto pass_day = GET_CONF(Comprehensive, comprehensive_common::pk_battle_fight_record_save_days);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(pass_day);
	return val;
}

uint32_t challenge_manager_t::get_record_num()
{
	return 50;
}

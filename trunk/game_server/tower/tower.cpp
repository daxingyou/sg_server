#include "tower.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "global_id.hpp"
#include "config/config_manager.hpp"
#include "tblh/Tower.tbls.h"
#include "tblh/TowerTrigger.tbls.h"
#include "role/role_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "time_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "mail/mail_manager.hpp"
USING_NS_COMMON;
tower_t::tower_t(uint64_t uid)
	: m_owner(uid)
{
	m_key = cache_key_t::create(m_owner, cache_name::tower);
}

tower_t::~tower_t()
{

}


void tower_t::init_new_data()
{
	m_reset_count = 1;
	m_max_reset_count = 1;
	m_current_tower = 1;
	m_current_achieve = 1;
	m_trigger_count = get_max_trigger_count();
	m_trigger_fight_count = get_max_trigger_fight_count();
	//save_self();
}

void tower_t::load_data(const proto::common::tower_data& td)
{
	m_record_tower = td.record_tower();
	m_current_tower = td.current_tower();
	m_reset_count = td.reset_count();
	m_max_reset_count = td.max_reset_count();
	m_auto_fight_time = td.auto_fight_time();
	m_current_achieve = td.current_achieve();
	m_first_day = td.first_day();
	m_trigger_count = td.trigger_count();
	m_trigger_fight_count = td.trigger_fight_count();
	m_send_mail_flag = td.send_mail_flag();

	uint32_t now_time = time_util_t::now_time();
	const proto::common::tower_trigger_data& ttd = td.tower_trigger();
	for (int32_t i = 0; i < ttd.self_trigger_size(); ++i)
	{
		const proto::common::tower_trigger& tt = ttd.self_trigger(i);
		if (tt.expired_time() > 0 && now_time >= tt.expired_time())
		{
			continue;
		}
		tower_trigger_ptr p_new_trigger = tower_trigger_ptr(new tower_trigger_t);
		p_new_trigger->load_data(tt);
		m_self_trigger.insert(std::make_pair(p_new_trigger->get_uid(), p_new_trigger));
	}
	for (int32_t i = 0; i < ttd.other_trigger_size(); ++i)
	{
		const proto::common::tower_trigger& tt = ttd.other_trigger(i);
		if (tt.expired_time() > 0 && now_time >= tt.expired_time())
		{
			continue;
		}
		tower_trigger_ptr p_new_trigger = tower_trigger_ptr(new tower_trigger_t);
		p_new_trigger->load_data(tt);
		m_other_trigger.insert(std::make_pair(p_new_trigger->get_uid(), p_new_trigger));
	}

	//容错代码
	if (m_current_tower == 0)
	{
		init_new_data();
		//log_error("user [%lu], tower = 0 , need init", m_owner );
	}


	//查看是否需要恢复定时器
	set_auto_fight_timer();
}

void tower_t::peek_data(proto::common::tower_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_record_tower(m_record_tower);
	p_data->set_current_tower(m_current_tower);
	p_data->set_reset_count(m_reset_count);
	p_data->set_max_reset_count(m_max_reset_count);
	p_data->set_auto_fight_time(m_auto_fight_time);
	p_data->set_current_achieve(m_current_achieve);
	p_data->set_first_day(m_first_day);
	p_data->set_trigger_count(m_trigger_count);
	p_data->set_trigger_fight_count(m_trigger_fight_count);
	p_data->set_send_mail_flag(m_send_mail_flag);

	uint32_t now_time = time_util_t::now_time();
	proto::common::tower_trigger_data* p_tower_trigger_data = p_data->mutable_tower_trigger();
	for (auto it : m_self_trigger)
	{
		tower_trigger_ptr p_trigger = it.second;
		if (NULL == p_trigger)
		{
			continue;
		}
		if (p_trigger->get_expired_time() > 0 && now_time >= p_trigger->get_expired_time())
		{
			continue;
		}
		proto::common::tower_trigger* p_single = p_tower_trigger_data->add_self_trigger();
		p_trigger->peek_data(p_single);
		TOWER_LOG("self_trigger[%lu]", p_trigger->get_uid());
	}
	for (auto it : m_other_trigger)
	{
		tower_trigger_ptr p_trigger = it.second;
		if (NULL == p_trigger)
		{
			continue;
		}
		if (p_trigger->get_expired_time() > 0 && now_time >= p_trigger->get_expired_time())
		{
			continue;
		}
		proto::common::tower_trigger* p_single = p_tower_trigger_data->add_other_trigger();
		p_trigger->peek_data(p_single);
		TOWER_LOG("other_trigger[%lu]", p_trigger->get_uid());
	}
	TOWER_LOG("self_trigger[%d] other_trigger[%d]", p_data->tower_trigger().self_trigger_size(), p_data->tower_trigger().other_trigger_size());
}

void tower_t::save_self()
{
	proto::common::tower_data tower_data;
	peek_data(&tower_data);
	role_unify_save::add_task(m_owner, m_key, tower_data);
}

void tower_t::one_day()
{
	if (m_reset_count < m_max_reset_count)
	{
		++m_reset_count;
	}
	m_trigger_count = get_max_trigger_count();
	m_trigger_fight_count = get_max_trigger_fight_count();
	uint32_t now_time = time_util_t::now_time();
	for (auto it : m_self_trigger)
	{
		tower_trigger_ptr p_trigger = it.second;
		if (NULL == p_trigger)
		{
			continue;
		}
		if (0 == p_trigger->get_expired_time())
		{
			p_trigger->set_expired_time(now_time);
		}
	}
	for (auto it : m_other_trigger)
	{
		tower_trigger_ptr p_trigger = it.second;
		if (NULL == p_trigger)
		{
			continue;
		}
		if (0 == p_trigger->get_expired_time())
		{
			p_trigger->set_expired_time(now_time);
		}
	}
	save_self();
}

uint64_t tower_t::gen_trigger_uid()
{
	uint64_t uid = common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_TOWER_TRIGGER);
	return uid;
}

void tower_t::add_other_trigger(const proto::common::tower_trigger& single)
{
	uint64_t uid = string_util_t::string_to_uint64(single.uid());
	if (m_other_trigger.find(uid) != m_other_trigger.end())
	{
		log_error("role[%lu] trigger[%lu] already exist", m_owner, uid);
		return;
	}
	tower_trigger_ptr p_new_trigger = tower_trigger_ptr(new tower_trigger_t);
	p_new_trigger->set_uid(uid);
	p_new_trigger->set_tid(single.tid());
	p_new_trigger->set_expired_time(single.expired_time());
	p_new_trigger->set_finder_name(single.finder_name());
	m_other_trigger.insert(std::make_pair(uid, p_new_trigger));
	TOWER_LOG("role[%lu] add other trigger[%lu:%d]", m_owner, uid, single.tid());
	save_self();

	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL != p_role)
	{
		proto::client::gc_tower_other_trigger_notify ntf;
		p_new_trigger->peek_data(ntf.mutable_other_trigger());
		p_role->send_msg_to_client(op_cmd::gc_tower_other_trigger_notify, ntf);
	}
}


void tower_t::trigger(uint32_t current_tower)
{
	if (0 == m_trigger_count)
	{
		TOWER_LOG("role[%lu] 0 == m_trigger_count", m_owner);
		return;
	}
	auto p_current_conf = GET_CONF(Tower, current_tower);
	if (NULL == p_current_conf)
	{
		log_error("Tower NULL == p_current_conf[%d]", current_tower);
		return;
	}
	auto p_record_conf = GET_CONF(Tower, m_record_tower);
	if (NULL == p_record_conf)
	{
		log_error("Tower NULL == p_record_conf[%d]", m_record_tower);
		return;
	}
	uint32_t time_id = get_double_time_id();
	uint32_t rate = p_current_conf->trigger_rate();
	if (time_manager_t::is_in_time(time_id))
	{
		rate = rate * get_double_value() / 100;
	}
	if (!random_util_t::selectByTenTh(rate))
	{
		TOWER_LOG("role[%lu] selectByTenTh[%d]", m_owner, p_current_conf->trigger_rate());
		return;
	}
	uint32_t rand_max = 0;
	for (auto it : p_record_conf->trigger())
	{
		rand_max += it.second;
	}
	if (0 == rand_max)
	{
		log_error("Tower[%d] 0 == rand_max", m_record_tower);
		return;
	}
	int32_t rand_val = random_util_t::randMin(0, rand_max);
	if (-1 == rand_val)
	{
		log_error("Tower[%d] rand_max invalid", m_record_tower);
		return;
	}
	uint32_t trigger_id = 0;
	for (auto it : p_record_conf->trigger())
	{
		if ((uint32_t)rand_val < it.second)
		{
			trigger_id = it.first;
			break;
		}
		else
		{
			rand_val -= it.second;
		}
	}
	if (0 == trigger_id)
	{
		log_error("Tower[%d] 0 == trigger_id", m_record_tower);
		return;
	}
	auto p_trigger_conf = GET_CONF(TowerTrigger, trigger_id);
	if (NULL == p_trigger_conf)
	{
		log_error("TowerTrigger NULL == p_trigger_conf[%d]", trigger_id);
		return;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	uint64_t uid = gen_trigger_uid();
	if (m_self_trigger.find(uid) != m_self_trigger.end())
	{
		log_error("role[%lu] trigger[%lu] already exist", m_owner, uid);
		return;
	}
	--m_trigger_count;
	tower_trigger_ptr p_new_trigger = tower_trigger_ptr(new tower_trigger_t);
	p_new_trigger->set_uid(uid);
	p_new_trigger->set_tid(trigger_id);
	p_new_trigger->set_expired_time(time_util_t::now_time() + p_trigger_conf->alive_time());
	p_new_trigger->set_finder_name(p_role->get_name());
	m_self_trigger.insert(std::make_pair(uid, p_new_trigger));
	TOWER_LOG("role[%lu] add self trigger[%lu:%d]", m_owner, uid, trigger_id);
	std::vector<std::string> param_str_list;
	param_str_list.push_back(p_role->get_name());
	param_str_list.push_back(common::string_util_t::uint32_to_string(trigger_id));
	sys_notice_manager_t::sys_notice_notify(p_role->get_uid(), proto::common::sys_notify_type_tower_trigger, 0, param_str_list);
	//通知到center
	proto::server::ge_tower_trigger_notify ntf;
	p_new_trigger->peek_data(ntf.add_tower_trigger());
	ntf.set_friend_uid(0);
	env::server->send_msg_to_center(op_cmd::ge_tower_trigger_notify, m_owner, ntf);

	//通知到客户端
	proto::client::gc_tower_trigger_notify ntf_to_client;
	ntf_to_client.set_trigger_tid(trigger_id);
	p_role->send_msg_to_client(op_cmd::gc_tower_trigger_notify, ntf_to_client);
}

void tower_t::peek_self_trigger(proto::common::tower_trigger_data* p_data)
{
	if (NULL == p_data)
	{
		return;
	}
	uint32_t now_time = time_util_t::now_time();
	
	for (auto it : m_self_trigger)
	{
		tower_trigger_ptr p_trigger = it.second;
		if (NULL == p_trigger)
		{
			continue;
		}
		if (now_time >= p_trigger->get_expired_time())
		{
			continue;
		}
		proto::common::tower_trigger* p_single = p_data->add_self_trigger();
		p_trigger->peek_data(p_single);
	}
}


void tower_t::send_all_self_trigger(uint64_t friend_uid)
{
	proto::server::ge_tower_trigger_notify ntf;
	uint32_t now_time = time_util_t::now_time();

	for (auto it : m_self_trigger)
	{
		tower_trigger_ptr p_trigger = it.second;
		if (NULL == p_trigger)
		{
			continue;
		}
		if (now_time >= p_trigger->get_expired_time())
		{
			continue;
		}
		proto::common::tower_trigger* p_single = ntf.add_tower_trigger();
		p_trigger->peek_data(p_single);
	}
	ntf.set_friend_uid(friend_uid);
	env::server->send_msg_to_center(op_cmd::ge_tower_trigger_notify, m_owner, ntf);
}


tower_trigger_ptr tower_t::get_trigger(uint64_t trigger_uid)
{
	auto it = m_self_trigger.find(trigger_uid);
	if (it != m_self_trigger.end())
	{
		return it->second;
	}
	it = m_other_trigger.find(trigger_uid);
	if (it != m_other_trigger.end())
	{
		return it->second;
	}
	return tower_trigger_ptr(NULL);
}


void tower_t::del_trigger(uint64_t trigger_uid)
{
	auto it = m_self_trigger.find(trigger_uid);
	if (it != m_self_trigger.end())
	{
		m_self_trigger.erase(it);
		return;
	}
	it = m_other_trigger.find(trigger_uid);
	if (it != m_other_trigger.end())
	{
		m_other_trigger.erase(it);
		return;
	}
}

uint32_t tower_t::get_max_trigger_count()
{
	return 3;
}

uint32_t tower_t::get_max_trigger_fight_count()
{
	return 3;
}

uint32_t tower_t::get_double_time_id()
{
	auto double_time_id = GET_CONF(Comprehensive, comprehensive_common::dreamland_hide);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(double_time_id);
	return val;
}

uint32_t tower_t::get_double_value()
{
	auto double_value = GET_CONF(Comprehensive, comprehensive_common::dreamland_hide_probability_up);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(double_value);
	return val;
}

void tower_t::send_auto_fight_over_mail(uint64_t user_id)
{
	auto p_global_conf = GET_CONF(Comprehensive, comprehensive_common::dreamland_tower_sweep);
	uint32_t mail_id = GET_COMPREHENSIVE_VALUE_1(p_global_conf);

	//发送邮件	
	mail_manager_t::send_mail(user_id, proto::common::MAIL_TYPE_SYS, mail_id, proto::common::MAIL_GET_TYPE_TOWER_OVER);

	role_ptr p_role = role_manager_t::find_role( user_id );
	if (NULL == p_role)
	{
		log_error("user:[%lu] p_role is NULL", user_id);
		return;
	}

	p_role->clear_tower_timer();
}

void tower_t::set_auto_fight_timer()
{
	uint32_t auto_fight_time = get_auto_fight_time();
	uint32_t now_time = time_util_t::now_time();
	if (auto_fight_time > 0 && now_time >= auto_fight_time  && m_send_mail_flag == 1)
	{
		m_send_mail_flag = 0;
		save_self();
		tower_t::send_auto_fight_over_mail(m_owner);
		return;
	}

	if (now_time < auto_fight_time)
	{
		if (NULL == m_af_over_timer)
		{
			m_af_over_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
		}
		else 
		{
			m_af_over_timer->cancel();
		}
		m_af_over_timer->expires_from_now(boost::posix_time::seconds((auto_fight_time - now_time) + 2)); // 加一个延迟时间
		m_af_over_timer->async_wait(boost::bind(&tower_t::on_auto_fight_over_timer, m_owner, boost::asio::placeholders::error));
		m_send_mail_flag = 1;
		save_self();
		return;
	}
}

void tower_t::on_auto_fight_over_timer(uint64_t user_id, const boost::system::error_code& ec)
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

	tower_t::send_auto_fight_over_mail(user_id);
}

void tower_t::del_auto_fight_timer() 
{
	if (m_af_over_timer) 
	{
		m_af_over_timer->cancel();
		m_af_over_timer.reset();
	}

	m_send_mail_flag = 0;
	save_self();
}



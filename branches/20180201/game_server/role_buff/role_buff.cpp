#include "role_buff.hpp"
#include "main/game_server.hpp"
#include "role/role_manager.hpp"
#include "tblh/RoleBuff.tbls.h"
#include "config_mgr.h"
#include "hero/hero_manager.hpp"

role_buff_t::role_buff_t(uint64_t owner)
	:m_owner(owner)
{

}

void role_buff_t::load_data(const proto::common::role_buff_single& buff_single)
{
	m_tid = buff_single.buff_id();
	uint32_t remain_time = buff_single.remain_time();
	do_effect(remain_time, false);
	
}

void role_buff_t::peek_data(proto::common::role_buff_single* p_buff_single)
{
	if (NULL == p_buff_single)
	{
		return;
	}
	p_buff_single->set_buff_id(m_tid);
	p_buff_single->set_remain_time(time_util_t::get_remain_time(m_timer));
}

void role_buff_t::do_effect(uint32_t remain_time, bool notify)
{
	if (NULL == m_timer)
	{
		m_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	}
	m_timer->expires_from_now(boost::posix_time::seconds(remain_time));
	m_timer->async_wait(boost::bind(&role_manager_t::on_del_buff, m_owner, m_tid, boost::asio::placeholders::error));
	RoleBuff* p_conf = GET_CONF(RoleBuff, m_tid);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", m_tid);
		return;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	switch (p_conf->type())
	{
	case role_buff_type_addspeed:
		{
			p_role->set_buff_speed(p_role->get_buff_speed() + p_conf->get_default_param());
			p_role->change_real_speed(notify ? role_t::speed_forced_update : role_t::speed_only_set);
			ROLE_BUFF_LOG("role[%lu] buff[%d] add_speed[%d]", m_owner, m_tid, p_conf->get_default_param());
		}
		break;
	case role_buff_type_addattr:
		{
			const param_vec& param_list = p_conf->get_all_param();
			std::vector<hero_ptr> hero_list;
			hero_ptr p_hero = NULL;
			hero_manager_t::get_all_heroes_include_hide(p_role, hero_list);
			for (uint32_t i = 0; i < hero_list.size(); ++i)
			{
				p_hero = hero_list[i];
				if (NULL == p_hero)
				{
					log_error("p_hero NULL");
					continue;
				}
				for (uint32_t x = 0; x < param_list.size(); ++x)
				{
					if (param_list[x].size() < 2)
					{
						log_error("rolebuff table param error id[%d]", m_tid);
						continue;
					}
					p_hero->set_add_attr(param_list[x][0], param_list[x][1]);
				}
			}
		}
		break;
	default:
		break;
	}
}

void role_buff_t::lose_effect(bool notify)
{
	if (NULL != m_timer)
	{
		m_timer->cancel();
	}
	auto p_conf = GET_CONF(RoleBuff, m_tid);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", m_tid);
		return;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	switch (p_conf->type())
	{
	case role_buff_type_addspeed:
		{
			if (p_role->get_buff_speed() > p_conf->get_default_param())
			{
				p_role->set_buff_speed(p_role->get_buff_speed() - p_conf->get_default_param());
			}
			else
			{
				p_role->set_buff_speed(0);
			}
			p_role->change_real_speed(notify ? role_t::speed_forced_update : role_t::speed_only_set);
			ROLE_BUFF_LOG("role[%lu] buff[%d] del_speed[%d]", m_owner, m_tid, p_conf->get_default_param());
		}
		break;
	case role_buff_type_addattr:
		{
			std::vector<hero_ptr> hero_list;
			hero_ptr p_hero = NULL;
			hero_manager_t::get_all_heroes_include_hide(p_role, hero_list);
			for (uint32_t i = 0; i < hero_list.size(); ++i)
			{
				p_hero = hero_list[i];
				if (NULL == p_hero)
				{
					log_error("p_hero NULL");
					continue;
				}
				p_hero->reset_add_attr();
			}
			proto::client::gc_role_change_data_notify ntf;
			hero_manager_t::peek_all_datas(p_role, ntf.mutable_change_data()->mutable_hero());
			p_role->send_msg_to_client(op_cmd::gc_role_change_data_notify, ntf);
		}
		break;
	default:
		break;
	}
}

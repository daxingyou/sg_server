#include "interior.hpp"
#include "utility.hpp"
#include "config/config_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "role/role.hpp"
#include "log_enum.hpp"
#include "item/item_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"
#include "role/money_manager.hpp"
USING_NS_COMMON;
interior_t::interior_t(uint64_t uid)
	: m_owner(uid)
{

}

interior_t::~interior_t()
{

}


void interior_t::init_new_data()
{
	m_level = 1;
	m_left_times = INTERIOR_TASK_MAX_COUNT;
	random_task();
}

void interior_t::load_data(const proto::common::interior_info& info)
{
    m_level = info.level();
    m_task_id = info.task_id();
    m_star_level = info.star_level();
	m_start_time = info.start_time();
	m_span_time = info.span_time();
	m_last_reward_time = info.last_reward_time();
	m_left_times = info.left_times();
	m_is_start = info.is_start();
	m_hero_id_list.clear();
	for (uint16_t i = 0; i < info.hero_id_list_size(); ++i)
	{
		m_hero_id_list.push_back(info.hero_id_list(i));
	}
}

void interior_t::peek_data(proto::common::interior_info* p_interior_info)
{
	p_interior_info->set_level(m_level);
	p_interior_info->set_task_id(m_task_id);
	p_interior_info->set_star_level(m_star_level);
	p_interior_info->set_start_time(m_start_time);
	p_interior_info->set_span_time(m_span_time);
	p_interior_info->set_last_reward_time(m_last_reward_time);
	p_interior_info->set_copper_left_time(get_copper_left_time());
	// p_interior_info->set_copper_count(get_copper());
	p_interior_info->set_task_left_time(get_task_left_time());
	p_interior_info->set_left_times(m_left_times);
	p_interior_info->set_is_start(m_is_start);
	for (uint16_t i = 0; i < m_hero_id_list.size(); ++i)
	{
		p_interior_info->add_hero_id_list(m_hero_id_list[i]);
	}
}

uint32_t interior_t::get_copper()
{
	uint32_t now = common::time_util_t::now_time();
	int32_t minute = (now - m_last_reward_time) / 60;
	if (minute <= 0)
	{
		return 0;
	}
	if (minute >= 1)
	{
		uint32_t second = (now - m_last_reward_time) % 60;

		m_last_reward_time = now - second;
	}
	if (minute > 20)
	{
		minute = 20;
	}
	uint32_t product_copper_speed = 0;
	uint32_t reward_copper = 0;
	BuildConf* p_config = GET_CONFIG_SPECIAL(Build, m_level, false, BuildConf);
	if (NULL != p_config)
	{
		product_copper_speed = p_config->_build.make_money();
		reward_copper = minute * product_copper_speed;
	}
	return reward_copper;
}

uint32_t interior_t::get_task_left_time()
{
	uint32_t task_left_time = 0;
	if (0 != m_task_id)
	{
		uint32_t now = common::time_util_t::now_time();
		task_left_time = now - m_start_time;
		task_left_time = task_left_time >= m_span_time ? 0 : m_span_time - task_left_time;
	}
	return task_left_time;

}
uint32_t interior_t::get_copper_left_time()
{
	uint32_t now = common::time_util_t::now_time();
	uint32_t copper_left_time = now - m_last_reward_time;
	copper_left_time = copper_left_time >= 20 * 60 ? 0 : 20 * 60 - copper_left_time;
	return copper_left_time;
}

uint32_t interior_t::random_task()
{
	auto confs = ConfMgr<Hades::Data::Interior, uint32_t, false, InteriorConf>::GetSingleton().GetAllConfs();
	if (!confs.empty())
	{
		int32_t rand_val = random_util_t::randBetween(0, (int32_t)confs.size() - 1);
		if (rand_val == -1)
		{
			log_error("Interior config rand from 0 to [%d] error", (int32_t)confs_vec.size() - 1);
			return;
		}

		auto it = confs.begin();
		for (uint16_t i = 0; i < rand_val; i++)
		{
			++it;
		}
		if (it != confs.end())
		{
			m_task_id = it->first;
			m_is_start = TASK_STATE_NONE;
			m_span_time = it->second._interior.complete_time() * 60;
			rand_val = random_util_t::randBetween(1, INTERIOR_TASK_STAR_LEVEL_MAX);
			if (rand_val == -1)
			{
				log_error("Interior config rand from 1 to [%d] error", (int32_t)INTERIOR_TASK_STAR_LEVEL_MAX);
				return;
			}
			m_star_level = rand_val;
			m_hero_id_list.clear();
		}
	}
	return m_task_id;
}

bool interior_t::upgrade_task_star_level()
{
	if (!m_is_start)
	{
		if (m_star_level < INTERIOR_TASK_STAR_LEVEL_MAX)
		{
			++m_star_level;
			return true;
		}
	}
	return false;
}

bool interior_t::complete_interior_task()
{
	if (m_is_start == TASK_STATE_COMPLETE)
	{
		return false;
	}
	m_is_start = TASK_STATE_COMPLETE;
	if (m_task_timer != NULL)
	{
		m_task_timer->cancel();
	}
    return true;
}

void interior_t::start_task()
{
	m_start_time = common::time_util_t::now_time();
	m_is_start = TASK_STATE_START;
	start_task_timer();
}

void interior_t::start_task_timer()
{
	if (m_task_id == 0)
	{ 
		return;
	}
    if (0 == m_span_time)
    {
        log_warn("m_span_time is 0");
        return;
    }
	//m_task_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
	//m_task_timer->expires_from_now(boost::posix_time::seconds(m_span_time));
	//m_task_timer->async_wait(boost::bind(&interior_t::task_finished_callback, this, boost::asio::placeholders::error));
}


void interior_t::task_finished_callback(const boost::system::error_code& ec)
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

	//owner->NotifyInteriorTaskFinished();
	m_is_start = TASK_STATE_COMPLETE;
	if (0 == m_owner)
	{
		return;
	}

	auto pRole = role_manager_t::find_role(m_owner);
	if (NULL == pRole)
	{
		log_error("null role");
		return;
	}

	proto::client::gc_interior_task_finished_notify ntf;
	ntf.set_task_id(m_task_id);
	pRole->send_msg_to_client(op_cmd::gc_interior_task_finished_notify, ntf);
}


void interior_t::notify_info_to_client()
{
	if (0 == m_owner)
	{
		return;
	}

	auto pRole = role_manager_t::find_role(m_owner);
	if (NULL == pRole)
	{
		log_error("null role");
		return;
	}

	proto::client::gc_interior_info_notify ntf;
	peek_data(ntf.mutable_interior());
	pRole->send_msg_to_client(op_cmd::gc_interior_info_notify, ntf);
}

bool interior_t::get_task_reward(proto::common::role_change_data* p_data)
{
	if (0 == m_owner)
	{
		return false;
	}

	auto pRole = role_manager_t::find_role(m_owner);
	if (NULL == pRole)
	{
		log_error("null role");
		return false;
	}

	auto build_conf = GET_CONFIG_SPECIAL(Build, m_level, false, BuildConf);
	auto task_conf = GET_CONFIG_SPECIAL(Interior, m_task_id, false, InteriorConf);
	if (NULL != build_conf && NULL != task_conf)
	{
		for (auto ptr : task_conf->item)
		{
			item_manager_t::add_item(m_owner, ptr.first, ptr.second, 
									 log_enum::source_type_interior_task_reward, m_task_id, p_data);
		}
		for (auto ptr : task_conf->coin)
		{
			money_manager_t::add_money(pRole, (proto::common::MONEY_TYPE)ptr.first, ptr.second * build_conf->_build.task_reward_coefficient(),
							  log_enum::source_type_interior_task_reward, m_task_id);
		}
	}

	m_is_start = 0;
	random_task();
	return true;
}


#include "time_manager.hpp"
#include "boost/bind.hpp"
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include "tblh/TimeTable.tbls.h"
#include "config_mgr.h"
#include "utility.hpp"
#include "tblh/time_type_enum.hpp"

boost::shared_ptr<boost::asio::deadline_timer> time_manager_t::m_timer = NULL;
boost::shared_ptr<boost::asio::io_service::strand> time_manager_t::m_strand = NULL;
bool time_manager_t::m_timer_cancel = false;
std::vector<uint32_t> time_manager_t::m_server_start_date_vec;
// std::vector<uint32_t> time_manager_t::m_server_start_time_vec;
time_manager_t::time_item_map time_manager_t::m_time_item_map;

bool time_manager_t::init(boost::asio::io_service& ios, std::string server_start_time)
{
    m_timer_cancel = false;

    m_timer.reset(new boost::asio::deadline_timer(ios));
    m_strand.reset(new boost::asio::io_service::strand(ios));
	m_timer->expires_from_now(boost::posix_time::millisec(1000));
    //m_timer->async_wait(boost::bind(&time_manager_t::on_timer, boost::asio::placeholders::error));
    m_timer->async_wait(m_strand->wrap(boost::bind(&time_manager_t::on_timer, boost::asio::placeholders::error)));

//     std::vector<std::string> vec;
//     common::string_util_t::split<std::string>(server_start_time, vec, " ");
//     if( vec.size() != 2)
//     {
//         log_error("invalid server start time");
//         return false;
//     }
    
    common::string_util_t::ParseIds(server_start_time, m_server_start_date_vec, '-');
//    common::string_util_t::ParseIds(vec[1], m_server_start_time_vec, ':');

    if(m_server_start_date_vec.size() != 3 /*|| m_server_start_time_vec.size() != 3*/)
    {
        log_error("invalid sever start date or time");
        return false;
    }

    std::map<uint32_t, TimeTable*> all_confs;
    GET_ALL_CONF(TimeTable, all_confs);
    for(std::map<uint32_t, TimeTable*>::iterator itr = all_confs.begin();
		itr != all_confs.end(); ++itr)
    {
		TimeTable* p_conf = itr->second;
		if (p_conf == NULL)
		{
			log_error("TimeTable[%u] null error", itr->first);
			return false;
		}

		timer_item_ptr p_timer_item = NULL;

		switch (p_conf->time_type())
		{
		case time_type_every_weekday:
			{
				p_timer_item.reset(new time_week_t());
			}
			break;
		case time_type_every_day:
			{
				p_timer_item.reset(new time_day_t());
			}
			break;
		case time_type_fixed_day:
		case time_type_from_server_open:
			{
				p_timer_item.reset(new time_once_t());
			}
			break;
		case time_type_last_day_of_month:
			{
				p_timer_item.reset(new time_month_last_day_t());
			}
			break;
		case time_type_first_day_of_month:
			{
				p_timer_item.reset(new time_month_first_day_t());
			}
			break;
		default:
			break;
		}

		if (NULL == p_timer_item)
		{
			log_error("p_timer_item null error");
			return false;
		}

		if (!p_timer_item->init(p_conf))
		{
			log_error("TimeTable[%u] init error", itr->first);
			return false;
		}

		/* add by hy 过期的时间id也必须存在
		if (p_timer_item->get_left_times() == 0) {
			// log_info("time_manager id[%u] is outdated!", itr->first);
			continue;
		}
		*/
		m_time_item_map.insert(std::make_pair(itr->first, p_timer_item));
    }

    return true;
}


bool time_manager_t::is_in_time(uint32_t id)
{
	time_item_map::const_iterator citr = m_time_item_map.find(id);
	if (citr != m_time_item_map.end())
	{
		const timer_item_ptr& p_timer_item = citr->second;
		if (p_timer_item == NULL)
		{
			log_error("p_timer_item null error");
			return false;
		}

		return p_timer_item->in_time();
	}

	return false;
}

uint32_t time_manager_t::register_func(uint32_t id, on_timer_callback func)
{
	time_item_map::iterator itr = m_time_item_map.find(id);
	if (itr != m_time_item_map.end())
	{
		timer_item_ptr p_timer_item = itr->second;
		if (p_timer_item == NULL)
		{
			log_error("p_timer_item null error");
			return 0;
		}

		return p_timer_item->register_func(func);
	}

	return 0;
}


void time_manager_t::unregister(uint32_t id, uint32_t key)
{
	time_item_map::iterator itr = m_time_item_map.find(id);
	if (itr != m_time_item_map.end())
	{
		timer_item_ptr p_timer_item = itr->second;
		if (p_timer_item != NULL)
		{
			p_timer_item->unregister(key);
		}
	}
}


void time_manager_t::unregister_all(uint32_t id)
{
	time_item_map::iterator itr = m_time_item_map.find(id);
	if (itr != m_time_item_map.end())
	{
		timer_item_ptr p_timer_item = itr->second;
		if (p_timer_item != NULL)
		{
			p_timer_item->unregister_all();
		}
	}
}


bool time_manager_t::is_timer_activating(uint32_t id)
{
	time_item_map::const_iterator citr = m_time_item_map.find(id);

	if (citr == m_time_item_map.end()) return false;
	if (citr->second == NULL) return false;
	return citr->second->get_left_times() > 0;
}

void time_manager_t::on_timer(const boost::system::error_code& ec)
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

	if (m_timer_cancel)
	{
		log_error("Timer was canceled");
		return;
	}

	uint32_t curr_time = common::time_util_t::now_time();
	for (time_item_map::const_iterator citr = m_time_item_map.begin(); citr != m_time_item_map.end(); ++citr)
	{
		if (NULL != citr->second)
		{
			citr->second->timer(curr_time);
		}
	}

    if (NULL != m_timer)
    {
        m_timer->expires_from_now(boost::posix_time::millisec(1000));
        m_timer->async_wait(boost::bind(&time_manager_t::on_timer, boost::asio::placeholders::error));
    }
}

void time_manager_t::cancel_timer()
{
    if(NULL != m_timer)
    {
        m_timer_cancel = true;
		m_time_item_map.clear();
        m_timer->cancel();
    }
}

bool time_manager_t::is_same_period(uint32_t id, uint32_t time_val)
{
	time_item_map::const_iterator citr = m_time_item_map.find(id);
	if (citr != m_time_item_map.end())
	{
		const timer_item_ptr& p_timer_item = citr->second;
		if (p_timer_item != NULL)
		{
			return time_val > p_timer_item->start_time() && time_val < p_timer_item->end_time();
		}
	}
	return false;
}


bool time_manager_t::check_previous_day(uint32_t curr_time, uint32_t check_time)
{
	time_item_map::const_iterator citr = m_time_item_map.find(common::time_type_enum::time_one_day);
	if (citr != m_time_item_map.end()) {
		if (citr->second != NULL) {
			return citr->second->check_prev_period(curr_time, check_time);
		}
	}

	return false;
}

bool time_manager_t::check_same_period(uint32_t id, uint32_t time_val)
{
	time_item_map::const_iterator citr = m_time_item_map.find(id);
	if (citr != m_time_item_map.end()) {
		const timer_item_ptr& p_timer_item = citr->second;
		if (p_timer_item != NULL) {
			return p_timer_item->check_same_period(time_val);
		}
	}
	return false;
}

std::pair<uint32_t, uint32_t> time_manager_t::get_refresh_time(uint32_t id)
{
	time_item_map::const_iterator citr = m_time_item_map.find(id);
	if (citr != m_time_item_map.end())
	{
		const timer_item_ptr& p_timer_item = citr->second;
		if (p_timer_item != NULL)
		{
			return std::pair<uint32_t, uint32_t>(p_timer_item->start_time(), p_timer_item->end_time());
		}
	}

	log_error("time table id[%u] get_refresh_time error", id);
	return std::pair<uint32_t, uint32_t>(0, 0);
}

uint32_t time_manager_t::get_next_refresh_time(uint32_t id)
{
	time_item_map::const_iterator citr = m_time_item_map.find(id);
	if (citr != m_time_item_map.end())
	{
		const timer_item_ptr& p_timer_item = citr->second;
		if (p_timer_item != NULL)
		{
			//return p_timer_item->end_time();
			uint32_t curr = common::time_util_t::now_time();
			if (p_timer_item->start_time() <= curr && curr < p_timer_item->end_time()) return p_timer_item->next_time();
			else return p_timer_item->start_time();
		}
	}

	log_error("time table id[%u] get_next_refresh_time error", id);
	return 0;
}


uint32_t time_manager_t::get_timer_end_time(uint32_t time_id)
{
	time_item_map::const_iterator citr = m_time_item_map.find( time_id );
	if (citr != m_time_item_map.end())
	{
		const timer_item_ptr& p_timer_item = citr->second;
		if (p_timer_item != NULL)
		{
			return p_timer_item->end_time();
		}
	}
	return 0;
}

//获取开始时间
uint32_t time_manager_t::get_timer_start_time(uint32_t time_id)
{
	time_item_map::const_iterator citr = m_time_item_map.find( time_id );
	if (citr != m_time_item_map.end())
	{
		const timer_item_ptr& p_timer_item = citr->second;
		if (p_timer_item != NULL)
		{
			return p_timer_item->start_time();
		}
	}
	return 0;
}

uint32_t time_manager_t::get_time_unique_id(uint32_t time_id, uint32_t check_time)
{
	time_item_map::const_iterator citr = m_time_item_map.find(time_id);
	if (citr != m_time_item_map.end())
	{
		const timer_item_ptr& p_timer_item = citr->second;
		if (p_timer_item != NULL)
		{
			return p_timer_item->get_time_unique_id(check_time);
		}
	}
	return 0;
}

uint32_t time_manager_t::get_time_by_unique_id(uint32_t time_id, uint32_t unique_id)
{
	time_item_map::const_iterator citr = m_time_item_map.find(time_id);
	if (citr != m_time_item_map.end())
	{
		const timer_item_ptr& p_timer_item = citr->second;
		if (p_timer_item != NULL)
		{
			return p_timer_item->get_time_by_unique_id(unique_id);
		}
	}
	return 0;
}


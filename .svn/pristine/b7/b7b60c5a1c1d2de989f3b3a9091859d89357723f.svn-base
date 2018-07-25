#include "collect_manager.hpp"
#include "config/config_manager.hpp"
#include "global_id.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role.hpp"
#include "task/task_manager.hpp"


std::map<uint64_t, collect_point_ptr> collect_manager_t::m_collect_points;

collect_manager_t::collect_manager_t()
{

}

collect_manager_t::~collect_manager_t()
{
	m_collect_points.clear();
}

uint32_t collect_manager_t::start_collect(role_ptr p_role, uint64_t collect_point_uid)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return errcode_enum::notice_unknown;
	}
	collect_point_ptr p_collect_point = find_collect_point(collect_point_uid);
	if (NULL == p_collect_point)
	{
		log_error("collect p_collect_point NULL uid[%lu]", collect_point_uid);
		return errcode_enum::notice_unknown;
	}
	uint32_t reply_code = p_collect_point->can_collect(p_role);
	if (reply_code != errcode_enum::error_ok)
		return reply_code;

	reply_code = p_collect_point->start_collect(p_role);
	p_role->change_object_state(proto::common::object_state_busy);

	return reply_code;
}

uint32_t collect_manager_t::end_collect(role_ptr p_role, uint64_t collect_point_uid, proto::common::role_change_data *p_data)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return errcode_enum::notice_unknown;
	}
	collect_point_ptr p_collect_point = find_collect_point(collect_point_uid);
	if (NULL == p_collect_point)
	{
		log_error("collect p_collect_point NULL uid[%lu]", collect_point_uid);
		return errcode_enum::notice_unknown;
	}

	uint32_t reply_code = p_collect_point->end_collect(p_role, p_data);
	p_role->change_object_state(proto::common::object_state_normal);
	if (reply_code == errcode_enum::error_ok)
	{
		auto p_task_mgr = p_role->get_task_mgr();
		if (NULL != p_task_mgr)
		{
			proto::common::role_change_data rcd;
			p_task_mgr->try_change_task_state(TASK_TARGET_TYPE_COLLECT, rcd, p_collect_point->get_tid(), 1);
		}
	}
	return reply_code;
}

uint32_t collect_manager_t::cancel_collect(role_ptr p_role, uint64_t collect_point_uid)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return errcode_enum::notice_unknown;
	}
	collect_point_ptr p_collect_point = find_collect_point(collect_point_uid);
	if (NULL == p_collect_point)
	{
		log_error("collect p_collect_point NULL uid[%lu]", collect_point_uid);
		return errcode_enum::notice_unknown;
	}

	p_collect_point->cancel_collect(p_role);
	p_role->change_object_state(proto::common::object_state_normal);
	return errcode_enum::error_ok;
}

void collect_manager_t::close()
{
	for (auto pair : m_collect_points)
	{
		if(pair.second == NULL)
			continue;
		pair.second->close();
	}
}

collect_point_ptr collect_manager_t::find_collect_point(uint64_t collect_point_id)
{
	auto itr = m_collect_points.find(collect_point_id);
	if (itr != m_collect_points.end())
	{
		return itr->second;
	}
	return NULL;
}

void collect_manager_t::delete_collect_point(uint64_t creature_id)
{
	auto itr = m_collect_points.find(creature_id);
	if (itr != m_collect_points.end())
	{
		// 有定时器需要在删除前cancel掉
		collect_point_ptr p_collect = itr->second;
		if (NULL != p_collect)
		{
			p_collect->del_clear();
		}

		m_collect_points.erase(itr);
	}
}

bool collect_manager_t::add_collect_point(collect_point_ptr p_collect_point)
{
	if (p_collect_point == NULL)
	{
		log_error("collect_point null error");
		return false;
	}

	std::pair<std::map<uint64_t, collect_point_ptr>::iterator, bool> ret = m_collect_points.insert(std::make_pair(p_collect_point->get_object_id(), p_collect_point));
	if (!ret.second)
	{
		log_error("collect_point[%lu] insert error, it is exist", p_collect_point->get_object_id());
		return false;
	}

	return true;
}

uint64_t collect_manager_t::get_collect_point_uid()
{
	uint64_t uid = common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_COLLECT_POINT);
	return uid;
}

void collect_manager_t::refresh_on_time(const boost::system::error_code& ec, uint64_t collect_point_uid)
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

	collect_point_ptr p_collect_point = find_collect_point(collect_point_uid);
	if (NULL == p_collect_point)
	{
		log_error("collect_manager_t p_collect_point NULL uid[%lu]", collect_point_uid);
		return;
	}

	p_collect_point->relive();
}

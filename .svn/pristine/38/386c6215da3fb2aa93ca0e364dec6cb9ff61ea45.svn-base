#include "red_point_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "tblh/Liveness.tbls.h"
#include "tblh/Activities.tbls.h"
#include "config_mgr.h"
#include "role/money_manager.hpp"

USING_NS_COMMON;

red_point_map_ptr red_point_manager_t::get_role_red_point_list(uint64_t role_uid)
{
	auto iter_list = m_all_role_red_point_list.find(role_uid);
	if (iter_list != m_all_role_red_point_list.end())
	{
		return iter_list->second;
	}
	return red_point_map_ptr();
}

bool red_point_manager_t::is_exist(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	red_point_map_ptr p_red_point_list = get_role_red_point_list(role_uid);
	if (NULL == p_red_point_list)
	{
		RED_POINT_LOG("role_uid[%lu] not exist red point", role_uid);
		return false;
	}

	auto iter_red = p_red_point_list->find(red_point_type);
	if (iter_red == p_red_point_list->end())
	{
		RED_POINT_LOG("role[%lu] no exist red_point_type[%d] ", role_uid, red_point_type);
		return false;
	}
		
	return true;
}

bool red_point_manager_t::is_exist(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	red_point_map_ptr p_red_point_list = get_role_red_point_list(role_uid);
	if (NULL == p_red_point_list)
	{
		RED_POINT_LOG("role_uid[%lu] not exist  red point", role_uid);
		return false;
	}
	
	return true;
}

void red_point_manager_t::add_red_point(uint64_t role_uid, red_point_ptr p_red_point)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	if (NULL == p_red_point)
	{
		log_error("NULL == p_red_point role_uid[%lu]", role_uid);
		return;
	}

	bool b_result = is_exist(role_uid);
	if (b_result)
	{
		red_point_map_ptr p_red_point_list = get_role_red_point_list(role_uid);
		if (NULL == p_red_point_list)
		{
			RED_POINT_LOG("role_uid[%lu] not exist red point", role_uid);
			return;
		}

		auto iter_red = p_red_point_list->find(p_red_point->get_type());
		if (iter_red == p_red_point_list->end())
		{
			p_red_point_list->insert(std::make_pair(p_red_point->get_type(), p_red_point));
			RED_POINT_LOG("role_uid[%lu] add red point[%d] to list ", role_uid, p_red_point->get_type());
		}
		else
		{
			red_point_ptr p_red = iter_red->second;
			if (NULL != p_red)
			{
				p_red->append_to_list(p_red_point->get_list());
			}
			else
			{
				p_red_point_list->insert(std::make_pair(p_red_point->get_type(), p_red_point));
				RED_POINT_LOG("role_uid[%lu] add red point[%d] to list ", role_uid, p_red_point->get_type());
			}
		}
	}
	else
	{
		red_point_map_ptr p_red_point_list(new red_point_map());
		p_red_point_list->insert(std::make_pair(p_red_point->get_type(), p_red_point));
		m_all_role_red_point_list.insert(std::make_pair(role_uid, p_red_point_list));
		RED_POINT_LOG("role_uid[%lu] create and add red point[%d] to list ", role_uid, p_red_point->get_type());
	}
}

void red_point_manager_t::remove_role_all_red_point(uint64_t role_uid)
{
	m_all_role_red_point_list.erase(role_uid);
}

void red_point_manager_t::remove_role_type_red_point(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	if (!is_exist(role_uid, red_point_type))
	{
		log_error("role_uid[%lu] not [%d]type red point",role_uid,red_point_type);
		return;
	}

	red_point_map_ptr p_red_point_list = get_role_red_point_list(role_uid);
	if (NULL == p_red_point_list)
	{
		RED_POINT_LOG("role_uid[%lu] not exist red point", role_uid);
		return;
	}
	p_red_point_list->erase(red_point_type);
}

red_point_ptr red_point_manager_t::get_red_point(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return red_point_ptr();
	}

	red_point_map_ptr p_red_point_list = get_role_red_point_list(role_uid);
	if (NULL == p_red_point_list)
	{
		RED_POINT_LOG("role_uid[%lu] not exist red point", role_uid);
		return red_point_ptr();
	}
	
	auto iter_red = p_red_point_list->find(red_point_type);
	if (iter_red == p_red_point_list->end())
	{
		RED_POINT_LOG("role[%lu] no exist red_point_type[%d] ", role_uid, red_point_type);
		return red_point_ptr();
	}
	return iter_red->second;
}

bool red_point_manager_t::send_notify_to_client(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type,std::string& dec)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	proto::client::gc_red_point_notify notify;
	proto::common::red_point_data* p_red_point_data = notify.mutable_red_info_list();
	proto::common::red_point_single* p_red_point_single = p_red_point_data->add_red_info_list();
	p_red_point_single->set_red_type(red_point_type);
	p_red_point_single->set_red_point_state(1);
	p_red_point_single->set_red_point_send_list(dec.c_str());
	p_role->send_msg_to_client(op_cmd::gc_red_point_notify, notify);
	return true;
}

bool red_point_manager_t::send_notify_to_client(uint64_t role_uid, proto::client::gc_red_point_notify& notify)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}
	if (!notify.has_red_info_list())
	{
		log_error("notify is empty!");
		return false;
	}
	p_role->send_msg_to_client(op_cmd::gc_red_point_notify, notify);
	return true;
}

void red_point_manager_t::red_point_notify(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type, uint32_t param)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}
	RED_POINT_LOG("role_uid[%lu] red point type[%d] param[%d]", role_uid, red_point_type, param);

	std::string dec = "";
	if (red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_MAIL &&
		red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_LIVENESS_GET_TREASURE &&
		red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_PRIVATE_CHAT &&
		red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_PAGE &&
		red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_PAGE_ELITE)
	{
		red_point_ptr p_red_point = red_point_manager_t::get_red_point(role_uid, red_point_type);
		if (NULL == p_red_point)
		{
			red_point_ptr p_red = red_point_ptr(new red_point_t(red_point_type, role_uid));

			p_red->append_to_list(param);
			red_point_manager_t::add_red_point(role_uid, p_red);
			dec = p_red->get_list();
			RED_POINT_LOG("role_uid[%lu] add red_point type[%d] dec[%s]", role_uid, red_point_type, dec.c_str());
		}
		else
		{
			p_red_point->append_to_list(param);
			dec = p_red_point->get_list();
			RED_POINT_LOG("role_uid[%lu] red_point[%d] dec[%s]", role_uid, red_point_type, dec.c_str());
		}
	}
	RED_POINT_LOG("role_uid[%lu] red point type[%d] param dec[%s]", role_uid, red_point_type, dec.c_str());
	if (!send_notify_to_client(role_uid, red_point_type, dec))
	{
		log_error("role_uid[%lu] send red point[%d] to client fail", role_uid, red_point_type);
		return ;
	}
}

void red_point_manager_t::red_point_notify(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type, std::set<uint32_t>& param_list)
{
	//TODO

	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}
	RED_POINT_LOG("role_uid[%lu] red point type[%d] param size[%d]", role_uid, red_point_type, (int32_t)param_list.size());

	std::string dec = "";
	if (red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_MAIL &&
		red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_LIVENESS_GET_TREASURE &&
		red_point_type != proto::common::RED_POINT_TYPE::RED_POINT_PRIVATE_CHAT)
	{
		red_point_ptr p_red_point = red_point_manager_t::get_red_point(role_uid, red_point_type);
		if (NULL == p_red_point)
		{
			red_point_ptr p_red = red_point_ptr(new red_point_t(red_point_type, role_uid));

			for (auto& it : param_list)
			{
				p_red->append_to_list(it);
			}
			
			red_point_manager_t::add_red_point(role_uid, p_red);
			dec = p_red->get_list();
			RED_POINT_LOG("role_uid[%lu] add red_point type[%d] dec[%s]", role_uid, red_point_type, dec.c_str());
		}
		else
		{
			for (auto& it : param_list)
			{
				p_red_point->append_to_list(it);
			}
			
			dec = p_red_point->get_list();
			RED_POINT_LOG("role_uid[%lu] red_point[%d] dec[%s]", role_uid, red_point_type, dec.c_str());
		}
	}
	RED_POINT_LOG("role_uid[%lu] red point type[%d] param dec[%s]", role_uid, red_point_type, dec.c_str());
	if (!send_notify_to_client(role_uid, red_point_type, dec))
	{
		log_error("role_uid[%lu] send red point[%d] to client fail", role_uid, red_point_type);
		return;
	}
}


void red_point_manager_t::init_red_point(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	red_point_map_ptr p_red_point_list = get_role_red_point_list(role_uid);
	if (NULL == p_red_point_list)
	{
		RED_POINT_LOG("role_uid[%lu] not exist red point", role_uid);
		return;
	}

	proto::client::gc_red_point_notify notify;
	proto::common::red_point_data* p_red_point_data = notify.mutable_red_info_list();
	if (p_red_point_list->empty())
	{
		RED_POINT_LOG("role[%lu] red_point empty",role_uid);
		return;
	}

	for (auto& it : *p_red_point_list)
	{
		red_point_ptr p_red_point = it.second;
		if (NULL == p_red_point)
		{
			RED_POINT_LOG("NULL == p_red_point role_uid[%lu]",role_uid);
			continue;
		}

		if (!p_red_point->get_state())
			continue;
		RED_POINT_LOG("role_uid[%lu] red point type[%d] dec[%s]",role_uid, it.first, p_red_point->get_list().c_str());
		proto::common::red_point_single* p_red_point_single = p_red_point_data->add_red_info_list();
		p_red_point_single->set_red_type((proto::common::RED_POINT_TYPE)it.first);
		p_red_point_single->set_red_point_state(1);
		p_red_point_single->set_red_point_send_list(p_red_point->get_list().c_str());
	}
	//if (check_page_point(role_uid))
	//{
	//	std::string str = "";
	//	proto::common::red_point_single* p_red_point_single = p_red_point_data->add_red_info_list();
	//	p_red_point_single->set_red_type(proto::common::RED_POINT_PAGE);
	//	p_red_point_single->set_red_point_state(1);
	//	p_red_point_single->set_red_point_send_list(str.c_str());
	//}

	if (check_activity_open_box(role_uid,true))
	{
		std::string str = "";
		proto::common::red_point_single* p_red_point_single = p_red_point_data->add_red_info_list();
		p_red_point_single->set_red_type(proto::common::RED_POINT_LIVENESS_GET_TREASURE);
		p_red_point_single->set_red_point_state(1);
		p_red_point_single->set_red_point_send_list(str.c_str());
	}
	send_notify_to_client(role_uid, notify);
}

void red_point_manager_t::check_red_point(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	red_point_map_ptr p_red_point_list = get_role_red_point_list(role_uid);
	if (NULL == p_red_point_list)
	{
		RED_POINT_LOG("role_uid[%lu] not exist red point", role_uid);
		return ;
	}
	for (auto& it : *p_red_point_list)
	{
		red_point_ptr p_red_point = it.second;
		if (NULL == p_red_point)
			continue;

		if (p_red_point->get_list().empty() && !p_red_point->get_state())
			remove_role_type_red_point(role_uid,p_red_point->get_type());
	}
}

bool red_point_manager_t::check_activity_open_box(uint64_t role_uid,bool is_online)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
	if (NULL == p_activity_mgr)
	{
		log_error("NULL == p_activity_mgr  role_uid[%lu]", role_uid);
		return false;
	}
	const uint32_t liveness_value = p_activity_mgr->get_liveness_count();
	std::map<uint32_t, Liveness*> liveness_map;
	GET_ALL_CONF(Liveness, liveness_map);
	for (auto& it : liveness_map)
	{
		if(p_activity_mgr->is_already_open_box(it.first))
			continue;

		if (liveness_value < it.first)
			continue;


		if (is_online)
		{
			return true;
		}
		else
		{
			red_point_notify(role_uid, proto::common::RED_POINT_TYPE::RED_POINT_LIVENESS_GET_TREASURE);
			break;
		}
	}

	return false;
}

void red_point_manager_t::check_activity_join(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	std::map<uint32_t, Activities*> confs;
	GET_ALL_CONF(Activities, confs);
	std::set<uint32_t> activity_id_send_list;
	for (auto& it : confs)
	{
		auto p_activity = it.second;
		if (NULL == p_activity)
			continue;

		if (p_activity->page() != red_point_activity_type::activity_day)
			continue;

		 if (0 < m_activity_join_list.count(it.first))
			 continue;

		 if (p_role->get_level() < p_activity->level())
			 continue;

		 const auto& iter = m_activity_join_list.find(role_uid);
		 if (iter != m_activity_join_list.end())
		 {
			 red_point_activity_join_map_ptr p_red_point = iter->second;
			 if (NULL == p_red_point)
			 {
				 continue;
			 }
			 else if(0 < p_red_point->count(it.first))
			 {
				 continue;
			 }
			 p_red_point->insert(it.first);
		 }
		 else
		 {
			 red_point_activity_join_map_ptr p_red_point_activity_join_list(new red_point_activity_join_set());
			 p_red_point_activity_join_list->insert(it.first);
			 m_activity_join_list.insert(std::make_pair(role_uid, p_red_point_activity_join_list));
		 }

		 activity_id_send_list.insert(it.first);
	}
	if (activity_id_send_list.empty())
		return;
	red_point_notify(role_uid, proto::common::RED_POINT_TYPE::RED_POINT_ACTIVITY_FIRST_ONCE_ENTER, activity_id_send_list);
}

bool red_point_manager_t::check_page_point(uint64_t role_uid)
{
	/*role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}
	std::map<uint32_t, uint32_t>& page_list = p_role->get_page_list();
	std::map<uint32_t, uint32_t>& page_task_list =p_role->get_page_task_map();

	for (auto& it : page_task_list)
	{
		if (1 == it.second)
			continue;
		return true;
	}

	for (auto& it : page_list)
	{
		if (1 == it.second)
			continue;
		return true;
	}

	RED_POINT_LOG("role[%lu] send red_point_type[%d]",role_uid, (uint32_t)proto::common::RED_POINT_TYPE::RED_POINT_PAGE);*/
	return false;
}

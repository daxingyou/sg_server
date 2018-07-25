#include "item_manager.hpp"
#include "main/center_server.hpp"
#include "common/config_mgr.h"
#include "redis_client.hpp"
#include "tblh/DropWorldTable.tbls.h"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"

USING_NS_COMMON;

item_manager_t::item_limit_map item_manager_t::m_item_limit_map;

item_manager_t::item_manager_t()
{

}

item_manager_t::~item_manager_t()
{
	m_item_limit_map.clear();
}

bool item_manager_t::init()
{
	bool bret = false;
	bret &= load_item_limit_from_config();
	bret &= load_item_limit_from_db();
	return true;
}


bool item_manager_t::close()
{
	bool bresult = true;
	item_limit_ptr p_item_limit = item_limit_ptr();
	for (auto iter : m_item_limit_map)
	{
		p_item_limit = iter.second;
		if (NULL == p_item_limit)
		{
			log_error("p_item_limit NULL");
			bresult = false;
			continue;
		}
		p_item_limit->close();
	}
	return bresult;
}

bool item_manager_t::can_give_item(uint32_t item_tid, uint32_t item_num)
{
	// 在限制列表里的判断是否可以给 不在限制里的认为可以给 不统计数量
	item_limit_ptr p_item_limit = get_item_limit(item_tid);
	if (p_item_limit)
	{
		return p_item_limit->can_add(item_num);
	}
	return true;
}


item_limit_ptr item_manager_t::give_item(uint32_t item_tid, uint32_t item_num)
{
	item_limit_ptr p_item_limit = get_item_limit(item_tid);
	if (p_item_limit)
	{
		p_item_limit->add_count(item_num);
		return p_item_limit;
	}	
	return item_limit_ptr();
}


item_limit_ptr item_manager_t::get_item_limit(uint32_t item_tid)
{
	item_limit_map::iterator iter = m_item_limit_map.find(item_tid);
	if (iter != m_item_limit_map.end())
		return iter->second;
	return item_limit_ptr();
}


bool item_manager_t::load_item_limit_from_config()
{
	item_limit_ptr p_item_limit = item_limit_ptr();

	std::map<uint32_t, DropWorldTable*> confs;
	GET_ALL_CONF(DropWorldTable, confs);

	DropWorldTable *p_drop_world = NULL;

	for (auto conf : confs)
	{
		p_drop_world = conf.second;
		if (NULL == p_drop_world)
		{
			log_error("[wys] item_manager p_drop_world NULL id[%d]", conf.first);
			return false;
		}
		p_item_limit.reset(new item_limit_t(conf.first));
		if (NULL == p_item_limit)
		{
			log_error("[wys] item_manager_t new item_limit_t is null");
			return false;
		}
		p_item_limit->init();
		m_item_limit_map.insert(std::make_pair(conf.first, p_item_limit));
	}
	return true;
}

bool item_manager_t::load_item_limit_from_db()
{
	std::vector<uint32_t> item_tids;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint32_t>>("item_limit", item_tids);

	item_limit_ptr p_item_limit = item_limit_ptr();
	for (auto item_tid : item_tids)
	{
		p_item_limit = get_item_limit(item_tid);
		if (NULL == p_item_limit)
		{
			log_warn("[wys] item_manager not find item_limt[%d]", item_tid);
			continue;
		}
		p_item_limit->load_from_db();
	}

	return true;
}

bool item_manager_t::handle_ge_get_item_limit_list(const network::tcp_socket_ptr& s)
{
	proto::server::eg_syn_item_limit_notify msg;
	proto::common::item_limit_data *p_item_limit_single = NULL;
	item_limit_ptr p_item_limit = item_limit_ptr();

	for (auto iter : m_item_limit_map)
	{
		p_item_limit = iter.second;
		if (NULL == p_item_limit)
		{
			log_error("p_item_limit NULL");
			return false;
		}
		p_item_limit_single = msg.add_item_list();
		if (NULL == p_item_limit_single)
		{
			log_error("p_item_limit_single NULL");
			return false;
		}
		
		p_item_limit_single->set_item_tid(p_item_limit->get_item_tid());
		p_item_limit_single->set_item_num(p_item_limit->get_count());
		p_item_limit_single->set_last_give_time(p_item_limit->get_last_give_time());
	}
	env::server->send_msg_to_game(s, op_cmd::eg_syn_item_limit_notify, 0, msg);
	return true;
}

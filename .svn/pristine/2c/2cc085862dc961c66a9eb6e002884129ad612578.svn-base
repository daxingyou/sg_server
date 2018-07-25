#include "mount.hpp"
#include "tblh/MountTable.tbls.h"
#include "common/config_mgr.h"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"

mount_t::mount_t(uint32_t tid, uint32_t state)
	: m_tid(tid)
	, m_state(state)
{

}

mount_t::~mount_t()
{

}

uint32_t mount_t::get_weight()
{
	MountTable *p_config = GET_CONF(MountTable, m_tid);
	if (NULL == p_config)
	{
		log_error("[wys] mount config[%d] NULL", m_tid);
		return 0;
	}
	return p_config->weight();
}

void mount_t::peek_data(proto::common::mount_data_single* mount_data)
{
	if (mount_data)
	{
		mount_data->set_mount_tid(m_tid);
		mount_data->set_mount_state((proto::common::mount_state_type)m_state);
	}
}

void mount_t::load_data(const proto::common::mount_data_single &mount_data)
{
	m_tid = mount_data.mount_tid();
	m_state = mount_data.mount_state();
}

mount_data_t::mount_data_t(uint64_t role_uid)
	: m_role_uid(role_uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::mount_data);
	m_mount_map.clear();
}


mount_data_t::~mount_data_t()
{
}

mount_ptr mount_data_t::get_mount(uint32_t tid)
{
	mount_map::iterator iter = m_mount_map.find(tid);
	if (iter != m_mount_map.end())
		return iter->second;
	return mount_ptr();
}

mount_ptr mount_data_t::get_use_mount()
{
	if (m_use_mount_tid != 0)
	{
		return get_mount(m_use_mount_tid);
	}
	return mount_ptr();
}

uint32_t mount_data_t::get_turn_mount_tid()
{
	if (m_turn_mount_tid != 0)
		return m_turn_mount_tid;
	else
		return m_use_mount_tid;
}

uint32_t mount_data_t::get_mount_size()
{
	return m_mount_map.size();
}

void mount_data_t::add_mount(mount_ptr p_mount)
{
	if (NULL == p_mount)
	{
		log_error("[wys] add_mount mount NULL");
		return;
	}
	m_mount_map.insert(std::make_pair(p_mount->get_tid(), p_mount));
	save_self();
}

bool mount_data_t::use_mount(uint32_t tid)
{
	mount_ptr p_mount = get_mount(tid);
	if (NULL == p_mount)
	{
		log_error("[wys] mount role mount[%d] not find", tid);
		return false;
	}

	bool need_save = false;
	if (m_use_mount_tid != tid)
		need_save = true;

	uint32_t state = proto::common::mount_state_type_normal;
	mount_ptr p_use_mount = get_use_mount();

	if (p_use_mount)
	{
		if (p_use_mount->get_state() == proto::common::mount_state_type_use_and_turn)
			state = proto::common::mount_state_type_turn;
		p_use_mount->set_state(state);
	}

	m_use_mount_tid = tid;
	state = proto::common::mount_state_type_use;
	if (p_mount->get_state() == proto::common::mount_state_type_turn)
		state = proto::common::mount_state_type_use_and_turn;
	p_mount->set_state(state);

	if (need_save)
		save_self();
	return true;
}

bool mount_data_t::turn_mount(uint32_t tid)
{
	mount_ptr p_mount = get_mount(tid);
	if (NULL == p_mount)
	{
		log_error("[wys] mount role mount[%d] not find", tid);
		return false;
	}
	if (m_turn_mount_tid != 0)
	{
		cancel_turn_mount(m_turn_mount_tid);
	}

	uint32_t state = proto::common::mount_state_type_turn;
	if (p_mount->get_state() == proto::common::mount_state_type_use)
		state = proto::common::mount_state_type_use_and_turn;

	p_mount->set_state(state);
	m_turn_mount_tid = tid;
	save_self();
	return true;
}

bool mount_data_t::cancel_turn_mount(uint32_t tid)
{
	mount_ptr p_mount = get_mount(tid);
	if (NULL == p_mount)
	{
		log_error("[wys] mount role mount[%d] not find", tid);
		return false;
	}
	uint32_t state = proto::common::mount_state_type_normal;
	uint32_t cur_state = p_mount->get_state();
	if ( proto::common::mount_state_type_use_and_turn  == cur_state)
	{
		state = proto::common::mount_state_type_use;

	}
	else if (proto::common::mount_state_type_use == cur_state )
	{
		state = cur_state;
	}
	p_mount->set_state(state);
	m_turn_mount_tid = 0;
	save_self();
	return true;
}

void mount_data_t::peek_data(proto::common::mount_data* trade_data)
{
	if (NULL == trade_data)
	{
		log_error("mount trade_data NULL");
		return;
	}
	mount_map::iterator iter = m_mount_map.begin();
	mount_map::iterator iter_end = m_mount_map.end();

	mount_ptr p_mount = mount_ptr();
	proto::common::mount_data_single *single = NULL;
	for (; iter != iter_end; ++iter)
	{
		p_mount = iter->second;
		if (NULL == p_mount)
		{
			log_error("[wys] mount p_mount NULL");
			continue;
		}
		single = trade_data->add_mount_data_list();
		if (NULL == single)
		{
			log_error("[wys] mount peek_data add_mount_data_list fail");
			break;
		}
		single->set_mount_tid(p_mount->get_tid());
		single->set_mount_state((proto::common::mount_state_type)p_mount->get_state());
	}
}

void mount_data_t::load_data(const proto::common::mount_data &trade_info)
{
	m_mount_map.clear();
	for (int i = 0; i < trade_info.mount_data_list_size(); ++i)
	{
		const proto::common::mount_data_single &single = trade_info.mount_data_list(i);
		mount_ptr mount = mount_ptr( new mount_t( single.mount_tid(), single.mount_state() ) );
		if (NULL == mount)
		{
			log_error("mount new mount fail pos[%d]", i);
			break;
		}
		if (single.mount_state() == proto::common::mount_state_type_use)
			m_use_mount_tid = single.mount_tid();
		else if(single.mount_state() == proto::common::mount_state_type_turn)
			m_turn_mount_tid = single.mount_tid();
		else if (single.mount_state() == proto::common::mount_state_type_use_and_turn)
		{
			m_turn_mount_tid = single.mount_tid();
			m_use_mount_tid = single.mount_tid();
		}
		m_mount_map.insert(std::make_pair(mount->get_tid(), mount));
	}
}

void mount_data_t::save_self()
{
	proto::common::mount_data mounts; 
	peek_data(&mounts);
	role_unify_save::add_task(m_role_uid, m_key, mounts);
}

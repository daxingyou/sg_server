#pragma once
#include "protos_fwd.hpp"
#include <map>
#include <boost/shared_ptr.hpp>

/*
author:wys
date:2016.12.07
desc:坐骑
*/

class mount_t
{
public:
	mount_t(uint32_t tid, uint32_t state);
	~mount_t();

	void		set_state(uint32_t state) { m_state = state; }

	uint32_t	get_tid() { return m_tid; }
	// 状态(幻化 使用等等)
	uint32_t	get_state() { return m_state; }
	// 负重(跑商用)
	uint32_t	get_weight();

	void		peek_data(proto::common::mount_data_single* mount_data);
	void		load_data(const proto::common::mount_data_single &mount_data);
private:
	uint32_t	m_tid = 0;
    uint32_t	m_state = 0;
};
typedef boost::shared_ptr<mount_t> mount_ptr;

class mount_data_t
{
public:
	typedef std::map<uint32_t, mount_ptr> mount_map;
public:
	mount_data_t(uint64_t role_uid);
	~mount_data_t();

	mount_ptr	get_mount(uint32_t tid);
	mount_ptr	get_use_mount();
	uint32_t	get_use_mount_tid() { return m_use_mount_tid; }
	uint32_t	get_turn_mount_tid();
	uint32_t	get_mount_size();

	void		add_mount(mount_ptr p_mount);
	bool		use_mount(uint32_t tid);
	bool		turn_mount(uint32_t tid);
	bool		cancel_turn_mount(uint32_t tid);
	
	void		peek_data(proto::common::mount_data* trade_data);
	void		load_data(const proto::common::mount_data &trade_info);
	void		save_self(bool is_right_now = false);

private:
	mount_map   m_mount_map;
	std::string	m_key = "";
	uint64_t	m_role_uid;
	uint32_t	m_use_mount_tid = 0;
	uint32_t	m_turn_mount_tid = 0;
};

typedef boost::shared_ptr<mount_data_t> mount_data_ptr;

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "macros.hpp"
#include "common.pb.h"

class patrol_hire_t
{
public:
	patrol_hire_t(uint64_t role_uid);
	~patrol_hire_t() {}

	void del_timer();

	static void on_over_timer_callback(uint64_t user_id, const boost::system::error_code& ec);

public:
	void	load_data(const proto::common::patrol_hire_data& data, uint32_t last_login_out);
	void	peek_data(proto::common::patrol_hire_data* p_data);
	void	save_self();

public:
	uint32_t get_map_id() const { return m_map_id; }
	void set_map_id(uint32_t map_id);

	uint32_t get_begin_time() const { return m_begin_time; }
	void reset_begin_time();

	void add_fast_patrol_count();
	uint32_t get_fast_patrol_count() const { return m_fast_patrol_count; }

	void one_day();
	void on_login();
	void on_over_time();
private:
	std::string m_key = "";
	uint64_t m_role_uid = 0;
	uint32_t m_map_id = 0;				///场景ID
	uint32_t m_begin_time = 0;			///开始巡逻时间
	uint32_t m_fast_patrol_count = 0;	///快速巡逻
	boost::shared_ptr<boost::asio::deadline_timer> m_over_timer = NULL;	///巡逻结果定时器
};

typedef boost::shared_ptr<patrol_hire_t> patrol_hire_ptr;


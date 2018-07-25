#ifndef __CENTER_REDBAG_H__
#define __CENTER_REDBAG_H__

#include "macros.hpp"

#include "server.pb.h"
#include "common.pb.h"
#include <boost/asio/deadline_timer.hpp>
#include <map>
#include <vector>

class global_user_t;
typedef boost::shared_ptr<global_user_t> global_user_ptr;
class redbag_t
{
public:
	redbag_t(){}
public:
	void set_uid(uint64_t val) { m_uid = val; }
	uint64_t get_uid() { return m_uid; }
	
	void set_sender_uid(uint64_t val) { m_sender_uid = val; }
	uint64_t get_sender_uid() { return m_sender_uid; }

	void set_count(uint32_t val) { m_count = val; }
	uint32_t get_count() { return m_count; }

	void set_money(uint32_t val) { m_money = val; }
	uint32_t get_money() { return m_money; }
	
	void set_rest_count(uint32_t val) { m_rest_count = val; }
	uint32_t get_rest_count() { return m_rest_count; }

	void set_rest_money(uint32_t val) { m_rest_money = val; }
	uint32_t get_rest_money() { return m_rest_money; }

	void set_send_time(uint32_t val) { m_send_time = val; }
	uint32_t get_send_time() { return m_send_time; }

	void set_finish_time(uint32_t val) { m_finish_time = val; }
	uint32_t get_finish_time() { return m_finish_time; }

	void peek_data(proto::common::redbag_data* p_data);
	void load_draw_data(const proto::common::draw_data& dd);

	void save_self();
public:
	void start_timer();
	void stop_timer();
public:
	uint32_t draw(global_user_ptr p_role, proto::common::redbag_data* p_data);
public:
	static uint32_t get_expired_time();
	static uint32_t get_draw_min();
	static uint32_t get_draw_max();
	static uint32_t get_back_item();
private:
	uint64_t m_uid = 0;
	uint64_t m_sender_uid = 0;
	uint32_t m_count = 0;
	uint32_t m_money = 0;
	std::map<uint64_t, std::pair<uint32_t,std::string>> m_draw_data;
	uint32_t m_rest_count = 0;
	uint32_t m_rest_money = 0;
	uint32_t m_send_time = 0;
	uint32_t m_finish_time = 0;
	boost::shared_ptr<boost::asio::deadline_timer> m_timer;
	
};
typedef boost::shared_ptr<redbag_t> redbag_ptr;

#endif
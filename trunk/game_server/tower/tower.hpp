#ifndef __GAME_TOWER_H__
#define __GAME_TOWER_H__

#include "protos_fwd.hpp"
#include "macros.hpp"
#include "tower_trigger.hpp"
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>

class tower_t
{
public:
	tower_t(uint64_t uid);
    ~tower_t();
public:
	void init_new_data();
	void load_data(const proto::common::tower_data& td);
	void peek_data(proto::common::tower_data* p_data);
	void save_self();

	void one_day();
public:
	uint32_t get_record_tower() { return m_record_tower; }
	void set_record_tower(uint32_t val) { m_record_tower = val; }

	uint32_t get_current_tower() { return m_current_tower; }
	void set_current_tower(uint32_t val) { m_current_tower = val; }

	uint32_t get_reset_count() { return m_reset_count; }
	void set_reset_count(uint32_t val) { m_reset_count = val; }

	uint32_t get_max_reset_count() { return m_max_reset_count; }
	void set_max_reset_count(uint32_t val) { m_max_reset_count = val; }

	uint32_t get_auto_fight_time() { return m_auto_fight_time; }
	void set_auto_fight_time(uint32_t val) { m_auto_fight_time = val; }

	uint32_t get_current_achieve() { return m_current_achieve; }
	void set_current_achieve(uint32_t val) { m_current_achieve = val; }

	uint32_t get_first_day() { return m_first_day; }
	void set_first_day(uint32_t val) { m_first_day = val; }

	uint32_t get_trigger_count() { return m_trigger_count; }
	void set_trigger_count(uint32_t val) { m_trigger_count = val; }

	uint32_t get_trigger_fight_count() { return m_trigger_fight_count; }
	void set_trigger_fight_count(uint32_t val) { m_trigger_fight_count = val; }
public:
	static uint32_t get_max_trigger_count();

	static uint32_t get_max_trigger_fight_count();

	static uint32_t get_double_time_id();

	static uint32_t get_double_value();

	static void on_auto_fight_over_timer( uint64_t user_id, const boost::system::error_code& ec);
	
	static void send_auto_fight_over_mail(uint64_t user_id);
public:
	uint64_t gen_trigger_uid();
	void add_other_trigger(const proto::common::tower_trigger& single);
	//触发隐藏层
	void trigger(uint32_t current_tower);
	void peek_self_trigger(proto::common::tower_trigger_data* p_data);
	void send_all_self_trigger(uint64_t friend_uid);
	tower_trigger_ptr get_trigger(uint64_t trigger_uid);
	void del_trigger(uint64_t trigger_uid);

	void set_auto_fight_timer();
	void del_auto_fight_timer();
private:
	std::string m_key = "";
	uint64_t m_owner = 0;
    uint32_t m_record_tower = 0;
	uint32_t m_current_tower = 0;
	uint32_t m_reset_count = 0;
	uint32_t m_max_reset_count = 0;
	uint32_t m_auto_fight_time = 0;
	uint32_t m_current_achieve = 0;
	uint32_t m_first_day = 0;
	tower_trigger_map m_self_trigger;
	tower_trigger_map m_other_trigger;
	uint32_t m_trigger_count = 0;
	uint32_t m_trigger_fight_count = 0;
	uint32_t m_send_mail_flag = 0;									//用于记录已经发送过通知的不会再通知. 1表示需要发送 0 表示不需要
	boost::shared_ptr<boost::asio::deadline_timer> m_af_over_timer;	//扫荡结束定时器
};
typedef boost::shared_ptr<tower_t> tower_ptr;
#endif
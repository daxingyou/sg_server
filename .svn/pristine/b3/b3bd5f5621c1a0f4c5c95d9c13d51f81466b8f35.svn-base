#ifndef __GAME_INTERIOR_H__
#define __GAME_INTERIOR_H__

#include "protos_fwd.hpp"
#include <boost/asio/deadline_timer.hpp>
#define INTERIOR_BUILD_LEVEL_MAX 10
#define INTERIOR_OPEN_LEVEL 10
#define INTERIOR_TASK_MAX_COUNT 15
#define INTERIOR_TASK_STAR_LEVEL_MAX 5
#define INTERIOR_TASK_REFRESH_COST 2000
#define INTERIOR_TASK_UPGRADE_STAR_COST 2000
#define INTERIOR_TASK_COMPLETE_COST 2000

enum TASK_STATE
{
	TASK_STATE_NONE = 0,
	TASK_STATE_START = 1,
	TASK_STATE_COMPLETE = 2,
};

class role_t;
class interior_t
{
public:
	interior_t(uint64_t uid);
    ~interior_t();

public:
	uint32_t get_level() { return m_level; }
	void set_level(uint32_t level) { m_level = level; }
	uint32_t get_left_times() { return m_left_times; }
	void set_left_times(uint32_t left_times) { m_left_times = left_times; }
	bool is_start() { return m_is_start; }
	void add_hero(uint32_t hero_id) { m_hero_id_list.push_back(hero_id); }

	uint32_t get_task_id() { return m_task_id; }
	uint32_t get_task_star_level() { return m_star_level; }

	void init_new_data();
    void load_data(const proto::common::interior_info& info);
    void peek_data(proto::common::interior_info* p_interior_info);

	uint32_t get_task_left_time();
	uint32_t get_copper_left_time();
	uint32_t get_copper();
	uint32_t random_task();
	bool get_task_reward(proto::common::role_change_data* p_data);
	bool upgrade_task_star_level();
	bool complete_interior_task();
	void start_task();
	void start_task_timer();
	// 任务定时器回调
	void task_finished_callback(const boost::system::error_code& ec);

	void notify_info_to_client();

	uint32_t get_garrison_city() { return m_garrison_city_id; }
	void set_garrison_city(uint32_t garrison_city_id) { m_garrison_city_id = garrison_city_id; }

	uint32_t get_offline_reward_calc_time() { return m_offline_reward_calc_time; }
	void set_offline_reward_calc_time(uint32_t val) { m_offline_reward_calc_time = val; }

	bool is_first_enter_game() { return m_first_enter_game; }
	void set_is_first_enter_game(bool val) { m_first_enter_game = val; }
private:
	uint64_t m_owner = 0;
    uint32_t m_level = 0;
    uint32_t m_task_id = 0;
	uint32_t m_star_level = 0;
	uint32_t m_start_time = 0;
	uint32_t m_span_time = 0;
	uint32_t m_last_reward_time = 0;
	std::vector<uint32_t> m_hero_id_list;
	uint32_t m_left_times = 0;
	uint32_t m_is_start = 0;
	uint32_t m_offline_reward_calc_time = 0;
	uint32_t m_garrison_city_id = 0;
	bool m_first_enter_game = true;
	boost::shared_ptr<boost::asio::deadline_timer> m_task_timer;
};
typedef boost::shared_ptr<interior_t> interior_ptr;
#endif
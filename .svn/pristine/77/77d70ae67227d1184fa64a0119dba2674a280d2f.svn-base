#ifndef __FIGHT_STUDY_HPP__
#define __FIGHT_STUDY_HPP__

#include "common/simple_timer.hpp"

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

enum fight_study_step
{
	fight_study_step_none = 0,
	fight_study_step_match = 1,
	fight_study_step_wait = 2,
};

// ÇÐ´è
class fight_study_manager_t
{
	friend class fight_study_msg_handle_t;
public:
	fight_study_manager_t(uint64_t owner);
	~fight_study_manager_t();

	bool fight_study_request(role_ptr role, uint64_t target_uid);
	bool fight_study_cancel(role_ptr role);
	bool fight_study_answer_request(role_ptr target, role_ptr role, uint32_t answer);

	bool is_in_study(role_ptr role);
	bool check_frequency(role_ptr role);
	uint32_t scene_check(const role_ptr& p_role);

protected:

	void start_match(role_ptr role, role_ptr target);
	void on_start_match_timer(const boost::system::error_code& ec);

	void start_wait(role_ptr role, role_ptr target);
	void on_start_wait_timer(const boost::system::error_code& ec);

	void cancel_match(role_ptr role);
	void cancel_all_match(role_ptr role);

	void fight_study_reply(role_ptr role, uint32_t reply_code);
	void fight_study_notify(role_ptr target, role_ptr role);
	void fight_study_answer_notify(role_ptr role, uint32_t answer);
	void fight_study_answer_reply(role_ptr target, uint32_t reply_code);

	uint32_t get_remain_time(role_ptr role);

private:

	uint64_t		m_owner = 0;
	simple_timer_t	m_timer;
};


#endif//__FIGHT_STUDY_HPP__
#pragma once

#include "protos_fwd.hpp"
#include "utility.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

enum page_reward_status : uint32_t
{
	PAGE_REWARD_NOT_GET = 0,			///未领取
	PAGE_REWARD_ALREADY_GET = 1,		///已领取
};

enum PAGE_OPEN_COND_ENUM : uint32_t
{
	PAGE_OPEN_COND_ELITE_ID = 1,	///章节精英ID
	PAGE_OPEN_COND_LEVEL = 2,		///等级
};

class page_manager_t
{
public:
	static void			init(uint64_t role_uid);
	static void			check_first_page_elite_open(uint64_t role_uid);
	static void			check_page_elite(uint64_t role_uid);
	static void			check_reward_status(uint64_t role_uid);
	static void			check_page_task_status(uint64_t role_uid,uint32_t task_id);
	static void			check_page_elite_task_status(uint64_t role_uid, uint32_t page_id, uint32_t against_id);
	static uint32_t		get_page_list(uint64_t role_uid,proto::client::gc_section_reply& reply);
	static uint32_t		get_page_pass_reward(uint32_t type,uint64_t role_uid,uint32_t id ,proto::client::gc_section_reward_reply& reply);
	static uint32_t		quick_fight_elite_page(uint64_t role_uid, uint32_t id , uint32_t type, proto::client::gc_quick_fight_elite_page_reply &reply);		//快速扫荡
	static uint32_t		reset_quick_fight_elite_page(uint64_t role_uid, uint32_t id , proto::client::gc_reset_quick_fight_elite_page_reply &reply);	//重置类型
	static void			page_task_finish_notify(uint64_t role_uid);
};

typedef boost::shared_ptr<page_manager_t>  page_mgr_ptr;

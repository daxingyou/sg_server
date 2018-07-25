#pragma once

#include <boost//smart_ptr/shared_ptr.hpp>
#include "protocpp/protos_fwd.hpp"
#include "achieve/achieve_common.hpp"
#include "active_reward.hpp"

class active_reward_manager_t
{
public:
	active_reward_manager_t();
	~active_reward_manager_t();

public:

	///主角等级
	static void		achieve_main_role_level(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t param1 = 0, uint32_t param2 = 0);

	///主角养成类
	static void		achieve_hero_cultivate(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t num = 1);

	///n * n 模式统一接口
	static void		achieve_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t num = 1);

	///n 累加模式统一接口
	static void		achieve_total_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t param = 0, uint32_t num = 1);

	///替换模式
	static void		achieve_replace_model_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id = 0, uint32_t new_value = 0);

public:
	static void		load_all_data(uint64_t uid, const proto::common::active_reward_data& data);
	static void		peek_all_data(uint64_t uid, proto::client::gc_active_reward_list_reply& reply, bool is_client = true);
	static void		save_all_self(uint64_t uid);

	static bool		add_active_reward(const uint64_t role_uid, const uint32_t id,const proto::common::Achieve_Event_Type type);
	static bool		add_active_reward(const uint64_t role_uid, active_reward_ptr p_active_reward);
	static active_reward_ptr	get_active_reward(const uint64_t role_uid,const uint32_t id);

	static bool		get_level_range(const uint32_t level,uint32_t& range);

	static void		init_task_list(const uint64_t role_uid);


	static void		get_active_reward_list(const uint64_t role_uid);

	static void		check_not_grant_reward_send_mail(const uint64_t role_uid);

	static uint32_t	get_active_reward_gift(const uint64_t role_uid, const uint32_t tid, proto::common::role_change_data* p_data);

private:

};

typedef boost::shared_ptr<active_reward_manager_t> active_reward_manager_ptr;


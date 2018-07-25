#ifndef __LIFEFORCE_MANAGER_H__
#define __LIFEFORCE_MANAGER_H__

#include "protocpp/protos_fwd.hpp"

#define MAX_LIFESTAR_LEVEL 9

class lifeforce_manager_t
{
public:

	//升级命星等级(点星)
	static void level_up_lifestar_level(uint64_t role_uid);
	//升级境界等级
	static void level_up_lifestate_level(uint64_t role_uid, proto::client::gc_break_life_state_reply& reply);
	//升级命师称号
	static void level_up_lifelabel_level(uint64_t role_uid);

public:
	//突破任务是否完成
	static bool break_task_is_finish(uint64_t role_uid, uint32_t task_id);

	//检查当前突破任务是否完成
	static bool check_break_task_is_finish(uint64_t role_uid, uint32_t task_id);
};

#endif // !__LIFEFORCE_MANAGER_H__

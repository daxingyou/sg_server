#ifndef __LIFEFORCE_ATTR_MANAGER_H__
#define __LIFEFORCE_ATTR_MANAGER_H__

#include "protocpp/common.pb.h"
#include "hero/hero.hpp"

enum LIFEFORCE_REWARD_TYPE : uint32_t
{
	LIFEFORCE_REWARD_ATTR = 1,				//基础单一属性
	LIFEFORCE_REWARD_SKILL = 2,				//技能奖励
	LIFEFORCE_REWARD_FORMATION = 3,			//阵法奖励
	LIFEFORCE_REWARD_ITEM = 4,				//物品奖励
};

class lifeforce_attr_manager_t
{

public:
	//统一发放命力奖励接口
	static bool grant_lifeforce_reward(uint64_t role_uid,uint32_t source_type, proto::common::role_change_data* p_data = NULL);

public:

	static void init_attr_reward(uint64_t role_uid){}
public:

	//加单一属性奖励
	static void add_single_attr(uint64_t role_uid, attr_map& attrs);

	static void add_single_attr_reward(uint64_t role_uid, proto::common::role_change_data* p_data = NULL);

	//命师称号加战斗力
	static uint32_t add_fighting(uint64_t role_uid);							
	
	//发放物品奖励
	static bool add_item_reward(uint64_t role_uid,uint32_t drop_id, uint32_t source_type, proto::common::role_change_data* p_data = NULL);

	//发放组合属性奖励 通过技能实现 激活技能即可 （目前激活技能无法实现 延后）
	static bool add_skill_reward(uint64_t role_uid,uint32_t skill_id, uint32_t source_type, proto::common::role_change_data* p_data = NULL);

	//发放阵法奖励
	static bool add_tactic_reward(uint64_t role_uid,uint32_t tactic_id, uint32_t source_type, proto::common::role_change_data* p_data = NULL);
};

#endif // !__LIFEFORCE_ATTR_MANAGER_H__

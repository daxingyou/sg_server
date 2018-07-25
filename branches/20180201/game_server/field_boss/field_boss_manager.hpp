#ifndef __GAME_FIELD_BOSS_MANAGER_H__
#define __GAME_FIELD_BOSS_MANAGER_H__

#include "server.pb.h"
#include "macros.hpp"
#include "role/role.hpp"
#include "server.pb.h"
#include "fight/combat.hpp"

class field_boss_manager_t
{
public:
	// center 召唤BOSS
	static bool summon_field_boss(uint32_t id, uint64_t scene_id, uint32_t monster_id, uint64_t object_id, uint32_t x, uint32_t y, uint32_t dir);
	// center 删除boss
	static void delete_field_boss(uint64_t object_id, proto::server::EM_FIELD_MODE mode);

public:
	static bool role_attack_field_boss(const role_ptr& p_role, uint64_t object_id, proto::client::gc_field_boss_attack_reply& reply);
	static void role_leave_fight(uint64_t object_id, uint64_t fight_id, const proto::common::fight_camp& self_camp);
	static void fight_role_hurt(uint64_t object_id, uint64_t fight_id, const proto::common::fight_camp& self_camp, const proto::common::fight_camp& enemy_camp, const combat_ptr& combat);

public:
	static void reward_clear();
	static void add_persion_reward(uint32_t monster_tid, uint32_t id);
	static const std::vector<uint32_t>& get_boss_persion_reward(uint32_t monster_tid);

	static void add_family_reward(uint32_t monster_tid, uint32_t id);
	static const std::vector<uint32_t>& get_boss_family_reward(uint32_t monster_tid);

protected:
	static std::map<uint32_t, std::vector<uint32_t>> m_field_boss_persion_reward;
	static std::map<uint32_t, std::vector<uint32_t>> m_field_boss_family_reward;
	// 一个空的奖励，用于错误返回
	static std::vector<uint32_t> m_empty_boss_reward;
};

#endif
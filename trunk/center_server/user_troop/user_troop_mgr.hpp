#ifndef __USER_TROOP_MGR_H__
#define __USER_TROOP_MGR_H__
#include "macros.hpp"
#include "user_troop.hpp"

// 几个特殊的组队类型（方剑）
#define TROOP_TYPE_DEFAULT 0	// 默认
#define ALL_TROOP_TYPE 100		// 全部
#define EXPEDITION_HELP_TROOP_TYPE 9999	// 远征

class global_troop_manager
{
public:
	// 生成一个组队唯一ID
	static uint64_t get_new_troop_uid();

public:
	typedef std::map<uint64_t, troop_ptr> troop_map;
	typedef std::map<uint64_t, troop_wptr> troop_wmap;
	typedef std::map<uint32_t, troop_wmap> troop_type_wmap;

	// 老的判断
	static bool check_type(uint32_t type);
	// 类型等级
	static uint32_t get_type_min_level(uint32_t type);
	// 最大等级
	static uint32_t get_max_level(uint32_t type);

	static troop_ptr get_troop(uint64_t troop_uid);

	// 删除所有组队信息
	static void del_all_troop();

public:
	// 创建一个组队
	static uint32_t create_new_troop(const global_user_ptr& p_user, uint32_t type);
	// 加入组队
	static uint32_t join_troop(const global_user_ptr& p_user, uint64_t troop_id);
	// 处理入队申请
	static uint32_t process_apply(const global_user_ptr& p_user, uint64_t member_uid, uint32_t process);
	// 场景服返回处理
	static uint32_t check_agree_success(const global_user_ptr& p_user, uint64_t member_uid);
	// 组队邀请
	static void troop_invite_join(const global_user_ptr& p_user, const proto::client::ce_troop_invite_join_request& request);
	// 退出队伍
	static uint32_t quit_troop(const global_user_ptr& p_user);
	// 踢出队伍
	static uint32_t kick_troop(const global_user_ptr& p_user, uint64_t member_uid);
	// 自动匹配
	static void auto_match(const global_user_ptr& p_user, uint32_t type);
	// 修改队伍信息
	static uint32_t modify_troop(const global_user_ptr& p_user, uint32_t new_type, uint32_t level_min, uint32_t level_max);
	// 解散队伍
	static uint32_t dissolve_troop(const global_user_ptr& p_user);

	// 获得队伍列表
	static void get_troop_list(const global_user_ptr& p_user, uint32_t type, proto::client::ec_troop_list_reply& reply);

public:
	// 邀请一个队员
	static uint32_t troop_invite_member(const global_user_ptr& p_user, uint64_t invite_uid);
	// 邀请一个化身
	static uint32_t troop_invite_shadow(const global_user_ptr& p_user, uint64_t friend_uid);

private:
	static troop_map m_troop_map;
	static troop_type_wmap m_troop_type_wmap;
};

#endif

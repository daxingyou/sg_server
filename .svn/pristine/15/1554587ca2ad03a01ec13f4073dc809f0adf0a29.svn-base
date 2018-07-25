#ifndef __USER_TROOP_MGR_H__
#define __USER_TROOP_MGR_H__
#include "macros.hpp"
#include "user_troop.hpp"
#include "server.pb.h"

class game_troop_mgr_t
{
public:
	typedef std::map<uint64_t, troop_ptr> troop_map;

	static troop_ptr get_troop(uint64_t troop_id);
	static void delete_troop(uint64_t troop_id);
	static void troop_event(uint64_t uid, proto::server::eg_troop_event_notify& ntf);
	
	// 删除所有组队信息
	static void del_all_troop();

	// 消息处理
public:
	static uint32_t create_troop(const role_ptr& p_role, const proto::client::cg_troop_create_request& request);

	static uint32_t auto_match(const role_ptr& p_role, uint32_t type);

	static uint32_t join_troop(const role_ptr& p_role, uint64_t troop_id);

	static uint32_t check_process_apply(uint64_t uid);

	static uint32_t back_troop(const role_ptr& p_role);

	static void process_invite(const role_ptr& p_role, uint64_t troop_id, uint32_t process);

	//接取抓鬼任务
	static uint32_t accept_zhuagui_task(const role_ptr& p_role, uint32_t task_id);	
public:
	static uint32_t troop_transmission_reading_end(role_ptr p_role);

private:
    static troop_map m_troop_map;
};
#endif

#ifndef __GAME_ROLE_MANAGER_H__
#define __GAME_ROLE_MANAGER_H__

#include <map>
#include "role.hpp"

typedef std::map<uint64_t, role_ptr> role_map;

class role_manager_t 
{

public:
	~role_manager_t();

	static role_ptr find_role(uint64_t uid);
	static const role_map& get_all_role(){ return m_role_list;}
	static bool add_role(role_ptr p_role);
    
    static void del_role(uint64_t uid, uint32_t kick_reason = common::KICK_ROLE_REASON_NONE);
    static void del_role(uint32_t gate_id);
    static void del_all_roles(common::KICK_ROLE_REASON kick_reason = common::KICK_ROLE_REASON_NONE);
	static void del_role_by_cross_server(uint64_t uid, proto::server::scene_change_type type);

	static void enter_game(const network::tcp_socket_ptr& s, uint64_t uid);
	//重置竞技场数据
	static void reset_arena();
	//竞技场奖励
	static void award_arena();
	//////////////////////////////////////////////////////////////////////////
	// 场景注册玩家
	static void scene_reg_user(uint64_t uid, const proto::server::eg_reg_user_scene_request& request);
	// 读档数据处理
	static void on_user_record(uint64_t uid, const proto::server::dg_role_data_reply& rdr);
	// gate离线、下线处理
	static void gate_unreg_user(uint64_t uid, uint32_t unreg_code, uint32_t kick_reason);
	// center离线、下线处理
	static void center_unreg_user(uint64_t uid, uint32_t unreg_code);
	// 玩家重连
	static void scene_relogin_user(uint64_t uid, uint32_t gate_id);

public:

	static void one_day(uint32_t curr_time, bool pass_week);
	// BUFF定时器触发
	static void on_del_buff(uint64_t role_uid, uint32_t buff_id, const boost::system::error_code& ec);
public:
	static role_map m_role_list;
};

#endif // !__GAME_ROLE_MANAGER_H__

#ifndef __ADMIN_FUNC_MANAGER_HPP__
#define __ADMIN_FUNC_MANAGER_HPP__

#include "center/center_msg_handler.hpp"
#include "server.pb.h"

class admin_func_manager_t {
public:
	static bool send_mail_to_center(std::string &json_data);

	static bool get_user_num_from_center( uint32_t admin_id );
	
	static bool send_notice_to_center(std::string &json_data);

	static bool transport_to_center(std::string &json_data);

	static bool set_user_create_limit(std::string &json_data);
	
	static bool refresh_ac_login_list();

	static bool manage_role(uint32_t admin_id, std::string &json_data);

	static bool search_role_byid(const network::tcp_socket_ptr& s, std::string &json_data);

	static bool search_role_byname(const network::tcp_socket_ptr& s, std::string &json_data);

	static bool reload_config(std::string &json_data);
public:
	static bool send_user_num_to_tools( uint32_t admin_id, uint32_t num );

	static bool send_manage_role_to_tools(const proto::server::em_manage_role_ntf& msg);

	// static bool notify_server_reload_xml(const std::string& server_type);
private:
	static bool search_role(const network::tcp_socket_ptr& s, uint64_t role_uid);
//解析blob数据的接口
public:
	//爬塔隐藏关卡
	static bool parse_tower_trigger(uint64_t role_uid, proto::common::tower_trigger_data* p_data);
	//远征数据
	static bool parse_expedition_data(uint64_t role_uid, proto::common::expedition_data* p_data);
	//活动数据
	static bool parse_activity_data(uint64_t role_uid, proto::common::activities_data* p_data);
	//抽卡数据
	static bool parse_luckydraw_data(uint64_t role_uid, proto::common::luckydraw_data* p_data);
	//技能数据
	static bool parse_skill_data(uint64_t hero_uid, proto::common::hero_skill_data* p_data);
	//天赋数据
	static bool parse_talent_data(uint64_t hero_uid, proto::common::talent_data* p_data);
	//进行中的任务
	static bool parse_cur_task(uint64_t role_uid, proto::common::cur_task_date* p_data);
	//已完成的任务
	static bool parse_comp_task(uint64_t role_uid, proto::common::comp_task_date* p_data);
	//炼丹数据
	static bool parse_liandan_data(uint64_t role_uid, proto::common::liandan_data* p_data);
	//装备方案数据
	static bool parse_equip_plan_data(uint64_t role_uid, proto::common::equip_plan_data* p_data);
	//祈愿记录
	static bool parse_prayer_log(uint64_t role_uid, proto::common::family_prayer_record_list* p_data);
	//全局杂项数据(类型不一样暂时不做)
	//static bool parse_global_server_data(uint32_t type, proto::common::family_prayer_record_list* p_data);
	//竞技场镜像数据
	static bool parse_arena_fight_data(uint64_t role_uid, proto::common::mirror_fight_data* p_data);
	//成就数据
	static bool parse_achieve_data(uint64_t role_uid, proto::common::achieve_data* p_data);
	//副本次数数据
	static bool parse_dungeon_times_data(uint64_t role_uid, proto::common::role_all_dungeon_times_data* p_data);
	//家族技能数据
	static bool parse_family_skill_data(uint64_t role_uid, proto::common::family_skill_data* p_data);
	//寄售行装备数据
	static bool parse_goods_equip_data(uint64_t goods_uid, proto::common::equip_single* p_data);
	//红包领取数据
	static bool parse_redbag_draw_data(uint64_t redbag_uid, proto::common::draw_data* p_data);
	//角色基本数据
	static bool parse_role_simple_data(uint64_t role_uid, proto::common::role_simple_data* p_data);
	//离线竞技场防守阵容
	static bool parse_offline_arena_fight_data(uint64_t role_uid, proto::common::mirror_fight_data* p_data);
	//离线竞技场战报数据
	static bool parse_offline_arena_record_data(uint64_t role_uid, proto::common::offline_arena_record_data* p_data);

};

#endif


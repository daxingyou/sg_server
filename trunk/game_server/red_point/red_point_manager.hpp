#ifndef __RED_POINT_MANAGER_H__
#define __RED_POINT_MANAGER_H__

#include "protocpp/common.pb.h"
#include "protocpp/client.pb.h"
#include "red_point.hpp"

enum red_point_activity_type : uint32_t
{
	activity_day = 1		 //日常活动
};

/*
 * 说明: 红点关服即全部销毁 不存在DB
*/
typedef std::map<uint32_t, red_point_ptr> red_point_map;
typedef std::set<uint32_t> red_point_activity_join_set;
typedef boost::shared_ptr<red_point_map> red_point_map_ptr;
typedef boost::shared_ptr<red_point_activity_join_set> red_point_activity_join_map_ptr;
class red_point_manager_t
{
public:
	red_point_manager_t(){}
	~red_point_manager_t(){}
public:
	///红点列表中是否存在该红点
	static bool is_exist(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type);
	///该玩家是否存在红点
	static bool is_exist(uint64_t role_uid);

	//加入红点到红点列表中
	static void add_red_point(uint64_t role_uid, red_point_ptr p_red_point);
	
	//获取红点
	static red_point_ptr get_red_point(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type);

public:
	//检查红点状态 清除红点
	static void check_red_point(uint64_t role_uid);
	static bool check_activity_open_box(uint64_t role_uid,bool is_online = false);
	static void check_activity_join(uint64_t role_uid);
	static bool check_page_point(uint64_t role_uid);
public:

	//红点统一通知接口
	static void red_point_notify(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type,uint32_t param = 0);
	static void red_point_notify(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type, std::set<uint32_t>& param_list);
	
	//发送通知给客户端
	static bool send_notify_to_client(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type,std::string& dec);

	//发送通知给客户端(统一发送)
	static bool send_notify_to_client(uint64_t role_uid, proto::client::gc_red_point_notify& notify);
public:
	//检查红点列表(没有取消的红点 继续发送客户端 显示红点 进入场景调用)
	static void init_red_point(uint64_t role_uid);
	//获取个人红点列表
	static red_point_map_ptr get_role_red_point_list(uint64_t role_uid);
public:
	//清除个人所有红点
	static void remove_role_all_red_point(uint64_t role_uid);
	//清除个人指定类型红点
	static void remove_role_type_red_point(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type);
	
};
static std::map<uint64_t, red_point_map_ptr> m_all_role_red_point_list;
static std::map<uint64_t, red_point_activity_join_map_ptr> m_activity_join_list;	//专门用于记录参与了那些活动 

#endif // !__RED_POINT_MANAGER_H__

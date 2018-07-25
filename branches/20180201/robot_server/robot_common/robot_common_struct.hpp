#ifndef __ROBOT_COMMON_STRUCT_HPP__
#define __ROBOT_COMMON_STRUCT_HPP__

#include "common/common_fwd.hpp"

#include <string>

struct robot_config_s
{
    std::string account_server_ip;
    uint32_t account_server_port;
    uint32_t network;
    uint32_t server_group_id;
    std::string server_list_type = "";
    int32_t channel;
    uint32_t start_id = 1;		// 机器人编号的起始id
    uint32_t number = 1;		// 机器人数量
    std::string sepcial = "";	// 指定id机器人
    uint32_t login_interval;	// 循环登陆间隔
	uint32_t lua_open;			// 是否开启lua
    uint32_t lua_heart;			// lua 心跳
	uint32_t heart_group;		// 分组
	uint32_t frame_time;		// 每帧时间 = lua_heart/heart_group
    int32_t wlog_info;
    int32_t wlog_trace;
	int32_t wlog_debug;
	bool    wlog_msg_count;
	std::string script_path;
	uint32_t gm_level = 0;	//用于刷账号的权限等级, 0表示没有， 1表示普通， 2 表示超级
	std::string robot_name = "";
};
typedef struct robot_config_s robot_config_t;

#define ROLE_JOB_SIZE 4

// enum  ===========================================================================================
enum login_state_em
{
    LOGIN_STATE_NONE = 0,
	LOGIN_STATE_LOGIN_PRE1,	// 登陆account分步1
	LOGIN_STATE_LOGIN_PRE2,	// 登陆account分步2
	LOGIN_STATE_LOGIN_ACCOUNT,	// 登陆account
    LOGIN_STATE_LOGIN_LOGIN,	// 登陆login
    LOGIN_STATE_LOGIN_GATE,		// 登陆 gate
    LOGIN_STATE_CREATE_USER,	// 创建角色
	LOGIN_STATE_LOAD_ROLE,		// 加载角色完成
	LOGIN_STATE_ENTER_SCENE,	// 进入场景
	LOGIN_STATE_CHANGE_SCENE,	// 跳场景
};

#endif  //__ROBOT_COMMON_STRUCT_HPP__

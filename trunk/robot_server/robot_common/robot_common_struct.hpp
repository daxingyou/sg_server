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
    uint32_t start_id = 1;		// �����˱�ŵ���ʼid
    uint32_t number = 1;		// ����������
    std::string sepcial = "";	// ָ��id������
    uint32_t login_interval;	// ѭ����½���
	uint32_t lua_open;			// �Ƿ���lua
    uint32_t lua_heart;			// lua ����
	uint32_t heart_group;		// ����
	uint32_t frame_time;		// ÿ֡ʱ�� = lua_heart/heart_group
    int32_t wlog_info;
    int32_t wlog_trace;
	int32_t wlog_debug;
	bool    wlog_msg_count;
	std::string script_path;
	uint32_t gm_level = 0;	//����ˢ�˺ŵ�Ȩ�޵ȼ�, 0��ʾû�У� 1��ʾ��ͨ�� 2 ��ʾ����
	std::string robot_name = "";
};
typedef struct robot_config_s robot_config_t;

#define ROLE_JOB_SIZE 4

// enum  ===========================================================================================
enum login_state_em
{
    LOGIN_STATE_NONE = 0,
	LOGIN_STATE_LOGIN_PRE1,	// ��½account�ֲ�1
	LOGIN_STATE_LOGIN_PRE2,	// ��½account�ֲ�2
	LOGIN_STATE_LOGIN_ACCOUNT,	// ��½account
    LOGIN_STATE_LOGIN_LOGIN,	// ��½login
    LOGIN_STATE_LOGIN_GATE,		// ��½ gate
    LOGIN_STATE_CREATE_USER,	// ������ɫ
	LOGIN_STATE_LOAD_ROLE,		// ���ؽ�ɫ���
	LOGIN_STATE_ENTER_SCENE,	// ���볡��
	LOGIN_STATE_CHANGE_SCENE,	// ������
};

#endif  //__ROBOT_COMMON_STRUCT_HPP__

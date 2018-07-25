#ifndef __COMMON_COMMON_STRUCT_HPP__
#define __COMMON_COMMON_STRUCT_HPP__

#include "log_enum.hpp"

NS_COMMON_BEGIN

#define OPEN_GM_OP


enum LOGIN_REPLY_CODE : uint32_t
{
    LRC_OK                  = 0,
    LRC_OTHER_ERROR         = 1,
    LRC_CHANNEL_ERROR       = 2, //渠道验证失败
    LRC_CHANNEL_CHECK_ERROR = 3, //渠道类型错误
    LRC_GATE_ERROR          = 4, //获取gate信息失败
};


enum KICK_ROLE_REASON : uint32_t
{
    KICK_ROLE_REASON_NONE               = 0,
    KICK_ROLE_REASON_NORMAL             = 1,    //客户端主动断开socket
    KICK_ROLE_REASON_PINGPONG_TIMEOUT   = 2,    //心跳超时
    KICK_ROLE_REASON_RECONNECT_TIMEOUT  = 3,    //重连超时
    KICK_ROLE_REASON_BY_OTHER           = 5,    //顶号
    KICK_ROLE_REASON_GATE_SERVER_CLOSED = 6,    //gate服务器关闭
    KICK_ROLE_REASON_GAME_SERVER_CLOSED = 7,    //game服务器关闭
	KICK_ROLE_REASON_CENTER_SERVER_CLOSED = 8,  //center服务器关闭
	KICK_ROLE_REASON_CROSS_END			= 9,    //跨服结束
	KICK_ROLE_REASON_ARENA_BATTLE_END   = 10,   //竞技场战斗结束
    KICK_ROLE_REASON_MSG_BOMB           = 12,   //消息轰炸
	KICK_ROLE_REASON_CROSS_SCENE		= 13,	//跨场景
	KICK_ROLE_REASON_CENTER_UNREG		= 14,	//中心服注销
	KICK_ROLE_REASON_GAME_UNREG			= 15,	//场景服注销
	KICK_ROLE_REASON_ERROR				= 16,	//发生错误注销
	KICK_ROLE_REASON_BAN				= 17,	//账号封停
	KICK_ROLE_REASON_DB_SERVER_CLOSED	= 18,   //db服务器关闭
};

enum ROLE_GAME_STATE : uint32_t
{
	ROLE_GAME_STATE_INIT,		// 无效状态
	ROLE_GAME_STATE_GATE,		// 登陆到GATE
	ROLE_GAME_STATE_REGATE,		// 重新登陆到GATE
	ROLE_GAME_STATE_LOGIN,		// 正在登陆场景
	ROLE_GAME_STATE_SCENE,		// 游戏状态
	ROLE_GAME_STATE_WAIT,		// 等待状态（跨服）
	ROLE_GAME_STATE_OFFLINE,	// 离线状态
};

enum UNREGUSER_RET_TYPE : uint32_t
{
	UNREGUSER_RET_LOGOUT = 0,	// 登出
	UNREGUSER_RET_OFFLINE = 1,	// 离线
	UNREGUSER_RET_ERROR = 2,	// 发生错误
	UNREGUSER_RET_NO_ROLE = 3,	// 账号没有角色
	UNREGUSER_RET_BAN = 4,		// 账号封停
};

enum ROLE_ONLINE_STATE : uint32_t
{
	ROLE_ONLINE_STATE_ONLINE = 0,		// 在线
	ROLE_ONLINE_STATE_OFFLINE,			// 离线
	ROLE_ONLINE_STATE_LOGOUT,			// 退出
};

NS_COMMON_END

#endif

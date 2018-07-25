#ifndef __FRIEND_MANAGER_H__
#define __FRIEND_MANAGER_H__

#include "macros.hpp"

class friend_manager_t
{
public:
	static bool init();
	static void reload();

public:
	static uint32_t get_friend_cnt1() { return m_friend_cnt1; }
	static uint32_t get_friend_cnt2() { return m_friend_cnt2; }
	static uint32_t get_friend_cnt3() { return m_friend_cnt3; }
	static uint32_t get_friend_cnt4() { return m_friend_cnt4; }
	static uint32_t get_friend_cnt5() { return m_friend_cnt5; }
	static uint32_t get_friend_cnt6() { return m_friend_cnt6; }
	static uint32_t get_friend_cnt7() { return m_friend_cnt7; }
	static uint32_t get_friend_cnt8() { return m_friend_cnt8; }
	static uint32_t get_friendmsg_strlen1() { return m_friendmsg_strlen1; }
	static uint32_t get_friendmsg_log1() { return m_friendmsg_log1; }
	static uint32_t get_friendmsg_log2() { return m_friendmsg_log2; }
	static uint32_t get_friendoutdel_limit() { return m_friendoutdel_limit; }

	static uint32_t get_max_save_time() { return m_max_save_time; }
	static uint32_t get_chat_max_content() { return m_chat_max_content; }

public:
	static uint32_t m_friend_cnt1;			// 最近联系列表数量上限
	static uint32_t m_friend_cnt2;			// 好友列表数量上限
	static uint32_t m_friend_cnt3;			// 黑名单列表数量上限
	static uint32_t m_friend_cnt4;			// 好友申请列表数量上限
	static uint32_t m_friend_cnt5;			// 置顶好友数量上限
	static uint32_t m_friend_cnt6;			// 系统推荐好友单次最多个数
	static uint32_t m_friend_cnt7;			// 系统推荐刷新间隔时间（秒）
	static uint32_t m_friend_cnt8;			// 系统推荐中来自排行榜和同等级段好友最少个数
	static uint32_t m_friendmsg_strlen1;	// 私聊字符字节长度限制
	static uint32_t m_friendmsg_log1;		// 私聊记录单个玩家保存条数上限
	static uint32_t m_friendmsg_log2;		// 私聊记录总量保存条数上限
	static uint32_t m_friendoutdel_limit;	// 自动删除好友时最少连续离线天数

	static uint32_t m_max_save_time;		// 好友聊天离线信息服务器保存秒数
	static uint32_t m_chat_max_content;		// 好友聊天离线信息服务器保存条数
};

#endif

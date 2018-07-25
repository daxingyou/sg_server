#ifndef __FRIEND_MGR_H__
#define __FRIEND_MGR_H__

#include <unordered_map>
#include <list>
#include <vector>

#include "friend_common.hpp"
#include "client.pb.h"
#include "server.pb.h"

typedef std::unordered_map<uint64_t, friend_info_ptr> friend_info_map;
typedef std::list<friend_info_ptr> friend_contact_list;
typedef std::list<friend_info_ptr> friend_apply_list;

class friend_mgr_t
{
public:
	friend_mgr_t(uint64_t role_uid);

	// 加载所有社会关系
	void load_all_relations();
	// 释放
	void clear_all();

public:

	// 清理好友申请列表
	void clear_apply_list(uint32_t del_num);
	// 清理最近联系列表
	void clear_contact_list(uint32_t del_num);
	// 直接删除一个社会关系
	bool del_relation(uint64_t friend_uid);
	// 得到好友信息
	friend_info_ptr get_friend_by_id(uint64_t friend_uid);
	// 清除好友申请信息
	bool clear_apply_info(friend_info_ptr p_friend);
	// 清除好友最近联系信息
	bool clear_contact_info(friend_info_ptr p_friend);
	// 清除好友制定信息
	bool clear_top_info(friend_info_ptr p_friend);

	// 有人添加我为好友，记录好友申请时间
	void add_apply_time(uint64_t friend_uid, uint32_t time_now);
	// 更新一个社会关系，是新记录还是只修改社会关系类型
	void update_friend_info(proto::common::friend_type old_type, friend_info_ptr p_friend, bool is_new_friend);
	// 更新好友关系为双向好友
	void update_friend_two_side(uint64_t friend_uid);
	// 更新好友关系为单向好友
	void update_friend_one_side(uint64_t friend_uid);

	// 更新最近联系时间(如果没有在我的列表中就新增加为陌生人，并通知客户端)
	bool update_contact_time(uint64_t friend_uid, uint32_t time_now, bool is_need_send = true);

	// 得到关系
	proto::common::friend_type get_friend_type_by_uid(uint64_t friend_uid);

public:
	// 得到所有好友列表
	void get_friend_list(proto::client::ec_get_friends_reply& reply);
	// 添加一个好友
	void add_friend_by_uid(uint64_t friend_uid, proto::client::ec_add_friend_reply& reply);
	// 删除一个好友
	void delete_friend_by_uid(uint64_t friend_uid, proto::client::ec_remove_friend_reply& reply);
	// 拉黑一个好友
	void black_friend_by_uid(uint64_t friend_uid, proto::client::ec_black_friend_reply& reply);
	// 好友列表查询推荐好友
	void get_recommend_friends(proto::client::ec_refresh_recommend_friends_list_reply& reply);
	// 置顶某个好友
	void top_friend_by_uid(uint64_t friend_uid, proto::client::ec_top_friend_reply& reply);
	// 取消置顶某个好友
	void cancel_top_friend_by_uid(uint64_t friend_uid, proto::client::ec_cancel_top_friend_reply& reply);
	// 清除7天未在线玩家
	void clear_friends_not_login_long_time(proto::client::ec_clear_friends_reply& reply);
	// 清理最近联系人
	void clear_recent_contact_friends();
	// 清理黑名单
	void clear_all_black_friends();
	// 清理好友申请列表
	void clear_all_apply_friends();
	// 取消拉黑
	void cancel_black_friend_by_uid(uint64_t friend_uid, proto::client::ec_cancel_black_friend_reply& reply);
	// 得到最近联系人列表
	void get_recent_friends_list(proto::client::ec_get_recent_friends_reply& reply);
	// 得到黑名单列表
	void get_black_friends_list(proto::client::ec_get_black_friends_reply& reply);
	// 得到好友申请人列表
	void get_apply_friends_list(proto::client::ec_get_friend_applier_list_reply& reply);

public:
	// 爬塔通知
	void tower_trigger_notify(const proto::server::ge_tower_trigger_notify& msg);
	// 私聊消息广播
	void chat_notice_all_friend(const proto::client::ce_personal_chat_notice_request& msg);
public:
	// 成为双向好友的处理
	void do_two_side_friend(friend_info_ptr p_friend);

protected:
	// 所有的社会关系
	friend_info_map m_friend_relations;
	// 最近联系列表
	friend_contact_list m_contact_list;
	// 最近联系人数量(gcc 实现size() 时间复杂度为O(N))
	uint32_t m_contact_size = 0;
	// 好友申请列表
	friend_apply_list m_apply_list;
	// 好友申请列表数量(gcc 实现size() 时间复杂度为O(N))
	uint32_t m_apply_size = 0;
	// 所有单向好友和双向数量
	uint32_t m_friend_size = 0;
	// 所有黑名单数量
	uint32_t m_black_size = 0;
	// 置顶好友数量
	uint32_t m_top_size = 0;

    uint64_t m_role_uid = 0;
};

#endif

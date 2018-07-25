#ifndef __FRIEND_DATA_MGR_H__
#define __FRIEND_DATA_MGR_H__

#include "macros.hpp"

#include "common.pb.h"
#include "client.pb.h"

#include "friend/friend_mgr.hpp"

class friend_data_mgr_t
{
public:
	// 数据是否存在
	static bool is_friend_key_exsit(const std::string& key);
	// 更新申请好友时间
	static bool update_apply_time(uint32_t uid, const std::string& key, uint32_t time);
	// 更新最近联系时间
	static bool update_contact_time(uint32_t uid, const std::string& key, uint32_t time);
	// 更新置顶信息
	static bool update_top_time(uint32_t uid, const std::string& key, bool is_top, uint32_t time = 0);
	// 清除一条好友所有数据
	static bool clear_friend_info(uint32_t uid, const std::string& key);
	// 添加一个新的社会关系
	static void update_all_friend_info(uint64_t uid, friend_info_ptr p_friend);
	// 更新好友类型
	static bool update_friend_type(uint32_t uid, const std::string& key, proto::common::friend_type type);

public:
	// 得到离线数据，好友类型
	static proto::common::friend_type get_offline_friend_type(uint64_t uid, uint64_t friend_uid);
	// 处理离线信息：好友申请时间
	static void update_offline_friend_apply_time(uint64_t uid, uint64_t friend_uid, uint32_t time_now);
	// 处理离线信息：最近联系时间
	static void update_offline_friend_contact_time(uint64_t uid, uint64_t friend_uid, uint32_t time_now);
	// 处理离线信息：修改好友类型
	static void update_offline_friend_type(uint64_t uid, uint64_t friend_uid, proto::common::friend_type type);
	// 处理离线信息：最后一次离线时间
	static uint32_t get_offline_logout_time(uint64_t uid);

public:
	// 加载所有好友信息，初始化最近联系列表和好友申请列表
	static void load_all_friends(uint64_t uid, friend_info_map& friends, 
		friend_contact_list& contact_list, uint32_t& contact_size, 
		friend_apply_list& apply_list, uint32_t& apply_size,
		uint32_t& friend_size, uint32_t& black_size, uint32_t& top_size);

public:
	// 用户信息是否存在
	static bool is_user_key_exsit(uint64_t uid);
	// 加载好友详细信息
	static void update_user_info(friend_info_ptr p_info);
	// 加载好友基本信息
	static void load_user_base_info(uint64_t friend_uid, proto::common::user_info* p_user_info);
	// 通过id得到一个用户信息
	static bool search_user_info_by_id(uint64_t uid, proto::common::user_info* p_user_info);
	// 通过名字得到一个用户信息
	static bool search_user_info_by_name(const std::string& name, proto::common::user_info* p_user_info);
};
#endif

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
	static bool update_apply_time(uint64_t uid, const std::string& key, uint32_t time);
	// 更新最近联系时间
	static bool update_contact_time(uint64_t uid, const std::string& key, uint32_t time);
	// 更新置顶信息
	static bool update_top_time(uint64_t uid, const std::string& key, bool is_top, uint32_t time = 0);
	// 清除一条好友所有数据
	static bool clear_friend_info(uint64_t uid, const std::string& key);
	// 添加一个新的社会关系
	static void update_all_friend_info(uint64_t uid, const friend_info_ptr& p_friend);
	// 更新好友类型
	static bool update_friend_type(uint64_t uid, const std::string& key, proto::common::friend_type type);
	// 更新化身召唤时间
	static bool update_shadow_time(uint64_t uid, const friend_info_ptr& p_friend);
	// 更新今日已经送给好友友情点的次数
	static void update_fp_has_sent_today(uint64_t role_id, uint32_t value);
	// 更新赠送次数的刷新时间
	static void update_refresh_sent_time(uint64_t role_id, uint32_t value);
	// 更新今日已经收取好友友情点的次数
	static void update_fp_has_received_today(uint64_t role_id, uint32_t value);
	// 更新收取次数的刷新时间
	static void update_refresh_received_time(uint64_t role_id, uint32_t value);
	// 更新友情点领取次数限制
	static void update_receive_limit(uint64_t role_id, uint32_t value);
	// 更新友情点赠送次数限制
	static void update_send_limit(uint64_t role_id, uint32_t value);
	// 更新friend_related初始化
	static void update_initialization(uint64_t role_id);
	// 更新已领取的羁绊奖励等级
	static void update_got_relation_award(uint64_t role_id, uint32_t level);
public:
	// 得到离线数据，好友类型
	static proto::common::friend_type get_offline_friend_type(uint64_t uid, uint64_t friend_uid);
	// 处理离线信息：好友申请时间
	static void update_offline_friend_apply_time(uint64_t uid, uint64_t friend_uid, uint32_t time_now);
	// 处理离线信息：最近联系时间
	static void update_offline_friend_contact_time(uint64_t uid, uint64_t friend_uid, uint32_t time_now);
	// 处理离线信息：修改好友类型
	static void update_offline_friend_type(uint64_t uid, uint64_t friend_uid, proto::common::friend_type type);
	// 处理离线信息：修改好友亲密度
	static void update_friend_friendliness_in_redis(uint64_t uid, uint64_t friend_uid, uint32_t friendliness_value);
	// 处理离线信息：修改好友可领取友情点
	static void update_i_can_get_fp_from_friend(uint64_t uid, uint64_t friend_uid, uint32_t value);
	// 修改上次领取友情点的时间戳
	static void update_last_i_got_fp_timestamp(uint64_t uid, uint64_t friend_uid, uint32_t timestamp);
	// 修改上次赠送友情点的时间戳
	static void update_last_i_sent_fp_timestamp(uint64_t uid, uint64_t friend_uid, uint32_t timestamp);
	// 得到离线数据 : 好友亲密度
	static uint32_t get_offline_friend_friendliness(uint64_t uid, uint64_t friend_uid);
	// 得到离线数据 : 今日增加的好友亲密度
	static uint32_t get_offline_friend_friendliness_added_today(uint64_t uid, uint64_t friend_uid);
	// 得到离线数据 : 上次刷新日增亲密度的时间
	static uint32_t get_offline_friend_friendliness_last_refresh_time(uint64_t uid, uint64_t friend_uid);
	// 处理离线数据 : 重置日增亲密度
	static void reset_offline_friend_friendliness_added_today(uint64_t uid, uint64_t friend_uid);
	// 处理离线信息：修改好友日增亲密度
	static void update_offline_friend_friendliness_added_today(uint64_t uid, uint64_t friend_uid, uint32_t friendliness_value_added_today);
	// 处理离线信息：最后一次离线时间
	static uint32_t get_offline_logout_time(uint64_t uid);
	// 得到离线数据: 魅力值
	//static uint32_t get_offline_player_charm(uint64_t uid);
	// 修改魅力值
	//static void update_player_charm(uint64_t uid, uint32_t value);
public:
	// 加载所有好友信息，初始化最近联系列表和好友申请列表
	static void load_all_friends(uint64_t uid, friend_info_map& friends, 
		friend_contact_list& contact_list, uint32_t& contact_size, 
		friend_apply_list& apply_list, uint32_t& apply_size,
		uint32_t& friend_size, uint32_t& black_size, uint32_t& top_size);

	static void load_all_friend_related(uint64_t uid, uint32_t& fp_has_sent_today, uint32_t& refresh_sent_time, uint32_t& fp_has_received_today, 
		uint32_t& refresh_received_time, uint32_t& send_limit, uint32_t& receive_limit, int8_t& initialization, uint32_t& got_relation_award);
public:
	// 用户信息是否存在
	static bool is_user_key_exsit(uint64_t uid);
	// 加载好友详细信息
	static void update_user_info(friend_info_ptr p_info);
	// 加载好友基本信息
	static void load_user_base_info(uint64_t friend_uid, proto::common::user_info* p_user_info);
	// 通过did得到一个用户信息
	static bool search_user_info_by_did(uint32_t did, proto::common::user_info* p_user_info);
	// 通过uid得到一个用户信息
	static bool search_user_info_by_uid(uint64_t uid, proto::common::user_info* p_user_info);
	// 通过名字得到一个用户信息
	static bool search_user_info_by_name(const std::string& name, proto::common::user_info* p_user_info);
};
#endif

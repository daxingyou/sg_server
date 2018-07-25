#ifndef __FRIEND_MGR_H__
#define __FRIEND_MGR_H__

#include <list>
#include <vector>
#include <map>

#include "friend_common.hpp"
#include "client.pb.h"
#include "server.pb.h"

typedef std::map<uint64_t, friend_info_ptr> friend_info_map;
typedef std::list<friend_info_ptr> friend_contact_list;
typedef std::list<friend_info_ptr> friend_apply_list;

struct Friendliness;
struct troop_member;
typedef boost::shared_ptr<troop_member> troop_member_ptr;
typedef std::vector<troop_member_ptr> troop_member_vec;

enum ChangeFriendlinessErrCode
{
	ChangeFriendlinessOK,
	ChangeFriendlinessReachDayLimit,
	ChangeFriendlinessReachTotalLimit,
	ChangeFriendlinessFailUnknown,
};

class friend_mgr_t
{
public:
	friend_mgr_t(uint64_t role_uid);

	// 加载所有社会关系
	void load_all_relations();
	// 加载所有好友相关数据
	void load_all_friend_related();
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
	// 召唤一个化身
	uint32_t summon_shadow_request(uint64_t friend_uid);

public:
	// 爬塔通知
	void tower_trigger_notify(const proto::server::ge_tower_trigger_notify& msg);
	// 私聊消息广播
	void chat_notice_all_friend(const proto::client::ce_personal_chat_notice_request& msg);
	//队伍好友推送
	void troop_notify(const troop_member_vec& members);
	//每日推送次数
	uint32_t get_troop_notify_count();
	//好友数量超过多少不推送
	uint32_t get_troop_notify_friend_count();
	//等级达到多少不推送
	uint32_t get_troop_notify_level();
public:
	// 成为双向好友的处理
	void do_two_side_friend(friend_info_ptr p_friend);

	////////////////////////////////////亲密度相关//////////////////////////////////
	static Friendliness* get_config_by_friendliness(int32_t friendliness_value);

	// 清空亲密度
	static void clear_friendliness_value_and_save_data(uint64_t my_role_id, uint64_t friend_role_id, FRIENDLINESS_CHANGE_REASON reason);
	// 更改亲密度的逻辑函数
	static ChangeFriendlinessErrCode change_friendliness_value_logic(int32_t& player_friendliness_value,
		int32_t& player_friendliness_added_today, int32_t& change_value);
	// 更改在线好友的起密度--一般不使用
	static ChangeFriendlinessErrCode change_online_friendliness_value(friend_info_ptr p_friend, int32_t& change_value);
	// 更改好友的亲密度并记录数据
	static void change_friendliness_and_save_data(uint64_t my_role_uid, uint64_t friend_uid, int32_t value, FRIENDLINESS_CHANGE_REASON reason, bool notify_client = true, int32_t* output_real_change = NULL, ChangeFriendlinessErrCode* output_err_code = NULL);
	// 判断是否需要重置日增亲密度
	static bool check_if_need_reset_friendliness_added_today(uint32_t last_refresh_time);
	// 重置日增
	static void reset_friendliness_added_today(int32_t& friendliness_added_today, uint32_t& friendliness_refresh_time, uint32_t now);
	//通知玩家亲密度变化
	void notify_player_friendliness_change(uint64_t friend_role_id, const std::string& friend_role_name, int32_t change_value, int32_t err_code, proto::common::change_friendliness_reason reason);
	//通知gameserver亲密度增加
	static void notify_game_friendliness_change(uint64_t my_role_id, uint64_t friend_role_id, int32_t change_value, 
		proto::common::change_friendliness_reason reason);
	//每日更新相关
	static void update_online(uint32_t timestamp);
	//重置个人所有relation的亲密度日增
	void reset_fat_in_my_relations(uint32_t now, bool reset_without_check);
	//打包玩家的亲密度数据
	void pack_my_friendliness_data(proto::server::eg_get_friendliness_reply& reply);
	void send_game_my_friendliness_data();
	/////////////////////////////////友情点相关///////////////////////////////////
	static void request_gameserver_to_do_friend_point_operation(uint64_t role_id, uint64_t friend_role_id, 
		proto::server::friend_point_operation operation, int32_t parameter1, int32_t parameter2);
	//赠送好友友情点
	int32_t give_friend_friend_point(uint64_t friend_role_id);
	int32_t get_friend_point_from_friend(uint64_t friend_role_id);
	void get_friend_point_from_friend_game_callback(uint64_t friend_role_id, uint32_t value, int32_t reply_code, const proto::common::role_change_data* p_change_data);
	//消息相关
	void send_msg_to_client_of_get_friend_point_reply(uint64_t friend_uid, int32_t reply_code, uint32_t get_timestamp, const proto::common::role_change_data* p_change_data);
	void send_msg_to_client_of_give_friend_point_reply(uint64_t friend_uid, int32_t reply_code, uint32_t give_timestamp, int32_t change_friendliness);
	//存储相关
	void save_fp_has_sent_today();
	void save_refresh_sent_time();
	void save_fp_has_received_today();
	void save_refresh_received_time();
	void save_send_limit();
	void save_receive_limit();
	void save_initialization();
	void save_got_relation_award();

	//重置友情点赠送和收取次数
	void reset_fp_sent_count(uint32_t now, bool reset_without_check);
	void reset_fp_received_count(uint32_t now, bool reset_without_check);
	// 判断是否需要重置友情点赠送和收取次数
	static bool check_if_need_reset_fp_count(uint32_t last_refresh_time);
	// 判断上次赠送时间是否过期
	static bool check_if_last_send_time_expired(uint32_t last_send_time);
	// 判断上次领取时间是否过期
	static bool check_if_last_receive_time_expired(uint32_t last_receive_time);
	// 设置该role对于friend的可领取友情点
	static void set_can_get_fp(uint64_t role_id, uint64_t friend_id, uint32_t value);

	///////////////////////羁绊奖励/////////////////////////////
	//uint32_t check_if_can_get_relation_award(uint32_t old_friendliness, uint32_t new_friendliness);
	uint32_t check_if_can_get_relation_award();
	//获得满足羁绊奖励条件的好友人数
	uint32_t get_satisfied_relation_award_friends_quantity(uint32_t level);
	bool check_if_can_get_relation_award_by_level(uint32_t level);
	bool check_if_has_got_relation_award(uint32_t level);
	void mark_has_got_relation_award(uint32_t level);
	void get_relation_award(uint32_t level);
	void get_relation_award_callback(uint32_t level, int32_t reply_code, const proto::common::role_change_data* p_change_data);
	void send_msg_to_client_of_get_relation_award_reply(uint32_t level, int32_t reply_code, const proto::common::role_change_data* p_change_data);
	//更新当前羁绊等级
	void update_current_relation_level();
	//获取当前可领取的羁绊奖励(按位显示，比如第3位为1表示可领取3级羁绊奖励)
	uint32_t get_can_get_relation_award();
	//告诉客户端当前可领取的羁绊奖励
	void notify_client_can_get_relation_award();
	//告诉客户端当前的羁绊界面数据
	void tell_client_data_on_relation_interface();
	/////////////////////礼物//////////////////////////////////
	void give_present(uint64_t target_role_id, uint32_t item_id, uint32_t item_quantity, bool confirm);
	void give_present_callback(uint64_t target_role_id, int32_t reply_code, const proto::common::role_change_data* p_change_data, 
		uint32_t item_id, uint32_t item_quantity, uint32_t item_add_friendliness, uint32_t item_add_charm, const std::string& item_name, uint32_t language_id);
	void send_msg_to_client_of_give_present_reply(uint64_t target_role_id, int32_t reply_code, const proto::common::role_change_data* p_change_data, uint32_t add_friendliness);
	static uint32_t add_charm(uint64_t target_role_id, uint32_t value);
	static void add_charm_logic(uint32_t& charm_value, uint32_t add_value);
	static void notify_player_get_present(uint32_t item_id, uint32_t item_quantity, uint32_t current_charm, uint64_t giver_role_id, 
		const std::string& giver_role_name, uint64_t player_role_id, uint32_t add_friendliness);
public:
	//上线时更新数据
	void modify_friendliness_data_on_player_enter();
	void modify_relation_award_data_on_player_enter();

	friend_apply_list get_apply_list_copy(){return m_apply_list;}
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


	//今日已经送给好友友情点的次数
	uint32_t m_fp_has_sent_today = 0;
	//上次刷新赠送次数的时间戳
	uint32_t m_last_refresh_sent_time = 0;
	//今日已经收取好友友情点的次数
	uint32_t m_fp_has_received_today = 0;
	//上次刷新领取次数的时间戳
	uint32_t m_last_refresh_received_time = 0;
	// 每日赠送友情点的额外次数上限
	uint32_t m_fp_send_limit = 0;
	// 每日领取友情点的额外次数上限
	uint32_t m_fp_receive_limit = 0;
	// 已领取的羁绊奖励
	uint32_t m_got_relation_award = 0;
	// 当前的羁绊奖励等级
	uint32_t m_current_relation_level = 0;
	// 魅力值
	//uint32_t m_charm = 0;
public:
	// 上次更新全体在线亲密度的时间戳 fat=friendliness added today
	static uint32_t s_m_last_reset_fat_time;
	// 上次更新全体在线友情点赠送收取次数记录的时间戳
	static uint32_t s_m_last_reset_fp_count_time;
	//// 每日赠送友情点的默认次数上限
	//static uint32_t s_m_fp_send_initial_limit;
	//// 每日领取友情点的默认次数上限
	//static uint32_t s_m_fp_receive_initial_limit;
	void load_compelete();
	bool has_data_loaded() { return m_data_loaded; }
	bool m_game_requested = false;
private:
	bool m_data_loaded = false;
};

#endif

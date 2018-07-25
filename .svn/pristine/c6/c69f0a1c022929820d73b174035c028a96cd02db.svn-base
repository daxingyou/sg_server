#pragma once

#include "family.hpp"
#include "user/global_user.hpp"
#include "server.pb.h"
#include "client.pb.h"

#include <deque>

class family_manager_t
{
public:
	typedef std::map<uint64_t, family_ptr> family_map;
	static family_map m_family_map;

	struct family_sort_t
	{
		uint64_t m_family_id = 0;
		uint32_t m_family_lv = 0;
		uint32_t m_prestige = 0;
		std::string m_family_name = "";

		family_sort_t(uint64_t family_id, uint32_t family_lv, const std::string& family_name, uint32_t prestige) 
			: m_family_id(family_id), m_family_lv(family_lv), m_family_name(family_name), m_prestige(prestige) 
		{
		}

		family_sort_t(const family_sort_t& ref)
		{
			this->m_family_id = ref.m_family_id;
			this->m_family_lv = ref.m_family_lv;
			this->m_prestige = ref.m_prestige;
			this->m_family_name = ref.m_family_name;
		}

		family_sort_t& operator =(const family_sort_t& ref)
		{
			this->m_family_id = ref.m_family_id;
			this->m_family_lv = ref.m_family_lv;
			this->m_prestige = ref.m_prestige;
			this->m_family_name = ref.m_family_name;

			return *this;
		}

		friend bool operator< (const family_sort_t& lhs, const family_sort_t& rhs)
		{
			if (lhs.m_prestige != rhs.m_prestige)
			{
				return lhs.m_prestige > rhs.m_prestige;
			}
			
			if (lhs.m_family_lv != rhs.m_family_lv)
			{
				return lhs.m_family_lv > rhs.m_family_lv;
			}

			return lhs.m_family_id < rhs.m_family_id;
		}
	};

	typedef std::vector<family_sort_t> family_sort_vec;
	static family_sort_vec m_family_sort_vec;

public:
	// 起服加载所有数据
	static bool init();
	// 清理
	static bool close();

	// 时间回调
public:
	// 每日更新回调
	static void daily_update_callback(uint32_t id, bool is_in_time);

	// 对战表回调
	static void war_init_callback(uint32_t id, bool is_in_time);
	// 备战回调
	static void war_prepare_callback(uint32_t id, bool is_in_time);
	// 开战回调
	static void war_start_callback(uint32_t id, bool is_in_time);

public:
	// 家族最大申请人数
	static uint32_t get_max_apply_size();

	// 得到建筑默认等级
	static uint32_t get_build_define_level(proto::common::family_build_type type);

	// 生成一个家族唯一ID
	static uint64_t get_new_family_uid();

public:
	// 创建家族
	static void create_family_from_game(const global_user_ptr& p_user, 
		const std::string& name, const std::string& declaration, 
		proto::server::eg_create_family_reply& reply);

	// 删除家族
	static bool del_family_when_no_member(uint64_t family_id);

	// 家族排名
	static void process_family_sort();

	// 邀请加入家族
	static uint32_t invite_family_member(const global_user_ptr& p_user, uint64_t invite_uid);

	// 请求加入家族
	static void join_family_request(const global_user_ptr& p_user, uint64_t family_id, proto::client::ec_join_family_reply& reply);

	// 拒绝加入家族
	static void decline_join_family(const global_user_ptr& p_user, uint64_t uid, proto::client::ec_decline_join_family_reply& reply);

	// 一键拒绝加入家族
	static void one_key_decline_join_family(const global_user_ptr& p_user, proto::client::ec_one_key_decline_join_family_reply& reply);

	// 同意加入家族
	static void approve_join_family(const global_user_ptr& p_user, uint64_t uid, proto::client::ec_approve_join_family_reply& reply);

	// 加入邀请回复
	static uint32_t reply_family_invite(const global_user_ptr& p_user, const proto::common::family_invite_result& result);

	// 一键同意加入家族
	static void one_key_approve_join_family(const global_user_ptr& p_user, proto::client::ec_one_key_approve_join_family_reply& reply);

	// 退家族
	static void leave_family(const global_user_ptr& p_user, proto::client::ec_leave_family_reply& reply);

	// 任命官职
	static uint32_t assign_member(const global_user_ptr& p_user, uint64_t uid, uint32_t officer);

	// 罢免官员
	static uint32_t fire_member(const global_user_ptr& p_user, uint64_t uid);

	// 开出成员
	static uint32_t kick_member(const global_user_ptr& p_user, uint64_t uid);

	// 得到家族列表
	static void get_family_list(uint32_t page, proto::client::ec_get_family_list_reply& reply);

	// 模糊查询
	static void get_family_fuzzy_query(const std::string& name, proto::client::ec_family_fuzzy_query_reply& reply);

	// 得到家族信息
	static void get_family_info(const global_user_ptr& p_user, proto::client::ec_get_family_info_reply& reply);

	// 得到家族成员信息
	static void get_family_member_list(const global_user_ptr& p_user, proto::client::ec_get_family_member_list_reply& reply);

	// 设置自动成为学徒
	static void toggle_auto_newbie(const global_user_ptr& p_user, proto::client::ec_toggle_auto_newbie_reply& reply);

	// 设置宣言
	static void modify_family_declaration(const global_user_ptr& p_user, const std::string& declaration, proto::client::ec_modify_family_declaration_reply& reply);

	// 设置公告
	static void modify_family_notification(const global_user_ptr& p_user, const std::string& notification, proto::client::ec_modify_family_notification_reply& reply);

	// 得到家族申请列表
	static void get_family_apply_list(const global_user_ptr& p_user, proto::client::ec_family_apply_list_reply& reply);

	// 得到建筑列表
	static void get_family_build_list(const global_user_ptr& p_user, proto::client::ec_get_family_build_info_reply& reply);

	// 升级建筑
	static void upgrade_family_build(const global_user_ptr& p_user, proto::common::family_build_type type, proto::client::ec_upgrade_family_build_reply& reply);

	// 清理建筑cd
	static void clear_family_build_cd(const global_user_ptr& p_user, proto::common::family_build_type type, proto::client::ec_clear_family_build_upgrade_cd_reply& reply);

	// 更新荣耀值
	static void family_update_glory(uint64_t role_uid, uint64_t family_id, uint32_t glory);
	
public:
	// 检测名字合法性
	static uint32_t is_family_name_vaild(const std::string& name);

	// 通知chat家族成员变化
	static void notify_chat_member_change(uint64_t family_id, uint64_t role_uid, uint32_t change_type);

	// 权限检查
	static bool has_family_privilige(uint32_t officer, family_privilige_type type);

public:
	static family_ptr get_family(uint64_t family_id);

	//////////////////////////////////////////////////////////////////////////
public:
	// 家族战对战数据
	struct family_war_info_t
	{
		proto::server::global_data_family_war family_war_data;
		uint32_t family_war_time = 0;	// 战斗开始时间
	};

	static family_war_info_t m_family_war_info;	// 家族对战状态

	struct family_against_data_t
	{
		uint64_t m_war_scene_id = 0;
		proto::common::family_against_data m_data;
	};

	typedef std::map<uint32_t, family_against_data_t> against_data_map;
	static against_data_map m_against_data_map;					// 家族战对战信息
	static proto::common::family_against_list m_against_list;	// 对战信息序列化加速访问

	typedef std::map<uint64_t, uint32_t> family_against_id_map;
	static family_against_id_map m_family_against_id_map;

	typedef std::deque<proto::common::family_war_history_data> history_data_deque;
	static history_data_deque m_history_deque;					// 历史信息

	struct score_sort_t
	{
		uint32_t m_id = 0;
		uint64_t m_family_id = 0;
		uint32_t m_score = 0;
		uint32_t m_prestige = 0;

		score_sort_t(uint32_t id, uint64_t family_id, uint32_t score, uint32_t prestige) : m_id(id), m_family_id(family_id), m_score(score), m_prestige(prestige) {};

		friend bool operator< (const score_sort_t& lhs, const score_sort_t& rhs)
		{
			if (lhs.m_score != rhs.m_score)
			{
				return lhs.m_score > rhs.m_score;
			}

			if (lhs.m_prestige != rhs.m_prestige)
			{
				return lhs.m_prestige > rhs.m_prestige;
			}

			return lhs.m_family_id < rhs.m_family_id;
		}
	};

public:
	static uint32_t get_week_id(uint32_t t);
	static uint32_t get_war_time(uint32_t id);

	// 生成对战列表
	static void update_against_list();

	// 更新新的历史数据
	static void update_new_history();

	// 生成对战列表
	static bool generate_war_list(uint32_t week_id);

	// 对对战家族按积分排序，并记录排名
	static void rank_war_list();

	// 设置家族战结束
	static void family_war_end();

public:
	// 家族战副本返回
	static void prepare_family_war_reply(uint32_t game_id, const proto::server::ge_prepare_family_war_reply& reply);

	// 得到当前对战信息
	static void get_family_war_info(proto::client::ec_get_family_war_list_reply& reply);

	// 得到历史对战信息
	static void get_history_info(uint32_t index, proto::client::ec_get_family_war_history_reply& reply);

	// 请求进入家族战副本
	static uint32_t ask_enter_family_war_dungeon(const global_user_ptr& p_user);

	// 家族战提前结束
	static void family_war_early_result(const proto::server::family_war_result_data& data);

	// 家族战结果
	static void family_war_result(const proto::server::ge_family_war_result_notify& results);

public:
	// 得到家族祈愿列表
	static void get_family_prayer_list(const global_user_ptr& p_user, proto::client::ec_family_prayer_list_reply& reply);
	// 得到祈愿奖励列表
	static void get_prayer_gift_reward(const global_user_ptr& p_user, proto::client::ec_prayer_gift_reward_reply& reply);
	// 得到祈愿记录
	static void get_prayer_record(const global_user_ptr& p_user, proto::client::ec_prayer_record_reply& reply);
	// 请求祈愿
	static uint32_t family_prayer_request(const global_user_ptr& p_user, uint32_t hero_id, uint32_t num);
	// 祈愿赠予
	static void family_prayer_gift_request(const global_user_ptr& p_user, 
		uint64_t gift_uid, uint32_t hero_tid, uint32_t hero_material_id, uint32_t rarity, uint32_t hero_num, 
		proto::server::eg_family_prayer_gift_failed_notify& reply);

public:
	// 遍历有没满足建国的家族
	static family_ptr is_can_create_country();

public:
	// 家族商店
	static void get_family_shop_data(const global_user_ptr& p_user, proto::client::ec_get_family_shop_reply& reply);
	// 购买
	static uint32_t family_shop_buy_request(const global_user_ptr& p_user, uint32_t index, uint32_t shop_item, uint32_t num);
	// game 购买失败回退次数
	static void family_shop_failed_back(const global_user_ptr& p_user, uint32_t refresh_time, uint32_t index, uint32_t num);
};
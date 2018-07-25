#pragma once

#include "family/family_manager.hpp"

class family_data_mgr_t
{
public:
	/**
	 * \brief 加载所有家族数据
	 */
	static bool load_all_datas(family_manager_t::family_map& familys);

	/**
	 * \brief 加载家族战数据
	 */
	static bool load_all_war_data(uint32_t cond, family_manager_t::against_data_map& against_map, family_manager_t::family_against_id_map& id_map);

	/**
	 * \brief 加载家族战历史数据
	 */
	static bool load_all_war_history_data(uint32_t cond, proto::common::family_war_history_data& history_data);

	/**
	 * \brief 是否有这个家族
	 */
	static bool is_family_key_exsit(uint64_t family_id);

	/**
	 * \brief 这个名字是否已经被使用
	 */
	static bool is_family_name_exsit(const std::string& name);

	/**
	 * \brief 家族名字
	 */
	static std::string get_family_name(uint64_t family_id);

	/**
	 * \brief 这个家族成员是否存在
	 */
	static bool is_member_key_exsit(uint64_t member_id);

	/**
	 * \brief 这个key的家族建筑是否存在
	 */
	static bool is_build_key_exsit(const std::string& key);

	/**
	 * \brief 加载家族成员
	 */
	static bool load_all_member(const family_ptr& p_family);

	/**
	* \brief 加载家族建筑
	*/
	static bool load_all_build(const family_ptr& p_family);

	/**
	 * \brief 删除一个家族成员信息
	 */
	static void del_family_member(uint64_t key);

	/**
	 * \brief 加载家族信息
	 */
	static bool load_family_data(const family_ptr& p_family);

	/**
	 * \brief 保存整个家族信息
	 */
	static void save_all(const family_ptr& p_family);

	/**
	 * \brief 保存家族申请列表
	 */
	static void save_apply_list(const family_ptr& p_family);

	/**
	 * \brief 保存所有建筑
	 */
	static void save_all_build(const family_ptr& p_family);

	/**
	* \brief 保存所有成员
	*/
	static void save_all_member(const family_ptr& p_family);

	/**
	 * \brief 保存成员信息
	 */
	static void save_one_member(const family_member_ptr& p_family_member);

	/**
	* \brief 保存建筑信息
	*/
	static void save_one_build(const family_build_ptr& p_family_build);

	/**
	 * \brief 删除一个家族
	 */
	static void del_family(uint64_t family_id);

	/**
	 * \brief 保存对战数据
	 */
	static void save_all_war_data(uint32_t week_id, const family_manager_t::against_data_map& against_map);

public:
	// 更新家族资金
	static void update_family_money(uint64_t family_id, uint32_t val);
	// 更新维护资金
	static void update_family_maintain(uint64_t family_id, uint32_t val, bool is_low);
	// 更新繁荣度
	static void update_family_prosperity(uint64_t family_id, uint32_t val);
	// 更新繁荣度加成
	static void update_online_prosperity_add_daily(uint64_t family_id, uint32_t val);
	// 更新繁荣度加成
	static void update_chat_prosperity_add_daily(uint64_t family_id, uint32_t val);
	// 更新繁荣度加成
	static void update_task_prosperity_add_daily(uint64_t family_id, uint32_t val);
	// 更新威望
	static void update_family_prestige(uint64_t family_id, uint32_t val);
	// 更新是否自动加入家族
	static void update_family_auto_newbie(uint64_t family_id, bool val);
	// 设置宣言
	static void update_family_declaration(uint64_t family_id, const std::string& val);
	// 设置公告
	static void update_family_notification(uint64_t family_id, const std::string& val);
	// 更新等级
	static void update_family_level(uint64_t family_id, uint32_t val);
	// 更新国家id
	static void update_family_country(uint64_t family_id, uint32_t val);
	// 更新家族国家忠诚度
	static void update_family_loyalty(uint64_t family_id, uint32_t val);
	// 更新退出国家次数
	static void update_quit_country(uint64_t family_id, uint32_t quit_time, uint32_t quit_count);

public:
	// 更新家族成员官职
	static void update_family_member_officer(uint64_t uid, uint32_t val);
	// 更新家族历史贡献
	static void update_total_family_contribution(uint64_t uid, uint32_t val);
	// 更新祈愿贡献
	static void update_prayer_contribution(uint64_t uid, uint32_t val);
	// 加载祈愿的次数数据
	static void load_prayer_info(const family_member_ptr& p_family_member);
	// 保存祈愿次数每日数据
	static void update_day_prayer_info(const family_member_ptr& p_family_member);
	// 保存祈愿剩余次数
	static void update_prayer_times(uint64_t uid, uint32_t val);
	// 保存每日祈愿奖励次数
	static void update_prayer_rwd_times(uint64_t uid, uint32_t val);
	// 保存祈愿次数每周数据
	static void update_week_prayer_info(const family_member_ptr& p_family_member);
	// 保存祈愿数据
	static void update_prayer_hero_info(const family_member_ptr& p_family_member);
	// 保存祈愿记录
	static void save_prayer_record(uint64_t uid, const proto::common::family_prayer_record_list& record_list);
	// 更新周奖励次数
	static void update_prayer_gift_times(uint64_t uid, uint32_t val);

public:
	// 家族成员限购信息
	static void load_member_shop_info(const family_member_ptr& p_family_member);
	// 加载家族商店信息
	static bool load_family_shop(const family_ptr& p_family);
	// 保存家族商店信息
	static void save_family_shop(const family_ptr& p_family);
	// 保存家族商店
	static void save_family_shop(uint64_t family_id, const proto::common::family_shop_data& data);
	// 保存家族商店个人信息
	static void save_family_shop_personal(uint64_t uid, const proto::common::family_shop_personal_data& data);
	// 清空家族商店个人信息
	static void clear_family_shop_personal(uint64_t uid);
};
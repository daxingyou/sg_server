#ifndef __GLOBAL_USER_DATA_MGR_H__
#define __GLOBAL_USER_DATA_MGR_H__

#include "common/macros.hpp"

#include "common.pb.h"

#include "user/global_user.hpp"

class global_user_data_mgr_t
{
public:
	/**
	* \brief 名字
	*/
	static std::string get_name(uint64_t uid);
	static void set_name(uint64_t uid, const std::string& name);

	/**
	 * \brief 国家id
	 */
	static uint32_t get_user_country_id(uint64_t uid);
	static void set_user_country_id(uint64_t uid, uint32_t val);

	/**
	* \brief 国家职位
	*/
	static uint32_t get_user_country_officer(uint64_t uid);
	static void set_user_country_officer(uint64_t uid, uint32_t val);

	/**
	* \brief 家族id
	*/
	static uint64_t get_user_family_id(uint64_t uid);
	static void set_user_family_id(uint64_t uid, uint64_t val);

	/**
	 * \brief 家族职位
	 */
	static uint32_t get_user_family_officer(uint64_t uid);
	static void set_user_family_officer(uint64_t uid, uint32_t val);

	/**
	* \brief 家族贡献
	*/
	static uint32_t get_family_contribution(uint64_t uid);
	static void set_family_contribution(uint64_t uid, uint32_t val);

	/**
	* \brief 最后下线时间
	*/
	static uint32_t get_last_logout_time(uint64_t uid);
	static void set_last_logout_time(uint64_t uid, uint32_t val);

	/**
	 * \brief 每日更新的时间记录
	 */
	static uint32_t get_update_time(uint64_t uid);
	static void set_update_time(uint64_t uid, uint32_t val);

	/**
	* \brief 等级
	*/
	static uint32_t get_user_level(uint64_t uid);
	static void set_user_level(uint64_t uid, uint32_t val);

	/**
	* \brief 战力
	*/
	static uint32_t get_user_fighting_value(uint64_t uid);
	static void set_user_fighting_value(uint64_t uid, uint32_t val);

	/**
	* \brief hero_tid
	*/
	static uint32_t get_user_hero_plugin(uint64_t uid);
	static void set_user_hero_plugin(uint64_t uid, uint32_t val);

	/**
	* \brief 排名
	*/
	static void set_rank(uint64_t uid, uint32_t rank_id, uint32_t rank);
	static uint32_t get_rank(uint64_t uid, uint32_t rank_id);

	/**
	* \brief 场景数据
	*/
	static uint64_t get_scene_id(uint64_t uid);
	static uint32_t get_map_tid(uint64_t uid);
	static void set_scene_id(uint64_t uid, uint64_t scene_id, uint32_t map_tid);

	/**
	* \brief 最后一次进的静态场景
	*/
	static uint32_t get_last_map_tid(uint64_t uid);
	static void set_last_map_tid(uint64_t uid, uint32_t val);

public:
	/**
	 * \brief 用户id是否存在
	 */
	static bool is_user_key_exsit(uint64_t uid);

	/**
	* \brief 上线加载基础信息
	*/
	static void load_global_user_common(uint64_t uid, global_user_common_t& common_info);

	/**
	 * \brief 用户创建了一个新账号
	 */
	static void create_new_global_user(uint64_t uid, const std::string& name, uint32_t create_time, uint32_t channel_id);
};

#endif//__GLOBAL_USER_DATA_MGR_H__


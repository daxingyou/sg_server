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

	static uint32_t get_did(uint64_t uid);
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
	 * \brief 最近一次上线时间
	 */
	static uint32_t get_last_login_time(uint64_t uid);
	static void set_last_login_time(uint64_t uid, uint32_t val);

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
	* \brief 每日组队好友推荐次数
	*/
	static uint32_t get_troop_friend_count(uint64_t uid);
	static void set_troop_friend_count(uint64_t uid, uint32_t val);

	/**
	* \brief 等级
	*/
	static uint32_t get_user_level(uint64_t uid);
	static void set_user_level(uint64_t uid, uint32_t val);

	/**
	* \brief 魅力
	*/
	static uint32_t get_user_charm(uint64_t uid);
	static void set_user_charm(uint64_t uid, uint32_t val);

	/**
	* \brief 战力
	*/
	static uint32_t get_user_fighting_value(uint64_t uid);
	static void set_user_fighting_value(uint64_t uid, uint32_t val);

	/**
	* \brief 头像和边框ID
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


	/**
	* \brief 模型ID
	*/
	static uint32_t get_user_plugin(uint64_t uid);
	static void set_user_plugin(uint64_t uid, uint32_t val);
public:
	/**
	 * \brief 用户id是否存在
	 */
	static bool is_user_key_exsit(uint64_t uid);

	/**
	 * \brief 账号是否被封停
	 */
	static bool is_user_ban(uint64_t uid);

	/**
	* \brief 账号封停时间
	*/
	static uint32_t get_user_unblock(uint64_t uid);

	/**
	* \brief 上线加载基础信息
	*/
	static bool load_global_user_common(uint64_t uid, global_user_common_t& common_info);
	/**
	* \brief 加载魅力值
	*/
	//static void load_global_user_common_charm(uint64_t uid, global_user_common_t& common_info);
	/**
	* \brief 角色封停
	*/
	static uint32_t ban_role(uint64_t uid, uint32_t offset_time);
	/**
	* \brief 角色解封
	*/
	static uint32_t unban_role(uint64_t uid);
	/**
	* \brief 角色禁言
	*/
	static uint32_t silence_role(uint64_t uid, uint32_t offset_time);
	/**
	* \brief 角色解禁
	*/
	static uint32_t unsilence_role(uint64_t uid);

	static uint32_t get_silence_time(uint64_t uid);
};

#endif//__GLOBAL_USER_DATA_MGR_H__


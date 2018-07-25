#ifndef __COMMON_SOURCE_TYPE_CONVERTER_HPP__
#define __COMMON_SOURCE_TYPE_CONVERTER_HPP__

#include "common/common_fwd.hpp"
#include "common/log_enum.hpp"
#include "common/assert.hpp"
#include <map>

USING_NS_COMMON;

typedef std::map<log_enum::logs_source_type_t, std::string> desc_map;

class source_type_converter_t
{
public:
    /**
     * 功能描述: 注册初始化所有的source_type描述
     * 返 回 值: @result true  : 注册成功
                 @result false : 注册失败
     */
    static bool init_desc();

    /**
     * 功能描述: 通过source_type查找对应的描述
     * 参数说明: @param [in]  source_type : 描述枚举值
                 @param [out] desc        : 返回的描述内容
     * 返 回 值: @result true  : 查找成功
                 @result false : 查找失败
     */
    static bool find_desc(const log_enum::logs_source_type_t source_type, std::string& desc);
    static bool find_desc(uint32_t source_type, std::string& desc);

private:
    static bool regist_desc(const log_enum::logs_source_type_t source_type, const std::string& desc);

	static bool regist_gm_desc();
    static bool regist_user_desc();
	static bool regist_interior_desc();
	static bool regist_world_cup_desc();
	static bool regist_hero_desc();
	static bool regist_task_desc();
	static bool regist_family_desc();
	static bool regist_trade_desc();
    static bool regist_country_desc();
    static bool regist_friend_desc();
	static bool regist_mail_desc();
	static bool regist_arena_desc();
	static bool regist_pk_desc();
	static bool regist_shop_desc();
	static bool regist_equip_desc();
	static bool regist_tower_desc();
	static bool regist_achieve_desc();
	static bool regist_lifeforce_desc();
	static bool regist_dungeon_desc();
	static bool regist_role_buff_desc();
	static bool regist_goods_desc();
    static bool regist_account_desc();
	static bool regist_new_guide_desc();
	static bool regist_expedition_desc();
	static bool regist_page_desc();
	static bool regist_monster_desc();
	static bool regist_active_reward_desc();
	static bool regist_recharge_desc();
	static bool regist_activity_desc();
	static bool regist_scene_desc();
	static bool regist_offline_arena_desc();
	static bool regist_role_vigour_desc();
	static bool regist_bussiness_act_desc();
	static bool regist_liandan_desc();
	static bool regist_patrol_hire_desc();
	static bool regist_general_event_desc();

	//static bool regist_combat_desc();
	

private:
    static desc_map m_desc_list;
};

#endif

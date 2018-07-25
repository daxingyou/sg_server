#include "source_type_converter.hpp"
#include "common/log.hpp"

desc_map source_type_converter_t::m_desc_list;

bool source_type_converter_t::init_desc()
{
    bool ret = true;

	ret &= regist_gm_desc();
	ret &= regist_user_desc();
	ret &= regist_interior_desc();
	ret &= regist_world_cup_desc();
	ret &= regist_hero_desc();
	ret &= regist_task_desc();
	ret &= regist_family_desc();
    ret &= regist_country_desc();
	ret &= regist_trade_desc();
	ret &= regist_friend_desc();
	ret &= regist_mail_desc();
	ret &= regist_arena_desc();
	ret &= regist_pk_desc();
	ret &= regist_shop_desc();
	ret &= regist_equip_desc();
	ret &= regist_tower_desc();
	ret &= regist_achieve_desc();
	ret &= regist_lifeforce_desc();
	ret &= regist_dungeon_desc();
	ret &= regist_role_buff_desc();
	ret &= regist_goods_desc();
    ret &= regist_account_desc();
	ret &= regist_new_guide_desc();
	ret &= regist_expedition_desc();
	ret &= regist_page_desc();
	ret &= regist_monster_desc();
	ret &= regist_active_reward_desc();
	ret &= regist_recharge_desc();
	ret &= regist_activity_desc();
	ret &= regist_scene_desc();
	ret &= regist_offline_arena_desc();
	ret &= regist_role_vigour_desc();
	ret &= regist_bussiness_act_desc();
	//ret &= regist_combat_desc();
    return ret;
}

bool source_type_converter_t::regist_desc(const log_enum::logs_source_type_t source_type, const std::string& desc)
{
	if (m_desc_list.end() == m_desc_list.find(source_type))
	{
		m_desc_list[source_type] = desc;
		return true;
	}

    log_error("desc list was exist source_type[%d]!", source_type);
	return false;
}

bool source_type_converter_t::find_desc(const log_enum::logs_source_type_t source_type, std::string& desc)
{
	desc = "";

	desc_map::iterator it = m_desc_list.find(source_type);
	if (it != m_desc_list.end())
	{
		desc = it->second;
		return true;
	}

	return false;
}

bool source_type_converter_t::find_desc(uint32_t source_type, std::string& desc)
{
	log_enum::logs_source_type_t type;
	type = log_enum::logs_source_type_t(source_type);

	return find_desc(type, desc);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool source_type_converter_t::regist_gm_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// gm 
	ret &= regist_desc(log_enum::source_type_gm_create_item,                    "gm创建道具");
	ret &= regist_desc(log_enum::source_type_gm_cost_item,                      "gm消耗道具");
	ret &= regist_desc(log_enum::source_type_gm_add_pk,							"gm增加PK值");
	ret &= regist_desc(log_enum::source_type_gm_add_money,						"gm增加钱币");
	ret &= regist_desc(log_enum::source_type_gm_dec_money,						"gm扣除钱币");
	ret &= regist_desc(log_enum::source_type_gm_add_hero,						"gm增加英雄");
	ret &= regist_desc(log_enum::source_type_gm_add_exp,						"gm增加经验");
	ret &= regist_desc(log_enum::source_type_gm_set_level,						"gm设置等级");
	ret &= regist_desc(log_enum::source_type_gm_add_prestige,					"gm增加声望");
	ret &= regist_desc(log_enum::source_type_gm_finish_task,					"gm完成任务");
	ret &= regist_desc(log_enum::source_type_gm_drop,							"gm掉落");
	
	// check size
	uint32_t real_num   = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_gm_end - log_enum::source_type_gm_begin;
	if (real_num != should_number)
	{
		log_error("add gm desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
bool source_type_converter_t::regist_user_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// user
	ret &= regist_desc(log_enum::source_type_role_login, "玩家登入");
	ret &= regist_desc(log_enum::source_type_role_logout_none, "玩家未知原因登出");
	ret &= regist_desc(log_enum::source_type_role_logout_normal, "玩家正常登出");
    ret &= regist_desc(log_enum::source_type_role_logout_pingpong_timeout, "玩家心跳超时暂时离线");
	ret &= regist_desc(log_enum::source_type_role_logout_kick_by_others, "玩家被顶号登出");
	ret &= regist_desc(log_enum::source_type_role_logout_ban_account, "玩家被封号登出");
	ret &= regist_desc(log_enum::source_type_role_logout_reconnect_timeout, "玩家重连超时登出");
	ret &= regist_desc(log_enum::source_type_role_logout_gate_server_close, "gate服务器关闭登出");
	ret &= regist_desc(log_enum::source_type_role_logout_game_server_close, "game服务器关闭登出");
	ret &= regist_desc(log_enum::source_type_role_logout_msg_bomb_kick, "玩家消息轰炸被踢");
	ret &= regist_desc(log_enum::source_type_role_logout_center_close, "中心服断开玩家账号注销");
	ret &= regist_desc(log_enum::source_type_role_logout_cross_server, "跨服账号本场景注销");
	ret &= regist_desc(log_enum::source_type_role_logout_error, "玩家账号发生错误注销");
	ret &= regist_desc(log_enum::source_type_role_create, "创建玩家");
	ret &= regist_desc(log_enum::source_type_role_offline_reward, "下线奖励");
	ret &= regist_desc(log_enum::source_type_role_use_item_task, "使用道具(任务类)");
	ret &= regist_desc(log_enum::source_type_role_luck_gift_return_surplus, "红包返还");
	ret &= regist_desc(log_enum::source_type_role_luck_gift_send, "发红包");
	ret &= regist_desc(log_enum::source_type_role_luck_gift_grab, "抢红包");
	ret &= regist_desc(log_enum::source_type_role_exchange, "货币转换");
	ret &= regist_desc(log_enum::source_type_role_every_day_reset, "每日重置");
	ret &= regist_desc(log_enum::source_type_role_every_week_reset, "每周重置");
	ret &= regist_desc(log_enum::source_type_role_sell_bag_item, "出售背包物品");
	ret &= regist_desc(log_enum::source_type_role_use_item_gift_package, "使用道具(礼包类)");
	ret &= regist_desc(log_enum::source_type_role_collect, "采集");
	ret &= regist_desc(log_enum::source_type_role_use_treasure_item, "使用寻宝道具(组队)");
	ret &= regist_desc(log_enum::source_type_role_use_note_closed, "使用未开启的盗墓者笔记");
	ret &= regist_desc(log_enum::source_type_role_use_note_opened, "使用已开启的盗墓者笔记");
	ret &= regist_desc(log_enum::source_type_role_return_treasure_item, "组队放弃挖宝");
	ret &= regist_desc(log_enum::source_type_role_troop_treasure, "组队挖宝");
	ret &= regist_desc(log_enum::source_type_role_use_item_mount, "使用道具(坐骑)");
	ret &= regist_desc(log_enum::source_type_role_auto_reduce_pk_value, "自动减少PK值");
	ret &= regist_desc(log_enum::source_type_role_challenge, "皇城约战");
	ret &= regist_desc(log_enum::source_type_role_use_item_reduce_pk_value, "使用洗红名道具");
	ret &= regist_desc(log_enum::source_type_role_use_gold_to_revive, "使用金币复活");
	ret &= regist_desc(log_enum::source_type_role_use_item_story, "使用剧情道具");
	ret &= regist_desc(log_enum::source_type_role_use_item_plugin, "使用皮肤道具");
	ret &= regist_desc(log_enum::source_type_role_use_item_source_package, "使用道具(资源礼包类)");
	ret &= regist_desc(log_enum::source_type_role_use_item_add_multi_time, "使用多倍时长道具");
	ret &= regist_desc(log_enum::source_type_role_merage_item,  "合成道具");
	ret &= regist_desc(log_enum::source_type_role_decompose_item,  "分解道具");
	ret &= regist_desc(log_enum::source_type_role_vigour_pill, "使用精力丹");
	ret &= regist_desc(log_enum::source_type_role_use_item_add_rolebuff, "使用道具添加角色buff");
	

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_role_end - log_enum::source_type_role_begin;
	if (real_number != should_number)
	{
		log_error("add user desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_interior_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	//interior
	ret &= regist_desc(log_enum::source_type_interior_task_reward, "内政任务奖励");
	ret &= regist_desc(log_enum::source_type_interior_task_active_item, "内政任务激活");
	ret &= regist_desc(log_enum::source_type_interior_task_finish_pay, "内政完成交付任务");
	ret &= regist_desc(log_enum::source_type_interior_task_finish, "内政完成任务");
	ret &= regist_desc(log_enum::source_type_interior_upgrade_build_cost_item, "内政升级建筑消耗");
	ret &= regist_desc(log_enum::source_type_interior_building_produce, "内政建筑产出");
	ret &= regist_desc(log_enum::source_type_interior_refresh_task, "内政刷新任务");
	ret &= regist_desc(log_enum::source_type_interior_upgrade_task_star_level, "内政升级任务星等级");
	ret &= regist_desc(log_enum::source_type_interior_complete_task, "内政任务完成");
	ret &= regist_desc(log_enum::source_type_interior_battle, "内政战斗");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_interior_end - log_enum::source_type_interior_begin;
	if (real_num != should_number)
	{
		log_error("add interior desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_world_cup_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	//arm
	ret &= regist_desc(log_enum::source_type_world_cup_bet, "演武大赛投注");
	ret &= regist_desc(log_enum::source_type_world_cup_reward_bet, "演武大赛投注返还");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_world_cup_end - log_enum::source_type_world_cup_begin;
	if (real_num != should_number)
	{
		log_error("add world_cup desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_hero_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	//hero
	ret &= regist_desc(log_enum::source_type_hero_levelup, "英雄养成-升级");
	ret &= regist_desc(log_enum::source_type_hero_starup, "英雄养成-升星");
	ret &= regist_desc(log_enum::source_type_hero_advanced, "英雄养成-突破");
	ret &= regist_desc(log_enum::source_type_hero_talentadd, "英雄养成-觉醒");
	ret &= regist_desc(log_enum::source_type_hero_levelup_fate, "英雄养成-羁绊升级");
	ret &= regist_desc(log_enum::source_type_hero_levelup_talent, "英雄养成-天赋升级");
	ret &= regist_desc(log_enum::source_type_hero_active_talent, "英雄养成-天赋觉醒");
	ret &= regist_desc(log_enum::source_type_hero_reset_talent, "英雄养成-天赋重置");
	ret &= regist_desc(log_enum::source_type_hero_arm_strengthen, "英雄养成-神兵强化");
	ret &= regist_desc(log_enum::source_type_hero_recruit, "英雄招募");
	ret &= regist_desc(log_enum::source_type_hero_levelup_skill, "英雄养成-技能升级");
	ret &= regist_desc(log_enum::source_type_hero_luckydraw, "英雄获取-抽卡");
	ret &= regist_desc(log_enum::source_type_hero_wakeup, "英雄养成-觉醒");
	ret &= regist_desc(log_enum::source_type_hero_story_add, "剧情添加");
	ret &= regist_desc(log_enum::source_type_hero_story_remove, "剧情删除");
	ret &= regist_desc(log_enum::source_type_hero_unlock, "武将解锁");
	ret &= regist_desc(log_enum::source_type_hero_buy_luckydraw, "英雄获取-祈福抽卡");
	ret &= regist_desc(log_enum::source_type_hero_open_luckydraw, "国王开启祈福抽卡");
	ret &= regist_desc(log_enum::source_type_hero_unlock_skill, "武将技能解锁");
	ret &= regist_desc(log_enum::source_type_hero_replace_skill, "武将技能替换");
	
	
	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_hero_end - log_enum::source_type_hero_begin;
	if (real_num != should_number)
	{
		log_error("add hero desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_task_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// task
	ret &= regist_desc(log_enum::source_type_task_accept, "接受任务");
	ret &= regist_desc(log_enum::source_type_task_state_change, "改变任务状态");
	ret &= regist_desc(log_enum::source_type_task_finish, "完成任务");
	ret &= regist_desc(log_enum::source_type_task_help_finish, "任务求助");
	ret &= regist_desc(log_enum::source_type_bounty_star_box, "星级宝箱");
	ret &= regist_desc(log_enum::source_type_bounty_finish, "完成国家悬赏");
	ret &= regist_desc(log_enum::source_type_bounty_accept, "接受国家悬赏");
	ret &= regist_desc(log_enum::source_type_bounty_refresh, "刷新国家悬赏");
	ret &= regist_desc(log_enum::source_type_task_refresh_shilian, "刷新试炼任务");
	ret &= regist_desc(log_enum::source_type_task_fight_win, "任务战斗胜利");
	ret &= regist_desc(log_enum::source_type_task_zhuogui_leader, "抓鬼任务队长奖励");
	ret &= regist_desc(log_enum::source_type_task_quick_finish, "任务快速完成奖励");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_task_end - log_enum::source_type_task_begin;
	if (real_number != should_number)
	{
		log_error("[wys] add task desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}


bool source_type_converter_t::regist_family_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// family
	ret &= regist_desc(log_enum::source_type_family_online_prosperity_add, "家族在线增加繁荣度");
	ret &= regist_desc(log_enum::source_type_family_chat_prosperity_add, "家族聊天增加繁荣度");
	ret &= regist_desc(log_enum::source_type_family_task_prosperity_add, "家族任务增加繁荣度");
	ret &= regist_desc(log_enum::source_type_family_do_activity, "家族活动");
	ret &= regist_desc(log_enum::source_type_family_create_cost, "创建家族消耗");
	ret &= regist_desc(log_enum::source_type_family_impeach_cost, "家族弹劾消耗");
	ret &= regist_desc(log_enum::source_type_family_impeach_return, "家族弹劾返还");
	ret &= regist_desc(log_enum::source_type_family_impeach_fail_return, "家族弹劾失败返还");
    ret &= regist_desc(log_enum::source_type_family_create_fail_return, "创建家失败返还");

    ret &= regist_desc(log_enum::source_type_family_create, "创建家族");
    ret &= regist_desc(log_enum::source_type_family_join, "加入家族");
    ret &= regist_desc(log_enum::source_type_family_assign, "家族任命");
	ret &= regist_desc(log_enum::source_type_family_approve_join, "同意加入家族");
	ret &= regist_desc(log_enum::source_type_family_upgrade_build, "升级家族建筑");
	ret &= regist_desc(log_enum::source_type_family_clear_upgrade_build_cd, "清除建筑升级cd");
	ret &= regist_desc(log_enum::source_type_family_toggle_auto_newbie, "是否自动加入学徒");
	ret &= regist_desc(log_enum::source_type_family_modify_declaration, "修改家族宣言");
	ret &= regist_desc(log_enum::source_type_family_modify_notification, "修改家族公告");
	ret &= regist_desc(log_enum::source_type_family_abandon_impeach, "取消弹劾");
	ret &= regist_desc(log_enum::source_type_family_approve_impeach, "响应弹劾");
	ret &= regist_desc(log_enum::source_type_family_kick, "踢掉成员");
	ret &= regist_desc(log_enum::source_type_family_impeach, "发起弹劾");
	ret &= regist_desc(log_enum::source_type_family_delete, "删除家族");
	ret &= regist_desc(log_enum::source_type_family_auto_join, "自动加入家族");
	ret &= regist_desc(log_enum::source_type_family_join_apply_list, "加入家族申请表");
	ret &= regist_desc(log_enum::source_type_family_decline_join, "拒绝加入家族申请");
	ret &= regist_desc(log_enum::source_type_family_decline_all_join, "拒绝所有家族申请");
	ret &= regist_desc(log_enum::source_type_family_leave, "离开家族");

	ret &= regist_desc(log_enum::source_type_family_prayer_distribute, "发布家族祈愿");
	ret &= regist_desc(log_enum::source_type_family_prayer_gift, "家族祈愿赠予");
	ret &= regist_desc(log_enum::source_type_family_prayer_receive, "收到祈愿碎片");
	ret &= regist_desc(log_enum::source_type_family_prayer_gift_back, "家族祈愿赠予失败返还");
	ret &= regist_desc(log_enum::source_type_family_prayer_complete, "家族祈愿赠完成");
	ret &= regist_desc(log_enum::source_type_family_prayer_week_gift_rwd, "家族祈愿赠周奖励");
	ret &= regist_desc(log_enum::source_type_family_prayer_day_time_refresh, "家族祈愿每日刷新");
	ret &= regist_desc(log_enum::source_type_family_prayer_week_time_refresh, "家族祈愿每周刷新");
	ret &= regist_desc(log_enum::source_type_family_prayer_gift_rwd, "家族祈愿每日赠予奖励");
	ret &= regist_desc(log_enum::source_type_family_invite_join, "邀请加入家族");
	ret &= regist_desc(log_enum::source_type_family_money_add, "家族资金增加");
	ret &= regist_desc(log_enum::source_type_family_money_cost, "家族资金消耗");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_family_end - log_enum::source_type_family_begin;
	if (real_number != should_number)
	{
		log_error("add family desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_country_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// country
	ret &= regist_desc(log_enum::source_type_country_period_establish, "国家建国倒计时");
	ret &= regist_desc(log_enum::source_type_country_hegemony, "国家建国时期");
	ret &= regist_desc(log_enum::source_type_country_king_family, "成为国王家族");
	ret &= regist_desc(log_enum::source_type_country_alliance, "国家同盟");
	ret &= regist_desc(log_enum::source_type_country_dismiss_alliance, "解除国家同盟");
	ret &= regist_desc(log_enum::source_type_country_modify_year_name, "修改国家年号");
	ret &= regist_desc(log_enum::source_type_country_modify_notice, "修改国家通知");
	ret &= regist_desc(log_enum::source_type_country_appoint_officer, "任命国家官员");
	ret &= regist_desc(log_enum::source_type_country_fire_officer, "解除国家官员");
	ret &= regist_desc(log_enum::source_type_country_resign_officer, "辞官");
	ret &= regist_desc(log_enum::source_type_country_country_level, "国家实力");
	ret &= regist_desc(log_enum::source_type_country_money_add, "国家资金增加");
	ret &= regist_desc(log_enum::source_type_country_money_cost, "国家资金减少");
	ret &= regist_desc(log_enum::source_type_country_change_name_flag, "国家名称旗帜修改");
	ret &= regist_desc(log_enum::source_type_country_change_name_free, "国家名称旗帜免费修改");
	ret &= regist_desc(log_enum::source_type_country_change_name, "国家名称修改");
	ret &= regist_desc(log_enum::source_type_country_change_flag, "国家旗帜修改");
	ret &= regist_desc(log_enum::source_type_country_change_year_name_free, "国家年号免费修改");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_country_end - log_enum::source_type_country_begin;
	if (real_number != should_number)
	{
		log_error("add country desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_friend_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	//friend
	ret &= regist_desc(log_enum::source_type_friend_add_friend, "添加好友");
	ret &= regist_desc(log_enum::source_type_friend_remove_friend, "删除好友");
	ret &= regist_desc(log_enum::source_type_friend_black_friend, "拉黑好友");
	ret &= regist_desc(log_enum::source_type_friend_serach_by_id, "id查找玩家");
	ret &= regist_desc(log_enum::source_type_friend_serach_by_name, "名字查找玩家");
	ret &= regist_desc(log_enum::source_type_friend_top_friend, "置顶好友");
	ret &= regist_desc(log_enum::source_type_friend_cancel_top_friend, "取消置顶好友");
	ret &= regist_desc(log_enum::source_type_friend_refresh_recommend_list, "刷新推荐好友");
	ret &= regist_desc(log_enum::source_type_friend_get_recent_friends, "获取最近好友列表");
	ret &= regist_desc(log_enum::source_type_friend_get_black_friends, "获取黑名单");
	ret &= regist_desc(log_enum::source_type_friend_cancel_black_friend, "取消拉黑好友");
	ret &= regist_desc(log_enum::source_type_friend_get_applier_list, "获取好友申请列表");
	ret &= regist_desc(log_enum::source_type_friend_approve_add_friend, "同意添加好友");
	ret &= regist_desc(log_enum::source_type_friend_clear_friends, "清除长时间不在线好友");
	ret &= regist_desc(log_enum::source_type_friend_clear_recent_friends, "清除最近联系人");
	ret &= regist_desc(log_enum::source_type_friend_clear_black_friends, "清除黑名单");
	ret &= regist_desc(log_enum::source_type_friend_clear_friend_appliers, "清除好友申请列表");
	ret &= regist_desc(log_enum::source_type_friend_clear_apply_time, "清除好友申请时间");
	ret &= regist_desc(log_enum::source_type_friend_clear_content_time, "清除最近聊天时间");
	ret &= regist_desc(log_enum::source_type_friend_update_friend_type, "更新好友类型");
	ret &= regist_desc(log_enum::source_type_friend_update_apply_time, "更新好友申请时间");
	ret &= regist_desc(log_enum::source_type_friend_new_relation, "添加一条新的社会关系");
	ret &= regist_desc(log_enum::source_type_friend_update_content_time, "更新最近联系时间");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_friend_end - log_enum::source_type_friend_begin;
	if (real_number != should_number)
	{
		log_error("add friend desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}


bool source_type_converter_t::regist_mail_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	//mail
	ret &= regist_desc(log_enum::source_type_mail_addenda, "邮件-附件道具");
	ret &= regist_desc(log_enum::source_type_mail_new_mail, "邮件-新邮件");
	ret &= regist_desc(log_enum::source_type_mail_open, "邮件-被打开");
	ret &= regist_desc(log_enum::source_type_mail_user_get_addenda, "邮件-被领取");
	ret &= regist_desc(log_enum::source_type_mail_delete, "邮件-被删除");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_mail_end - log_enum::source_type_mail_begin;
	if (real_num != should_number)
	{
		log_error("add mail desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}


bool source_type_converter_t::regist_arena_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// trade
	ret &= regist_desc(log_enum::source_type_arena_buy_ticket, "竞技场购买门票");
	ret &= regist_desc(log_enum::source_type_arena_battle_win, "竞技场战斗胜利");
	ret &= regist_desc(log_enum::source_type_arena_daily, "竞技场跨天");
	ret &= regist_desc(log_enum::source_type_arena_season_finish, "竞技场赛季结束");
	ret &= regist_desc(log_enum::source_type_arena_join, "进入竞技场");
	ret &= regist_desc(log_enum::source_type_arena_battle_lose, "竞技场战斗失败");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_arena_end - log_enum::source_type_arena_begin;
	if (real_number != should_number)
	{
		log_error("[wys] add arena desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}


bool source_type_converter_t::regist_pk_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// pk
	ret &= regist_desc(log_enum::source_type_pk_fight, "发起PK");
	ret &= regist_desc(log_enum::source_type_pk_attacker_win, "pk发起方获胜");
	ret &= regist_desc(log_enum::source_type_pk_attacker_lose, "pk发起方失败");
	ret &= regist_desc(log_enum::source_type_pk_defenser_win, "pk防御方获胜");
	ret &= regist_desc(log_enum::source_type_pk_defenser_lose, "pk防御方失败");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_pk_end - log_enum::source_type_pk_begin;
	if (real_number != should_number)
	{
		log_error("[wys] add pk desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}


bool source_type_converter_t::regist_shop_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// shop user
	ret &= regist_desc(log_enum::source_type_shop_buy, "商店-购买商品扣钱");
	ret &= regist_desc(log_enum::source_type_shop_buy_center_failed_back, "商店-购买失败返还钱");
	ret &= regist_desc(log_enum::source_type_buy_no_limit, "商店-购买没限购的物品");
	ret &= regist_desc(log_enum::source_type_buy_limit, "商店-购买有限购的物品");
	ret &= regist_desc(log_enum::source_type_random_shop_buy, "随机商店-购买商品");
	ret &= regist_desc(log_enum::source_type_random_shop_item, "随机商店-购买物品");
	ret &= regist_desc(log_enum::source_type_random_shop_free_refresh, "随机商店-免费刷新");
	ret &= regist_desc(log_enum::source_type_random_shop_cost_refresh, "随机商店-道具刷新");
	ret &= regist_desc(log_enum::source_type_random_shop_time_refresh, "随机商店-自动刷新");
	ret &= regist_desc(log_enum::source_type_family_shop_buy, "家族商店-购买商品");
	ret &= regist_desc(log_enum::source_type_family_shop_item, "家族商店-购买物品");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_shop_end - log_enum::source_type_shop_begin;
	if (real_num != should_number)
	{
		log_error("add shop desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}


bool source_type_converter_t::regist_equip_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// equip
	ret &= regist_desc(log_enum::source_type_equip_exchange, "装备兑换");
	ret &= regist_desc(log_enum::source_type_equip_remake_attr, "装备基础属性重铸");
	ret &= regist_desc(log_enum::source_type_equip_remake_exattr, "装备判词重铸");
	ret &= regist_desc(log_enum::source_type_equip_smelt, "装备熔炼");
	ret &= regist_desc(log_enum::source_type_equip_on, "装备穿戴");
	ret &= regist_desc(log_enum::source_type_equip_off, "装备卸下");
	ret &= regist_desc(log_enum::source_type_equip_replace_exattr, "装备判词属性替换");
	ret &= regist_desc(log_enum::source_type_equip_smelt_box, "熔炼宝箱领取");
	ret &= regist_desc(log_enum::source_type_equip_strengthen, "装备强化");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_equip_end - log_enum::source_type_equip_begin;
	if (real_number != should_number)
	{
		log_error("[wys] add equip desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}


bool source_type_converter_t::regist_tower_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// tower
	ret &= regist_desc(log_enum::source_type_tower_fight, "手动爬塔");
	ret &= regist_desc(log_enum::source_type_tower_auto_fight, "扫荡");
	ret &= regist_desc(log_enum::source_type_tower_auto_fight_immediately, "立即扫荡");
	ret &= regist_desc(log_enum::source_type_tower_auto_fight_reward, "爬塔奖励");
	ret &= regist_desc(log_enum::source_type_tower_achieve_reward, "爬塔成就奖励");
	ret &= regist_desc(log_enum::source_type_tower_fight_win, "爬塔战斗胜利");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_tower_end - log_enum::source_type_tower_begin;
	if (real_number != should_number)
	{
		log_error("[wys] add tower desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_trade_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// trade
	ret &= regist_desc(log_enum::source_type_trade_buy_item, "贸易购买物品");
	ret &= regist_desc(log_enum::source_type_trade_sell_item, "贸易出售物品");
	ret &= regist_desc(log_enum::source_type_trade_supplement_item, "补充贸易物品"); 
	ret &= regist_desc(log_enum::source_type_trade_gm_add_trade_point, "GM增加贸易点"); 
	ret &= regist_desc(log_enum::source_type_trade_init_trade_point, "初始化贸易点");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_trade_end - log_enum::source_type_trade_begin;
	if (real_number != should_number)
	{
		log_error("[wys] add trade desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}


bool source_type_converter_t::regist_achieve_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// achieve
	ret &= regist_desc(log_enum::source_type_achieve_person_reward, "成就领取奖励");
	ret &= regist_desc(log_enum::source_type_achieve_single_finish, "成就完成");
	ret &= regist_desc(log_enum::source_type_achieve_add_count, "成就增加进度");

	ret &= regist_desc(log_enum::source_type_achieve_main_role_level, "主角达到等级n");
	ret &= regist_desc(log_enum::source_type_achieve_main_role_fight, "主角战斗力达到n");
	ret &= regist_desc(log_enum::source_type_achieve_equip_type_number, "获得n件指定品质的装备");
	
	ret &= regist_desc(log_enum::source_type_achieve_equip_type_same, "集齐n品质套装");
	ret &= regist_desc(log_enum::source_type_achieve_equip_recast, "重铸n次装备");
	ret &= regist_desc(log_enum::source_type_achieve_equip_exattr_number_star, "装备同时拥有n个指定星判词");
	
	ret &= regist_desc(log_enum::source_type_achieve_hero_number, "拥有n名武将");
	ret &= regist_desc(log_enum::source_type_achieve_hero_number_level, "拥有n名指定级武将");
	ret &= regist_desc(log_enum::source_type_achieve_hero_number_type, "拥有指定品质武将n名");
	
	ret &= regist_desc(log_enum::source_type_achieve_hero_number_star, "拥有n名指定星级武将");
	ret &= regist_desc(log_enum::source_type_achieve_hero_number_talent_level, "将n名武将天赋提升至指定级");
	ret &= regist_desc(log_enum::source_type_achieve_hero_number_skill_level, "将n名武将技能提升至指定级");
	
	ret &= regist_desc(log_enum::source_type_achieve_total_copper, "累计获得n个铜币");
	ret &= regist_desc(log_enum::source_type_achieve_shop_buy_number, "在n类商店购物n次");
	ret &= regist_desc(log_enum::source_type_achieve_mount_number_type, "获得指定品质坐骑n匹");

	ret &= regist_desc(log_enum::source_type_achieve_task_finish_number, "完成n个任务");
	ret &= regist_desc(log_enum::source_type_achieve_task_finish_country_number, "完成国家任务n次");
	ret &= regist_desc(log_enum::source_type_achieve_task_finish_family_number, "完成家族任务n次");
	
	ret &= regist_desc(log_enum::source_type_achieve_trade_number, "完成跑商n次");
	ret &= regist_desc(log_enum::source_type_achieve_trade_total_gain_number, "跑商累计利润达到n");
	ret &= regist_desc(log_enum::source_type_achieve_country_bounty_star_number, "完成n星国家悬赏n次");

	ret &= regist_desc(log_enum::source_type_achieve_treasure_activity_number, "完成寻宝活动n次");
	ret &= regist_desc(log_enum::source_type_achieve_treasure_activity_special_event_number, "寻宝活动遭遇n次特殊事件");
	ret &= regist_desc(log_enum::source_type_achieve_channel_chat, "使用n频道n次");

	ret &= regist_desc(log_enum::source_type_achieve_family, "加入一个家族:包括担任官职");
	ret &= regist_desc(log_enum::source_type_achieve_country, "加入一个国家:包括担任官职");
	ret &= regist_desc(log_enum::source_type_achieve_empire_city_challenge_number, "进行n次皇城约战");
	
	ret &= regist_desc(log_enum::source_type_achieve_pk_value, "pk值达到n");
	ret &= regist_desc(log_enum::source_type_achieve_package_copper_number, "包裹内持有n枚铜币");
	ret &= regist_desc(log_enum::source_type_achieve_task_finish_trial_number, "完成[]次试炼任务");

	ret &= regist_desc(log_enum::source_type_achieve_have_friend_number, "累计添加[n]好友");
	ret &= regist_desc(log_enum::source_type_achieve_trade_single_gain, "跑商单次利润达到[]");
	ret &= regist_desc(log_enum::source_type_achieve_task_finish_star_trial_number, "完成[m]星试炼任务[n]次");

	ret &= regist_desc(log_enum::source_type_achieve_total_login, "累计登陆");
	ret &= regist_desc(log_enum::source_type_achieve_succession_login, "连续登陆");
	ret &= regist_desc(log_enum::source_type_achieve_task_finish_chase_ghost_number, "捉鬼任务完成[]次");

	ret &= regist_desc(log_enum::source_type_achieve_chase_ghost_circle_number, "捉鬼任务完成环[]数");
	ret &= regist_desc(log_enum::source_type_achieve_field_boss_kill_number, "野外boss击杀[]数");
	ret &= regist_desc(log_enum::source_type_achieve_arena_battle_single_win_number, "跨服竞技单次[]胜");
	
	ret &= regist_desc(log_enum::source_type_achieve_arena_battle_total_win_number, "跨服竞技总计[]胜");
	ret &= regist_desc(log_enum::source_type_achieve_tower_level, "千层塔达到[]层");
	ret &= regist_desc(log_enum::source_type_achieve_expedition_single_point, "远征单次积分达到[]");
	
	ret &= regist_desc(log_enum::source_type_achieve_expedition_store_buy_number, "远征在神秘商店购买[]次货物");
	ret &= regist_desc(log_enum::source_type_achieve_expedition_store_total_cost_honour, "神秘商店累计消耗[]荣耀值");
	ret &= regist_desc(log_enum::source_type_achieve_expedition_help_others, "远征帮助他人[]次");
	
	ret &= regist_desc(log_enum::source_type_achieve_expedition_challenge_pass_number, "远征挑战15次关卡[m]次");
	ret &= regist_desc(log_enum::source_type_achieve_family_war_number, "参加家族战[]次");
	ret &= regist_desc(log_enum::source_type_achieve_family_war_kill_player_number, "家族战击杀玩家[]次");
	
	ret &= regist_desc(log_enum::source_type_achieve_family_war_dragon_harm, "家族战中对龙柱造成伤害[]");
	ret &= regist_desc(log_enum::source_type_achieve_family_war_occupied_sun_or_moon_number, "家族战中占领日月曜台[]次");
	ret &= regist_desc(log_enum::source_type_achieve_king_war_number, "王城战[]次数");

	ret &= regist_desc(log_enum::source_type_achieve_king_war_kill_player_number, "王城战击杀玩家[]次");
	ret &= regist_desc(log_enum::source_type_achieve_king_war_dragon_harm, "王城战中对龙脉造成伤害[]");
	ret &= regist_desc(log_enum::source_type_achieve_king_war_quick_revive, "王城战中立即复活[]次");

	ret &= regist_desc(log_enum::source_type_achieve_king_war_start_weapon, "王城战中启动神器[]次");
	ret &= regist_desc(log_enum::source_type_achieve_grade_pass_dungeon_number, "以[]评分通关副本[]次");
	ret &= regist_desc(log_enum::source_type_achieve_dungeon_total_pass, "名将挑战累计通过[]次");

	ret &= regist_desc(log_enum::source_type_achieve_rank_type_level, "[]排行榜达到[]名次");
	ret &= regist_desc(log_enum::source_type_achieve_task_finish_exp_circle_number, "经验环任务完成[]次");
	ret &= regist_desc(log_enum::source_type_achieve_task_exp_circle_item_quality_number, "经验环中提交x品质的物品y次");

	ret &= regist_desc(log_enum::source_type_achieve_hero_up_level_number, "提升武将等级x次");
	ret &= regist_desc(log_enum::source_type_achieve_strengthen_equip_number, "强化x次装备");
	ret &= regist_desc(log_enum::source_type_achieve_tower_number, "爬塔x次层");
	ret &= regist_desc(log_enum::source_type_achieve_patrol_kill_monster_group_number, "巡逻中击杀x组怪");


	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_achieve_end - log_enum::source_type_achieve_begin;
	if (real_number != should_number)
	{
		log_error("[wys] add achieve desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_lifeforce_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();

	// lifeforce
	ret &= regist_desc(log_enum::source_type_lifeforce_up_lifestar_level, "命星升级"); 
	ret &= regist_desc(log_enum::source_type_lifeforce_up_lifestate_level, "境界升级");
	ret &= regist_desc(log_enum::source_type_lifeforce_up_lifelabel_level, "命力称号升级");

	// check size
	uint32_t real_number = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_lifeforce_end - log_enum::source_type_lifeforce_begin;
	if (real_number != should_number)
	{
		log_error("[wys] add lifeforce desc failed! real_number[%d] didn't equal to should_number[%d]", real_number, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_dungeon_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// dungeon
	ret &= regist_desc(log_enum::source_type_dungeon_open, "副本-副本开启");
	ret &= regist_desc(log_enum::source_type_dungeon_enter, "副本-副本进入");
	ret &= regist_desc(log_enum::source_type_dungeon_leave, "副本-副本离开");
	ret &= regist_desc(log_enum::source_type_dungeon_pass, "副本-通关");
	ret &= regist_desc(log_enum::source_type_dungeon_first_pass, "副本-首次通关");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_dungeon_end - log_enum::source_type_dungeon_begin;
	if (real_num != should_number)
	{
		log_error("add dungeon desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_role_buff_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// role_buff
	ret &= regist_desc(log_enum::source_type_role_buff_timeout, "玩家BUFF时间结束");
	ret &= regist_desc(log_enum::source_type_role_buff_replace, "玩家BUFF被替换");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_role_buff_end - log_enum::source_type_role_buff_begin;
	if (real_num != should_number)
	{
		log_error("add role_buff desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_goods_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// goods
	ret &= regist_desc(log_enum::source_type_goods_buy, "购买寄售商品");
	ret &= regist_desc(log_enum::source_type_goods_sell, "商品寄售");
	ret &= regist_desc(log_enum::source_type_goods_return, "取回寄售商品");
	ret &= regist_desc(log_enum::source_type_goods_resell, "重新上架商品");
	ret &= regist_desc(log_enum::source_type_goods_profit, "领取寄售收益");
	ret &= regist_desc(log_enum::source_type_goods_cancel, "商品下架");
	
	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_goods_end - log_enum::source_type_goods_begin;
	if (real_num != should_number)
	{
		log_error("add goods desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_account_desc()
{
    bool ret = true;
    uint32_t begin_size = m_desc_list.size();
    // account
    ret &= regist_desc(log_enum::source_type_account_registered,        "账号-新账户注册登陆");
    ret &= regist_desc(log_enum::source_type_account_login,             "账号-老帐户登陆");
    ret &= regist_desc(log_enum::source_type_account_create_role,       "账号-创建角色");

    // check size
    uint32_t real_num = m_desc_list.size() - begin_size;
    uint32_t should_number = log_enum::source_type_account_end - log_enum::source_type_account_begin;
    if (real_num != should_number)
    {
        log_error("add account desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
        return false;
    }

    return ret;
}

bool source_type_converter_t::regist_new_guide_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// new_guide
	ret &= regist_desc(log_enum::source_type_finish_new_guide, "完成新手指导");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_new_guide_end - log_enum::source_type_new_guide_begin;
	if (real_num != should_number)
	{
		log_error("add new guide desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_expedition_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// new_guide
	ret &= regist_desc(log_enum::source_type_expedition_win, "远征胜利");
	ret &= regist_desc(log_enum::source_type_expedition_shop, "远征商店");
	ret &= regist_desc(log_enum::source_type_expedition_award, "远征奖励");
	ret &= regist_desc(log_enum::source_type_expedition_box, "远征宝箱");
	ret &= regist_desc(log_enum::source_type_expedition_refresh, "远征刷新");
	ret &= regist_desc(log_enum::source_type_expedition_help, "远征帮助");
	ret &= regist_desc(log_enum::source_type_expedition_call_help, "远征求助");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_expedition_end - log_enum::source_type_expedition_begin;
	if (real_num != should_number)
	{
		log_error("add expedition desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_page_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// page
	ret &= regist_desc(log_enum::source_type_page_pass, "章节通关");
	ret &= regist_desc(log_enum::source_type_page_task_finish, "章节任务完成"); 
	ret &= regist_desc(log_enum::source_type_page_pass_fight_pve, "章节通关后精英副本PVE战斗胜利");
	ret &= regist_desc(log_enum::source_type_page_reward_login_auto_send, "章节奖励上线自动发放");
	ret &= regist_desc(log_enum::source_type_page_task_quick_finish, "精英章节扫荡奖励");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_page_end - log_enum::source_type_page_begin;
	if (real_num != should_number)
	{
		log_error("add page desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_monster_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// monster
	ret &= regist_desc(log_enum::source_type_dark_monster_die, "怪物-暗雷怪死亡");
	ret &= regist_desc(log_enum::source_type_light_monster_win, "怪物-明雷怪死亡");
	ret &= regist_desc(log_enum::source_type_monster_fight_pve, "怪物-打怪");
	ret &= regist_desc(log_enum::source_type_field_boss_pve, "野外boss");
	ret &= regist_desc(log_enum::source_type_against_pve, "对阵战斗");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_monster_end - log_enum::source_type_monster_begin;
	if (real_num != should_number)
	{
		log_error("add monster desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_active_reward_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// active_reward
	ret &= regist_desc(log_enum::source_type_active_reward_finish, "完成活跃奖励任务");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_active_reward_end - log_enum::source_type_active_reward_begin;
	if (real_num != should_number)
	{
		log_error("add active_reward desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_recharge_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// recharge 
	ret &= regist_desc(log_enum::source_type_recharge_success, "充值成功");
	ret &= regist_desc(log_enum::source_type_recharge_gift, "充值礼包领取");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_recharge_end - log_enum::source_type_recharge_begin;
	if (real_num != should_number)
	{
		log_error("add recharge desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_activity_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// activity
	ret &= regist_desc(log_enum::source_type_activity_done, "活动完成");
	ret &= regist_desc(log_enum::source_type_activity_new_day, "每天重置");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_activity_end - log_enum::source_type_activity_begin;
	if (real_num != should_number)
	{
		log_error("add activity desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_scene_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// scene_log
	ret &= regist_desc(log_enum::source_type_scene_jump, "跳场景");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_scene_end - log_enum::source_type_scene_begin;
	if (real_num != should_number)
	{
		log_error("add scene desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_offline_arena_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// offline_arena_log
	ret &= regist_desc(log_enum::source_type_offline_arena_record_award, "离线竞技场历史最高排名奖励");
	ret &= regist_desc(log_enum::source_type_offline_arena_class_award, "离线竞技场分段晋级奖励");
	ret &= regist_desc(log_enum::source_type_offline_arena_buy, "离线竞技场购买攻打次数");
	ret &= regist_desc(log_enum::source_type_offline_arena_win, "离线竞技场胜利");
	ret &= regist_desc(log_enum::source_type_offline_arena_lose, "离线竞技场失败");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_offline_arena_end - log_enum::source_type_offline_arena_begin;
	if (real_num != should_number)
	{
		log_error("add offline_arena desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}


bool source_type_converter_t::regist_role_vigour_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// vigour_log
	ret &= regist_desc(log_enum::source_type_vigour_buy_add, "精力-购买增加");
	ret &= regist_desc(log_enum::source_type_vigour_time_add, "精力-时间增加");
	ret &= regist_desc(log_enum::source_type_vigour_pill_add, "精力-精力丹增加");
	ret &= regist_desc(log_enum::source_type_vigour_gm_add, "精力-gm增加");
	ret &= regist_desc(log_enum::source_type_vigour_gm_reduce, "精力-gm减少");
	ret &= regist_desc(log_enum::source_type_vigour_dungeon_reduce, "精力-副本消耗");
	ret &= regist_desc(log_enum::source_type_vigour_buy_times_clear, "精力购买次数清理");
	ret &= regist_desc(log_enum::source_type_vigour_buy_times_add, "精力购买次数增加");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_vigour_end - log_enum::source_type_vigour_begin;
	if (real_num != should_number)
	{
		log_error("add vigour desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}

bool source_type_converter_t::regist_bussiness_act_desc()
{
	bool ret = true;
	uint32_t begin_size = m_desc_list.size();
	// monster
	ret &= regist_desc(log_enum::source_type_bussiness_money_tree, "摇钱树奖励");
	ret &= regist_desc(log_enum::source_type_bussiness_seven_day_login, "福利-七日登录奖励");

	// check size
	uint32_t real_num = m_desc_list.size() - begin_size;
	uint32_t should_number = log_enum::source_type_bussiness_activity_end - log_enum::source_type_bussiness_activity_begin;
	if (real_num != should_number)
	{
		log_error("add bussiness_act desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
		return false;
	}

	return ret;
}


// 
// bool source_type_converter_t::regist_combat_desc()
// {
// 	bool ret = true;
// 	uint32_t begin_size = m_desc_list.size();
// 	// scene_log
// 	ret &= regist_desc(log_enum::source_type_combat_start, "战斗开始");
// 	ret &= regist_desc(log_enum::source_type_combat_finish, "战斗结束");
// 
// 	// check size
// 	uint32_t real_num = m_desc_list.size() - begin_size;
// 	uint32_t should_number = log_enum::source_type_combat_end - log_enum::source_type_combat_begin;
// 	if (real_num != should_number)
// 	{
// 		log_error("add combat desc failed! real_num[%d] didn't equal to should_number[%d]", real_num, should_number);
// 		return false;
// 	}
// 
// 	return ret;
// }
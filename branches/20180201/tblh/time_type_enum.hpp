#ifndef __COMMON_TIME_TYPE_CODE__
#define __COMMON_TIME_TYPE_CODE__
#include "macros.hpp"
NS_COMMON_BEGIN
namespace time_type_enum
{
    enum time_type_code : uint32_t
    {
        time_one_day = 1, // 每日更新时间重置 
        time_all_day = 2, // 全天的活动使用 
        time_king_war1 = 8, // 王城争夺战报名时间 
        time_king_war2 = 9, // 王城争夺战备战时间 
        time_king_war3 = 10, // 王城争夺战正式开战时间 
        time_activity9 = 11, // 逐鹿中原 
        time_activity10 = 12, // 皇城争霸战 
        time_activity11 = 13, // 国战 
        time_activity12 = 14, // 税官掠夺 
        time_activity17 = 15, // 家族集会 
        time_activity18 = 16, // 家族战 
        time_pk_battle1 = 17, // 皇城约战广播 
        time_pk_battle2 = 18, // 皇城约战准备开始 
        time_arena1 = 19, // 跨服天梯每天开启时间 
        time_week = 20, // 每周更新时间重置 
        time_family_war1 = 21, // 家族战对战表生成时间 
        time_family_war2 = 22, // 家族战备战时间 
        time_family_war3 = 23, // 家族战开战时间 
        time_shop1 = 100, // 商店每天12点重置 
        time_shop2 = 101, // 商店每周1-12点重置 
        time_shop3 = 102, // 商店每月第1天-12点重置 
        time_shop4 = 103, // 上架每天1点，持续10分钟 
        time_shop5 = 200, // 随机商店刷新时间 
        time_dreamland_hide = 210, // 千层塔（时空幻境）隐藏层出现概率翻倍时间 
        time_chat_get_arena_data = 220, // 聊天服每日获取数据时间（也是攻略获取跨服竞技场数据时间） 
        time_offline_arena = 230, // 离线竞技场结算时间 
        time_country_unlock = 300, // 国家系统开启条件：服务器开启天数 
		
    };
}
NS_COMMON_END
#endif

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
        time_one_month = 3, // 每月更新时间 
        time_king_war1 = 8, // 立国战报名时间 
        time_king_war2 = 9, // 立国战备战时间 
        time_king_war3 = 10, // 立国战正式开战时间 
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
        time_qiyun_01 = 24, // 气运值开启1 
        time_qiyun_02 = 25, // 气运值开启2 
        time_shop1 = 100, // 商店每天5点重置 
        time_shop2 = 101, // 商店每周1-12点重置 
        time_shop3 = 102, // 商店每月第1天-12点重置 
        time_shop4 = 103, // 上架每天1点，持续10分钟 
        time_shop5 = 200, // 随机商店刷新时间 
        time_dreamland_hide = 210, // 千层塔（时空幻境）隐藏层出现概率翻倍时间 
        time_chat_get_arena_data = 220, // 聊天服每日获取数据时间（也是攻略获取跨服竞技场数据时间） 
        time_offline_arena = 230, // 离线竞技场结算时间 
        time_country_unlock = 300, // 国家系统开启条件：服务器开启天数 
        time_world_event = 310, // 天下大势开启时间 
        time_yunying1 = 400, // 运营时间管理测试1，每天10点开启，持续1小时关闭 
        time_yunying2 = 401, // 运营时间管理测试2，开服第3天12点开启 
        time_yunying3 = 402, // 运营时间管理测试3，2018/3/12 21:00:00开启，持续1天关闭 
        time_yunying4 = 999, // 收集兑换开启时间测试，每周一开启，持续1天 
        time_chunjie = 1000, // 2018春节 
        time_guoqin = 1001, // 2018国庆 
        time_wuyueceshi = 1002, // 2018五月测试 
		
    };
}
NS_COMMON_END
#endif

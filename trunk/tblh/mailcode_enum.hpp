#ifndef __COMMON_MAIL_CODE__
#define __COMMON_MAIL_CODE__
#include "macros.hpp"
NS_COMMON_BEGIN
namespace mailcode_enum
{
    enum mail_code : uint32_t
    {
        sys_mail_1 = 1, // ϵͳ����1 
        sys_mail_2 = 2, // ϵͳ����2 
        sys_mail_3 = 3, // ϵͳ����3 
        sys_mail_4 = 4, // ϵͳ����4 
        sys_mail_5 = 5, // ϵͳ����5 
        sys_mail_6 = 6, // ϵͳ����6 
        sys_mail_7 = 7, // ϵͳ����7 
        sys_mail_collect = 8, // �����Ʒ 
        sys_mail_family = 9, // ������� 
        sys_mail_country = 10, // ������� 
        sys_mail_hero = 11, // �佫���� 
        sys_mail_8 = 12, // ���������ʼ� 
        sys_mail_cdkey = 100, // ����һ�ϵͳ 
        sys_mail_date_war = 1200, // Լս�����ʼ� 
        sys_mail_arena_daily_1 = 2001, // ����ÿ�ܶ�λ���� 
        sys_mail_arena_daily_2 = 2002, // ����ÿ�ܶ�λ���� 
        sys_mail_arena_daily_3 = 2003, // ����ÿ�ܶ�λ���� 
        sys_mail_arena_daily_4 = 2004, // ����ÿ�ܶ�λ���� 
        sys_mail_arena_daily_5 = 2005, // ����ÿ�ܶ�λ���� 
        sys_mail_arena_daily_6 = 2006, // ����ÿ�ܶ�λ���� 
        sys_mail_arena_season_1 = 2020, // ѧͽ 
        sys_mail_arena_season_2 = 2021, // ���� 
        sys_mail_arena_season_3 = 2022, // ��Ӣ 
        sys_mail_arena_season_4 = 2023, // ר�� 
        sys_mail_arena_season_5 = 2024, // ��ʦ 
        sys_mail_arena_season_6 = 2025, // ���� 
        sys_mail_arena_season_7 = 2026, // ��˵ 
        sys_mail_arena_season_8 = 2027, // �� 
        sys_mail_arena_season_9 = 2028, // ��ǰ20 
        sys_mail_arena_season_10 = 2029, // �񻰵�3 
        sys_mail_arena_season_11 = 2030, // �񻰵�2 
        sys_mail_arena_season_12 = 2031, // �񻰵�1 
        sys_mail_ranking = 2100, // ���˳�� 
        sys_mail_boss_1 = 3001, // ħ�������˺����� 
        sys_mail_boss_2 = 3002, // ħ�������˺����� 
        sys_mail_boss_3 = 3003, // ħ�������˺����� 
        sys_mail_boss_4 = 3004, // ħ�������˺����� 
        sys_mail_boss_5 = 3005, // ħ�����ּ��影�� 
        sys_mail_boss_6 = 3006, // ħ�����ּ��影�� 
        sys_mail_boss_7 = 3007, // ħ�����ּ��影�� 
        sys_mail_boss_8 = 3008, // ħ�����ּ��影�� 
        sys_mail_boss_9 = 3009, // ħ�����ֻ�ɱ���� 
        sys_mail_treasure_3 = 3101, // �ر�ͼ���������ʼ� 
        sys_mail_family_pray_1 = 3201, // ������Ըÿ�ܽ��� 
        sys_mail_family_pray_2 = 3202, // ������Ը 
        sys_mail_family_demise_leader = 3203, // ���� 
        sys_offline_arena_daily = 3301, // ������ÿ�ս��㽱�� 
        sys_tower_sweep = 3401, // ���̸�����ɨ�������ʾ 
        sys_mail_king_victory_reward = 3501, // ����ս���� 
        sys_mail_king_defeat_reward = 3502, // ����ս���� 
        sys_mail_king_merit_level = 3503, // ����ս���� 
        sys_mail_king_merit_rank = 3504, // ����ս���� 
        sys_mail_king_signup_success = 3505, // ����ս 
        sys_mail_assist_to_main = 3506, // ����ս��ս���� 
        sys_mail_assist_to_friend = 3507, // ����ս��ս���� 
        sys_mail_main_cancel_assist = 3508, // ����սȡ����ս 
        sys_mail_cancel_assist_to_main = 3509, // ����սȡ����ս 
        sys_mail_cancel_assist_to_friend = 3510, // ����սȡ����ս 
        sys_mail_family_war_victory_reward = 3601, // ����ս���� 
        sys_mail_family_war_defeat_reward = 3602, // ����ս���� 
        sys_mail_family_war_merit_level = 3603, // ����ս���� 
        sys_mail_family_war_merit_rank = 3604, // ����ս���� 
        sys_mail_family_war_announce = 3605, // ����ս֪ͨ 
        sys_mail_plot_1 = 1001, // ��ת������Ա 
        sys_mail_plot_2 = 1002, // ���ҹ���Ա 
        sys_mail_plot_3 = 1003, // Զ������Ա 
        sys_mail_level_5 = 1010, // �ȼ����� 
        sys_mail_level_10 = 1011, // �ȼ����� 
        sys_mail_level_15 = 1012, // �ȼ����� 
        sys_mail_level_20 = 1013, // �ȼ����� 
        sys_mail_level_25 = 1014, // �ȼ����� 
        sys_mail_level_30 = 1015, // �ȼ����� 
        sys_mail_level_35 = 1016, // �ȼ����� 
        sys_mail_level_40 = 1017, // �ȼ����� 
        sys_mail_red_envelope_1 = 4001, // ������ 
        sys_country_alliance = 5000, // ����ͬ�� 
        sys_mail_draw = 6000, // �����鿨���� 
        sys_mail_cash_gift_1 = 180001, // ����һ� 
        sys_mail_cash_gift_2 = 180002, // ����һ� 
        sys_mail_cash_gift_3 = 180003, // ����һ� 
        sys_mail_cash_gift_4 = 180004, // ���ؽ������ 
        sys_mail_cash_gift_5 = 180005, // �һ��� 
        sys_mail_cash_gift_6 = 180006, // ����һ� 
        sys_mail_cash_gift_7 = 180007, // ����һ� 
        sys_mail_cash_gift_8 = 180008, // ����һ� 
        partol_mail = 200000, // Ѳ������ 
		
    };
}
NS_COMMON_END
#endif

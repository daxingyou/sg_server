#pragma once
#include "common.pb.h"

//tolua_begin
namespace script
{
	enum combat_act_type
	{
		combat_act_type_skill = proto::common::combat_act_type_skill,
		combat_act_type_buff = proto::common::combat_act_type_buff,
		combat_act_type_buff_remove = proto::common::combat_act_type_buff_remove,
		combat_act_type_tactic_on = proto::common::combat_act_type_tactic_on,
		combat_act_type_tactic_off = proto::common::combat_act_type_tactic_off,
		combat_act_type_dodge = proto::common::combat_act_type_dodge,
		combat_act_type_heal = proto::common::combat_act_type_heal,
		combat_act_type_heal_crit = proto::common::combat_act_type_heal_crit,
		combat_act_type_damage = proto::common::combat_act_type_damage,
		combat_act_type_ext = proto::common::combat_act_type_ext,
		combat_act_type_crit = proto::common::combat_act_type_crit,
		combat_act_type_absorb = proto::common::combat_act_type_absorb,
		combat_act_type_reflect = proto::common::combat_act_type_reflect,
		combat_act_type_leech = proto::common::combat_act_type_leech,
		combat_act_type_share_damage = proto::common::combat_act_type_share_damage,
		combat_act_type_relive = proto::common::combat_act_type_relive,
		combat_act_type_block = proto::common::combat_act_type_block,
		combat_act_type_counter = proto::common::combat_act_type_counter,
		combat_act_type_combo = proto::common::combat_act_type_combo,
		combat_act_type_follow = proto::common::combat_act_type_follow,
		combat_act_type_pincer_attack = proto::common::combat_act_type_pincer_attack,
		combat_act_type_att_speed_inc = proto::common::combat_act_type_att_speed_inc,
		combat_act_type_att_speed_dec = proto::common::combat_act_type_att_speed_dec,
		combat_act_type_kill_leech = proto::common::combat_act_type_kill_leech,
		combat_act_type_recover = proto::common::combat_act_type_recover,
		combat_act_type_syn_cd = proto::common::combat_act_type_syn_cd,
		combat_act_type_syn_hp = proto::common::combat_act_type_syn_hp,
		combat_act_type_syn_wave = proto::common::combat_act_type_syn_wave,
		combat_act_type_beheaded = proto::common::combat_act_type_beheaded,
		combat_act_type_syn_atk_count = proto::common::combat_act_type_syn_atk_count,
		combat_act_type_buff_damage = proto::common::combat_act_type_buff_damage,
		combat_act_type_buff_heal = proto::common::combat_act_type_buff_heal,
		combat_act_type_buff_dot = proto::common::combat_act_type_buff_dot,
		combat_act_type_accept_damage = proto::common::combat_act_type_accept_damage,
		combat_act_type_be_share_damage = proto::common::combat_act_type_be_share_damage,
		combat_act_type_special_skill = proto::common::combat_act_type_special_skill,
		combat_act_type_immuno = proto::common::combat_act_type_immuno,
		combat_act_type_trigger_skill = proto::common::combat_act_type_trigger_skill,
	};
}


/// @brief 输出调试日志
void c_debug( const char* msg );

/// @brief 输出调试日志
void c_info(const char* msg);

/// @brief 输出调试日志
void c_error(const char* msg);

/// @brief 断言失败
void c_assert_fail( const char* msg );

/// @brief 获得脚本根目录
const char* c_get_root_dir();

/// @brief 加载一个脚本文件
bool c_do_file( const char* filename );

/// @brief 获得文件夹下的所有文件名
const char* c_get_file_list_by_folder( const char* folder );

/// @brief 获得随机值
int c_rand_between(int a, int b);

/// @brief 获取万分之的几率
bool c_select_by_tenth(unsigned int tenth);


//tolua_end
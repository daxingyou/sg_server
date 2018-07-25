#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"
USING_NS_COMMON;
/// @brief    表格结构:SkillEffectTableBase
/// @remark   记录大小:136 X32
///           记录大小:184 X64
///           字段数量:34
///           数据文件:j技能效果表.xlsx Sheet1
///           排序主键:技能ID,技能等级
#pragma pack(1)
struct SkillEffectTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0D86F809; ///< II|IIIIIIVVVIIVIIIIIIIIIIVVVVVVVVII
#else
	static const int          FormatCheckCode = 0x0D9F0009; ///< II|IIIIIIvvvIIvIIIIIIIIIIvvvvvvvvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32_move_7(id, level); }; // TODO: 请用 技能ID,技能阶数,技能等级 生成唯一键

	uint32_t                  id;                           ///< 技能ID
	uint32_t                  type;                         ///< 技能类型
	uint32_t                  level;                        ///< 技能等级

	uint32_t                  consum_type;                  ///< 消耗类型
	uint32_t                  consum_value;                 ///< 消耗值
	uint32_t                  cd;                           ///< 冷却时间
	uint32_t                  start_cd;                     ///< 技能开场cd
	uint32_t                  camera_rate;                  ///< 镜头触发概率
	const char*               hurt_rate;                    ///< 伤害系数(分段填写多个系数)
	const char*               dispel_buff1;                 ///< 驱散buff1
	const char*               dispel_buff2;                 ///< 驱散buff2
	uint32_t                  refind_target;                ///< 目标死亡重新查找目标
	uint32_t                  hurt;                         ///< 真实伤害
	const char*               levelup_consume;              ///< 升级消耗
	uint32_t                  levelup_money;                ///< 升级消耗铜币
	uint32_t                  levelup_herosoul;             ///< 升级消耗将魂
	uint32_t                  release_ai;                   ///< 释放AI
	uint32_t                  target_dead_ai;               ///< 目标死亡死AI
	uint32_t                  refind_target_type;           ///< 目标死亡时寻找下一个目标的类型
	uint32_t                  target_type;                  ///< 技能目标类型
	uint32_t                  attack_target;                ///< 技能攻击目标
	uint32_t                  play_time;                    ///< 技能预计时间（总时间，服务器预算）
	uint32_t                  camera_skill_play_time;       ///< 技能+镜头预计时间
	uint32_t                  target_count;                 ///< 随机目标数量
	const char*               buff1;                        ///< buff1
	const char*               buff2;                        ///< buff2
	const char*               buff3;                        ///< buff3
	const char*               gx_buff;                      ///< 光环buff
	const char*               bd_buff;                      ///< 被动buff
	const char*               hp_hurt_rate;                 ///< 体力伤害系数
	const char*               defend_hurt_rate;             ///< 防御伤害系数
	const char*               speed_hurt_rate;              ///< 速度伤害系数
	uint32_t                  ai_id;                        ///< 作用类型编号
	uint32_t                  change_mod_pre_skill;         ///< 变身前对应技能id
};
#pragma pack()

typedef std::vector< std::pair<uint32_t,uint32_t> > buff_info_vec;
typedef Dawn::quintuple<uint32_t, uint32_t, uint32_t, uint32_t, buff_info_vec> add_buff_t;
typedef std::vector<add_buff_t> skill_add_buff_vec;

typedef Dawn::triple<uint32_t, uint32_t, uint32_t> skill_remove_buff_t;
typedef std::vector<skill_remove_buff_t> skill_remove_buff_vec;

typedef std::vector<uint32_t> buff_dispel_info_vec;
typedef Dawn::sextuple<uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, buff_dispel_info_vec> dispel_buff_t;
typedef std::vector<dispel_buff_t> skill_dispel_buff_vec;


struct SkillEffectTable : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  type()                        { return t_type; }
	uint32_t                  level()                       { return t_level; }

	uint32_t                  consum_type()                 { return t_consum_type; }
	uint32_t                  consum_value()                { return t_consum_value; }
	uint32_t                  cd()                          { return t_cd; }
	uint32_t                  start_cd()                    { return t_start_cd; }
	uint32_t                  camera_rate()                 { return t_camera_rate; }
	const std::vector<uint32_t>&        hurt_rate() { return t_hurt_rate; }
	uint32_t							hurt_rate(uint32_t pos) { return pos >= t_hurt_rate.size() ? 0 : t_hurt_rate[pos]; }

	const add_buff_t&			get_gx_buff() { return t_add_gx_buff; }
	const add_buff_t&			get_bd_buff() { return t_add_bd_buff; }

	const std::vector<skill_remove_buff_vec>&        remove_buffs() { return t_remove_buffs; }
	const std::vector<skill_remove_buff_vec>&        remove_self_buffs() { return t_remove_self_buffs; }
	const std::vector<skill_remove_buff_vec>&        remove_friend_buffs() { return t_remove_friend_buffs; }

	uint32_t                  refind_target()               { return t_refind_target; }
	uint32_t                  hurt()                        { return t_hurt; }
	const std::pair<uint32_t, uint32_t>&        levelup_consume()             { return t_levelup_consume; }
	uint32_t                  levelup_money() { return t_levelup_money; }
	uint32_t                  levelup_herosoul() { return t_levelup_herosoul; }
	uint32_t                  release_ai()                  { return t_release_ai; }
	uint32_t                  target_dead_ai()              { return t_target_dead_ai; }
	uint32_t                  refind_target_type()          { return t_refind_target_type; }
	uint32_t                  target_type()                 { return t_target_type; }
	uint32_t                  attack_target()               { return t_attack_target; }
	uint32_t                  play_time()                   { return t_play_time; }
	uint32_t                  camera_skill_play_time()      { return t_camera_skill_play_time; }
	uint32_t                  target_count()                { return t_target_count; }

	const std::vector<uint32_t>&        hp_hurt_rate() { return t_hp_hurt_rate; }
	uint32_t							hp_hurt_rate(uint32_t pos) { return pos >= t_hp_hurt_rate.size() ? 0 : t_hp_hurt_rate[pos]; }
	const std::vector<uint32_t>&        defend_hurt_rate() { return t_defend_hurt_rate; }
	uint32_t							defend_hurt_rate(uint32_t pos) { return pos >= t_defend_hurt_rate.size() ? 0 : t_defend_hurt_rate[pos]; }
	const std::vector<uint32_t>&        speed_hurt_rate() { return t_speed_hurt_rate; }
	uint32_t							speed_hurt_rate(uint32_t pos) { return pos >= t_speed_hurt_rate.size() ? 0 : t_speed_hurt_rate[pos]; }
	uint32_t                  ai_id()                       { return t_ai_id; }
	uint32_t                  change_mod_pre_skill()        { return t_change_mod_pre_skill; }
	
private:
	uint32_t                  t_id;                         ///< 技能ID
	uint32_t                  t_type;                       ///< 技能类型
	uint32_t                  t_level;                      ///< 技能等级

	uint32_t                  t_consum_type;                ///< 消耗类型
	uint32_t                  t_consum_value;               ///< 消耗值
	uint32_t                  t_cd;                         ///< 冷却时间
	uint32_t                  t_start_cd;                   ///< 技能开场cd
	uint32_t                  t_camera_rate;                ///< 镜头触发概率
	std::vector<uint32_t>               t_hurt_rate;                  ///< 伤害系数(分段填写多个系数)
	std::vector<skill_remove_buff_vec>               t_remove_buffs;               ///< 移除目标buff
	std::vector<skill_remove_buff_vec>               t_remove_self_buffs;          ///< 移除施法者buff
	std::vector<skill_remove_buff_vec>               t_remove_friend_buffs;        ///< 移除友方buff
	uint32_t                  t_refind_target;              ///< 目标死亡重新查找目标
	uint32_t                  t_hurt;                       ///< 真实伤害
	std::pair<uint32_t,uint32_t>               t_levelup_consume;            ///< 升级消耗
	uint32_t                  t_levelup_money;              ///< 升级消耗铜币
	uint32_t                  t_levelup_herosoul;           ///< 升级消耗将魂
	uint32_t                  t_release_ai;                 ///< 释放AI
	uint32_t                  t_target_dead_ai;             ///< 目标死亡死AI
	uint32_t                  t_refind_target_type;         ///< 目标死亡时寻找下一个目标的类型
	uint32_t                  t_target_type;                ///< 技能目标类型
	uint32_t                  t_attack_target;              ///< 技能攻击目标
	uint32_t                  t_play_time;                  ///< 技能预计时间（总时间，服务器预算）
	uint32_t                  t_camera_skill_play_time;     ///< 技能+镜头预计时间
	uint32_t                  t_target_count;               ///< 随机目标数量
	uint32_t                  t_ai_id;                      ///< 作用类型编号	
	uint32_t                  t_change_mod_pre_skill;       ///< 变身前对应技能id
	std::vector<uint32_t>               t_hp_hurt_rate;                  ///< 伤害系数(分段填写多个系数)
	std::vector<uint32_t>               t_defend_hurt_rate;              ///< 伤害系数(分段填写多个系数)
	std::vector<uint32_t>               t_speed_hurt_rate;               ///< 伤害系数(分段填写多个系数)

	skill_add_buff_vec			t_add_buff_vec;				/// 添加的buff
	skill_dispel_buff_vec		t_dispel_buff_vec;			/// 驱散的buff
	add_buff_t					t_add_gx_buff;				/// 光环buff
	add_buff_t					t_add_bd_buff;				/// 被动buff

public:
	void reset();
	void fill( const SkillEffectTableBase& base );
	const char* getClassName() const { return "j技能效果表"; }

	static const std::string getBinaryName();

	const skill_add_buff_vec& get_add_buffs(/*uint32_t when, uint32_t to*/)
	{
// 		for (uint32_t i = 0; i < t_add_buff_vec.size(); ++i)
// 		{
// 			if (t_add_buff_vec[i].second == when)
// 				return &(t_add_buff_vec[i]);
// 		}
		return t_add_buff_vec;
	}

	const skill_dispel_buff_vec& get_dispel_buffs(/*uint32_t when, uint32_t to*/)
	{
		return t_dispel_buff_vec;
	}

	bool is_ignore_damage(size_t phase)
	{
		if (phase >= t_hurt_rate.size())
		{
			return true;
		}

		if (t_hurt_rate[phase] == 0)
		{
			return true;
		}

		return false;
	}

private:
	void fill_add_buff_field_single(const char* buff_field, add_buff_t& buff_single)
	{
		if (strlen(buff_field) == 0)
			return;

		buff_info_vec buff_vec;

		typedef Dawn::quintuple<uint32_t, uint32_t, uint32_t, uint32_t, VarType> temp_param;
		temp_param temp_buf;
		Dawn::parseTuple(temp_buf, buff_field, "-");
		Dawn::parseTupleContainer< std::pair< uint32_t, uint32_t > >(buff_vec, (const char*)temp_buf.fifth, ":,");
		buff_single.first = temp_buf.first;
		buff_single.second = temp_buf.second;
		buff_single.third = temp_buf.third;
		buff_single.fourth = temp_buf.fourth;
		if (buff_vec.size() > 0)
			buff_single.fifth = buff_vec;
	}

	void fill_add_buff_field(const char* buff_field)
	{
		if (strlen(buff_field) == 0)
			return;

		add_buff_t add_buff; 
		fill_add_buff_field_single(buff_field, add_buff);

		t_add_buff_vec.push_back(add_buff);
	}

	void fill_dispel_buff_field(const char* buff_field)
	{
		if (strlen(buff_field) == 0)
			return;

		buff_dispel_info_vec buff_vec;
		dispel_buff_t dispel_buff;

		typedef Dawn::sextuple<uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, VarType> temp_param;
		temp_param temp_buf;
		Dawn::parseTuple(temp_buf, buff_field, "-");
		Dawn::parseContainer<uint32_t>(buff_vec, (const char*)temp_buf.sixth, ",");
		dispel_buff.first = temp_buf.first;
		dispel_buff.second = temp_buf.second;
		dispel_buff.third = temp_buf.third;
		dispel_buff.fourth = temp_buf.fourth;
		dispel_buff.fifth = temp_buf.fifth;
		if (buff_vec.size() > 0)
			dispel_buff.sixth = buff_vec;

		t_dispel_buff_vec.push_back(dispel_buff);
	}
};

typedef DataTableManager< SkillEffectTable, SkillEffectTableBase > SkillEffectTableManager;

inline void SkillEffectTable::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_level = 0;
	this->t_consum_type = 0;
	this->t_consum_value = 0;
	this->t_cd = 0;
	this->t_start_cd = 0;
	this->t_camera_rate = 0;
	this->t_hurt_rate.clear();
	this->t_refind_target = 0;
	this->t_hurt = 0;
	//this->t_levelup_consume.clear();
	this->t_levelup_money = 0;
	this->t_levelup_herosoul = 0;
	this->t_release_ai = 0;
	this->t_target_dead_ai = 0;
	this->t_refind_target_type = 0;
	this->t_target_type = 0;
	this->t_attack_target = 0;
	this->t_play_time = 0;
	this->t_camera_skill_play_time = 0;
	this->t_target_count = 0;
	this->t_hp_hurt_rate.clear();
	this->t_defend_hurt_rate.clear();
	this->t_speed_hurt_rate.clear();
	this->t_ai_id = 0;
	this->t_change_mod_pre_skill = 0;
		
	t_add_buff_vec.clear();
	t_dispel_buff_vec.clear();
};

inline void SkillEffectTable::fill( const SkillEffectTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_level = base.level;
	this->t_consum_type = base.consum_type;
	this->t_consum_value = base.consum_value;
	this->t_cd = base.cd;
	this->t_start_cd = base.start_cd;
	this->t_camera_rate = base.camera_rate;
	//if( base.hurt_rate == NULL ) this->t_hurt_rate.clear(); else this->t_hurt_rate.assign( base.hurt_rate );
	Dawn::parseContainer<uint32_t>(this->t_hurt_rate, base.hurt_rate, "$");

	this->t_refind_target = base.refind_target;
	this->t_hurt = base.hurt;
	//if( base.levelup_consume == NULL ) this->t_levelup_consume.clear(); else this->t_levelup_consume.assign( base.levelup_consume );
	Dawn::parseTuple(this->t_levelup_consume, base.levelup_consume, ":");
	this->t_levelup_money = base.levelup_money;
	this->t_levelup_herosoul = base.levelup_herosoul;
	this->t_release_ai = base.release_ai;
	this->t_target_dead_ai = base.target_dead_ai;
	this->t_refind_target_type = base.refind_target_type;
	this->t_target_type = base.target_type;
	this->t_attack_target = base.attack_target;
	this->t_play_time = base.play_time;
	this->t_camera_skill_play_time = base.camera_skill_play_time;
	this->t_target_count = base.target_count;
	this->t_ai_id = base.ai_id;
	this->t_change_mod_pre_skill = base.change_mod_pre_skill;
		
	Dawn::parseContainer<uint32_t>(this->t_hp_hurt_rate, base.hp_hurt_rate, "$");
	Dawn::parseContainer<uint32_t>(this->t_defend_hurt_rate, base.defend_hurt_rate, "$");
	Dawn::parseContainer<uint32_t>(this->t_speed_hurt_rate, base.speed_hurt_rate, "$");


	fill_add_buff_field(base.buff1);
	fill_add_buff_field(base.buff2);
	fill_add_buff_field(base.buff3);

	fill_add_buff_field_single(base.gx_buff, t_add_gx_buff);
	fill_add_buff_field_single(base.bd_buff, t_add_bd_buff);

	fill_dispel_buff_field(base.dispel_buff1);
	fill_dispel_buff_field(base.dispel_buff2);

};


inline const std::string SkillEffectTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SkillEffectTable.tbls64";
#else
	return "SkillEffectTable.tbls32";
#endif
};

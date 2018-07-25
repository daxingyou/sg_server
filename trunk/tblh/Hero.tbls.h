#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:HeroBase
/// @remark   记录大小:100 X32
///           记录大小:120 X64
///           字段数量:25
///           数据文件:w武将基础表.xlsx 武将基础表
///           排序主键:武将id
#pragma pack(1)
struct HeroBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0FB51649; ///< I|IIIIIVVVVViIIVIIIIIIIII
#else
	static const int          FormatCheckCode = 0x01CD7059; ///< I|IIIIIvvvvviIIvIIIIIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 武将id 生成唯一键

	uint32_t                  id;                           ///< 武将id

	uint32_t                  star;                         ///< 初始星级
	uint32_t                  terry;                        ///< 武将系别
	uint32_t                  rarity;                       ///< 稀有度
	uint32_t                  sex;                          ///< 性别
	uint32_t                  atk_mode;                     ///< 攻击方式（近战、远战）
	const char*               default_skills;               ///< 初始技能列
	const char*               wakeup_skills;                ///< 觉醒技能列
	const char*               more_skills;                  ///< 额外技能列
	const char*               change_model_skills;          ///< 变身技能列
	const char*               wakeup_consume_material;      ///< 觉醒消耗材料/数量
	int32_t                   wakeup_consume_money;         ///< 觉醒消耗铜币
	uint32_t                  base_action_bar;              ///< 出生行动条
	uint32_t                  base_atk_range;               ///< 出生攻击浮动
	const char*               wakeup_attr;                  ///< 其他属性串
	uint32_t                  talent;                       ///< 武将天赋
	uint32_t                  profession;                   ///< 武将职业
	uint32_t                  level_curve;                  ///< 武将升级经验曲线
	uint32_t                  compose_count;                ///< 合成碎片数量
	uint32_t                  smelt_count;                  ///< 分解碎片数量
	uint32_t                  material_id;                  ///< 武将信物ID
	uint32_t                  default_model;                ///< 武将初始模型ID
	uint32_t                  script_id;                    ///< 脚本ID
	uint32_t                  hero_type;                    ///< 武将类型
};
#pragma pack()


typedef std::vector<skill_cd_info_t> skills_vec;
typedef std::vector<skills_vec> change_model_skills_vec;

struct Hero : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  star()                        { return t_star; }
	uint32_t                  terry() { return t_terry; }
	uint32_t                  rarity() { return t_rarity; }
	uint32_t                  sex() { return t_sex; }
	uint32_t                  atk_mode()                    { return t_atk_mode; }
	const std::vector<uint32_t>&				default_skills()              { return t_default_skills; }
	const std::vector<uint32_t>&				wakeup_skills() { return t_wakeup_skills; }
	const std::vector<uint32_t>&				more_skills()                  { return t_more_skills; }
	const std::map<uint32_t, uint32_t>&			wakeup_consume_material()     { return t_wakeup_consume_material; }
	int32_t                   wakeup_consume_money()        { return t_wakeup_consume_money; }
	uint32_t                  base_action_bar()             { return t_base_action_bar; }
	uint32_t                  base_atk_range()              { return t_base_atk_range; }
	const std::map<uint32_t, uint32_t>&			wakeup_attr()                 { return t_wakeup_attr; }
	uint32_t                  talent()						{ return t_talent; }
	uint32_t                  profession()					{ return t_profession; }
	uint32_t                  level_curve()                 { return t_level_curve; }
	uint32_t                  compose_count()               { return t_compose_count; }
	uint32_t                  smelt_count()                 { return t_smelt_count; }
	uint32_t                  material_id()                 { return t_material_id; }
	uint32_t                  default_model()               { return t_default_model; }
	uint32_t                  script_id()                   { return t_script_id; }
	uint32_t                  hero_type()                   { return t_hero_type; }

private:
	uint32_t                  t_id;                         ///< 武将id

	uint32_t                  t_star;                       ///< 初始星级
	uint32_t                  t_terry;                      ///< 武将系别
	uint32_t                  t_rarity;                     ///< 稀有度
	uint32_t                  t_sex;                        ///< 性别
	uint32_t                  t_atk_mode;                   ///< 攻击方式（近战、远战）
	int32_t                   t_wakeup_consume_money;       ///< 觉醒消耗铜币
	uint32_t                  t_base_action_bar;            ///< 出生行动条
	uint32_t                  t_base_atk_range;             ///< 出生攻击浮动
	uint32_t                  t_talent;                     ///< 武将天赋
	uint32_t                  t_profession;                 ///< 武将职业
	uint32_t                  t_level_curve;                ///< 武将升级经验曲线
	uint32_t                  t_compose_count;              ///< 合成碎片数量
	uint32_t                  t_smelt_count;                ///< 分解碎片数量
	uint32_t                  t_material_id;                ///< 武将信物ID
	uint32_t                  t_default_model;              ///< 武将初始模型ID
	uint32_t                  t_script_id;                  ///< 脚本ID
	uint32_t                  t_hero_type;                  ///< 武将类型

	std::vector<uint32_t>			t_default_skills;				///< 初始技能列
	std::vector<uint32_t>			t_wakeup_skills;					///< 全部技能列
	std::vector<uint32_t>			t_more_skills;					///< 全部技能列
	std::map<uint32_t, uint32_t>    t_wakeup_consume_material;		///< 觉醒消耗材料/数量
	std::map<uint32_t, uint32_t>    t_wakeup_attr;					///< 其他属性串
	change_model_skills_vec			t_change_model_skills;			///< 变身技能

public:
	void reset();
	void fill( const HeroBase& base );
	const char* getClassName() const { return "w武将基础表"; }

	const change_model_skills_vec &get_change_model_skills() { return t_change_model_skills; }

	void fill_turn_skill(const char *p_skill_str) {
		if (NULL == p_skill_str) {
			return;
		}

		skills_vec temp;

		std::vector<std::string> conditions;
		std::vector<std::string> cond_vec_1;
		std::vector<uint32_t> cond_vec_2;

		string_util_t::split<std::string>(std::string(p_skill_str), conditions, "$");
		for (size_t i = 0; i < conditions.size(); ++i) {
			skills_vec temp;
			string_util_t::split<std::string>(conditions[i], cond_vec_1, ";");
			for (size_t j = 0; j < cond_vec_1.size(); ++j) {
				string_util_t::split<uint32_t>(cond_vec_1[j], cond_vec_2, ":");
				if (cond_vec_2.size() == 2) {
					skill_cd_info_t skill_cd_info;
					skill_cd_info.skill_id = cond_vec_2[0];
					skill_cd_info.is_use_public_cd = cond_vec_2[1];
					temp.push_back(skill_cd_info);
				}
				cond_vec_2.clear();
			}
			if( temp.size() > 0)
				this->t_change_model_skills.push_back(temp);
			cond_vec_1.clear();
		}
	}

	static const std::string getBinaryName();

	const skills_vec& get_change_model_phase_skills(uint32_t phase)
	{
		if (phase >= this->t_change_model_skills.size())
			return this->t_change_model_skills[0];
		return this->t_change_model_skills[phase];
	}

};


typedef DataTableManager< Hero, HeroBase > HeroManager;

inline void Hero::reset()
{
	this->t_id = 0;
	this->t_star = 0;
	this->t_terry = 0;
	this->t_rarity = 0;
	this->t_sex = 0;
	this->t_atk_mode = 0;
	this->t_default_skills.clear();
	this->t_wakeup_skills.clear();
	this->t_more_skills.clear();
	this->t_change_model_skills.clear();
	this->t_wakeup_consume_material.clear();
	this->t_wakeup_consume_money = 0;
	this->t_base_action_bar = 0;
	this->t_base_atk_range = 0;
	this->t_wakeup_attr.clear();
	this->t_talent = 0;
	this->t_profession = 0;
	this->t_level_curve = 0;
	this->t_compose_count = 0;
	this->t_smelt_count = 0;
	this->t_material_id = 0;
	this->t_default_model = 0;
	this->t_script_id = 0;
	this->t_hero_type = 0;
};

inline void Hero::fill( const HeroBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_star = base.star;
	this->t_terry = base.terry;
	this->t_rarity = base.rarity;
	this->t_sex = base.sex;
	this->t_atk_mode = base.atk_mode;
	Dawn::parseContainer<uint32_t>(this->t_default_skills, base.default_skills, ":");
	Dawn::parseContainer<uint32_t>(this->t_wakeup_skills, base.wakeup_skills, ":");
	Dawn::parseContainer<uint32_t>(this->t_more_skills, base.more_skills, ":");
	//if( base.wakeup_consume_material == NULL ) this->t_wakeup_consume_material.clear(); else this->t_wakeup_consume_material.assign( base.wakeup_consume_material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_wakeup_consume_material, base.wakeup_consume_material, ":$");
	this->t_wakeup_consume_money = base.wakeup_consume_money;
	this->t_base_action_bar = base.base_action_bar;
	this->t_base_atk_range = base.base_atk_range;
	//if( base.wakeup_attr == NULL ) this->t_wakeup_attr.clear(); else this->t_wakeup_attr.assign( base.wakeup_attr );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_wakeup_attr, base.wakeup_attr, ":$");
	this->t_talent = base.talent;
	this->t_profession = base.profession;
	this->t_level_curve = base.level_curve;
	this->t_compose_count = base.compose_count;
	this->t_smelt_count = base.smelt_count;
	this->t_material_id = base.material_id;
	this->t_default_model = base.default_model;
	this->t_script_id = base.script_id;
	this->t_hero_type = base.hero_type;

	fill_turn_skill(base.change_model_skills);
	/*
	skills_vec temp;
	this->t_change_model_skills.push_back(temp);
	Dawn::parseDoubleContainer<uint32_t, skills_vec, change_model_skills_vec>(this->t_change_model_skills, base.change_model_skills, ":$");
	*/
};

inline const std::string Hero::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Hero.tbls64";
#else
	return "Hero.tbls32";
#endif
};

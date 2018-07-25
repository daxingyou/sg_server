#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:MonsterHeroBase
/// @remark   ��¼��С:116 X32
///           ��¼��С:144 X64
///           �ֶ�����:29
///           �����ļ�:g�����.xlsx �����佫��
///           ��������:�佫ID
#pragma pack(1)
struct MonsterHeroBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0507DFD9; ///< I|IVVIVVIIIIIIIIVIIIIIIIVIIVII
#else
	static const int          FormatCheckCode = 0x0567C7F9; ///< I|IvvIvvIIIIIIIIvIIIIIIIvIIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �佫ID ����Ψһ��

	uint32_t                  id;                           ///< �佫ID

	uint32_t                  task_sign;                    ///< ������ID
	const char*               skills;                       ///< ������
	const char*               change_model_skills;          ///< ��������
	uint32_t                  is_dynamic;                   ///< �Ƿ�Ϊ��̬��
	const char*               attrs;                        ///< ����
	const char*               dyn_param;                    ///< ��̬��ϵ��
	uint32_t                  atk_type;                     ///< ���������ͣ��﹥��������
	uint32_t                  atk_mode;                     ///< ������ʽ����ս��Զս��
	uint32_t                  counter_skill_id;             ///< ��������id
	uint32_t                  sex;                          ///< �Ա�
	uint32_t                  script_id;                    ///< �ű�ID
	uint32_t                  profession;                   ///< ְҵ
	uint32_t                  camp;                         ///< ������Ӫ
	uint32_t                  show_cond;                    ///< ��ʾ����
	const char*               show_cond_param;              ///< ��������
	uint32_t                  terry;                        ///< ����ϵ��
	uint32_t                  level;                        ///< ����ȼ�
	uint32_t                  grade;                        ///< Ʒ�ױ�ID
	uint32_t                  debut;                        ///< �ǳ�չʾ
	uint32_t                  resource_id;                  ///< ������ԴID
	uint32_t                  monster_type;                 ///< ��������
	uint32_t                  scale;                        ///< ģ�ͷŴ����
	const char*               hero_name_id;                 ///< �佫����ID
	uint32_t                  boss_hp_plies;                ///< BOSSѪ������
	uint32_t                  event_type;                   ///< ���鴥������
	const char*               event_param;                  ///< ������������
	uint32_t                  plot_id;                      ///< ����Ի�ID
	uint32_t                  would_show;                   ///< �Ƿ���ʾ
};
#pragma pack()

typedef std::vector<uint32_t> skills_vec;
typedef std::vector<skills_vec> change_model_skills_vec;

struct MonsterHero : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  task_sign() { return t_task_sign; }
	const std::vector<uint32_t>& get_skill_list() { return t_skills; }
	bool					  is_dynamic()                  { return t_is_dynamic > 0; }
	const std::map<uint32_t, int>&        attrs()                       { return t_attrs; }
	const std::map<uint32_t, int>&        dyn_param()                   { return t_dyn_param; }
	uint32_t                  atk_type() { return t_atk_type; }
	uint32_t                  atk_mode() { return t_atk_mode; }
	uint32_t                  counter_skill_id() { return t_counter_skill_id; }
	uint32_t                  sex()							{ return t_sex; }
	uint32_t                  script_id()                   { return t_script_id; }
	uint32_t                  profession() { return t_profession; }
	uint32_t                  camp() { return t_camp; }
	uint32_t                  show_cond() { return t_show_cond; }
	//const std::vector<uint32_t>&        show_cond_param() { return t_show_cond_param; }
	uint32_t				  show_cond_param(uint32_t pos) { return pos >= t_show_cond_param.size() ? 0 : t_show_cond_param[pos]; }
	uint32_t                  terry()                       { return t_terry; }
	uint32_t                  level()                       { return t_level; }
	uint32_t                  grade()                       { return t_grade; }
	uint32_t                  debut()                       { return t_debut; }
	uint32_t                  resource_id()                 { return t_resource_id; }
	uint32_t                  monster_type()                { return t_monster_type; }
	uint32_t                  scale()                       { return t_scale; }
	const std::string&        hero_name_id()                { return t_hero_name_id; }
	uint32_t                  boss_hp_plies()               { return t_boss_hp_plies; }
	uint32_t                  event_type()                  { return t_event_type; }
	const std::string&        event_param()                 { return t_event_param; }
	uint32_t                  plot_id()                     { return t_plot_id; }
	uint32_t                  would_show()                  { return t_would_show; }

private:
	uint32_t                  t_id;                         ///< �佫ID

	uint32_t                  t_task_sign;                  ///< ������ID
	uint32_t                  t_is_dynamic;                 ///< �Ƿ�Ϊ��̬��
	std::map<uint32_t, int>               t_attrs;                      ///< ����
	std::map<uint32_t, int>               t_dyn_param;                  ///< ��̬��ϵ��
	uint32_t                  t_atk_type;                   ///< ���������ͣ��﹥��������
	uint32_t                  t_atk_mode;                   ///< ������ʽ����ս��Զս��
	uint32_t                  t_counter_skill_id;           ///< ��������id
	uint32_t                  t_sex;                        ///< �Ա�
	uint32_t                  t_script_id;                  ///< �ű�ID
	uint32_t                  t_profession;                 ///< ְҵ
	uint32_t                  t_camp;                       ///< ������Ӫ
	uint32_t                  t_show_cond;                  ///< ��ʾ����
	std::vector<uint32_t>	  t_show_cond_param;			///< ��ʾ����
	uint32_t                  t_terry;                      ///< ����ϵ��
	uint32_t                  t_level;                      ///< ����ȼ�
	uint32_t                  t_grade;                      ///< Ʒ�ױ�ID
	uint32_t                  t_debut;                      ///< �ǳ�չʾ
	uint32_t                  t_resource_id;                ///< ������ԴID
	uint32_t                  t_monster_type;               ///< ��������
	uint32_t                  t_scale;                      ///< ģ�ͷŴ����
	std::string               t_hero_name_id;               ///< �佫����ID
	uint32_t                  t_boss_hp_plies;              ///< BOSSѪ������
	uint32_t                  t_event_type;                 ///< ���鴥������
	std::string               t_event_param;                ///< ������������
	uint32_t                  t_plot_id;                    ///< ����Ի�ID
	uint32_t                  t_would_show;                 ///< �Ƿ���ʾ
	std::vector<uint32_t>	  t_skills;						///< ������
	change_model_skills_vec	  t_change_model_skills;		///< ��������

public:
	void reset();
	void fill( const MonsterHeroBase& base );
	const char* getClassName() const { return "g�����"; }
	static const std::string getBinaryName();

	const skills_vec& get_change_model_phase_skills(uint32_t phase)
	{
		if (phase >= this->t_change_model_skills.size())
			return this->t_change_model_skills[0];
		return this->t_change_model_skills[phase];
	}

	int get_attr(uint32_t type)
	{
		std::map<uint32_t, int>::const_iterator it = t_attrs.find(type);
		if (it != t_attrs.end())
		{
			return it->second;
		}

		return 0;
	}
};

typedef DataTableManager< MonsterHero, MonsterHeroBase > MonsterHeroManager;

inline void MonsterHero::reset()
{
	this->t_id = 0;
	this->t_task_sign = 0;
	this->t_skills.clear();
	this->t_change_model_skills.clear();
	this->t_is_dynamic = 0;
	this->t_attrs.clear();
	this->t_dyn_param.clear();
	this->t_atk_type = 0;
	this->t_atk_mode = 0;
	this->t_counter_skill_id = 0;
	this->t_sex = 0;
	this->t_script_id = 0;
	this->t_profession = 0;
	this->t_camp = 0;
	this->t_show_cond = 0;
	this->t_show_cond_param.clear();
	this->t_terry = 0;
	this->t_level = 0;
	this->t_grade = 0;
	this->t_debut = 0;
	this->t_resource_id = 0;
	this->t_monster_type = 0;
	this->t_scale = 0;
	this->t_hero_name_id.clear();
	this->t_boss_hp_plies = 0;
	this->t_event_type = 0;
	this->t_event_param.clear();
	this->t_plot_id = 0;
	this->t_would_show = 0;
};

inline void MonsterHero::fill( const MonsterHeroBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_task_sign = base.task_sign;
	Dawn::parseContainer<uint32_t>(this->t_skills, base.skills, ":");
	this->t_is_dynamic = base.is_dynamic;
	//if( base.attrs == NULL ) this->t_attrs.clear(); else this->t_attrs.assign( base.attrs );
	Dawn::parseMapContainer<uint32_t, int>(this->t_attrs, base.attrs, ":$");
	//if( base.dyn_param == NULL ) this->t_dyn_param.clear(); else this->t_dyn_param.assign( base.dyn_param );
	Dawn::parseMapContainer<uint32_t, int>(this->t_dyn_param, base.dyn_param, ":$");
	this->t_atk_type = base.atk_type;
	this->t_atk_mode = base.atk_mode;
	this->t_counter_skill_id = base.counter_skill_id;
	this->t_sex = base.sex;
	this->t_script_id = base.script_id;
	this->t_profession = base.profession;
	this->t_camp = base.camp;
	this->t_show_cond = base.show_cond;
	//if (base.show_cond_param == NULL) this->t_show_cond_param.clear(); else this->t_show_cond_param.assign(base.show_cond_param);
	Dawn::parseContainer<uint32_t>(this->t_show_cond_param, base.show_cond_param, "$");
	this->t_terry = base.terry;
	this->t_level = base.level;
	this->t_grade = base.grade;
	this->t_debut = base.debut;
	this->t_resource_id = base.resource_id;
	this->t_monster_type = base.monster_type;
	this->t_scale = base.scale;
	if( base.hero_name_id == NULL ) this->t_hero_name_id.clear(); else this->t_hero_name_id.assign( base.hero_name_id );
	this->t_boss_hp_plies = base.boss_hp_plies;
	this->t_event_type = base.event_type;
	if( base.event_param == NULL ) this->t_event_param.clear(); else this->t_event_param.assign( base.event_param );
	this->t_plot_id = base.plot_id;
	this->t_would_show = base.would_show;

	skills_vec temp;
	this->t_change_model_skills.push_back(temp);
	Dawn::parseDoubleContainer<uint32_t, skills_vec, change_model_skills_vec>(this->t_change_model_skills, base.change_model_skills, ":$");
};

inline const std::string MonsterHero::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MonsterHero.tbls64";
#else
	return "MonsterHero.tbls32";
#endif
};

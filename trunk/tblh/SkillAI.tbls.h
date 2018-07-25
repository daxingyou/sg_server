#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:SkillAIBase
/// @remark   ��¼��С:56 X32
///           ��¼��С:56 X64
///           �ֶ�����:14
///           �����ļ�:j���ܲ���.xlsx ��������ѡ��
///           ��������:�������ͱ��
#pragma pack(1)

enum skill_ai_type
{
	skill_ai_start_rate = 0,               ///< ��ʼ
	skill_ai_friend_hp_75_rate,            ///< �������Ѿ�Ѫ������75
	skill_ai_friend_hp_50_rate,            ///< �������Ѿ�Ѫ������50
	skill_ai_friend_hp_25_rate,            ///< �������Ѿ�Ѫ������25
	skill_ai_friend_hp_75_add_rate,        ///< �������Ѿ�Ѫ������75(����)
	skill_ai_friend_hp_50_add_rate,        ///< �������Ѿ�Ѫ������50(����)
	skill_ai_friend_hp_25_add_rate,        ///< �������Ѿ�Ѫ������25(����)
	skill_ai_friend_dead_rate,             ///< �Ѿ�����ʱ
	skill_ai_friend_debuff_rate,           ///< �Ѿ��м���ʱ
	skill_ai_friend_debuff_add_rate,       ///< �Ѿ��м���ʱ(����)
	skill_ai_enemy_gain_rate,              ///< �о�������ʱ
	skill_ai_enemy_gain_add_rate,          ///< �о�������ʱ(����)
	skill_ai_enemy_all_immuno_rate,        ///< �о�ȫ���������

	skill_ai_type_count
};

struct SkillAIBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005129; ///< I|iiiiiiiiiiiii
#else
	static const int          FormatCheckCode = 0x00005129; ///< I|iiiiiiiiiiiii
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �������ͱ�� ����Ψһ��

	uint32_t                  id;                           ///< �������ͱ��

	int32_t                   start_rate;                   ///< ��ʼ
	int32_t                   friend_hp_75_rate;            ///< �������Ѿ�Ѫ������75
	int32_t                   friend_hp_50_rate;            ///< �������Ѿ�Ѫ������50
	int32_t                   friend_hp_25_rate;            ///< �������Ѿ�Ѫ������25
	int32_t                   friend_hp_75_add_rate;        ///< �������Ѿ�Ѫ������75(����)
	int32_t                   friend_hp_50_add_rate;        ///< �������Ѿ�Ѫ������50(����)
	int32_t                   friend_hp_25_add_rate;        ///< �������Ѿ�Ѫ������25(����)
	int32_t                   friend_dead_rate;             ///< �Ѿ�����ʱ
	int32_t                   friend_dec_rate;              ///< �Ѿ��м���ʱ
	int32_t                   friend_dec_add_rate;          ///< �Ѿ��м���ʱ(����)
	int32_t                   enemy_inc_rate;               ///< �о�������ʱ
	int32_t                   enemy_inc_add_rate;           ///< �о�������ʱ(����)
	int32_t                   enemy_all_immuno_rate;        ///< �о�ȫ���������
};
#pragma pack()

struct SkillAI : public Entry
{
	uint32_t                  id()                          { return t_id; }

	
	int32_t	get_ai_rate(uint32_t i) { return i >= skill_ai_type_count ? -1 : t_all_skill_ai_type[i]; }

private:
	uint32_t                  t_id;                         ///< �������ͱ��


	std::vector<int32_t>	  t_all_skill_ai_type;			///ȫ��AI����
public:
	void reset();
	void fill( const SkillAIBase& base );
	const char* getClassName() const { return "j���ܲ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SkillAI, SkillAIBase > SkillAIManager;

inline void SkillAI::reset()
{
	this->t_id = 0;
};

inline void SkillAI::fill( const SkillAIBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	t_all_skill_ai_type.push_back(base.start_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_75_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_50_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_25_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_75_add_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_50_add_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_25_add_rate);
	t_all_skill_ai_type.push_back(base.friend_dead_rate);
	t_all_skill_ai_type.push_back(base.friend_dec_rate);
	t_all_skill_ai_type.push_back(base.friend_dec_add_rate);
	t_all_skill_ai_type.push_back(base.enemy_inc_rate);
	t_all_skill_ai_type.push_back(base.enemy_inc_add_rate);
	t_all_skill_ai_type.push_back(base.enemy_all_immuno_rate);
};

inline const std::string SkillAI::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SkillAI.tbls64";
#else
	return "SkillAI.tbls32";
#endif
};

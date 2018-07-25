#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:FightTypeTableBase
/// @remark   ��¼��С:40 X32
///           ��¼��С:40 X64
///           �ֶ�����:10
///           �����ļ�:d��ս��������.xlsx ��ս��������
///           ��������:��ս���ͱ��
#pragma pack(1)
struct FightTypeTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0D8CD009; ///< I|IIIIIIIII
#else
	static const int          FormatCheckCode = 0x0D8CD009; ///< I|IIIIIIIII
#endif

															//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ��ս���ͱ�� ����Ψһ��

	uint32_t                  id;                           ///< ��ս���ͱ��

	uint32_t                  ignore_lineup;                ///< ��������
	uint32_t                  max_visit;                    ///< ��ս��������
	uint32_t                  team_2_exp_addition;          ///< 2����Ӽӳɾ�����ֱ�
	uint32_t                  team_3_exp_addition;          ///< 3����Ӽӳɾ�����ֱ�
	uint32_t                  team_2_coin_addition;         ///< 2����Ӽӳɻ�����ֱ�
	uint32_t                  team_3_coin_addition;         ///< 3����Ӽӳɻ�����ֱ�
	uint32_t                  auto_fight_inherit;           ///< �Զ�ս���̳�
	uint32_t                  fight_with_team;              ///< �Ƿ��������ս��
	uint32_t                  is_replay;                    ///< ����ط�
};
#pragma pack()

struct FightTypeTable : public Entry
{
	uint32_t                  id() { return t_id; }

	uint32_t                  ignore_lineup() { return t_ignore_lineup; }
	uint32_t                  max_visit() { return t_max_visit; }
	uint32_t                  team_2_exp_addition() { return t_team_2_exp_addition; }
	uint32_t                  team_3_exp_addition() { return t_team_3_exp_addition; }
	uint32_t                  team_2_coin_addition() { return t_team_2_coin_addition; }
	uint32_t                  team_3_coin_addition() { return t_team_3_coin_addition; }
	uint32_t                  auto_fight_inherit() { return t_auto_fight_inherit; }
	uint32_t                  fight_with_team()             { return t_fight_with_team; }
	uint32_t                  is_replay()                   { return t_is_replay; }

private:
	uint32_t                  t_id;                         ///< ��ս���ͱ��

	uint32_t                  t_ignore_lineup;              ///< ��������
	uint32_t                  t_max_visit;                  ///< ��ս��������
	uint32_t                  t_team_2_exp_addition;        ///< 2����Ӽӳɾ�����ֱ�
	uint32_t                  t_team_3_exp_addition;        ///< 3����Ӽӳɾ�����ֱ�
	uint32_t                  t_team_2_coin_addition;       ///< 2����Ӽӳɻ�����ֱ�
	uint32_t                  t_team_3_coin_addition;       ///< 3����Ӽӳɻ�����ֱ�
	uint32_t                  t_auto_fight_inherit;         ///< �Զ�ս���̳�
	uint32_t                  t_fight_with_team;            ///< �Ƿ��������ս��
	uint32_t                  t_is_replay;                  ///< ����ط�

public:
	void reset();
	void fill(const FightTypeTableBase& base);
	const char* getClassName() const { return "d��ս��������"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FightTypeTable, FightTypeTableBase > FightTypeTableManager;

inline void FightTypeTable::reset()
{
	this->t_id = 0;
	this->t_ignore_lineup = 0;
	this->t_max_visit = 0;
	this->t_team_2_exp_addition = 0;
	this->t_team_3_exp_addition = 0;
	this->t_team_2_coin_addition = 0;
	this->t_team_3_coin_addition = 0;
	this->t_auto_fight_inherit = 0;
	this->t_fight_with_team = 0;
	this->t_is_replay = 0;
};

inline void FightTypeTable::fill(const FightTypeTableBase& base)
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_ignore_lineup = base.ignore_lineup;
	this->t_max_visit = base.max_visit;
	this->t_team_2_exp_addition = base.team_2_exp_addition;
	this->t_team_3_exp_addition = base.team_3_exp_addition;
	this->t_team_2_coin_addition = base.team_2_coin_addition;
	this->t_team_3_coin_addition = base.team_3_coin_addition;
	this->t_auto_fight_inherit = base.auto_fight_inherit;
	this->t_fight_with_team = base.fight_with_team;
	this->t_is_replay = base.is_replay;
};

inline const std::string FightTypeTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FightTypeTable.tbls64";
#else
	return "FightTypeTable.tbls32";
#endif
};

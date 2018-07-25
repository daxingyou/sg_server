#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:ArenaClassTableBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:28 X64
///           �ֶ�����:7
///           �����ļ�:j������.xlsx arena_class
///           ��������:��λ
#pragma pack(1)
struct ArenaClassTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DDDAC9; ///< i|IIIIII
#else
	static const int          FormatCheckCode = 0x00DDDAC9; ///< i|IIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return arena_level; }; // TODO: ���� ��λ ����Ψһ��

	int32_t                   arena_level;                  ///< ��λ

	uint32_t                  score;                        ///< ������
	uint32_t                  win_honor;                    ///< ս��ʤ���������
	uint32_t                  lose_honor;                   ///< ս��ʧ���������
	uint32_t                  max_honor;                    ///< ��������
	uint32_t                  award;                        ///< ��λ����
	uint32_t                  reset_stage;                  ///< ���ú��λ
};
#pragma pack()

struct ArenaClassTable : public Entry
{
	int32_t                   arena_level()                 { return t_arena_level; }

	uint32_t                  score()                       { return t_score; }
	uint32_t                  win_honor()                   { return t_win_honor; }
	uint32_t                  lose_honor()                  { return t_lose_honor; }
	uint32_t                  max_honor()                   { return t_max_honor; }
	uint32_t                  award()                       { return t_award; }
	uint32_t                  reset_stage()                 { return t_reset_stage; }

private:
	int32_t                   t_arena_level;                ///< ��λ

	uint32_t                  t_score;                      ///< ������
	uint32_t                  t_win_honor;                  ///< ս��ʤ���������
	uint32_t                  t_lose_honor;                 ///< ս��ʧ���������
	uint32_t                  t_max_honor;                  ///< ��������
	uint32_t                  t_award;                      ///< ��λ����
	uint32_t                  t_reset_stage;                ///< ���ú��λ

public:
	void reset();
	void fill( const ArenaClassTableBase& base );
	const char* getClassName() const { return "j������"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ArenaClassTable, ArenaClassTableBase > ArenaClassTableManager;

inline void ArenaClassTable::reset()
{
	this->t_arena_level = 0;
	this->t_score = 0;
	this->t_win_honor = 0;
	this->t_lose_honor = 0;
	this->t_max_honor = 0;
	this->t_award = 0;
	this->t_reset_stage = 0;
};

inline void ArenaClassTable::fill( const ArenaClassTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_arena_level = base.arena_level;
	this->t_score = base.score;
	this->t_win_honor = base.win_honor;
	this->t_lose_honor = base.lose_honor;
	this->t_max_honor = base.max_honor;
	this->t_award = base.award;
	this->t_reset_stage = base.reset_stage;
};

inline const std::string ArenaClassTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ArenaClassTable.tbls64";
#else
	return "ArenaClassTable.tbls32";
#endif
};

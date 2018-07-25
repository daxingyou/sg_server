#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:SkillTableBase
/// @remark   ��¼��С:4 X32
///           ��¼��С:4 X64
///           �ֶ�����:1
///           �����ļ�:j����.xlsx NewSkill
///           ��������:����ID
#pragma pack(1)
struct SkillTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#else
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����ID ����Ψһ��

	uint32_t                  id;                           ///< ����ID

};
#pragma pack()

struct SkillTable : public Entry
{
	uint32_t                  id()                          { return t_id; }


private:
	uint32_t                  t_id;                         ///< ����ID


public:
	void reset();
	void fill( const SkillTableBase& base );
	const char* getClassName() const { return "j����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SkillTable, SkillTableBase > SkillTableManager;

inline void SkillTable::reset()
{
	this->t_id = 0;
};

inline void SkillTable::fill( const SkillTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
};

inline const std::string SkillTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SkillTable.tbls64";
#else
	return "SkillTable.tbls32";
#endif
};

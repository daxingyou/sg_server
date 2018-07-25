#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:LevelAddExpRateTableBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:16 X64
///           �ֶ�����:4
///           �����ļ�:t���´���.xlsx ����ӳ�
///           ��������:id
#pragma pack(1)
struct LevelAddExpRateTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  level_up;                     ///< �������up
	uint32_t                  level_down;                   ///< �������down
	uint32_t                  buff_id;                      ///< buff_id
};
#pragma pack()

struct LevelAddExpRateTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level_up()                    { return t_level_up; }
	uint32_t                  level_down()                  { return t_level_down; }
	uint32_t                  buff_id()                     { return t_buff_id; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_level_up;                   ///< �������up
	uint32_t                  t_level_down;                 ///< �������down
	uint32_t                  t_buff_id;                    ///< buff_id

public:
	void reset();
	void fill( const LevelAddExpRateTableBase& base );
	const char* getClassName() const { return "t���´���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelAddExpRateTable, LevelAddExpRateTableBase > LevelAddExpRateTableManager;

inline void LevelAddExpRateTable::reset()
{
	this->t_id = 0;
	this->t_level_up = 0;
	this->t_level_down = 0;
	this->t_buff_id = 0;
};

inline void LevelAddExpRateTable::fill( const LevelAddExpRateTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level_up = base.level_up;
	this->t_level_down = base.level_down;
	this->t_buff_id = base.buff_id;
};

inline const std::string LevelAddExpRateTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelAddExpRateTable.tbls64";
#else
	return "LevelAddExpRateTable.tbls32";
#endif
};

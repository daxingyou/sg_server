#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:ArenaWeekAwardTableBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:8 X64
///           �ֶ�����:2
///           �����ļ�:j������.xlsx arena_week_awards
///           ��������:��λID
#pragma pack(1)
struct ArenaWeekAwardTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ��λID ����Ψһ��

	uint32_t                  id;                           ///< ��λID

	uint32_t                  mail_id;                      ///< �����ʼ�ID
};
#pragma pack()

struct ArenaWeekAwardTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  mail_id()                     { return t_mail_id; }

private:
	uint32_t                  t_id;                         ///< ��λID

	uint32_t                  t_mail_id;                    ///< �����ʼ�ID

public:
	void reset();
	void fill( const ArenaWeekAwardTableBase& base );
	const char* getClassName() const { return "j������"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ArenaWeekAwardTable, ArenaWeekAwardTableBase > ArenaWeekAwardTableManager;

inline void ArenaWeekAwardTable::reset()
{
	this->t_id = 0;
	this->t_mail_id = 0;
};

inline void ArenaWeekAwardTable::fill( const ArenaWeekAwardTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_mail_id = base.mail_id;
};

inline const std::string ArenaWeekAwardTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ArenaWeekAwardTable.tbls64";
#else
	return "ArenaWeekAwardTable.tbls32";
#endif
};

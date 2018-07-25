#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:ArenaWeekAwardTableBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:j竞技场.xlsx arena_week_awards
///           排序主键:段位ID
#pragma pack(1)
struct ArenaWeekAwardTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 段位ID 生成唯一键

	uint32_t                  id;                           ///< 段位ID

	uint32_t                  mail_id;                      ///< 奖励邮件ID
};
#pragma pack()

struct ArenaWeekAwardTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  mail_id()                     { return t_mail_id; }

private:
	uint32_t                  t_id;                         ///< 段位ID

	uint32_t                  t_mail_id;                    ///< 奖励邮件ID

public:
	void reset();
	void fill( const ArenaWeekAwardTableBase& base );
	const char* getClassName() const { return "j竞技场"; }

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

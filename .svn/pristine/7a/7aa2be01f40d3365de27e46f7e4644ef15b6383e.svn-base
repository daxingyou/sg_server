#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LevelRewardMailTableBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:d等级目标奖励.xlsx Sheet2
///           排序主键:ID
#pragma pack(1)
struct LevelRewardMailTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	uint32_t                  level;                        ///< 领取等级
	uint32_t                  mail_id;                      ///< 邮件ID
};
#pragma pack()

struct LevelRewardMailTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	uint32_t                  mail_id()                     { return t_mail_id; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_level;                      ///< 领取等级
	uint32_t                  t_mail_id;                    ///< 邮件ID

public:
	void reset();
	void fill( const LevelRewardMailTableBase& base );
	const char* getClassName() const { return "d等级目标奖励"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelRewardMailTable, LevelRewardMailTableBase > LevelRewardMailTableManager;

inline void LevelRewardMailTable::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_mail_id = 0;
};

inline void LevelRewardMailTable::fill( const LevelRewardMailTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_mail_id = base.mail_id;
};

inline const std::string LevelRewardMailTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelRewardMailTable.tbls64";
#else
	return "LevelRewardMailTable.tbls32";
#endif
};

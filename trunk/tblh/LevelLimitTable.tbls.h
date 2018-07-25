#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LevelLimitTableBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:t天下大势.xlsx 卡级
///           排序主键:等级
#pragma pack(1)
struct LevelLimitTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 等级 生成唯一键

	uint32_t                  id;                           ///< 等级

	uint32_t                  finish_task;                  ///< 完成任务
	uint32_t                  max_exp;                      ///< 经验上限
	uint32_t                  exchange_rate;                ///< 转换比例
};
#pragma pack()

struct LevelLimitTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  finish_task()                 { return t_finish_task; }
	uint32_t                  max_exp()                     { return t_max_exp; }
	uint32_t                  exchange_rate()               { return t_exchange_rate; }

private:
	uint32_t                  t_id;                         ///< 等级

	uint32_t                  t_finish_task;                ///< 完成任务
	uint32_t                  t_max_exp;                    ///< 经验上限
	uint32_t                  t_exchange_rate;              ///< 转换比例

public:
	void reset();
	void fill( const LevelLimitTableBase& base );
	const char* getClassName() const { return "t天下大势"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelLimitTable, LevelLimitTableBase > LevelLimitTableManager;

inline void LevelLimitTable::reset()
{
	this->t_id = 0;
	this->t_finish_task = 0;
	this->t_max_exp = 0;
	this->t_exchange_rate = 0;
};

inline void LevelLimitTable::fill( const LevelLimitTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_finish_task = base.finish_task;
	this->t_max_exp = base.max_exp;
	this->t_exchange_rate = base.exchange_rate;
};

inline const std::string LevelLimitTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelLimitTable.tbls64";
#else
	return "LevelLimitTable.tbls32";
#endif
};

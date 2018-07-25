#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:DropWorldTableBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:d掉落表.xlsx 世界掉落表
///           排序主键:id
#pragma pack(1)
struct DropWorldTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  drop_max_num;                 ///< 掉落最大数量
	uint32_t                  drop_interval;                ///< 掉落最小间隔
	uint32_t                  time_interval;                ///< 周期清零
};
#pragma pack()

struct DropWorldTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_max_num()                { return t_drop_max_num; }
	uint32_t                  drop_interval()               { return t_drop_interval; }
	uint32_t                  time_interval()               { return t_time_interval; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_drop_max_num;               ///< 掉落最大数量
	uint32_t                  t_drop_interval;              ///< 掉落最小间隔
	uint32_t                  t_time_interval;              ///< 周期清零

public:
	void reset();
	void fill( const DropWorldTableBase& base );
	const char* getClassName() const { return "d掉落表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< DropWorldTable, DropWorldTableBase > DropWorldTableManager;

inline void DropWorldTable::reset()
{
	this->t_id = 0;
	this->t_drop_max_num = 0;
	this->t_drop_interval = 0;
	this->t_time_interval = 0;
};

inline void DropWorldTable::fill( const DropWorldTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_max_num = base.drop_max_num;
	this->t_drop_interval = base.drop_interval;
	this->t_time_interval = base.time_interval;
};

inline const std::string DropWorldTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "DropWorldTable.tbls64";
#else
	return "DropWorldTable.tbls32";
#endif
};

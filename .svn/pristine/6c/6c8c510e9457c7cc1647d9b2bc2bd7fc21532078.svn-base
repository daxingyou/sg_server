#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:PatrolLevelBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:x巡逻表.xlsx 逆命师加成
///           排序主键:特权等级
#pragma pack(1)
struct PatrolLevelBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 特权等级 生成唯一键

	uint32_t                  id;                           ///< 特权等级

	uint32_t                  time;                         ///< 间隔时间
};
#pragma pack()

struct PatrolLevel : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  time()                        { return t_time; }

private:
	uint32_t                  t_id;                         ///< 特权等级

	uint32_t                  t_time;                       ///< 间隔时间

public:
	void reset();
	void fill( const PatrolLevelBase& base );
	const char* getClassName() const { return "x巡逻表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< PatrolLevel, PatrolLevelBase > PatrolLevelManager;

inline void PatrolLevel::reset()
{
	this->t_id = 0;
	this->t_time = 0;
};

inline void PatrolLevel::fill( const PatrolLevelBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_time = base.time;
};

inline const std::string PatrolLevel::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "PatrolLevel.tbls64";
#else
	return "PatrolLevel.tbls32";
#endif
};

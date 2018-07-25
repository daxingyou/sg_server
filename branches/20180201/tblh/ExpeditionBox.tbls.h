#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:ExpeditionBoxBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:y远征表.xlsx 活跃宝箱
///           排序主键:需求活跃值
#pragma pack(1)
struct ExpeditionBoxBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return glory; }; // TODO: 请用 需求活跃值 生成唯一键

	uint32_t                  glory;                        ///< 需求活跃值

	uint32_t                  reward;                       ///< 奖励物品
};
#pragma pack()

struct ExpeditionBox : public Entry
{
	uint32_t                  glory()                       { return t_glory; }

	uint32_t                  reward()                      { return t_reward; }

private:
	uint32_t                  t_glory;                      ///< 需求活跃值

	uint32_t                  t_reward;                     ///< 奖励物品

public:
	void reset();
	void fill( const ExpeditionBoxBase& base );
	const char* getClassName() const { return "y远征表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ExpeditionBox, ExpeditionBoxBase > ExpeditionBoxManager;

inline void ExpeditionBox::reset()
{
	this->t_glory = 0;
	this->t_reward = 0;
};

inline void ExpeditionBox::fill( const ExpeditionBoxBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_glory = base.glory;
	this->t_reward = base.reward;
};

inline const std::string ExpeditionBox::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ExpeditionBox.tbls64";
#else
	return "ExpeditionBox.tbls32";
#endif
};

#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TradeProsperityBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:t跑商表.xlsx trade_prosperity
///           排序主键:繁荣度等级
#pragma pack(1)
struct TradeProsperityBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return level; }; // TODO: 请用 繁荣度等级 生成唯一键

	uint32_t                  level;                        ///< 繁荣度等级

	uint32_t                  prosperity;                   ///< 繁荣度数值
};
#pragma pack()

struct TradeProsperity : public Entry
{
	uint32_t                  level()                       { return t_level; }

	uint32_t                  prosperity()                  { return t_prosperity; }

private:
	uint32_t                  t_level;                      ///< 繁荣度等级

	uint32_t                  t_prosperity;                 ///< 繁荣度数值

public:
	void reset();
	void fill( const TradeProsperityBase& base );
	const char* getClassName() const { return "t跑商表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TradeProsperity, TradeProsperityBase > TradeProsperityManager;

inline void TradeProsperity::reset()
{
	this->t_level = 0;
	this->t_prosperity = 0;
};

inline void TradeProsperity::fill( const TradeProsperityBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_level = base.level;
	this->t_prosperity = base.prosperity;
};

inline const std::string TradeProsperity::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TradeProsperity.tbls64";
#else
	return "TradeProsperity.tbls32";
#endif
};

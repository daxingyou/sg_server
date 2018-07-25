#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TradeCultureBase
/// @remark   记录大小:4 X32
///           记录大小:4 X64
///           字段数量:1
///           数据文件:t跑商表.xlsx trade_culture
///           排序主键:文化圈id
#pragma pack(1)
struct TradeCultureBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#else
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 文化圈id 生成唯一键

	uint32_t                  id;                           ///< 文化圈id

};
#pragma pack()

struct TradeCulture : public Entry
{
	uint32_t                  id()                          { return t_id; }


private:
	uint32_t                  t_id;                         ///< 文化圈id


public:
	void reset();
	void fill( const TradeCultureBase& base );
	const char* getClassName() const { return "t跑商表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TradeCulture, TradeCultureBase > TradeCultureManager;

inline void TradeCulture::reset()
{
	this->t_id = 0;
};

inline void TradeCulture::fill( const TradeCultureBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
};

inline const std::string TradeCulture::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TradeCulture.tbls64";
#else
	return "TradeCulture.tbls32";
#endif
};

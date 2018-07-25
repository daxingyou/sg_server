#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:CountryFlagBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:g国家表.xlsx country_flag
///           排序主键:id
#pragma pack(1)
struct CountryFlagBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  unlock_condi;                 ///< 解锁条件
};
#pragma pack()

struct CountryFlag : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  unlock_condi()                { return t_unlock_condi; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_unlock_condi;               ///< 解锁条件

public:
	void reset();
	void fill( const CountryFlagBase& base );
	const char* getClassName() const { return "g国家表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< CountryFlag, CountryFlagBase > CountryFlagManager;

inline void CountryFlag::reset()
{
	this->t_id = 0;
	this->t_unlock_condi = 0;
};

inline void CountryFlag::fill( const CountryFlagBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_unlock_condi = base.unlock_condi;
};

inline const std::string CountryFlag::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "CountryFlag.tbls64";
#else
	return "CountryFlag.tbls32";
#endif
};

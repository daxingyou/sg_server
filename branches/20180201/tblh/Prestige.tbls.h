#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:PrestigeBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:prestige.xlsx prestige
///           排序主键:声望等级
#pragma pack(1)
struct PrestigeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 声望等级 生成唯一键

	uint32_t                  id;                           ///< 声望等级

	uint32_t                  exp;                          ///< 升级经验
	uint32_t                  prestige_limit;               ///< 声望每日上限
};
#pragma pack()

struct Prestige : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  exp()                         { return t_exp; }
	uint32_t                  prestige_limit()              { return t_prestige_limit; }

private:
	uint32_t                  t_id;                         ///< 声望等级

	uint32_t                  t_exp;                        ///< 升级经验
	uint32_t                  t_prestige_limit;             ///< 声望每日上限

public:
	void reset();
	void fill( const PrestigeBase& base );
	const char* getClassName() const { return "prestige"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Prestige, PrestigeBase > PrestigeManager;

inline void Prestige::reset()
{
	this->t_id = 0;
	this->t_exp = 0;
	this->t_prestige_limit = 0;
};

inline void Prestige::fill( const PrestigeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_exp = base.exp;
	this->t_prestige_limit = base.prestige_limit;
};

inline const std::string Prestige::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Prestige.tbls64";
#else
	return "Prestige.tbls32";
#endif
};

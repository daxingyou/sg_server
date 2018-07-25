#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:PatrolExpBase
/// @remark   记录大小:12 X32
///           记录大小:16 X64
///           字段数量:3
///           数据文件:x巡逻表.xlsx 经验获取
///           排序主键:ID
#pragma pack(1)
struct PatrolExpBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000511A9; ///< I|VI
#else
	static const int          FormatCheckCode = 0x000513A9; ///< I|vI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	const char*               count;                        ///< 对阵场数
	uint32_t                  exp;                          ///< 经验获取
};
#pragma pack()

struct PatrolExp : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::pair<uint32_t, uint32_t>&        count()                       { return t_count; }
	uint32_t                  exp()                         { return t_exp; }

private:
	uint32_t                  t_id;                         ///< ID

	std::pair<uint32_t, uint32_t>               t_count;                      ///< 对阵场数
	uint32_t                  t_exp;                        ///< 经验获取

public:
	void reset();
	void fill( const PatrolExpBase& base );
	const char* getClassName() const { return "x巡逻表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< PatrolExp, PatrolExpBase > PatrolExpManager;

inline void PatrolExp::reset()
{
	this->t_id = 0;
	//this->t_count.clear();
	this->t_exp = 0;
};

inline void PatrolExp::fill( const PatrolExpBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.count == NULL ) this->t_count.clear(); else this->t_count.assign( base.count );
	Dawn::parseTuple<uint32_t, uint32_t>(this->t_count, base.count, "~");
	this->t_exp = base.exp;
};

inline const std::string PatrolExp::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "PatrolExp.tbls64";
#else
	return "PatrolExp.tbls32";
#endif
};

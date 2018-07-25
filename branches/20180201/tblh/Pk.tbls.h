#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:PkBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:ppk表.xlsx 奖励表
///           排序主键:id
#pragma pack(1)
struct PkBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510EA9; ///< I|IVI
#else
	static const int          FormatCheckCode = 0x005110A9; ///< I|IvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  type;                         ///< 奖惩类型
	const char*               value_range;                  ///< PK值档次区间
	uint32_t                  param;                        ///< 参数
};
#pragma pack()

struct Pk : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	const std::pair<uint32_t, uint32_t>&        value_range()                 { return t_value_range; }
	uint32_t                  param()                       { return t_param; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_type;                       ///< 奖惩类型
	std::pair<uint32_t, uint32_t>               t_value_range;                ///< PK值档次区间
	uint32_t                  t_param;                      ///< 参数

public:
	void reset();
	void fill( const PkBase& base );
	const char* getClassName() const { return "ppk表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Pk, PkBase > PkManager;

inline void Pk::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	//this->t_value_range.clear();
	this->t_param = 0;
};

inline void Pk::fill( const PkBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	//if( base.value_range == NULL ) this->t_value_range.clear(); else this->t_value_range.assign( base.value_range );
	Dawn::parseTuple(this->t_value_range, base.value_range);
	this->t_param = base.param;
};

inline const std::string Pk::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Pk.tbls64";
#else
	return "Pk.tbls32";
#endif
};

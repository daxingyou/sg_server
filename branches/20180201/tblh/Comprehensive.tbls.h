#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:ComprehensiveBase
/// @remark   记录大小:16 X32
///           记录大小:28 X64
///           字段数量:4
///           数据文件:q全局表.xlsx data
///           排序主键:序号id
#pragma pack(1)
struct ComprehensiveBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00511BB6; ///< I|VVV
#else
	static const int          FormatCheckCode = 0x00513DD6; ///< I|vvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 序号id 生成唯一键

	uint32_t                  id;                           ///< 序号id

	const char*               parameter1;                   ///< 参数1
	const char*               parameter2;                   ///< 参数2
	const char*               parameter3;                   ///< 参数3
};
#pragma pack()

struct Comprehensive : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        parameter1()                  { return t_parameter1; }
	const std::string&        parameter2()                  { return t_parameter2; }
	const std::string&        parameter3()                  { return t_parameter3; }

private:
	uint32_t                  t_id;                         ///< 序号id

	std::string               t_parameter1;                 ///< 参数1
	std::string               t_parameter2;                 ///< 参数2
	std::string               t_parameter3;                 ///< 参数3

public:
	void reset();
	void fill( const ComprehensiveBase& base );
	const char* getClassName() const { return "q全局表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Comprehensive, ComprehensiveBase > ComprehensiveManager;

inline void Comprehensive::reset()
{
	this->t_id = 0;
	this->t_parameter1.clear();
	this->t_parameter2.clear();
	this->t_parameter3.clear();
};

inline void Comprehensive::fill( const ComprehensiveBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.parameter1 == NULL ) this->t_parameter1.clear(); else this->t_parameter1.assign( base.parameter1 );
	if( base.parameter2 == NULL ) this->t_parameter2.clear(); else this->t_parameter2.assign( base.parameter2 );
	if( base.parameter3 == NULL ) this->t_parameter3.clear(); else this->t_parameter3.assign( base.parameter3 );
};

inline const std::string Comprehensive::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Comprehensive.tbls64";
#else
	return "Comprehensive.tbls32";
#endif
};

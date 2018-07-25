#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:AchieverecordBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:c成就表.xlsx 个人历程
///           排序主键:历程编号
#pragma pack(1)
struct AchieverecordBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00511AD9; ///< I|VII
#else
	static const int          FormatCheckCode = 0x00513AD9; ///< I|vII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 历程编号 生成唯一键

	uint32_t                  id;                           ///< 历程编号

	const char*               content;                      ///< 文字内容
	uint32_t                  condition;                    ///< 判断条件
	uint32_t                  param;                        ///< 条件参数
};
#pragma pack()

struct Achieverecord : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        content()                     { return t_content; }
	uint32_t                  condition()                   { return t_condition; }
	uint32_t                  param()                       { return t_param; }

private:
	uint32_t                  t_id;                         ///< 历程编号

	std::string               t_content;                    ///< 文字内容
	uint32_t                  t_condition;                  ///< 判断条件
	uint32_t                  t_param;                      ///< 条件参数

public:
	void reset();
	void fill( const AchieverecordBase& base );
	const char* getClassName() const { return "c成就表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Achieverecord, AchieverecordBase > AchieverecordManager;

inline void Achieverecord::reset()
{
	this->t_id = 0;
	this->t_content.clear();
	this->t_condition = 0;
	this->t_param = 0;
};

inline void Achieverecord::fill( const AchieverecordBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.content == NULL ) this->t_content.clear(); else this->t_content.assign( base.content );
	this->t_condition = base.condition;
	this->t_param = base.param;
};

inline const std::string Achieverecord::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Achieverecord.tbls64";
#else
	return "Achieverecord.tbls32";
#endif
};

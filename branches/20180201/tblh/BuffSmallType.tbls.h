#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:BuffSmallTypeBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:g光环表.xlsx buff小类型关系表
///           排序主键:buff小类型id
#pragma pack(1)
struct BuffSmallTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return buff_small_type; }; // TODO: 请用 buff小类型id 生成唯一键

	uint32_t                  buff_small_type;              ///< buff小类型id

	uint32_t                  same_source;                  ///< 同一来源
	uint32_t                  diff_source;                  ///< 不同来源
};
#pragma pack()

struct BuffSmallType : public Entry
{
	uint32_t                  buff_small_type()             { return t_buff_small_type; }

	uint32_t                  same_source()                 { return t_same_source; }
	uint32_t                  diff_source()                 { return t_diff_source; }

private:
	uint32_t                  t_buff_small_type;            ///< buff小类型id

	uint32_t                  t_same_source;                ///< 同一来源
	uint32_t                  t_diff_source;                ///< 不同来源

public:
	void reset();
	void fill( const BuffSmallTypeBase& base );
	const char* getClassName() const { return "g光环表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< BuffSmallType, BuffSmallTypeBase > BuffSmallTypeManager;

inline void BuffSmallType::reset()
{
	this->t_buff_small_type = 0;
	this->t_same_source = 0;
	this->t_diff_source = 0;
};

inline void BuffSmallType::fill( const BuffSmallTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_buff_small_type = base.buff_small_type;
	this->t_same_source = base.same_source;
	this->t_diff_source = base.diff_source;
};

inline const std::string BuffSmallType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "BuffSmallType.tbls64";
#else
	return "BuffSmallType.tbls32";
#endif
};

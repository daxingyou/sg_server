#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
/// @brief    表格结构:FrameTableBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:t头像表.xlsx Frame
///           排序主键:边框id
#pragma pack(1)
struct FrameTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 边框id 生成唯一键

	uint32_t                  id;                           ///< 边框id

	const char*               cond;                         ///< 激活条件
};
#pragma pack()

struct FrameTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t, uint32_t>&        cond()       { return t_cond; }

private:
	uint32_t                  t_id;                         ///< 边框id

	std::map<uint32_t, uint32_t>   t_cond;                  ///< 激活条件

public:
	void reset();
	void fill( const FrameTableBase& base );
	const char* getClassName() const { return "t头像表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FrameTable, FrameTableBase > FrameTableManager;

inline void FrameTable::reset()
{
	this->t_id = 0;
	this->t_cond.clear();
};

inline void FrameTable::fill( const FrameTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	///if( base.cond == NULL ) this->t_cond.clear(); else this->t_cond.assign( base.cond );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_cond, base.cond, ":$");
};

inline const std::string FrameTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FrameTable.tbls64";
#else
	return "FrameTable.tbls32";
#endif
};

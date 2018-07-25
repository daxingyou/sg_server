#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:HandBookBase
/// @remark   记录大小:16 X32
///           记录大小:24 X64
///           字段数量:4
///           数据文件:s收集奖励表.xlsx Sheet1
///           排序主键:收集id
#pragma pack(1)
struct HandBookBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00511BA9; ///< I|VVI
#else
	static const int          FormatCheckCode = 0x00513DA9; ///< I|vvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 收集id 生成唯一键

	uint32_t                  id;                           ///< 收集id

	const char*               heros;                        ///< 武将id
	const char*               attr_add;                     ///< 属性奖励
	uint32_t                  drop;                         ///< 道具奖励
};
#pragma pack()

struct HandBook : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        heros()                       { return t_heros; }
	const std::map<uint32_t,uint32_t>&        attr_add()                    { return t_attr_add; }
	uint32_t                  drop()                        { return t_drop; }

private:
	uint32_t                  t_id;                         ///< 收集id

	std::vector<uint32_t>               t_heros;                      ///< 武将id
	std::map<uint32_t, uint32_t>               t_attr_add;                   ///< 属性奖励
	uint32_t                  t_drop;                       ///< 道具奖励

public:
	void reset();
	void fill( const HandBookBase& base );
	const char* getClassName() const { return "s收集奖励表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HandBook, HandBookBase > HandBookManager;

inline void HandBook::reset()
{
	this->t_id = 0;
	this->t_heros.clear();
	this->t_attr_add.clear();
	this->t_drop = 0;
};

inline void HandBook::fill( const HandBookBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.heros == NULL ) this->t_heros.clear(); else this->t_heros.assign( base.heros );
	Dawn::parseContainer<uint32_t>(this->t_heros, base.heros, "$");
	//if( base.attr_add == NULL ) this->t_attr_add.clear(); else this->t_attr_add.assign( base.attr_add );
	Dawn::parseMapContainer<uint32_t,uint32_t>(this->t_attr_add, base.attr_add, ":$");
	this->t_drop = base.drop;
};

inline const std::string HandBook::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HandBook.tbls64";
#else
	return "HandBook.tbls32";
#endif
};

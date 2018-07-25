#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LiandanluTableBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:h合成表.xlsx 炼丹炉
///           排序主键:炼丹槽位ID
#pragma pack(1)
struct LiandanluTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 炼丹槽位ID 生成唯一键

	uint32_t                  id;                           ///< 炼丹槽位ID

	const char*               material;                     ///< 解锁道具
};
#pragma pack()

struct LiandanluTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t, uint32_t>&        material()                    { return t_material; }

private:
	uint32_t                  t_id;                         ///< 炼丹槽位ID

	std::map<uint32_t, uint32_t>               t_material;                   ///< 解锁道具

public:
	void reset();
	void fill( const LiandanluTableBase& base );
	const char* getClassName() const { return "h合成表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LiandanluTable, LiandanluTableBase > LiandanluTableManager;

inline void LiandanluTable::reset()
{
	this->t_id = 0;
	this->t_material.clear();
};

inline void LiandanluTable::fill( const LiandanluTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.material == NULL ) this->t_material.clear(); else this->t_material.assign( base.material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_material, base.material, ":$");
};

inline const std::string LiandanluTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LiandanluTable.tbls64";
#else
	return "LiandanluTable.tbls32";
#endif
};

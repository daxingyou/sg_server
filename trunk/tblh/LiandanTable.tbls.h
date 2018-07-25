#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LiandanTableBase
/// @remark   记录大小:28 X32
///           记录大小:36 X64
///           字段数量:7
///           数据文件:h合成表.xlsx 丹药
///           排序主键:合成Id
#pragma pack(1)
struct LiandanTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0EBABBD9; ///< I|IVVIVII
#else
	static const int          FormatCheckCode = 0x00DB5BC9; ///< I|IvvIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 合成Id 生成唯一键

	uint32_t                  id;                           ///< 合成Id

	uint32_t                  drop_id;                      ///< 合成掉落ID
	const char*               material;                     ///< 合成材料
	const char*               money;                        ///< 消耗货币
	uint32_t                  time;                         ///< 合成时间
	const char*               done_cost;                    ///< 加速消耗
	uint32_t                  need_level;                   ///< 解锁战队等级
	uint32_t                  max_count;                    ///< 每次最大合成
};
#pragma pack()

struct LiandanTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_id()                     { return t_drop_id; }
	const std::map<uint32_t, uint32_t>&        material()                    { return t_material; }
	const std::map<uint32_t, uint32_t>&                  money() { return t_money; }
	uint32_t                  time()                        { return t_time; }
	const Dawn::triple<uint32_t, uint32_t, uint32_t>&        done_cost()                   { return t_done_cost; }
	uint32_t                  need_level()                  { return t_need_level; }
	uint32_t                  max_count()                   { return t_max_count; }

private:
	uint32_t                  t_id;                         ///< 合成Id

	uint32_t                  t_drop_id;                    ///< 合成掉落ID
	std::map<uint32_t, uint32_t>               t_material;                   ///< 合成材料
	std::map<uint32_t, uint32_t>               t_money;                     ///< 消耗货币
	uint32_t                  t_time;                       ///< 合成时间
	Dawn::triple<uint32_t, uint32_t, uint32_t>               t_done_cost;                  ///< 加速消耗
	uint32_t                  t_need_level;                 ///< 解锁战队等级
	uint32_t                  t_max_count;                  ///< 每次最大合成

public:
	void reset();
	void fill( const LiandanTableBase& base );
	const char* getClassName() const { return "h合成表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LiandanTable, LiandanTableBase > LiandanTableManager;

inline void LiandanTable::reset()
{
	this->t_id = 0;
	this->t_drop_id = 0;
	this->t_material.clear();
	this->t_money.clear();
	this->t_time = 0;
	this->t_done_cost.clear();
	this->t_need_level = 0;
	this->t_max_count = 0;
};

inline void LiandanTable::fill( const LiandanTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_id = base.drop_id;
	//if( base.material == NULL ) this->t_material.clear(); else this->t_material.assign( base.material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_material, base.material, ":$");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_money, base.money, ":$");
	this->t_time = base.time;
	//if( base.done_cost == NULL ) this->t_done_cost.clear(); else this->t_done_cost.assign( base.done_cost );
	Dawn::parseTuple(this->t_done_cost, base.done_cost, ":");
	this->t_need_level = base.need_level;
	this->t_max_count = base.max_count;
};

inline const std::string LiandanTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LiandanTable.tbls64";
#else
	return "LiandanTable.tbls32";
#endif
};

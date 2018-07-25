#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:TreasureBase
/// @remark   记录大小:12 X32
///           记录大小:20 X64
///           字段数量:3
///           数据文件:c藏宝图表.xlsx treasure
///           排序主键:藏宝图类型id
#pragma pack(1)
struct TreasureBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000511B6; ///< I|VV
#else
	static const int          FormatCheckCode = 0x000513D6; ///< I|vv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 藏宝图类型id 生成唯一键

	uint32_t                  id;                           ///< 藏宝图类型id

	const char*               treasure_make_id;             ///< 制造id池
	const char*               treasure_make_prob;           ///< 对应的概率
};
#pragma pack()

struct Treasure : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        treasure_make_id()            { return t_treasure_make_id; }
	const std::vector<uint32_t>&        treasure_make_prob()          { return t_treasure_make_prob; }

private:
	uint32_t                  t_id;                         ///< 藏宝图类型id

	std::vector<uint32_t>               t_treasure_make_id;           ///< 制造id池
	std::vector<uint32_t>               t_treasure_make_prob;         ///< 对应的概率

public:
	void reset();
	void fill( const TreasureBase& base );
	const char* getClassName() const { return "c藏宝图表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Treasure, TreasureBase > TreasureManager;

inline void Treasure::reset()
{
	this->t_id = 0;
	this->t_treasure_make_id.clear();
	this->t_treasure_make_prob.clear();
};

inline void Treasure::fill( const TreasureBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.treasure_make_id == NULL ) this->t_treasure_make_id.clear(); else this->t_treasure_make_id.assign( base.treasure_make_id );
	Dawn::parseContainer<uint32_t>(this->t_treasure_make_id, base.treasure_make_id, "$");
	//if( base.treasure_make_prob == NULL ) this->t_treasure_make_prob.clear(); else this->t_treasure_make_prob.assign( base.treasure_make_prob );
	Dawn::parseContainer<uint32_t>(this->t_treasure_make_prob, base.treasure_make_prob, "$");
};

inline const std::string Treasure::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Treasure.tbls64";
#else
	return "Treasure.tbls32";
#endif
};

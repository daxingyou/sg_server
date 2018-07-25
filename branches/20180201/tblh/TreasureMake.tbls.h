#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:TreasureMakeBase
/// @remark   记录大小:20 X32
///           记录大小:32 X64
///           字段数量:5
///           数据文件:c藏宝图表.xlsx treasure_make
///           排序主键:id
#pragma pack(1)
struct TreasureMakeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EBB6; ///< I|IVVV
#else
	static const int          FormatCheckCode = 0x05110DD6; ///< I|Ivvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  map;                          ///< 场景id
	const char*               pos;                          ///< 坐标
	const char*               event_id;                     ///< 事件id池
	const char*               event_prob;                   ///< 对应的概率
};
#pragma pack()

struct TreasureMake : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  map()                         { return t_map; }
	const std::pair<uint32_t, uint32_t>&        pos()                         { return t_pos; }
	const std::vector<uint32_t>&        event_id()                    { return t_event_id; }
	const std::vector<uint32_t>&        event_prob()                  { return t_event_prob; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_map;                        ///< 场景id
	std::pair<uint32_t, uint32_t>               t_pos;                        ///< 坐标
	std::vector<uint32_t>               t_event_id;                   ///< 事件id池
	std::vector<uint32_t>               t_event_prob;                 ///< 对应的概率

public:
	void reset();
	void fill( const TreasureMakeBase& base );
	const char* getClassName() const { return "c藏宝图表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TreasureMake, TreasureMakeBase > TreasureMakeManager;

inline void TreasureMake::reset()
{
	this->t_id = 0;
	this->t_map = 0;
	//this->t_pos.clear();
	this->t_event_id.clear();
	this->t_event_prob.clear();
};

inline void TreasureMake::fill( const TreasureMakeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_map = base.map;
	//if( base.pos == NULL ) this->t_pos.clear(); else this->t_pos.assign( base.pos );
	Dawn::parseTuple(this->t_pos, base.pos, "$");
	//if( base.event_id == NULL ) this->t_event_id.clear(); else this->t_event_id.assign( base.event_id );
	Dawn::parseContainer<uint32_t>(this->t_event_id, base.event_id, "$");
	//if( base.event_prob == NULL ) this->t_event_prob.clear(); else this->t_event_prob.assign( base.event_prob );
	Dawn::parseContainer<uint32_t>(this->t_event_prob, base.event_prob, "$");
};

inline const std::string TreasureMake::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TreasureMake.tbls64";
#else
	return "TreasureMake.tbls32";
#endif
};

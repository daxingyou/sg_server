#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:TreasureEventBase
/// @remark   记录大小:12 X32
///           记录大小:16 X64
///           字段数量:3
///           数据文件:c藏宝图表.xlsx treasure_event
///           排序主键:id
#pragma pack(1)
struct TreasureEventBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510E6; ///< I|IV
#else
	static const int          FormatCheckCode = 0x00051106; ///< I|Iv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  type;                         ///< 事件类型
	const char*               drop_id;                      ///< 掉落ID
};
#pragma pack()

struct TreasureEvent : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	const std::vector<uint32_t>&        drop_id()                     { return t_drop_id; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_type;                       ///< 事件类型
	std::vector<uint32_t>               t_drop_id;                    ///< 掉落ID

public:
	void reset();
	void fill( const TreasureEventBase& base );
	const char* getClassName() const { return "c藏宝图表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TreasureEvent, TreasureEventBase > TreasureEventManager;

inline void TreasureEvent::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_drop_id.clear();
};

inline void TreasureEvent::fill( const TreasureEventBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	//if( base.drop_id == NULL ) this->t_drop_id.clear(); else this->t_drop_id.assign( base.drop_id );
	Dawn::parseContainer<uint32_t>(this->t_drop_id, base.drop_id, "$");
};

inline const std::string TreasureEvent::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TreasureEvent.tbls64";
#else
	return "TreasureEvent.tbls32";
#endif
};

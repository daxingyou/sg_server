#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"

/// @brief    表格结构:AchievelistBase
/// @remark   记录大小:24 X32
///           记录大小:32 X64
///           字段数量:6
///           数据文件:c成就表.xlsx 成就列表
///           排序主键:成就ID
#pragma pack(1)
struct AchievelistBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010EADB6; ///< I|IVIIV
#else
	static const int          FormatCheckCode = 0x0110AE56; ///< I|IvIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 成就ID 生成唯一键

	uint32_t                  id;                           ///< 成就ID

	uint32_t                  level;                        ///< 成就等级
	const char*               condition;                    ///< 成就完成条件(事件:ID:数量)
	uint32_t                  drop_id;                      ///< 奖励道具（掉落ID)
	uint32_t                  type;                         ///< 成就类型(搜索)
	const char*               des;                          ///< 成就描述
};
#pragma pack()

struct Achievelist : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	const Dawn::triple< uint32_t, uint32_t, uint32_t>&        condition()                   { return t_condition; }
	uint32_t                  drop_id()                     { return t_drop_id; }
	uint32_t                  type()                        { return t_type; }
	const std::string&        des()                         { return t_des; }

private:
	uint32_t                  t_id;                         ///< 成就ID

	uint32_t                  t_level;                      ///< 成就等级
	Dawn::triple< uint32_t,uint32_t, uint32_t>			t_condition;                  ///< 成就完成条件(事件:ID:数量)
	uint32_t                  t_drop_id;                    ///< 奖励道具（掉落ID)
	uint32_t                  t_type;                       ///< 成就类型(搜索)
	std::string               t_des;                        ///< 成就描述

public:
	void reset();
	void fill( const AchievelistBase& base );
	const char* getClassName() const { return "c成就表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Achievelist, AchievelistBase > AchievelistManager;

inline void Achievelist::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_condition.clear();
	this->t_drop_id = 0;
	this->t_type = 0;
	this->t_des.clear();
};

inline void Achievelist::fill( const AchievelistBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	//if( base.condition == NULL ) this->t_condition.clear(); else this->t_condition.assign( base.condition );
	Dawn::parseTuple(this->t_condition, base.condition, ":");
	this->t_drop_id = base.drop_id;
	this->t_type = base.type;
	if( base.des == NULL ) this->t_des.clear(); else this->t_des.assign( base.des );
};

inline const std::string Achievelist::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Achievelist.tbls64";
#else
	return "Achievelist.tbls32";
#endif
};

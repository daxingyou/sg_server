#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SDTargetCountTableBase
/// @remark   记录大小:12 X32
///           记录大小:16 X64
///           字段数量:3
///           数据文件:c成就表.xlsx 七日目标
///           排序主键:成就ID
#pragma pack(1)
struct SDTargetCountTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000511A9; ///< I|VI
#else
	static const int          FormatCheckCode = 0x000513A9; ///< I|vI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 成就ID 生成唯一键

	uint32_t                  id;                           ///< 成就ID

	const char*               condition;                    ///< 成就完成条件(事件:ID:数量)
	uint32_t                  drop_id;                      ///< 奖励道具（掉落ID)
};
#pragma pack()

struct SDTargetCountTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        condition()                   { return t_condition; }
	uint32_t                  drop_id()                     { return t_drop_id; }

	uint32_t					get_type() { return  t_type; }
	uint32_t					get_eventid() { return t_eventid; }
	uint32_t					get_num() { return t_num;  }

private:
	uint32_t                  t_id;                         ///< 成就ID

	std::string               t_condition;                  ///< 成就完成条件(事件:ID:数量)
	uint32_t                  t_drop_id;                    ///< 奖励道具（掉落ID)

	uint32_t				t_type;
	uint32_t				t_eventid;
	uint32_t				t_num;
public:
	void reset();
	void fill( const SDTargetCountTableBase& base );
	const char* getClassName() const { return "c成就表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SDTargetCountTable, SDTargetCountTableBase > SDTargetCountTableManager;

inline void SDTargetCountTable::reset()
{
	this->t_id = 0;
	this->t_condition.clear();
	this->t_drop_id = 0;
};

inline void SDTargetCountTable::fill( const SDTargetCountTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.condition == NULL ) this->t_condition.clear(); else this->t_condition.assign( base.condition );
	this->t_drop_id = base.drop_id;

	std::vector<uint32_t>tmp_vector;
	common::string_util_t::split<uint32_t>(std::string(base.condition), tmp_vector, ":");
	if (tmp_vector.size() == 3)
	{
		this->t_type = tmp_vector[0];
		this->t_eventid = tmp_vector[1];
		this->t_num = tmp_vector[2];
	}

};

inline const std::string SDTargetCountTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SDTargetCountTable.tbls64";
#else
	return "SDTargetCountTable.tbls32";
#endif
};

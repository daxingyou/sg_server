#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/utility.hpp"

/// @brief    表格结构:BusinessActiveTableBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:y运营活动表.xlsx 活动管理表
///           排序主键:活动ID
#pragma pack(1)
struct BusinessActiveTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00511AD9; ///< I|VII
#else
	static const int          FormatCheckCode = 0x00513AD9; ///< I|vII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 活动ID 生成唯一键

	uint32_t                  id;                           ///< 活动ID

	const char*               open_cond;                    ///< 开启条件
	uint32_t                  time_id;                      ///< 时间ID
	uint32_t                  remove_cond;                  ///< 移除条件
};
#pragma pack()

struct BusinessActiveTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        open_cond()                   { return t_open_cond; }
	uint32_t                  time_id()                     { return t_time_id; }
	uint32_t                  remove_cond()                 { return t_remove_cond; }
	uint32_t				  get_open_cond() { return t_open_cond_type; }
	uint32_t				  get_open_value() { return t_open_cond_value; }

private:
	uint32_t                  t_id;                         ///< 活动ID

	std::string               t_open_cond;                  ///< 开启条件
	uint32_t                  t_time_id;                    ///< 时间ID
	uint32_t                  t_remove_cond;                ///< 移除条件

	uint32_t				  t_open_cond_type = 0;				//开启条件类型
	uint32_t				  t_open_cond_value = 0;			//开启条件数值
public:
	void reset();
	void fill( const BusinessActiveTableBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< BusinessActiveTable, BusinessActiveTableBase > BusinessActiveTableManager;

inline void BusinessActiveTable::reset()
{
	this->t_id = 0;
	this->t_open_cond.clear();
	this->t_time_id = 0;
	this->t_remove_cond = 0;
	this->t_open_cond_type = 0;
	this->t_open_cond_value = 0;
};

inline void BusinessActiveTable::fill( const BusinessActiveTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.open_cond == NULL ) this->t_open_cond.clear(); else this->t_open_cond.assign( base.open_cond );
	this->t_time_id = base.time_id;
	this->t_remove_cond = base.remove_cond;

	if (base.open_cond)
	{
		std::vector<std::uint32_t> conditions;
		common::string_util_t::split<uint32_t>(std::string(base.open_cond), conditions, ":");
		if (conditions.size() == 2)
		{
			this->t_open_cond_type = conditions[0];
			this->t_open_cond_value = conditions[1];
		}
	}
};

inline const std::string BusinessActiveTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "BusinessActiveTable.tbls64";
#else
	return "BusinessActiveTable.tbls32";
#endif
};

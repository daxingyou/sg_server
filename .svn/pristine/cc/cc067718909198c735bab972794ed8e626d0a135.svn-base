#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:GeneralEventTargetTableBase
/// @remark   记录大小:24 X32
///           记录大小:24 X64
///           字段数量:6
///           数据文件:t天下大势.xlsx 目标
///           排序主键:id
#pragma pack(1)
struct GeneralEventTargetTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0DDD8CE6; ///< I|IIIIIIV
#else
	static const int          FormatCheckCode = 0x0DDD8D06; ///< I|IIIIIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  finish_type;                  ///< 完成类型
	uint32_t                  finish_cond;                  ///< 完成条件
	uint32_t                  finish_value;                 ///< 完成值
	uint32_t                  finish_num;                   ///< 完成数量
	uint32_t                  drop_id;                      ///< 获得奖励
	uint32_t                  is_record;                    ///< 记录
	const char*               open_fun;                     ///< 开启玩法
};
#pragma pack()

struct GeneralEventTargetTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  finish_type()                 { return t_finish_type; }
	uint32_t                  finish_cond()                 { return t_finish_cond; }
	uint32_t                  finish_value()                { return t_finish_value; }
	uint32_t                  finish_num()                  { return t_finish_num; }
	uint32_t                  drop_id()                     { return t_drop_id; }
	uint32_t                  is_record()                   { return t_is_record; }
	const std::string&        open_fun()                    { return t_open_fun; }
	std::vector<uint32_t>	  &open_fun_list() { return t_open_fun_list; }				///开启列表

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_finish_type;                ///< 完成类型
	uint32_t                  t_finish_cond;                ///< 完成条件
	uint32_t                  t_finish_value;               ///< 完成值
	uint32_t                  t_finish_num;                 ///< 完成数量
	uint32_t                  t_drop_id;                    ///< 获得奖励
	uint32_t                  t_is_record;                  ///< 记录
	std::string               t_open_fun;                   ///< 开启玩法
	std::vector<uint32_t>	  t_open_fun_list;				///开启列表

public:
	void reset();
	void fill( const GeneralEventTargetTableBase& base );
	const char* getClassName() const { return "t天下大势"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< GeneralEventTargetTable, GeneralEventTargetTableBase > GeneralEventTargetTableManager;

inline void GeneralEventTargetTable::reset()
{
	this->t_id = 0;
	this->t_finish_type = 0;
	this->t_finish_cond = 0;
	this->t_finish_value = 0;
	this->t_finish_num = 0;
	this->t_drop_id = 0;
	this->t_is_record = 0;
	this->t_open_fun.clear();
};

inline void GeneralEventTargetTable::fill( const GeneralEventTargetTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_finish_type = base.finish_type;
	this->t_finish_cond = base.finish_cond;
	this->t_finish_value = base.finish_value;
	this->t_finish_num = base.finish_num;
	this->t_drop_id = base.drop_id;
	this->t_is_record = base.is_record;
	if( base.open_fun == NULL ) this->t_open_fun.clear(); else this->t_open_fun.assign( base.open_fun );

	if (NULL != base.open_fun)
	{
		common::string_util_t::split<uint32_t>(std::string(base.open_fun), t_open_fun_list, "$");
	}
};

inline const std::string GeneralEventTargetTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "GeneralEventTargetTable.tbls64";
#else
	return "GeneralEventTargetTable.tbls32";
#endif
};

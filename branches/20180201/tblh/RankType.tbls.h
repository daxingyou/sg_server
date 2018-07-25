#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:RankTypeBase
/// @remark   记录大小:28 X32
///           记录大小:32 X64
///           字段数量:7
///           数据文件:p排行榜.xlsx 排行榜类型
///           排序主键:类型ID
#pragma pack(1)
struct RankTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DDDBB9; ///< I|IIIIVI
#else
	static const int          FormatCheckCode = 0x00DDE5B9; ///< I|IIIIvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return type_id; }; // TODO: 请用 类型ID 生成唯一键

	uint32_t                  type_id;                      ///< 类型ID

	uint32_t                  flush_type;                   ///< 刷新类型
	uint32_t                  clear_type;                   ///< 清除类型
	uint32_t                  min_value;                    ///< 上榜条件
	uint32_t                  count;                        ///< 显示名次
	const char*               name;                         ///< 说明
	uint32_t                  owner_type;                   ///< 归属
};
#pragma pack()

struct RankType : public Entry
{
	uint32_t                  type_id()                     { return t_type_id; }

	uint32_t                  flush_type()                  { return t_flush_type; }
	uint32_t                  clear_type()                  { return t_clear_type; }
	uint32_t                  min_value()                   { return t_min_value; }
	uint32_t                  count()                       { return t_count; }
	const std::string&        name()                        { return t_name; }
	uint32_t                  owner_type()                  { return t_owner_type; }

private:
	uint32_t                  t_type_id;                    ///< 类型ID

	uint32_t                  t_flush_type;                 ///< 刷新类型
	uint32_t                  t_clear_type;                 ///< 清除类型
	uint32_t                  t_min_value;                  ///< 上榜条件
	uint32_t                  t_count;                      ///< 显示名次
	std::string               t_name;                       ///< 说明
	uint32_t                  t_owner_type;                 ///< 归属

public:
	void reset();
	void fill( const RankTypeBase& base );
	const char* getClassName() const { return "p排行榜"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RankType, RankTypeBase > RankTypeManager;

inline void RankType::reset()
{
	this->t_type_id = 0;
	this->t_flush_type = 0;
	this->t_clear_type = 0;
	this->t_min_value = 0;
	this->t_count = 0;
	this->t_name.clear();
	this->t_owner_type = 0;
};

inline void RankType::fill( const RankTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_type_id = base.type_id;
	this->t_flush_type = base.flush_type;
	this->t_clear_type = base.clear_type;
	this->t_min_value = base.min_value;
	this->t_count = base.count;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_owner_type = base.owner_type;
};

inline const std::string RankType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RankType.tbls64";
#else
	return "RankType.tbls32";
#endif
};

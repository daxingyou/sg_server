#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TroopTypeBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:z组队活动表.xlsx team_activities
///           排序主键:组队活动编号
#pragma pack(1)
struct TroopTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return type; }; // TODO: 请用 组队活动编号 生成唯一键

	uint32_t                  type;                         ///< 组队活动编号

	uint32_t                  big_type;                     ///< 组队活动类型编号
	uint32_t                  activity_id;                  ///< 组队活动对应活动编号
};
#pragma pack()

struct TroopType : public Entry
{
	uint32_t                  type()                        { return t_type; }

	uint32_t                  big_type()                    { return t_big_type; }
	uint32_t                  activity_id()                 { return t_activity_id; }

private:
	uint32_t                  t_type;                       ///< 组队活动编号

	uint32_t                  t_big_type;                   ///< 组队活动类型编号
	uint32_t                  t_activity_id;                ///< 组队活动对应活动编号

public:
	void reset();
	void fill( const TroopTypeBase& base );
	const char* getClassName() const { return "z组队活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TroopType, TroopTypeBase > TroopTypeManager;

inline void TroopType::reset()
{
	this->t_type = 0;
	this->t_big_type = 0;
	this->t_activity_id = 0;
};

inline void TroopType::fill( const TroopTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_type = base.type;
	this->t_big_type = base.big_type;
	this->t_activity_id = base.activity_id;
};

inline const std::string TroopType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TroopType.tbls64";
#else
	return "TroopType.tbls32";
#endif
};

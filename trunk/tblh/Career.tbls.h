#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:CareerBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:c成长历程.xlsx 成长历程
///           排序主键:等级
#pragma pack(1)
struct CareerBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 等级 生成唯一键

	uint32_t                  id;                           ///< 等级

	uint32_t                  drop_id;                      ///< 奖励ID
};
#pragma pack()

struct Career : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_id()                     { return t_drop_id; }

private:
	uint32_t                  t_id;                         ///< 等级

	uint32_t                  t_drop_id;                    ///< 奖励ID

public:
	void reset();
	void fill( const CareerBase& base );
	const char* getClassName() const { return "c成长历程"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Career, CareerBase > CareerManager;

inline void Career::reset()
{
	this->t_id = 0;
	this->t_drop_id = 0;
};

inline void Career::fill( const CareerBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_id = base.drop_id;
};

inline const std::string Career::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Career.tbls64";
#else
	return "Career.tbls32";
#endif
};

#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:PatrolHireBase
/// @remark   记录大小:24 X32
///           记录大小:24 X64
///           字段数量:6
///           数据文件:x巡逻表.xlsx 巡逻奖励
///           排序主键:场景ID
#pragma pack(1)
struct PatrolHireBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DDD89; ///< I|IIIII
#else
	static const int          FormatCheckCode = 0x010DDD89; ///< I|IIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 场景ID 生成唯一键

	uint32_t                  id;                           ///< 场景ID

	uint32_t                  drop_one;                     ///< 单场掉落
	uint32_t                  drop_ten;                     ///< 十场掉落
	uint32_t                  drop_hundred;                 ///< 百场掉落
	uint32_t                  drop_thousand;                ///< 千场掉落
	uint32_t                  level;                        ///< 解锁等级
};
#pragma pack()

struct PatrolHire : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_one()                    { return t_drop_one; }
	uint32_t                  drop_ten()                    { return t_drop_ten; }
	uint32_t                  drop_hundred()                { return t_drop_hundred; }
	uint32_t                  drop_thousand()               { return t_drop_thousand; }
	uint32_t                  level()                       { return t_level; }

private:
	uint32_t                  t_id;                         ///< 场景ID

	uint32_t                  t_drop_one;                   ///< 单场掉落
	uint32_t                  t_drop_ten;                   ///< 十场掉落
	uint32_t                  t_drop_hundred;               ///< 百场掉落
	uint32_t                  t_drop_thousand;              ///< 千场掉落
	uint32_t                  t_level;                      ///< 解锁等级

public:
	void reset();
	void fill( const PatrolHireBase& base );
	const char* getClassName() const { return "x巡逻表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< PatrolHire, PatrolHireBase > PatrolHireManager;

inline void PatrolHire::reset()
{
	this->t_id = 0;
	this->t_drop_one = 0;
	this->t_drop_ten = 0;
	this->t_drop_hundred = 0;
	this->t_drop_thousand = 0;
	this->t_level = 0;
};

inline void PatrolHire::fill( const PatrolHireBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_one = base.drop_one;
	this->t_drop_ten = base.drop_ten;
	this->t_drop_hundred = base.drop_hundred;
	this->t_drop_thousand = base.drop_thousand;
	this->t_level = base.level;
};

inline const std::string PatrolHire::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "PatrolHire.tbls64";
#else
	return "PatrolHire.tbls32";
#endif
};

#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TowerAchieveBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:s时空幻境.xlsx 首通奖励表
///           排序主键:id
#pragma pack(1)
struct TowerAchieveBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  floor;                        ///< 层数
	uint32_t                  award;                        ///< 奖励
};
#pragma pack()

struct TowerAchieve : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  floor()                       { return t_floor; }
	uint32_t                  award()                       { return t_award; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_floor;                      ///< 层数
	uint32_t                  t_award;                      ///< 奖励

public:
	void reset();
	void fill( const TowerAchieveBase& base );
	const char* getClassName() const { return "s时空幻境"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TowerAchieve, TowerAchieveBase > TowerAchieveManager;

inline void TowerAchieve::reset()
{
	this->t_id = 0;
	this->t_floor = 0;
	this->t_award = 0;
};

inline void TowerAchieve::fill( const TowerAchieveBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_floor = base.floor;
	this->t_award = base.award;
};

inline const std::string TowerAchieve::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TowerAchieve.tbls64";
#else
	return "TowerAchieve.tbls32";
#endif
};

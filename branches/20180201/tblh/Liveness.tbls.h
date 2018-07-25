#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"

/// @brief    表格结构:LivenessBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:h活动表.xlsx 奖励列表
///           排序主键:需求活跃度
#pragma pack(1)
struct LivenessBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return liveness; }; // TODO: 请用 需求活跃度 生成唯一键

	uint32_t                  liveness;                     ///< 需求活跃度

	const char*               award;                        ///< 道具ID
};
#pragma pack()

struct Liveness : public Entry
{
	uint32_t                  liveness()                    { return t_liveness; }

	const std::pair<uint32_t, uint32_t>&        award()                       { return t_award; }

private:
	uint32_t                  t_liveness;                   ///< 需求活跃度

	std::pair<uint32_t, uint32_t>               t_award;                      ///< 道具ID

public:
	void reset();
	void fill( const LivenessBase& base );
	const char* getClassName() const { return "h活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Liveness, LivenessBase > LivenessManager;

inline void Liveness::reset()
{
	this->t_liveness = 0;
	//this->t_award.clear();
};

inline void Liveness::fill( const LivenessBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_liveness = base.liveness;
	//if( base.award == NULL ) this->t_award.clear(); else this->t_award.assign( base.award );
	Dawn::parseTuple(this->t_award, base.award, "$");
};

inline const std::string Liveness::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Liveness.tbls64";
#else
	return "Liveness.tbls32";
#endif
};

#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:BountyBase
/// @remark   记录大小:44 X32
///           记录大小:56 X64
///           字段数量:11
///           数据文件:g国家悬赏表.xlsx Sheet1
///           排序主键:悬赏id
#pragma pack(1)
struct BountyBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0AB73769; ///< I|VIIIVVIIVII
#else
	static const int          FormatCheckCode = 0x0C98D489; ///< I|vIIIvvIIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 悬赏id 生成唯一键

	uint32_t                  id;                           ///< 悬赏id

	const char*               level_range;                  ///< 等级区间
	uint32_t                  star;                         ///< 难度星级
	uint32_t                  prob;                         ///< 悬赏概率
	uint32_t                  target_id;                    ///< 悬赏目标id
	const char*               destiny_id;                   ///< 宿命卡牌id
	const char*               restraint_id;                 ///< 克制卡牌id
	uint32_t                  restraint_target;             ///< 克制目标id
	uint32_t                  circle_id;                    ///< 环任务id
	const char*               award;                        ///< 普通掉落包
	uint32_t                  prestige_award;               ///< 声望奖励
	uint32_t                  country_award;                ///< 国家资源奖励
};
#pragma pack()

struct Bounty : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        level_range() { return t_level_range; }
	uint32_t                  star()                        { return t_star; }
	uint32_t                  prob()                        { return t_prob; }
	uint32_t                  target_id()                   { return t_target_id; }
	const std::vector<uint32_t>&        destiny_id()                  { return t_destiny_id; }
	const std::vector<uint32_t>&        restraint_id()                { return t_restraint_id; }
	uint32_t                  restraint_target()            { return t_restraint_target; }
	uint32_t                  circle_id()                   { return t_circle_id; }
	const std::vector<uint32_t>&        award()                       { return t_award; }
	uint32_t                  prestige_award()              { return t_prestige_award; }
	uint32_t                  country_award()               { return t_country_award; }

private:
	uint32_t                  t_id;                         ///< 悬赏id

	std::vector<uint32_t>				t_level_range;                ///< 等级区间
	uint32_t                  t_star;                       ///< 难度星级
	uint32_t                  t_prob;                       ///< 悬赏概率
	uint32_t                  t_target_id;                  ///< 悬赏目标id
	std::vector<uint32_t>               t_destiny_id;                 ///< 宿命卡牌id
	std::vector<uint32_t>               t_restraint_id;               ///< 克制卡牌id
	uint32_t                  t_restraint_target;           ///< 克制目标id
	uint32_t                  t_circle_id;                  ///< 环任务id
	std::vector<uint32_t>               t_award;                      ///< 普通掉落包
	uint32_t                  t_prestige_award;             ///< 声望奖励
	uint32_t                  t_country_award;              ///< 国家资源奖励

public:
	void reset();
	void fill( const BountyBase& base );
	const char* getClassName() const { return "g国家悬赏表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Bounty, BountyBase > BountyManager;

inline void Bounty::reset()
{
	this->t_id = 0;
	this->t_level_range.clear();
	this->t_star = 0;
	this->t_prob = 0;
	this->t_target_id = 0;
	this->t_destiny_id.clear();
	this->t_restraint_id.clear();
	this->t_restraint_target = 0;
	this->t_circle_id = 0;
	this->t_award.clear();
	this->t_prestige_award = 0;
	this->t_country_award = 0;
};

inline void Bounty::fill( const BountyBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	Dawn::parseContainer<uint32_t>(this->t_level_range, base.level_range, "$");
	this->t_star = base.star;
	this->t_prob = base.prob;
	this->t_target_id = base.target_id;
	//if( base.destiny_id == NULL ) this->t_destiny_id.clear(); else this->t_destiny_id.assign( base.destiny_id );
	Dawn::parseContainer<uint32_t>(this->t_destiny_id, base.destiny_id, "$");
	//if( base.restraint_id == NULL ) this->t_restraint_id.clear(); else this->t_restraint_id.assign( base.restraint_id );
	Dawn::parseContainer<uint32_t>(this->t_restraint_id, base.restraint_id, "$");
	this->t_restraint_target = base.restraint_target;
	this->t_circle_id = base.circle_id;
	//if( base.award == NULL ) this->t_award.clear(); else this->t_award.assign( base.award );
	Dawn::parseContainer<uint32_t>(this->t_award, base.award, "$");
	this->t_prestige_award = base.prestige_award;
	this->t_country_award = base.country_award;
};

inline const std::string Bounty::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Bounty.tbls64";
#else
	return "Bounty.tbls32";
#endif
};

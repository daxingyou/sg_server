#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LuckydrawBase
/// @remark   记录大小:36 X32
///           记录大小:36 X64
///           字段数量:9
///           数据文件:c抽卡表.xlsx Sheet1
///           排序主键:ID
#pragma pack(1)
struct LuckydrawBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0DD8CD09; ///< I|IIIIIIII
#else
	static const int          FormatCheckCode = 0x0DD8CD09; ///< I|IIIIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	uint32_t                  type;                         ///< 卡包ID
	uint32_t                  level;                        ///< 珍惜等级
	uint32_t                  vip_level;                    ///< VIP等级
	uint32_t                  award_type;                   ///< 掉落类型
	uint32_t                  award_id;                     ///< 掉落ID
	uint32_t                  star;                         ///< 星级
	uint32_t                  count;                        ///< 数量
	uint32_t                  prob;                         ///< 概率
};
#pragma pack()

struct Luckydraw : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  level()                       { return t_level; }
	uint32_t                  vip_level()                   { return t_vip_level; }
	uint32_t                  award_type()                  { return t_award_type; }
	uint32_t                  award_id()                    { return t_award_id; }
	uint32_t                  star()                        { return t_star; }
	uint32_t                  count()                       { return t_count; }
	uint32_t                  prob()                        { return t_prob; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_type;                       ///< 卡包ID
	uint32_t                  t_level;                      ///< 珍惜等级
	uint32_t                  t_vip_level;                  ///< VIP等级
	uint32_t                  t_award_type;                 ///< 掉落类型
	uint32_t                  t_award_id;                   ///< 掉落ID
	uint32_t                  t_star;                       ///< 星级
	uint32_t                  t_count;                      ///< 数量
	uint32_t                  t_prob;                       ///< 概率

public:
	void reset();
	void fill( const LuckydrawBase& base );
	const char* getClassName() const { return "c抽卡表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Luckydraw, LuckydrawBase > LuckydrawManager;

inline void Luckydraw::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_level = 0;
	this->t_vip_level = 0;
	this->t_award_type = 0;
	this->t_award_id = 0;
	this->t_star = 0;
	this->t_count = 0;
	this->t_prob = 0;
};

inline void Luckydraw::fill( const LuckydrawBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_level = base.level;
	this->t_vip_level = base.vip_level;
	this->t_award_type = base.award_type;
	this->t_award_id = base.award_id;
	this->t_star = base.star;
	this->t_count = base.count;
	this->t_prob = base.prob;
};

inline const std::string Luckydraw::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Luckydraw.tbls64";
#else
	return "Luckydraw.tbls32";
#endif
};

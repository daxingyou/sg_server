#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
/// @brief    表格结构:SectionEliteBase
/// @remark   记录大小:32 X32
///           记录大小:36 X64
///           字段数量:8
///           数据文件:z章节表.xlsx 精英挑战表
///           排序主键:id
#pragma pack(1)
struct SectionEliteBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0DEA8CD9; ///< I|IIVIIII
#else
	static const int          FormatCheckCode = 0x0E0A8CD9; ///< I|IIvIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  fight_id;                     ///< 对战id
	uint32_t                  page_id;                      ///< 所属章节
	const char*               cond_list;                    ///< 开启精英条件
	uint32_t                  drop_id;                      ///< 关卡奖励
	uint32_t                  sd_drop_id;                   ///< 扫荡奖励
	uint32_t                  max_sd_count;                 ///< 扫荡次数上限
	uint32_t                  sd_cost;                      ///< 消耗精力
};
#pragma pack()

struct SectionElite : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  fight_id()                    { return t_fight_id; }
	uint32_t                  page_id()                     { return t_page_id; }
	const std::map<uint32_t, uint32_t>&   cond_list()       { return t_cond_list; }
	uint32_t                  drop_id()                     { return t_drop_id; }
	uint32_t                  sd_drop_id()                  { return t_sd_drop_id; }
	uint32_t                  max_sd_count()                { return t_max_sd_count; }
	uint32_t                  sd_cost()                     { return t_sd_cost; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_fight_id;                   ///< 对战id
	uint32_t                  t_page_id;                    ///< 所属章节
	std::map<uint32_t, uint32_t>   t_cond_list;             ///< 开启精英条件
	uint32_t                  t_drop_id;                    ///< 关卡奖励
	uint32_t                  t_sd_drop_id;                 ///< 扫荡奖励
	uint32_t                  t_max_sd_count;               ///< 扫荡次数上限
	uint32_t                  t_sd_cost;                    ///< 消耗精力

public:
	void reset();
	void fill( const SectionEliteBase& base );
	const char* getClassName() const { return "z章节表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SectionElite, SectionEliteBase > SectionEliteManager;

inline void SectionElite::reset()
{
	this->t_id = 0;
	this->t_fight_id = 0;
	this->t_page_id = 0;
	this->t_cond_list.clear();
	this->t_drop_id = 0;
	this->t_sd_drop_id = 0;
	this->t_max_sd_count = 0;
	this->t_sd_cost = 0;
};

inline void SectionElite::fill( const SectionEliteBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_fight_id = base.fight_id;
	this->t_page_id = base.page_id;
	//if( base.cond_list == NULL ) this->t_cond_list.clear(); else this->t_cond_list.assign( base.cond_list );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_cond_list, base.cond_list, ":$");
	this->t_drop_id = base.drop_id;
	this->t_sd_drop_id = base.sd_drop_id;
	this->t_max_sd_count = base.max_sd_count;
	this->t_sd_cost = base.sd_cost;
};

inline const std::string SectionElite::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SectionElite.tbls64";
#else
	return "SectionElite.tbls32";
#endif
};

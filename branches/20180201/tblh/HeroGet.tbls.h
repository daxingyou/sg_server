#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:HeroGetBase
/// @remark   记录大小:28 X32
///           记录大小:36 X64
///           字段数量:7
///           数据文件:hero_get.xlsx hero_get
///           排序主键:武将id
#pragma pack(1)
struct HeroGetBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DEA8F6; ///< I|IIVIIV
#else
	static const int          FormatCheckCode = 0x00E0A916; ///< I|IIvIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 武将id 生成唯一键

	uint32_t                  id;                           ///< 武将id

	uint32_t                  appear_limit;                 ///< 出现条件（天下大势等级）
	uint32_t                  prestige_limit;               ///< 声望等级
	const char*               visit_quest;                  ///< 寻访任务起始id
	uint32_t                  hire_price;                   ///< 招募声望点数
	uint32_t                  hire_item;                    ///< 招募信物道具id
	const char*               extra_require;                ///< 额外招募条件
};
#pragma pack()

struct HeroGet : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  appear_limit()                { return t_appear_limit; }
	uint32_t                  prestige_limit()              { return t_prestige_limit; }
	const std::vector<uint32_t>& visit_quest()              { return t_visit_quest; }
	uint32_t                  hire_price()                  { return t_hire_price; }
	uint32_t                  hire_item()                   { return t_hire_item; }
	std::map<uint32_t, uint32_t>& extra_require()           { return t_extra_require; }

private:
	uint32_t                  t_id;                         ///< 武将id

	uint32_t                  t_appear_limit;               ///< 出现条件（天下大势等级）
	uint32_t                  t_prestige_limit;             ///< 声望等级
	std::vector<uint32_t>     t_visit_quest;                ///< 寻访任务起始id
	uint32_t                  t_hire_price;                 ///< 招募声望点数
	uint32_t                  t_hire_item;                  ///< 招募信物道具id
	std::map<uint32_t,uint32_t> t_extra_require;            ///< 额外招募条件

public:
	void reset();
	void fill( const HeroGetBase& base );
	const char* getClassName() const { return "hero_get"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroGet, HeroGetBase > HeroGetManager;

inline void HeroGet::reset()
{
	this->t_id = 0;
	this->t_appear_limit = 0;
	this->t_prestige_limit = 0;
	this->t_visit_quest.clear();
	this->t_hire_price = 0;
	this->t_hire_item = 0;
	this->t_extra_require.clear();
};

inline void HeroGet::fill( const HeroGetBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_appear_limit = base.appear_limit;
	this->t_prestige_limit = base.prestige_limit;
	if (base.visit_quest != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.visit_quest), t_visit_quest, "$");
	}
	this->t_hire_price = base.hire_price;
	this->t_hire_item = base.hire_item;
	if (base.extra_require != NULL)
	{
		std::vector<std::string> vec;
		string_util_t::split<std::string>(base.extra_require, vec, "$");
		string_util_t::Parse_id_cnt_str_to_map(vec, t_extra_require);
	}
};

inline const std::string HeroGet::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroGet.tbls64";
#else
	return "HeroGet.tbls32";
#endif
};

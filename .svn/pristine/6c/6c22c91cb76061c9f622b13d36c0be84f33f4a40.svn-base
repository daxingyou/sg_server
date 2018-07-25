#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LuckydrawWishBase
/// @remark   记录大小:28 X32
///           记录大小:40 X64
///           字段数量:7
///           数据文件:c抽卡表.xlsx 卡包详情
///           排序主键:卡包ID
#pragma pack(1)
struct LuckydrawWishBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DDEEA6; ///< I|IIIVVV
#else
	static const int          FormatCheckCode = 0x00DE08C6; ///< I|IIIvvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 卡包ID 生成唯一键

	uint32_t                  id;                           ///< 卡包ID

	uint32_t                  type;                         ///< 抽卡类型
	uint32_t                  default_open;                 ///< 默认开启
	uint32_t                  open_time;                    ///< 开启时间
	const char*               open_resource;                ///< 开启资源
	const char*               cost_resource;                ///< 货币消耗
	const char*               item_resource;                ///< 道具消耗
};
#pragma pack()

struct luckydraw_cost 
{
	uint32_t id = 0;
	uint32_t num = 0;
};

struct LuckydrawWish : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  default_open()                { return t_default_open; }
	uint32_t                  open_time()                   { return t_open_time; }

private:
	uint32_t                  t_id;                         ///< 卡包ID

	uint32_t                  t_type;                       ///< 抽卡类型
	uint32_t                  t_default_open;               ///< 默认开启
	uint32_t                  t_open_time;                  ///< 开启时间

public:
	luckydraw_cost			   open_resource;				
	luckydraw_cost			   cost_resource;				
	luckydraw_cost			   item_resource;				

public:
	void reset();
	void fill( const LuckydrawWishBase& base );
	const char* getClassName() const { return "c抽卡表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LuckydrawWish, LuckydrawWishBase > LuckydrawWishManager;

inline void LuckydrawWish::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_default_open = 0;
	this->t_open_time = 0;
};

inline void LuckydrawWish::fill( const LuckydrawWishBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_default_open = base.default_open;
	this->t_open_time = base.open_time;

	//开启道具
	if (NULL != base.open_resource)
	{
		std::vector<std::uint32_t> conditions;
		string_util_t::split<uint32_t>(std::string(base.open_resource), conditions, ":");
		if (conditions.size() == 2)
		{
			this->open_resource.id = conditions[0];
			this->open_resource.num = conditions[1];
		}
	}

	//抽卡消耗
	if (NULL != base.cost_resource)
	{
		std::vector<std::uint32_t> conditions;
		string_util_t::split<uint32_t>(std::string(base.cost_resource), conditions, ":");
		if (conditions.size() == 2)
		{
			this->cost_resource.id = conditions[0];
			this->cost_resource.num = conditions[1];
		}
	}
	
	//道具消耗
	if (NULL != base.item_resource)
	{
		std::vector<std::uint32_t> conditions;
		string_util_t::split<uint32_t>(std::string(base.item_resource), conditions, ":");
		if (conditions.size() == 2)
		{
			this->item_resource.id = conditions[0];
			this->item_resource.num = conditions[1];
		}
	}
};

inline const std::string LuckydrawWish::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LuckydrawWish.tbls64";
#else
	return "LuckydrawWish.tbls32";
#endif
};

#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:RechargeCheapBase
/// @remark   记录大小:32 X32
///           记录大小:48 X64
///           字段数量:8
///           数据文件:y运营活动表.xlsx 每日特惠
///           排序主键:ID
#pragma pack(1)
struct RechargeCheapBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0DDEEAB6; ///< I|IIIVVVV
#else
	static const int          FormatCheckCode = 0x0DE08CD6; ///< I|IIIvvvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	uint32_t                  count;                        ///< 充值额度
	uint32_t                  drop;                         ///< 掉落ID
	uint32_t                  limit;                        ///< 限购次数
	const char*               name;                         ///< 礼包名字
	const char*               desc;                         ///< 礼包描述
	const char*               display;                      ///< 礼包物品预览
	const char*               pic;                          ///< 预览礼包图片
};
#pragma pack()

struct RechargeCheap : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  count()                       { return t_count; }
	uint32_t                  drop()                        { return t_drop; }
	uint32_t                  limit()                       { return t_limit; }
	const std::string&        name()                        { return t_name; }
	const std::string&        desc()                        { return t_desc; }
	const std::string&        display()                     { return t_display; }
	const std::string&        pic()                         { return t_pic; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_count;                      ///< 充值额度
	uint32_t                  t_drop;                       ///< 掉落ID
	uint32_t                  t_limit;                      ///< 限购次数
	std::string               t_name;                       ///< 礼包名字
	std::string               t_desc;                       ///< 礼包描述
	std::string               t_display;                    ///< 礼包物品预览
	std::string               t_pic;                        ///< 预览礼包图片

public:
	void reset();
	void fill( const RechargeCheapBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RechargeCheap, RechargeCheapBase > RechargeCheapManager;

inline void RechargeCheap::reset()
{
	this->t_id = 0;
	this->t_count = 0;
	this->t_drop = 0;
	this->t_limit = 0;
	this->t_name.clear();
	this->t_desc.clear();
	this->t_display.clear();
	this->t_pic.clear();
};

inline void RechargeCheap::fill( const RechargeCheapBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_count = base.count;
	this->t_drop = base.drop;
	this->t_limit = base.limit;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	if( base.desc == NULL ) this->t_desc.clear(); else this->t_desc.assign( base.desc );
	if( base.display == NULL ) this->t_display.clear(); else this->t_display.assign( base.display );
	if( base.pic == NULL ) this->t_pic.clear(); else this->t_pic.assign( base.pic );
};

inline const std::string RechargeCheap::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RechargeCheap.tbls64";
#else
	return "RechargeCheap.tbls32";
#endif
};

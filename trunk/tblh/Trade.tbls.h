#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TradeBase
/// @remark   记录大小:28 X32
///           记录大小:44 X64
///           字段数量:7
///           数据文件:t跑商表.xlsx trade
///           排序主键:商会id
#pragma pack(1)
struct TradeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DEBEA6; ///< I|IIVVVV
#else
	static const int          FormatCheckCode = 0x00E0D8C6; ///< I|IIvvvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 商会id 生成唯一键

	uint32_t                  id;                           ///< 商会id

	uint32_t                  scene;                        ///< 所在场景
	uint32_t                  type;                         ///< 类型
	const char*               sell_goods;                   ///< 出售货物
	const char*               sell_cnt;                     ///< 出售货物数量
	const char*               unlock_type;                  ///< 货物解锁条件
	const char*               unlock_para;                  ///< 货物解锁参数
};
#pragma pack()

struct Trade : public Entry
{
	uint32_t                  id()                          { return t_id; }

// 	const std::string&        sell_goods()                  { return t_sell_goods; }
// 	const std::string&        sell_cnt()                    { return t_sell_cnt; }
// 	const std::string&        unlock_type()                 { return t_unlock_type; }
// 	const std::string&        unlock_para()                 { return t_unlock_para; }
	uint32_t                  scene()                       { return t_scene; }
	uint32_t                  type()                        { return t_type; }
	uint32_t		sell_goods_size() { return (uint32_t)t_sell_goods_vec.size(); }
	uint32_t		sell_cnt_size() { return (uint32_t)t_sell_cnt_vec.size(); }
	uint32_t		unlock_type_size() { return (uint32_t)t_unlock_type_vec.size(); }
	uint32_t		unlock_para_size() { return (uint32_t)t_unlock_para_vec.size(); }

	uint32_t        sell_goods(uint32_t pos) { return pos >= sell_goods_size() ? 0 : t_sell_goods_vec[pos]; }
	uint32_t        sell_cnt(uint32_t pos) { return pos >= sell_cnt_size() ? 0 : t_sell_cnt_vec[pos]; }
	uint32_t        unlock_type(uint32_t pos) { return pos >= unlock_type_size() ? 0 : t_unlock_type_vec[pos]; }
	uint32_t        unlock_para(uint32_t pos) { return pos >= unlock_para_size() ? 0 : t_unlock_para_vec[pos]; }
private:
	uint32_t                  t_id;                         ///< 商会id

	uint32_t                  t_scene;                      ///< 所在场景
	uint32_t                  t_type;                       ///< 类型
	std::string               t_sell_goods;                 ///< 出售货物
	std::string               t_sell_cnt;                   ///< 出售货物数量
	std::string               t_unlock_type;                ///< 货物解锁条件
	std::string               t_unlock_para;                ///< 货物解锁参数

	std::vector<uint32_t>	  t_sell_goods_vec;
	std::vector<uint32_t>	  t_sell_cnt_vec;
	std::vector<uint32_t>	  t_unlock_type_vec;
	std::vector<uint32_t>	  t_unlock_para_vec;

public:
	void reset();
	void fill( const TradeBase& base );
	const char* getClassName() const { return "t跑商表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Trade, TradeBase > TradeManager;

inline void Trade::reset()
{
	this->t_id = 0;
	this->t_scene = 0;
	this->t_type = 0;
	this->t_sell_goods.clear();
	this->t_sell_cnt.clear();
	this->t_unlock_type.clear();
	this->t_unlock_para.clear();

	this->t_sell_goods_vec.clear();
	this->t_sell_cnt_vec.clear();
	this->t_unlock_type_vec.clear();
	this->t_unlock_para_vec.clear();
};

inline void Trade::fill( const TradeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_scene = base.scene;
	this->t_type = base.type;
	if( base.sell_goods == NULL ) this->t_sell_goods.clear(); else this->t_sell_goods.assign( base.sell_goods );
	if( base.sell_cnt == NULL ) this->t_sell_cnt.clear(); else this->t_sell_cnt.assign( base.sell_cnt );
	if( base.unlock_type == NULL ) this->t_unlock_type.clear(); else this->t_unlock_type.assign( base.unlock_type );
	if( base.unlock_para == NULL ) this->t_unlock_para.clear(); else this->t_unlock_para.assign( base.unlock_para );
	
	if (base.sell_goods != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.sell_goods), t_sell_goods_vec, "$");
	}
	if (base.sell_cnt != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.sell_cnt), t_sell_cnt_vec, "$");
	}
	if (base.unlock_type != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.unlock_type), t_unlock_type_vec, "$");
	}
	if (base.unlock_para != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.unlock_para), t_unlock_para_vec, "$");
	}
};

inline const std::string Trade::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Trade.tbls64";
#else
	return "Trade.tbls32";
#endif
};

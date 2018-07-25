#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:ExpeditionShopBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:y远征表.xlsx 神秘商店
///           排序主键:ID
#pragma pack(1)
struct ExpeditionShopBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< 物品ID

	uint32_t                  count;                        ///< 可购买数量
	uint32_t                  money;                        ///< 金币价格
	uint32_t                  glory;                        ///< 荣耀值价格
};
#pragma pack()

struct ExpeditionShop : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  count()                       { return t_count; }
	uint32_t                  money()                       { return t_money; }
	uint32_t                  glory()                       { return t_glory; }

private:
	uint32_t                  t_id;                         ///< 物品ID

	uint32_t                  t_count;                      ///< 可购买数量
	uint32_t                  t_money;                      ///< 金币价格
	uint32_t                  t_glory;                      ///< 荣耀值价格

public:
	void reset();
	void fill( const ExpeditionShopBase& base );
	const char* getClassName() const { return "y远征表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ExpeditionShop, ExpeditionShopBase > ExpeditionShopManager;

inline void ExpeditionShop::reset()
{
	this->t_id = 0;
	this->t_count = 0;
	this->t_money = 0;
	this->t_glory = 0;
};

inline void ExpeditionShop::fill( const ExpeditionShopBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_count = base.count;
	this->t_money = base.money;
	this->t_glory = base.glory;
};

inline const std::string ExpeditionShop::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ExpeditionShop.tbls64";
#else
	return "ExpeditionShop.tbls32";
#endif
};

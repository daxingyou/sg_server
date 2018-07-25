#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:RechargeBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:c充值表.xlsx Sheet1
///           排序主键:id
#pragma pack(1)
struct RechargeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  CNY;                          ///< 人名币金额
	uint32_t                  money;                        ///< 获得金币
	uint32_t                  gift_money;                   ///< 首充赠送金币
};
#pragma pack()

struct Recharge : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  CNY()                         { return t_CNY; }
	uint32_t                  money()                       { return t_money; }
	uint32_t                  gift_money()                  { return t_gift_money; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_CNY;                        ///< 人名币金额
	uint32_t                  t_money;                      ///< 获得金币
	uint32_t                  t_gift_money;                 ///< 首充赠送金币

public:
	void reset();
	void fill( const RechargeBase& base );
	const char* getClassName() const { return "c充值表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Recharge, RechargeBase > RechargeManager;

inline void Recharge::reset()
{
	this->t_id = 0;
	this->t_CNY = 0;
	this->t_money = 0;
	this->t_gift_money = 0;
};

inline void Recharge::fill( const RechargeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_CNY = base.CNY;
	this->t_money = base.money;
	this->t_gift_money = base.gift_money;
};

inline const std::string Recharge::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Recharge.tbls64";
#else
	return "Recharge.tbls32";
#endif
};

#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:LuckydrawTypeBase
/// @remark   记录大小:20 X32
///           记录大小:32 X64
///           字段数量:5
///           数据文件:c抽卡表.xlsx 抽卡类型
///           排序主键:抽卡类型
#pragma pack(1)
struct LuckydrawTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x011AEBF9; ///< I|VIVVI
#else
	static const int          FormatCheckCode = 0x013B0DF9; ///< I|vIvvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 抽卡类型 生成唯一键

	uint32_t                  id;                           ///< 抽卡类型

	const char*               card;                         ///< 卡包ID
	uint32_t                  default_open;                 ///< 默认开启
	const char*               money;                        ///< 货币消耗
	const char*               material;                     ///< 道具消耗
	uint32_t                  add_lucky;                    ///< 获得气运值
};
#pragma pack()

struct LuckydrawType : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        card()                        { return t_card; }
	uint32_t                  default_open()                { return t_default_open; }
	const std::pair<uint32_t,uint32_t>&        money()                       { return t_money; }
	const std::pair<uint32_t,uint32_t>&        material()                    { return t_material; }
	uint32_t                  add_lucky() { return t_add_lucky; }
private:
	uint32_t                  t_id;                         ///< 抽卡类型

	std::vector<uint32_t>               t_card;                       ///< 卡包ID
	uint32_t                  t_default_open;               ///< 默认开启
	std::pair<uint32_t, uint32_t>               t_money;                      ///< 货币消耗
	std::pair<uint32_t, uint32_t>               t_material;                   ///< 道具消耗
	uint32_t                  t_add_lucky;                  ///< 获得气运值
public:
	void reset();
	void fill( const LuckydrawTypeBase& base );
	const char* getClassName() const { return "c抽卡表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LuckydrawType, LuckydrawTypeBase > LuckydrawTypeManager;

inline void LuckydrawType::reset()
{
	this->t_id = 0;
	this->t_card.clear();
	this->t_default_open = 0;
	//this->t_money.clear();
	//this->t_material.clear();
	this->t_add_lucky = 0;
};

inline void LuckydrawType::fill( const LuckydrawTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.card == NULL ) this->t_card.clear(); else this->t_card.assign( base.card );
	Dawn::parseContainer<uint32_t>(this->t_card, base.card, "$");
	this->t_default_open = base.default_open;
	//if( base.money == NULL ) this->t_money.clear(); else this->t_money.assign( base.money );
	Dawn::parseTuple(this->t_money, base.money, ":");
	//if( base.material == NULL ) this->t_material.clear(); else this->t_material.assign( base.material );
	Dawn::parseTuple(this->t_material, base.material, ":");
	this->t_add_lucky = base.add_lucky;
};

inline const std::string LuckydrawType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LuckydrawType.tbls64";
#else
	return "LuckydrawType.tbls32";
#endif
};

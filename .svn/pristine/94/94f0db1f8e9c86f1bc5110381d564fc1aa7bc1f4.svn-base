#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:EquipSoulBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:z装备表.xlsx 装备注灵
///           排序主键:器灵ID
#pragma pack(1)
struct EquipSoulBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510DDE6; ///< I|IIIV
#else
	static const int          FormatCheckCode = 0x0510DE06; ///< I|IIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 器灵ID 生成唯一键

	uint32_t                  id;                           ///< 器灵ID

	uint32_t                  special;                      ///< 特技
	uint32_t                  rate;                         ///< 成功率
	uint32_t                  equip_pos;                    ///< 部位
	const char*               money;                        ///< 消耗
};
#pragma pack()

struct EquipSoul : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  special()                     { return t_special; }
	uint32_t                  rate()                        { return t_rate; }
	uint32_t                  equip_pos()                   { return t_equip_pos; }
	const std::pair<uint32_t,uint32_t>&        money()                       { return t_money; }

private:
	uint32_t                  t_id;                         ///< 器灵ID

	uint32_t                  t_special;                    ///< 特技
	uint32_t                  t_rate;                       ///< 成功率
	uint32_t                  t_equip_pos;                  ///< 部位
	std::pair<uint32_t, uint32_t>               t_money;                      ///< 消耗

public:
	void reset();
	void fill( const EquipSoulBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipSoul, EquipSoulBase > EquipSoulManager;

inline void EquipSoul::reset()
{
	this->t_id = 0;
	this->t_special = 0;
	this->t_rate = 0;
	this->t_equip_pos = 0;
	//this->t_money.clear();
};

inline void EquipSoul::fill( const EquipSoulBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_special = base.special;
	this->t_rate = base.rate;
	this->t_equip_pos = base.equip_pos;
	//if( base.money == NULL ) this->t_money.clear(); else this->t_money.assign( base.money );
	Dawn::parseTuple(this->t_money, base.money, ":");
};

inline const std::string EquipSoul::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipSoul.tbls64";
#else
	return "EquipSoul.tbls32";
#endif
};

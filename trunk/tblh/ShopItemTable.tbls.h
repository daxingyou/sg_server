#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "tblh/RandomShopConfig.hpp"

/// @brief    ���ṹ:ShopItemTableBase
/// @remark   ��¼��С:48 X32
///           ��¼��С:52 X64
///           �ֶ�����:12
///           �����ļ�:s�̵��.xlsx Sheet3
///           ��������:ID
#pragma pack(1)
struct ShopItemTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0CD00329; ///< I|IIIIIIIIIVI
#else
	static const int          FormatCheckCode = 0x0CD01D29; ///< I|IIIIIIIIIvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ID ����Ψһ��

	uint32_t                  id;                           ///< ID

	uint32_t                  pool_id;                      ///< ��ID
	uint32_t                  family_limit;                 ///< �����޹�
	uint32_t                  personal_limit;               ///< �����޹�
	uint32_t                  family_level;                 ///< ����ȼ�
	uint32_t                  personal_level;               ///< ���˵ȼ�
	uint32_t                  item_id;                      ///< ����ID
	uint32_t                  item_val;                     ///< ����
	uint32_t                  money_type;                   ///< ��������
	uint32_t                  price;                        ///< �۸�
	const char*               lv_limit;                     ///< �ȼ�����
	uint32_t                  proportion;                   ///< �������
};
#pragma pack()

struct ShopItemTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  pool_id()                     { return t_pool_id; }
	uint32_t                  family_limit()				{ return t_family_limit; }
	uint32_t                  personal_limit()				{ return t_personal_limit; }
	uint32_t                  family_level()				{ return t_family_level; }
	uint32_t                  personal_level()				{ return t_personal_level; }
	uint32_t                  item_id()                     { return t_item_id; }
	uint32_t                  item_val()                    { return t_item_val; }
	uint32_t                  money_type()                  { return t_money_type; }
	uint32_t                  price()                       { return t_price; }
	uint32_t                  proportion()                  { return t_proportion; }
	uint32_t				  lv_min()						{ return t_lv_min; }
	uint32_t				  lv_max()						{ return t_lv_max; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_pool_id;                    ///< ��ID
	uint32_t                  t_family_limit;               ///< �����޹�
	uint32_t                  t_personal_limit;             ///< �����޹�
	uint32_t                  t_family_level;               ///< ����ȼ�
	uint32_t                  t_personal_level;             ///< ���˵ȼ�
	uint32_t                  t_item_id;                    ///< ����ID
	uint32_t                  t_item_val;                   ///< ����
	uint32_t                  t_money_type;                 ///< ��������
	uint32_t                  t_price;                      ///< �۸�
	uint32_t				  t_lv_min;						///< �ȼ�����
	uint32_t				  t_lv_max;						///< �ȼ�����
	uint32_t                  t_proportion;                 ///< �������

public:
	void reset();
	void fill( const ShopItemTableBase& base );
	const char* getClassName() const { return "s�̵��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ShopItemTable, ShopItemTableBase > ShopItemTableManager;

inline void ShopItemTable::reset()
{
	this->t_id = 0;
	this->t_pool_id = 0;
	this->t_family_limit = 0;
	this->t_personal_limit = 0;
	this->t_family_level = 0;
	this->t_personal_level = 0;
	this->t_item_id = 0;
	this->t_item_val = 0;
	this->t_money_type = 0;
	this->t_price = 0;
	this->t_lv_min = 0;
	this->t_lv_max = 0;
	this->t_proportion = 0;
};

inline void ShopItemTable::fill( const ShopItemTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_pool_id = base.pool_id;
	this->t_family_limit = base.family_limit;
	this->t_personal_limit = base.personal_limit;
	this->t_family_level = base.family_level;
	this->t_personal_level = base.personal_level;
	this->t_item_id = base.item_id;
	this->t_item_val = base.item_val;
	this->t_money_type = base.money_type;
	this->t_price = base.price;
	if (base.lv_limit != NULL)
	{
		std::vector<uint32_t> lvs;
		common::string_util_t::split<uint32_t>(std::string(base.lv_limit), lvs, ":");
		if (lvs.size() == 2)
		{
			this->t_lv_min = lvs[0];
			this->t_lv_max = lvs[1];
		}
	}
	this->t_proportion = base.proportion;

	RandomShopConfig_t::add_pool_item(this);
};

inline const std::string ShopItemTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ShopItemTable.tbls64";
#else
	return "ShopItemTable.tbls32";
#endif
};

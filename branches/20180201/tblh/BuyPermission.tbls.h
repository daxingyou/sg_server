#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:BuyPermissionBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:g����Ȩ�ޱ�.xlsx buy_permission
///           ��������:ID
#pragma pack(1)
struct BuyPermissionBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000511A9; ///< I|VI
#else
	static const int          FormatCheckCode = 0x000513A9; ///< I|vI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ID ����Ψһ��

	uint32_t                  id;                           ///< ID

	const char*               price_list;                   ///< ÿ�ι���۸�

	uint32_t                  max_buy_count;                ///< �������
};
#pragma pack()

struct BuyPermission : public Entry
{
	uint32_t                  id()                          { return t_id; }
	//const std::string&        price_list()                  { return t_price_list; }

	uint32_t                  max_buy_count()               { return t_max_buy_count; }

	uint32_t get_price( uint32_t count )
	{
		if (count > this->t_price_list.size())
			return 0;

		return t_price_list[count];
	}
private:
	uint32_t                  t_id;                         ///< ID
	//std::string               t_price_list;                 ///< ÿ�ι���۸�

	uint32_t                  t_max_buy_count;              ///< �������

	std::vector<uint32_t> t_price_list;
public:
	void reset();
	void fill( const BuyPermissionBase& base );
	const char* getClassName() const { return "g����Ȩ�ޱ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< BuyPermission, BuyPermissionBase > BuyPermissionManager;

inline void BuyPermission::reset()
{
	this->t_id = 0;
	this->t_price_list.clear();
	this->t_max_buy_count = 0;
};

inline void BuyPermission::fill( const BuyPermissionBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.price_list == NULL ) this->t_price_list.clear(); else this->t_price_list.assign( base.price_list );
	this->t_max_buy_count = base.max_buy_count;
	common::string_util_t::split<uint32_t>(std::string(base.price_list), this->t_price_list, ":");
};

inline const std::string BuyPermission::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "BuyPermission.tbls64";
#else
	return "BuyPermission.tbls32";
#endif
};

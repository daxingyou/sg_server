#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:EquipExchangeBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:36 X64
///           �ֶ�����:7
///           �����ļ�:zװ����.xlsx װ���һ���
///           ��������:����id
#pragma pack(1)
struct EquipExchangeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0DD8FB79; ///< I|IIIIIVVI
#else
	static const int          FormatCheckCode = 0x0DD91D79; ///< I|IIIIIvvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����id ����Ψһ��

	uint32_t                  id;                           ///< ����id

	uint32_t                  exchange_pool;                ///< ��ͨ�һ���
	uint32_t                  ex_exchange_pool;             ///< ǿ���һ���
	uint32_t                  exchange_lucky_pool;          ///< ��ͨ���˶һ���
	uint32_t                  ex_exchange_lucky_pool;       ///< ǿ�����˶һ���
	uint32_t                  bonus;                        ///< �һ��¼���¼����
	const char*               material;                     ///< ��ͨ������Ʒid
	const char*               ex_material;                  ///< ǿ��������Ʒid
	uint32_t                  copper;                       ///< ÿ������ͭ��
};
#pragma pack()

struct EquipExchange : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  exchange_pool()               { return t_exchange_pool; }
	uint32_t                  ex_exchange_pool()            { return t_ex_exchange_pool; }
	uint32_t                  exchange_lucky_pool() { return t_exchange_lucky_pool; }
	uint32_t                  ex_exchange_lucky_pool() { return t_ex_exchange_lucky_pool; }
	uint32_t                  bonus()                       { return t_bonus; }
	const std::map<uint32_t,uint32_t>&        material()                    { return t_material; }
	const std::map<uint32_t, uint32_t>&        ex_material()                 { return t_ex_material; }
	uint32_t                  copper()                      { return t_copper; }

private:
	uint32_t                  t_id;                         ///< ����id

	uint32_t                  t_exchange_pool;              ///< ��ͨ�һ���
	uint32_t                  t_ex_exchange_pool;           ///< ǿ���һ���
	uint32_t                  t_exchange_lucky_pool;        ///< ��ͨ���˶һ���
	uint32_t                  t_ex_exchange_lucky_pool;     ///< ǿ�����˶һ���
	uint32_t                  t_bonus;                      ///< �һ��¼���¼����
	std::map<uint32_t, uint32_t>               t_material;                   ///< ��ͨ������Ʒid
	std::map<uint32_t, uint32_t>               t_ex_material;                ///< ǿ��������Ʒid
	uint32_t                  t_copper;                     ///< ÿ������ͭ��

public:
	void reset();
	void fill( const EquipExchangeBase& base );
	const char* getClassName() const { return "zװ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipExchange, EquipExchangeBase > EquipExchangeManager;

inline void EquipExchange::reset()
{
	this->t_id = 0;
	this->t_exchange_pool = 0;
	this->t_ex_exchange_pool = 0;
	this->t_exchange_lucky_pool = 0;
	this->t_ex_exchange_lucky_pool = 0;
	this->t_bonus = 0;
	this->t_material.clear();
	this->t_ex_material.clear();
	this->t_copper = 0;
};

inline void EquipExchange::fill( const EquipExchangeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_exchange_pool = base.exchange_pool;
	this->t_ex_exchange_pool = base.ex_exchange_pool;
	this->t_exchange_lucky_pool = base.exchange_lucky_pool;
	this->t_ex_exchange_lucky_pool = base.ex_exchange_lucky_pool;
	this->t_bonus = base.bonus;
	//if( base.material == NULL ) this->t_material.clear(); else this->t_material.assign( base.material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_material, base.material, ":$");
	//if( base.ex_material == NULL ) this->t_ex_material.clear(); else this->t_ex_material.assign( base.ex_material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_ex_material, base.ex_material, ":$");
	this->t_copper = base.copper;
};

inline const std::string EquipExchange::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipExchange.tbls64";
#else
	return "EquipExchange.tbls32";
#endif
};

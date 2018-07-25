#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:MoneyTreeTableBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:28 X64
///           �ֶ�����:5
///           �����ļ�:yҡǮ��.xlsx ҡǮ��
///           ��������:ҡǮ���ȼ�
#pragma pack(1)
struct MoneyTreeTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511ADE6; ///< I|VIIV
#else
	static const int          FormatCheckCode = 0x0513AE06; ///< I|vIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return lv; }; // TODO: ���� ҡǮ���ȼ� ����Ψһ��

	uint32_t                  lv;                           ///< ҡǮ���ȼ�

	const char*               consum;                       ///< �������ĵ��ߺ�����
	uint32_t                  max_count;                    ///< ҡһҡ����
	uint32_t                  exGet;                        ///< ������
	const char*               crit_chance;                  ///< ��������������
};
#pragma pack()

struct MoneyTreeTable : public Entry
{
	uint32_t                  lv()                          { return t_lv; }

	uint32_t                  max_count()                   { return t_max_count; }
	uint32_t                  exGet()                       { return t_exGet; }

	std::map<uint32_t, uint32_t>    &get_levelup_cost_map() { return t_levelup_cost; };				//��������
	std::map<uint32_t, uint32_t>    &get_crit_chance_map() { return t_crit_chance;  };				//��������
private:
	uint32_t                  t_lv;                         ///< ҡǮ���ȼ�

	uint32_t                  t_max_count;                  ///< ҡһҡ����
	uint32_t                  t_exGet;                      ///< ������

	std::map<uint32_t,uint32_t>     t_levelup_cost;				//��������
	std::map<uint32_t,uint32_t>     t_crit_chance;				//��������
public:
	void reset();
	void fill( const MoneyTreeTableBase& base );
	const char* getClassName() const { return "yҡǮ��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MoneyTreeTable, MoneyTreeTableBase > MoneyTreeTableManager;

inline void MoneyTreeTable::reset()
{
	this->t_lv = 0;
	this->t_max_count = 0;
	this->t_exGet = 0;
	this->t_crit_chance.clear();
	this->t_levelup_cost.clear();
};

inline void MoneyTreeTable::fill( const MoneyTreeTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_lv = base.lv;
	this->t_max_count = base.max_count;
	this->t_exGet = base.exGet;

	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_crit_chance, base.crit_chance, "$:");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_levelup_cost, base.consum, "$:");
};

inline const std::string MoneyTreeTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MoneyTreeTable.tbls64";
#else
	return "MoneyTreeTable.tbls32";
#endif
};

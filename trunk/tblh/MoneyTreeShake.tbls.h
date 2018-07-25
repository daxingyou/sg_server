#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:MoneyTreeShakeBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:yҡǮ��.xlsx ����
///           ��������:ҡһҡ����
#pragma pack(1)
struct MoneyTreeShakeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return time; }; // TODO: ���� ҡһҡ���� ����Ψһ��

	uint32_t                  time;                         ///< ҡһҡ����

	const char*               consum;                       ///< ҡǮ������Ԫ��
};
#pragma pack()

struct MoneyTreeShake : public Entry
{
	uint32_t                  time()                        { return t_time; }

	uint32_t				 get_cost_money(uint32_t tree_level)
	{
		tree_level = tree_level - 1;
		if (tree_level > t_consum_money.size())
		{
			return 0;
		}
		return t_consum_money[tree_level];
	}
private:
	uint32_t                  t_time;                       ///< ҡһҡ����

	std::vector<uint32_t>     t_consum_money;
public:
	void reset();
	void fill( const MoneyTreeShakeBase& base );
	const char* getClassName() const { return "yҡǮ��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MoneyTreeShake, MoneyTreeShakeBase > MoneyTreeShakeManager;

inline void MoneyTreeShake::reset()
{
	this->t_time = 0;
	this->t_consum_money.clear();
};

inline void MoneyTreeShake::fill( const MoneyTreeShakeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_time = base.time;
	common::string_util_t::split<uint32_t>(std::string(base.consum), this->t_consum_money, "$");
};

inline const std::string MoneyTreeShake::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MoneyTreeShake.tbls64";
#else
	return "MoneyTreeShake.tbls32";
#endif
};

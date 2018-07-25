#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:CountryFlagBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:8 X64
///           �ֶ�����:2
///           �����ļ�:g���ұ�.xlsx country_flag
///           ��������:id
#pragma pack(1)
struct CountryFlagBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  unlock_condi;                 ///< ��������
};
#pragma pack()

struct CountryFlag : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  unlock_condi()                { return t_unlock_condi; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_unlock_condi;               ///< ��������

public:
	void reset();
	void fill( const CountryFlagBase& base );
	const char* getClassName() const { return "g���ұ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< CountryFlag, CountryFlagBase > CountryFlagManager;

inline void CountryFlag::reset()
{
	this->t_id = 0;
	this->t_unlock_condi = 0;
};

inline void CountryFlag::fill( const CountryFlagBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_unlock_condi = base.unlock_condi;
};

inline const std::string CountryFlag::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "CountryFlag.tbls64";
#else
	return "CountryFlag.tbls32";
#endif
};

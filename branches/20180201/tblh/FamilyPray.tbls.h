#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:FamilyPrayBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:j�����.xlsx family_pray
///           ��������:�������
#pragma pack(1)
struct FamilyPrayBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ������� ����Ψһ��

	uint32_t                  id;                           ///< �������

	const char*               drop;                         ///< ��������id
};
#pragma pack()

struct FamilyPray : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t, uint32_t>& drop()              { return t_drop; }

private:
	uint32_t                  t_id;                         ///< �������

	std::map<uint32_t, uint32_t> t_drop;					///< ��������id

public:
	void reset();
	void fill( const FamilyPrayBase& base );
	const char* getClassName() const { return "j�����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FamilyPray, FamilyPrayBase > FamilyPrayManager;

inline void FamilyPray::reset()
{
	this->t_id = 0;
	this->t_drop.clear();
};

inline void FamilyPray::fill( const FamilyPrayBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;

	if (base.drop != NULL)
	{
		common::Misc::Parse2SeperatorDataInfo(std::string(base.drop), this->t_drop, "$");
	}
};

inline const std::string FamilyPray::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FamilyPray.tbls64";
#else
	return "FamilyPray.tbls32";
#endif
};

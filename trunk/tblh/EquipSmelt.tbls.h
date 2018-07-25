#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:EquipSmeltBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:8 X64
///           �ֶ�����:2
///           �����ļ�:zװ����.xlsx װ��������
///           ��������:�������ID
#pragma pack(1)
struct EquipSmeltBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �������ID ����Ψһ��

	uint32_t                  id;                           ///< �������ID

	uint32_t                  drop_id;                      ///< ��������id
};
#pragma pack()

struct EquipSmelt : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_id()                     { return t_drop_id; }

private:
	uint32_t                  t_id;                         ///< �������ID

	uint32_t                  t_drop_id;                    ///< ��������id

public:
	void reset();
	void fill( const EquipSmeltBase& base );
	const char* getClassName() const { return "zװ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipSmelt, EquipSmeltBase > EquipSmeltManager;

inline void EquipSmelt::reset()
{
	this->t_id = 0;
	this->t_drop_id = 0;
};

inline void EquipSmelt::fill( const EquipSmeltBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_id = base.drop_id;
};

inline const std::string EquipSmelt::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipSmelt.tbls64";
#else
	return "EquipSmelt.tbls32";
#endif
};

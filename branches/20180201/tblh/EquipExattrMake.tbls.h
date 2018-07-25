#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:EquipExattrMakeBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:8 X64
///           �ֶ�����:2
///           �����ļ�:zװ����.xlsx �д�ϴ��
///           ��������:װ��Ʒ��
#pragma pack(1)
struct EquipExattrMakeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� װ��Ʒ�� ����Ψһ��

	uint32_t                  id;                           ///< װ��Ʒ��

	uint32_t                  count;                        ///< �д�����
};
#pragma pack()

struct EquipExattrMake : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  count()                       { return t_count; }

private:
	uint32_t                  t_id;                         ///< װ��Ʒ��

	uint32_t                  t_count;                      ///< �д�����

public:
	void reset();
	void fill( const EquipExattrMakeBase& base );
	const char* getClassName() const { return "zװ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipExattrMake, EquipExattrMakeBase > EquipExattrMakeManager;

inline void EquipExattrMake::reset()
{
	this->t_id = 0;
	this->t_count = 0;
};

inline void EquipExattrMake::fill( const EquipExattrMakeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_count = base.count;
};

inline const std::string EquipExattrMake::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipExattrMake.tbls64";
#else
	return "EquipExattrMake.tbls32";
#endif
};

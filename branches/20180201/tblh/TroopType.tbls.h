#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TroopTypeBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:12 X64
///           �ֶ�����:3
///           �����ļ�:z��ӻ��.xlsx team_activities
///           ��������:��ӻ���
#pragma pack(1)
struct TroopTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return type; }; // TODO: ���� ��ӻ��� ����Ψһ��

	uint32_t                  type;                         ///< ��ӻ���

	uint32_t                  big_type;                     ///< ��ӻ���ͱ��
	uint32_t                  activity_id;                  ///< ��ӻ��Ӧ����
};
#pragma pack()

struct TroopType : public Entry
{
	uint32_t                  type()                        { return t_type; }

	uint32_t                  big_type()                    { return t_big_type; }
	uint32_t                  activity_id()                 { return t_activity_id; }

private:
	uint32_t                  t_type;                       ///< ��ӻ���

	uint32_t                  t_big_type;                   ///< ��ӻ���ͱ��
	uint32_t                  t_activity_id;                ///< ��ӻ��Ӧ����

public:
	void reset();
	void fill( const TroopTypeBase& base );
	const char* getClassName() const { return "z��ӻ��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TroopType, TroopTypeBase > TroopTypeManager;

inline void TroopType::reset()
{
	this->t_type = 0;
	this->t_big_type = 0;
	this->t_activity_id = 0;
};

inline void TroopType::fill( const TroopTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_type = base.type;
	this->t_big_type = base.big_type;
	this->t_activity_id = base.activity_id;
};

inline const std::string TroopType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TroopType.tbls64";
#else
	return "TroopType.tbls32";
#endif
};

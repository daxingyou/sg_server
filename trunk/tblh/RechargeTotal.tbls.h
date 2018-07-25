#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:RechargeTotalBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:16 X64
///           �ֶ�����:3
///           �����ļ�:y��Ӫ���.xlsx �۳����
///           ��������:��ֵ���
#pragma pack(1)
struct RechargeTotalBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510E6; ///< I|IV
#else
	static const int          FormatCheckCode = 0x00051106; ///< I|Iv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ��ֵ��� ����Ψһ��

	uint32_t                  id;                           ///< ��ֵ���

	uint32_t                  award;                        ///< ����ID
	const char*               display;                      ///< չʾID
};
#pragma pack()

struct RechargeTotal : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  award()                       { return t_award; }
	const std::string&        display()                     { return t_display; }

private:
	uint32_t                  t_id;                         ///< ��ֵ���

	uint32_t                  t_award;                      ///< ����ID
	std::string               t_display;                    ///< չʾID

public:
	void reset();
	void fill( const RechargeTotalBase& base );
	const char* getClassName() const { return "y��Ӫ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RechargeTotal, RechargeTotalBase > RechargeTotalManager;

inline void RechargeTotal::reset()
{
	this->t_id = 0;
	this->t_award = 0;
	this->t_display.clear();
};

inline void RechargeTotal::fill( const RechargeTotalBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_award = base.award;
	if( base.display == NULL ) this->t_display.clear(); else this->t_display.assign( base.display );
};

inline const std::string RechargeTotal::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RechargeTotal.tbls64";
#else
	return "RechargeTotal.tbls32";
#endif
};

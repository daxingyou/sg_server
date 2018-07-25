#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:FamilyWarPersonalBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:20 X64
///           �ֶ�����:4
///           �����ļ�:j����ս.xlsx ����ս����ս������
///           ��������:id
#pragma pack(1)
struct FamilyWarPersonalBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DE6; ///< I|IIV
#else
	static const int          FormatCheckCode = 0x00510E06; ///< I|IIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  level;                        ///< ս���ȼ�
	uint32_t                  exploit;                      ///< ս��ֵ
	const char*               reward;                       ///< ����
};
#pragma pack()

struct FamilyWarPersonal : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	uint32_t                  exploit()                     { return t_exploit; }
	const std::map<uint32_t, uint32_t>& reward()			{ return t_reward; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_level;                      ///< ս���ȼ�
	uint32_t                  t_exploit;                    ///< ս��ֵ
	std::map<uint32_t, uint32_t> t_reward;					///< ����

public:
	void reset();
	void fill( const FamilyWarPersonalBase& base );
	const char* getClassName() const { return "j����ս"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FamilyWarPersonal, FamilyWarPersonalBase > FamilyWarPersonalManager;

inline void FamilyWarPersonal::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_exploit = 0;
	this->t_reward.clear();
};

inline void FamilyWarPersonal::fill( const FamilyWarPersonalBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_exploit = base.exploit;
	
	if (base.reward != NULL) {
		common::Misc::Parse2SeperatorDataInfo(std::string(base.reward), this->t_reward, "$");
	}
};

inline const std::string FamilyWarPersonal::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FamilyWarPersonal.tbls64";
#else
	return "FamilyWarPersonal.tbls32";
#endif
};

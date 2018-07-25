#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:FamilyBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:28 X64
///           �ֶ�����:7
///           �����ļ�:j�����.xlsx family_build
///           ��������:ID
#pragma pack(1)
struct FamilyBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x02FFF8E9; ///< i|iiiiii
#else
	static const int          FormatCheckCode = 0x02FFF8E9; ///< i|iiiiii
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ID ����Ψһ��

	int32_t                   id;                           ///< id

	int32_t                   type;                         ///< ��������
	int32_t                   level;                        ///< �����ȼ�
	int32_t                   require;                      ///< �����ʽ�Ҫ��
	int32_t                   cost;                         ///< �����ʽ�����
	int32_t                   cd_time;                      ///< ������ȴʱ��/��
	int32_t                   cd_clean_cost;                ///< �����ȴ�ʽ�����
};
#pragma pack()

struct Family : public Entry
{
	int32_t                   id()                          { return t_id; }

	int32_t                   type()                        { return t_type; }
	int32_t                   level()                       { return t_level; }
	int32_t                   require()                     { return t_require; }
	int32_t                   cost()                        { return t_cost; }
	int32_t                   cd_time()                     { return t_cd_time; }
	int32_t                   cd_clean_cost()               { return t_cd_clean_cost; }

private:
	int32_t                   t_id;                         ///< id

	int32_t                   t_type;                       ///< ��������
	int32_t                   t_level;                      ///< �����ȼ�
	int32_t                   t_require;                    ///< �����ʽ�Ҫ��
	int32_t                   t_cost;                       ///< �����ʽ�����
	int32_t                   t_cd_time;                    ///< ������ȴʱ��/��
	int32_t                   t_cd_clean_cost;              ///< �����ȴ�ʽ�����

public:
	void reset();
	void fill( const FamilyBase& base );
	const char* getClassName() const { return "j�����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Family, FamilyBase > FamilyManager;

inline void Family::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_level = 0;
	this->t_require = 0;
	this->t_cost = 0;
	this->t_cd_time = 0;
	this->t_cd_clean_cost = 0;
};

inline void Family::fill( const FamilyBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_level = base.level;
	this->t_require = base.require;
	this->t_cost = base.cost;
	this->t_cd_time = base.cd_time;
	this->t_cd_clean_cost = base.cd_clean_cost;
};

inline const std::string Family::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Family.tbls64";
#else
	return "Family.tbls32";
#endif
};

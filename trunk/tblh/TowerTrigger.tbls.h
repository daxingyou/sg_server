#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TowerTriggerBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:24 X64
///           �ֶ�����:5
///           �����ļ�:sʱ�ջþ�.xlsx ���ز�
///           ��������:���ز�id
#pragma pack(1)
struct TowerTriggerBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511ADD9; ///< I|VIII
#else
	static const int          FormatCheckCode = 0x0513ADD9; ///< I|vIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ���ز�id ����Ψһ��

	uint32_t                  id;                           ///< ���ز�id

	const char*               name;                         ///< ˽���йؿ���
	uint32_t                  alive_time;                   ///< ����ʱ��sec
	uint32_t                  against_id;                   ///< ����id
	uint32_t                  awards;                       ///< ����
};
#pragma pack()

struct TowerTrigger : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        name()                        { return t_name; }
	uint32_t                  alive_time()                  { return t_alive_time; }
	uint32_t                  against_id()                  { return t_against_id; }
	uint32_t                  awards()                      { return t_awards; }

private:
	uint32_t                  t_id;                         ///< ���ز�id

	std::string               t_name;                       ///< ˽���йؿ���
	uint32_t                  t_alive_time;                 ///< ����ʱ��sec
	uint32_t                  t_against_id;                 ///< ����id
	uint32_t                  t_awards;                     ///< ����

public:
	void reset();
	void fill( const TowerTriggerBase& base );
	const char* getClassName() const { return "sʱ�ջþ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TowerTrigger, TowerTriggerBase > TowerTriggerManager;

inline void TowerTrigger::reset()
{
	this->t_id = 0;
	this->t_name.clear();
	this->t_alive_time = 0;
	this->t_against_id = 0;
	this->t_awards = 0;
};

inline void TowerTrigger::fill( const TowerTriggerBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_alive_time = base.alive_time;
	this->t_against_id = base.against_id;
	this->t_awards = base.awards;
};

inline const std::string TowerTrigger::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TowerTrigger.tbls64";
#else
	return "TowerTrigger.tbls32";
#endif
};

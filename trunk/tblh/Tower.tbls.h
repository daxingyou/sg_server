#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:TowerBase
/// @remark   ��¼��С:32 X32
///           ��¼��С:36 X64
///           �ֶ�����:8
///           �����ļ�:sʱ�ջþ�.xlsx ʱ�ջþ�������
///           ��������:����
#pragma pack(1)
struct TowerBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0DD8CD36; ///< I|IIIIIIIV
#else
	static const int          FormatCheckCode = 0x0DD8CED6; ///< I|IIIIIIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ���� ����Ψһ��

	uint32_t                  id;                           ///< ����

	uint32_t                  against_id;                   ///< ����id
	uint32_t                  herosoul;                     ///< ÿ�㽱������
	uint32_t                  awards;                       ///< ����
	uint32_t                  first_herosoul;               ///< ��ͨ��������
	uint32_t                  first_awards;                 ///< ��ͨ����
	uint32_t                  sweep_time;                   ///< ɨ��ʱ��
	uint32_t                  trigger_rate;                 ///< �������ز����
	const char*               trigger;                      ///< �������ز�ID�Ͷ�Ӧ�ĸ��ʱ�
};
#pragma pack()

struct Tower : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  against_id()                  { return t_against_id; }
	uint32_t                  herosoul()                    { return t_herosoul; }
	uint32_t                  awards()                      { return t_awards; }
	uint32_t                  first_herosoul()              { return t_first_herosoul; }
	uint32_t                  first_awards()                { return t_first_awards; }
	uint32_t                  sweep_time()                  { return t_sweep_time; }
	uint32_t                  trigger_rate() { return t_trigger_rate; }
	const std::map<uint32_t,uint32_t>&        trigger()                     { return t_trigger; }

private:
	uint32_t                  t_id;                         ///< ����

	uint32_t                  t_against_id;                 ///< ����id
	uint32_t                  t_herosoul;                   ///< ÿ�㽱������
	uint32_t                  t_awards;                     ///< ����
	uint32_t                  t_first_herosoul;             ///< ��ͨ��������
	uint32_t                  t_first_awards;               ///< ��ͨ����
	uint32_t                  t_sweep_time;                 ///< ɨ��ʱ��
	uint32_t                  t_trigger_rate;               ///< �������ز����
	std::map<uint32_t, uint32_t>               t_trigger;                    ///< �������ز�ID�Ͷ�Ӧ�ĸ��ʱ�

public:
	void reset();
	void fill( const TowerBase& base );
	const char* getClassName() const { return "sʱ�ջþ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Tower, TowerBase > TowerManager;

inline void Tower::reset()
{
	this->t_id = 0;
	this->t_against_id = 0;
	this->t_herosoul = 0;
	this->t_awards = 0;
	this->t_first_herosoul = 0;
	this->t_first_awards = 0;
	this->t_sweep_time = 0;
	this->t_trigger_rate = 0;
	this->t_trigger.clear();
};

inline void Tower::fill( const TowerBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_against_id = base.against_id;
	this->t_herosoul = base.herosoul;
	this->t_awards = base.awards;
	this->t_first_herosoul = base.first_herosoul;
	this->t_first_awards = base.first_awards;
	this->t_sweep_time = base.sweep_time;
	this->t_trigger_rate = base.trigger_rate;
	//if( base.trigger == NULL ) this->t_trigger.clear(); else this->t_trigger.assign( base.trigger );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_trigger, base.trigger, ":$");
};

inline const std::string Tower::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Tower.tbls64";
#else
	return "Tower.tbls32";
#endif
};

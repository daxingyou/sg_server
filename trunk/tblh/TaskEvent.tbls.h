#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TaskEventBase
/// @remark   ��¼��С:24 X32
///           ��¼��С:28 X64
///           �ֶ�����:6
///           �����ļ�:r�����.xlsx �����¼�
///           ��������:�¼�ID
#pragma pack(1)
struct TaskEventBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DEA89; ///< I|IIVII
#else
	static const int          FormatCheckCode = 0x010E0A89; ///< I|IIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �¼�ID ����Ψһ��

	uint32_t                  id;                           ///< �¼�ID

	uint32_t                  buff_id;                      ///< ����ID
	uint32_t                  hero_id;                      ///< �佫ID
	const char*               money;                        ///< ����۸�
	uint32_t                  time;                         ///< ʱ������
	uint32_t                  rate;                         ///< ���ָ���
};
#pragma pack()

struct TaskEvent : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  buff_id()                     { return t_buff_id; }
	uint32_t                  hero_id()                     { return t_hero_id; }
	const std::pair<uint32_t,uint32_t>&        money()                       { return t_money; }
	uint32_t                  time()                        { return t_time; }
	uint32_t                  rate()                        { return t_rate; }

private:
	uint32_t                  t_id;                         ///< �¼�ID

	uint32_t                  t_buff_id;                    ///< ����ID
	uint32_t                  t_hero_id;                    ///< �佫ID
	std::pair<uint32_t, uint32_t>               t_money;                      ///< ����۸�
	uint32_t                  t_time;                       ///< ʱ������
	uint32_t                  t_rate;                       ///< ���ָ���

public:
	void reset();
	void fill( const TaskEventBase& base );
	const char* getClassName() const { return "r�����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TaskEvent, TaskEventBase > TaskEventManager;

inline void TaskEvent::reset()
{
	this->t_id = 0;
	this->t_buff_id = 0;
	this->t_hero_id = 0;
	//this->t_money.clear();
	this->t_time = 0;
	this->t_rate = 0;
};

inline void TaskEvent::fill( const TaskEventBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_buff_id = base.buff_id;
	this->t_hero_id = base.hero_id;
	//if( base.money == NULL ) this->t_money.clear(); else this->t_money.assign( base.money );
	Dawn::parseTuple(this->t_money, base.money, ":");
	this->t_time = base.time;
	this->t_rate = base.rate;
};

inline const std::string TaskEvent::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TaskEvent.tbls64";
#else
	return "TaskEvent.tbls32";
#endif
};

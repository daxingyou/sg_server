#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:ActivitiesBase
/// @remark   ��¼��С:40 X32
///           ��¼��С:44 X64
///           �ֶ�����:10
///           �����ļ�:h���.xlsx activities
///           ��������:�id
#pragma pack(1)
struct ActivitiesBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x08CE7009; ///< I|IIIIIIVIII
#else
	static const int          FormatCheckCode = 0x08D07009; ///< I|IIIIIIvIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �id ����Ψһ��

	uint32_t                  id;                           ///< �id

	uint32_t                  type;                         ///< �����
	uint32_t                  numbers;                      ///< �����
	uint32_t                  times;                        ///< �������
	uint32_t                  reset_type;                   ///< ������������
	uint32_t                  level;                        ///< ����Ƶȼ�
	uint32_t                  liveness;                     ///< ��Ծֵ����
	const char*               time_id;                      ///< �ʱ��
	uint32_t                  page;                         ///< �������ҳ
	uint32_t                  quick_cost;                   ///< һ���������
	uint32_t                  task_type;                    ///< ��������
};
#pragma pack()

struct Activities : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  numbers()                     { return t_numbers; }
	uint32_t                  times()                       { return t_times; }
	uint32_t                  reset_type()                  { return t_reset_type; }
	uint32_t                  level()                       { return t_level; }
	uint32_t                  liveness()                    { return t_liveness; }
	const std::vector<uint32_t>&        time_id()                     { return t_time_id; }
	uint32_t                  page()                        { return t_page; }
	uint32_t                  quick_cost()                  { return t_quick_cost; }
	uint32_t                  task_type()                   { return t_task_type; }

private:
	uint32_t                  t_id;                         ///< �id

	uint32_t                  t_type;                       ///< �����
	uint32_t                  t_numbers;                    ///< �����
	uint32_t                  t_times;                      ///< �������
	uint32_t                  t_reset_type;                 ///< ������������
	uint32_t                  t_level;                      ///< ����Ƶȼ�
	uint32_t                  t_liveness;                   ///< ��Ծֵ����
	std::vector<uint32_t>               t_time_id;                    ///< �ʱ��
	uint32_t                  t_page;                       ///< �������ҳ
	uint32_t                  t_quick_cost;                 ///< һ���������
	uint32_t                  t_task_type;                  ///< ��������

public:
	void reset();
	void fill( const ActivitiesBase& base );
	const char* getClassName() const { return "h���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Activities, ActivitiesBase > ActivitiesManager;

inline void Activities::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_numbers = 0;
	this->t_times = 0;
	this->t_reset_type = 0;
	this->t_level = 0;
	this->t_liveness = 0;
	this->t_time_id.clear();
	this->t_page = 0;
	this->t_quick_cost = 0;
	this->t_task_type = 0;
};

inline void Activities::fill( const ActivitiesBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_numbers = base.numbers;
	this->t_times = base.times;
	this->t_reset_type = base.reset_type;
	this->t_level = base.level;
	this->t_liveness = base.liveness;
	//if( base.time_id == NULL ) this->t_time_id.clear(); else this->t_time_id.assign( base.time_id );
	Dawn::parseContainer<uint32_t>(this->t_time_id, base.time_id, "$");
	this->t_page = base.page;
	this->t_quick_cost = base.quick_cost;
	this->t_task_type = base.task_type;
};

inline const std::string Activities::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Activities.tbls64";
#else
	return "Activities.tbls32";
#endif
};

#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/utility.hpp"
USING_NS_COMMON;

/// @brief    ���ṹ:TaskCircleBase
/// @remark   ��¼��С:24 X32
///           ��¼��С:32 X64
///           �ֶ�����:6
///           �����ļ�:r���񻷱�.xlsx Sheet1
///           ��������:������ID
#pragma pack(1)
struct TaskCircleBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0EBABDA9; ///< I|IVVIVfI
#else
	static const int          FormatCheckCode = 0x00DB5DB9; ///< I|IvvIvfI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ������ID ����Ψһ��

	uint32_t                  id;                           ///< ������ID

	uint32_t                  type;                         ///< ��������
	const char*               level_range;                  ///< �ȼ�ɸѡ
	const char*               circle_range;                 ///< ��������
	uint32_t                  task_id_style;                ///< ����ID��ʽ
	const char*               task_id_range;                ///< ����ID����
	float                     exp_coeff;                    ///< �������齱��ϵ��
	uint32_t                  award;                        ///< �ֶν���
};
#pragma pack()

struct TaskCircle : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	const std::vector<uint32_t>&        level_range() { return t_level_range; }
	const std::vector<uint32_t>&        circle_range()                { return t_circle_range; }
	uint32_t                  task_id_style()               { return t_task_id_style; }
	const std::vector<uint32_t>&        task_id_range()               { return t_task_id_range; }
	float                     exp_coeff()                   { return t_exp_coeff; }
	uint32_t                  award() { return t_award; }

private:
	uint32_t                  t_id;                         ///< ������ID

	uint32_t                  t_type;                       ///< ��������
	std::vector<uint32_t>     t_level_range;				///< �ȼ�ɸѡ
	std::vector<uint32_t>     t_circle_range;               ///< ��������
	uint32_t                  t_task_id_style;              ///< ����ID��ʽ
	std::vector<uint32_t>     t_task_id_range;              ///< ����ID����
	float                     t_exp_coeff;                  ///< �������齱��ϵ��
	uint32_t                  t_award;                      ///< �ֶν���

public:
	void reset();
	void fill( const TaskCircleBase& base );
	const char* getClassName() const { return "r���񻷱�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TaskCircle, TaskCircleBase > TaskCircleManager;

inline void TaskCircle::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_level_range.clear();
	this->t_circle_range.clear();
	this->t_task_id_style = 0;
	this->t_task_id_range.clear();
	this->t_exp_coeff = 0;
	this->t_award = 0;
};

inline void TaskCircle::fill( const TaskCircleBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	//if (base.level_range == NULL) this->t_level_range.clear(); else this->t_level_range.assign(base.level_range);
	if (base.level_range != NULL)
	{
		string_util_t::split<uint32_t>(base.level_range, t_level_range, "~");
	}
	//if( base.circle_range == NULL ) this->t_circle_range.clear(); else this->t_circle_range.assign( base.circle_range );
	if (base.circle_range != NULL)
	{
		string_util_t::split<uint32_t>(base.circle_range, t_circle_range, "$");
	}
	this->t_task_id_style = base.task_id_style;
	//if( base.task_id_range == NULL ) this->t_task_id_range.clear(); else this->t_task_id_range.assign( base.task_id_range );
	if (base.task_id_range != NULL)
	{
		string_util_t::split<uint32_t>(base.task_id_range, t_task_id_range, "$");
	}
	this->t_exp_coeff = base.exp_coeff;
	this->t_award = base.award;
};

inline const std::string TaskCircle::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TaskCircle.tbls64";
#else
	return "TaskCircle.tbls32";
#endif
};

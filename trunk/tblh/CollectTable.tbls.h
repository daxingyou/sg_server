#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:CollectTableBase
/// @remark   ��¼��С:40 X32
///           ��¼��С:44 X64
///           �ֶ�����:10
///           �����ļ�:c�ɼ���.xlsx Sheet1
///           ��������:ID
#pragma pack(1)
struct CollectTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0EFCD009; ///< I|IIIVIIIII
#else
	static const int          FormatCheckCode = 0x00FCD039; ///< I|IIIvIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ID ����Ψһ��

	uint32_t                  id;                           ///< ID

	uint32_t                  type;                         ///< ����
	uint32_t                  collect_time;                 ///< ����ʱ��
	uint32_t                  collect_cond;                 ///< �ɼ�����
	const char*               collect_cond_param;           ///< ��������
	uint32_t                  cost_energy;                  ///< ���Ļ���ֵ
	uint32_t                  drop_bag_id;                  ///< �����ID
	uint32_t                  collect_times_max;            ///< �ɼ���������
	uint32_t                  refresh_time;                 ///< ˢ��ʱ��
	uint32_t                  is_share;                     ///< ����ɼ�
};
#pragma pack()

struct CollectTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  collect_time()                { return t_collect_time; }
	uint32_t                  collect_cond()                { return t_collect_cond; }
	const std::string&        collect_cond_param()          { return t_collect_cond_param; }
	uint32_t                  cost_energy()                 { return t_cost_energy; }
	uint32_t                  drop_bag_id()                 { return t_drop_bag_id; }
	uint32_t                  collect_times_max()           { return t_collect_times_max; }
	uint32_t                  refresh_time()                { return t_refresh_time; }
	uint32_t                  is_share()                    { return t_is_share; }

	uint32_t				  collect_cond_param_size() { return t_collect_cond_param_vec.size(); }
	uint32_t				  collect_cond_param(uint32_t pos) { return pos >= collect_cond_param_size() ? 0 : t_collect_cond_param_vec[pos]; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_type;                       ///< ����
	uint32_t                  t_collect_time;               ///< ����ʱ��
	uint32_t                  t_collect_cond;               ///< �ɼ�����
	std::string               t_collect_cond_param;         ///< ��������
	uint32_t                  t_cost_energy;                ///< ���Ļ���ֵ
	uint32_t                  t_drop_bag_id;                ///< �����ID
	uint32_t                  t_collect_times_max;          ///< �ɼ���������
	uint32_t                  t_refresh_time;               ///< ˢ��ʱ��
	uint32_t                  t_is_share;                   ///< ����ɼ�
	
	std::vector<uint32_t>	  t_collect_cond_param_vec;

public:
	void reset();
	void fill( const CollectTableBase& base );
	const char* getClassName() const { return "c�ɼ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< CollectTable, CollectTableBase > CollectTableManager;

inline void CollectTable::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_collect_time = 0;
	this->t_collect_cond = 0;
	this->t_collect_cond_param.clear();
	this->t_cost_energy = 0;
	this->t_drop_bag_id = 0;
	this->t_collect_times_max = 0;
	this->t_refresh_time = 0;
	this->t_is_share = 0;

	this->t_collect_cond_param_vec.clear();
};

inline void CollectTable::fill( const CollectTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_collect_time = base.collect_time;
	this->t_collect_cond = base.collect_cond;
	if( base.collect_cond_param == NULL ) this->t_collect_cond_param.clear(); else this->t_collect_cond_param.assign( base.collect_cond_param );
	this->t_cost_energy = base.cost_energy;
	this->t_drop_bag_id = base.drop_bag_id;
	this->t_collect_times_max = base.collect_times_max;
	this->t_refresh_time = base.refresh_time;
	this->t_is_share = base.is_share;

	if (base.collect_cond_param != NULL)
		string_util_t::split<uint32_t>(std::string(base.collect_cond_param), this->t_collect_cond_param_vec, "$");
};

inline const std::string CollectTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "CollectTable.tbls64";
#else
	return "CollectTable.tbls32";
#endif
};

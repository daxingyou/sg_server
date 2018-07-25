#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
#include <set>
/// @brief    ���ṹ:ActiveRewardBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:28 X64
///           �ֶ�����:5
///           �����ļ�:h���.xlsx ��Ծ�б�
///           ��������:ID
#pragma pack(1)
struct ActiveRewardBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511AEA9; ///< I|VIVI
#else
	static const int          FormatCheckCode = 0x0513B0A9; ///< I|vIvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return ID; }; // TODO: ���� ID ����Ψһ��

	uint32_t                  ID;                           ///< ID

	const char*               condition;                    ///< �������
	uint32_t                  drop_id;                      ///< ��ɽ���
	const char*               level_range;                  ///< �ȼ���
	uint32_t                  liveness;                     ///< ��Ծֵ
};
#pragma pack()

struct ActiveReward : public Entry
{
	uint32_t                  ID()                          { return t_ID; }

	const Dawn::triple< uint32_t, uint32_t, uint32_t>&        condition()                   { return t_condition; }
	uint32_t                  drop_id()                     { return t_drop_id; }
	const std::string&        level_range()                 { return t_level_range; }
	uint32_t                  liveness()                    { return t_liveness; }
	const std::set<uint32_t>&		  level_range_set()				{ return t_level_range_set; }
private:
	uint32_t                  t_ID;                         ///< ID

	Dawn::triple< uint32_t, uint32_t, uint32_t>               t_condition;                  ///< �������
	uint32_t                  t_drop_id;                    ///< ��ɽ���
	std::string               t_level_range;                ///< �ȼ���
	uint32_t                  t_liveness;                   ///< ��Ծֵ
	std::set<uint32_t>		  t_level_range_set;			///< �ȼ����б�

public:
	void reset();
	void fill( const ActiveRewardBase& base );
	const char* getClassName() const { return "h���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ActiveReward, ActiveRewardBase > ActiveRewardManager;

inline void ActiveReward::reset()
{
	this->t_ID = 0;
	this->t_condition.clear();
	this->t_drop_id = 0;
	this->t_level_range.clear();
	this->t_liveness = 0;
	this->t_level_range_set.clear();
};

inline void ActiveReward::fill( const ActiveRewardBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_ID = base.ID;
	//if( base.condition == NULL ) this->t_condition.clear(); else this->t_condition.assign( base.condition );
	Dawn::parseTuple(this->t_condition, base.condition, ":");
	this->t_drop_id = base.drop_id;
	if( base.level_range == NULL ) this->t_level_range.clear(); else this->t_level_range.assign( base.level_range );
	this->t_liveness = base.liveness;
	if (NULL != base.level_range)
	{
		std::vector<uint32_t> vec;
		common::string_util_t::split<uint32_t>(std::string(base.level_range), vec, "$");
		for (auto& it : vec)
		{
			t_level_range_set.insert(it);
		}
	}
};

inline const std::string ActiveReward::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ActiveReward.tbls64";
#else
	return "ActiveReward.tbls32";
#endif
};

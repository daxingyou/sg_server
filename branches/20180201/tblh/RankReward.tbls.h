#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"

/// @brief    ���ṹ:RankRewardBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:48 X64
///           �ֶ�����:7
///           �����ļ�:p���а�.xlsx ���а���
///           ��������:id
#pragma pack(1)
struct RankRewardBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00EBBEA6; ///< I|IVVVVV
#else
	static const int          FormatCheckCode = 0x010DD8C6; ///< I|Ivvvvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  rank_type;                    ///< ���а�����
	const char*               rank_range;                   ///< ����
	const char*               item;                         ///< ���߽���
	const char*               day_reward;                   ///< ÿ�ս���
	const char*               week_reward;                  ///< ÿ�ܽ���
	const char*               month_reward;                 ///< ÿ�½���
};
#pragma pack()

struct RankReward : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  rank_type()                   { return t_rank_type; }
	const std::pair< uint32_t, uint32_t >&        rank_range()                  { return t_rank_range; }
	const std::vector< std::pair< uint32_t, uint32_t > >&        item()                        { return t_item; }
	const std::vector< std::pair< uint32_t, uint32_t > >&        day_reward()                  { return t_day_reward; }
	const std::vector< std::pair< uint32_t, uint32_t > >&        week_reward()                 { return t_week_reward; }
	const std::vector< std::pair< uint32_t, uint32_t > >&        month_reward()                { return t_month_reward; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_rank_type;                  ///< ���а�����
	std::pair< uint32_t, uint32_t >               t_rank_range;                 ///< ����
	std::vector< std::pair< uint32_t, uint32_t > >               t_item;                       ///< ���߽���
	std::vector< std::pair< uint32_t, uint32_t > >               t_day_reward;                 ///< ÿ�ս���
	std::vector< std::pair< uint32_t, uint32_t > >               t_week_reward;                ///< ÿ�ܽ���
	std::vector< std::pair< uint32_t, uint32_t > >               t_month_reward;               ///< ÿ�½���

public:
	void reset();
	void fill( const RankRewardBase& base );
	const char* getClassName() const { return "p���а�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RankReward, RankRewardBase > RankRewardManager;

inline void RankReward::reset()
{
	this->t_id = 0;
	this->t_rank_type = 0;
	this->t_rank_range = std::pair< uint32_t, uint32_t >();
	this->t_item.clear();
	this->t_day_reward.clear();
	this->t_week_reward.clear();
	this->t_month_reward.clear();
};

inline void RankReward::fill( const RankRewardBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_rank_type = base.rank_type;
	//if( base.rank_range == NULL ) this->t_rank_range.clear(); else this->t_rank_range.assign( base.rank_range );
	Dawn::parseTuple(this->t_rank_range, base.rank_range);
	//if( base.item == NULL ) this->t_item.clear(); else this->t_item.assign( base.item );
	Dawn::parseTupleContainer< std::pair< uint32_t, uint32_t > >(this->t_item, base.item, "-;");
	//if( base.day_reward == NULL ) this->t_day_reward.clear(); else this->t_day_reward.assign( base.day_reward );
	Dawn::parseTupleContainer< std::pair< uint32_t, uint32_t > >(this->t_day_reward, base.day_reward, "-;");
	//if( base.week_reward == NULL ) this->t_week_reward.clear(); else this->t_week_reward.assign( base.week_reward );
	Dawn::parseTupleContainer< std::pair< uint32_t, uint32_t > >(this->t_week_reward, base.week_reward, "-;");
	//if( base.month_reward == NULL ) this->t_month_reward.clear(); else this->t_month_reward.assign( base.month_reward );
	Dawn::parseTupleContainer< std::pair< uint32_t, uint32_t > >(this->t_month_reward, base.month_reward, "-;");
};

inline const std::string RankReward::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RankReward.tbls64";
#else
	return "RankReward.tbls32";
#endif
};

struct RankRewardExec : public execEntry<RankReward>
{
	RankReward* result;
	uint32_t type;
	uint32_t index;
	uint32_t param;
	RankRewardExec(uint32_t _type, uint32_t _index, uint32_t _param) : result(NULL), type(_type), index(_index), param(_param) {};
	bool exec(RankReward *entry)
	{
		if (entry->rank_type() == type
			&& entry->rank_range().first <= index
			&& entry->rank_range().second >= index)
		{
			result = entry;
			return false;
		}

		return true;
	}
};

inline RankReward* get_rank_reward_entry(uint32_t type, uint32_t index, uint32_t param)
{
	if (NULL == RankRewardManager::getInstancePtr())
	{
		log_error("NULL == RankRewardManager::getInstancePtr()");
		return NULL;
	}
	RankRewardExec exec(type, index, param);
	RankRewardManager::getInstancePtr()->execEveryData(exec);

	return exec.result;
}
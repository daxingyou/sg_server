#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/utility.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"
USING_NS_COMMON;
/// @brief    表格结构:TaskBase
/// @remark   记录大小:79 X32
///           记录大小:111 X64
///           字段数量:22
///           数据文件:r任务表.xlsx 任务
///           排序主键:任务ID
#pragma pack(1)
struct TaskBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x057B4379; ///< I|VVIVIIVVBBIBIVIIVIIII
#else
	static const int          FormatCheckCode = 0x0C9ABD69; ///< I|vvIvIIvvBBIBIvIIvIIII
#endif

	//------------------------
	uint32_t getUniqueID() const // TODO: 请用 任务ID 生成唯一键
	{
		return id;
	}

	uint32_t                  id;                           ///< 任务ID

	const char*               name;                         ///< 任务名
	const char*               pre_task;                     ///< 接受任务的前置任务ID
	uint32_t                  level;                        ///< 接受任务等级
	const char*               accept_cond;                  ///< 接受任务条件
	uint32_t                  type;                         ///< 任务类型
	uint32_t                  accept_npc;                   ///< 接受任务npcID
	const char*               access_item;                  ///< 接受任务给予的道具ID
	const char*               complete_cond;                ///< 完成条件
	uint8_t                   auto_complete;                ///< 是否自动提交
	uint8_t                   drop;                         ///< 是否可以放弃
	uint32_t                  pay_npc;                      ///< 跳转任务npcID
	uint8_t                   auto_go;                      ///< 是否自动执行
	uint32_t                  exp;                          ///< 奖励经验
	const char*               coin;                         ///< 奖励金钱
	uint32_t                  hero;                         ///< 奖励英雄
	uint32_t                  itemtype;                     ///< 道具奖励方式
	const char*               item;                         ///< 奖励道具ID
	uint32_t                  family_gold;                  ///< 奖励家族资金
	uint32_t                  country_gold;                 ///< 奖励国家资金
	uint32_t                  buff;                         ///< 增益奖励
	uint32_t                  mail;                         ///< 邮件发放
};
#pragma pack()


typedef std::tuple<uint32_t, uint32_t, uint32_t, int, float, float, uint32_t> task_target;
struct Task : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        name()                        { return t_name; }
	const std::vector<uint32_t>&        pre_task()          { return t_pre_task; }
	uint32_t                  level()                       { return t_level; }
	const std::vector<Dawn::triple<uint32_t,uint32_t,uint32_t>>&        accept_cond() { return t_accept_cond; }
	uint32_t                  type()                        { return t_type; }
	uint32_t                  accept_npc()                  { return t_accept_npc; }
	const std::pair<uint32_t, uint32_t>&        access_item(){ return access_id_cnt; }
	const std::vector<task_target>&        complete_cond()  { return target_type_id_cnt; }
	uint8_t                   auto_complete()               { return t_auto_complete; }
	uint8_t                   drop()                        { return t_drop; }
	uint32_t                  pay_npc()                     { return t_pay_npc; }
	uint8_t                   auto_go()						{ return t_auto_go; }
	uint32_t                  exp()                         { return t_exp; }
	const std::map<uint32_t, uint32_t>&        coin()       { return t_coin; }
	uint32_t                  hero() { return t_hero; }
	uint32_t                  itemtype()                    { return t_itemtype; }
	const std::vector<std::pair<uint32_t, uint32_t>>&        item()       { return reward_id_cnt; }
	uint32_t                  family_gold()                 { return t_family_gold; }
	uint32_t                  country_gold()                { return t_country_gold; }
	uint32_t                  buff()                        { return t_buff; }
	uint32_t                  mail()                        { return t_mail; }

private:
	uint32_t                  t_id;                         ///< 任务ID

	std::string               t_name;                       ///< 任务名
	std::vector<uint32_t>     t_pre_task;                   ///< 接受任务的前置任务ID
	uint32_t                  t_level;                      ///< 接受任务等级
	std::vector<Dawn::triple<uint32_t, uint32_t, uint32_t>> t_accept_cond; ///< 接受任务条件
	uint32_t                  t_type;                       ///< 任务类型
	uint32_t                  t_accept_npc;                 ///< 接受任务npcID
	//std::string               t_access_item;                ///< 接受任务给予的道具ID
	//std::string               t_complete_cond;              ///< 完成条件
	uint8_t                   t_auto_complete;              ///< 是否自动提交
	uint8_t                   t_drop;                       ///< 是否可以放弃
	uint32_t                  t_pay_npc;                    ///< 跳转任务npcID
	uint8_t                   t_auto_go;                    ///< 是否自动执行
	uint32_t                  t_exp;                        ///< 奖励经验
	std::map<uint32_t, uint32_t>             t_coin;        ///< 奖励金钱
	uint32_t                  t_hero;                       ///< 奖励英雄
	uint32_t                  t_itemtype;                   ///< 道具奖励方式
	//std::string               t_item;                       ///< 奖励道具ID
	uint32_t                  t_family_gold;                ///< 奖励家族资金
	uint32_t                  t_country_gold;               ///< 奖励国家资金
	uint32_t                  t_buff;                       ///< 增益奖励
	uint32_t                  t_mail;                       ///< 邮件发放

	std::pair<uint32_t, uint32_t> access_id_cnt;
	std::vector<task_target> target_type_id_cnt;
	std::vector<std::pair<uint32_t, uint32_t>> reward_id_cnt; 

	bool t_is_ordered;

public:
	void reset();
	void fill( const TaskBase& base );
	const char* getClassName() const { return "task"; }

	static const std::string getBinaryName();

public:
	
	bool have_target(uint32_t type, uint32_t targetId)
	{
		for (auto type_id_cnt : target_type_id_cnt)
		{
			if (std::get<0>(type_id_cnt) == type && (std::get<1>(type_id_cnt) == targetId || 0 == std::get<1>(type_id_cnt)))
			{
				return true;
			}
		}

		return false;
	}

	uint32_t get_target_cnt(uint32_t type, uint32_t targetId)
	{
		for (auto ptr : target_type_id_cnt)
		{
			if (std::get<0>(ptr) == type && std::get<1>(ptr) == targetId)
			{
				return std::get<2>(ptr);
			}
		}
		return 0;
	}

	uint32_t get_target_type()
	{
		if (!target_type_id_cnt.empty())
		{
			const task_target& tt = target_type_id_cnt[0];
			return std::get<0>(tt);
		}

		return 0;
	}

	task_target* get_target(uint32_t index)
	{
		if (index >= target_type_id_cnt.size())
		{
			return NULL;
		}

		return &(target_type_id_cnt[index]);
	}

	bool is_ordered()
	{
		return t_is_ordered;
	}
};


typedef DataTableManager< Task, TaskBase > TaskManager;

inline void Task::reset()
{
	this->t_id = 0;
	this->t_name.clear();
	this->t_pre_task.clear();
	this->t_level = 0;
	this->t_accept_cond.clear();
	this->t_type = 0;
	this->t_accept_npc = 0;
	//this->t_access_item.clear();
	//this->t_complete_cond.clear();
	this->t_auto_complete = 0;
	this->t_drop = 0;
	this->t_pay_npc = 0;
	this->t_auto_go = 0;
	this->t_exp = 0;
	this->t_coin.clear();
	this->t_hero = 0;
	this->t_itemtype = 0;
	//this->t_item.clear();
	this->t_family_gold = 0;
	this->t_country_gold = 0;
	target_type_id_cnt.clear();
	reward_id_cnt.clear();
	this->t_buff = 0;
	this->t_mail = 0;
};

inline void Task::fill( const TaskBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	//if( base.pre_task == NULL ) this->t_pre_task.clear(); else this->t_pre_task.assign( base.pre_task );
	if (base.pre_task != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.pre_task), t_pre_task, "$");
	}
	this->t_level = base.level;
	//if (base.accept_cond == NULL) this->t_accept_cond.clear(); else this->t_accept_cond.assign(base.accept_cond);
	Dawn::parseTupleContainer<Dawn::triple<uint32_t, uint32_t, uint32_t>>(this->t_accept_cond, base.accept_cond, ":$");
	this->t_type = base.type;
	this->t_accept_npc = base.accept_npc;
	//if( base.access_item == NULL ) this->t_access_item.clear(); else this->t_access_item.assign( base.access_item );
	//if( base.complete_cond == NULL ) this->t_complete_cond.clear(); else this->t_complete_cond.assign( base.complete_cond );
	this->t_auto_complete = base.auto_complete;
	this->t_drop = base.drop;
	this->t_pay_npc = base.pay_npc;
	this->t_auto_go = base.auto_go;
	this->t_exp = base.exp;
	//if( base.coin == NULL ) this->t_coin.clear(); else this->t_coin.assign( base.coin );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_coin, base.coin, ":$");
	this->t_hero = base.hero;
	this->t_itemtype = base.itemtype;
	//if( base.item == NULL ) this->t_item.clear(); else this->t_item.assign( base.item );
	this->t_family_gold = base.family_gold;
	this->t_country_gold = base.country_gold;
	Dawn::parseTuple(this->access_id_cnt, base.access_item, ":");
	Dawn::parseTupleContainer<std::pair<uint32_t, uint32_t>>(this->reward_id_cnt, base.item, ",;");
	std::vector<std::string> vec;
	vec.clear();
	string_util_t::split<std::string>(base.complete_cond, vec, "$");
	string_util_t::Parse_type_id_cnt_str_to_map(vec, target_type_id_cnt, &t_is_ordered);
	this->t_buff = base.buff;
	this->t_mail = base.mail;
};

inline const std::string Task::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Task.tbls64";
#else
	return "Task.tbls32";
#endif
};

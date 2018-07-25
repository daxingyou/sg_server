#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "tblh/DropTableConfig.hpp"

/// @brief    表格结构:DropTableBase
/// @remark   记录大小:36 X32
///           记录大小:36 X64
///           字段数量:9
///           数据文件:d掉落表.xlsx 道具包表
///           排序主键:id
#pragma pack(1)
struct DropTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0CD00059; ///< I|IIIIIIIIIII
#else
	static const int          FormatCheckCode = 0x0CD00059; ///< I|IIIIIIIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  drop_item_type;               ///< 物品类型
	uint32_t                  group_id;                     ///< 道具包组ID
	uint32_t                  drop_num;                     ///< 掉落组最小掉落数量
	uint32_t                  prob_type;                    ///< 概率类型
	uint32_t                  rand_type;                    ///< 返还类型
	uint32_t                  drop_item_tid;                ///< 掉落道具ID
	uint32_t                  drop_prob;                    ///< 掉落概率
	uint32_t                  drop_item_num;                ///< 道具数量
	uint32_t                  cond_type;                    ///< 限制条件类型
	uint32_t                  cond_param1;                  ///< 限制参数1
	uint32_t                  cond_param2;                  ///< 限制参数2
};
#pragma pack()

struct DropTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_item_type()				{ return t_drop_item_type; }
	uint32_t                  group_id()                    { return t_group_id; }
	uint32_t                  drop_num()                    { return t_drop_num; }
	uint32_t                  prob_type()                   { return t_prob_type; }
	uint32_t                  rand_type()                   { return t_rand_type; }
	uint32_t                  drop_item_tid()               { return t_drop_item_tid; }
	uint32_t                  drop_prob()                   { return t_drop_prob; }
	uint32_t                  drop_item_num()               { return t_drop_item_num; }
	uint32_t                  cond_type()                   { return t_cond_type; }
	uint32_t                  cond_param1()                 { return t_cond_param1; }
	uint32_t                  cond_param2()                 { return t_cond_param2; }

private:
	uint32_t                  t_id;                         ///< id
	uint32_t                  t_drop_item_type;             ///< 物品类型
	uint32_t                  t_group_id;                   ///< 道具包组ID
	uint32_t                  t_drop_num;                   ///< 掉落组最小掉落数量
	uint32_t                  t_prob_type;                  ///< 概率类型
	uint32_t                  t_rand_type;                  ///< 返还类型
	uint32_t                  t_drop_item_tid;              ///< 掉落道具ID
	uint32_t                  t_drop_prob;                  ///< 掉落概率
	uint32_t                  t_drop_item_num;              ///< 道具数量
	uint32_t                  t_cond_type;                  ///< 限制条件类型
	uint32_t                  t_cond_param1;                ///< 限制参数1
	uint32_t                  t_cond_param2;                ///< 限制参数2

public:
	void reset();
	void fill( const DropTableBase& base );
	const char* getClassName() const { return "d掉落表"; }

	static const std::string getBinaryName();
};


typedef DataTableManager< DropTable, DropTableBase > DropTableManager;

inline void DropTable::reset()
{
	this->t_id = 0;
	this->t_drop_item_type = 0;
	this->t_group_id = 0;
	this->t_drop_num = 0;
	this->t_prob_type = 0;
	this->t_rand_type = 0;
	this->t_drop_item_tid = 0;
	this->t_drop_prob = 0;
	this->t_drop_item_num = 0;
	this->t_cond_type = 0;
	this->t_cond_param1 = 0;
	this->t_cond_param2 = 0;
};

inline void DropTable::fill( const DropTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_item_type = base.drop_item_type;
	this->t_group_id = base.group_id;
	this->t_drop_num = base.drop_num;
	this->t_prob_type = base.prob_type;
	this->t_rand_type = base.rand_type;
	this->t_drop_item_tid = base.drop_item_tid;
	this->t_drop_prob = base.drop_prob;
	this->t_drop_item_num = base.drop_item_num;
	this->t_cond_type = base.cond_type;
	this->t_cond_param1 = base.cond_param1;
	this->t_cond_param2 = base.cond_param2;

	DropTableConfig_t::fill_drop_table(this->t_group_id, this->t_id);

};

inline const std::string DropTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "DropTable.tbls64";
#else
	return "DropTable.tbls32";
#endif
};

#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:GeneralEventTableBase
/// @remark   记录大小:24 X32
///           记录大小:32 X64
///           字段数量:6
///           数据文件:t天下大势.xlsx 大势
///           排序主键:id
#pragma pack(1)
struct GeneralEventTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0ABBD039; ///< I|IIVIVIIII
#else
	static const int          FormatCheckCode = 0x0B5BD3D9; ///< I|IIvIvIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  level;                        ///< 等级
	uint32_t                  end_open_time;                ///< 开启倒计时h
	const char*               speed_open_time;              ///< 缩短倒计时
	uint32_t                  is_open;                      ///< 是否开启
	const char*               target_id_list;               ///< 目标id
	uint32_t                  speed_target;                 ///< 检测目标
	uint32_t                  touch_notice;                 ///< 触发通知
	uint32_t                  open_notice;                  ///< 开启通知
	uint32_t                  card_time;                    ///< 卡包持续时间
};
#pragma pack()

struct GeneralEventTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	uint32_t                  end_open_time()               { return t_end_open_time; }
	const std::string&        speed_open_time()             { return t_speed_open_time; }
	uint32_t                  is_open()                     { return t_is_open; }
	const std::string&        target_id_list()              { return t_target_id_list; }
	
	const uint32_t			  get_add_speed_cond() { return t_speed_cond; }
	const uint32_t			  get_add_speed_time() { return t_speed_time; }
	uint32_t                  speed_target()                { return t_speed_target; }
	uint32_t                  touch_notice()                { return t_touch_notice; }
	uint32_t                  open_notice()                 { return t_open_notice; }
	uint32_t                  card_time() { return t_card_time; }

	const std::set<uint32_t> &get_target_list() { return t_target_list; }
private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_level;                      ///< 等级
	uint32_t                  t_end_open_time;              ///< 开启倒计时h
	std::string               t_speed_open_time;            ///< 缩短倒计时
	uint32_t                  t_is_open;                    ///< 是否开启
	std::string               t_target_id_list;             ///< 目标id
	uint32_t				  t_speed_cond;					//加速条件
	uint32_t				  t_speed_time;					//加速时间
	std::set<uint32_t>t_target_list;						//目标列表
	uint32_t                  t_speed_target;               ///< 检测目标
	uint32_t                  t_touch_notice;               ///< 触发通知
	uint32_t                  t_open_notice;                ///< 开启通知
	uint32_t                  t_card_time;                  ///< 卡包持续时间

public:
	void reset();
	void fill( const GeneralEventTableBase& base );
	const char* getClassName() const { return "t天下大势"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< GeneralEventTable, GeneralEventTableBase > GeneralEventTableManager;

inline void GeneralEventTable::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_end_open_time = 0;
	this->t_speed_open_time.clear();
	this->t_is_open = 0;
	this->t_target_id_list.clear();
	this->t_target_list.clear();
	this->t_speed_cond = 0;
	this->t_speed_time = 0;
	this->t_speed_target = 0;
	this->t_touch_notice = 0;
	this->t_open_notice = 0;
	this->t_card_time = 0;
};

inline void GeneralEventTable::fill( const GeneralEventTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_end_open_time = base.end_open_time;
	if( base.speed_open_time == NULL ) this->t_speed_open_time.clear(); else this->t_speed_open_time.assign( base.speed_open_time );
	this->t_is_open = base.is_open;
	if( base.target_id_list == NULL ) this->t_target_id_list.clear(); else this->t_target_id_list.assign( base.target_id_list );
	
	if (NULL != base.target_id_list)
	{
		std::vector<uint32_t> tmp_vec;
		common::string_util_t::split<uint32_t>(std::string(base.target_id_list), tmp_vec, ":");
		for (auto it : tmp_vec)
		{
			t_target_list.insert(it);
		}
	}
	if (NULL != base.speed_open_time)
	{
		std::vector<uint32_t> tmp_vec;
		common::string_util_t::split<uint32_t>(std::string(base.speed_open_time), tmp_vec, "$");
		if (tmp_vec.size() == 2)
		{
			t_speed_time = tmp_vec[0];
			t_speed_cond = tmp_vec[1];
		}
	}
	this->t_speed_target = base.speed_target;
	this->t_touch_notice = base.touch_notice;
	this->t_open_notice = base.open_notice;
	this->t_card_time = base.card_time;
};

inline const std::string GeneralEventTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "GeneralEventTable.tbls64";
#else
	return "GeneralEventTable.tbls32";
#endif
};

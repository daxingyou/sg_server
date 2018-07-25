#ifndef __BUFF_HPP__
#define __BUFF_HPP__

#include "tblh/BuffTable.tbls.h"
#include "combat.hpp"


struct buff_run_attr
{
	buff_run_attr() {}
	uint32_t id				= 0;
	uint32_t tid			= 0;
	uint32_t round			= 0;		//剩余回合数
	uint32_t count			= 0;
	uint32_t layer			= 0;
	uint32_t step			= 0;
	uint32_t damage_heal	= 0;
	uint32_t hero_round		= 0;
	uint64_t attacker		= 0;
	uint32_t trigger_count	= 0;		//触发次数
	uint32_t max_trigger_count = 0;
	uint32_t param1			= 0;		// 特殊参数
	uint32_t round_remove_type = 0;
};									  

class buff_t
{
public:
	buff_t();
	buff_t(uint32_t id, BuffTable* conf, const uint64_t attacker, uint32_t skill, uint32_t level, const uint32_t layer, uint32_t hero_round = 0, uint32_t count = 0);
	~buff_t();

	uint32_t get_id();
	uint32_t get_tid();
	BuffTable* get_conf();

	uint32_t get_skill();
	uint32_t get_skill_level() { return m_skill_level;  }

	uint64_t get_attacker();
	uint32_t get_hero_round();

	uint32_t get_layer();
	void set_layer(uint32_t value);
	void add_layer(uint32_t value = 1);
	void sub_layer(uint32_t value = 1);

	bool has_element(int e);

	uint32_t get_overlay_type();
	uint32_t get_max_layer();
	
	uint32_t get_step();
	void set_step(uint32_t step);

	uint32_t get_damage_heal();
	void set_damage_heal(uint32_t value);

	void set_round(uint32_t round);
	void add_round(uint32_t value = 1);
	void sub_round(uint32_t value = 1);
	uint32_t get_round();

	void set_count(uint32_t count);
	void add_count(uint32_t value = 1);
	void sub_count(uint32_t value = 1);
	uint32_t get_count();

	void set_trigger_count(uint32_t count);
	void add_trigger_count(uint32_t value = 1);
	void sub_trigger_count(uint32_t value = 1);
	uint32_t get_trigger_count();

	uint32_t get_max_trigger_count();

	bool is_valid();
	bool is_persistence();
	buff_effect_type get_effect_type();

	uint32_t get_remove_type();
	uint32_t get_trigger_type();
	uint32_t get_layer_remove_type();
	const std::set<uint32_t> *get_triger_type_list();

	bool need_remove_when_dead();
	bool need_remove_when_revive();//复活时移除

	uint32_t get_small_type();
	uint32_t get_big_type();
	uint32_t get_worth();		// 获取价值
	
	void peek_data(proto::common::fight_buff_single_data *buff_data);
	
	bool is_trigger_type(uint32_t trigger_type);

	uint32_t get_round_remove_type() { return m_run_attr.round_remove_type;  }

public:
	void sub_cd() { if (m_cd > 0) { --m_cd; } }
	void set_cd(uint32_t cd) { m_cd = cd; }
	uint32_t get_cd() { return m_cd; }

	uint32_t get_phase_run() { return m_phase_run;  }
	void add_phase_run() { ++m_phase_run;  }
	void clear_phase_run() { m_phase_run = 0;  }
public:
	void add_trigger_attr(uint32_t type, int32_t value) { m_attr[type] += value; }
	int32_t get_trigger_attr(uint32_t type);
	void clear_trigger_attr() { m_attr.clear(); }
public:
	buff_run_attr m_run_attr;
	uint32_t m_skill = 0;
	uint32_t m_skill_level = 1;			//技能等级
	uint32_t m_cd = 0;					//buff生效cd
	uint32_t m_phase_run = 0;			//一段run
	std::map<uint32_t, int32_t>	m_attr;	//buff改变的属性（用于数据还原,此处如果做的更好，还可以扩展成其他行为 ）
	bool is_public_use = true;
	uint8_t public_use_type = 0;		//0表示全体， 1表示camp1, 2表示camp2(只对光环buff有效)
};



#endif//__BUFF_HPP__
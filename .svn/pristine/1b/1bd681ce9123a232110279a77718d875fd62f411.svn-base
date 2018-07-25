#ifndef __BUFF_HPP__
#define __BUFF_HPP__

#include "tblh/BuffTable.tbls.h"
#include "combat.hpp"


struct buff_run_attr
{
	buff_run_attr() {}
	uint32_t id				= 0;
	uint32_t tid			= 0;
	uint32_t round			= 0;
	uint32_t count			= 0;
	uint32_t layer			= 0;
	uint32_t step			= 0;
	uint32_t damage_heal	= 0;
	uint32_t hero_round		= 0;
	uint64_t attacker		= 0;
	uint32_t trigger_count	= 0;
	uint32_t max_trigger_count = 0;
	uint32_t param1			= 0;		// 特殊参数
};									  

class buff_t
{
public:
	buff_t();
	buff_t(uint32_t id, BuffTable* conf, const uint64_t attacker, uint32_t skill, const uint32_t layer, uint32_t hero_round = 0);
	~buff_t();

	uint32_t get_id();
	uint32_t get_tid();
	BuffTable* get_conf();

	uint32_t get_skill();

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

	bool need_remove_when_dead();

	uint32_t get_small_type();
	uint32_t get_big_type();
	uint32_t get_worth();		// 获取价值
	
	void peek_data(proto::common::fight_buff_single_data *buff_data);

public:
	buff_run_attr m_run_attr;
	uint32_t m_skill = 0;

};

#endif//__BUFF_HPP__
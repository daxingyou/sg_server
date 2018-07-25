#ifndef __BUFF_MANAGER_HPP__
#define __BUFF_MANAGER_HPP__

#include "buff.hpp"
#include "buff_element.hpp"
#include "fight_common.hpp"

enum ROUND_REMOVE_TYPE {
	ROUND_REMOVE_TYPE_NORMAL = 0,
	ROUND_REMOVE_TYPE_BOMB   = 1,
	ROUND_REMOVE_TYPE_DOT    = 2,
};


class buff_manager_t
{

public:
	buff_manager_t();
	~buff_manager_t();

public:

	static bool init();

	void		close();

	void		set_owner(fight_hero_ptr owner);

// 	void		round_start();
// 	void		round_end();
// 	void		before_attack();
// 	void		after_attack();
// 	void		before_be_attack();
// 	void		after_be_attack();
// 	void		before_attack_phase();
// 	void		after_attack_phase();
// 	void		before_be_attack_phase();
// 	void		after_be_attack_phase();
	void		on_dead(); 
	void		on_revive();

	// -----------------------------------------主动buff--------------------------------------------------------------------
	buff_ptr	add_buff(const uint32_t id, const uint64_t attacker = 0, const uint32_t skill = 0, const uint32_t layer = 1, const uint32_t round = 0 );
	bool		can_add_buff(BuffTable* p_buff_conf, uint32_t skill);

	// 改变BUFF回合
	void		change_debuff_round(const int change);
	void		change_profit_buff_round(const int change);
	void		change_buff_round(const int buff_tid, const int change);

	bool		remove_buff_by_tid(uint32_t tid, uint64_t attacker_id = 0);
	bool		remove_buff_by_small_type(uint32_t small_type, uint64_t attacker_id = 0);
	bool		remove_buff(uint32_t id, uint64_t attacker_id = 0, const bool dispel = false);
	uint32_t	remove_buff(buff_ptr buff, uint64_t attacker_id = 0, const bool dispel = false);
	uint32_t	remove_profit_buff_by_count(uint32_t count);
	uint32_t	remove_debuff_by_count(uint32_t count);
	void		remove_all_buff();
	void		dispel_buff(const buff_dispel_info_vec& buffs, uint32_t num, fight_hero_ptr p_attacker);		// 移除多种效果类型BUFF
	uint32_t	dispel_buff(uint32_t type, uint32_t num, uint64_t attacker_id = 0);		// 移除指定效果类型BUFF
	
	buff_ptr	get_buff_by_tid(const uint32_t tid, uint64_t attacker_id = 0) const;
	buff_ptr	get_random_buff_by_bigtype(const uint32_t type);
	buff_ptr	get_random_buff_by_smalltype(const int32_t type);
	uint32_t    get_all_count_by_effect_type(const int32_t type);

	void		add_trigger_count(const uint32_t id, uint32_t count = 1);
	void		sub_trigger_count(const uint32_t id, uint32_t count = 1);
	void		sub_buff_layer(const uint32_t tid, uint64_t attacker = 0, uint32_t count = 1);
	bool		add_buff_layer(const uint32_t tid, uint64_t attacker = 0, uint32_t count = 1);
	bool		add_buff_layer(buff_ptr buff, uint32_t count = 1);
	bool		sub_buff_layer(buff_ptr buff, uint32_t count = 1);
	void		sub_buff_layer_or_remove(const uint32_t tid, uint64_t attacker = 0, uint32_t count = 1);
	uint32_t	get_buff_layer(const uint32_t id);
	uint32_t	get_buff_layer_by_tid(uint32_t tid, uint64_t attacker = 0);
	uint32_t	get_buff_count_by_tid(uint32_t tid, uint64_t attacker = 0);
	uint32_t    get_buff_layer_by_smalltype(uint32_t tid, uint64_t attacker = 0);
	//根据buff增减益类型计算层数
	uint32_t	get_buff_layer_by_effect_type(buff_effect_type type);
	uint32_t	get_buff_round(const uint32_t id);
	uint32_t	get_buff_round_by_tid(uint32_t tid, uint64_t attacker = 0);
	uint64_t	get_buff_attacker(const uint32_t id);
	bool		check_passive_buff_cd(buff_ptr buff);
	void		add_passive_buff_cd(buff_ptr buff, uint32_t old_turn_level);
	void		clear_trigger_count();

	// 获取减益BUFF数量
	uint32_t	get_debuff_count();
	// 获取增益BUFF数量
	uint32_t	get_profit_buff_count();
	// 引爆BUFF
	void		detonate_buff(const int element_type, const fight_hero_ptr p_event_owner, bool is_remove = true );
	// 引爆buff
	void		detonate_buff_by_smalltype(const uint32_t buff_small_type, const fight_hero_ptr p_event_owner, bool is_remove = true);
	// 获取大类型BUFF数量
	uint32_t	get_count_by_big_type(const int type);
	// 获取小类型BUFF数量
	uint32_t	get_count_by_small_type(const int type);
	buff_ptr	get_buff_by_small_type(const uint32_t small_type, uint64_t attacker_id = 0) const;
	// 获取驱散BUFF数量
	void		add_dispel_buff_count(uint32_t count = 1) { m_dispel_buff_count += count; }
	uint32_t    get_dispel_buff_count() const { return m_dispel_buff_count; }
	void		clear_dispel_buff_count() { m_dispel_buff_count = 0; }
	// 或者开始事件
	void		on_round_start();
	//----------------------------------------------------------------------------------------

	// --------------------------------非主动buff---------------------------------------------
	buff_ptr	add_special_buff(const uint32_t id, const uint64_t attacker = 0, const uint32_t skill = 0, const uint32_t layer = 1);
	bool		remove_special_buff(uint32_t id, uint64_t attacker_id = 0);
	bool		remove_special_buff_by_tid(uint32_t tid, uint64_t attacker_id = 0);
	void		remove_all_special_buff();
	//----------------------------------------------------------------------------------------

public:
	fight_hero_ptr	get_owner();
	void		peek_all_buff_data(proto::common::fight_buff_data *buff_data);
	bool		update_buff(FIGHT_EVENT check_type, const fight_hero_ptr p_event_owner);
	buff_ptr	get_buff_by_id(const uint32_t id) const;
	void		sub_buff_cd();
	void		clear_phase_run_info();

public:
	static		buff_ptr make_new_buff( combat_ptr p_combat, uint32_t id, uint64_t uid, uint32_t skill_id, uint32_t level, uint32_t layer = 0 );
	uint32_t	run_public_buff(buff_ptr buff, const fight_hero_ptr p_event_owner, uint32_t event_num);
	void		do_specail_buff_event(uint32_t type, fight_hero_ptr p_attacker );

protected:
	uint32_t	run_buff(buff_ptr buff, const int numchange = 0, const fight_hero_ptr  p_event_owner = NULL, uint32_t event_num = 0 );
	uint32_t	remove_buff_by_count(uint32_t count, buff_effect_type type);
	uint32_t	get_buff_count(buff_effect_type type);
	void		change_buff_round_by_type(const int change,buff_effect_type type);
	void		change_buff_round(buff_ptr p_buff, const int change);
	bool		buff_safe_check();

	// ----------------------------------主动buff-----------------------------
	buff_ptr	add_new_buff(buff_ptr buff);
	buff_ptr	cover_buff(buff_ptr os, BuffTable* ns, uint32_t layer = 0);
	uint32_t	clac_gain_and_debuff(BuffTable* ns);

	bool		check_trigger(FIGHT_EVENT check_type, buff_ptr buff, uint32_t& notify, bool needprobability = true, const fight_hero_ptr p_event_owner = NULL );
	bool		check_remove_layer(FIGHT_EVENT check_type, buff_ptr buff, uint32_t& notify);
	bool		check_remove(FIGHT_EVENT check_type, buff_ptr buff, uint32_t& notify);
	void		remove_buff_when_dead();
	void		remove_buff_when_revive();
	bool		in_remove_set(buff_ptr buff) const;
	bool		in_remove_set(uint32_t buff_id) const;
	//--------------------------------------------------------------------

	// --------------------------非主动buff--------------------------------------
	void		add_new_special_buff(buff_ptr buff);
	bool		remove_special_buff(buff_ptr buff, uint32_t attacker_id = 0);

	//----------------------------------------------------------------------------
private:
	typedef		std::vector<buff_ptr> buff_vec;
	typedef		std::set<buff_ptr> buff_set;

	uint32_t		m_dispel_buff_count = 0;		// 驱散BUFF数量
	buff_vec		m_buffs;
	buff_vec		m_special_buffs;
	buff_set		m_remove_set;
	fight_hero_wptr m_owner;

private:
	static std::map<uint32_t, elenment_func> m_funclist;
};

#endif//__BUFF_MANAGER_HPP__
#ifndef __GAME_FORMATION_MANAGER_H__
#define __GAME_FORMATION_MANAGER_H__
#include <map>
#include "formation.hpp"

class role_t;

typedef boost::shared_ptr<role_t> role_ptr;

class formation_manager_t
{
public:
	formation_manager_t(uint64_t owner);
	virtual ~formation_manager_t();

	// 数据相关
public:
	// load datas from db
	void load_all_datas(const proto::common::formation_data& data);
	// send datas to client
	void peek_all_datas(proto::common::formation_data* p_data);
	// send datas to client
	void peek_datas(uint32_t formation_id, proto::common::formation_single* p_data);
	// send datas to client
	void peek_cur_formation_datas(proto::common::formation_single* p_data);

	void peek_fight_data(proto::common::hero_data* p_data);
	// 管理函数
public:

	// 添加布阵
	bool add_formation(formation_ptr p_formation);
	// 添加布阵
	void add_formation(const proto::common::formation_single& single);
	// 检查武将上阵要求
	bool check_hero_up_form(uint32_t fight_type, hero_ptr p_hero, role_ptr p_role);
	// 是否是有效的布阵
	virtual bool is_validate_formation(
		role_ptr p_role, 
		formation_ptr p_formation, 
		uint32_t fight_type, 
		const std::map<uint32_t, PosHeroData> &hero_pos,
		uint32_t tactic_id);
	// 添加新的布阵
	virtual bool init_formation();
	virtual void set_troop_member_num(uint32_t value)
	{
	}
	// 更新布阵
	virtual bool update_formation(
		role_ptr p_role, 
		uint32_t form_index, 
		uint32_t fight_type, 
		const std::map<uint32_t, PosHeroData> &hero_pos, 
		uint32_t tactic_id);

	virtual bool update_team_formation(
		role_ptr p_role,
		uint32_t member_size,
		const std::map<uint32_t, PosHeroData> &hero_pos,
		uint32_t tactic_id);

	// 获取布阵信息
	const std::map<uint32_t, PosHeroData>& get_pos_hero(uint32_t form_index);

	// 更新当前阵容
	bool update_cur_form_id(uint32_t form_id);
	//计算战斗力
	uint32_t calc_fighting();
	//是否在当前阵容
	bool is_in_cur_form(uint64_t hero_uid);
	//更新防守阵容
	void update_offline_arena_form(uint64_t hero_uid);
	//更新防守阵容
	void update_offline_arena_form();
	//获取当前阵容的武将数量
	uint32_t get_cur_hero_count();
	//添加武将给全部布阵
	void add_hero_to_all_formation(role_ptr p_role, uint64_t hero_uid);
	//移除全部布阵中的武将
	void remove_hero_from_all_formation(role_ptr p_role, uint64_t hero_uid);
	//替换全部布阵中的武将
	void replace_hero_from_all_formation(role_ptr p_role, uint64_t old_hero_uid, uint64_t new_hero_uid);
public:

	formation_ptr get_formation(uint32_t form_id);
	formation_ptr get_cur_formation() { return get_formation(get_cur_formation_id()); }
	inline void set_cur_formation_id(uint32_t formation_id) { m_cur_formation_id = formation_id; }
	inline uint32_t get_cur_formation_id() { return m_cur_formation_id > 0 ? m_cur_formation_id : 1; }
	inline uint32_t get_formation_cnt() { return static_cast<uint32_t>(m_formation_list.size()); }

	static void peek_expedition_formation_data(role_ptr p_role, proto::common::fight_special_hero_data *hero_data);
	static void peek_family_formation_data(role_ptr p_role, proto::common::fight_special_hero_data *hero_data);
protected:
	uint64_t m_owner = 0;
	uint32_t m_cur_formation_id = 0;
	formation_map m_formation_list;

};

// 组队布阵类
class troop_formation_manager_t : public formation_manager_t
{
public:
	troop_formation_manager_t(uint64_t owner);
	virtual ~troop_formation_manager_t();
	// 添加新的布阵
	virtual bool init_formation();
	// 是否是有效的布阵
	virtual bool is_validate_formation(
		role_ptr p_role, 
		formation_ptr p_formation, 
		uint32_t fight_type, 
		const std::map<uint32_t, PosHeroData> &hero_pos, 
		uint32_t tactic_id);
	// 更新布阵
	virtual bool update_formation(
		role_ptr p_role, 
		uint32_t form_index, 
		uint32_t fight_type, 
		const std::map<uint32_t, PosHeroData> &hero_pos, 
		uint32_t tactic_id);
	virtual bool update_team_formation(
		role_ptr p_role,
		uint32_t member_size,
		const std::map<uint32_t, PosHeroData> &hero_pos,
		uint32_t tactic_id)
	{
		return false;
	}
	virtual void set_troop_member_num(uint32_t value)
	{
		m_troop_member_num = value;
	}
private:
	uint32_t m_troop_member_num;
};

typedef boost::shared_ptr<formation_manager_t> formation_manager_ptr;

#endif // !__GAME_FORMATION_MANAGER_H__
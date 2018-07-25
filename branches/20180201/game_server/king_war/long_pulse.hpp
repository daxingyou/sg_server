#ifndef __GAME_LONG_PULSE_H__
#define __GAME_LONG_PULSE_H__

#include "monster/monster.hpp"
#include "king_war_common.hpp"

#include "client.pb.h"
#include "role/role.hpp"

#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

class long_pulse_t : public monster_t
{
public:
	long_pulse_t() {}
	virtual ~long_pulse_t();

	virtual void del_clear();

	//是否是龙脉
	virtual bool is_long_pulse() { return true; }
	// 获取龙脉指针
	virtual long_pulse_ptr get_long_pulse() { return boost::dynamic_pointer_cast<long_pulse_t>(shared_from_this()); }
	// 获取对象场景属性
	virtual void get_object_info(proto::common::object_common_info* object_info);

public:
	// 生物进入视野
	virtual void other_object_into_vision(const object_id_type& object);
	// 通知离开视野
	virtual void other_object_out_of_vision(const object_id_type& object);

public:
	// 怪物初始化
	bool init_long_pulse(const monster_common::king_war_monster& define, Monster* monster);

	// 获取最大血量
	virtual uint32_t get_max_hp() { return m_max_hp; }
	// 获取血量
	virtual uint32_t get_current_hp() { return m_curr_hp; }
	// 造成伤害
	void hurt_by_family(uint32_t damage, uint64_t role_uid, uint64_t family_id);
	// 出生
	virtual void on_object_born();
	// 被家族击杀
	virtual void set_object_die(uint64_t obj_killer = 0);
	virtual void on_object_die();
	// 时间到了复活了
	virtual void on_object_revive();

	uint32_t get_country_id() const { return m_country_id; }
	void set_country_id(uint32_t country_id) { m_country_id = country_id; }

public:
	// 获得龙脉类型
	king_war_obj_type get_long_obj_type() const { return m_obj_type; }

	// 获得占领家族
	uint64_t get_occupy_family() const { return m_occupy_family; }

	// 设置参战家族
	void set_king_war_family(king_war_side_type type, uint64_t family_id);

	// 请求加入防御队列
	uint32_t ask_add_defence_list(role_ptr p_role);

	// 请求攻击龙脉记时
	uint32_t ask_add_attack_list(role_ptr p_role);

	// 重置攻击时间为0
	void ask_end_attack(uint64_t role_uid);

	// 请求造成伤害
	void ask_hurt_long(role_ptr p_role, proto::client::gc_king_war_attack_end_reply& reply);

	// 是否是参战家族
	king_war_side_type get_king_war_side_type(uint64_t family_id);

	// 设置周围人的icon
	void update_nearby_icon();

	// 加定时龙气
	void add_family_long_gas();

public:
	// 战斗，不能进入战斗直接返回0
	virtual uint64_t fight_with_role(role_ptr p_role) { return 0; }

	// 定时器回调
public:
	void on_revive_timer_call_back();

	void on_occupied_timer_call_back();

protected:
	uint32_t m_curr_hp = 0;		// 当前血量
	uint32_t m_max_hp = 0;		// 最大血量

	uint32_t m_country_id = 0;	// 国家id

	// 攻击龙脉每轮判断时间
	uint32_t m_round_time = 0;

	// 龙脉类型
	king_war_obj_type m_obj_type = king_war_obj_type_none;
	// 参战家族
	uint64_t m_war_family[king_war_side_max] = { 0 };
	// 占领家族
	uint64_t m_occupy_family = 0;
	// 龙脉区域
	uint32_t m_area_id = 0;

	// 防御列表，不是实时更新，只会在有人请求时验证更新
	std::set<uint64_t> m_defence_list;
	// 攻击列表(uid,时间)
	std::map<uint64_t, uint32_t> m_attack_list;
	// 伤害
	uint32_t m_family_hurt[king_war_side_max] = { 0 };

	// 定时器，注意删怪物的的时候清除，防止泄露
protected:
	boost::shared_ptr<boost::asio::deadline_timer> m_revive_timer = NULL;	// 复活定时器

	boost::shared_ptr<boost::asio::deadline_timer> m_occupied_timer = NULL;	// 占领加龙气定时器
};

#endif

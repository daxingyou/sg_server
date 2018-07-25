#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "scene_common.hpp"

#include <vector>
#include <map>
#include <set>

#include "object/object_position.hpp"
#include "monster/monster.hpp"
#include "map.hpp"
#include "tblh/Monster.tbls.h"
#include "config/config_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "object/object_manager.hpp"
#include "role/role_manager.hpp"
#include "collect/collect_manager.hpp"

//////////////////////////////////////////////////////////////////////////
class scene_t;
typedef boost::shared_ptr<scene_t> scene_ptr;

class king_war_scene_t;
typedef boost::shared_ptr<king_war_scene_t> king_war_scene_ptr;

class dungeon_scene_t;
typedef boost::shared_ptr<dungeon_scene_t> dungeon_scene_ptr;

class family_war_scene_t;
typedef boost::shared_ptr<family_war_scene_t> family_war_scene_ptr;

// 
struct scene_select_t
{
	virtual ~scene_select_t() {};
	virtual bool exec(const role_ptr& p_role) const = 0;
};

// 场景先init,要使用就start 不要了就unload
class scene_t : public scene_base, public boost::enable_shared_from_this<scene_t>
{
public:
	scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param);
	virtual ~scene_t();

	// 初始化放在这里
	virtual bool init();
	// 清空人（必须自己清空）生物（删除地图首先调用）,清除脚本对象，清理之后就必须删除该场景了
	virtual void clear();
	// 清除全场景除人意外的生物
	void clear_all_monster();
	// 启动场景
	virtual bool start();
	// 卸载场景
	bool unload();
	// 加载所有地图生物(静态npc有客户端自己生成)
	virtual void load_map_object();
	// 加载所有地图所有采集物
	virtual void load_map_collect();
	// 初始化场景生物
	bool init_object(monster_ptr p_monster, const monster_common::monster_define_t& define);
	// 复活生物
	bool revive_object(monster_ptr p_monster, float x, float y, float dir);
	// 初始化采集点
	bool init_collect_point(collect_point_ptr p_collect_point, bool is_new = true);
	// 召唤一个怪物
	template <typename Monster_T>
	monster_ptr summon_monster(uint32_t id, float x, float y, float dir = 0, uint64_t object_id = 0);

	collect_point_ptr summon_collect_point(uint32_t tid, float x, float y);

	uint64_t get_scene_id() const { return m_scene_id; }
	uint32_t get_map_tid() const { return m_map_tid; }
	uint32_t get_map_group_id();
	proto::server::em_map_type get_map_type() const { return m_map_type; }
	uint32_t get_city_id();

	// 设置是否是分流场景
	void set_diversion(bool is_diversion, uint32_t num);
	uint32_t is_diversion() const { return m_is_diversion; }
	uint32_t get_scene_max_num() const { return m_scene_max_num; }

	// 得到场景类型
	proto::server::em_scene_type get_scene_type() const { return m_scene_type; }
	// 得到场景附加参数
	uint32_t get_scene_type_param() const { return m_type_param; }

	inline map_ptr get_map_ptr() { return m_map_wptr.lock(); }

	// 获取场景指针
	scene_ptr get_scene_ptr() { return shared_from_this(); }
	// 是否是王城
	virtual bool is_king_war_city() { return false; }
	// 获取王城战场景指针
	virtual king_war_scene_ptr get_king_war_scene_ptr() { return king_war_scene_ptr(); }
	// 是否是副本
	virtual bool is_dungeon_scene() { return false; }
	// 获取副本场景指针
	virtual dungeon_scene_ptr get_dungeon_scene_ptr() { return dungeon_scene_ptr(); }
	// 获得副本id
	virtual uint32_t get_dungeon_id() { return 0; }
	// 是否是家族战副本
	virtual bool is_family_war_scene() { return false; }
	// 获取副本场景指针
	virtual family_war_scene_ptr get_family_war_scene_ptr() { return family_war_scene_ptr(); }

	/*场景相关*/
public:
	// 得到场景玩家进入后的出生坐标
	virtual sPos get_scene_born_position(const role_ptr& p_role);
	// 玩家是否能进场景
	virtual uint32_t scene_enter_check(const role_ptr& p_role) { return 0; }
	// 进入场景
	bool on_enter_s(object_base_ptr p_object, float x, float y, float dir = 0.f);
	// 离开场景
	void on_leave_s(object_id_type object_id);
	void on_leave_s(object_base_ptr p_object);
	// 移动同步
	bool on_move_s(const object_base_ptr& p_object, float to_x, float to_y, float direction, uint64_t time_stamp, uint32_t high);
	// 得到生物附近生物，复杂
	bool get_nearby_objects(float x, float y, std::vector<object_id_type>& objects_nearby, bool role_only = false, float distance = 0, bool check_block = false);
	// 通知移动消息
	void notify_nearbies_move_s(object_base_ptr p_object, const nine_vector& nines);

	// 生物9屏更新
	bool object_into_vision(object_base_ptr p_object, const nine_vector& nines);
	// 人物视野更新
	bool role_into_vision(role_ptr p_role, const nine_vector& nines);
	// 生物离开9屏
	bool object_out_of_vision(object_base_ptr p_object, const nine_vector& nines);
	// 人物视野更新
	bool role_out_of_vision(role_ptr p_role, const nine_vector& nines);

	// 获取9屏生物信息
	bool send_nine_to_me(uint32_t nine_key, proto::client::gc_role_into_vision_notify* notify);

	// 获取区域ID
	uint32_t get_area_id(float x, float y);

	// 得到复活点
	uint32_t get_revive_point();

	// 场景是否能够组队
	virtual uint32_t check_scene_troop();

	/*定时器*/
public:
	// 到达最大时间限制
	virtual bool reach_max_time(uint32_t curr) const { return false; }
	// 到达场景时间限制
	virtual bool reach_end_time(uint32_t curr) const { return false; }

	// 1秒定时器
	virtual void update(uint32_t curr_time) {};

	/*人物相关*/
public:
	virtual void on_role_enter(role_ptr p_role);
	virtual void on_role_leave(role_ptr p_role);

	uint32_t get_role_size() const { return m_roles.size(); }

	// 虚函数玩家离开场景中的一个区域的回调
	virtual void role_leave_area(role_ptr p_role, uint32_t area_id) {};
	// 虚函数玩家进入场景中的一个区域的回调
	virtual void role_enter_area(role_ptr p_role, uint32_t area_id) {};

	// 复活某玩家
	virtual void on_revive_role(role_ptr p_role);
	virtual void revive_all_role() {};

	// 有玩家杀了别人
	virtual void on_role_kill_other(role_ptr p_role, object_base_ptr p_enemy) {};

	// 自己被别人击杀
	virtual void on_other_kill_me(role_ptr p_role, object_base_ptr p_enemy);

	// 显示复活界面
	virtual void show_revive_ui(role_ptr p_role);

	// 登陆死亡处理
	virtual void on_login_die(role_ptr p_role);

	// 是否有玩家在副本中战斗
	virtual bool is_have_fight();

	// 设置所有战斗失败
	virtual void set_all_combat_failed();

	// 强制所有玩家离开场景(默认是空函数，对那些有生命周期的场景派生这个函数)
	virtual void forced_all_leave() {};

	/*场景BOSS相关*/
public:
	void add_field_boss(uint64_t object_id);
	void del_field_boss(uint64_t object_id);

	bool scene_have_boss() const { return !m_field_boss.empty(); }

	/* 消息相关 */
public:
	// 场景转发，如果有选择条件，设置回调
	template<typename T_MSG>
	void send_msg_to_scene(uint16_t cmd, const T_MSG& msg, const scene_select_t* SELECT = NULL)
	{
		// 没人直接return
		if (m_roles.empty())
		{
			return;
		}

		proto::server::ga_broadcast_notify msg_to_gate;

		for (std::set<uint64_t>::const_iterator citr = m_roles.begin();
			citr != m_roles.end(); ++citr)
		{
			if (NULL != SELECT)
			{
				role_ptr p_role = role_manager_t::find_role(*citr);
				if (NULL == p_role || !SELECT->exec(p_role))
				{
					continue;
				}
			}

			msg_to_gate.add_role_list(*citr);
		}

		// 给gate转发
		if (msg_to_gate.role_list_size() > 0)
		{
			msg_to_gate.set_cmd(cmd);

			std::string cmd_data;
			network::wrap_msg(cmd_data, msg);
			msg_to_gate.set_data(cmd_data);

			env::server->send_msg_to_all_gate(op_cmd::ga_broadcast_notify, 0, msg_to_gate);
		}
	}

	// 9屏转发
	void send_msg_to_nine(uint32_t nine_key, uint16_t cmd, const std::string& cmd_data);
	
	// 成员变量
protected:
	uint64_t m_scene_id = 0;
	uint32_t m_map_tid = 0;
	proto::server::em_scene_type m_scene_type = proto::server::em_scene_type_none;
	uint32_t m_type_param = 0;
	bool m_is_diversion = false;	// 是否是主场景的分流场景
	uint32_t m_scene_max_num = 0;	// 场景中最多容纳人数
	proto::server::em_map_type m_map_type = proto::server::map_type_none;
	map_wptr m_map_wptr;	// map的弱指针

protected:
	// 格子定义
	typedef std::set<object_id_type> scene_grid;
	// 视野范围内的格子
	typedef std::vector<scene_grid*> scene_vision;

	std::set<uint64_t> m_roles;
	uint32_t m_grid_num = 0;	// 安全起见记录格子数
	scene_grid** m_scene_objects = NULL;//new 维护场景中每个点对应的生物

	// 一些场景信息
protected:
	std::set<uint64_t> m_field_boss;	// 该场景中的首领id
};

template <typename Monster_T>
monster_ptr scene_t::summon_monster(uint32_t id, float x, float y, float dir/* = 0*/, uint64_t object_id/* = 0*/)
{
	Monster* monster = GET_CONF(Monster, id);
	if (NULL == monster) {
		log_error("scene::load_map_object not found monster [%u]", id);
		return NULL;
	}

	monster_common::monster_define_t define;
	define.id = id;
	define.pos_x = x;
	define.pos_y = y;
	define.dir = dir;

	monster_ptr p_monster(new Monster_T());
	if (NULL == p_monster) {
		log_error("monster null error");
		return NULL;
	}

	if (!p_monster->init_monster(define, monster, object_id)) {
		log_error("monster[%u] init error", define.id);
		return NULL;
	}

	if (!init_object(p_monster, define)) {
		log_error("monster[%u] scene init error", define.id);
		return NULL;
	}

	SCENE_LOG("map[%u] summon monster[%u] pos[%f, %f]", m_map_tid, id, x, y);
	return p_monster;
}

#endif // !__GAME_SCENE_H__

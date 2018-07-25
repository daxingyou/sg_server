#ifndef __GAME_WORLD_BOSS_H__
#define __GAME_WORLD_BOSS_H__

#include "monster/monster.hpp"

#include <map>
#include <vector>
#include <boost/asio/deadline_timer.hpp>
#include "server.pb.h"
#include "fight/combat.hpp"

class field_boss_t : public monster_t
{
public:
	field_boss_t();
	virtual ~field_boss_t();

	// 清理
	virtual void del_clear();

	//是否是野外boss
	virtual bool is_field_boss() { return true; }

	// 获取对象指针
	virtual field_boss_ptr get_field_boss() { return boost::dynamic_pointer_cast<field_boss_t>(shared_from_this()); }

	// 获取对象场景属性
	virtual void get_object_info(proto::common::object_common_info* object_info);

public:
	// 生物进入视野
	virtual void other_object_into_vision(const object_id_type& object);
	// 通知离开视野
	virtual void other_object_out_of_vision(const object_id_type& object);
	// 进入场景
	virtual void on_enter_scene(scene_ptr p_scene);
	// 离开场景
	virtual void on_leave_scene(scene_ptr p_scene);

public:
	struct boss_role_info
	{
		uint64_t role_id = 0;			// 人物id
		std::string role_name;			// 名字
		uint32_t hurt_time = 0;			// 造成的伤害时间
		uint32_t hurt = 0;				// 伤害
		uint32_t leave_fight_time = 0;	// 离开战斗时间

		// 返回true的排在前面
		bool operator< (const boss_role_info& item) const
		{
			if (this->hurt > item.hurt) {
				return false;
			}

			if (this->hurt == item.hurt) {
				if (this->hurt_time != item.hurt_time) {
					return this->hurt_time > item.hurt_time;
				} else {
					return this->role_id < item.role_id;
				}
			}
			return true;
		}
	};
	// 2个容器都持有指针，删除的时候一起删除
	typedef boost::shared_ptr<boss_role_info> boss_role_info_ptr;
	typedef std::vector<boss_role_info_ptr> boss_role_vec;	// 排序容器中
	typedef std::map<uint64_t, boss_role_info_ptr> boss_role_info_map;

	struct monster_hero_info
	{
		uint32_t m_id;
		uint32_t m_curr_hp;	// 当前血量
		uint32_t m_max_hp;	// 最大血量

		monster_hero_info() : m_id(0), m_curr_hp(0), m_max_hp(0) {};
		monster_hero_info(uint32_t id, uint32_t curr, uint32_t max) : m_id(id), m_curr_hp(curr), m_max_hp(max) {};
	};

	typedef std::map<uint32_t, monster_hero_info> monster_hero_map;
	// 当前和参加boss战的id
	typedef std::set<uint64_t> field_boss_combat;
	// 家族伤害排名
	typedef std::map<uint64_t, uint32_t> family_hurt_map;

public:
	// 怪物初始化
	virtual bool init_monster(const monster_common::monster_define_t& define, Monster* monster, uint64_t object_id = 0);
	virtual uint64_t fight_with_role(role_ptr p_role) { return 0; };
	// 对战id实时计算
	virtual uint32_t get_against_form_id();

	// 获取最大血量
	virtual uint32_t get_max_hp() const { return m_boss_max_hp; }
	virtual void set_max_hp(uint32_t value) { m_boss_max_hp = value; }
	// 获取血量
	virtual uint32_t get_current_hp() const { return m_boss_curr_hp; }
	virtual void set_current_hp(uint32_t value) { m_boss_curr_hp = value; }

	virtual void on_tick(uint64_t msecs);

public:
	// 设置配置表id
	void set_id(uint32_t id) { m_id = id; }
	uint32_t get_id() const { return m_id; }

	// 得到一个玩家的伤害信息
	boss_role_info_ptr get_boss_role_info(uint64_t uid);
	boss_role_info_ptr add_boss_role_info(uint64_t uid);

	// boss 伤害更新
	void fight_role_hurt(uint64_t fight_id, const proto::common::fight_camp& self_camp, const proto::common::fight_camp& enemy_camp, const combat_ptr& combat);
	void update_all_combat(uint64_t exception = 0);
	bool get_hero_state_data(proto::common::hero_state_data* data);
	bool sync_all_boss_hp( uint64_t fight_id,  const proto::common::hero_state_data& data);

	// 排名
	void calc_persion_rank(proto::server::EM_FIELD_MODE mode, proto::server::ge_field_boss_rank_notify& ntf);
	// 家族排名
	void calc_family_rank(proto::server::EM_FIELD_MODE mode, proto::server::ge_field_boss_rank_notify& ntf);

	// 攻击boss冷却
	uint32_t get_fight_cd_time(uint64_t uid);

	void add_combat(uint64_t id);
	void del_combat(uint64_t id);
	void leave_fight(uint64_t fight_id, const proto::common::fight_camp& self_camp);
	// 设置所有战斗失败
	void set_all_fight_failed(uint64_t exception = 0);
	//设置所有战斗成功
	void set_all_fight_success(uint64_t exception = 0);
	// 设置活动结束
	void field_boss_end(proto::server::EM_FIELD_MODE mode);
	// 发放奖励
	void start_settlement(proto::server::EM_FIELD_MODE mode);

protected:
	uint32_t m_id = 0;		// 配置表id
	uint32_t m_against_form_id = 0;
	monster_hero_map m_hero_map;
	uint32_t m_boss_curr_hp = 0;
	uint32_t m_boss_max_hp = 0;

	boss_role_vec m_boss_role_vec;
	boss_role_info_map m_boss_role_map;
	field_boss_combat m_field_boss_combat;
	family_hurt_map m_family_hurt_map;
	std::vector<uint64_t> m_killers;	// 击杀者可能多个，因为组队
	std::string m_killer_name;

	// 设置战斗是否结束
	bool m_is_fight_end = false;
protected:
	// 定时器
	bool m_is_need_sync = false;
	common::cTimer m_sync_sec;
};

#endif
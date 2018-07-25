#ifndef __USER_TRROP_HPP__
#define __USER_TRROP_HPP__

#include "macros.hpp"
#include "common.pb.h"
#include <map>
#include <deque>
#include <boost/shared_ptr.hpp>
#include "role/role_manager.hpp"
#include "formation/formation_manager.hpp"

class scene_t;
typedef boost::shared_ptr<scene_t> scene_ptr;

class troop_t
{
public:
	troop_t(uint64_t uid);

	struct troop_member
	{
		uint64_t m_uid = 0;

		uint64_t get_uid() const { return m_uid; }

		proto::common::EM_SCENEOBJECT_TYPE get_object_type() const { return m_base.object_type(); }

		object_id_type get_object_id_type() { return object_id_type(m_uid, m_base.object_type()); }

		const std::string& get_name() const { return m_base.name(); }

		uint32_t get_plugin() const { return m_base.plugin(); }

		uint32_t get_fighting() const { return m_base.fighting(); }

		uint32_t get_level() const { return m_base.level(); }

		uint32_t get_state() const { return m_base.state(); }

		void set_state(uint32_t val) { m_base.set_state(val); }

		uint32_t get_fight_times() const { return m_base.fight_times(); }

		uint64_t get_owner_uid() const { return common::string_util_t::string_to_uint64(m_base.owner_uid()); }

		const proto::server::troop_extra_info& get_member_extra() const { return m_extra_info; }

		void load_data(proto::common::user_troop_member* p_data);
		void load_extra(proto::server::troop_extra_info* p_data);

		void peek_data(proto::common::user_troop_member* p_data) {
			if (p_data) p_data->CopyFrom(m_base);
		}

		void set_team_check(proto::common::team_check_type type) { m_team_check_type = type; }
		proto::common::team_check_type get_team_check() const { return m_team_check_type; }

		proto::common::user_troop_member m_base;
		proto::common::team_check_type m_team_check_type = proto::common::team_check_type_none;

		bool is_trade_state() const { return m_is_trade; }
		void set_is_trade(bool is_trade) { m_is_trade = is_trade; }
		bool m_is_trade = false;

		proto::server::troop_extra_info m_extra_info;
	};

	typedef boost::shared_ptr<troop_member> troop_member_ptr;
	typedef std::vector<troop_member_ptr> troop_member_vec;
	typedef std::pair<troop_member_ptr, int32_t> troop_member_index;

public:
	// 队伍id
	uint64_t get_uid() const { return m_uid; }
	// 队伍类型
	uint32_t get_type() const { return m_type; }

	const troop_member_vec& get_members() { return m_troop_members; }
	void get_normal_members(troop_member_vec& members);

	// 队伍人数
	uint32_t get_member_size() const { return m_troop_members.size(); }
	// 是否满员
	bool is_full() const { return m_troop_members.size() >= 3; }
	// 获得玩家的数量
	bool get_role_size() const;

	// 获取队员
	troop_member_ptr get_member(uint64_t member_uid);
	troop_member_index get_member_index(uint64_t member_uid);

	// 队长id
	uint64_t get_leader_id() const { return m_leader_uid; }

	// 队伍数据
	void peek_data(proto::common::troop_data* p_data);

	// 跨场景处理
public:
	// 跨场景队伍处理
	void cross_server_init(const role_ptr& p_role);
	void cross_server_update(const role_ptr& p_role, proto::server::scene_change_type type);
	// 传送读条结束
	uint32_t do_transmission_reading_end(role_ptr p_role);

	// 事件通知
	void notify_event(uint32_t event_id, const std::string& member_name);

	// 各个事件的处理
public:
	void process_event(uint32_t event_id, uint64_t member_id, proto::server::eg_troop_event_notify& ntf);

protected:
	void do_create_troop(uint64_t member_id, proto::server::troop_server_data* p_server_data);
	void do_change_leader(uint64_t member_id, proto::server::troop_server_data* p_server_data);
	void do_member_join(uint64_t member_id, proto::server::troop_server_data* p_server_data);
	void do_member_quit(uint64_t member_id, bool is_notify);
	void do_member_kick(uint64_t member_id);
	void do_member_leave(uint64_t member_id);
	void do_member_back(uint64_t member_id, proto::server::troop_server_data* p_server_data);
	void do_troop_modify(uint64_t member_id, proto::server::troop_server_data* p_server_data);
	void do_member_update(uint64_t member_id, proto::server::troop_server_data* p_server_data);
	void do_member_online(uint64_t member_id, proto::server::troop_server_data* p_server_data);
	void do_troop_dissolve(uint64_t member_id);
	void do_member_transmission(uint64_t member_id);
	void do_member_zhuagui_task(uint64_t member_id, proto::server::troop_server_data* p_server_data);

	// 更新
	void update_troop_modify(proto::common::troop_data* td);
	// 创建一个伙伴
	void do_create_partner(const troop_member_ptr& p_member, uint64_t scene_uid, uint32_t index);

	// 移动相关
public:
	// 添加队长移动位置
	void leader_move(const scene_ptr& p_scene, uint32_t speed, float x, float y, float direction, uint32_t high);
	void member_fast_move(const scene_ptr& p_scene, uint32_t move_type, float x, float y, float dir);
	void reset_move(float x, float y, float dir);
	void follow_jump_scene(proto::server::scene_change_type change_type, uint64_t scene_id, float des_x = -1, float des_y = -1, float des_dir = 0.f);
	void follow_cross_server(const proto::server::cross_server_data& data);
	// 检查队伍中是否有人在跑商
	bool is_member_trade();
	bool is_troop_trade();

	// 检查队伍中是否有人不能进场景
	uint32_t member_scene_check(const scene_ptr& p_scene);

	// 更新乘骑状态和速度
	void update_mount_state(proto::common::role_mode_state state, uint32_t speed);
	// 更新跑商状态
	bool update_trade_state(const role_ptr& p_role, proto::common::role_mode_state state);
	// 更新队伍速度
	void update_troop_speed(uint32_t new_speed);

	// 设置队伍速度
	uint32_t get_troop_speed() const { return m_troop_speed; }

	// 巡逻
public:
	void update_member_patrol(uint64_t uid, bool is_patrol);

	// 副本相关
public:
	// 开始进行就位
	void start_team_check(uint32_t dungeon_id);
	// 确认就位
	void role_team_check(uint64_t uid, proto::common::team_check_type type);
	// 检测是否所有队员都确认
	void check_team_all();
	// 就位确认结束
	void clear_team_check();
	// 请求谁没有确认
	void get_no_confirm_member(proto::client::gc_ask_team_check_reply& reply);

	// 布阵相关
public:
	// 布阵相关
	troop_formation_manager_t& get_formation_mgr() { return m_formation_mgr; }
	// 广播给队员组队布阵信息
	void notify_formation();
	// 填充组队布阵武将
	bool fill_member_hero_to_formation();

	//藏宝
public:
	//藏宝
	void peek_treasure_data(proto::common::treasure_data* p_treasure_data);
	//放入藏宝道具
	bool use_treasure_item(uint64_t role_uid, uint32_t item_tid);
	//队长开启寻宝
	void open_treasure(uint64_t role_uid);
	//放弃寻宝
	void drop_treasure(uint64_t role_uid);
	//队长挖宝
	void gather_treasure(uint64_t role_uid);
	//寻宝返还邮件ID
	uint32_t get_treasure_mail_id();
	// 尝试生成宝藏
	void try_open_treasure();

	// 远征相关
public:
	bool check_family(uint64_t family_id);

public:
	// 给本队玩家广播消息
	template<typename T_MSG>
	void send_msg_to_all(uint16_t cmd, const T_MSG& protobuf_msg);

protected:
	uint64_t m_uid = 0;
	troop_member_vec m_troop_members;

	uint64_t m_leader_uid = 0;
	bool m_is_free_join = true;
	uint32_t m_type = 0;
	uint32_t m_level_min = 0;
	uint32_t m_level_max = 0;

	// 组队战斗（wsy）
	troop_formation_manager_t m_formation_mgr;

	// 藏宝(fj)
	std::map<uint64_t, uint32_t> m_treasure_item;
	uint32_t m_treasure_id = 0;

	// 就位确认信息(qhj)
	uint32_t m_dungeon_id = 0;		// 就位确认副本id
	uint32_t m_team_check_end = 0;	// 队伍确认结束时间

	// 组队移动相关，保存队长位置
private:
	struct troop_move
	{
		float m_dir = 0.f;
		float m_x = 0.f;
		float m_y = 0.f;
		uint32_t m_high = 0;

		troop_move() {};
		troop_move(float x, float y, float dir, uint32_t h) : m_x(x), m_y(y), m_dir(dir), m_high(h) {};
	};

	static const int32_t TROOP_MEMBER_MAX_COUNT = 3;

	typedef std::deque<troop_move> troop_move_deque;
	troop_move_deque m_moves;

	// 队伍移动速度有队长更新
private:
	uint32_t m_troop_speed = 0;
	proto::common::role_mode_state m_troop_mode = proto::common::role_mode_state_none;
};

template<typename T_MSG>
void troop_t::send_msg_to_all(uint16_t cmd, const T_MSG& protobuf_msg)
{
	for (const auto& p_member : m_troop_members) {
		if (NULL == p_member) continue;
		if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER) continue;
		role_ptr p_role = role_manager_t::find_role(p_member->get_uid());
		if (NULL != p_role) p_role->send_msg_to_client(cmd, protobuf_msg);
	}
}

typedef boost::shared_ptr<troop_t> troop_ptr;

#endif

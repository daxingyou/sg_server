#ifndef __GAME_KING_WAR_SCENE_H__
#define __GAME_KING_WAR_SCENE_H__

#include "scene/scene.hpp"
#include "king_war_common.hpp"

#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

class king_war_scene_t : public scene_t
{
public:
	king_war_scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param);
	virtual ~king_war_scene_t();

	// 初始化放在这里
	virtual bool init();
	// 清空人和生物（删除地图首先调用）
	virtual void clear();
	// 是否是王城
	virtual bool is_king_war_city() { return true; }
	// 获取王城战场景指针
	virtual king_war_scene_ptr get_king_war_scene_ptr() { return boost::dynamic_pointer_cast<king_war_scene_t>(shared_from_this()); }

	// 王城战的一些信息
public:
	// 王城id
	uint32_t get_country_id() const { return m_country_id; }

	// 进入备战期
	void ready_king_war();
	// 开启王城战
	void start_king_war();
	// 王城战结束
	void end_king_war();

	// 得到王城状态
	proto::common::king_war_state get_king_war_state() const { return m_king_war_state; }
	void set_king_war_state(proto::common::king_war_state state) { m_king_war_state = state; }

	// 设置王城战家族
	void set_king_war_family(king_war_side_type type, uint64_t family_id);

	// 得到王城战场景玩家数量
	uint32_t get_scene_family_num(king_war_side_type type);

	// 处理本场景的玩家
	uint32_t king_war_scene_process(role_ptr p_role);

	// 是否是参战家族
	king_war_side_type get_king_war_side_type(uint64_t family_id);

	/*人物相关*/
public:
	virtual void on_role_enter(role_ptr p_role);
	virtual void on_role_leave(role_ptr p_role);

	// 虚函数玩家离开场景中的一个区域的回调
	virtual void role_leave_area(role_ptr p_role, uint32_t area_id);
	// 虚函数玩家进入场景中的一个区域的回调
	virtual void role_enter_area(role_ptr p_role, uint32_t area_id);

	// 复活某玩家
	virtual void on_revive_role(role_ptr p_role);

	// 给场景中的玩家通知龙脉信息
	void update_long_info(proto::client::gc_update_long_info_notify& notify);

	// 全场景同步龙脉信息
	void notify_scene_long_info();

	// 申请能否采集
	uint32_t is_can_collect_artifacts(role_ptr p_role);

	// 采集神器成功
	void role_get_artifacts(role_ptr p_role, uint32_t tid);

	// 使用神器
	bool role_user_artifacts(role_ptr p_role, uint32_t index);

	// 有玩家杀了别人
	virtual void on_role_kill_other(role_ptr p_role, object_position_ptr p_enemy);

	// 自己被别人击杀
	virtual void on_other_kill_me(role_ptr p_role, object_position_ptr p_enemy);

	// 显示复活界面
	virtual void show_revive_ui(role_ptr p_role);

	// 登陆检测要不要给他复活
	virtual void on_login_die(role_ptr p_role);

	// 定时器回调
public:
	void on_ready_timer_call_back(const boost::system::error_code& ec);

	void on_refresh_info_call_back(const boost::system::error_code& ec);

	void on_artifacts_call_back(const boost::system::error_code& ec);

	// 加载场景时加载，不需要清理的信息
protected:
	// 国家id
	uint32_t m_country_id = 0;
	// 复活区信息
	king_war_relive_side m_relive_side[king_war_side_max];

	// 王城战信息
public:
	typedef boost::shared_ptr<king_war_long_pulse> king_war_long_pulse_ptr;
	typedef std::map<uint32_t, king_war_long_pulse_ptr> king_war_long_pulse_map;

	king_war_long_pulse_ptr get_king_war_long_pulse_by_area_id(uint32_t area_id);

protected:
	// 龙脉信息
	king_war_long_pulse_map m_long_pulse_map;
	// 家族战状态
	proto::common::king_war_state m_king_war_state = proto::common::king_war_state_end;
	// 参战家族
	uint64_t m_family[king_war_side_max];
	// 场景中两个家族的玩家id
	std::set<uint64_t> m_family_roles[king_war_side_max];
	// 神器信息
	artifacts_info_t m_artifacts_info;

	// 定时器，注意删场景的时候清除，防止泄露
protected:
	boost::shared_ptr<boost::asio::deadline_timer> m_ready_timer = NULL;	// 准备时间拉人回调

	boost::shared_ptr<boost::asio::deadline_timer> m_refresh_timer = NULL;	// 刷新龙脉信息定时器

	boost::shared_ptr<boost::asio::deadline_timer> m_artifacts_timer = NULL;	// 神器定时器
};

#endif

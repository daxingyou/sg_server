#ifndef __ROBOT_HPP__
#define __ROBOT_HPP__

#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "network/io_base.hpp"
#include "network/tcp_socket.hpp"
#include "protocpp/client.pb.h"
#include "protocpp/common.pb.h"
#include "robot_common/sMisc.hpp"
#include "robot_common/sTime.hpp"
#include "robot_common/robot_common_struct.hpp"
#include "main/robot_server_fwd.hpp"
#include "main/robot_server.hpp"

enum fight_type : uint32_t
{
	FIGHT_DUNGEON = 1,
	FIGHT_PVE = 2,
	FIGHT_PVP = 3,
};

class script_robot_t;

class robot_t : public network::connector_callback_t
              , public boost::enable_shared_from_this<robot_t>
{
public:
    explicit robot_t(uint64_t uid);
	explicit robot_t(const std::string& name);
    ~robot_t();

public:
	// 人物信息
	struct robot_info
	{
		proto::common::role_data role;
		proto::common::item_data item;
		proto::common::hero_data hero;
		proto::common::formation_data formation;
		proto::common::task_data task;
		proto::common::country_bounty_data bounty_data;
		proto::common::arena_role_data arena_data;

		uint64_t scene_id = 0;
		uint32_t map_id = 0;

		uint64_t jump_scene_id = 0;
		uint32_t jump_map_tid = 0;

		// 当前坐标
		spos curr_pos;
		float dir = 0.f;
	};

    virtual const char* name();
    virtual void  on_connect(const network::tcp_socket_ptr& s);
    virtual void  on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
    virtual void  on_close(const network::tcp_socket_ptr& s);
    
    // =========================================================================
    uint64_t get_robot_id() const { return m_robot_id; }
    void set_robot_id(const uint64_t robot_id) { m_robot_id = robot_id; }

    network::tcp_socket_ptr get_socket() const { return m_socket.lock(); }
    
    login_state_em get_login_state() const { return m_login_state; }
    void set_login_state(const login_state_em login_state) { m_login_state = login_state; }

    uint64_t get_uid() const { return m_uid; }
    void set_uid(const uint64_t uid) { m_uid = uid; }
    
    std::string get_guid() const { return m_guid; }
    void set_guid(const std::string& guid) { m_guid = guid; }

	std::string get_session_id() const { return m_session_id;  }
	void set_session_id(const std::string &session_id) { m_session_id = session_id; }

	std::string get_nickname() const { return m_nickname; }
	void set_nickname(const std::string& nickname) { m_nickname = nickname; }

    // =========================================================================
    bool connect(const std::string& ip, uint16_t port);
    bool disconnect();  //just disconnect socket

	void set_login_start_time(uint64_t ntime) { m_login_start_time = ntime; }
	void set_login_success();
	bool heartbeat(uint64_t ntime);

public:
	bool login_account_server();
    bool login_login_server();
    bool login_gate_server(int32_t reconnect);

public:
    void enter_game();
    void create_new_role();

public:
	// robot 在游戏了可以写逻辑函数
	void robot_in_game();
	// 说话
	void talking(proto::common::chat_channel chan, const char* str);
    //gm命令
    bool opt_gm_cmd(std::string cmd);
	// 竞技场
	void arena_join();
	void arena_match();
	void arena_match_reply(proto::client::gc_arena_match_reply &msg);
	void arena_battle_finish(proto::client::gc_arena_battle_result_notify &msg);
	// 是否加入竞技场
	bool is_join_arena();
	void set_join_arena(bool is_join);
	bool is_arena_fighting();

    // 邮件
	void update_mail_list(const proto::client::ec_mail_list_notify& notify);
	void update_mail_content(const proto::client::ec_mail_content_reply& reply);
	void update_mail_state_chage(const proto::client::ec_mail_state_change_notify& notify);
	proto::common::mail_base_info* get_mail_info(uint64_t mail_id);

	//////////////////////////////////////////////////////////////////////////
	// 更新一些人物场景信息
	void update_robot_info(const proto::client::gc_enter_game_reply& msg);
	void update_scene_info(const proto::client::gc_enter_scene_notify& msg);
	void enter_scene(uint64_t scene_id);
	void enter_scene_reply(const proto::client::gc_enter_scene_reply& msg);
	void update_into_vision(const proto::client::gc_role_into_vision_notify& msg);
	void update_out_of_vision(const proto::client::gc_role_outo_vision_notify& msg);
	void update_vision_info(const proto::client::gc_scene_object_info_notify& msg);

	bool is_task_done() { return m_task_done; }

	void game_heart_beat();
	bool is_init_lua() { return m_init_lua; }
	void init_lua();
	void set_open_lua(bool is_open) { m_is_open_lua = is_open; }

    void finish_new_role_guide_notify(uint32_t guide_tid);

	void update_robot_hero_info(const proto::client::gc_enter_game_for_hero_reply& msg);
	void update_robot_item_info(const proto::client::gc_enter_game_for_item_reply& msg);
public:
	// 脚本代理接口
	script_robot_t* get_script_proxy() const;

	const robot_info& get_robot_info() const { return m_robot_info; }

	void move_to_postion(float x, float z);
	void fast_move(float x, float z);
	bool move_arrive(float x, float z);
	void task_done();
	float get_currX();
	float get_currY();
	void talk_about(uint32_t type, const char* str);
	void jump_scene(uint32_t map_tid);
	void jump_id(uint32_t jump_tid);
	void toggle_mount(uint32_t state);

	// 战斗
public:
	// 尝试进入战斗
	bool try_enter_fight();
	// 设置自动战斗
	void try_auto_fight();

	uint64_t get_fight_id() const { return m_fight_uid; }
	void set_fight_uid(uint64_t fight_id);
	uint64_t get_object_id();

	// 邮件
public:
	// 获取邮件列表
	void get_mail_list();
	// 是否有可以打开的邮件
	bool have_new_mail();
	// 打开一份邮件
	void open_one_mail();
	// 随机给自己发邮件
	void random_mail_send_self(uint32_t num);
	// 获得邮件数量
	int32_t get_mail_size();
	// 删除一份邮件
	void del_one_mail();
	// 得到一个有附件的邮件索引
	int32_t get_addenda_mail_index();
	// 得到一个邮件的附件
	void get_mail_addenda(int32_t index);
	// 一键删除所有邮件
	void one_key_del_all();

	// 副本
public:
	// 离开副本
	void try_leave_dungeon();
	// 进度
	void notify_dungeon_schedule(proto::client::gc_dungeon_schedule_data_notify &msg);
	void notify_dungeon_schedule(proto::client::gc_notify_dungeon_result_notify &msg);
	// 副本是否结束
	bool is_all_killed();
	// 得到一个可以战斗的怪物
	int32_t get_fight_index();
	// 尝试请求打开一个副本
	bool try_open_dungeon(uint32_t dungeon_id);
	// 开启副本错误处理
	void open_dungeon_failed(uint32_t back_code);
	// 是否在副本中
	bool is_in_dungeon(uint32_t dungeon_id);
	// 清理视野生物
	void clear_object_info() { m_object_vision_list.clear(); }
	// 清除副本信息
	void clear_dungeon();
	// 战斗人物数据变化
	void fight_end_update_info(proto::client::gc_combat_result_ntf& msg);
	// 快速移动到副本目标附近
	void dungeon_fast_move_to(int32_t index);
	// 是否在某个目标附近
	bool is_near_dungeon_target(int32_t index);

	// 副本信息
private:
	uint32_t m_dungeon_map_id = 0;		//副本地图ID
	uint32_t m_dungeon_id = 0;			//副本ID
	proto::common::dungeon_state_type m_dungeon_state = proto::common::dungeon_state_none;
	std::vector<proto::common::dungeon_schedule_data> m_schedule_datas;
	int32_t m_schedule_index = -1;

	// 视野9屏信息
private:
	std::map<uint64_t, proto::common::object_common_info> m_object_vision_list;

private:
    uint64_t					m_robot_id = 0;
	std::string					m_special_id = "";

    network::tcp_socket_wptr	m_socket;
    login_state_em				m_login_state;
    uint64_t					m_uid = 0;				// 由服务器提供
	std::string					m_guid = "";			// 由服务器提供
	std::string					m_nickname = "";		// 由服务器提供
	std::string					m_session_id = "";		//由服务器提供


	uint64_t					m_curr_time;
	cTimer						m_heart_time;
	bool						m_init_lua;

	robot_info					m_robot_info;	// 保持服务器发过来的信息

	uint64_t					m_login_start_time;	// 开始登陆时间
	bool						m_enter_success;	// 成功登陆
	bool						m_task_done;		// 任务完成
	// 缓存进入场景的操作
	// 由于进入场景的操作在同服务器处理太快，导致gate人物状态还未设置就把进入场景的消息发送过去
    bool						m_delay_enter_scene = false;
    // 延迟登陆场景时间
    uint64_t					m_delay_time = 0;

	proto::client::ec_mail_list_notify m_mail_lists;	// 邮件列表

	bool						m_is_arena_fighting;

	uint64_t					m_fight_uid = 0;
	bool						m_is_auto_fight = false;

private:
	// 是否开启lua
	bool						m_is_open_lua = false;
	// 脚本指针
	script_robot_t*				m_script_proxy = NULL;

public:
	struct robot_delay
	{
		uint32_t id = 0;	/// 序列
		uint64_t sendtime = 0;
		uint64_t receivetime = 0;
	};

	// 压力测试
	typedef boost::shared_ptr<robot_delay> robot_delay_ptr;
	typedef std::vector<robot_delay_ptr> robot_delay_array;
	robot_delay_array m_robot_delay;
	uint32_t m_send_num;
	uint32_t m_recv_num;

	void add_send_one();
	void add_recv_one();
};

typedef boost::shared_ptr<robot_t> robot_ptr;
typedef boost::weak_ptr<robot_t> robot_wptr;


#endif  //__ROBOT_HPP__

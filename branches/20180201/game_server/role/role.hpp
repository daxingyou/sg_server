#ifndef __GAME_ROLE_H__
#define __GAME_ROLE_H__

#include "object/object_position.hpp"
#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "item/item.hpp"
#include "hero/hero.hpp"
#include "formation/formation.hpp"
#include "common.pb.h"
#include "gate/gate_manager.hpp"
#include "item/package_manager.hpp"
#include "trade/role_trade.hpp"
#include "money.hpp"
#include "formation/formation_manager.hpp"
#include "role_scene_info.hpp"
#include "mount/mount.hpp"
#include "task/task_manager.hpp"
#include "task/bounty_manager.hpp"
#include "activity/activity_manager.hpp"
#include "fight_study/fight_study_manager.hpp"
#include "arena/arena.hpp"
#include "common/simple_timer.hpp"
#include "achieve/achieve_manager.hpp"
#include "tower/tower.hpp"
#include "luckydraw/luckydraw.hpp"
#include "fight/role_combat_result.hpp"
#include "dungeon/role_dungeon_data.hpp"
#include "formation/tactic_manager.hpp"
#include "role_buff/role_buff_manager.hpp"
#include "expedition/expedition.hpp"
#include "achieve/active_reward.hpp"
#include "shop/role_random_shop.hpp"
#include "level_reward/role_level_reward.hpp"
#include "level_reward/role_level_reward_mail.hpp"
#include "business_activity/business_activity_manager.hpp"


//typedef boost::shared_ptr<task_manager_t> task_mgr_ptr;
class script_role_t;

enum ROLE_STATE : uint32_t
{
	ROLE_STATE_NONE,
	ROLE_STATE_LOADING,     //��½��ȡ������
	ROLE_STATE_OK          //��½��ȡ���ݳɹ�
};

enum role_job
{
	role_job_qingyun = 3,
	role_job_baihua = 7,
};

enum NEW_ROLE_GUIDE_STATUS : uint32_t
{
    NEW_ROLE_GUIDE_STATUS_DOING         = 1, //�����
    NEW_ROLE_GUIDE_STATUS_READY_DONE    = 2, //׼����ɣ���Ҫ�ȴ��ͻ��˵������Ϣ��������������ɣ�
    NEW_ROLE_GUIDE_STATUS_REALLY_DONE   = 3, //������ɣ��յ��ͻ��˵������Ϣ��Ϊ����Ҫ�ȴ�������������
};

struct new_role_guide_t
{
    uint32_t tid                    = 0;
    bool is_not_waiting             = true; //false--��Ҫ�ȴ��ͻ��˷������״̬��true--ֱ����ɣ�����ȴ�
	uint32_t event_type				= 1;	//1--��������;2--����
    NEW_ROLE_GUIDE_STATUS status    = NEW_ROLE_GUIDE_STATUS_DOING;
};

class role_t : public object_position_t
{
    /*
    logic:  ��ɫ�����������
    author: ervery one
    */
public:
    role_t(uint64_t uid);
    virtual ~role_t();

	virtual void del_clear();

    template<typename T_MSG>
    void send_msg_to_client(uint16_t cmd, const T_MSG& protobuf_msg)
    {
        const network::tcp_socket_ptr& p_gate_socket = gate_manager_t::get_gate_socket_by_id(m_gate_id, m_server_id);
        if (NULL == p_gate_socket)
        {
            log_error("NULL == p_gate_socket: send cmd[%d]", cmd);
            return;
        }
        env::server->send_msg_to_gate(p_gate_socket, cmd, m_uid, protobuf_msg);
    }

	template<typename T_MSG>
	void send_msg_to_gate(uint16_t cmd, const T_MSG& protobuf_msg)
	{
		const network::tcp_socket_ptr& p_gate_socket = gate_manager_t::get_gate_socket_by_id(m_gate_id, m_server_id);
		if (NULL == p_gate_socket)
		{
			log_error("NULL == p_gate_socket [%u][%u]", m_gate_id, m_server_id);
			return;
		}

		env::server->send_msg_to_gate(p_gate_socket, cmd, m_uid, protobuf_msg);
	}

    virtual role_ptr get_role() { return boost::dynamic_pointer_cast<role_t>(shared_from_this()); } // ��ȡ��ɫָ��
    virtual proto::common::EM_SCENEOBJECT_TYPE get_object_type() { return proto::common::SCENEOBJECT_USER; }
    virtual bool is_role() { return true; } // �Ƿ��ǽ�ɫ
    virtual void get_object_info(proto::common::object_common_info* object_info);

	void load_role_data(const proto::common::role_data& rd);
	void load_cross_data(const proto::common::role_data& rd);

    void peek_all_datas(proto::client::gc_enter_game_reply* egn);
    void peek_data(proto::common::role_data* data, bool is_to_client);
    void peek_global_user_data(proto::server::global_user_data* data);
	void peek_cross_data(proto::common::role_cross_data* data, proto::common::role_cross_type type);
	void peek_fight_data(proto::common::mirror_fight_data* data);

    void save_all_datas(bool is_imm_save = false);
    void save_self(bool is_right_now = false);
    void notify_save_data();
    void unify_save_data();
	bool is_new_role() { return m_is_new_role; }

    void get_personal_info(proto::common::personal_info* personal);
    void set_personal_info(const proto::common::personal_info& personal);

	//////////////////////////////////////////////////////////////////////////
	// �Ƿ��ǵ�½
	bool is_login_scene() const { return m_login_type != proto::server::scene_change_none; }
	// ���õ�½����
	void set_login_type(proto::server::scene_change_type type) { m_login_type = type; }

	// �������ķ��·�����
	void set_center_user_data(const proto::server::eg_reg_user_scene_request& data);

	// ��ʼ���볡��
	bool reg_scene();

	// ���ض�������
	void load_all_datas(proto::server::dg_role_data_reply const& rdr);

	// �·��������ݣ���½����
	bool do_login_scene(const scene_ptr& p_login_scene);

	// ׼����½����
	void ready_enter_scene();

	// �ǳ�
	void leave_game();

	// ����
	void offline();

	// ֪ͨ������������½����
	void notify_other_server_ready_enter_scene();

	// ֪ͨ���������������½
	void notify_other_server_role_login_scene();

	// ֪ͨ����������ɾ������
	void notify_the_other_servers_del_role();

	//////////////////////////////////////////////////////////////////////////

	void notify_new_guide(); //zwx

    void init_new_role_datas(uint32_t role_type); //�½�ɫ��ʼ��

    inline const std::string& get_key() { return m_key; }

    inline uint64_t get_uid() { return m_uid; }

    inline std::string get_name() { return m_name; }
    inline void set_name(std::string name) { m_name = name; }

    inline void set_gm(uint32_t gm) { m_gm = gm; }
    inline bool is_gm() { return m_gm >= 1 ? true : false; }
    inline bool is_super_gm() { return m_gm >= 2 ? true : false; }

    inline ROLE_STATE get_state() const { return m_state; }
    inline void set_state(ROLE_STATE st) { m_state = st; }

    inline uint32_t get_gate_id() const { return m_gate_id; }
	inline void set_gate_id(uint32_t val, uint32_t server_id) { m_gate_id = val; set_server_id(server_id); }

	inline uint32_t get_server_id() const { return m_server_id; }
	inline void set_server_id(uint32_t val) { m_server_id = val; }
	void syn_cross_id(uint32_t cross_id);

    inline uint32_t get_create_time() const { return m_create_time; }

    inline void set_last_login_time(uint32_t val) { m_last_login_time = val; }
    inline uint32_t get_last_login_time() const { return m_last_login_time; }

    void set_last_logout_time(uint32_t logout_time);
    inline uint32_t get_last_logout_time() const { return m_last_logout_time; }

    inline void set_online_time(uint32_t val) { m_online_time = val; }
    inline uint32_t get_online_time() const { return m_online_time; }

    inline void set_cumulative_login_days(uint32_t val) { m_cumulative_login_days = val; }
    inline uint32_t get_cumulative_login_days() const { return m_cumulative_login_days; }

    inline void set_continue_login_days(uint32_t val) { m_continue_login_days = val; }
    inline uint32_t get_continue_login_days() const { return m_continue_login_days; }

    inline void set_online_state(ROLE_ONLINE_STATE val) { m_online_state = val; }
    inline ROLE_ONLINE_STATE get_online_state() const { return m_online_state; }

    inline void set_role_type(uint32_t role_type) { m_role_type = role_type; }

    inline script_role_t* get_script_role() const { return m_script_role; }

	// ���죨�Ƿ���ܣ�
    void on_next_day(uint32_t curr_time, bool is_pass_week);
	// ���촦���Ƿ������ߴ����ǵ�½����
    void do_next_day(uint32_t curr_time, bool is_online);
	// ���ܴ���
    void do_next_week(uint32_t curr_time);

    // ����ȼ�
    uint32_t get_exp() { return m_exp; }
    uint32_t get_level() { return m_level; }
    bool add_exp(uint32_t add_val, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data = NULL);
    void set_exp(uint32_t set_val, uint32_t source_typ, uint32_t source_param, proto::common::role_change_data* p_data = NULL);
    void set_level(uint32_t set_val, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data = NULL);

    // ��ʵ��
    uint32_t get_hp() { return 0; }
    uint32_t get_max_hp() { return 0; }

    // ���µ�½ʱ��
    void update_login_time();

    //��ʼ����������
    void init_new_role_guide(uint32_t guide_tid, bool is_not_waiting,uint32_t event_type);
    //����������� ���Խű�
    void finish_new_role_guide_from_script(uint32_t guide_tid);
    //����������� ���Կͻ���
    void finish_new_role_guide_from_client(uint32_t guide_tid);
    //�ű��¼�������
    void script_event_level_up(uint32_t level);
    //�ű��¼�����������
    void script_event_create_item(uint32_t item_itd);
    //�ű��¼�����������
    void script_event_enter_area(uint32_t area_itd);
	//�ű��¼�������ָ�������ָ���غ�(����ID���غ���)
	void script_event_enter_fight(uint32_t fight_tid, uint32_t round_tid);
	//�ű��¼�������ָ�������ָ���غϲ���ս��(����ID���غ���)
	void script_event_ready_fight(uint32_t fight_tid, uint32_t round_tid);
	//�ű��¼���ʹ��ָ������(����ID)
	void script_event_use_item(uint32_t item_tid);
	//�ű��¼�: �½�ɫ���볡��
	void script_event_new_role_enter_scene();
	//�ű��¼�: ���һ��������������(���ڴ��������������¼�)
	void script_event_finish_new_guide(uint32_t guide_tid);

	bool init_new_guide();

	void init_gm_level();
private:
    std::string m_key = "";                 //cache key
    uint64_t m_uid = 0;                     //��ɫΨһID
    ROLE_STATE m_state = ROLE_STATE_NONE;   //��ɫ״̬
    std::string m_name = "";                //��ɫ��
    int32_t m_gm = 0;                       //�ý�ɫ�Ƿ���gmȨ��
    bool m_is_need_save = false;            //�����Ƿ񱣴�
    uint32_t m_create_time = 0;             //��ɫ����ʱ��
    uint32_t m_last_login_time = 0;         //��ɫ��һ�εĵ�½ʱ��
    uint32_t m_last_logout_time = 0;        //��ɫ��һ�εĵǳ�ʱ��
    uint32_t m_online_time = 0;             //��ɫ����ʱ��
    uint32_t m_gate_id = 0;                 //��ɫ���ڵ�gate_server��id
	uint32_t m_server_id = 0;				//���ʱserver_id��һ���Ǳ���server_id
    script_role_t* m_script_role = NULL;    //��ɫ�Ľű�ָ��
    uint32_t m_mode_id = 0;                 //��ɫģ��ID
    proto::common::ROLE_MODE m_role_mode = proto::common::ROLE_MODE_USER;   //��ɫģ��״̬
	ROLE_ONLINE_STATE m_online_state = ROLE_ONLINE_STATE_LOGOUT;               //��ɫ�Ƿ�����(������ɾ������)
	bool m_is_new_role = false;             //�Ƿ������û�
    uint32_t m_role_type = 0;               //������ɫ�����HeroJobTransfer���key
	uint32_t m_exp = 0;						//����
	uint32_t m_level = 0;					//�ȼ�
    uint32_t m_continue_login_days = 0;		//������½����
    uint32_t m_cumulative_login_days = 0;	//�ۼƵ�½����
	uint32_t m_update_day_time = 0;         //��ɫ��һ�ο��촦���ʱ��
	uint32_t m_update_week_time = 0;        //��ɫ��һ�ο��ܴ����ʱ��
    std::map<uint32_t, new_role_guide_t> m_new_role_guide_list; //���������б�
	proto::server::scene_change_type m_login_type = proto::server::scene_change_none; // ������½����

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
    logic:  ����ϵͳ
    author: ����
    */
public:
    inline task_mgr_ptr get_task_mgr(){ return m_task_mgr;}
	inline bounty_mgr_ptr get_bounty_mgr() { return m_bounty_mgr; }
	inline activity_mgr_ptr get_activity_mgr() { return m_activity_mgr; }
	void script_event_task_accept(uint32_t task_id);
	void script_event_task_finish(uint32_t task_id);
	void accept_sync_task(const proto::common::task_state &task_info, proto::common::role_change_data &rcd);
	void drop_circle_task(uint32_t task_type);
private:
    task_mgr_ptr m_task_mgr;
	bounty_mgr_ptr m_bounty_mgr;
	activity_mgr_ptr m_activity_mgr;
    /*
    logic:  ����ϵͳ
    author: ����
    */
public:
	formation_manager_t m_formation_mgr;
    /*
    logic:  ս��ϵͳ(�غ���pve\pvp����ʱ��)
    author: ����
    */
    /*
    logic:  ս�����㻺��
    author: ��ɽ��
    */
	role_combat_result m_combat_result;
public:
//     void set_fight_data(const proto::common::fight_data& data); //����ս������ wsy ע�� �ɵ�ս��
//     proto::common::fight_data& get_fight_data();                //��ȡս������ wsy ע�� �ɵ�ս��
	uint32_t get_fighting() { return m_fighting; }                                   //???
	void calc_fighting();
	uint64_t get_fight_uid() { return m_role_fight_data.fight_uid(); }
	void set_fight_uid(uint64_t uid);
	bool can_enter_fight();
	void check_update_fighting(uint64_t hero_uid);
	bool get_auto_fight_flag();
	void save_auto_fight_flag(uint32_t auto_fight_flag);
	uint64_t get_watching_fight_uid() { return m_role_fight_data.watching_fight_uid(); }
	uint32_t set_watching_fight_uid(uint64_t uid, bool need_delete=true);
    proto::common::role_fight_data& get_role_fight_data() { return m_role_fight_data; }

	void combat_notify_round(uint64_t against_id, uint32_t round);
	void combat_notify_prepare(uint64_t against_id, uint32_t round);
public:
	fight_study_manager_t m_fight_study_mgr;

private:
//    proto::common::fight_data		m_fight_data;		// ���ڿͻ��˼����ս������ wsy ע�� �ɵ�ս��
	proto::common::role_fight_data	m_role_fight_data;	// �°�ս������
	uint32_t m_fighting = 0;
    /*
    logic:  ����ϵͳ
    author: ��ɽ��
    */
public:
    inline uint32_t get_chat_channel() { return m_chat_channel; }
    inline void set_chat_channel(uint32_t val) { m_chat_channel = val; }
private:
    uint32_t m_chat_channel = 0;    //��ǰ����Ƶ��
	uint32_t m_luck_gift_gold = 0;  //�����������������

    /*
    logic:  ����ϵͳ
    author: ��ɽ��
    */
public:
    inline role_trade_t& get_trade_info() { return m_trade; }
    inline void set_city_id(uint32_t city_id) { m_city_id = city_id; }
    uint32_t get_city_id() { return m_city_id; }
private:
    role_trade_t m_trade;
    uint32_t m_city_id = 0; //��ǰ���ڵĳ���

	/*
	logic: ����
	author: ��ɽ��
	*/
public:
    inline mount_data_ptr get_mount_data() { return m_mount_data; }
    inline mount_ptr get_cur_use_mount() 
    { 
        if (NULL != m_mount_data)
        {
            return m_mount_data->get_use_mount();
        }
        return mount_ptr();
    }

private:
	mount_data_ptr m_mount_data = mount_data_ptr();

    /*
    logic:  ���߱���ϵͳ
    author: ��ɽ��
    */
public:
    package_manager_t  m_package_manager;

	/*
	logic:  ������ϵͳ
	author: ��ɽ��
	*/
public:
	arena_t& get_arena() { return m_arena; }
private:
	arena_t  m_arena;

	/*
	logic:  װ��ϵͳ
	author: ����
	*/
public:
	uint32_t get_equip_smelt_value() { return m_equip_smelt_value; }
	void add_equip_smelt_value(uint32_t val) { m_equip_smelt_value += val; }

	void add_equip_smelt_box(uint32_t box_id) { m_equip_smelt_box_vec.push_back(box_id); }
	const std::vector<uint32_t>& get_equip_smelt_box_vec() { return m_equip_smelt_box_vec; }

	uint32_t get_equip_lucky() { return m_equip_lucky; }
	void set_equip_lucky(uint32_t val) { m_equip_lucky = val; }
private:
	std::vector<uint32_t>  m_equip_smelt_box_vec;
	uint32_t m_equip_smelt_value = 0;
	uint32_t m_equip_lucky = 0;
	/*
	logic:  ��ɫ״̬
	author: ��ɽ��
	*/
public:
	enum speed_update_type
	{
		speed_change_update = 0,	// �޸ĲŸ���
		speed_only_set = 1,			// ֻ���ò�����
		speed_forced_update = 2,	// ǿ�Ƹ��£�������û�ı��ٶ�
	};
	/**
	* \brief �ٶ����
	*/
	void change_real_speed(speed_update_type type = speed_change_update);
	void update_troop_mount(proto::common::role_mode_state team_state, uint32_t speed);
	void update_troop_speed(uint32_t speed);

	// �õ��ٶ�
	uint32_t get_speed_by_type(proto::common::role_mode_state state);

	/**
	 * \brief �ı�״̬
	 * \brief state_param ״̬��������
	 */
	uint32_t change_object_state(proto::common::object_state_type new_state, uint64_t state_param = 0);
	// ״̬��������
	uint64_t get_object_state_param() const { return m_obj_state_param; }
	// �뿪״̬
	void on_leave_object_state(proto::common::object_state_type old_state);

	// �ͻ������������������
	uint32_t toggle_ride_mount(proto::common::role_mode_state new_state);
	// ��������״̬
	uint32_t toggle_trade_state(proto::common::role_mode_state new_state);

	proto::common::role_mode_state get_mode_state() const { return m_mode_state; }
	
	// ��������
	virtual void on_object_die();
	
	// ������ص�
	virtual void on_object_revive();

	// ���ø�����
	inline void set_revive_time(uint32_t val) { m_revive_time = val; }

	// ��ø����ʱ��
	uint32_t get_revive_time() const { return m_die_time + m_revive_time; }

	// ���ô��Ͷ���
	uint32_t start_transmission(proto::common::transmission_type type = proto::common::transmission_type_user);
	uint32_t get_transmission_type() const { return m_transmission_type; }

	// �Ƿ��ڴ���
	bool is_troo_transmission() const { return m_transmission_type == proto::common::transmission_type_troop; }
	// ����ȡ�����Ͷ���
	void cancel_transmission();
	// ��϶���
	void do_break_transmission();
	// ��϶�������
	void on_break_transmission(bool is_notify = true);

	// ���Ͷ�ʱ������
	void on_transmission_timer();
	// ���Ͷ�ʱ������
	static void transmission_callback(const boost::weak_ptr<role_t>& p_wrole, const boost::system::error_code& ec);

private:
	proto::common::role_mode_state m_mode_state = proto::common::role_mode_state_normal;	// ��ɫ
	uint64_t m_obj_state_param = 0;		// ״̬��������
	uint32_t m_transmission_type = proto::common::transmission_type_none;
	boost::shared_ptr<boost::asio::deadline_timer> m_transmission_timer;

	// ��������ʱ��
	uint32_t m_die_time = 0;
	// ���ø���ʱ��
	uint32_t m_revive_time = 0;

    /*
    logic:  �佫ϵͳ
    author: ����
    */
public:
    inline uint64_t get_main_hero_uid() { return m_main_hero_uid; }
	inline void set_main_hero_uid(uint64_t uid) { m_main_hero_uid = uid; }
	inline uint32_t get_main_hero_tid() { return m_main_hero_tid; }
	void set_main_hero_tid(uint32_t tid);
	inline uint32_t get_main_hero_plugin() { return m_main_hero_plugin; }
	void set_main_hero_plugin(uint32_t plugin);
    inline uint32_t get_job() { return m_job; }
	inline void set_job(uint32_t val) { m_job = val; }
private:
    uint64_t m_main_hero_uid = 0; //���ڽ�ɫ��Ӣ��
	uint32_t m_main_hero_tid = 0;
	uint32_t m_main_hero_plugin = 0;
	uint32_t m_job = 0;	//���ְҵ�������佫��ְҵ��
public:
    hero_map m_hero_list;

    /*
    logic:  ����ϵͳ
    author: ����
    */
public:
	inline money_ptr get_money() { return m_money; }
private:
	money_ptr m_money;
	
	/*
	logic:  �鿨ϵͳ
	author: ����
	*/
public:
	inline luckydraw_ptr get_luckydraw() { return m_luckydraw; }
private:
	luckydraw_ptr m_luckydraw;

    /*
    logic:  ��������
    author: Ǯ����
    */
public:
	// �ƶ���������
	virtual void move_to(float x, float y, float time_stamp, float direction);
    // ���볡��
    virtual void on_enter_scene(scene_ptr p_scene);
    // �뿪����
    virtual void on_leave_scene(scene_ptr p_scene);
    // �ƶ�����
    virtual void on_move_event(scene_ptr p_scene);
	// ������
	void do_something_for_area(scene_ptr p_scene);

    // ���°���
    void update_area_trigger();
    // ֪ͨ��Χ����Լ��ĳ�����Ϣ
    void notify_object_common_info_for_nearbies();
	// �õ��������id
	uint32_t get_role_area_id() { return m_role_scene.get_current_area_tid(); }
	// �õ����������Ϣ
	const role_scene_t& get_role_scene_info() { return m_role_scene; }
	// �ͻ��˻�ȡ������Ϣ
	void role_enter_scene(uint64_t scene_id, proto::client::gc_enter_scene_reply& reply);

protected:
	role_scene_t m_role_scene;

    /*
    logic:  ���ϵͳ
    author: qhj
    */
public:
    uint64_t get_troop_id() const { return m_troop_id; }
	bool is_leader() const { return m_is_leader; }
	uint32_t get_troop_state() const { return m_troop_state; }

	uint32_t can_create_troop();
	uint32_t can_join_troop();
	// �ж��Ƿ��Ƕ�Ա������û����
	bool is_troop_member_not_leave() const { return m_troop_id != 0 && !m_is_leader && m_troop_state == proto::common::troop_state_normal; }
	// ������Ϣ����
	void update_troop_info(uint64_t troop_id, uint32_t state = 0, bool is_leader = false);

private:
    uint64_t m_troop_id = 0;
	bool m_is_leader = false;
	uint32_t m_troop_state = proto::common::troop_state_normal;

    /*
    logic:  ����ϵͳ
    author: �����
    */
public:
	void join_family(uint64_t family_id, const std::string& family_name, uint32_t officer, uint32_t country_id);
	void leave_family();

    uint64_t get_family_id() const { return m_family_id; }
    const std::string& get_family_name() { return m_family_name; }

	void set_family_officer(uint32_t val);
	uint32_t get_family_officer() const { return m_family_officer; }

	uint32_t get_country_id() const { return m_country_id; }
	void set_country_id(uint32_t country_id);
	void on_join_country();
	void set_country_officer(uint32_t val);
	uint32_t get_country_officer() const { return m_country_officer; }

	/*
		��Ӫ�����
	*/
	business_activity_manager_t &get_business_act_mgr() { return m_business_act_mgr; };
	business_activity_manager_t  m_business_act_mgr;
private:
    uint64_t m_family_id = 0;
    std::string m_family_name = "";
	uint32_t m_family_officer = 0;
	uint32_t m_country_id = 0;
	uint32_t m_country_officer = 0;
    /*
    logic:  PKģʽ״̬
    author: ����
    */
public:
	uint32_t get_errantry() { return m_errantry; }
	void set_errantry(uint32_t val) { m_errantry = val; }
	void add_errantry(uint32_t val, uint32_t source_type, uint32_t source_param);

	uint32_t get_pk_count() { return m_pk_count; }
	void set_pk_count(uint32_t val) { m_pk_count = val; }
	uint32_t get_target_count() { return m_target_count; }
	void set_target_count(uint32_t val) { m_target_count = val; }
	uint32_t get_pk_value() { return m_pk_value; }
	void set_pk_value(uint32_t val);
	void add_pk_value(uint32_t val, uint32_t source_type, uint32_t source_param);
	void del_pk_value(uint32_t val, uint32_t source_type, uint32_t source_param);
	//��ΪPK��������PKֵҪ��ս����ʼ����һ�̼��㣬����Ҫ��¼����
	uint32_t get_calc_pk_value() { return m_calc_pk_value; }
	void record_calc_pk_value() { m_calc_pk_value = m_pk_value; }
    inline proto::common::EM_PK_MODE get_pk_mode() { return m_pk_mode; }
    inline uint32_t get_pk_punish_time() { return m_pk_punishment_time; }
    bool change_pk_mode(proto::common::EM_PK_MODE mode); //�ı�PK״̬
	//�������
	void move_to_prison();
	// ����PKֵ��ʱ������
	void on_reduce_pk_value_timer();
	// ������ʱ������
	void on_prison_timer();
	// �Ƿ��ڻʳ�
	bool is_in_king_city();
	// ����PKֵ��ʱ������
	static void on_reduce_pk_timer(const boost::weak_ptr<role_t>& p_wrole, const boost::system::error_code& ec);
	// ������ʱ������
	static void on_prison_timeout(const boost::weak_ptr<role_t>& p_wrole, const boost::system::error_code& ec);

public:
	//��Ӽ����ʽ� 
	static void add_family_money(role_ptr &p_role, uint32_t money);
	//��ӹ����ʽ�
	static void add_country_money(role_ptr &p_role, uint32_t money);
private:
	uint32_t m_pk_value = 0;
	uint32_t m_calc_pk_value = 0;
    proto::common::EM_PK_MODE m_pk_mode = proto::common::PK_PEACE; //PKģʽ
    uint32_t m_pk_punishment_time = 0; // �����ͷ�ʱ��
	uint32_t m_pk_value_reduce_time = 0; //��һ�ν���PKֵ��ʱ��
	uint32_t m_pk_count = 0; //ʣ�๥������
	uint32_t m_target_count = 0; //ʣ�౻��������
	uint32_t m_errantry = 0; //����ֵ
	uint32_t m_fight_end_time = 0;	// ս������ʱ��(�����ж�״̬)
	boost::shared_ptr<boost::asio::deadline_timer> m_pk_reduce_timer;
	boost::shared_ptr<boost::asio::deadline_timer> m_pk_punish_timer;
	/**
	 * \brief ����ս
	 */
public:
	bool is_in_king_war() const { return m_king_war_warfare > 0; }
	void set_king_war_warfare(uint32_t val) { m_king_war_warfare = val; }
	uint32_t get_king_war_warfare() const { return m_king_war_warfare; }

private:
	uint32_t m_king_war_warfare = 0;	// ս��ֵ�������ֵ��ʾ��������ս��

	/**
	 * \brief ����ս
	 */
public:
	bool is_in_family_war() const { return m_is_in_family_war; }
	void set_in_family_war(bool val);
	void set_family_war_heros(std::set<uint64_t>& heros);
	void add_family_war_hero(uint64_t hero_uid);
	void del_family_war_hero(uint64_t hero_uid);
	bool is_hero_active(uint64_t hero_uid);
	bool have_hero_live() { return m_family_war_hero.size() > 0; }
	const std::set<uint64_t>& get_family_war_hero() { return m_family_war_hero; }
private:
	bool m_is_in_family_war = false;	// �Ƿ��ڼ���ս
	std::set<uint64_t> m_family_war_hero;

	/*
	*@ logic:  �ɾ�ϵͳ
	*@ author: ����ѧ
	*/
public:
	std::map<uint32_t, achieve_ptr>& get_achieve_map() { return m_achieve_map; }
	inline uint32_t get_achieve_level() { return m_achieve_level; }
	inline void set_achieve_level(uint32_t val) { m_achieve_level = val; }
	inline uint32_t get_achieve_recv_reward_level() { return m_achieve_recv_reward_level; }
	inline void set_achieve_recv_reward_level(uint32_t val) { m_achieve_recv_reward_level = val; }

	//inline achieve_mgr_t get_achieve_mgr() { return m_achieve_mgr; }
private:
	std::map<uint32_t, achieve_ptr>	m_achieve_map;
	uint32_t m_achieve_level = 1;	//�ɾ͵ȼ�
	uint32_t m_achieve_recv_reward_level = 0;	//�ɾ���ȡ�����ȼ� ��ʾ��ȡ���ڼ���
	
	/*
	logic:  ����ϵͳ
	author: ����
	*/
public:
	inline tower_ptr get_tower() { return m_tower; }
	
	inline void clear_tower_timer() { m_tower->del_auto_fight_timer(); }
private:
	tower_ptr m_tower;
	
	/*
	logic:  Զ��ϵͳ
	author: ����
	*/
public:
	inline expedition_ptr get_expedition() { return m_expedition; }

private:
	expedition_ptr m_expedition;
	/*
	*@ logic:  ����
	*@ author: qhj
	*/
public:
	inline role_dungeon_data_t& get_dungeon_data() { return m_role_dungeon_data_t; }

private:
	role_dungeon_data_t m_role_dungeon_data_t;

	/*
	*@ logic:  ����̵�
	*@ author: qhj
	*/
public:
	inline role_random_shop_t& get_random_shop_data() { return m_role_random_shop; }

private:
	role_random_shop_t m_role_random_shop;
	
	/*
	 *@ logic:  ����  
	 *@ author: ����ѧ 
	 */

public:
	void reset_lifestar_level() { m_lifestar_level = 0; }		//��������
	bool level_up_lifestar();									//��������
	bool level_up_lifestate(uint32_t& recode);					//��������
	bool level_up_lifelabel();									//��ʦ�ƺ�����

	uint32_t get_lifestar_level() { return m_lifestar_level; }
	uint32_t get_lifestate_level() { return m_lifestate_level; }
	uint32_t get_lifeforce_label() { return m_lifeforce_label; }
private:
	uint32_t m_lifestar_level = 0;	//���ǵȼ�(��ʼ�� 0)
	uint32_t m_lifestate_level = 1;	//����ȼ�(��ʼ�� 1)
	uint32_t m_lifeforce_label = 0;	//��ʦ�ƺ�

	/*
	 *@ logic:  ���ܴ���
	 *@ author: ����
	*/

public:
	//�����Ƿ���
	bool is_function_open(uint32_t func);
	//���ܿ���
	void open_function(uint32_t func);
	//���ܹر�
	void close_function(uint32_t func);
	//�������й���
	void open_all_function();
private:
	uint32_t m_function_code = 0;	//���ܴ���(��ʼ�� 0)

	/*
	 *@ logic:  �󷨴���
	 *@ author: ����
	*/

public:
	inline tactic_mgr_ptr get_tactic_mgr() { return m_tactic_mgr; }
private:
	tactic_mgr_ptr m_tactic_mgr;

	/*
	*@ logic:  BUFF����
	*@ author: ����
	*/

public:
	inline role_buff_mgr_ptr get_role_buff_mgr() { return m_role_buff_mgr; }
private:
	role_buff_mgr_ptr m_role_buff_mgr;

	/*
	*@ logic:  npc��ʾ����
	*@ author: qhj
	*/

public:
	void set_npc_code(uint32_t func);
private:
	uint32_t m_npc_code = 0;	//���ܴ���(��ʼ�� 0)

	/*
	*@ logic:  �ɼ�����ʾ����
	*@ author: qhj
	*/

public:
	void set_collect_code(uint32_t func);
private:
	uint32_t m_collect_code = 0;	//���ܴ���(��ʼ�� 0)

	/*
	*@ logic:  �½ڴ���
	*@ author: zwx
	*/
public:
	std::set<uint32_t>& get_page_list() { return m_page_list; }
	std::map<uint32_t, uint32_t>& get_page_task_map() { return m_page_task_map; }
	std::map<uint32_t, uint32_t>& get_page_elite_map() { return m_page_elite_map; }
	std::set<uint32_t>&	get_page_elite_open_list() { return m_page_elite_open_list; }
	std::map<uint32_t, uint32_t>&	get_quick_elite_page_map() { return m_quick_elite_page_map; }
	std::map<uint32_t, uint32_t>&	get_reset_quick_elite_page_map() { return m_reset_quick_elite_page_map; }
private:

	std::set<uint32_t>					m_page_list;		///�Ѿ�ͨ�ص��½�ID

	std::map<uint32_t, uint32_t>		m_page_task_map;	///�½�����ͨ�ؽ����б� {�½������tid ��ȡ״̬(1:��ȡ 0:δ��ȡ)}

	std::map<uint32_t, uint32_t>		m_page_elite_map;	///�½ھ�Ӣ�����б�{��Ӣ����ID  ��ȡ״̬(1:��ȡ 0:δ��ȡ)}

	std::set<uint32_t>					m_page_elite_open_list; ///�½ھ�Ӣ�����б�(��ս����)

	std::map<uint32_t, uint32_t>		m_quick_elite_page_map;		//��Ӣ������ɨ������

	std::map<uint32_t, uint32_t>		m_reset_quick_elite_page_map;	//��Ӣ���������ô���
	/*
	*@ logic:  ��Ծ����(ÿ��ˢ�µĵ��ճɾ�)
	*@ author: zwx
	*/
public:
	std::map<uint32_t, active_reward_ptr>& get_active_reward_map() { return m_active_reward_map; }

	std::set<uint32_t>&		get_active_reward_task_list() { return m_daily_task_list; }

private:
	///��Ծ��������(��DB)
	std::map<uint32_t, active_reward_ptr>		m_active_reward_map;


	///ÿ��̶������б�
	std::set<uint32_t>							m_daily_task_list;
public:


	///��ֵ���
	void		add_total_cny(uint32_t val, proto::common::role_change_data* p_data);
	uint32_t	get_total_cny() { return m_total_cny; }

	void		set_vip_level(uint32_t vip_level, proto::common::role_change_data* p_data);
	uint32_t	get_vip_level() { return m_vip_level; }

	bool		is_first_recharge();
	void		add_recharge_count();
	uint32_t	get_recharge_count() { return m_recharge_count; }
	uint32_t	get_recharge_reward_flag() { return m_recharge_reward_flag; }

	bool		already_get_recharge_gift(uint32_t bit = 0);
	void		set_recharge_reward_flag(uint32_t bit = 0);
	uint32_t	get_recharge_gift(proto::common::role_change_data* p_data);
private:
	uint32_t	m_vip_level = 0;				///vip�ȼ�
	uint32_t	m_total_cny = 0;				///�ۼƳ�ֵ���
	uint32_t	m_recharge_count = 0;			///��ֵ����(�׳� �γ�)
	uint32_t	m_recharge_reward_flag = 0;		///��ֵ������ȡ���λ(�׳� �γ�)

		/*
	*@ logic:  Ѳ��
	*@ author: fangjian
	*/
public:
	uint32_t	patrol_request(uint32_t map_id, uint32_t patrol);
	bool		set_patrol(bool val);
	void		troop_patrol(bool val);
	bool		is_patrol() { return m_role_scene.is_patrol_state(); }

	uint32_t	get_patrol_count() { return m_patrol_count; }
	void		set_patrol_count(uint32_t val) { m_patrol_count = val; }
	uint32_t	get_patrol_exp();
private:
	uint32_t	m_patrol_count = 0;

	/*
	*@ logic:  ��������
	*@ author: fangjian
	*/
public:
	void		update_offline_data(const proto::common::offline_role_data& ord);

	/**
	 * \brief �ȼ�����
	 * \auther qhj
	 */
public:
	inline role_level_reward_t& get_level_reward_data() { return m_role_level_reward; }
	uint32_t get_level_reward(uint32_t id, proto::common::role_change_data* p_data);

private:
	role_level_reward_t m_role_level_reward;

	/*
	*@ logic:  ���
	*@ author: fangjian
	*/
public:
	//֪ͨ�쵽���
	void draw_notify(uint32_t money);
	//�����ͺ��
	uint32_t send_redbag(uint32_t count, uint32_t money, proto::common::role_change_data* p_data);

	static uint32_t get_draw_min();

	static uint32_t get_draw_max();

	static proto::common::MONEY_TYPE get_redbag_money_type();

	/*
	 *@ �����౶����
	 *@ auther zwx
	 */
public:
	std::map<uint32_t, uint32_t>& get_multi_time_map() { return m_multi_time_list; }
	bool get_multi_state() { return m_multi_state; }
	void set_multi_state(bool state = false) { m_multi_state = state; }


public:
	/* 
	*@ logic:  �ȼ������ʼ�
	*@ author: huangyang
	*/
	void check_send_level_reward_mail();

private:
	role_level_reward_mail_t m_role_level_reward_mail;

private:
	///�౶���� ����ʱ��(ʱ���) ���б�ĿǰԼ�����������ͬ����Ķ౶����
	std::map<uint32_t, uint32_t>	m_multi_time_list;

	bool m_multi_state = false;

	// �������
public:
	// �õ���Ҿ���ֵ
	uint32_t get_vigour();
	// ��Ҿ�����������
	uint32_t get_vigour_base_limit();
	// ��Ҿ����������
	uint32_t get_vigour_max_limit();
	// �õ�������������
	uint32_t get_max_buy_times();
	// ���Ӿ���ֵ(��������ӿ�֮ǰ�ȵ���get_vigour�������������µľ���ֵ)
	void add_vigour(uint32_t val, uint32_t source_type, uint32_t source_param = 0);
	// �۳�����ֵ
	bool user_vigour(uint32_t val, uint32_t source_type, uint32_t source_param = 0);
	// �õ��Ѿ�����
	uint32_t get_vigour_buy_times() const { return m_vigour_buy; }
	// ���ӹ������
	void add_vigour_buy_times(uint32_t val = 1);
	// ��վ����������
	void clear_vigour_buy_times(bool is_notify = true);

protected:
	uint32_t m_vigour = 0;					// ���Ǿ���(һ��Ҫ�и���ʼֵ����Ȼ�����Զ����ӣ��Զ����ӿ���ʱ�ڿ۵�ʱ��)
	uint32_t m_vigour_buy = 0;				// ���Ǿ����������
	uint32_t m_vigour_time = 0;				// �����ϴξ�������ʱ��
	uint32_t m_next_add_time = 0;			// �����´ξ�������ʱ��

public:

	///ͬ���佫�Լ�װ����Ϣ��center 
	void update_role_info_to_center(proto::common::role_data_type type = proto::common::simple_info_type,bool is_save = true);
};
typedef boost::shared_ptr<role_t> role_ptr;

#endif // !__GAME_ROLE_H__

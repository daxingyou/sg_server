#pragma once

#include "scene/scene.hpp"

#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>
#include "family_war_common.hpp"
#include "scene/scene_state.hpp"
#include "fight/combat.hpp"

class family_war_scene_t : public scene_t
{
public:
	family_war_scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param);
	virtual ~family_war_scene_t() {};

 	// ��ʼ����������
 	virtual bool init();
 	// ����˺����ɾ����ͼ���ȵ��ã�
 	virtual void clear();
 	// ��������
 	virtual bool start();
	// �������е�ͼ����(��̬npc�пͻ����Լ�����)
	virtual void load_map_object();

	// �Ƿ��Ǽ���ս����
	virtual bool is_family_war_scene() { return true; }
	// ��ȡ��������ָ��
	virtual family_war_scene_ptr get_family_war_scene_ptr() { return boost::dynamic_pointer_cast<family_war_scene_t>(shared_from_this()); }

	/*�������*/
 public:
 	// �õ�������ҽ����ĳ�������
 	virtual sPos get_scene_born_position(const role_ptr& p_role);

	// �������ʱ������
	bool reach_max_time(uint32_t curr) const { return m_last_time <= curr; }

	// ������ʱ���ص�
	virtual void update(uint32_t curr_time);

	virtual void forced_all_leave();

 	// ����ս��һЩ��Ϣ
public:
	// ����ս״̬
	proto::common::family_war_state get_war_state() const { return m_state_type; }
	// ���ü���ս������Ϣ
	void set_family_war_date(const proto::server::family_war_dungeon_data& data);
	// ��ս����ս
	void prepare_family_war(uint32_t prepare_time);
 	// ��ʼ����ս
 	void start_family_war(uint32_t end_time);
 	// ����սս����
 	void end_family_war(bool is_early_end = false);
	// ����Ӯ�ļ���
	uint64_t get_win_family() const { return m_win_family; }
	// ����ʤ������
	family_war_side_type settlement_win_family();

	// ����սid
	uint32_t get_family_war_id() const { return m_family_war_id; }

	// ����ս����
	uint32_t get_family_war_member(family_war_side_type type);

	// �Ƿ��ǲ�ս����
	family_war_side_type get_family_war_side_type(uint64_t family_id);

	// �õ�����
	uint32_t get_family_score(family_war_side_type type);

	// �õ��жԼ���
	family_war_side_type get_enemy_side_type(family_war_side_type type);

	// ��ս����
	void peek_family_war_fight_info(proto::client::gc_family_war_fight_info_notify& notify);

	// �ٻ�����
	void family_war_summon_long(family_war_long_type type);

	// ȫ����ˢ�¼���ս��Ϣ
	void notify_scene_war_info();

	// ����ս������̨
	void family_occupied_platform(const role_ptr& p_role, family_war_platform_type platform_type, family_war_side_type side_type);

	// ����ս��ս���Գ��˺�(���������Ƿ����)
	bool long_hurt_family_flag(family_war_side_type type, uint32_t hurt);

	// ����ս��ս������˺�(���������Ƿ����)
	bool platform_hurt_family_flag(family_war_platform_type platform_type, family_war_side_type type, uint32_t hurt);

	// ��ʱ����
	void rank_all_score();

	// ��������
	uint32_t attack_long(const role_ptr& p_role);
	// �غ��˺�֪ͨ
	void attack_long_round(uint64_t uid, uint64_t fight_id, const proto::common::fight_camp& enemy_camp, const combat_ptr& p_combat);
	// ս������֪ͨ
	void attack_long_end(uint64_t uid, uint64_t fight_id, const combat_ptr& p_combat);
	// ֪ͨ��������
	void family_war_long_death();

	// �õ�title
	family_war_kill_title get_kill_title(uint32_t kill_num);
	// ��ɱ��õĽ���
	uint32_t get_reward_by_self_title(family_war_kill_title title);
	// ��ɱ�ƺ���ҽ���
	uint32_t get_reward_by_other_title(family_war_kill_title title);
	// title name
	std::string get_title_name(family_war_kill_title title);

	// �õ�����id
	uint32_t get_long_type_monster_tid(family_war_long_type type);

	// �õ�ս������
	void peek_family_war_fight_data(uint64_t uid, proto::client::gc_family_war_fight_data_reply& reply);

	// ����ս��
	uint32_t role_enter_battlefield(role_ptr p_role);

	// ����ս�����
	void get_family_war_result(uint64_t uid, proto::client::gc_family_war_result_info_reply& reply);
 
 	/*�������*/
 public:
 	virtual void on_role_enter(role_ptr p_role);
 	virtual void on_role_leave(role_ptr p_role);
 
 	// �麯������뿪�����е�һ������Ļص�
 	virtual void role_leave_area(role_ptr p_role, uint32_t area_id);
 	// �麯����ҽ��볡���е�һ������Ļص�
 	virtual void role_enter_area(role_ptr p_role, uint32_t area_id);
 
 	// ����ĳ���
 	virtual void on_revive_role(role_ptr p_role);
 
	// �����ɱ�˱���
	virtual void on_role_kill_other(role_ptr p_role, object_position_ptr p_enemy);

	// �Լ������˻�ɱ
	virtual void on_other_kill_me(role_ptr p_role, object_position_ptr p_enemy);

 	// ��ʾ�������
 	virtual void show_revive_ui(role_ptr p_role);
 
 	// ��½���Ҫ��Ҫ��������
 	virtual void on_login_die(role_ptr p_role);
 
public:
	// ����ս�ݵ���Ϣ
	struct family_war_stronghold
	{
		uint64_t m_family_id = 0;
		uint32_t m_jump_id = 0;				// �����͸����
		uint32_t m_area_id = 0;				// �����͸��������
		uint32_t m_war_id = 0;				// ս������ת��
		std::set<uint64_t> m_family_roles;	// �������
		uint64_t m_kill_max_uid = 0;		// ɱ�����
		uint32_t m_kill_max_cnt = 0;		// ���ɱ����
		uint64_t m_hurt_max_uid = 0;		// �˺����
		uint32_t m_hurt_max_cnt = 0;		// ����˺���
		uint32_t m_occupied_cnt = 0;		// ռ�����
		uint32_t m_fight_win_cnt = 0;		// ս��ʤ������

		proto::client::family_war_fight_data m_data;

		void add_war_member(uint64_t uid);
		void del_war_member(uint64_t uid);

		bool is_flag_die() const;
		void hurt_flag(uint32_t val);

		void set_platform_state(family_war_platform_type type, bool is_occupied);

		void add_occupied_long();

		void add_score(uint32_t val);

		void add_fight_win_cnt();

		const std::string& get_family_name() const;
	};

	// ����ս������Ϣ
	struct family_war_long_info
	{
		uint64_t m_object_id = 0;
		uint32_t m_area_id = 0;
		uint32_t m_refresh_times = 0;
		family_war_long_state m_long_state = family_war_long_state_die;

		std::set<uint64_t> m_near_roles[family_war_side_max];	// �������

		void add_nearby_role(uint64_t uid, family_war_side_type type);
		void del_nearby_role(uint64_t uid, family_war_side_type type);

		// ����
		bool long_wei(const role_ptr& p_role, family_war_side_type type);
	};

	// ����̨��Ϣ
	struct family_war_platform_info
	{
		uint32_t m_interval = 0;
		uint32_t m_hurt = 0;
		uint64_t m_object_id = 0;
		family_war_side_type m_hurt_side = family_war_side_none;	// ���ĸ�����ս������˺�
	};

	// �����Ϣ
	struct family_war_role_info_t
	{
		uint64_t m_role_uid = 0;
		std::map<uint64_t, proto::common::family_war_hero_type> m_heros;
		proto::client::family_war_role_data m_data;
		uint32_t m_all_hurts = 0;
		uint32_t m_kill_count = 0;
		uint32_t m_death_count = 0;
		uint32_t m_kills = 0;
		family_war_kill_title m_kill_title = family_war_kill_title_0;

		bool m_update_hero = false;

		family_war_role_info_t(uint64_t uid);

		void init();

		void peek_hero_data(proto::client::gc_formations_info_notify& notify, bool peek_hero = true);

		void peek_hero_data(proto::client::gc_ask_formations_info_reply& notify);

		bool have_hero_live();

		void init_role_heros(const role_ptr& p_role);

		void notify_role_info(const role_ptr& p_role);

		// �����佫
		uint32_t role_hero_up(const role_ptr& p_role, uint64_t hero_uid);

		// �����佫
		uint32_t role_hero_down(const role_ptr& p_role, uint64_t hero_uid);

		// �����佫
		uint32_t role_hero_revive(const role_ptr& p_role, uint64_t hero_uid);

		// �佫����
		void role_hero_die(const role_ptr& p_role, uint64_t hero_uid);

		// ��������
		void update_rank(uint32_t rank);

		// ���ӻ���
		void add_score(const role_ptr& p_role, uint32_t score);

		// �۳��ж���
		void less_action_point(const role_ptr& p_role, uint32_t val, bool is_update = true);

		// ����ɱ����
		void add_one_kill();

		// ������������
		void add_self_death();
	};

	typedef boost::shared_ptr<family_war_role_info_t> family_war_role_info_ptr;
	typedef std::map<uint64_t, family_war_role_info_ptr> family_war_role_map;

	family_war_role_info_ptr get_family_war_role_info(uint64_t uid);

	family_war_role_info_ptr add_family_war_role_info(uint64_t uid);

	struct score_sort_t
	{
		uint64_t m_uid = 0;
		uint32_t m_score = 0;
		boost::weak_ptr<family_war_role_info_t> m_wptr;	// Ϊ�˼�������

		score_sort_t(uint64_t uid, uint32_t score, const family_war_role_info_ptr& ptr) : m_uid(uid), m_score(score), m_wptr(ptr) {};

		static bool score_sort_ex(const score_sort_t& lhs, const score_sort_t& rhs);
	};

protected:
	uint32_t m_last_time = 0;		// ������ʱ�䣬Ŀǰ��һ��
	scene_state_controler m_state_controler;
	bool m_update_rank = false;		// ���������仯

protected:
	uint32_t m_family_war_id = 0;	// ����ս���
	proto::common::family_war_state m_state_type = proto::common::family_war_state_end;
	uint32_t m_state_time = 0;		// ��ǰ״̬ʱ��
	uint64_t m_win_family = 0;

	// ����ս������Ϣ
	family_war_long_info m_family_war_long_info;

	// ����ս������Ϣ
	family_war_stronghold m_stronghold_side[family_war_side_max];

	// ����ս�����Ϣ
	family_war_role_map m_family_war_role_map;

	// ����̨
	family_war_platform_info m_family_war_platform[family_war_platform_max];

	// ��ʱ��
public:
	static void on_long_refresh_timer(const boost::weak_ptr<family_war_scene_t>& p_wlong, const boost::system::error_code& ec);
	void on_long_refresh();
	void on_update_long_state();

	static void on_platform_hurt_timer(const boost::weak_ptr<family_war_scene_t>& p_wlong, family_war_platform_type type, const boost::system::error_code& ec);
	void on_platform_hurt(family_war_platform_type type);
	
protected:
	boost::shared_ptr<boost::asio::deadline_timer> m_long_timer = NULL;	// ����ˢ�¶�ʱ��

	boost::shared_ptr<boost::asio::deadline_timer> m_platform_timer[family_war_platform_max] = { NULL };	// ����ˢ�¶�ʱ��
};
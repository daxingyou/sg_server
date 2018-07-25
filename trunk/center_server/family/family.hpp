#pragma once

#include <map>
#include <set>

#include "family_member.hpp"
#include "family_build.hpp"
#include "tblh/family_enum.hpp"
#include <boost/enable_shared_from_this.hpp>

#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"

#include "client.pb.h"
#include "family_shop.hpp"

class family : public boost::enable_shared_from_this<family>
{
	friend class family_data_mgr_t;

public:
	family(uint64_t family_id);
	~family();

	typedef std::map<uint64_t, family_member_ptr> family_member_map;

	typedef std::set<uint64_t> officer_vec;
	typedef std::map<uint32_t, officer_vec> officer_map;

	typedef std::map<uint64_t, uint32_t> family_apply_map;

	typedef std::map<proto::common::family_build_type, family_build_ptr> family_build_map;

	typedef std::map<uint64_t, uint32_t> invite_list_map;

public:
	// ��ʼ����������
	bool init(const global_user_ptr& p_user, const std::string& family_name, const std::string& declaration);

	// �������
	bool join_in(uint64_t uid, uint32_t officer);
	bool join_in(const global_user_ptr& p_user, uint32_t officer);

	// �������б�ɾ��
	void del_from_apply_list(uint64_t uid);

	// �뿪����
	bool leave_out(const global_user_ptr& p_user);
	bool leave_out(uint64_t uid);

	// ���������ְ
	uint32_t assign_member(uint64_t master_id, uint64_t uid, uint32_t officer);

	// �����峤
	uint32_t family_demise(const global_user_ptr& p_master, uint64_t uid);

	// ���������б�
	bool add_appliy_list(uint64_t uid);

	// �Ƿ��Ѿ��������б�cd��
	bool is_in_invite_list_cd(uint64_t uid);
	// ���������б���
	void add_in_invite_list(uint64_t uid);
	// �ܾ������б�ɾ��
	void del_from_invite_list(uint64_t uid);
	// ͬ������������
	uint32_t agree_invite(const global_user_ptr& p_user, uint64_t inviter_uid);

	// �ܾ�ĳ�����
	bool decline_apply_user(uint64_t uid);
	// �ܾ������������
	void decline_all_apply_user();

	// ͬ��������
	uint32_t approve_join(uint64_t master_uid, uint64_t uid, bool is_save = true);
	// ͬ�������������
	uint32_t approve_all_apply_user(uint64_t master_uid, proto::client::ec_one_key_approve_join_family_reply& reply);

	// toogle �����Զ������¾�
	void toggle_aoto_newbie();

	// ��������
	const std::string& update_family_declaration(const std::string& str);

	// ���ù���
	const std::string& update_family_notification(const std::string& str);

	// ��������
	uint32_t upgrade_build(proto::common::family_build_type type, proto::client::ec_upgrade_family_build_reply& reply);

	// ������cd
	uint32_t clear_build_cd(proto::common::family_build_type type);

protected:
	// �������
	family_member_ptr member_in(const global_user_ptr& p_user, uint32_t officer = family_officer_type_newbie, bool is_save = true);
	// �������
	family_member_ptr member_in(uint64_t uid, uint32_t officer = family_officer_type_newbie);

	// �뿪����
	bool member_out(uint64_t uid);

public:
	// ����id
	uint64_t get_family_id() const { return m_family_id; }
	// ����ȼ�
	uint32_t get_level() const { return m_data.level(); }
	// ���彨���ȼ�
	uint32_t get_build_level(proto::common::family_build_type type) const;
	// �����ȼ�
	void set_level(uint32_t val);
	// �������id
	uint32_t get_country_id() const { return m_data.country(); }
	// �Ƿ��Զ�����xinjun
	bool is_auto_newbie_state() const { return m_data.auto_newbie(); }
	// ��������
	const std::string& get_name() const { return m_data.name(); }
	// �õ��峤id
	uint64_t get_general_id() const { return common::string_util_t::string_to_uint64(m_data.patriarch()); };
	// �õ����鳤id
	uint64_t get_deputy_general_id() const;
	// �õ����ٶ�
	uint32_t get_prosperity() const { return m_data.prosperity(); }
	// �õ�����
	uint32_t get_prestige() const { return m_data.prestige(); }
	// ����ʱ��
	uint32_t get_create_time() const { return m_create_time; }
	// �ҳ϶�
	uint32_t get_loyalty() const { return m_data.loyalty(); }

	family_member_ptr get_family_member(uint64_t uid);

	family_build_ptr get_family_build(proto::common::family_build_type type);

	// �����ʽ�
	uint32_t get_money() const { return m_data.money(); }
	// ���Ӽ����ʽ�
	void add_money(uint32_t val, uint32_t type, uint64_t uid = 0);
	// �����ʽ�
	void cost_money(uint32_t val, uint32_t type, uint64_t uid = 0);
	
	// ����ά������
	void set_maintain(uint32_t cost, bool is_low);

	// �Ƿ��Զ�����
	bool is_auto_newbie(uint32_t level);

	// �õ�����ְλ������
	uint32_t get_officer_count(uint32_t officer);

	// peek ����
	void peek_data(proto::common::family_list_info* p_data);
	void get_family_info(proto::client::ec_get_family_info_reply& reply);
	void get_family_member_list(proto::client::ec_get_family_member_list_reply& reply);
	void get_family_member_list(proto::client::ec_open_appoint_reply& reply);
	void get_family_apply_list(proto::client::ec_family_apply_list_reply& reply);
	void get_family_build_list(proto::client::ec_get_family_build_info_reply& reply);

	// ��֯��Ը����
	void get_family_prayer_list(proto::common::family_prayer_list* p_list);

public:
	// ���³�Ա����
	void update_member_size();
	// �����Ա����
	uint32_t get_max_member_size() const { return m_data.member_cnt_max(); }
	uint32_t get_max_newbie_size() const { return m_newbie_cnt_max; }
	// �����Ա��
	uint32_t get_member_size() const { return m_member_map.size(); }
	// �����Ա�Ƿ�����
	bool is_member_full() const { return m_member_map.size() >= get_max_member_size(); }
	// ���м����Ա
	const family_member_map& get_all_member() const { return m_member_map; }

	// ���¼���ά���ѣ�ÿ������Ƿ��ǵ�ά��״̬
	void update_maintain_cost();
	uint32_t get_normal_maintain_cost();
	// ����ά����
	uint32_t get_maintain_cost() const { return m_data.matain_cost(); }
	// �Ƿ��ڵ�ά��״̬
	bool is_low_maintenance() const { return m_is_low_maintenance; }

	// ��������ʽ�
	void update_max_money();
	// �ʽ�����
	uint32_t get_max_money() const { return m_data.max_money(); }

	// ���·��ٶ�
	void update_prosperity();

	// ��������
	void update_prestige_daily();

	// ѧͽ�Զ���Ϊ��ʽ��Ա
	void auto_newbie_member();

	// ���³�Ա����״̬
	void update_member_online_state(const global_user_ptr& p_user);

	// ���Ա仯֪ͨ
	void update_member_attr(const global_user_ptr& p_user);

	// ���幱������
	void add_family_contribution(uint64_t uid, uint32_t val);

	// ������ҫֵ
	void update_member_glory(uint64_t role_uid, uint32_t glory);
public:
	// �Ƿ��ڵ���
	bool is_impeacher() const { return m_impeacher != 0; }
	// ���µ���
	void update_impeacher();

	//���¼�����ҫֵ����ˢ������
	void update_glory();
	// ����ս
public:
	uint32_t get_family_war_member_cnt(uint32_t level);

	// ����
public:
	// ����ı��˹��Ҵ���
	void change_country(uint32_t new_country_id);

	// �Ƿ��ǹ�������
	bool is_king_family() const { return m_is_king_family; }

	// �����Ƿ��ǹ�������
	void set_king_family(bool is_king) { m_is_king_family = is_king; }

	// �����Ƿ��ǹ�������
	void change_king_family(bool is_king);

	bool check_quit_country_times();

	// �õ���������ս�ı�������id
	uint32_t get_king_war_apply_country() const { return m_king_war_apply_country; }

	// ��������ս��������id
	void set_king_war_apply_country(uint32_t country_id);

	// �õ���������ս�Ĳ�ս����id
	uint32_t get_king_war_fight_country() const { return m_king_war_fight_country; }

	// ��������ս��ս����id
	void set_king_war_fight_country(uint32_t country_id);

	// �����̵�
public:
	// ����Ƿ�Ҫˢ���̵�
	bool check_shop_expired();
	// �̵�����
	void peek_family_shop_data(proto::common::family_shop_data* p_data);
	// �̵�����
	const family_shop_t& get_family_shop() const { return m_family_shop; }
	// ������Ʒ
	uint32_t family_shop_buy(const global_user_ptr& p_user, uint32_t index, uint32_t shop_item, uint32_t num);
	// ����ʧ�ܻ��˴���
	void family_shop_failed_back(const global_user_ptr& p_user, uint32_t refresh_time, uint32_t index, uint32_t num);

public:
	template<typename T_MSG>
	void send_msg_to_family(uint16_t cmd, const T_MSG& msg)
	{
		std::string cmd_data;
		network::wrap_msg(cmd_data, msg);

		proto::server::ea_broadcast_notify msg_to_gate;
		msg_to_gate.set_cmd(cmd);
		msg_to_gate.set_data(cmd_data);

		for (family_member_map::const_iterator citr = m_member_map.begin();
			citr != m_member_map.end(); ++citr)
		{
			if (citr->second != NULL && citr->second->is_online())
			{
				msg_to_gate.add_role_list(citr->first);
			}
		}

		env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
	}

	// ���������߳�Ա����ϵͳ�㲥
	void send_sys_notice_to_member(uint32_t sys_tid);

	// ��������ҷ���ϵͳ�ʼ�
	void send_sys_mail_to_member(uint32_t mail_tid);
public:
	// ֪ͨ�������ز�
	void tower_trigger_notify(uint64_t role_uid, const proto::server::ge_tower_trigger_notify& msg);
protected:
	uint64_t m_family_id = 0;					// ����Ψһid
	proto::common::family_data m_data;			// һЩ������Ϣ
	uint32_t m_newbie_cnt_max = 0;				// ѧͽ��Ա����
	bool m_is_king_family = false;				// �Ƿ��ǹ�������
	uint32_t m_quit_time = 0;					// �˳�ͬ��ʱ��
	uint32_t m_quit_count = 0;					// �˳�ͬ�˴���

	family_member_map m_member_map;				// �����Ա
	officer_map m_officer_map;					// �����ְ�б�

	family_apply_map m_apply_map;				// ���������б�

	family_build_map m_build_map;				// ���彨���б�

	uint32_t m_online_prosperity_add_daily = 0;	// ���ռ��己�ٶ����߼ӳ�
	uint32_t m_online_prosperity_daily_max = 0;	// ���ռ��己�ٶ����߼ӳ�����
	uint32_t m_chat_prosperity_add_daily = 0;	// ���ռ��己�ٶ�˵���ӳ�
	uint32_t m_chat_prosperity_daily_max = 0;	// ���ռ��己�ٶ�˵���ӳ�����
	uint32_t m_task_prosperity_add_daily = 0;	// ���ռ��己�ٶ�����ӳ�
	uint32_t m_task_prosperity_daily_max = 0;	// ���ռ��己�ٶ�����ӳ�����

	uint32_t m_prestige_add_daily = 0;			// ���ռ�������ͳ��

	bool m_is_low_maintenance = false;			// �Ƿ��ڵ�ά��״̬

	uint32_t m_create_time = 0;					// ���崴��ʱ��

	uint32_t m_glory = 0;						// ������ҫֵ�����������ݿ⣬ʵʱ���ݳ�Ա��ҫֵ�����������ֵ���ڼ�����ҫ���У�
protected:
	// �������
	uint64_t m_impeacher = 0;					// ��������
	uint32_t m_impeach_time = 0;				// ����ʱ��
	uint32_t m_impeacher_cnt = 0;				// ��Ӧ����

	// �����б�<uid,time_out>
	invite_list_map m_invite_list_map;

protected:
	family_shop_t m_family_shop;

protected:
	uint32_t m_king_war_apply_country = 0;			// ����ս��������
	uint32_t m_king_war_fight_country = 0;		// ����ս��ս����
};

typedef boost::shared_ptr<family> family_ptr;
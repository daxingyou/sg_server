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

	// ��ʼ����������
	virtual bool init();
	// ����˺����ɾ����ͼ���ȵ��ã�
	virtual void clear();
	// �Ƿ�������
	virtual bool is_king_war_city() { return true; }
	// ��ȡ����ս����ָ��
	virtual king_war_scene_ptr get_king_war_scene_ptr() { return boost::dynamic_pointer_cast<king_war_scene_t>(shared_from_this()); }

	// �������е�ͼ�ɼ���
	virtual void load_map_collect();

	// ����ս��һЩ��Ϣ
public:
	// ����id
	uint32_t get_country_id() const { return m_country_id; }

	// ���뱸ս��
	void ready_king_war();
	// ��������ս
	void start_king_war();
	// ����ս����
	void end_king_war();

	// �õ�����״̬
	uint32_t get_king_war_state() const { return m_king_war_state; }
	void set_king_war_state(uint32_t state) { m_king_war_state = state; }

	// ��������ս����
	void set_king_war_family(king_war_side_type type, const king_war_family& family_info);

	// �������������
	void king_war_scene_process(role_ptr p_role);

	// �Ƿ��ǲ�ս����
	uint32_t get_king_war_side_type(uint64_t family_id);

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

	// �������е����֪ͨ������Ϣ
	void update_long_info(proto::client::gc_update_long_info_notify& notify);

	// ȫ����ͬ��������Ϣ
	void notify_scene_long_info();

	// �����ܷ�ɼ�
	uint32_t is_can_collect_artifacts(role_ptr p_role);

	// �ɼ������ɹ�
	void role_get_artifacts(const role_ptr& p_role, uint64_t artifacts_uid, uint32_t buff_tid);

	// �����ɱ�˱���
	virtual void on_role_kill_other(role_ptr p_role, object_base_ptr p_enemy);

	// �Լ������˻�ɱ
	virtual void on_other_kill_me(role_ptr p_role, object_base_ptr p_enemy);

	// ��ʾ�������
	virtual void show_revive_ui(role_ptr p_role);

	// ��½���Ҫ��Ҫ��������
	virtual void on_login_die(role_ptr p_role);

	// ��ʱ֪ͨ���ķ�������
	void update_center_role_num();

	uint32_t ask_king_war_enter_scene(const role_ptr& p_role);

	/*�������*/
public:
	// �õ�������ҽ����ĳ�������
	virtual sPos get_scene_born_position(const role_ptr& p_role);
	// ����Ƿ��ܽ�����
	virtual uint32_t scene_enter_check(const role_ptr& p_role);

	// �����Ƿ��ܹ����
	virtual uint32_t check_scene_troop();

	// ��ʱ���ص�
public:
	void on_ready_timer_call_back(const boost::system::error_code& ec);

	void on_refresh_info_call_back(const boost::system::error_code& ec);

	void on_artifacts_call_back(const boost::system::error_code& ec);

	// ���س���ʱ���أ�����Ҫ�������Ϣ
protected:
	// ����id
	uint32_t m_country_id = 0;
	// ��������Ϣ
	king_war_relive_side m_relive_side[king_war_side_max];

	// ����ս��Ϣ
public:
	typedef boost::shared_ptr<king_war_long_pulse> king_war_long_pulse_ptr;
	typedef std::map<uint32_t, king_war_long_pulse_ptr> king_war_long_pulse_map;

	king_war_long_pulse_ptr get_king_war_long_pulse_by_area_id(uint32_t area_id);

	// ���²��õ������е��ѷ�λ��
	void get_king_war_roles_pos(uint32_t side, proto::common::king_war_side_roles* p_roles);
	// �õ�ս��
	uint32_t get_king_war_warfare(uint32_t side_type) const;

	// �õ���������ʱ
	uint32_t get_artifacts_start_left() const;

protected:
	// ������Ϣ
	king_war_long_pulse_map m_long_pulse_map;
	// ����ս״̬
	uint32_t m_king_war_state = proto::common::king_war_state_end;
	// ��ս����
	king_war_family m_fight_family[king_war_side_max];
	// ������������������id
	std::set<uint64_t> m_family_roles[king_war_side_max];
	// ������Ϣ
	artifacts_info_t m_artifacts_info;
	// ������Ϣ�б�
	collect_define_vec m_artifacts_vec;
	// �����������˵�λ��
	proto::common::king_war_side_roles m_king_war_role[king_war_side_max];
	// �����������˵�ս��
	uint32_t m_king_war_warfare[king_war_side_max] = { 0 };
	// ͬ��λ�õ�ʱ��
	uint32_t m_roles_pos_time = 0;

	// ��ʱ����ע��ɾ������ʱ���������ֹй¶
protected:
	boost::shared_ptr<boost::asio::deadline_timer> m_ready_timer = NULL;	// ׼��ʱ�����˻ص�

	boost::shared_ptr<boost::asio::deadline_timer> m_refresh_timer = NULL;	// ˢ��������Ϣ��ʱ��

	boost::shared_ptr<boost::asio::deadline_timer> m_artifacts_timer = NULL;	// ������ʱ��
};

#endif

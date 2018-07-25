#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <map>
#include <vector>

#include "tblh/country_enum.hpp"
#include "country_common.hpp"
#include "user/global_user.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "client.pb.h"
#include "family/family.hpp"

class country_t;
typedef boost::shared_ptr<country_t> country_ptr;

class country_t : 
	public boost::enable_shared_from_this<country_t>
{
	friend class country_data_mgr_t;
public:
	country_t() = delete;
	country_t(uint32_t country_id);
	~country_t() = default;

public:
	struct king_war_info
	{
		proto::common::king_war_state m_state = proto::common::king_war_state_end;	// ״̬
		uint64_t m_family_defensive = 0;										// ���ط�����
		uint32_t m_defensive_cost = 0;											// ���ط������ʽ�
		uint64_t m_family_attack = 0;											// ����������
		uint32_t m_attack_cost = 0;												// �����������ʽ�
		uint32_t m_state_end_time = 0;											// ��ǰ״̬����ʱ��
		uint32_t m_start_time = 0;												// ����ս��ʼʱ��

		void reset() {
			m_state = proto::common::king_war_state_end;					// ״̬
			m_family_defensive = 0;											// ���ط�����
			m_defensive_cost = 0;											// ���ط������ʽ�
			m_family_attack = 0;											// ����������
			m_attack_cost = 0;												// �����������ʽ�
			m_state_end_time = 0;											// ��ǰ״̬����ʱ��
			m_start_time = 0;												// ����ս��ʼʱ��
		}
	};

	struct country_info_t
	{
		uint64_t m_king_family_id = 0;			// ��������
		proto::common::country_info_data info;	// ������Ϣ
		std::map<uint32_t, proto::common::country_officer_data> officers; // ��ְ��Ϣ
		king_war_info m_king_war_info;			// ����ս��Ϣ
		std::set<uint32_t> m_flag_list;			// ��õ�ϡ�����ӵ��б�
	};

	struct king_war_apply
	{
		uint64_t m_family_id = 0;
		uint32_t m_king_war_id = 0;	// ����id
		uint32_t m_bid_cost = 0;
		EM_BID_STATE m_bid_state = EM_BID_STATE::EM_BID_STATE_NONE;
		uint32_t m_time = 0;

		void reset() {
			m_family_id = 0;
			m_king_war_id = 0;
			m_bid_cost = 0;
			m_bid_state = EM_BID_STATE::EM_BID_STATE_NONE;
			m_time = 0;
		}
	};

	typedef boost::shared_ptr<king_war_apply> king_war_apply_ptr;
	typedef std::map<uint64_t, king_war_apply_ptr> king_war_apply_map;

	struct family_force_t
	{
		uint64_t family_uid = 0;
		proto::common::country_family_data base;

		family_force_t(uint64_t uid) : family_uid(uid)
		{
			this->base.set_family_uid(common::string_util_t::uint64_to_string(uid));
		}

		family_force_t(const family_force_t& ref)
		{
			this->family_uid = ref.family_uid;
			this->base.CopyFrom(ref.base);
		}

		family_force_t& operator =(const family_force_t& ref)
		{
			this->family_uid = ref.family_uid;
			this->base.CopyFrom(ref.base);

			return *this;
		}

		friend bool operator< (const family_force_t& lhs, const family_force_t& rhs) {
			if (lhs.base.loyalty() != rhs.base.loyalty()) {
				return lhs.base.loyalty() > rhs.base.loyalty();
			}

			if (lhs.base.level() != rhs.base.level()) {
				return lhs.base.level() > rhs.base.level();
			}

			return lhs.family_uid < rhs.family_uid;
		}

		void peek_data(proto::common::country_family_data* p_data)
		{
			if (p_data) {
				p_data->CopyFrom(base);
			}
		}
	};

	typedef std::vector<family_force_t> family_force_vec;

	typedef std::map<uint64_t, ROLE_ONLINE_STATE> country_member_map;

public:
	template<typename T_MSG>
	void send_msg_to_country(uint16_t cmd, const T_MSG& msg);

public:
	void peek_common_data(proto::common::country_common* p_base);
	void peek_base_data(proto::common::country_base_data* p_base);
	void peek_country_info(proto::common::country_info_data* p_data);
	void peek_country_data(proto::client::ec_country_info_reply& reply);
	void peek_country_flag(proto::client::ec_country_flag_list_reply& reply);

public:
	// ����id
	uint32_t get_country_id() const { return m_id; }
	// ��������
	const std::string& get_country_name() const { return m_country_info.info.base().name(); }
	uint32_t get_country_flag() const { return m_country_info.info.base().flag(); }
	// �õ���������
	uint64_t get_king_family() const { return m_country_info.m_king_family_id; }
	// �����µļ���
	void join_country(const family_ptr& p_family);
	// �����µļ���
	void add_family(const family_ptr& p_family);
	// �뿪���崦��
	void quit_country(const family_ptr& p_family);
	// ɾ������
	void del_family(const family_ptr& p_family);
	// ���ù�����Ϣ
	void reset_king_family();
	// �����ҳ϶�
	void update_loyalty(const family_ptr& p_family);
	// �޸����
	void do_modify_year_name(uint64_t uid, const std::string& name);
	// �޸����
	uint32_t modify_year_name(const global_user_ptr& p_user, const std::string& name);
	// �Ƿ����������
	bool is_have_flag(uint32_t flag);
	// �޸Ĺ�������
	void do_modify_country_name(uint64_t uid, const std::string& name);
	// �޸Ĺ�������
	void do_modify_country_flag(uint64_t uid, uint32_t flag);
	// �޸Ĺ������ֺ�����
	uint32_t modify_name_flag(const global_user_ptr& p_user, const std::string& name, uint32_t flag);
	// �޸Ĺ���
	void modify_notice(uint64_t uid, const std::string& notice);
	// ɾ��һ����Ա
	void del_officer(uint32_t officer_id, uint64_t uid);
	// �Ĺ�ְ�Ƿ��Ѿ����˵���
	bool is_officer_appoint(uint32_t officer_id);
	// �Ƿ���cd�ڼ�
	bool is_officer_appoint_cd(uint32_t officer_id);
	// ���ù�Ա��ֻ�����ö�����
	void set_officer_appoint(uint32_t officer_id, const global_user_ptr& p_user, proto::common::country_officer_data* p_data);
	// ���ù�Ա��ֻ�����ö�����
	void set_officer_appoint(uint32_t officer_id, uint64_t uid, proto::common::country_officer_data* p_data);
	// �����Ա
	void fire_officer(uint32_t officer_id, proto::common::country_officer_data* p_data);
	// �ǹ�
	void resign_officer(const global_user_ptr& p_user, proto::common::country_officer_data* p_data);
	// ��������
	void get_country_family_list(proto::client::ec_country_family_list_reply& reply);
	// ���¹�ְ
	void update_officer_attr(uint32_t officer_type, const global_user_ptr& p_user);

	// ����������������
	void calc_rank();
	// Ȩ�޼��
	static bool has_family_privilige(uint32_t officer, uint32_t type);

	// ÿ�ո��¹���
	uint32_t update_new_power(uint32_t rank_value);

	// ����ʵ��
	void update_power_level(uint32_t type);

	// �����ʽ�
	uint32_t get_money() const { return m_country_info.info.money(); }
	// ���ӹ����ʽ�
	void add_money(uint32_t val, uint32_t type, uint64_t uid = 0);
	// ���Ĺ����ʽ�
	void cost_money(uint32_t val, uint32_t type, uint64_t uid = 0);

public:
	/**
	 * \brief ��ʼ��������ս�ȴ���
	 */
	void init(uint32_t now_time);

	/**
	 * \brief ���ҳ�Ա���ߴ���
	 */
	void ready_enter_scene(global_user_ptr p_user);

	/**
	 * \brief ����������������������ս���壬�۳��ʽ������ķ���
	 */
	bool king_war_apply_end();

	/**
	 * \brief ���������˵ı����ʽ�
	 */
	void king_war_apply_back();

	/**
	 * \brief ���ز�ս���屨���ʽ�
	 */
	void king_war_error_back();

	/**
	 * \brief �õ�һ��������ߵļ���
	 */
	king_war_apply_ptr get_king_war_attack_family(uint64_t except = 0);

	/**
	 * \brief ��������ս��������������ս��Ϣ
	 */
	void king_war_end(bool is_normal_end, uint64_t king_family_id = 0);

	/**
	 * \brief �õ�������Ϣ
	 */
	king_war_apply_ptr get_king_war_apply_info(uint64_t family_id);

	/**
	* \brief ���һ�����ϱ���Ҫ��ļ���
	*/
	king_war_apply_ptr add_king_war_family(uint64_t family_id);

	/**
	 * \brief �õ��б�
	 */
	const king_war_apply_map& get_king_war_apply_list() const { return m_king_war_apply_map; }

	/**
	 * \brief ������������һЩ����
	 */
	bool set_new_king_family(uint64_t king_family_id);

	/**
	 * \brief ��ʼ����ս����
	 */
	void start_king_war_sign_up(uint32_t start_time, uint32_t state_end_time);

	/**
	* \brief ��������ս����
	*/
	void end_king_war_sign_up();

	/**
	* \brief  ����ս��ս��ʼ
	*/
	void start_king_war_prepare(uint32_t state_end_time);

	/**
	* \brief  ����ս��ս����
	*/
	void end_king_war_prepare();

	/**
	* \brief  ����սս����ʼ
	*/
	void start_king_war_fight(uint32_t state_end_time);

	/**
	* \brief  ����սս������
	*/
	void end_king_war_fight();

	/**
	 * \brief �õ��ù�������ս��Ϣ
	 */
	const king_war_info& get_king_war_info() const { return m_country_info.m_king_war_info; }

	/**
	 * \brief ֪ͨ����������ս״̬
	 */
	void notify_self_king_war_state();

	/**
	 * \brief  ��������ս���
	 */
	void set_king_war_result(uint64_t win_family_id);

	/**
	 * \brief ������Ϣ
	 */
	const country_info_t& get_country_info() const { return m_country_info; }

protected:
	uint32_t m_id = 0;								// ����id
	country_info_t m_country_info;					// �����Ļ�����Ϣ
	king_war_apply_map m_king_war_apply_map;		// ����ս�����б�
	country_member_map m_country_member_map;		// ���ҳ�Ա
	family_force_vec m_family_force_vec;			// ��������
};

template<typename T_MSG>
void country_t::send_msg_to_country(uint16_t cmd, const T_MSG& msg)
{
	std::string cmd_data;
	network::wrap_msg(cmd_data, msg);

	proto::server::ea_broadcast_notify msg_to_gate;
	msg_to_gate.set_cmd(cmd);
	msg_to_gate.set_data(cmd_data);

	for (country_member_map::const_iterator citr = m_country_member_map.begin();
		citr != m_country_member_map.end(); ++citr) {
		if (citr->second == ROLE_ONLINE_STATE_ONLINE) {
			msg_to_gate.add_role_list(citr->first);
		}
	}

	env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
}

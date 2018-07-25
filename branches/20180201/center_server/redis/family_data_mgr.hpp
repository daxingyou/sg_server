#pragma once

#include "family/family_manager.hpp"

class family_data_mgr_t
{
public:
	/**
	 * \brief �������м�������
	 */
	static bool load_all_datas(family_manager_t::family_map& familys);

	/**
	 * \brief ���ؼ���ս����
	 */
	static bool load_all_war_data(uint32_t cond, family_manager_t::against_data_map& against_map, family_manager_t::family_against_id_map& id_map);

	/**
	 * \brief ���ؼ���ս��ʷ����
	 */
	static bool load_all_war_history_data(uint32_t cond, proto::common::family_war_history_data& history_data);

	/**
	 * \brief �Ƿ����������
	 */
	static bool is_family_key_exsit(uint64_t family_id);

	/**
	 * \brief ��������Ƿ��Ѿ���ʹ��
	 */
	static bool is_family_name_exsit(const std::string& name);

	/**
	 * \brief ��������
	 */
	static std::string get_family_name(uint64_t family_id);

	/**
	 * \brief ��������Ա�Ƿ����
	 */
	static bool is_member_key_exsit(uint64_t member_id);

	/**
	 * \brief ���key�ļ��彨���Ƿ����
	 */
	static bool is_build_key_exsit(const std::string& key);

	/**
	 * \brief ���ؼ����Ա
	 */
	static bool load_all_member(const family_ptr& p_family);

	/**
	* \brief ���ؼ��彨��
	*/
	static bool load_all_build(const family_ptr& p_family);

	/**
	 * \brief ɾ��һ�������Ա��Ϣ
	 */
	static void del_family_member(uint64_t key);

	/**
	 * \brief ���ؼ�����Ϣ
	 */
	static bool load_family_data(const family_ptr& p_family);

	/**
	 * \brief ��������������Ϣ
	 */
	static void save_all(const family_ptr& p_family);

	/**
	 * \brief ������������б�
	 */
	static void save_apply_list(const family_ptr& p_family);

	/**
	 * \brief �������н���
	 */
	static void save_all_build(const family_ptr& p_family);

	/**
	* \brief �������г�Ա
	*/
	static void save_all_member(const family_ptr& p_family);

	/**
	 * \brief �����Ա��Ϣ
	 */
	static void save_one_member(const family_member_ptr& p_family_member);

	/**
	* \brief ���潨����Ϣ
	*/
	static void save_one_build(const family_build_ptr& p_family_build);

	/**
	 * \brief ɾ��һ������
	 */
	static void del_family(uint64_t family_id);

	/**
	 * \brief �����ս����
	 */
	static void save_all_war_data(uint32_t week_id, const family_manager_t::against_data_map& against_map);

public:
	// ���¼����ʽ�
	static void update_family_money(uint64_t family_id, uint32_t val);
	// ����ά���ʽ�
	static void update_family_maintain(uint64_t family_id, uint32_t val, bool is_low);
	// ���·��ٶ�
	static void update_family_prosperity(uint64_t family_id, uint32_t val);
	// ���·��ٶȼӳ�
	static void update_online_prosperity_add_daily(uint64_t family_id, uint32_t val);
	// ���·��ٶȼӳ�
	static void update_chat_prosperity_add_daily(uint64_t family_id, uint32_t val);
	// ���·��ٶȼӳ�
	static void update_task_prosperity_add_daily(uint64_t family_id, uint32_t val);
	// ��������
	static void update_family_prestige(uint64_t family_id, uint32_t val);
	// �����Ƿ��Զ��������
	static void update_family_auto_newbie(uint64_t family_id, bool val);
	// ��������
	static void update_family_declaration(uint64_t family_id, const std::string& val);
	// ���ù���
	static void update_family_notification(uint64_t family_id, const std::string& val);
	// ���µȼ�
	static void update_family_level(uint64_t family_id, uint32_t val);
	// ���¹���id
	static void update_family_country(uint64_t family_id, uint32_t val);
	// ���¼�������ҳ϶�
	static void update_family_loyalty(uint64_t family_id, uint32_t val);
	// �����˳����Ҵ���
	static void update_quit_country(uint64_t family_id, uint32_t quit_time, uint32_t quit_count);

public:
	// ���¼����Ա��ְ
	static void update_family_member_officer(uint64_t uid, uint32_t val);
	// ���¼�����ʷ����
	static void update_total_family_contribution(uint64_t uid, uint32_t val);
	// ������Ը����
	static void update_prayer_contribution(uint64_t uid, uint32_t val);
	// ������Ը�Ĵ�������
	static void load_prayer_info(const family_member_ptr& p_family_member);
	// ������Ը����ÿ������
	static void update_day_prayer_info(const family_member_ptr& p_family_member);
	// ������Ըʣ�����
	static void update_prayer_times(uint64_t uid, uint32_t val);
	// ����ÿ����Ը��������
	static void update_prayer_rwd_times(uint64_t uid, uint32_t val);
	// ������Ը����ÿ������
	static void update_week_prayer_info(const family_member_ptr& p_family_member);
	// ������Ը����
	static void update_prayer_hero_info(const family_member_ptr& p_family_member);
	// ������Ը��¼
	static void save_prayer_record(uint64_t uid, const proto::common::family_prayer_record_list& record_list);
	// �����ܽ�������
	static void update_prayer_gift_times(uint64_t uid, uint32_t val);

public:
	// �����Ա�޹���Ϣ
	static void load_member_shop_info(const family_member_ptr& p_family_member);
	// ���ؼ����̵���Ϣ
	static bool load_family_shop(const family_ptr& p_family);
	// ��������̵���Ϣ
	static void save_family_shop(const family_ptr& p_family);
	// ��������̵�
	static void save_family_shop(uint64_t family_id, const proto::common::family_shop_data& data);
	// ��������̵������Ϣ
	static void save_family_shop_personal(uint64_t uid, const proto::common::family_shop_personal_data& data);
	// ��ռ����̵������Ϣ
	static void clear_family_shop_personal(uint64_t uid);
};
#ifndef __COUNTRY_DATA_MGR_HPP__
#define __COUNTRY_DATA_MGR_HPP__

#include "country/country_mgr.hpp"

class country_data_mgr_t
{
public:
	/**
	* \brief �������й�������
	*/
	static bool load_all_datas(country_mgr_t::country_map& countries);

	/**
	 * \brief ����������
	 */
	static void update_country_year_name(uint32_t country_id, const std::string& name);

	/**
	* \brief ������ҹ���
	*/
	static void update_country_notice(uint32_t country_id, const std::string& notice);

	/**
	* \brief ��������ʽ�
	*/
	static void update_country_money(uint32_t country_id, uint32_t val);

	/**
	* \brief ���¹���
	*/
	static void update_country_power(uint32_t country_id, uint32_t val);

	/**
	 * \brief ���¹�������
	 */
	static void update_power_level(uint32_t country_id, uint32_t val);

	/**
	 * \brief ��������ְ
	 */
	static void save_country_officer(uint32_t country_id, uint32_t officer_id, const proto::common::country_officer_data& data);

	/**
	 * \brief ������Ϣ�Ƿ����
	 */
	static bool is_country_key_exsit(uint32_t country_id);

	/**
	 * \brief ����ս������Ϣ�Ƿ����
	 */
	static bool is_king_war_apply_key_exsit(uint64_t family_id);

	/**
	 * \brief ��ʼ�������б�
	 */
	static void init_king_war_offer_info(uint32_t king_war_id, country_t::king_war_apply_map& apply_map);

	/**
	 * \brief �޸Ĺ�������
	 */
	static void update_country_name(uint32_t country_id, const std::string& name);

	/**
	 * \brief �޸�����id
	 */
	static void update_country_flag(uint32_t country_id, uint32_t flag);

	/**
	 * \brief �޸������б�
	 */
	static void update_country_flag_list(uint32_t country_id, const std::set<uint32_t>& list);

	/**
	 * \brief �޸�������Ѵ���
	 */
	static void update_country_free_times(uint32_t country_id, uint32_t val);

	/**
	* \brief �޸������Ѵ���
	*/
	static void update_year_name_free_times(uint32_t country_id, uint32_t val);

public:
	/**
	 * \brief ���¹�������ս��Ϣ
	 */
	static void update_country_king_war_info(uint32_t country_id, const country_t::king_war_info& info);

	/**
	 * \brief �޸�սǰ����
	 */
	static void update_country_notice_info(uint32_t country_id, uint32_t type, const country_t::king_war_info& info);

	/**
	 * \brief ���µ���
	 */
	static void update_country_like_info(uint32_t country_id, uint32_t type, const country_t::king_war_info& info);

	/**
	 * \brief ���¹�������id
	 */
	static void update_country_king_family_id(uint32_t country_id, uint64_t family_id);

	/**
	 * \brief ��������ս��Ϣ
	 */
	static void clear_king_war_info(uint32_t country_id);

	/**
	 * \brief ��������ս������Ϣ
	 */
	static void clear_king_war_apply_info(uint64_t family_id);

	/**
	 * \brief ���¾����ʽ�
	 */
	static void update_king_war_apply_cost(uint64_t family_id, country_t::king_war_apply_ptr info);

	/**
	 * \brief ������ս����id
	 */
	static void update_country_help_family(uint32_t country_id, uint32_t type, uint64_t help_family_uid);
	
public:
	// ���¹��������Ϣ
	static void update_king_luckydraw_info(uint32_t country_id, std::string &val);
	
	// ���¹����ʽ���Ϣ
	static void update_king_luckydraw_money(uint32_t country_id, uint32_t val);

	//��ù��������Ϣ
	static std::string load_king_luckydraw_info(uint32_t country_id);

	//��ù����ʽ���Ϣ
	static uint32_t load_king_luckydraw_money(uint32_t country_id);

};
#endif

#ifndef __COUNTRY_DATA_MGR_HPP__
#define __COUNTRY_DATA_MGR_HPP__

#include "country/country_mgr.hpp"

class country_data_mgr_t
{
public:
	/**
	* \brief 加载所有国家数据
	*/
	static bool load_all_datas(country_mgr_t::country_map& countries);

	/**
	 * \brief 保存国家年号
	 */
	static void update_country_year_name(uint32_t country_id, const std::string& name);

	/**
	* \brief 保存国家公告
	*/
	static void update_country_notice(uint32_t country_id, const std::string& notice);

	/**
	* \brief 保存国库资金
	*/
	static void update_country_money(uint32_t country_id, uint32_t val);

	/**
	* \brief 更新国力
	*/
	static void update_country_power(uint32_t country_id, uint32_t val);

	/**
	 * \brief 更新国力排行
	 */
	static void update_power_level(uint32_t country_id, uint32_t val);

	/**
	 * \brief 保存家族官职
	 */
	static void save_country_officer(uint32_t country_id, uint32_t officer_id, const proto::common::country_officer_data& data);

	/**
	 * \brief 国家信息是否存在
	 */
	static bool is_country_key_exsit(uint32_t country_id);

	/**
	 * \brief 王城战申请信息是否存在
	 */
	static bool is_king_war_apply_key_exsit(uint64_t family_id);

	/**
	 * \brief 初始化竞价列表
	 */
	static void init_king_war_offer_info(uint32_t king_war_id, country_t::king_war_apply_map& apply_map);

	/**
	 * \brief 修改国家名字
	 */
	static void update_country_name(uint32_t country_id, const std::string& name);

	/**
	 * \brief 修改旗子id
	 */
	static void update_country_flag(uint32_t country_id, uint32_t flag);

	/**
	 * \brief 修改旗子列表
	 */
	static void update_country_flag_list(uint32_t country_id, const std::set<uint32_t>& list);

	/**
	 * \brief 修改名称免费次数
	 */
	static void update_country_free_times(uint32_t country_id, uint32_t val);

	/**
	* \brief 修改年号免费次数
	*/
	static void update_year_name_free_times(uint32_t country_id, uint32_t val);

public:
	/**
	 * \brief 更新国家王城战信息
	 */
	static void update_country_king_war_info(uint32_t country_id, const country_t::king_war_info& info);

	/**
	 * \brief 修改战前宣言
	 */
	static void update_country_notice_info(uint32_t country_id, uint32_t type, const country_t::king_war_info& info);

	/**
	 * \brief 更新点赞
	 */
	static void update_country_like_info(uint32_t country_id, uint32_t type, const country_t::king_war_info& info);

	/**
	 * \brief 更新国王家族id
	 */
	static void update_country_king_family_id(uint32_t country_id, uint64_t family_id);

	/**
	 * \brief 清理王城战信息
	 */
	static void clear_king_war_info(uint32_t country_id);

	/**
	 * \brief 清理王朝战报名信息
	 */
	static void clear_king_war_apply_info(uint64_t family_id);

	/**
	 * \brief 更新竞价资金
	 */
	static void update_king_war_apply_cost(uint64_t family_id, country_t::king_war_apply_ptr info);

	/**
	 * \brief 更新助战家族id
	 */
	static void update_country_help_family(uint32_t country_id, uint32_t type, uint64_t help_family_uid);
	
public:
	// 更新国王礼包信息
	static void update_king_luckydraw_info(uint32_t country_id, std::string &val);
	
	// 更新国王资金信息
	static void update_king_luckydraw_money(uint32_t country_id, uint32_t val);

	//获得国王礼包信息
	static std::string load_king_luckydraw_info(uint32_t country_id);

	//获得国王资金信息
	static uint32_t load_king_luckydraw_money(uint32_t country_id);

};
#endif

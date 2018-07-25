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
	enum king_war_type : uint32_t
	{
		king_war_defensive = 0,
		king_war_attack = 1,
		king_war_type_max = 2,
		king_war_type_error = king_war_type_max,
	};

	struct king_war_info
	{
		proto::common::king_war_state m_state = proto::common::king_war_state_end;	// 状态
		uint64_t m_family_defensive = 0;										// 防守方家族
		uint64_t m_defensive_help_family = 0;									// 防守方助战家族
		uint32_t m_defensive_cost = 0;											// 防守方竞价资金
		std::string m_defensive_notice_user;									// 战前宣言的玩家名称
		uint32_t m_defensive_notice_officer = 0;								// 战前宣言的玩家职位
		std::string m_defensive_notice;											// 战前宣言
		uint32_t m_defensive_like_num = 0;										// 战前宣言的点赞
		uint32_t m_defensive_dislike_num = 0;									// 战前宣言的踩数
		uint64_t m_family_attack = 0;											// 进攻方家族
		uint64_t m_attack_help_family = 0;										// 进攻方助战家族
		uint32_t m_attack_cost = 0;												// 进攻方竞价资金
		std::string m_attack_notice_user;										// 战前宣言的玩家名称
		uint32_t m_attack_notice_officer = 0;									// 战前宣言的玩家职位
		std::string m_attack_notice;											// 战前宣言
		uint32_t m_attack_like_num = 0;											// 战前宣言的点赞
		uint32_t m_attack_dislike_num = 0;										// 战前宣言的踩数
		uint32_t m_state_end_time = 0;											// 当前状态结束时间
		uint32_t m_start_time = 0;												// 王城战开始时间
		uint32_t m_defensive_num = 0;											// 防御方人数
		uint32_t m_attack_num = 0;												// 进攻方人数

		std::set<uint64_t> m_invited_family[king_war_type_max];					// 已经邀请的家族
		std::set<uint64_t> m_refused_family[king_war_type_max];					// 已经拒绝的家族

		void reset() {
			m_state = proto::common::king_war_state_end;
			m_family_defensive = 0;
			m_defensive_help_family = 0;
			m_defensive_cost = 0;
			m_defensive_notice_user.clear();
			m_defensive_notice_officer = 0;
			m_defensive_notice.clear();
			m_defensive_like_num = 0;
			m_defensive_dislike_num = 0;
			m_family_attack = 0;
			m_attack_help_family = 0;
			m_attack_cost = 0;
			m_attack_notice_user.clear();
			m_attack_notice_officer = 0;
			m_attack_notice.clear();
			m_attack_like_num = 0;
			m_attack_dislike_num = 0;
			m_state_end_time = 0;
			m_start_time = 0;
			m_defensive_num = 0;
			m_attack_num = 0;

			for (uint32_t i = 0; i < king_war_type_max; ++i) {
				m_invited_family[i].clear();
				m_refused_family[i].clear();
			}
		}
	};

	struct country_info_t
	{
		uint64_t m_king_family_id = 0;			// 国王家族
		proto::common::country_info_data info;	// 国家信息
		std::map<uint32_t, proto::common::country_officer_data> officers; // 官职信息
		king_war_info m_king_war_info;			// 王城战信息
		std::set<uint32_t> m_flag_list;			// 获得的稀有旗子的列表
	};

	struct king_war_apply
	{
		uint64_t m_family_id = 0;
		uint32_t m_king_war_id = 0;	// 国家id
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
			: family_uid(ref.family_uid)
			, base(ref.base)
		{
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
	// 国家id
	uint32_t get_country_id() const { return m_id; }
	// 国家名字
	const std::string& get_country_name() const { return m_country_info.info.base().name(); }
	uint32_t get_country_flag() const { return m_country_info.info.base().flag(); }
	// 得到国王家族
	uint64_t get_king_family() const { return m_country_info.m_king_family_id; }
	// 加入新的家族
	void join_country(const family_ptr& p_family);
	// 加入新的家族
	void add_family(const family_ptr& p_family);
	// 离开家族处理
	void quit_country(const family_ptr& p_family);
	// 家族退出国家
	void del_family(const family_ptr& p_family, bool is_del_family = false);
	// 家族删除
	void del_family_king_war_clear(const family_ptr& p_family);
	// 重置国王信息
	void reset_king_family();
	// 更新忠诚度
	void update_loyalty(const family_ptr& p_family);
	// 修改年号
	void do_modify_year_name(uint64_t uid, const std::string& name);
	// 修改年号
	uint32_t modify_year_name(const global_user_ptr& p_user, const std::string& name);
	// 是否有这个旗帜
	bool is_have_flag(uint32_t flag);
	// 修改国家名称
	void do_modify_country_name(uint64_t uid, const std::string& name);
	// 修改国家旗帜
	void do_modify_country_flag(uint64_t uid, uint32_t flag);
	// 修改国家名字和旗帜
	uint32_t modify_name_flag(const global_user_ptr& p_user, const std::string& name, uint32_t flag);
	// 修改公告
	void modify_notice(uint64_t uid, const std::string& notice);
	// 删除一个官员
	void del_officer(uint32_t officer_id, uint64_t uid);
	// 改官职是否已经有人担任
	bool is_officer_appoint(uint32_t officer_id);
	// 是否在cd期间
	bool is_officer_appoint_cd(uint32_t officer_id);
	// 设置官员（只做设置动作）
	void set_officer_appoint(uint32_t officer_id, const global_user_ptr& p_user, proto::common::country_officer_data* p_data);
	// 设置官员（只做设置动作）
	void set_officer_appoint(uint32_t officer_id, uint64_t uid, proto::common::country_officer_data* p_data);
	// 罢免官员
	void fire_officer(uint32_t officer_id, proto::common::country_officer_data* p_data);
	// 辞官
	void resign_officer(const global_user_ptr& p_user, proto::common::country_officer_data* p_data);
	// 禅让国王
	void demise_king(const global_user_ptr& p_user, uint64_t uid);
	// 势力界面
	void get_country_family_list(proto::client::ec_country_family_list_reply& reply);
	// 更新官职
	void update_officer_attr(uint32_t type, const global_user_ptr& p_user);

	// 家族势力重新排名
	void calc_rank();
	// 权限检查
	static bool has_family_privilige(uint32_t officer, uint32_t type);

	// 每日更新国力
	uint32_t update_new_power(uint32_t rank_value);

	// 更新实力
	void update_power_level(uint32_t type);

	// 国家资金
	uint32_t get_money() const { return m_country_info.info.money(); }
	// 增加国家资金
	void add_money(uint32_t val, uint32_t type, uint64_t uid = 0);
	// 消耗国家资金
	void cost_money(uint32_t val, uint32_t type, uint64_t uid = 0);

	// 国家人员上线
	void king_war_user_on_login(const global_user_ptr& p_user);
public:
	/**
	 * \brief 初始化，王城战等处理
	 */
	void init(uint32_t now_time);

	/**
	 * \brief 王城战状态
	 */
	proto::common::king_war_state get_king_war_state() const { return m_country_info.m_king_war_info.m_state; }

	/**
	 * \brief 报名结束，产生王城争夺战家族，扣除资金，其他的返还
	 */
	bool king_war_apply_end();

	/**
	 * \brief 返还所有人的报名资金
	 */
	void king_war_apply_back();

	/**
	 * \brief 返回参战家族报名资金
	 */
	void king_war_error_back();

	/**
	 * \brief 得到一个竞价最高的家族
	 */
	king_war_apply_ptr get_king_war_attack_family(uint64_t except = 0);

	/**
	 * \brief 王城争夺战结束，清理王城战信息
	 */
	void king_war_end(bool is_normal_end, uint64_t king_family_id = 0);

	/**
	* \brief 王城争夺战结束，因为另外一个家族删除了
	*/
	void king_war_end_by_del(uint64_t del_family_id);

	/**
	 * \brief 得到竞价信息
	 */
	king_war_apply_ptr get_king_war_apply_info(uint64_t family_id);

	/**
	* \brief 添加一个符合报名要求的家族
	*/
	king_war_apply_ptr add_king_war_family(uint64_t family_id, uint32_t money);

	/**
	* \brief 删除一个竞价家族
	*/
	void del_king_war_apply_family(const family_ptr& p_family);

	/**
	 * \brief 是否是参战家族
	 */
	uint32_t get_king_war_family_type(uint64_t family_id) const;

	/**
	 * \brief 是否是助战家族
	 */
	uint32_t get_king_war_help_family_type(uint64_t family_id) const;

	/**
	 * \brief 得到助战家族id
	 */
	uint64_t get_king_war_help_family(uint32_t type) const;

	/**
	 * \brief 得到已经邀请列表
	 */
	const std::set<uint64_t>& get_king_war_invited_list(uint32_t type) const;

	/**
	* \brief 得到已经拒绝列表
	*/
	const std::set<uint64_t>& get_king_war_refused_list(uint32_t type) const;

	/**
	 * \brief 家族助战邀请
	 */
	void add_king_war_help_invited_list(uint32_t type, uint64_t family_uid);

	/**
	* \brief 家族助战拒绝列表
	*/
	void add_king_war_help_refused_list(uint32_t type, uint64_t family_uid);

	/**
	* \brief 设置助战家族
	*/
	void set_king_war_help_family(uint32_t type, const family_ptr& p_invite_family);

	/**
	 * \brief 添加宣言
	 */
	void update_king_war_notice(const global_user_ptr& p_user, const std::string& notice);

	/**
	 * \brief 是否已经点过赞或灭
	 */
	bool is_do_like(uint32_t type, uint64_t uid);

	/**
	* \brief 点赞或者踩
	*/
	void do_king_war_like(uint32_t type, uint64_t uid, uint32_t like);

	/**
	 * \brief 得到列表
	 */
	const king_war_apply_map& get_king_war_apply_list() const { return m_king_war_apply_map; }

	/**
	 * \brief 变更国王家族的一些处理
	 */
	bool set_new_king_family(uint64_t king_family_id);

	/**
	 * \brief 开始王城战报名
	 */
	void start_king_war_sign_up(uint32_t start_time, uint32_t state_end_time);

	/**
	* \brief 结束王城战报名
	*/
	void end_king_war_sign_up();

	/**
	* \brief  王城战备战开始
	*/
	void start_king_war_prepare(uint32_t state_end_time);

	/**
	* \brief  王城战备战结束
	*/
	void end_king_war_prepare();

	/**
	* \brief  王城战战斗开始
	*/
	void start_king_war_fight(uint32_t state_end_time);

	/**
	* \brief  王城战战斗结束
	*/
	void end_king_war_fight();

	/**
	 * \brief 得到该国家王城战信息
	 */
	const king_war_info& get_king_war_info() const { return m_country_info.m_king_war_info; }

	/**
	 * \brief 通知本国家王城战状态
	 */
	void notify_self_king_war_state();

	/**
	 * \brief  处理王城战结果
	 */
	void set_king_war_result(const proto::server::ge_king_war_result_notify& ntf);

	/**
	 * \brief 国家信息
	 */
	const country_info_t& get_country_info() const { return m_country_info; }

	/**
	 * \brief 王城战信息报名信息
	 */
	void get_king_war_sign_up(uint64_t family_id, proto::common::king_war_country_sign_up_single* info);

	/**
	* \brief 王城战信息对战信息
	*/
	void get_king_war_fight(uint64_t uid, proto::common::king_war_country_pre_single* info);

	/**
	 * \brief 得到家族战信息
	 */
	void peek_king_war_fight_by_type(uint32_t type, uint64_t uid, proto::common::king_war_family_info* info);

	/**
	 * \brief 重置王城战数据
	 */
	void reset_king_war();

	/**
	 * \brief 王城战信息
	 */
	void peek_king_war_state(proto::client::ec_king_war_notice_notify& ntf);

	/**
	 * \brief 是否是参战家族
	 */
	bool is_king_war_family(uint64_t family_id);

	/**
	 * \brief 设置王城战场景中人的数量
	 */
	void set_king_war_scene_role_num(uint32_t def_num, uint32_t att_num);

	/**
	 * \brief 是否被邀请助战
	 */
	uint64_t is_king_war_fight_invite(uint32_t type, uint64_t family_uid);

	/**
	 * \brief 清楚这个家族的邀请
	 */
	void del_king_war_fight_invite(uint32_t type, uint64_t family_id);

	/**
	 * \brief 清除这个家族在这个国家的所有王城战邀请
	 */
	void del_all_king_war_fight_invite(uint64_t family_id);

	/**
	 * \brief 同意助战
	 */
	void agree_king_war_fight_invite(uint32_t type, const family_ptr& p_invited_family, const family_ptr& p_family);

	/**
	 * \brief 取消邀请
	 */
	void cancel_king_war_fight_invite(uint32_t type, const family_ptr& p_invite_family);

	/**
	* \brief 取消助战
	*/
	void cancel_king_war_fight_help(uint32_t type, const family_ptr& p_family);

protected:
	uint32_t m_id = 0;								// 国家id
	country_info_t m_country_info;					// 本国的基本信息
	king_war_apply_map m_king_war_apply_map;		// 王城战竞价列表
	country_member_map m_country_member_map;		// 国家成员
	family_force_vec m_family_force_vec;			// 国家势力
	std::string m_family_like_key[king_war_type_max];// redis 缓存key
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

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
	// 初始化创建家族
	bool init(const global_user_ptr& p_user, const std::string& family_name, const std::string& declaration);

	// 加入家族
	bool join_in(uint64_t uid, uint32_t officer);
	bool join_in(const global_user_ptr& p_user, uint32_t officer);

	// 从申请列表删除
	void del_from_apply_list(uint64_t uid);

	// 离开家族
	bool leave_out(const global_user_ptr& p_user);
	bool leave_out(uint64_t uid);

	// 任命家族官职
	uint32_t assign_member(uint64_t master_id, uint64_t uid, uint32_t officer);

	// 禅让族长
	uint32_t family_demise(const global_user_ptr& p_master, uint64_t uid);

	// 加入申请列表
	bool add_appliy_list(uint64_t uid);

	// 是否已经在邀请列表cd中
	bool is_in_invite_list_cd(uint64_t uid);
	// 加入邀请列表中
	void add_in_invite_list(uint64_t uid);
	// 拒绝，从列表删除
	void del_from_invite_list(uint64_t uid);
	// 同意加入家族邀请
	uint32_t agree_invite(const global_user_ptr& p_user, uint64_t inviter_uid);

	// 拒绝某个玩家
	bool decline_apply_user(uint64_t uid);
	// 拒绝所有申请玩家
	void decline_all_apply_user();

	// 同意加入家族
	uint32_t approve_join(uint64_t master_uid, uint64_t uid, bool is_save = true);
	// 同意所有申请玩家
	uint32_t approve_all_apply_user(uint64_t master_uid, proto::client::ec_one_key_approve_join_family_reply& reply);

	// toogle 设置自动加入新军
	void toggle_aoto_newbie();

	// 设置宣言
	const std::string& update_family_declaration(const std::string& str);

	// 设置公告
	const std::string& update_family_notification(const std::string& str);

	// 升级建筑
	uint32_t upgrade_build(proto::common::family_build_type type, proto::client::ec_upgrade_family_build_reply& reply);

	// 清理建筑cd
	uint32_t clear_build_cd(proto::common::family_build_type type);

protected:
	// 加入家族
	family_member_ptr member_in(const global_user_ptr& p_user, uint32_t officer = family_officer_type_newbie, bool is_save = true);
	// 加入家族
	family_member_ptr member_in(uint64_t uid, uint32_t officer = family_officer_type_newbie);

	// 离开家族
	bool member_out(uint64_t uid);

public:
	// 家族id
	uint64_t get_family_id() const { return m_family_id; }
	// 家族等级
	uint32_t get_level() const { return m_data.level(); }
	// 家族建筑等级
	uint32_t get_build_level(proto::common::family_build_type type) const;
	// 升级等级
	void set_level(uint32_t val);
	// 家族国家id
	uint32_t get_country_id() const { return m_data.country(); }
	// 是否自动设置xinjun
	bool is_auto_newbie_state() const { return m_data.auto_newbie(); }
	// 家族名字
	const std::string& get_name() const { return m_data.name(); }
	// 得到族长id
	uint64_t get_general_id() const { return common::string_util_t::string_to_uint64(m_data.patriarch()); };
	// 得到副组长id
	uint64_t get_deputy_general_id() const;
	// 得到繁荣度
	uint32_t get_prosperity() const { return m_data.prosperity(); }
	// 得到威望
	uint32_t get_prestige() const { return m_data.prestige(); }
	// 创建时间
	uint32_t get_create_time() const { return m_create_time; }
	// 忠诚度
	uint32_t get_loyalty() const { return m_data.loyalty(); }

	family_member_ptr get_family_member(uint64_t uid);

	family_build_ptr get_family_build(proto::common::family_build_type type);

	// 家族资金
	uint32_t get_money() const { return m_data.money(); }
	// 增加家族资金
	void add_money(uint32_t val, uint32_t type, uint64_t uid = 0);
	// 消耗资金
	void cost_money(uint32_t val, uint32_t type, uint64_t uid = 0);
	
	// 设置维护费用
	void set_maintain(uint32_t cost, bool is_low);

	// 是否自动加入
	bool is_auto_newbie(uint32_t level);

	// 得到各个职位的数量
	uint32_t get_officer_count(uint32_t officer);

	// peek 数据
	void peek_data(proto::common::family_list_info* p_data);
	void get_family_info(proto::client::ec_get_family_info_reply& reply);
	void get_family_member_list(proto::client::ec_get_family_member_list_reply& reply);
	void get_family_member_list(proto::client::ec_open_appoint_reply& reply);
	void get_family_apply_list(proto::client::ec_family_apply_list_reply& reply);
	void get_family_build_list(proto::client::ec_get_family_build_info_reply& reply);

	// 组织祈愿数据
	void get_family_prayer_list(proto::common::family_prayer_list* p_list);

public:
	// 更新成员数量
	void update_member_size();
	// 家族成员数量
	uint32_t get_max_member_size() const { return m_data.member_cnt_max(); }
	uint32_t get_max_newbie_size() const { return m_newbie_cnt_max; }
	// 家族成员数
	uint32_t get_member_size() const { return m_member_map.size(); }
	// 家族成员是否满了
	bool is_member_full() const { return m_member_map.size() >= get_max_member_size(); }
	// 所有家族成员
	const family_member_map& get_all_member() const { return m_member_map; }

	// 更新家族维护费，每天计算是否是低维护状态
	void update_maintain_cost();
	uint32_t get_normal_maintain_cost();
	// 家族维护费
	uint32_t get_maintain_cost() const { return m_data.matain_cost(); }
	// 是否处于低维护状态
	bool is_low_maintenance() const { return m_is_low_maintenance; }

	// 更新最大资金
	void update_max_money();
	// 资金上限
	uint32_t get_max_money() const { return m_data.max_money(); }

	// 更新繁荣度
	void update_prosperity();

	// 更新威望
	void update_prestige_daily();

	// 学徒自动成为正式成员
	void auto_newbie_member();

	// 更新成员在线状态
	void update_member_online_state(const global_user_ptr& p_user);

	// 属性变化通知
	void update_member_attr(const global_user_ptr& p_user);

	// 家族贡献增加
	void add_family_contribution(uint64_t uid, uint32_t val);

	// 更新荣耀值
	void update_member_glory(uint64_t role_uid, uint32_t glory);
public:
	// 是否处于弹劾
	bool is_impeacher() const { return m_impeacher != 0; }
	// 更新弹劾
	void update_impeacher();

	//更新家族荣耀值并且刷新排行
	void update_glory();
	// 家族战
public:
	uint32_t get_family_war_member_cnt(uint32_t level);

	// 国家
public:
	// 家族改变了国家处理
	void change_country(uint32_t new_country_id);

	// 是否是国王家族
	bool is_king_family() const { return m_is_king_family; }

	// 设置是否是国王家族
	void set_king_family(bool is_king) { m_is_king_family = is_king; }

	// 设置是否是国王家族
	void change_king_family(bool is_king);

	bool check_quit_country_times();

	// 得到本次王城战的报名国家id
	uint32_t get_king_war_apply_country() const { return m_king_war_apply_country; }

	// 设置王城战报名国家id
	void set_king_war_apply_country(uint32_t country_id);

	// 得到本次王城战的参战国家id
	uint32_t get_king_war_fight_country() const { return m_king_war_fight_country; }

	// 设置王城战参战国家id
	void set_king_war_fight_country(uint32_t country_id);

	// 家族商店
public:
	// 检测是否要刷新商店
	bool check_shop_expired();
	// 商店数据
	void peek_family_shop_data(proto::common::family_shop_data* p_data);
	// 商店数据
	const family_shop_t& get_family_shop() const { return m_family_shop; }
	// 购买商品
	uint32_t family_shop_buy(const global_user_ptr& p_user, uint32_t index, uint32_t shop_item, uint32_t num);
	// 购买失败回退次数
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

	// 给所有在线成员发送系统广播
	void send_sys_notice_to_member(uint32_t sys_tid);

	// 给所有玩家发送系统邮件
	void send_sys_mail_to_member(uint32_t mail_tid);
public:
	// 通知发现隐藏层
	void tower_trigger_notify(uint64_t role_uid, const proto::server::ge_tower_trigger_notify& msg);
protected:
	uint64_t m_family_id = 0;					// 家族唯一id
	proto::common::family_data m_data;			// 一些基础信息
	uint32_t m_newbie_cnt_max = 0;				// 学徒成员上限
	bool m_is_king_family = false;				// 是否是国王家族
	uint32_t m_quit_time = 0;					// 退出同盟时间
	uint32_t m_quit_count = 0;					// 退出同盟次数

	family_member_map m_member_map;				// 家族成员
	officer_map m_officer_map;					// 家族官职列表

	family_apply_map m_apply_map;				// 家族申请列表

	family_build_map m_build_map;				// 家族建筑列表

	uint32_t m_online_prosperity_add_daily = 0;	// 今日家族繁荣度上线加成
	uint32_t m_online_prosperity_daily_max = 0;	// 今日家族繁荣度上线加成上限
	uint32_t m_chat_prosperity_add_daily = 0;	// 今日家族繁荣度说话加成
	uint32_t m_chat_prosperity_daily_max = 0;	// 今日家族繁荣度说话加成上限
	uint32_t m_task_prosperity_add_daily = 0;	// 今日家族繁荣度任务加成
	uint32_t m_task_prosperity_daily_max = 0;	// 今日家族繁荣度任务加成上限

	uint32_t m_prestige_add_daily = 0;			// 今日家族威望统计

	bool m_is_low_maintenance = false;			// 是否处于低维护状态

	uint32_t m_create_time = 0;					// 家族创建时间

	uint32_t m_glory = 0;						// 家族荣耀值（不保存数据库，实时根据成员荣耀值计算出来的总值用于家族荣耀排行）
protected:
	// 弹劾相关
	uint64_t m_impeacher = 0;					// 发起弹劾人
	uint32_t m_impeach_time = 0;				// 发起时间
	uint32_t m_impeacher_cnt = 0;				// 相应人数

	// 邀请列表<uid,time_out>
	invite_list_map m_invite_list_map;

protected:
	family_shop_t m_family_shop;

protected:
	uint32_t m_king_war_apply_country = 0;			// 王城战报名国家
	uint32_t m_king_war_fight_country = 0;		// 王城战参战国家
};

typedef boost::shared_ptr<family> family_ptr;
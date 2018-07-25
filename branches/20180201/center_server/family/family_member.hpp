#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "family_common.hpp"
#include "common.pb.h"
#include "user/global_user.hpp"
#include <map>

class family_member;
typedef boost::shared_ptr<family_member> family_member_ptr;

class family_member : public boost::enable_shared_from_this<family_member>
{
	friend class family_data_mgr_t;

public:
	family_member(uint64_t uid, uint64_t family_id);
	~family_member() {};

public:
	bool init(const global_user_ptr& p_user, uint32_t officer);

	bool init(uint32_t officer);

	bool update_officer(uint32_t new_officer);

	// 设置在线
	void set_online();
	// 设置离线
	void set_offline(uint32_t last_time);

	// 人物熟悉变化
	void update_attr(const global_user_ptr& p_user);

	void peek_data(proto::common::family_member* p_data);
	void peek_prayer_data(proto::common::family_prayer_data* p_data);
	void peek_prayer_times_data(proto::client::ec_family_prayer_list_reply& reply);

public:
	uint64_t get_uid() const { return m_uid; }

	// 是否在线
	bool is_online() const;

	// 等级
	uint32_t get_level() const { return m_base.level(); }

	// 历史贡献
	uint32_t get_total_contribution() const { return m_base.total_family_contribution(); }
	// 增加历史贡献
	void add_total_contribution(uint32_t val);

	// 得到加入时间
	uint32_t get_join_time() const { return m_join_time; }

	// 职位
	uint32_t get_officer() const { return m_base.officer(); }

	// 名字
	const std::string& get_name() const { return m_base.role_name(); }

	// 头像
	uint32_t get_plugin() const { return m_base.plugin(); }

	// 荣耀值
	void set_glory(uint32_t glory) { m_base.set_glory(glory); }
	uint32_t get_glory() { return m_base.glory(); }

	// 是否有祈愿信息
	bool is_have_prayer_hero();

	// 得到赠与次数
	uint32_t get_prayer_gift_times();

	// 得到赠送和获得记录
	void get_prayer_record_list(proto::common::family_prayer_record_list* p_list);

	// 得到祈愿次数
	uint32_t get_prayer_times();

	// 得到奖励领取次数
	uint32_t get_prayer_rwd_times();

	// 请求祈愿
	void ask_prayer_hero(uint32_t hero_tid, uint32_t num);

	// 赠予碎片
	uint32_t ask_gift_hero(const family_member_ptr& p_family_member, uint32_t hero_tid, uint32_t hero_material_id, uint32_t rarity, uint32_t hero_num);

	// 赠予别人成功的处理(返回赠予奖励道具id)
	void do_gift_reward(const family_member_ptr& p_family_member, uint32_t hero_tid, uint32_t hero_material_id, uint32_t rarity, uint32_t hero_num, uint32_t& gift_rwd_item);

	// 家族商店限购信息
public:
	// 得到限购信息
	const proto::common::family_shop_personal_data& get_shop_personal_data() const { return m_shop_personal_data; }

	// 得到个人限购数据
	void peek_family_shop_personal_data(proto::common::family_shop_personal_data* p_data);

	// 返回当前购买数量
	uint32_t get_personal_buy_count(uint32_t index);

	// 增加限购数量
	void add_personal_buy_count(uint32_t refresh_time, uint32_t index, uint32_t num);

	// 失败回退
	void personal_failed_back(uint32_t refresh_time, uint32_t index, uint32_t num);

private:
	// 更新
	void prayer_day_update(bool is_save = true);
	void prayer_week_update(bool is_save = true);

	// 赠与次数
	uint32_t get_gift_time(uint64_t uid);
	// 增加本轮赠与次数
	void add_gift_time(uint64_t uid);

	// 增加祈愿贡献
	void add_prayer_contribution(uint32_t val);

	// 增加周赠与次数并给予奖励
	void add_prayer_gift_times();

	// 检测限购是否过期
	void check_shop_time_out();

protected:
	uint64_t m_uid = 0;						// 玩家uid
	uint64_t m_family_id = 0;				// 家族id
	proto::common::family_member m_base;	// 基础信息，用于发给客户端
	uint32_t m_join_time = 0;				// 加入家族时间
	family_impeach_type m_is_impeach_ack = family_impeach_type_none;	 // 弹劾响应

protected:
	// 祈愿次数的数据（这些数据在离开家族后不删除）
	uint32_t m_last_prayer_day_time = 0;
	uint32_t m_prayer_times = 0;
	uint32_t m_prayer_rwd_times = 0;
	uint32_t m_last_prayer_week_time = 0;
	uint32_t m_prayer_gift_times = 0;

	uint32_t m_prayer_hero_tid = 0;			// 祈愿武将tid
	uint32_t m_prayer_hero_num = 0;			// 祈愿武将当前数量
	uint32_t m_prayer_hero_max = 0;			// 祈愿武将最大数量
	uint32_t m_prayer_start_time = 0;		// 祈愿开始时间
	uint32_t m_prayer_complete_time = 0;	// 祈愿完成后什么时候把这个记录删除
	proto::common::family_gift_data_list m_gift_list;		// 记录赠送的人的次数
	std::map<uint64_t, uint32_t> m_gift_list_map;
	proto::common::family_prayer_record_list m_record_list; // 赠送和获得记录

protected:
	proto::common::family_shop_personal_data m_shop_personal_data;
};
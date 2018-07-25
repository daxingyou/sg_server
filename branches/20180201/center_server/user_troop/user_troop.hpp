#ifndef __USER_TRROP_HPP__
#define __USER_TRROP_HPP__

#include "macros.hpp"
#include <map>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "common.pb.h"
#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"

class troop_t
{
public:
	troop_t(uint64_t troop_uid, uint64_t leader_uid);

	struct troop_member
	{
		troop_member(uint64_t uid) : m_uid(uid) {
			m_base.set_id(common::string_util_t::uint64_to_string(uid));
		}

		uint64_t get_uid() const { return m_uid; }

		void set_name(const std::string& val) { m_base.set_name(val); }
		const std::string& get_name() const { return m_base.name(); }

		void set_plugin(uint32_t val) { m_base.set_plugin(val); }
		uint32_t get_plugin() const { return m_base.plugin(); }

		void set_fighting(uint32_t val) { m_base.set_fighting(val); }
		uint32_t get_fighting() const { return m_base.fighting(); }

		void set_level(uint32_t val) { m_base.set_level(val); }
		uint32_t get_level() const { return m_base.level(); }

		void set_state(uint32_t val) { m_base.set_state(val); }
		uint32_t get_state() const { return m_base.state(); }

		void peek_data(proto::common::user_troop_member* p_data) { 
			if (p_data) p_data->CopyFrom(m_base); 
		}

		uint64_t m_uid = 0;//玩家uid
		proto::common::user_troop_member m_base;
	};

	typedef boost::shared_ptr<troop_member> troop_member_ptr;
	typedef std::vector<troop_member_ptr> troop_member_vec;
	typedef std::list<troop_member_ptr> troop_apply_list;

public:
	// 场景同步
	void sync_to_all_game(uint32_t event_id, uint64_t member_id);

public:
	void peek_data(uint32_t event_id, uint64_t member_id, proto::common::troop_data* p_data);
	void peek_data(proto::common::troop_data* p_data);
	void peek_apply_member_data(proto::client::ec_troop_apply_member_reply& reply);
	void peek_zg_task_data(proto::common::task_state *p_data);
public:
	uint64_t get_uid() const { return m_uid; }

	troop_member_ptr get_troop_member(uint64_t member_uid);

	// 加入一个队员并设置改队员队伍id
	// 除了创建队伍，其他情况加入队伍都是暂离状态
	bool add_member(const global_user_ptr& p_user, proto::common::troop_state_type type);
	// 删除一个队员
	uint32_t remove_member(const global_user_ptr& p_member_user, uint32_t event_id);

	// 加入申请队列
	void add_apply_member(const global_user_ptr& p_user);

	// 是否自动加入
	bool is_free_join() const { return m_is_free_join; }

	// 队长id
	uint64_t get_leader_id() const { return m_leader_id; }
	void set_leader_id(uint64_t leader_id) { m_leader_id = leader_id; }

	// 队伍类型
	uint32_t get_type() const { return m_type; }
	void set_type(uint32_t type) { m_type = type; }

	// 队伍等级设置
	void set_level_limit(uint32_t level_min, uint32_t level_max) { m_level_min = level_min; m_level_max = level_max; }
	uint32_t get_level_min() const { return m_level_min; }
	uint32_t get_level_max() const { return m_level_max; }

	// 是否满员
	bool is_full() const { return m_troop_members.size() >= 3; }
	// 是否空队伍
	bool is_empty() const { return m_troop_members.empty(); }

	// 随机指定一个新队长
	bool random_new_leader();
	// 强行指定一个新队长
	bool appoint_new_leader(uint64_t uid);

	void update_member_level(uint64_t member_uid, uint32_t level);
	void update_member_online(const global_user_ptr& p_user, uint32_t online);
	void update_member_fighting(uint64_t member_uid, uint32_t fighting);

	uint32_t sync_task_info(const global_user_ptr& p_user, const proto::common::task_state &task_info);
public:
	// 请求加入队伍
	uint32_t ask_join(const global_user_ptr& p_user);

	// 请求入队处理
	uint32_t process_apply(uint64_t uid, uint32_t process);

	// 同意入队处理返回
	uint32_t agree_apply(const global_user_ptr& p_member);

	// 从邀请列表删除
	void del_invite(uint64_t uid);

	// 邀请处理
	void invite_join(const global_user_ptr& p_invite_user, const global_user_ptr& p_member);
	// 相应邀请
	uint32_t process_invite(const global_user_ptr& p_user, uint32_t process);

	// 设置自动加入
	uint32_t toggle_join_flag(uint64_t uid);

	// 设置设置暂离
	uint32_t set_leave_troop(const global_user_ptr& p_user);
	uint32_t back_troop(const global_user_ptr& p_user);

	// 交换队长
	uint32_t transfer_leader(const global_user_ptr& p_user, uint64_t member_uid);

	// 强制删除队伍，清理队员
	void do_dissolve();

	// 队员传送成功
	void do_transmission_result(uint64_t uid, uint32_t result);

protected:
	static const uint32_t TROOP_APPLY_MAX_NUM = 100;

protected:
	uint64_t m_uid = 0;
	uint64_t m_leader_id = 0;
	bool m_is_free_join = true;		// 默认自动加队
	uint32_t m_type = 0;
	uint32_t m_level_min = 0;
	uint32_t m_level_max = 0;
	proto::common::task_state m_zg_task_info;	//抓鬼任务信息

	troop_member_vec m_troop_members;
	troop_apply_list m_apply_members; // 申请列表
	uint32_t m_apply_num = 0;	// 记录下最大申请人数
	std::map<uint64_t, uint32_t> m_invite_list;	// 邀请
};

typedef boost::shared_ptr<troop_t> troop_ptr;
typedef boost::weak_ptr<troop_t> troop_wptr;

#endif

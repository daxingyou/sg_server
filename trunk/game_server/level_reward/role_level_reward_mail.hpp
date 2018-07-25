#ifndef __ROLE_LEVEL_REWARD_MAIL_H__
#define __ROLE_LEVEL_REWARD_MAIL_H__

#include "macros.hpp"
#include "protos_fwd.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <set>

/*
* 等级奖励邮件
* 本模块只需要发送邮件，不需要通知客户端。
*/

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class role_level_reward_mail_t
{
public:
	role_level_reward_mail_t(uint64_t uid);
	~role_level_reward_mail_t();
public:
	void load_data(const proto::common::level_reward_mail_recived_list & data);
	void peek_data(proto::common::level_reward_mail_recived_list &p_data);
	void save_data();
public:
	uint32_t send_level_reward_mail(uint32_t id, role_ptr p_role);

	//检测是否需要补领
	void check_send_level_reward_mail( role_ptr p_role);

	//检测是否领取过
	bool has_send_level_reward_mail(uint32_t id);
public: 
	uint64_t m_role_uid = 0;
	std::string m_key = "";
	std::set<uint32_t> m_recived_set;
};

#endif

#ifndef __CENTER_MAIL_DATA_H__
#define __CENTER_MAIL_DATA_H__

#include <map>
#include <set>

#include "macros.hpp"
#include "mail_common.hpp"
#include "common.pb.h"
#include "client.pb.h"
#include "server.pb.h"
#include <boost/shared_ptr.hpp>

typedef std::map<uint64_t, proto::common::mail_base_info> mail_base_map;

class global_user_t;
typedef boost::shared_ptr<global_user_t> global_user_ptr;

class mail_user_data_t
{
public:
	mail_user_data_t(uint64_t role_uid);

	// 上线邮件处理
	void on_login_load();

	// 自动清理已读邮件
	void clear_opened_mail();
	// 

	// 删除超时邮件
	void clear_time_out_mail();

	// 强制删除邮件（删除数量）
	void force_del_mail(uint32_t num = 1);

	// 清空
	void clear();

	// 是否有未读邮件
	bool is_have_new();

	// 是否已打开
	bool is_have_new_mail();

	// 设置邮件被领取并且删除
	void set_mail_get_delete(uint64_t mail_id, proto::client::ec_mail_state_change_notify& notify);

	// 清理邮件
	void clear_mail(EM_MAIL_DEL_TYPE type = MAIL_DEL_USER);

public:
	// 得到邮件列表
	void get_mail_list(proto::client::ec_mail_list_notify& notify);

	// 得到邮件内容
	void get_mail_content(uint64_t mail_id, proto::client::ec_mail_content_reply& reply);

	// 删除邮件
	void del_mail_by_id(uint64_t mail_id, proto::client::ec_mail_state_change_notify& notify, EM_MAIL_DEL_TYPE type = MAIL_DEL_USER);

	// 清理邮件
	void clear_mail(proto::client::ec_mail_state_change_notify& notify, EM_MAIL_DEL_TYPE type = MAIL_DEL_USER);

	// 强制删除邮件（删除数量）,通知client
	//void force_del_mail(proto::client::ec_mail_state_change_notify& notify, uint32_t num = 1);

	// 领取邮件附件，发送给game
	uint32_t mail_get_send_to_game(uint64_t mail_id, global_user_ptr p_user);

	// 领取附件，game返回
	void mail_get_game_back(global_user_ptr p_user, const proto::server::ge_mail_get_addenda_reply& reply);

	// 一键领取所有邮件
	void get_all_mails(global_user_ptr p_user);

	// 新邮件
	bool add_new_mail(const new_mail_define& mail_define, global_user_ptr p_user);

	//添加全局新邮件
	bool add_new_global_mail(const new_mail_define& mail_define, global_user_ptr p_user);

	// 是否已经有全局邮件
	bool is_have_global_mail(uint64_t id) { return m_global_mail_map.find(id) != m_global_mail_map.end(); }

	void set_have_global_mail(uint64_t id) { m_global_mail_map.insert(id);  }

private:
	mail_base_map m_mail_map;
	uint64_t m_role_uid = 0;
	uint32_t m_force_del_num = 0;

	std::set<uint64_t> m_global_mail_map;
};

#endif

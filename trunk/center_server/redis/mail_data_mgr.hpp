#ifndef __MAIL_DATA_MGR_H__
#define __MAIL_DATA_MGR_H__

#include "common.pb.h"
#include "client.pb.h"
#include "mail/mail_user_data.hpp"
#include "mail/global_mail_manager.hpp"

class mail_data_mgr_t
{
public:
	// 加载所有邮件
	static void load_user_mails(uint64_t uid, mail_base_map& mail_map, std::set<uint64_t>& global_mail_set);
	// 设置邮件删除标志，将在下一次清理时候删除
	static void set_mail_delete(uint64_t mail_id, uint64_t uid, EM_MAIL_DEL_TYPE type);
	// 邮件被领取并设置删除标志，将在下一次清理时候删除
	static void set_mail_get_delete(uint64_t mail_id, uint64_t uid, EM_MAIL_DEL_TYPE type);
	// 邮件是否存在
	static bool is_mail_exsit(uint64_t mail_id);
	// 得到邮件附件信息
	static bool get_mail_content(uint64_t mail_id, uint64_t uid, proto::client::ec_mail_content_reply& reply);
	// 检查邮件是否可以被领取（防止被多次领取）
	static bool check_can_get(uint64_t mail_id, uint32_t now_time);
	// 得到附件信息，并设置正在领取时间
	static void get_mail_addenda(uint64_t mail_id, uint32_t now_time, proto::server::eg_mail_get_addenda_request& addenda);
	// 新邮件
	static bool add_new_mail(const new_mail_define& mail_define);
	// 清除正在被领取的标志位
	static void clear_get_mask(uint64_t mail_id);

public:
	// 加载所有全局邮件
	static bool load_global_mails(global_mail_ptr_map& global_mail_map);
	// 添加一个新的全局邮件
	static bool add_new_global_mail(global_mail_ptr p_mail);
};
#endif
#ifndef __CENTER_MAIL_MANAGER_H__
#define __CENTER_MAIL_MANAGER_H__

#include "macros.hpp"
#include "mail_common.hpp"

#include "server.pb.h"
#include "common.pb.h"

#include <map>
#include <vector>

class mail_manager_t
{
public:
	// 生成一个邮件唯一ID
	static uint64_t get_new_mail_uid();
	// 每天凌晨删除过期和具有删除标志的邮件
	static void clear_old_mail();
public:
	// 给一个玩家发系统邮件（邮件内容和附件全走邮件配置），适合配置好的系统邮件
	static void send_mail(uint64_t to_uid, proto::common::em_mail_type mail_type, uint32_t content_id,
		proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// 给一组玩家发系统邮件（邮件内容和附件全走邮件配置），适合配置好的系统邮件
	static void send_mail(const std::vector<uint64_t>& to_uids, proto::common::em_mail_type mail_type, uint32_t content_id,
		proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// 给一个玩家发系统邮件（邮件内容走配置，附件道具自己定义），适合背包满给道具
	static void send_mail(uint64_t to_uid, proto::common::em_mail_type mail_type, uint32_t content_id,
		const std::map<uint32_t, uint32_t>& item_map, proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// 给一组玩家发系统邮件（邮件内容走配置，附件道具自己定义），适合背包满给道具
	static void send_mail(const std::vector<uint64_t>& to_uids, proto::common::em_mail_type mail_type, uint32_t content_id,
		const std::map<uint32_t, uint32_t>& item_map, proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// 给一个玩家发送一个完全自己定义内容的邮件
	static void send_mail(uint64_t to_uid, proto::common::em_mail_type mail_type,
		uint64_t from_uid, const std::string& from_name,
		const std::string& title, const std::string& content,
		const std::map<uint32_t, uint32_t>& item_map,
		proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// 给一组玩家发送一个完全自己定义内容的邮件
	static void send_mail(const std::vector<uint64_t>& to_uids, proto::common::em_mail_type mail_type,
		uint64_t from_uid, const std::string& from_name,
		const std::string& title, const std::string& content,
		const std::map<uint32_t, uint32_t>& item_map,
		proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

public:
	// 邮件处理
	static void send_new_mail(const new_mail_define& mail_define);

	static void send_new_global_mail(const new_mail_define& mail_define);
};

#endif
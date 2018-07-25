#ifndef __CENTER_GLOBAL_MAIL_MANAGER_H__
#define __CENTER_GLOBAL_MAIL_MANAGER_H__

#include "macros.hpp"
#include "mail_common.hpp"
#include <boost/shared_ptr.hpp>

#include <map>
#include <set>

class global_user_t;
typedef boost::shared_ptr<global_user_t> global_user_ptr;

typedef boost::shared_ptr<global_mail_t> global_mail_ptr;
typedef std::map<uint64_t, global_mail_ptr> global_mail_ptr_map;

class global_mail_manager_t
{
public:
	static bool init();
	static bool close_clear();

public:
	// 检测玩家
	static bool check_user_new(global_user_ptr p_user);

public:
	// 添加一个自定义的全局邮件
	static bool add_new_global_mail(uint32_t type,	// 全局邮件类型
		uint32_t param1,							// 附加参数1
		uint32_t param2,							// 附加参数2
		std::set<uint64_t>& uids,				// type=GLOBAL_MAIL_USERS 时填写指定玩家的id
		uint32_t start_time,						// 开始时间
		uint32_t end_time,						// 结束时间 0 表示一直有效
		const std::string& from_name,				// 发信人
		const std::string& title,					// 标题
		const std::string& content,				// 内容
		const std::string& items,					// 附件（itemid:num$itemid:num）
		const std::string& desc					// 备注
	);

public:
	static global_mail_ptr_map m_global_mail_map;
};

#endif

#ifndef __CENTER_MAIL_COMMON_H__
#define __CENTER_MAIL_COMMON_H__

#include "common.pb.h"
#include "macros.hpp"

#include <set>

#define MAX_MIAL_SIZE	100			// 最大邮件数

#define MAX_MAIL_TIME (3600*24*30)	// 邮件持续时间

#define GET_MAIL_COOLTIME 200	// 领取邮件冷却时间 单位毫秒

// 邮件删除类型
enum EM_MAIL_DEL_TYPE
{
	MAIL_DEL_USER			= 1,		// 玩家自己删除的
	MAIL_DEL_TIME_OUT		= 2,		// 过期
	MAIL_DEL_TOO_MUCH		= 3,		// 邮件太多强制删除最老的一封
	MAIL_DEL_GET			= 4,		// 邮件领取删除
	MAIL_DEL_OPEN			= 5,		// 没有附件的邮件打开删除
	MAIL_DEL_GM			= 6,		// GM 强制删除
};

// 全局邮件类型
enum EM_GLOBAL_MAIL_TYPE
{
	GLOBAL_MAIL_NONE		= 0,		// 无效
	GLOBAL_MAIL_ALL			= 1,		// 给所有人
	GLOBAL_MAIL_USERS		= 2,		// 给制定ID的玩家
	GLOBAL_MAIL_AREAN		= 3,		// 竞技场处理
	GLOBAL_MAIL_GM			= 5,		// gm发的全服邮件

	GLOBAL_MAIL_MAX,
};

struct new_mail_define
{
	uint64_t mail_id;							// id
	proto::common::em_mail_state mail_state;		// 邮件状态
	proto::common::em_mail_type mail_type;		// 邮件类型
	uint64_t global_id;						// 全局邮件id
	uint64_t to_uid;							// 收信id
	uint64_t from_uid;						// 发信id
	std::string from_name;						// 发信名字
	uint32_t create_time;						// 发信时间
	uint32_t del_time;						// 删除时间
	uint32_t content_id;						// 邮件配置
	std::string title;						// 标题
	std::string content;						// 内容
	std::string items;						// 附件
	proto::common::em_mail_addenda item_state;	// 附件状态
	proto::common::em_mail_get_type get_type;	// 获得类型
	//std::string desc;							// 备注

	void reset()
	{
		mail_id = 0;
		mail_state = proto::common::MAIL_STATE_NEW;
		mail_type = proto::common::MAIL_TYPE_SYS;
		global_id = 0;
		to_uid = 0;
		from_uid = 0;
		from_name.clear();
		create_time = 0;
		del_time = 0;
		content_id = 0;
		title.clear();
		content.clear();
		items.clear();
		item_state = proto::common::MAIL_ADDENDA_NONE;
		get_type = proto::common::MAIL_GET_TYPE_SYS;
		//desc.clear();
	}
};

struct global_mail_t
{
	uint64_t mail_id;					// ID
	uint32_t mail_type;				// 全局邮件类型 
	uint32_t mail_param1;				// 附加参数1
	uint32_t mail_param2;				// 附加参数2
	std::set<uint64_t> mail_uids;		// 当邮件为制定玩家的时候，放玩家id
	uint32_t start_time;				// 全局邮件有效时间
	uint32_t end_time;				// 全局邮件有效时间
	std::string from_name;				// 发信人
	std::string title;				// 标题
	std::string content;				// 内容
	std::string items;				// 附件
	std::string desc;					// 备注

	global_mail_t() : mail_id(0), mail_type(GLOBAL_MAIL_NONE), mail_param1(0), mail_param2(0), start_time(0), end_time(0) {};

	bool is_have_uid(uint64_t id) { 
		if (mail_uids.empty()) {
			return true;
		}
		return mail_uids.find(id) != mail_uids.end(); 
	}
};

#endif
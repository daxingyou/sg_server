#ifndef __FRIEND_COMMON_H__
#define __FRIEND_COMMON_H__

#include "macros.hpp"

#include <boost/shared_ptr.hpp>
#include "common.pb.h"
#include "log.hpp"
#include "utility.hpp"
USING_NS_COMMON;

#define friend_recommand_level_near 5

// 日志辅助类型
enum EM_FRIEND_PARAM
{
	FRIEND_CLEAR_LONG_TIME = 1,		// 清理7天未上线
	FRIEND_CLEAR_RECENT_CONTACT = 2,	// 清理最近联系人
	FRIEND_CLEAR_BLACK_FRIENDS = 3,		// 清理黑名单列表
	FRIEND_CLEAR_FRIEND_APPLIERS = 4,	// 清理申请列表
	FRIEND_CANCEL_BLACK = 5,			// 取消拉黑
	FRIEND_APPLY_TOO_MUCH = 6,			// 申请列表过多
	FRIEND_APPLY_TOO_MUCH_ADN_DEL = 7,	// 申请列表过多，清理最早的，并删除社会关系
	FRIEND_CONTACT_TOO_MUCH = 8,		// 最近联系人列表过多
	FRIEND_CONTACT_TOO_MUCH_ADN_DEL = 9, // 最近联系人列表过多，清理最早的，并删除社会关系
	FRIEND_ROLE_DEL = 10,				// 玩家删除一个好友
	FRIEND_ADD_FRIEND_ME = 11,			// 有玩家关注我，自动加我好友申请人
};

// 好友信息，把protobuf数据封一层
struct friend_info_t
{
	uint64_t m_friend_id;
	std::string m_redis_key;
	uint64_t m_rt;	// 用户信息最后更新时间
	proto::common::friend_member m_member;

	friend_info_t() : m_friend_id(0), m_rt(0) {};
};

typedef boost::shared_ptr<friend_info_t> friend_info_ptr;

struct friend_contact_t
{
	// 升序
	bool operator() (const friend_info_ptr& lptr, const friend_info_ptr& rptr) const
	{
		if (lptr != NULL && rptr != NULL)
		{
			if (lptr->m_member.contact_time() < rptr->m_member.contact_time())
			{
				return true;
			}
			else if (lptr->m_member.contact_time() > rptr->m_member.contact_time())
			{
				return false;
			}
			else
			{
				return lptr->m_friend_id > rptr->m_friend_id;
			}
		}
		else
		{
			log_error("friend_contact_t error!");
			return false;
		}
	}
};

struct friend_apply_t
{
	// 升序
	bool operator() (const friend_info_ptr& lptr, const friend_info_ptr& rptr) const
	{
		if (lptr != NULL && rptr != NULL)
		{
			if (lptr->m_member.time() < rptr->m_member.time())
			{
				return true;
			}
			else if (lptr->m_member.time() > rptr->m_member.time())
			{
				return false;
			}
			else
			{
				return lptr->m_friend_id > rptr->m_friend_id;
			}
		}
		else
		{
			log_error("friend_apply_t error!");
			return false;
		}
	}
};

#endif

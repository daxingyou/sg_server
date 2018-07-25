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
	// ������
	static bool check_user_new(global_user_ptr p_user);

public:
	// ���һ���Զ����ȫ���ʼ�
	static bool add_new_global_mail(uint32_t type,	// ȫ���ʼ�����
		uint32_t param1,							// ���Ӳ���1
		uint32_t param2,							// ���Ӳ���2
		std::set<uint64_t>& uids,				// type=GLOBAL_MAIL_USERS ʱ��дָ����ҵ�id
		uint32_t start_time,						// ��ʼʱ��
		uint32_t end_time,						// ����ʱ�� 0 ��ʾһֱ��Ч
		const std::string& from_name,				// ������
		const std::string& title,					// ����
		const std::string& content,				// ����
		const std::string& items,					// ������itemid:num$itemid:num��
		const std::string& desc					// ��ע
	);

public:
	static global_mail_ptr_map m_global_mail_map;
};

#endif

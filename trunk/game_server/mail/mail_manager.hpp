#ifndef __GAME_MAIL_MANAGER_H__
#define __GAME_MAIL_MANAGER_H__

#include "server.pb.h"
#include "macros.hpp"
#include "common.pb.h"

#include <map>
#include <vector>

class mail_manager_t
{
public:
	// ��һ����ҷ�ϵͳ�ʼ����ʼ����ݺ͸���ȫ���ʼ����ã����ʺ����úõ�ϵͳ�ʼ�
	static void send_mail(uint64_t to_uid, proto::common::em_mail_type mail_type, uint32_t content_id, 
		proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// ��һ����ҷ�ϵͳ�ʼ����ʼ����ݺ͸���ȫ���ʼ����ã����ʺ����úõ�ϵͳ�ʼ�
	static void send_mail(const std::vector<uint64_t>& to_uids, proto::common::em_mail_type mail_type, uint32_t content_id,
		proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// ��һ����ҷ�ϵͳ�ʼ����ʼ����������ã����������Լ����壩���ʺϱ�����������
	static void send_mail(uint64_t to_uid, proto::common::em_mail_type mail_type, uint32_t content_id, 
		const std::map<uint32_t, uint32_t>& item_map, proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// ��һ����ҷ�ϵͳ�ʼ����ʼ����������ã����������Լ����壩���ʺϱ�����������
	static void send_mail(const std::vector<uint64_t>& to_uids, proto::common::em_mail_type mail_type, uint32_t content_id,
		const std::map<uint32_t, uint32_t>& item_map, proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// ��һ����ҷ���һ����ȫ�Լ��������ݵ��ʼ�
	static void send_mail(uint64_t to_uid, proto::common::em_mail_type mail_type,
		uint64_t from_uid, const std::string& from_name,
		const std::string& title, const std::string& content,
		const std::map<uint32_t, uint32_t>& item_map,
		proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);

	// ��һ����ҷ���һ����ȫ�Լ��������ݵ��ʼ�
	static void send_mail(const std::vector<uint64_t>& to_uids, proto::common::em_mail_type mail_type,
		uint64_t from_uid, const std::string& from_name,
		const std::string& title, const std::string& content,
		const std::map<uint32_t, uint32_t>& item_map,
		proto::common::em_mail_get_type get_type = proto::common::MAIL_GET_TYPE_SYS);
};

#endif
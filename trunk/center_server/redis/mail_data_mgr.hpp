#ifndef __MAIL_DATA_MGR_H__
#define __MAIL_DATA_MGR_H__

#include "common.pb.h"
#include "client.pb.h"
#include "mail/mail_user_data.hpp"
#include "mail/global_mail_manager.hpp"

class mail_data_mgr_t
{
public:
	// ���������ʼ�
	static void load_user_mails(uint64_t uid, mail_base_map& mail_map, std::set<uint64_t>& global_mail_set);
	// �����ʼ�ɾ����־��������һ������ʱ��ɾ��
	static void set_mail_delete(uint64_t mail_id, uint64_t uid, EM_MAIL_DEL_TYPE type);
	// �ʼ�����ȡ������ɾ����־��������һ������ʱ��ɾ��
	static void set_mail_get_delete(uint64_t mail_id, uint64_t uid, EM_MAIL_DEL_TYPE type);
	// �ʼ��Ƿ����
	static bool is_mail_exsit(uint64_t mail_id);
	// �õ��ʼ�������Ϣ
	static bool get_mail_content(uint64_t mail_id, uint64_t uid, proto::client::ec_mail_content_reply& reply);
	// ����ʼ��Ƿ���Ա���ȡ����ֹ�������ȡ��
	static bool check_can_get(uint64_t mail_id, uint32_t now_time);
	// �õ�������Ϣ��������������ȡʱ��
	static void get_mail_addenda(uint64_t mail_id, uint32_t now_time, proto::server::eg_mail_get_addenda_request& addenda);
	// ���ʼ�
	static bool add_new_mail(const new_mail_define& mail_define);
	// ������ڱ���ȡ�ı�־λ
	static void clear_get_mask(uint64_t mail_id);

public:
	// ��������ȫ���ʼ�
	static bool load_global_mails(global_mail_ptr_map& global_mail_map);
	// ���һ���µ�ȫ���ʼ�
	static bool add_new_global_mail(global_mail_ptr p_mail);
};
#endif
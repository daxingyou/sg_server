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

	// �����ʼ�����
	void on_login_load();

	// �Զ������Ѷ��ʼ�
	void clear_opened_mail();
	// 

	// ɾ����ʱ�ʼ�
	void clear_time_out_mail();

	// ǿ��ɾ���ʼ���ɾ��������
	void force_del_mail(uint32_t num = 1);

	// ���
	void clear();

	// �Ƿ���δ���ʼ�
	bool is_have_new();

	// �Ƿ��Ѵ�
	bool is_have_new_mail();

	// �����ʼ�����ȡ����ɾ��
	void set_mail_get_delete(uint64_t mail_id, proto::client::ec_mail_state_change_notify& notify);

	// �����ʼ�
	void clear_mail(EM_MAIL_DEL_TYPE type = MAIL_DEL_USER);

public:
	// �õ��ʼ��б�
	void get_mail_list(proto::client::ec_mail_list_notify& notify);

	// �õ��ʼ�����
	void get_mail_content(uint64_t mail_id, proto::client::ec_mail_content_reply& reply);

	// ɾ���ʼ�
	void del_mail_by_id(uint64_t mail_id, proto::client::ec_mail_state_change_notify& notify, EM_MAIL_DEL_TYPE type = MAIL_DEL_USER);

	// �����ʼ�
	void clear_mail(proto::client::ec_mail_state_change_notify& notify, EM_MAIL_DEL_TYPE type = MAIL_DEL_USER);

	// ǿ��ɾ���ʼ���ɾ��������,֪ͨclient
	//void force_del_mail(proto::client::ec_mail_state_change_notify& notify, uint32_t num = 1);

	// ��ȡ�ʼ����������͸�game
	uint32_t mail_get_send_to_game(uint64_t mail_id, global_user_ptr p_user);

	// ��ȡ������game����
	void mail_get_game_back(global_user_ptr p_user, const proto::server::ge_mail_get_addenda_reply& reply);

	// һ����ȡ�����ʼ�
	void get_all_mails(global_user_ptr p_user);

	// ���ʼ�
	bool add_new_mail(const new_mail_define& mail_define, global_user_ptr p_user);

	//���ȫ�����ʼ�
	bool add_new_global_mail(const new_mail_define& mail_define, global_user_ptr p_user);

	// �Ƿ��Ѿ���ȫ���ʼ�
	bool is_have_global_mail(uint64_t id) { return m_global_mail_map.find(id) != m_global_mail_map.end(); }

	void set_have_global_mail(uint64_t id) { m_global_mail_map.insert(id);  }

private:
	mail_base_map m_mail_map;
	uint64_t m_role_uid = 0;
	uint32_t m_force_del_num = 0;

	std::set<uint64_t> m_global_mail_map;
};

#endif

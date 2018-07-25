#ifndef __FRIEND_DATA_MGR_H__
#define __FRIEND_DATA_MGR_H__

#include "macros.hpp"

#include "common.pb.h"
#include "client.pb.h"

#include "friend/friend_mgr.hpp"

class friend_data_mgr_t
{
public:
	// �����Ƿ����
	static bool is_friend_key_exsit(const std::string& key);
	// �����������ʱ��
	static bool update_apply_time(uint32_t uid, const std::string& key, uint32_t time);
	// ���������ϵʱ��
	static bool update_contact_time(uint32_t uid, const std::string& key, uint32_t time);
	// �����ö���Ϣ
	static bool update_top_time(uint32_t uid, const std::string& key, bool is_top, uint32_t time = 0);
	// ���һ��������������
	static bool clear_friend_info(uint32_t uid, const std::string& key);
	// ���һ���µ�����ϵ
	static void update_all_friend_info(uint64_t uid, friend_info_ptr p_friend);
	// ���º�������
	static bool update_friend_type(uint32_t uid, const std::string& key, proto::common::friend_type type);

public:
	// �õ��������ݣ���������
	static proto::common::friend_type get_offline_friend_type(uint64_t uid, uint64_t friend_uid);
	// ����������Ϣ����������ʱ��
	static void update_offline_friend_apply_time(uint64_t uid, uint64_t friend_uid, uint32_t time_now);
	// ����������Ϣ�������ϵʱ��
	static void update_offline_friend_contact_time(uint64_t uid, uint64_t friend_uid, uint32_t time_now);
	// ����������Ϣ���޸ĺ�������
	static void update_offline_friend_type(uint64_t uid, uint64_t friend_uid, proto::common::friend_type type);
	// ����������Ϣ�����һ������ʱ��
	static uint32_t get_offline_logout_time(uint64_t uid);

public:
	// �������к�����Ϣ����ʼ�������ϵ�б�ͺ��������б�
	static void load_all_friends(uint64_t uid, friend_info_map& friends, 
		friend_contact_list& contact_list, uint32_t& contact_size, 
		friend_apply_list& apply_list, uint32_t& apply_size,
		uint32_t& friend_size, uint32_t& black_size, uint32_t& top_size);

public:
	// �û���Ϣ�Ƿ����
	static bool is_user_key_exsit(uint64_t uid);
	// ���غ�����ϸ��Ϣ
	static void update_user_info(friend_info_ptr p_info);
	// ���غ��ѻ�����Ϣ
	static void load_user_base_info(uint64_t friend_uid, proto::common::user_info* p_user_info);
	// ͨ��id�õ�һ���û���Ϣ
	static bool search_user_info_by_id(uint64_t uid, proto::common::user_info* p_user_info);
	// ͨ�����ֵõ�һ���û���Ϣ
	static bool search_user_info_by_name(const std::string& name, proto::common::user_info* p_user_info);
};
#endif

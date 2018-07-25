#ifndef __RED_POINT_MANAGER_H__
#define __RED_POINT_MANAGER_H__

#include "protocpp/common.pb.h"
#include "protocpp/client.pb.h"
#include "red_point.hpp"

enum red_point_activity_type : uint32_t
{
	activity_day = 1		 //�ճ��
};

/*
 * ˵��: ���ط���ȫ������ ������DB
*/
typedef std::map<uint32_t, red_point_ptr> red_point_map;
typedef std::set<uint32_t> red_point_activity_join_set;
typedef boost::shared_ptr<red_point_map> red_point_map_ptr;
typedef boost::shared_ptr<red_point_activity_join_set> red_point_activity_join_map_ptr;
class red_point_manager_t
{
public:
	red_point_manager_t(){}
	~red_point_manager_t(){}
public:
	///����б����Ƿ���ڸú��
	static bool is_exist(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type);
	///������Ƿ���ں��
	static bool is_exist(uint64_t role_uid);

	//�����㵽����б���
	static void add_red_point(uint64_t role_uid, red_point_ptr p_red_point);
	
	//��ȡ���
	static red_point_ptr get_red_point(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type);

public:
	//�����״̬ ������
	static void check_red_point(uint64_t role_uid);
	static bool check_activity_open_box(uint64_t role_uid,bool is_online = false);
	static void check_activity_join(uint64_t role_uid);
	static bool check_page_point(uint64_t role_uid);
public:

	//���ͳһ֪ͨ�ӿ�
	static void red_point_notify(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type,uint32_t param = 0);
	static void red_point_notify(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type, std::set<uint32_t>& param_list);
	
	//����֪ͨ���ͻ���
	static bool send_notify_to_client(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type,std::string& dec);

	//����֪ͨ���ͻ���(ͳһ����)
	static bool send_notify_to_client(uint64_t role_uid, proto::client::gc_red_point_notify& notify);
public:
	//������б�(û��ȡ���ĺ�� �������Ϳͻ��� ��ʾ��� ���볡������)
	static void init_red_point(uint64_t role_uid);
	//��ȡ���˺���б�
	static red_point_map_ptr get_role_red_point_list(uint64_t role_uid);
public:
	//����������к��
	static void remove_role_all_red_point(uint64_t role_uid);
	//�������ָ�����ͺ��
	static void remove_role_type_red_point(uint64_t role_uid, proto::common::RED_POINT_TYPE red_point_type);
	
};
static std::map<uint64_t, red_point_map_ptr> m_all_role_red_point_list;
static std::map<uint64_t, red_point_activity_join_map_ptr> m_activity_join_list;	//ר�����ڼ�¼��������Щ� 

#endif // !__RED_POINT_MANAGER_H__

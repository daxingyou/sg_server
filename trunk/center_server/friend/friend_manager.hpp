#ifndef __FRIEND_MANAGER_H__
#define __FRIEND_MANAGER_H__

#include "macros.hpp"

class friend_manager_t
{
public:
	static bool init();
	static void reload();

public:
	static uint32_t get_friend_cnt1() { return m_friend_cnt1; }
	static uint32_t get_friend_cnt2() { return m_friend_cnt2; }
	static uint32_t get_friend_cnt3() { return m_friend_cnt3; }
	static uint32_t get_friend_cnt4() { return m_friend_cnt4; }
	static uint32_t get_friend_cnt5() { return m_friend_cnt5; }
	static uint32_t get_friend_cnt6() { return m_friend_cnt6; }
	static uint32_t get_friend_cnt7() { return m_friend_cnt7; }
	static uint32_t get_friend_cnt8() { return m_friend_cnt8; }
	static uint32_t get_friendmsg_strlen1() { return m_friendmsg_strlen1; }
	static uint32_t get_friendmsg_log1() { return m_friendmsg_log1; }
	static uint32_t get_friendmsg_log2() { return m_friendmsg_log2; }
	static uint32_t get_friendoutdel_limit() { return m_friendoutdel_limit; }

	static uint32_t get_max_save_time() { return m_max_save_time; }
	static uint32_t get_chat_max_content() { return m_chat_max_content; }
	static int32_t get_initial_friends_friendliness() { return m_initial_friends_friendliness; }
	static int32_t get_max_friendliness_add_one_day() { return m_max_friendliness_add_one_day; }
	static int32_t get_max_friendliness() { return m_max_friendliness; }
public:
	static uint32_t m_friend_cnt1;			// �����ϵ�б���������
	static uint32_t m_friend_cnt2;			// �����б���������
	static uint32_t m_friend_cnt3;			// �������б���������
	static uint32_t m_friend_cnt4;			// ���������б���������
	static uint32_t m_friend_cnt5;			// �ö�������������
	static uint32_t m_friend_cnt6;			// ϵͳ�Ƽ����ѵ���������
	static uint32_t m_friend_cnt7;			// ϵͳ�Ƽ�ˢ�¼��ʱ�䣨�룩
	static uint32_t m_friend_cnt8;			// ϵͳ�Ƽ����������а��ͬ�ȼ��κ������ٸ���
	static uint32_t m_friendmsg_strlen1;	// ˽���ַ��ֽڳ�������
	static uint32_t m_friendmsg_log1;		// ˽�ļ�¼������ұ�����������
	static uint32_t m_friendmsg_log2;		// ˽�ļ�¼����������������
	static uint32_t m_friendoutdel_limit;	// �Զ�ɾ������ʱ����������������

	static uint32_t m_max_save_time;		// ��������������Ϣ��������������
	static uint32_t m_chat_max_content;		// ��������������Ϣ��������������

	static int32_t m_initial_friends_friendliness;	//˫����ѵĳ�ʼ���ܶ�
	static int32_t m_max_friendliness_add_one_day;	//һ�������ӵ����ܶ�����
	static int32_t m_max_friendliness;		//���ܶ�����

	static int32_t m_fp_send_initial_limit;		//ÿ��ÿ����������n�������
	static int32_t m_fp_receive_initial_limit;	//��ÿ��������ȡn�������
	static int32_t m_friendliness_added_by_send_fp;	//������������ӵ����ܶ���ֵ

};

#endif

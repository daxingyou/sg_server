#ifndef __ROLE_BUFF_H__
#define __ROLE_BUFF_H__
#include "macros.hpp"
#include "protos_fwd.hpp"
#include <boost/asio/deadline_timer.hpp>

enum role_buff_type
{
	role_buff_type_addspeed = 1,		// ���ٶ�
	role_buff_type_addattr = 2,			// ������
	role_buff_type_add_percent_speed = 3,// �Ӱٷֱ��ٶ�
	role_buff_type_addexp = 4,			//��ֱȾ���
	role_buff_type_change_plugin = 5,	// ���߱���
	role_buff_type_task_plugin = 6,		// �������
};

class role_buff_t
{
public:
	role_buff_t(uint64_t owner);

	void set_tid(uint32_t val) { m_tid = val; }
	uint32_t get_tid() { return m_tid; }

	void del_timer();
public:
	void load_data(const proto::common::role_buff_single& buff_single);
	void peek_data(proto::common::role_buff_single* p_buff_single);
public:
	//BUFF��Ч����ʱ���������Ϊ�������������������¼���BUFF
	void do_effect(uint32_t remain_time, bool notify=true);
	//BUFFʧЧ
	void lose_effect(bool notify);
private:
	uint64_t m_owner = 0;
	uint32_t m_tid = 0;
	boost::shared_ptr<boost::asio::deadline_timer> m_timer;
};
typedef boost::shared_ptr<role_buff_t> role_buff_ptr;

#endif // !__BOUNTY_H__


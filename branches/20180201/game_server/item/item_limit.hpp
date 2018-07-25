#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/system/error_code.hpp>

/*
author: wys
time:2016.12.30
desc:center ��Ʒ������ ����ȫ��Ͷ����
*/

class item_limit_t : public boost::enable_shared_from_this<item_limit_t>
{
public:
	item_limit_t(uint32_t item_tid, uint32_t item_num, uint32_t last_give_time);
	~item_limit_t();

	uint32_t	get_count() { return m_count; }
	void		set_count(uint32_t count);
	bool		can_add(uint32_t item_num);
	void		set_last_give_time(uint32_t last_give_time) { m_last_give_time = last_give_time; }

private:
	uint32_t m_item_tid = 0;		// ��Ʒtid
	uint32_t m_count = 0;			// �ۼƷ�������
	uint32_t m_last_give_time = 0;	// �ϴη���ʱ��
};

typedef boost::shared_ptr<item_limit_t> item_limit_ptr;
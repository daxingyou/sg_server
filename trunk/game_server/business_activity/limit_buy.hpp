//7���޹�
#ifndef _LIMIT_BUY_HPP_
#define _LIMIT_BUY_HPP_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"
#include <set>
#include <string>

class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class limit_buy_t: public base_business_active_t
{
public:
	bool load_data(const proto::common::limit_buy_save_data &data);
	bool peek_data(proto::common::limit_buy_save_data &data);
	bool save_data();
public:
	limit_buy_t(uint64_t role_id);

	uint32_t get_limit_buy_info(proto::client::gc_get_limit_buy_info_reply &reply);

	uint32_t get_prize_status(uint32_t index);

	uint32_t buy_limit( uint32_t index, proto::common::role_change_data *rcd);

	uint32_t get_buy_num(uint32_t index);

	void init_newbie() { m_day = 1; save_data(); }

	void one_day();
public:
	//�Ƿ��ڻ��
	virtual bool is_active();

	//�Ƿ���ȡ�����н���
	virtual bool is_get_all_prize();

	//�Ƿ��ǽ���ʱ����
	virtual bool is_end_show_time();

	//��ȡ���ʼʱ��, �����0��ʾδ�μӹ���Ҳ����Ҫ��ʾ
	virtual uint32_t get_active_start_time(uint32_t start_time);

	//��ȡ�����ʱ��,����ģ������������չ
	virtual uint32_t get_active_end_time();

	//���û��ʼ
	virtual void set_active_start();

	//���û����
	virtual void set_active_stop();

	//��ȡ���
	virtual uint32_t get_red_num();
private:
	std::string m_key = "";

	uint64_t m_role_id = 0;

	uint32_t m_day = 1;						//��¼����

	uint32_t m_red = 0;

	std::map<uint32_t, uint32_t> m_prize_list;	//�����б�
};
#endif 
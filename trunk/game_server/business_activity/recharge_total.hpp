//��Ӫ�-���߶һ�ϵͳ

#ifndef _RECHARGE_TOTAL_
#define _RECHARGE_TOTAL_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"
#include <set>
#include <string>


class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class recharge_total_t : public base_business_active_t
{
public:
	bool save_data();
	bool peek_data(  proto::common::recharge_total_data &data );
	bool load_data( const proto::common::recharge_total_data &data);
	void peek_client_data(proto::common::recharge_total_client_data &data);
public:	
	recharge_total_t(uint64_t role_id);

	uint32_t on_login();
	
	uint32_t award(uint32_t id, proto::common::role_change_data* p_data);

	//�賿���մ���
	void one_day();
public:
	//�Ƿ��ڻ��
	virtual bool is_active();

	//�Ƿ���ȡ�����н���
	virtual bool is_get_all_prize();

	//�Ƿ��ǽ���ʱ����
	virtual bool is_end_show_time();

	//��ȡ���ʼʱ��, �����0��ʾδ�μӹ���Ҳ����Ҫ��ʾ
	virtual uint32_t get_active_start_time();

	//��ȡ�����ʱ��,����ģ������������չ
	virtual uint32_t get_active_end_time();

	//���û��ʼ
	virtual void set_active_start( uint32_t start_time );

	//���û����
	virtual void set_active_stop();

	//��ȡ���
	virtual uint32_t get_red_num();

private:
	std::string m_key;

	uint64_t m_role_uid;

	uint32_t m_start_time;

	std::vector<uint32_t> m_recharge_total_vec;		//�Ѿ���ȡ�Ľ���
};


#endif

//��Ӫ�-���߶һ�ϵͳ

#ifndef _EXCHANGE_ITEM_
#define _EXCHANGE_ITEM_

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"
#include <set>
#include <string>


class role_t;
typedef boost::shared_ptr<role_t> role_ptr;

class exchange_item_t : public base_business_active_t
{
public:
	bool save_data();
	bool peek_data(  proto::common::exchange_item_save_data &data );
	bool load_data( const proto::common::exchange_item_save_data &data);
public:	
	exchange_item_t(uint64_t role_id);

	uint32_t do_exchange_item( uint32_t id, proto::common::role_change_data *rcd );

	uint32_t get_exchange_info( proto::client::gc_get_exchange_info_reply &reply);

	uint32_t on_login();
	
	uint32_t get_cur_num(uint32_t id);
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

	std::map<uint32_t, uint32_t> m_exchange_map;		//�һ��Ĵ���
};


#endif

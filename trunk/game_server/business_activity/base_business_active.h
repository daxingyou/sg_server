#ifndef _BASE_BUSINESS_ACTIVE_T
#define _BASE_BUSINESS_ACTIVE_T

#include "protos_fwd.hpp"

class base_business_active_t
{
public:
	//�Ƿ��ڻ��
	virtual bool is_active() { return false; };

	//�Ƿ���ȡ�����н���
	virtual bool is_get_all_prize() { return true; }

	//�Ƿ��ǽ���ʱ����
	virtual bool is_end_show_time() { return true;  }

	//��ȡ���ʼʱ��, �����0��ʾ��Զ������
	virtual uint32_t get_active_start_time() { return 0; }

	//��ȡ�����ʱ��,����ģ������������չ0��ʾ��������
	virtual uint32_t get_active_end_time() { return 0; }

	//���û��ʼ
	virtual void set_active_start( uint32_t time ) {}
	
	//���û����
	virtual void set_active_stop() {}

	//��ȡ���
	virtual uint32_t get_red_num(){ return 0; }

};

enum BA_OPEN_TYPE
{
	TYPE_LEVEL = 1,
}; 

enum BA_DEL_TYPE
{
	TYPE_NONE = 0,	//�����Ƴ�
	TYPE_ZERO = 1,	//0���Ƴ�
};

#endif

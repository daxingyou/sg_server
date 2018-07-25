#pragma once

#include "macros.hpp"
#include "protos_fwd.hpp"
#include "base_business_active.h"

/**
 *  �����̵����� wys 2018.3.1
 */

class activity_shop : public base_business_active_t
{
public:
	activity_shop(uint64_t uid);
	~activity_shop();

	void load_data(const proto::common::personal_info & info);
	void peek_data(proto::common::personal_info *p_info);

	void		one_day();

	uint32_t	get_wusheng_score() { return m_wusheng_score; }
	uint32_t	get_wusheng_score_today() { return m_wusheng_score_today; }
	uint32_t	get_wusheng_lucky() { return m_wusheng_lucky; }
	uint32_t	get_wusheng_today_buy() { return m_wusheng_today_buy; }

	void		add_wusheng_score(uint32_t score);
	void		dec_wusheng_score(uint32_t score);
	void		add_wusheng_today_buy(uint32_t num);
// 	void		add_wusheng_lucky(uint32_t lucky);
// 	void		dec_wusheng_lucky(uint32_t lucky);
	void		set_lucky(uint32_t lucky);

	//active��صĽӿڱ�ʾ�Ƿ�ù��ܼ��show��صĽӿڱ�ʾ�ͻ����Ƿ���ʾ���ͻ�����ʾ�Ĺ��ܲ�һ���ڼ���״̬���رռ����˵Ĺ���Ҳ���ܻ��ڿͻ�����ʾ
	//�Ƿ��ڻ�У�Ӱ��������繺��ȣ�������Ӱ����ʾ
	virtual bool is_active();

	//�Ƿ���ȡ�����н���
	virtual bool is_get_all_prize();

	//�Ƿ��ǽ���ʱ���ˣ���ʾ�ͻ��˽��������ʾ��ʱ��
	virtual bool is_end_show_time();

	//��ȡ���ʼʱ��, �����0��ʾδ�μӹ���Ҳ����Ҫ��ʾ
	virtual uint32_t get_active_start_time();

	//��ȡ�����ʱ��,����ģ������������չ
	virtual uint32_t get_active_end_time();

	//���û��ʼ
	virtual void set_active_start(uint32_t start_time);

	//���û����
	virtual void set_active_stop();

	//��ȡ���
	virtual uint32_t get_red_num();

private:
	uint64_t	m_role_uid = 0;
	uint32_t	m_wusheng_score = 0;			//��ʥ�������
	uint32_t	m_wusheng_score_today = 0;		//��ʥ������ջ���
	uint32_t	m_wusheng_lucky = 0;			//��ʥ��������ֵ
	uint32_t	m_wusheng_today_buy = 0;		//��ʥ������չ������
};
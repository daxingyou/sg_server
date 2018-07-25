#pragma once

#include "macros.hpp"
#include "common.pb.h"
#include "trade_item.hpp"
#include <vector>
#include <unordered_map>
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>

typedef std::map<uint32_t,trade_item_ptr> trade_item_map;

class trade_t
{
public:
	trade_t(uint32_t trade_id,uint32_t scene_id);
	~trade_t() {}

public:
	void init();
	void close();

public:
	///��ȡ����ID
	uint32_t get_id() { return m_id; }

	///��ȡ����ID
	uint32_t get_scene_id() { return m_scene_id; }

	///��ȡó���¼���ʼʱ��
	uint32_t get_event_start_time() { return m_event_start_time; }

	///��ȡó���¼�����
	uint32_t get_event_param() { return m_event_param; }

	///��ȡó���¼�����
	uint32_t get_event_market() { return m_event_market; }

	///��ȡó���¼�ͳ��
	uint32_t get_event_count() { return m_event_count; }

	///��ȡó���¼�
	uint32_t get_event_type() { return m_event_type; }

public:
	///��ȡó����Ʒ
	trade_item_ptr			get_trade_item(uint32_t trade_item_tid);

	///��ȡó����Ʒ����
	trade_item_type_ptr		get_trade_item_type(uint32_t trade_item_type);

	///��ȡȫ��ó����Ʒ�б�
	const trade_item_map&	get_trade_item_list() { return m_trade_item_list; }

	///�������ó�׵���Ʒ
	bool					trade_buy_item(trade_item_ptr p_trade_item, uint32_t item_num);
	///���۳���ó�׵���Ʒ
	bool					trade_sell_item(trade_item_ptr p_trade_item, uint32_t item_num);
	///������ó���¼��Ƿ����
	bool					check_trade_event_is_end();

	///��ȡ���ٶ�
	uint32_t get_prosperity();
	
	///��ȡռ�ж�
	uint32_t get_hold();

	///��ȡ���ٶȵȼ�
	uint32_t get_prosperity_level();
public:
	///����ó�����鵽ʱ����
	bool	market_on_time();

	///����ó�����ÿɹ���������ʱ����
	bool	reset_buy_num_on_time();

	///����ó���¼����õ�ʱ����
	bool	event_reset_on_time(proto::common::trade_event_type event_type);

	///����ó���¼���ʱ����
	void	event_on_time();

	///ó���¼�ֹͣ
	void	event_stop();

	///��ʼó���¼���ʱ��
	void	start_event_timer();

	///ȡ��ó���¼���ʱ��
	void	cancel_event_timer();

private:
	uint32_t m_id = 0;						///����ID
	uint32_t m_scene_id = 0;				///sceneID
	uint32_t m_event_start_time = 0;		///ó���¼���ʼʱ��
	uint32_t m_event_param = 0;				///ó���¼��Ĳ���
	uint32_t m_event_market = 10000;		///ó���¼�������
	uint32_t m_event_count = 0;				///ó���¼�ͳ��
	uint32_t m_event_type = 0;				///ó���¼�

private:
	boost::shared_ptr<boost::asio::deadline_timer> m_timer_event;	///�¼�����ʱ��

	///trade_item_type_map				m_trade_item_type_list;			///������Ʒ�����б�
	trade_item_map					m_trade_item_list;				///������Ʒ�б�
	std::vector<trade_item_ptr>		m_event_item_list;				///ó���¼�������Ʒ
};

typedef boost::shared_ptr<trade_t>	trade_ptr;
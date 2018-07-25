#pragma once

#include "trade.hpp"
#include "protos_fwd.hpp"
#include <unordered_map>
#include <boost/system/error_code.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <tuple>


class trade_manager_t
{
public:
	trade_manager_t();
	~trade_manager_t();
public:
	typedef std::map<uint32_t, trade_ptr> trade_map;
	typedef std::tuple<uint32_t, uint32_t,uint32_t> sug_trade_info;
	typedef std::map<uint32_t, std::tuple<uint32_t, uint32_t, uint32_t> > trade_hot_map;
public:
	static void			close();
	static bool			init();
	static bool			init_all_trade();
	static trade_ptr	get_trade(uint32_t trade_id);

	///ó�����鵽ʱ����
	static void			market_on_time(const boost::system::error_code& ec);
	
	///ó�����ÿɹ���������ʱ����
	static void			buy_num_on_time(const boost::system::error_code& ec);
	
	///ó���¼���ʱ����
	static void			event_reset_on_time(const boost::system::error_code& ec);

	///��ȡó���б�
	static const trade_map&	get_all_trade() { return m_trade_map; }


	///�Ƽ�ó��
	static void			push_trade();

	///�����б�
	static const trade_hot_map& get_trade_hot_map() { return m_hot_map; }

public:
	///�򿪶�ʱ��
	static void			start_market_timer();
	static void			start_buy_num_timer();
	static void			start_event_timer();

	///�رն�ʱ��
	static void			cancel_market_timer();
	static void			cancel_buy_num_timer();
	static void			cancel_event_timer();

	///��ȡ��ʱ��ʱ��
	static uint32_t		get_market_reset_time() { return m_market_reset_time; }
	static uint32_t		get_buy_num_reset_time() { return m_buy_num_reset_time; }
	static uint32_t		get_event_reset_time() { return m_event_reset_time; }

public:
	///ó���¼���ʱ����
	static void			event_on_time(const boost::system::error_code& ec, uint32_t trade_id);

public:
	///��ȡ�Ƽ�ó����Ϣ
	static void			get_suggest_trade_info(uint32_t& trade_id,uint32_t& item_tid, uint32_t& sell_trade_id);

public:
	///������̽���֪ͨ
	static uint32_t		troop_trade_view_notify(const uint64_t role_uid,const uint32_t trade_id);

private:
	static trade_map m_trade_map;												///ó���б�
	static sug_trade_info m_sug_trade_info;										///�Ƽ�����(����ID ��ƷID ����ƷΪ��������������)
	static trade_hot_map  m_hot_map;											///���������б�
	static boost::shared_ptr<boost::asio::deadline_timer> m_market_timer;		///ó�����鶨ʱ��
	static boost::shared_ptr<boost::asio::deadline_timer> m_buy_num_timer;		///ó�׿ɹ�����Ʒ������ʱ��
	static boost::shared_ptr<boost::asio::deadline_timer> m_event_timer;		///ó���¼���ʱ��
	static uint32_t m_market_reset_time;										///ó����������ʱ��
	static uint32_t m_buy_num_reset_time;										///ó�׿ɹ�����Ʒ��������ʱ��
	static uint32_t m_event_reset_time;											///ó���¼�����ʱ��
};

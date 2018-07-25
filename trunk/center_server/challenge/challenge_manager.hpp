#ifndef __CENTER_CHALLENGE_MANAGER_H__
#define __CENTER_CHALLENGE_MANAGER_H__

#include "macros.hpp"

#include "server.pb.h"
#include "common.pb.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <map>
#include <vector>
#include <deque>
class global_user_t;
typedef boost::shared_ptr<global_user_t> global_user_ptr;

class challenge_t : public boost::enable_shared_from_this<challenge_t>
{
public:
	challenge_t(){}
public:
	void set_uid(uint64_t val) { m_uid = val; }
	uint64_t get_uid() { return m_uid; }
	void set_challenge_time(uint32_t val) { m_challenge_time = val; }
	uint32_t get_challenge_time() { return m_challenge_time; }
	void set_state(uint32_t val) { m_state = val; }
	uint32_t get_state() { return m_state; }
	void set_challenger_uid(uint64_t val) { m_challenger_uid = val; }
	uint64_t get_challenger_uid() { return m_challenger_uid; }
	void set_target_uid(uint64_t val) { m_target_uid = val; }
	uint64_t get_target_uid() { return m_target_uid; }
	void set_can_notice(bool val) { m_can_notice = val; }
	bool can_notice() { return m_can_notice; }

	void peek_data(proto::common::challenge_info* p_info);
public:
	void start_process_timer();
	void stop_process_timer();
	void start_notice_timer();
	void stop_notice_timer();
private:
	uint64_t m_uid = 0;
	uint32_t m_challenge_time = 0;
	uint64_t m_challenger_uid = 0;
	uint64_t m_target_uid = 0;
	uint32_t m_state = 0;
	boost::shared_ptr<boost::asio::deadline_timer> m_process_timer;
	bool m_can_notice = true;
};
typedef boost::shared_ptr<challenge_t> challenge_ptr;
typedef std::map<uint64_t, challenge_ptr> challenge_map;
typedef std::deque<challenge_ptr> challenge_record;

class challenge_manager_t
{
public:
	challenge_manager_t(){}
	~challenge_manager_t(){}
	// ����һ��ΨһID
	static uint64_t get_new_challenge_uid();

	static void load_data();

	static void peek_challenge_record(proto::client::ec_challenge_record_reply& reply);

	static void close_timer();
public:
	static uint32_t challenge_request(global_user_ptr p_role, uint64_t target_uid);
	static uint32_t challenge_process_request(global_user_ptr p_role, uint32_t process);
	static void challenge_timeout(uint64_t challenger_uid, const boost::system::error_code& ec);
	static void challenge_fight_result(uint64_t challenger_uid, uint32_t result);
public:
	static void one_day();
	//��ս�����ǰ�㲥���������
	static void notice_challenge(uint32_t id, bool is_in_time);
	//��Լս���������ʱ�㲥��װ���ã�
	static void notice_single_challenge(uint64_t role_uid);
	//װ��CD
	static void on_notice_timer(uint64_t challenger_uid, const boost::system::error_code& ec);
	//սǰһ����׼��
	static void ready_challenge(uint32_t id, bool is_in_time);
	//��ʽ����
	static void begin_challenge(const boost::system::error_code& ec);
	//սǰһ�����ڴ��Ͳ�֪ͨ�ͻ���
	static void check_ready(uint64_t role_uid);
	//��ȡָ����ҽ����Լս
	static challenge_ptr get_today_challenge(uint64_t role_uid);
	//��ȡָ����������Լս
	static challenge_ptr get_tomorrow_challenge(uint64_t role_uid);
	//��ȡָ����Һ����Լս
	static challenge_ptr get_day_after_tomorrow_challenge(uint64_t role_uid);

	static void challenge_done(challenge_ptr p_challenge);

	static void challenge_award(challenge_ptr p_challenge);
public:
	//���ȼ����
	static uint32_t get_delta_level();
	//װ��CD
	static uint32_t get_notice_cd();
	//����ս��ǰ�ĵ���ʱ
	static uint32_t get_fight_start_time();
	//Լս��Ӧ����ʱ
	static uint32_t get_process_time();
	//Լս��¼��Զ������
	static uint32_t get_pass_day();
	//Լս��¼�������
	static uint32_t get_record_num();
private:
	static challenge_record m_challenge_record;
	static challenge_map m_today_challenge_map;
	static challenge_map m_tomorrow_challenge_map;
	static challenge_map m_day_after_tomorrow_challenge_map;
	static boost::shared_ptr<boost::asio::deadline_timer> m_timer;
};

#endif
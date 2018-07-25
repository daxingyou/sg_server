#pragma once
#include "message.hpp"
#include "utility.hpp"

struct msg_recv_monitor_t
{
	uint16_t cmd = 0;
	uint16_t msg_len = 0;
	uint32_t recv_interval = 0;
	uint32_t recv_time = 0;
	uint64_t recv_time_ms = 0;
};

struct msg_send_monitor_t
{
	uint16_t cmd = 0;
	uint16_t msg_len = 0;
	uint32_t send_interval = 0;
	uint32_t send_time = 0;
	uint64_t send_time_ms = 0;
};

class msg_count_t
{
public:
	static bool pass_cmd(uint16_t cmd);
	
	// 统计消息发送
	static void push_msg_send_info(const network::msg_buf_ptr& msg_buf, bool b_write);
	// 统计消息接收
	static void push_msg_recv_info(const network::msg_buf_ptr& msg_buf, uint16_t cmd, bool b_write);
	// 写消息发送的日志
	static void write_msg_send_count_data_file(bool b_write);
	// 写消息接收的日志
	static void write_msg_recv_count_data_file(bool b_write);
private:
	typedef std::vector<msg_recv_monitor_t> msg_recv_count_vec;
	typedef std::map<uint16_t, msg_recv_count_vec> msg_recv_count_map;
	static msg_recv_count_map m_msg_recv_count_data;

	typedef std::vector<msg_send_monitor_t> msg_send_count_vec;
	typedef std::map<uint16_t, msg_send_count_vec> msg_send_count_map;
	static msg_send_count_map m_msg_send_count_data;
};
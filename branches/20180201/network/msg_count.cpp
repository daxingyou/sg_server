#include "msg_count.hpp"
#include "op_cmd.hpp"
#include "cmd_desc.hpp"

#define MSG_COUNT_OPEN	 0
#define MSG_COUNT_MAX_SIZE 100

msg_count_t::msg_recv_count_map msg_count_t::m_msg_recv_count_data;
msg_count_t::msg_send_count_map msg_count_t::m_msg_send_count_data;


USING_NS_NETWORK;

//  屏蔽pingpong消息 还要屏蔽的加这
bool msg_count_t::pass_cmd(uint16_t cmd)
{
	switch (cmd)
	{
	case op_cmd::gc_role_outo_vision_notify:
	case op_cmd::gc_role_into_vision_notify:
    case op_cmd::gc_batch_move_notify:
	case op_cmd::gc_move_notify:
    case op_cmd::gc_field_boss_pos_info_notify:
    case op_cmd::gc_system_hint_notify:
    case op_cmd::gc_change_sys_state_notify:
    case op_cmd::gc_sys_notice_notify:
    case op_cmd::gc_scene_object_info_notify:
    case op_cmd::gc_update_long_info_notify:
    case op_cmd::gc_king_war_scene_start_notify:
    case op_cmd::gc_artifact_left_time_notify:
    case op_cmd::gc_object_hp_change_notify:
    case op_cmd::gc_dungeon_schedule_data_notify:
    case op_cmd::gc_notify_dungeon_result_notify:
    case op_cmd::gc_object_scene_jump_notify:
        return true;
    case op_cmd::ac_pingpong_reply:
    case op_cmd::ac_connect_reply:
    case op_cmd::nc_server_info_reply:
    case op_cmd::ec_king_war_notice_notify:
    case op_cmd::lc_login_reply:
        return true;
    case op_cmd::ga_role_move_notify:
	case op_cmd::ga_role_into_vision_notify:
	case op_cmd::ga_role_out_vision_notify:
        return true;
	case op_cmd::ca_pingpong_request:
    case op_cmd::ca_connect_request:
    case op_cmd::cg_move_request:
	case op_cmd::cn_server_info_request:
	case op_cmd::cl_login_request:
		return true;
	default:
		break;
	}
	return false;
}

void msg_count_t::push_msg_send_info(const network::msg_buf_ptr& msg_buf, bool b_write)
{
	if (!b_write)
		return;

	msg_head_t head;
	char* dst = (char*)&head;
	char* p = msg_buf->begin();
	std::char_traits<char>::copy(dst, p, sizeof(msg_head_t));

	if (pass_cmd(head.cmd))
		return;

	msg_send_monitor_t msg_count_info;
	msg_count_info.cmd = head.cmd;
	msg_count_info.msg_len = head.len - sizeof(uint64_t);
	msg_count_info.send_interval = 0;
	msg_count_info.send_time_ms = common::time_util_t::now_msecs();
	msg_count_info.send_time = time(NULL);

	bool b_is_need_clear = false;

	msg_send_count_map::iterator iter = m_msg_send_count_data.find(msg_count_info.cmd);
	if (iter != m_msg_send_count_data.end())
	{
		msg_send_monitor_t &last_msg_info = iter->second[iter->second.size() - 1];
		msg_count_info.send_interval = msg_count_info.send_time_ms - last_msg_info.send_time_ms;

		iter->second.push_back(msg_count_info);
		if (iter->second.size() >= MSG_COUNT_MAX_SIZE)
			b_is_need_clear = true;
	}
	else
	{
		msg_send_count_vec msg_vec;
		msg_vec.push_back(msg_count_info);
		m_msg_send_count_data.insert(std::make_pair(msg_count_info.cmd, msg_vec));
	}
	if (b_is_need_clear)
	{
		write_msg_send_count_data_file(true);
		for (auto pair : m_msg_send_count_data)
		{
			pair.second.clear();
		}
		m_msg_send_count_data.clear();
	}
}

void msg_count_t::write_msg_send_count_data_file(bool b_write)
{
	if (!b_write)
		return;

	std::ofstream log_msg_count;
	std::ofstream log_msg_detail;
	std::string file_msg_count = "./logs/msg_send_count";
	std::string file_msg_detail = "./logs/msg_send_detail";

	time_t t = time(NULL);
	struct tm *ptm = localtime(&t);
	char timestamp[128];
	strftime(timestamp, 128, "%Y%m%d-%H%M%S.csv", ptm);

	file_msg_count.append(timestamp);
	file_msg_detail.append(timestamp);

	log_msg_count.open(file_msg_count, std::ofstream::app);
	log_msg_detail.open(file_msg_detail, std::ofstream::app);
	do
	{
		if (!log_msg_count.is_open())
		{
			printf("\033[31mcreate file[%s] failed!\033[0m\n", file_msg_count.c_str());
			break;
		}
		if (!log_msg_detail.is_open())
		{
			printf("\033[31mcreate file[%s] failed!\033[0m\n", file_msg_detail.c_str());
			break;
		}
	} while (0);

	uint64_t msg_len_avg = 0;
	uint64_t msg_interval_avg = 0;

	char str_tmp[1024] = { 0 };

	log_msg_count << "cmd,desc,count,len_avg,send_interval_avg" << "\n";
	log_msg_detail << "cmd,desc,msg_len,msg_send_time,msg_send_interval" << "\n";

	time_t msg_send_time = 0;
	for (auto map_pair : m_msg_send_count_data)
	{
		msg_len_avg = 0;
		msg_interval_avg = 0;

		for (auto vec_pair : map_pair.second)
		{
			msg_len_avg += vec_pair.msg_len;
			msg_interval_avg += vec_pair.send_interval;

			str_tmp[0] = '\0';

			msg_send_time = vec_pair.send_time;
			ptm = localtime(&msg_send_time);
			strftime(timestamp, 128, "%Y%m%d-%H:%M:%S", ptm);
			snprintf(str_tmp, 1024, "%d,%s,%d,%s:%d,%d", vec_pair.cmd, CMD_DESC(vec_pair.cmd), vec_pair.msg_len, timestamp, (uint32_t)(vec_pair.send_time_ms - vec_pair.send_time * 1000), vec_pair.send_interval);
			log_msg_detail << str_tmp << "\n";
		}

		if (map_pair.second.size() == 0)
		{
			continue;
		}

		msg_len_avg = msg_len_avg / map_pair.second.size();
		msg_interval_avg = msg_interval_avg / map_pair.second.size();

		str_tmp[0] = '\0';
		snprintf(str_tmp, 1024, "%d,%s,%d,%lu,%lu", map_pair.first, CMD_DESC(map_pair.first), (int)map_pair.second.size(), msg_len_avg, msg_interval_avg);
		log_msg_count << str_tmp << "\n";
	}
	log_msg_count.close();
	log_msg_detail.close();
}

void msg_count_t::push_msg_recv_info(const network::msg_buf_ptr& msg_buf, uint16_t cmd, bool b_write)
{
	if (!b_write)
		return;
	
	if (pass_cmd(cmd))
		return;

	msg_recv_monitor_t msg_count_info;
	msg_count_info.cmd = cmd;
	msg_count_info.msg_len = msg_buf->size();
	msg_count_info.recv_interval = 0;
	msg_count_info.recv_time_ms = common::time_util_t::now_msecs();
	msg_count_info.recv_time = time(NULL);

	bool b_is_need_clear = false;
	int  vec_size = 0;

	msg_recv_count_map::iterator iter = m_msg_recv_count_data.find(cmd);
	if (iter != m_msg_recv_count_data.end())
	{
		msg_recv_monitor_t &last_msg_info = iter->second[iter->second.size() - 1];
		msg_count_info.recv_interval = msg_count_info.recv_time_ms - last_msg_info.recv_time_ms;

		iter->second.push_back(msg_count_info);
		vec_size = iter->second.size();
		if (vec_size >= MSG_COUNT_MAX_SIZE)
			b_is_need_clear = true;
	}
	else
	{
		msg_recv_count_vec msg_vec;
		msg_vec.push_back(msg_count_info);
		m_msg_recv_count_data.insert(std::make_pair(cmd, msg_vec));
	}
	if (b_is_need_clear)
	{
		write_msg_recv_count_data_file(true);
		for (auto pair : m_msg_recv_count_data)
		{
			pair.second.clear();
		}
		m_msg_recv_count_data.clear();
	}
}

void msg_count_t::write_msg_recv_count_data_file(bool b_write)
{
	if (!b_write)
		return;

	std::ofstream log_msg_count;
	std::ofstream log_msg_detail;
	std::string file_msg_count = "./logs/msg_recv_count";
	std::string file_msg_detail = "./logs/msg_recv_detail";

	time_t t = time(NULL);
	struct tm *ptm = localtime(&t);
	char timestamp[128];
	strftime(timestamp, 128, "%Y%m%d-%H%M%S.csv", ptm);

	file_msg_count.append(timestamp);
	file_msg_detail.append(timestamp);

	log_msg_count.open(file_msg_count, std::ofstream::app);
	log_msg_detail.open(file_msg_detail, std::ofstream::app);
	do
	{
		if (!log_msg_count.is_open())
		{
			printf("\033[31mcreate file[%s] failed!\033[0m\n", file_msg_count.c_str());
			break;
		}
		if (!log_msg_detail.is_open())
		{
			printf("\033[31mcreate file[%s] failed!\033[0m\n", file_msg_detail.c_str());
			break;
		}
	} while (0);

	uint64_t msg_len_avg = 0;
	uint64_t msg_interval_avg = 0;

	char str_tmp[1024] = { 0 };

	log_msg_count << "cmd,desc,count,len_avg,recv_interval_avg" << "\n";
	log_msg_detail << "cmd,desc,msg_len,msg_recv_time,msg_recv_interval" << "\n";

	time_t msg_recv_time = 0;
	for (auto map_pair : m_msg_recv_count_data)
	{
		msg_len_avg = 0;
		msg_interval_avg = 0;

		for (auto vec_pair : map_pair.second)
		{
			msg_len_avg += vec_pair.msg_len;
			msg_interval_avg += vec_pair.recv_interval;

			str_tmp[0] = '\0';
			msg_recv_time = vec_pair.recv_time;
			ptm = localtime(&msg_recv_time);
			strftime(timestamp, 128, "%Y%m%d-%H:%M:%S", ptm);
			snprintf(str_tmp, 1024, "%d,%s,%d,%s:%d,%d", vec_pair.cmd, CMD_DESC(vec_pair.cmd), vec_pair.msg_len, timestamp, (uint32_t)(vec_pair.recv_time_ms - vec_pair.recv_time * 1000), vec_pair.recv_interval);
			log_msg_detail << str_tmp << "\n";
		}

		if (map_pair.second.size() == 0)
		{
			continue;
		}

		msg_len_avg = msg_len_avg / map_pair.second.size();
		msg_interval_avg = msg_interval_avg / map_pair.second.size();

		str_tmp[0] = '\0';
		snprintf(str_tmp, 1024, "%d,%s,%d,%lu,%lu", map_pair.first, CMD_DESC(map_pair.first), (int)map_pair.second.size(), msg_len_avg, msg_interval_avg);
		log_msg_count << str_tmp << "\n";
	}
	log_msg_count.close();
	log_msg_detail.close();
}

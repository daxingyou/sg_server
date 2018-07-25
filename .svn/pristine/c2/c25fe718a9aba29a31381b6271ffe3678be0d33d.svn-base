#include "combat_log.hpp"
#include <iostream>
#include <fstream>
#include "common/CTFile.h"

combat_log_t::combat_log_t()
{
}

combat_log_t::~combat_log_t()
{
}

//void combat_log_t::write_combat_log(const process_map& action, const proto::common::fight_data& data, uint64_t fight_uid)
//{
//
//	std::ofstream file_ofs;
//
//	std::string file_path = "./combat_logs/";
//	std::string file_name = string_util_t::uint64_to_string(fight_uid);
//	file_path += file_name;
//
//	file_ofs.open(file_path.c_str(), std::ofstream::app);
//	if (!file_ofs.is_open())
//	{
//		printf("\033[31mcreate combat_log[%s] failed!\033[0m\n", file_path.c_str());
//		return;
//	}
//
//	std::string combat_log = "";
//
//	file_ofs << "fight_uid:" << fight_uid << ";";
//
//	data.SerializeToString(&combat_log);
//	file_ofs << combat_log.c_str() << ";";
//
//	for (auto pair : action)
//	{
//		combat_log.clear();
//		pair.second.SerializeToString(&combat_log);
//		file_ofs << combat_log.c_str() << ";";
//	}
//	file_ofs.flush();
//
//
//	if (file_ofs.is_open())
//	{
//		file_ofs.close();
//	}
//}

void combat_log_t::write_combat_log(const process_map& action, const proto::common::fight_data& data, uint64_t fight_uid)
{
	common::CTWriteFile file_ofs;

	std::string file_path = "../../nohup_run/combat_logs/";
	std::string file_name = string_util_t::uint64_to_string(fight_uid);
	file_path += file_name.c_str();

	file_ofs.Open(file_path.c_str());
	if (!file_ofs.HasOpened())
	{
		printf("\033[31mcreate combat_log[%s] failed!\033[0m\n", file_path.c_str());
		return;
	}

	std::string combat_log = "";

// 	combat_log.append("fight_uid:");
// 	combat_log.append(string_util_t::uint64_to_string(fight_uid));
// 	combat_log.append(";");
// 	file_ofs.Write(combat_log.c_str(), combat_log.length());
// 	combat_log.clear();
	
	const int combat_data_len = 10240;
	char combat_data[combat_data_len] = { 0 };
	proto::common::fight_video_data video_data;
	// 拷贝战斗初始数据
	video_data.mutable_data()->CopyFrom(data);
	// 拷贝战斗行为数据
	process_map::const_iterator iter = action.begin();
	process_map::const_iterator iter_end = action.end();
	proto::common::combat_action_data *p_action_data = NULL;
	for (; iter != iter_end; ++iter)
	{
		p_action_data = video_data.mutable_action_data()->add_action_data();
		p_action_data->CopyFrom(iter->second);
	}

	// 太长了
	if (video_data.ByteSize() >= combat_data_len)
	{
		printf("\033[31mcombat_log fight_data_size[%d] >= combat_data_len[%d]!\033[0m\n", video_data.ByteSize(), combat_data_len);
		return;
	}

	video_data.SerializeToArray(combat_data, video_data.ByteSize());
	combat_log.assign(combat_data, video_data.ByteSize());

	proto::common::fight_video_data test;
	test.ParseFromArray(combat_log.c_str(), combat_log.length());

	file_ofs.Write(combat_log.c_str(), combat_log.length());
	combat_log.clear();

	file_ofs.CloseFile();
}


void combat_log_t::read_combat_log(uint64_t fight_uid)
{
	std::string file_path = "../../nohup_run/combat_logs/";
	std::string file_name = string_util_t::uint64_to_string(fight_uid);
	file_path += file_name.c_str();

	common::CTReadFile file_ifs;
	file_ifs.Open(file_path.c_str());
	if (!file_ifs.HasOpened())
	{
		printf("\033[31mload combat_log[%s] failed!\033[0m\n", file_path.c_str());
		return;
	}

	std::string combat_log = "";

	file_ifs.ReadAll();
	combat_log.assign(file_ifs.GetBuf(), file_ifs.ReadSize());
	//combat_log = file_ifs.GetBuf();
	
	proto::common::fight_video_data data;
	int n = data.ParseFromArray(combat_log.c_str(), combat_log.length());

	file_ifs.CloseFile();
}

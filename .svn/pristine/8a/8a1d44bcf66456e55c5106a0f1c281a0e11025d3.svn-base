#include "combat_log.hpp"
#include <iostream>
#include <fstream>
#include "common/CTFile.h"

//战斗日志最大长度
static const int COMBAT_DATA_MAX_LEN = 10240;

combat_log_t::combat_log_t()
{
}

combat_log_t::~combat_log_t()
{
}

void combat_log_t::write_combat_log(const process_map& action, const proto::common::fight_data& data, uint64_t fight_uid)
{
    proto::common::fight_video_data video_data;
    video_data.mutable_data()->CopyFrom(data);
    process_map::const_iterator iter = action.begin();
    process_map::const_iterator iter_end = action.end();
    proto::common::combat_action_data *p_action_data = NULL;
    for (; iter != iter_end; ++iter)
    {
        p_action_data = video_data.mutable_action_data()->add_action_data();
        p_action_data->CopyFrom(iter->second);
    }
    if (video_data.ByteSize() >= COMBAT_DATA_MAX_LEN)
    {
        log_error("combat_log[%lu] fight_data_size[%d] >= COMBAT_DATA_MAX_LEN[%d]", fight_uid, video_data.ByteSize(), COMBAT_DATA_MAX_LEN);
        return;
    }

	std::string file_path = env::xml->combat_path + string_util_t::uint64_to_string(fight_uid) + ".fight";
    common::CTWriteFile file_ofs;
    file_ofs.Open(file_path.c_str());
	if (!file_ofs.HasOpened())
	{
		log_error("create combat_log[%s] failed!", file_path.c_str());
        return;
	}
	std::string combat_log = "";
    char combat_data[COMBAT_DATA_MAX_LEN] = { 0 };
    video_data.SerializeToArray(combat_data, video_data.ByteSize());
	combat_log.assign(combat_data, video_data.ByteSize());
	file_ofs.Write(combat_log.c_str(), combat_log.length());
	combat_log.clear();

	file_ofs.CloseFile();
}

void combat_log_t::read_combat_log(uint64_t fight_uid)
{
    std::string file_path = env::xml->combat_path + string_util_t::uint64_to_string(fight_uid) + ".fight";

	common::CTReadFile file_ifs;
	file_ifs.Open(file_path.c_str());
	if (!file_ifs.HasOpened())
	{
        log_error("create combat_log[%s] failed!", file_path.c_str());
        return;
	}

	std::string combat_log = "";
	file_ifs.ReadAll();
	combat_log.assign(file_ifs.GetBuf(), file_ifs.ReadSize());
	proto::common::fight_video_data data;
	data.ParseFromArray(combat_log.c_str(), combat_log.length());

	file_ifs.CloseFile();
}

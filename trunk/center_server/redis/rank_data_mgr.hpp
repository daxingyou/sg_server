#ifndef __RANK_DATA_MGR_H__
#define __RANK_DATA_MGR_H__

#include "common/macros.hpp"
#include "common.pb.h"

class rank_data_mgr_t
{
public:
	// 数据是否存在
	static bool is_rank_exsit(const std::string& rank, uint64_t uid);
	// 删除排行榜数据
	static void del_rank_sort(const std::string& rank, uint64_t uid);


};

#endif//__RANK_DATA_MGR_H__
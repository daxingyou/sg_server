#ifndef __RANK_DATA_MGR_H__
#define __RANK_DATA_MGR_H__

#include "common/macros.hpp"
#include "common.pb.h"

class rank_data_mgr_t
{
public:
	rank_data_mgr_t();
	~rank_data_mgr_t();
	
	static void load_rank_data(uint32_t rank_id, proto::common::rank_data& data);
	static void save_rank_data(uint32_t rank_id, const proto::common::rank_data& data);
};

#endif//__RANK_DATA_MGR_H__
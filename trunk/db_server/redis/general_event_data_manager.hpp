#include "common/macros.hpp"
#include "protos_fwd.hpp"

/*
分开存，一起读取设计
*/
class general_event_data_manager_t
{
public:
	static bool load_general_event_data(uint64_t uid, proto::common::general_event_save_data * p_list);

	static bool save_general_event_data(uint64_t uid, const proto::common::general_event_save_data  &p_list);
};

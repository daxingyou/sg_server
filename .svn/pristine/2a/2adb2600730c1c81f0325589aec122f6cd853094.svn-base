
#ifndef __DATA_THRESHOLD_CHECK_HPP__
#define __DATA_THRESHOLD_CHECK_HPP__

#include "threshold_check.hpp"
#include "macros.hpp"
#include "gm.pb.h"

#define GAME_CHECK_SPECIAL_DATA(_uid, _type, _value, _notify) \
{ \
	if (threshold_check::check_special_value(_type, _value)) \
	{ \
		proto::gm::gm_data_threshold_notify noti_msg; \
		noti_msg.set_uid(_uid); \
		noti_msg.set_type(_type); \
		noti_msg.set_value(_value); \
		log_error("role[%lu] the data[%d] is greater than the threshold[%d]", _uid, _type, _value); \
		if (_notify) \
		{ \
			env::server->send_msg_to_center(op_cmd::gm_data_threshold_notify, _uid, noti_msg); \
		} \
	} \
}

#endif

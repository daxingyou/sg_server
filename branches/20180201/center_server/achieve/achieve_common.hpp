#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include "protocpp/protos_fwd.hpp"


/*
*	成就模块公共基类
*/

class achieve_common_t
{
public:
	achieve_common_t() {}
	~achieve_common_t() {}

public:
	/*
	*	public API
	*/

	///成就进度通知统一API
	static void			notify_progress_state(uint64_t uid, proto::common::Achieve_Event_Type type, uint32_t param1 = 1, uint32_t param2 = 1);

};

typedef boost::shared_ptr<achieve_common_t>	achieve_common_ptr;


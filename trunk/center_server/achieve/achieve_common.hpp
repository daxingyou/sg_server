#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include "protocpp/protos_fwd.hpp"


/*
*	�ɾ�ģ�鹫������
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

	///�ɾͽ���֪ͨͳһAPI
	static void			notify_progress_state(uint64_t uid, proto::common::Achieve_Event_Type type, uint32_t param1 = 1, uint32_t param2 = 1);

};

typedef boost::shared_ptr<achieve_common_t>	achieve_common_ptr;


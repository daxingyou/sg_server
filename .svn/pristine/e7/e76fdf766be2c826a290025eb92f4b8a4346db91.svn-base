#ifndef __ACHIEVE_METHOD_H__
#define __ACHIEVE_METHOD_H__

#include <boost/smart_ptr/shared_ptr.hpp>
#include "common.pb.h"
#include "tblh/Achievelist.tbls.h"
#include "tblh/Achievelevel.tbls.h"
#include "hero/hero_manager.hpp"


class achieve_t;
class achieve_data_manager;
class achieve_method_t
{
public:
	///主角等级
	static void achieve_main_role_level(const uint64_t uid, const proto::common::Achieve_Event_Type type,uint32_t param1 = 0, uint32_t param2 = 0);
	
	///主角养成类
	static void achieve_hero_cultivate(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id,uint32_t num = 1);

	///加入一个家族
	static void achieve_add_family(const uint64_t uid, const proto::common::Achieve_Event_Type type,uint32_t param1, uint32_t param2);

	///加入一个国家
	static void achieve_add_country(const uint64_t uid, const proto::common::Achieve_Event_Type type,uint32_t param1,uint32_t param);

	///判词
	static void achieve_exattr(const uint64_t uid, const proto::common::Achieve_Event_Type type, std::vector<uint32_t>& vec);


public:

	static void achieve_equip_type_same(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t suit_id, uint32_t param);
	static void achieve_equip_exattr(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t star_level,uint32_t param);

	///n * n 模式统一接口
	static void achieve_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id,uint32_t num = 1);

	///n 累加模式统一接口
	static void achieve_total_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type,uint32_t param = 0, uint32_t num = 1);

	///替换模式
	static void achieve_replace_model_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id = 0, uint32_t new_value = 0);

	///直接替换模式(不论替换值与旧值的大小)
	static void achieve_direct_replace_model_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id = 0, uint32_t new_value = 0);
};

typedef boost::shared_ptr<achieve_method_t>	achieve_method_ptr;


#endif	// __ACHIEVE_METHOD_H__

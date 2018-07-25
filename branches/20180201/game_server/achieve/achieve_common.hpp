#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include "protocpp/protos_fwd.hpp"
#include "achieve_common_enum.hpp"
#include "common/config_mgr.h"
#include "common/utility.hpp"
#include "common/MyTuple.h"



/*
 *	�ɾ�ģ�鹫������
 */

class achieve_common_t
{
public:
	achieve_common_t();
	~achieve_common_t();

public:
	/*
	 *	public API
	 */ 

	///��ȡID(�����¼�����)
	static bool			get_config_id(const uint64_t role_uid,achieve_common_type type,uint32_t achieve_event_type,uint32_t& id, uint32_t event_id = 0);

	///��ȡΨһID
	static uint64_t		gen_uid(achieve_common_type type);

	///�ɾͽ���֪ͨͳһAPI
	static void			notify_progress_state(uint64_t uid, proto::common::Achieve_Event_Type type, uint32_t param1 = 1, uint32_t param2 = 1);

	///��ȡ�������
	static uint32_t		get_target_count(achieve_common_type type,uint32_t id);

	///��ȡdrop_id
	static uint32_t		get_drop_id(achieve_common_type type, uint32_t id);

	static bool			add_element(achieve_common_type type,const uint64_t role_uid ,const uint32_t id);

	static bool			get_hero_achieve_total(const uint64_t role_uid, const proto::common::Achieve_Event_Type type, const uint32_t event_id, uint32_t& total);
public:

	///���ǵȼ�
	static void			achieve_main_role_level(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t param1 = 0, uint32_t param2 = 0);

	///����������
	static void			achieve_hero_cultivate(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t num = 1);

	///n * n ģʽͳһ�ӿ�
	static void			achieve_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t num = 1);

	///n �ۼ�ģʽͳһ�ӿ�
	static void			achieve_total_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t param = 0, uint32_t num = 1);

	///�滻ģʽ
	static void			achieve_replace_model(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id = 0, uint32_t new_value = 0);


	///ֱ���滻ģʽ
	static void			achieve_direct_replace_model_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id = 0, uint32_t new_value = 0);


};

typedef boost::shared_ptr<achieve_common_t>	achieve_common_ptr;



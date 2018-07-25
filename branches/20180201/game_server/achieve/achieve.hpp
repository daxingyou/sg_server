#ifndef __ACHIEVE_H__
#define __ACHIEVE_H__

#include <boost/smart_ptr/shared_ptr.hpp>
#include "tblh/Achievelist.tbls.h"
#include "tblh/Achievelevel.tbls.h"
#include "tblh/Achieverecord.tbls.h"
#include "protocpp/common.pb.h"
#include "achieve_common_enum.hpp"

/*
* =====================================================================================
*
*    Filename:  ACHIEVE.hpp
*
*    Description:  成就逻辑
*
* =====================================================================================
*/

class role_t;
class achieve_manager_t;
class achieve_method_t;
class achieve_t
{
public:
	achieve_t() {}
	achieve_t(uint64_t uid, uint32_t achieve_id,uint64_t role_uid);

	
	void load_data(uint64_t role_uid,const proto::common::achieve_data_single& achieve_data_single);
	void peek_data(proto::common::achieve_data_single* p_achieve_data_single,bool is_client = true);
	void save_self(uint64_t role_uid, bool is_new = false);

	std::string get_key() const { return m_key; }

	uint64_t get_uid() const { return m_uid; }
	void set_uid(uint64_t val) { m_uid = val; }

	uint32_t get_tid() { return m_tid; }
	void set_tid(uint32_t val) { m_tid = val; }

	uint32_t get_level() { return m_level; }
	bool add_level();

	uint32_t get_count() { return m_count; }
	void set_count(const uint32_t val) { m_count = val; }

	std::string get_info_list() { return m_finish_info_list; }
	void set_info_list(const std::string& val) { m_finish_info_list = val; }


	void get_achieve_finish_info(proto::common::achieve_finish_info* p_achieve_finish_info);
	void split_achieve_finish_info(const std::string &str,
								std::vector<proto::common::achieve_finish_info_single>& list);
	
	Achievelist* get_config(uint32_t level);
private:
	std::string m_key = "";
	uint64_t m_uid = 0;
	/*ID、等级、进度*/
	uint32_t		m_tid = 0;
	uint32_t		m_level = 0;
	uint32_t		m_count = 0;

	/*成就完成信息列表格式为：level:time|level:time|level:time.....*/
	std::string		m_finish_info_list = "";

	//role_id
	uint64_t m_role_uid = 0;
};
typedef boost::shared_ptr<achieve_t>	achieve_ptr;

#endif // !__ACHIEVE_H__


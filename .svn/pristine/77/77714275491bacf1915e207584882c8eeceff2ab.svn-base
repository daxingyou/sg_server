#ifndef __BOUNTY_MANAGER_H__
#define __BOUNTY_MANAGER_H__

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "common.pb.h"
#include "client.pb.h"
#include "macros.hpp"
#include "bounty.hpp"

enum star_box_state
{
	star_box_state_none = 0,	//未达到领取条件
	star_box_state_finish = 1,	//达到领取条件未领取
	star_box_state_over	= 2,	//已领取
};

class bounty_manager_t : public boost::enable_shared_from_this<bounty_manager_t>
{
public:
	bounty_manager_t(uint64_t role_uid);
    ~bounty_manager_t() {}

	void init();
public:
	void load_data(const proto::common::country_bounty_data& country_bounty_data);
	void peek_data(proto::common::country_bounty_data* p_country_bounty_data);
	void save_self();
	//读取全局配置
	static void load_comprehensive();
	//生成国家悬赏任务
	void make_bounty(uint32_t level);
	//每日更新
	void one_day();
public:
	//接取国家悬赏任务（唯一入口）
	bool accept_bounty(uint32_t bounty_id);
	//刷新国家悬赏任务
	bool refresh_bounty();
	//领取宝箱
	bool bounty_box(uint32_t box_id, proto::common::role_change_data* p_data);
	//完成任务（任务类中的finish_task会调用本函数）
	void finish_task(uint32_t circle_id);
	//放弃任务（任务类中的drop_task会调用本函数）
	void drop_task(uint32_t circle_id);
	//计算当前拥有的任务星级（添加武将时调用）
	void calc_star(uint32_t hero_tid);
	//快速完成悬赏任务
	void quick_finish_task(uint32_t circle_id, proto::common::role_change_data *rcd);
private:
	uint32_t get_star_index(const std::vector<uint32_t>& prob_vec);
	bounty_ptr get_bounty(uint32_t bounty_id);
	bool is_bounty_doing();
	//计算指定任务的星级
	void calc_star(bounty_ptr p_bounty);
	//生成指定星级的国家悬赏任务id
	uint32_t make_bounty_id(uint32_t star, uint32_t level, const std::vector<uint32_t>& exclude_vec);
public:

	uint32_t rand_task_id( uint32_t level );
private:
	std::string m_key = "";
	uint64_t m_role_uid = 0;
	std::vector<bounty_ptr>	m_bounty_vec;
	uint32_t m_refresh_count = 0;		//剩余刷新次数
	uint32_t m_star_count = 0;			//已完成的星数
	std::vector<uint32_t> m_box_vec;	//已领取的星级宝箱
	static uint32_t s_max_accept_count;		//最大可接取次数
	static uint32_t s_refresh_count;		//每天的刷新次数
	static uint32_t s_min_level;		//最低接取等级
	static std::vector<uint32_t> s_first_prob;	//第一个悬赏的概率（3，4，5）
	static std::vector<uint32_t> s_second_prob; //第二个悬赏的概率（1---5）
	static std::vector<uint32_t> s_third_prob;  //第三个悬赏的概率（1---5）
	static std::map<uint32_t, uint32_t> s_box_info;  //宝箱需要的星数:掉落
	static std::map<uint32_t, std::vector<uint32_t>> s_bounty_map_by_star; //星级为索引的悬赏列表
};
typedef boost::shared_ptr<bounty_manager_t> bounty_mgr_ptr;
#endif // !__BOUNTY_MANAGER_H__


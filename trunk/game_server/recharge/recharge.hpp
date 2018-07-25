#ifndef __GAME_RECHARGE_H__
#define __GAME_RECHARGE_H__

#include "protos_fwd.hpp"
#include "macros.hpp"

class recharge_t
{
public:
	recharge_t(uint64_t uid);
    ~recharge_t();
public:
	void load_data(const proto::common::recharge_data& ld);
	void peek_data(proto::common::recharge_data* p_data);
	void save_self();

	void one_day();

	uint32_t get_red_num();
public:

	///��ֵ���
	void		add_total_cny(uint32_t val, proto::common::role_change_data* p_data);
	uint32_t	get_total_cny() { return m_total_cny; }

	void		set_vip_level(uint32_t vip_level, proto::common::role_change_data* p_data);
	uint32_t	get_vip_level() { return m_vip_level; }

	bool		is_first_recharge();
	void		add_recharge_count();
	uint32_t	get_recharge_count() { return m_recharge_count; }
	uint32_t	get_recharge_reward_flag() { return m_recharge_reward_flag; }

	bool		already_get_recharge_gift(uint32_t bit = 0);
	void		set_recharge_reward_flag(uint32_t bit = 0);
	uint32_t	get_recharge_gift(proto::common::role_change_data* p_data);
	
	uint32_t rmb_recharge(uint32_t tid, proto::common::role_change_data* p_data);

	void get_recharge_cheap(proto::client::gc_recharge_cheap_reply& reply);

	uint32_t cheap_buy(uint32_t cheap_id, proto::client::gc_recharge_cheap_buy_reply& reply);
private:
	uint32_t get_cheap_count(uint32_t cheap_id);
private:
	std::string m_key = "";
	uint64_t m_owner = 0;

	uint32_t	m_vip_level = 0;				///vip�ȼ�
	uint32_t	m_total_cny = 0;				///�ۼƳ�ֵ���
	uint32_t	m_recharge_count = 0;			///��ֵ����(�׳� �γ�)
	uint32_t	m_recharge_reward_flag = 0;		///��ֵ������ȡ���λ(�׳� �γ�)

	std::map<uint32_t, uint32_t> m_cheap_count;	//ÿ���ػ�
};
typedef boost::shared_ptr<recharge_t> recharge_ptr;
#endif
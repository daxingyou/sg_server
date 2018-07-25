#ifndef __GAME_EXPEDITION_H__
#define __GAME_EXPEDITION_H__

#include "mirror_fight_data.hpp"
#include "hero_save_data.hpp"

class expedition_stage_t
{
public:
	expedition_stage_t();
	~expedition_stage_t();
public:
	void load_data(const proto::common::expedition_stage_single& ed);
	void load_fight_data(const proto::common::expedition_stage_single& ed);
	void peek_data(proto::common::expedition_stage_single* p_data);

	void fill_data();
public:
	uint32_t get_tid() { return m_tid; }
	void set_tid(uint32_t val) { m_tid = val; }
	void set_pass_num(uint32_t val) { m_pass_num = val; }
	uint32_t get_pass_num() { return m_pass_num; }
	void set_award_num(uint32_t val) { m_award_num = val; }
	uint32_t get_award_num() { return m_award_num; }
public:
	uint32_t fight(uint64_t owner);
	void save_data(const proto::common::fight_save_data& esd);
private:
	uint32_t m_tid = 0;
	uint32_t m_pass_num = 0;
	uint32_t m_award_num = 0;
	std::vector<mirror_fight_data_ptr> m_fight_data;
};
typedef boost::shared_ptr<expedition_stage_t> expedition_stage_ptr;

class expedition_t
{
public:
	expedition_t(uint64_t uid);
    ~expedition_t();
public:
	void load_data(const proto::common::expedition_data& ed);
	void peek_data(proto::common::expedition_data* p_data);
	void save_self();
public:
	void set_pass_num(uint32_t val) { m_pass_num = val; }
	uint32_t get_pass_num() { return m_pass_num; }
	void set_refresh_count(uint32_t val) { m_refresh_count = val; }
	uint32_t get_refresh_count() { return m_refresh_count; }
	void set_help_count(uint32_t val) { m_help_count = val; }
	uint32_t get_help_count() { return m_help_count; }
	void clear_glory() { m_glory = 0; }
public:
	void init_new_data();
	void reset_shop();
	void one_day();
	void make_data();
	void load_fight_data(const proto::server::eg_expedition_fight_data_reply& reply);
	//攻打
	uint32_t fight(uint32_t stage_id);
	//攻打结果
	void fight_result(uint32_t stage_id, bool is_win, const proto::common::fight_save_data& fsd, 
		const proto::common::fight_save_data& esd, const std::vector<uint64_t>& helpers);
	//请求刷新
	uint32_t refresh_request();
	//刷新结果
	void refresh_reply(const proto::server::eg_expedition_refresh_reply& reply);
	//刷险单个关卡
	void refresh_stage_reply(const proto::server::eg_expedition_refresh_stage_reply& reply);
	//求助
	uint32_t help_request(const std::vector<uint64_t>& helper_list);
	//购买
	uint32_t shop_request(uint32_t buy_style, proto::client::gc_expedition_shop_reply& reply);
	//领取奖励
	uint32_t award(uint32_t stage_id, proto::common::role_change_data* p_data);
	//领取宝箱
	uint32_t box(uint32_t box_id, proto::client::gc_expedition_box_reply& reply);
	//同步荣耀值
	void notify_glory_to_center();

	static uint32_t get_stage_pass_num();
	static uint32_t get_max_pass_num();
	static uint32_t get_min_level();
	static uint32_t get_max_help_count();
	static uint32_t get_help_reward();
	static uint32_t get_hero_min_level();
public:
	int32_t get_hp(uint64_t hero_uid);
	int32_t get_skill_cd(uint64_t hero_uid, uint32_t skill_id);
	void save_data(const proto::common::fight_save_data& fsd);
	//回复血量
	uint32_t shop_hp(uint64_t hero_uid);
	//清技能CD
	uint32_t shop_cd(uint64_t hero_uid);
	//回复满状态
	uint32_t shop_all(uint64_t hero_uid);
	const std::map<uint64_t, hero_save_data_ptr> get_hero_list() { return m_hero_map; }
	bool has_hero();
private:
	std::string m_key = "";
	uint64_t m_owner = 0;

	uint32_t m_pass_num = 0;
	uint32_t m_refresh_count = 0;
	std::map<uint32_t, expedition_stage_ptr> m_stage_map;
	std::map<uint64_t, hero_save_data_ptr> m_hero_map;
	uint32_t m_help_count = 0;
	uint32_t m_call_help_count = 0;
	uint32_t m_glory = 0;
	uint32_t m_shop_id = 0;
	uint32_t m_is_selled = 0;
	std::vector<uint32_t> m_box_vec;
	uint32_t m_liveness = 0;
};
typedef boost::shared_ptr<expedition_t> expedition_ptr;
#endif
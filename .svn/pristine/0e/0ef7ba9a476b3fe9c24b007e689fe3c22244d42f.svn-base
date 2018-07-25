#ifndef SKILL_DATA_HPP
#define SKILL_DATA_HPP

#include "tblh/SkillEffectTable.tbls.h"
#include "protocpp/common.pb.h"

class fight_hero_t;
typedef boost::shared_ptr<fight_hero_t> fight_hero_ptr;

struct skill_data_t
{
public:
	void set_skill_cd(uint32_t cd);
public:
	uint32_t skill_id = 0;
	uint32_t skill_level = 0;				//技能等级
	uint32_t skill_cd = 0;					//技能cd
	proto::common::skill_type type;			//技能类型
	uint32_t is_use_public_cd = 0;		//是否使用公共技能CD
};


//技能原始变身设计 既有替换又有强化，没有使用很合理的 buff_element增强来做，导致了同一个位置的技能，
//可能使用公共CD ,或者独立CD。只能采用位置对应，双CD走的方式来实现了
struct skill_info_t
{
public:
	skill_data_t *get_skill_data_by_skill_id(uint32_t skill_id );
	skill_data_t *get_skill_data_by_turn_level(uint32_t turn_level);

	uint32_t get_skill_cd(uint32_t skill_id );
	uint32_t get_skill_id(uint32_t turn_level);
	uint32_t get_skill_level(uint32_t skill_id );

	void set_skill_cd(uint32_t skill_id,uint32_t cd);
	void add_skill_cd( fight_hero_ptr owner, uint32_t skill_id, uint32_t cd = 99);
	void add_skill_cd_ex(uint32_t skill_id,uint32_t cd);
	void sub_skill_cd(uint32_t skill_id, uint32_t cd);

	void set_pub_skill_cd(uint32_t cd);
public:
	uint32_t m_public_cd = 0;						//公共CD
	uint32_t m_skill_level = 0;						//公共技能等级
	std::vector<skill_data_t> m_skill_data_list;	//变身对应-技能列表(0) 是普通技能
};

#endif

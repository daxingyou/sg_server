#ifndef __GAME_FORMATION_H__
#define __GAME_FORMATION_H__
#include <map>
#include "protos_fwd.hpp"
#include "hero/hero.hpp"

#define FORM_SIZE 9
#define FORM_MAX_HERO 5
#define  DEFAULT_FORM_ID 1
#define  DEFAULT_ROLE_POS 1
#define  MAX_FORM_COUNT 10


class object_base_t;
typedef boost::shared_ptr<object_base_t> object_base_ptr;

typedef std::pair<uint64_t, proto::common::EM_SCENEOBJECT_TYPE> object_id_type;
struct PosHeroData
{
	object_id_type	id;
	uint64_t		uid;


	PosHeroData(uint64_t owner, proto::common::EM_SCENEOBJECT_TYPE type, uint64_t uid)
	{
		this->id.first = owner;
		this->id.second = type;
		this->uid = uid;
	}

	PosHeroData(object_id_type id, uint64_t uid)
	{
		this->id = id;
		this->uid = uid;
	}

	PosHeroData()
	{
		this->uid = 0;
	}
};

class formation_t
{
public:
	formation_t(uint64_t owner);
	formation_t(uint32_t index, uint64_t owner);

	virtual ~formation_t();

	void load_data(const proto::common::formation_single& single);
	void peek_data(proto::common::formation_single* p_single);
	void save_self(bool is_new = false);

	void peek_fight_data(proto::common::hero_data* p_data);

	void form(uint32_t pos, uint64_t unique_id);
	void form(uint32_t pos, PosHeroData& data);
	void remove(uint32_t pos);
    void remove(uint64_t unique_id);
	// 更新布阵
	void update_formation(
		const std::map<uint32_t, PosHeroData> &pos_hero,
		uint32_t tactic_id, 
		bool is_new = false, 
		bool is_save = true);
	void add_hero_to_formation(uint64_t unique_id);
	void remove_hero_from_formation(uint64_t unique_id);
	void replace_hero_from_formation(uint64_t old_hero_uid, uint64_t new_hero_uid);
	bool has_hero(uint64_t unique_id);

	

	// 仅玩家可用接口
	uint32_t get_pos_by_unique_id(uint64_t unique_id);
	hero_ptr get_hero_by_pos(uint32_t pos);


	// 清空布阵武将
	void clear_heroes();
	void reset_heroes();
	// get set
	//计算战力
	uint32_t calc_fighting();
public:
	uint32_t get_index() { return m_index; }

    const std::map<uint32_t, PosHeroData>& get_pos_hero() {return m_pos_hero;}
    uint32_t get_free_pos();
	uint32_t get_tactic_id() { return m_tactic_id; }

	void set_index(uint32_t index) { m_index = index; }
	void set_tactic_id(uint32_t tactic_id) { m_tactic_id = tactic_id; }
	// 成员变量
	uint32_t get_hero_count() { return m_pos_hero.size(); }
protected:

	uint64_t m_owner = 0;
	uint32_t m_index = 0;
	std::map<uint32_t, PosHeroData> m_pos_hero;
	uint32_t m_tactic_id = 0;
};

class troop_formation_t :public formation_t
{
public:
	troop_formation_t(uint32_t index, uint64_t troop_id);
	~troop_formation_t() { }

	troop_formation_t& operator=(formation_t& form);

	
private:
	uint64_t m_troop_id;
	
};
typedef boost::shared_ptr<formation_t> formation_ptr;
typedef boost::shared_ptr<troop_formation_t> troop_formation_ptr;
typedef std::map<uint32_t, formation_ptr> formation_map;

#endif // !__GAME_FORMATION_H__

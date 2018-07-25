#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <set>
#include <vector>
#include <unordered_map>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "map_common.hpp"
#include "map_area.hpp"
#include "monster/monster_define.hpp"
#include "tblh/MapTable.tbls.h"
#include "collect/collect_point.hpp"
#include "map_patrol.hpp"

typedef std::vector<uint32_t> nine_vector;					// 9����Ϣ
typedef std::unordered_map<uint32_t, nine_vector> nine_map;	// 9����Ϣ

typedef boost::shared_ptr<map_area_t> map_area_ptr;
// ��������Ϣ
typedef std::map<uint32_t, map_area_ptr> map_area_map;
// ����������Ϣ
typedef std::vector<monster_common::monster_define_t> monster_define_vec;
// �ɼ�����Ϣ
typedef std::vector<t_collect_define> collect_define_vec;
// ����ս����
typedef std::vector<monster_common::king_war_monster> king_war_define_vec;
// ������̬����
typedef std::vector<monster_common::dungeon_summon_dynamic> dungeon_summon_dynamic_vec;
// ������ʱ����
typedef std::vector<monster_common::dungeon_summon_delay> dungeon_summon_delay_vec;
// ����Ѳ�߹�������
typedef boost::shared_ptr<map_patrol_t> map_patrol_ptr;

class map_t
{
public:
	map_t(uint32_t map_id, proto::server::em_map_type type);
	~map_t();

public:
	// Ĭ�ϼ���9����Ϣ
	bool load(MapTable* p_map, bool is_load_all = true);
	// �����赲��������Ϣ
	bool load_map_block(const char* filename);
	// ���ص�ͼ��ȫ�������׹֡��ɼ��㡢����ս������Ϣ
	bool load_scene_info(const char* filename);
	// ����
	void clear();

public:
	// ��ͼid
	uint32_t get_map_tid() const { return m_map_id; }
	uint32_t get_map_group_id() const { return m_map_group_id; }
	// ���Ͷ���
	proto::server::em_map_type get_map_type() const { return m_map_type; }

	// ��������
	uint32_t get_map_max_size() const { return m_map_max_size; }

	// ��֤����Ϸ�
	const bool pos_validate(const uint32_t x, const uint32_t y) const { return x < m_max_x && y < m_max_y; }

	// ��������赲��Ϣ
	const bool check_block(const uint32_t x, const uint32_t y) const
	{
		if (pos_validate(x, y))
		{
			return (m_grid_tiles[y * m_max_x + x].flags == 0);
		}
		else
		{
			return true;
		}
	}

	// �����赲��Ϣ
	void set_block(const uint32_t x, const uint32_t y, const uint8_t block)
	{
		if (pos_validate(x, y))
		{
			m_grid_tiles[y * m_max_x + x].type |= block;
		}
	}

	// ���Ŀ���赲����
	void clear_block(const uint32_t x, const uint32_t y, const uint8_t block)
	{
		if (pos_validate(x, y))
		{
			m_grid_tiles[y * m_max_x + x].type &= ~block;
		}
	}

	// ��ȡ�ر�����
	const grid_tile_t* get_grid_tile(const uint32_t x, const uint32_t y) const
	{
		if (pos_validate(x, y))
		{
			return &m_grid_tiles[y * m_max_x + x];
		}
		else
		{
			return NULL;
		}
	}
	
	// ����id
	uint32_t get_area_id(uint32_t x, uint32_t y) const
	{
		if (!check_block(x, y))
		{
			return m_grid_tiles[y * m_max_x + x].flags;
		}
		else
		{
			return 0;
		}
	}

	/**
	* \brief ����������ȡ��Χ9������Ļ���
	* \param index_key ������
	*/
	const nine_vector& get_nine_keys(uint32_t index_key);

	/**
	* \brief ����������ȡ��ǰ��3������5������Ļ���
	* \param posi ������
	* \param direct ����
	* \return ��������������
	*/
	const nine_vector& get_direct_screen(uint32_t index_key, int32_t dir);

	/**
	* \brief ����������ȡ����3������5������Ļ���
	* \param posi ������
	* \param direct ����
	* \return ��������������
	*/
	const nine_vector& get_reversedirect_screen(uint32_t index_key, int32_t dir);

	/*  ������x,yת���ɸ������Ӧ_scene_map�е�����.
	x,y ���꣬ index_x ,index_y ���ض�Ӧ��������
	����ֵ���ҵ��Ծ͵���������true������false
	*/
	uint32_t convert_xy_2_map_key(float x, float y);

	/**
	* \brief ���������жϾŸ�����
	* \param oldindex ԭ�������
	* \param newindex �����������
	* \return ������
	*/
	int32_t getScreenDirect(uint32_t oldindex, uint32_t newindex) const
	{
		uint32_t orgX = oldindex % m_max_sector_x;
		uint32_t orgY = oldindex / m_max_sector_x;
		uint32_t newX = newindex % m_max_sector_x;
		uint32_t newY = newindex / m_max_sector_x;

		if (abs((int32_t)newX - (int32_t)orgX) > 1 || abs((int32_t)newY - (int32_t)orgY) > 1)
			return _DIR_NOT_CROSS;

		if (orgX == newX && orgY >= newY + 1)
		{
			return _DIR_UP;
		}
		else if (orgX + 1 <= newX && orgY >= newY + 1)
		{
			return _DIR_UPRIGHT;
		}
		else if (orgX + 1 <= newX && orgY == newY)
		{
			return _DIR_RIGHT;
		}
		else if (orgX + 1 <= newX && orgY + 1 <= newY)
		{
			return _DIR_RIGHTDOWN;
		}
		else if (orgX == newX && orgY + 1 <= newY)
		{
			return _DIR_DOWN;
		}
		else if (orgX >= 1 + newX && orgY + 1 <= newY)
		{
			return _DIR_DOWNLEFT;
		}
		else if (orgX >= 1 + newX && orgY == newY)
		{
			return _DIR_LEFT;
		}
		else if (orgX >= 1 + newX && orgY >= 1 + newY)
		{
			return _DIR_LEFTUP;
		}
		else
		{
			log_error("scrren direct old[%u] new[%u]", oldindex, newindex);
			return _DIR_NOT_CROSS;
		}
	}

	static int32_t get_comp_dir(float from_x, float from_y, float to_x, float to_y) {
		int32_t x = to_x - from_x;
		int32_t y = to_y - from_y;
		int32_t absx = abs(x);
		int32_t absy = abs(y);

		if (absx > absy && absy < absx / 2) {
			y = 0;
		} else if (absx < absy && absx < absy / 2) {
			x = 0;
		}

		int32_t bkDir = _DIR_UP;

		if (x < 0 && y < 0)
			bkDir = _DIR_LEFTUP;
		else if (x < 0 && 0 == y)
			bkDir = _DIR_LEFT;
		else if (x < 0 && y > 0)
			bkDir = _DIR_DOWNLEFT;
		else if (0 == x && y > 0)
			bkDir = _DIR_DOWN;
		else if (x > 0 && y > 0)
			bkDir = _DIR_RIGHTDOWN;
		else if (x > 0 && 0 == y)
			bkDir = _DIR_RIGHT;
		else if (x > 0 && y < 0)
			bkDir = _DIR_UPRIGHT;
		else if (0 == x && y < 0)
			bkDir = _DIR_UP;

		return bkDir;
	}

public:
	// ������
	uint32_t get_born_point(const uint32_t level);
	// �����
	uint32_t get_revive_point() const { return m_revive_point; }
	// ����id
	uint32_t get_city_id() const { return m_city_id; }

	// �õ�������Ϣ
	bool load_area_info(const boost::property_tree::ptree& pt);
	map_area_ptr get_map_area_info(uint32_t id);
	// �õ��ɼ����б�
	bool load_map_collect_points(const boost::property_tree::ptree& pt);
	const collect_define_vec& get_map_collect_points() { return m_collect_vec; }

	// ����������
	bool read_monster_config(const boost::property_tree::ptree& pt, monster_common::monster_define_t* define);
	// �õ������б�
	bool load_map_monsters(const boost::property_tree::ptree& pt);
	const monster_define_vec& get_map_monsters() { return m_monster_vec; }
	// �õ�����սnpc�б�
	bool load_map_king_war(const boost::property_tree::ptree& pt);
	const king_war_define_vec& get_map_king_war() { return m_king_war_vec; }
	// �õ�������̬�����б�
	bool load_map_summon_dynamic(const boost::property_tree::ptree& pt);
	const dungeon_summon_dynamic_vec& get_map_summon_dynamic() { return m_dungeon_summon_dynamic_vec; }
	// �õ������ӳ��ٻ��б�
	bool load_map_summon_delay(const boost::property_tree::ptree& pt);
	const dungeon_summon_delay_vec& get_map_summon_delay() { return m_dungeon_summon_delay_vec; }
	// �õ�Ѳ�߹�����Ϣ
	bool load_map_patrol_config(const boost::property_tree::ptree& pt);
	const map_patrol_ptr& get_map_patrol_info() { return m_map_patrol_info; }

	// ��ͼ������Ϣ
private:
	//AStarPath aStarPath;
	uint32_t m_map_id = 0;						// ��ͼid
	proto::server::em_map_type m_map_type = proto::server::map_type_none;				// ��ͼ����
	uint32_t m_map_group_id = 0;	// ������id

	uint32_t m_max_x = 0;
	uint32_t m_max_y = 0;

	grid_tiles m_grid_tiles;					// ������Ϣ

	// 9��Ļ��Ϣ
private:
	uint32_t m_max_sector_x = 0;				// ����
	uint32_t m_max_sector_y = 0;				// ����
	uint32_t m_map_max_size = 0;				// ��ͼ�����Ļ��
	nine_map m_nine_map;						// 9����ϵ
	nine_map m_direct_screen[_DIR_MAX];			// ����仯������������
	nine_map m_reversedirect_screen[_DIR_MAX];	// ����仯������������
	static nine_vector CONST_NINE_VECTOR;		// ��̬������ģ��protobuf

	// ������Ϣ
private:
	uint32_t m_born_point = 0;		// ��������ת��id
	std::vector<std::pair<uint32_t, uint32_t>> m_config_born_list;	// ��ת�������б�
	uint32_t m_revive_point = 0;	// ��������ת��
	uint32_t m_city_id = 0;			// ��Ӧ�ĳ���

	map_area_map m_area_map;			// ������Ϣ
	monster_define_vec m_monster_vec;	// ��������
	collect_define_vec m_collect_vec;	// ����ɼ���
	king_war_define_vec m_king_war_vec;	// ����սnpc
	dungeon_summon_dynamic_vec m_dungeon_summon_dynamic_vec;	// ���������ٻ�����
	dungeon_summon_delay_vec m_dungeon_summon_delay_vec;	// ������ʱ�ٻ�����
	map_patrol_ptr m_map_patrol_info = NULL;		// Ѳ�߹�����Ϣ
};

typedef boost::weak_ptr<map_t> map_wptr;
typedef boost::shared_ptr<map_t> map_ptr;

#endif

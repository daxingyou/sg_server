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

typedef std::vector<uint32_t> nine_vector;					// 9屏信息
typedef std::unordered_map<uint32_t, nine_vector> nine_map;	// 9屏信息

typedef boost::shared_ptr<map_area_t> map_area_ptr;
// 区域暗雷信息
typedef std::map<uint32_t, map_area_ptr> map_area_map;
// 区域明雷信息
typedef std::vector<monster_common::monster_define_t> monster_define_vec;
// 采集点信息
typedef std::vector<t_collect_define> collect_define_vec;
// 王城战龙脉
typedef std::vector<monster_common::king_war_monster> king_war_define_vec;
// 副本动态怪物
typedef std::vector<monster_common::dungeon_summon_dynamic> dungeon_summon_dynamic_vec;
// 副本定时怪物
typedef std::vector<monster_common::dungeon_summon_delay> dungeon_summon_delay_vec;
// 场景巡逻怪物配置
typedef boost::shared_ptr<map_patrol_t> map_patrol_ptr;

class map_t
{
public:
	map_t(uint32_t map_id, proto::server::em_map_type type);
	~map_t();

public:
	// 默认加载9屏信息
	bool load(MapTable* p_map, bool is_load_all = true);
	// 加载阻挡和区域信息
	bool load_map_block(const char* filename);
	// 加载地图安全区、明雷怪、采集点、王城战怪物信息
	bool load_scene_info(const char* filename);
	// 清理
	void clear();

public:
	// 地图id
	uint32_t get_map_tid() const { return m_map_id; }
	uint32_t get_map_group_id() const { return m_map_group_id; }
	// 类型定义
	proto::server::em_map_type get_map_type() const { return m_map_type; }

	// 最大格子数
	uint32_t get_map_max_size() const { return m_map_max_size; }

	// 验证坐标合法
	const bool pos_validate(const uint32_t x, const uint32_t y) const { return x < m_max_x && y < m_max_y; }

	// 检查坐标阻挡信息
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

	// 设置阻挡信息
	void set_block(const uint32_t x, const uint32_t y, const uint8_t block)
	{
		if (pos_validate(x, y))
		{
			m_grid_tiles[y * m_max_x + x].type |= block;
		}
	}

	// 清除目标阻挡点标记
	void clear_block(const uint32_t x, const uint32_t y, const uint8_t block)
	{
		if (pos_validate(x, y))
		{
			m_grid_tiles[y * m_max_x + x].type &= ~block;
		}
	}

	// 获取地表数据
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
	
	// 区域id
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
	* \brief 以中心屏获取周围9屏的屏幕编号
	* \param index_key 中心屏
	*/
	const nine_vector& get_nine_keys(uint32_t index_key);

	/**
	* \brief 以中心屏获取向前的3屏或者5屏的屏幕编号
	* \param posi 中心屏
	* \param direct 方向
	* \return 输出的屏编号向量
	*/
	const nine_vector& get_direct_screen(uint32_t index_key, int32_t dir);

	/**
	* \brief 以中心屏获取向后的3屏或者5屏的屏幕编号
	* \param posi 中心屏
	* \param direct 方向
	* \return 输出的屏编号向量
	*/
	const nine_vector& get_reversedirect_screen(uint32_t index_key, int32_t dir);

	/*  把坐标x,y转换成该坐标对应_scene_map中的索引.
	x,y 坐标， index_x ,index_y 返回对应的索引。
	返回值：找到对就的索引返回true，否则false
	*/
	uint32_t convert_xy_2_map_key(float x, float y);

	/**
	* \brief 根据两屏判断九个方向
	* \param oldindex 原点屏编号
	* \param newindex 结束点屏编号
	* \return 方向编号
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
	// 出生点
	uint32_t get_born_point(const uint32_t level);
	// 复活点
	uint32_t get_revive_point() const { return m_revive_point; }
	// 城市id
	uint32_t get_city_id() const { return m_city_id; }

	// 得到区域信息
	bool load_area_info(const boost::property_tree::ptree& pt);
	map_area_ptr get_map_area_info(uint32_t id);
	// 得到采集点列表
	bool load_map_collect_points(const boost::property_tree::ptree& pt);
	const collect_define_vec& get_map_collect_points() { return m_collect_vec; }

	// 读怪物配置
	bool read_monster_config(const boost::property_tree::ptree& pt, monster_common::monster_define_t* define);
	// 得到明雷列表
	bool load_map_monsters(const boost::property_tree::ptree& pt);
	const monster_define_vec& get_map_monsters() { return m_monster_vec; }
	// 得到王城战npc列表
	bool load_map_king_war(const boost::property_tree::ptree& pt);
	const king_war_define_vec& get_map_king_war() { return m_king_war_vec; }
	// 得到副本动态怪物列表
	bool load_map_summon_dynamic(const boost::property_tree::ptree& pt);
	const dungeon_summon_dynamic_vec& get_map_summon_dynamic() { return m_dungeon_summon_dynamic_vec; }
	// 得到副本延迟召唤列表
	bool load_map_summon_delay(const boost::property_tree::ptree& pt);
	const dungeon_summon_delay_vec& get_map_summon_delay() { return m_dungeon_summon_delay_vec; }
	// 得到巡逻怪物信息
	bool load_map_patrol_config(const boost::property_tree::ptree& pt);
	const map_patrol_ptr& get_map_patrol_info() { return m_map_patrol_info; }

	// 地图基础信息
private:
	//AStarPath aStarPath;
	uint32_t m_map_id = 0;						// 地图id
	proto::server::em_map_type m_map_type = proto::server::map_type_none;				// 地图类型
	uint32_t m_map_group_id = 0;	// 场景组id

	uint32_t m_max_x = 0;
	uint32_t m_max_y = 0;

	grid_tiles m_grid_tiles;					// 格子信息

	// 9屏幕信息
private:
	uint32_t m_max_sector_x = 0;				// 屏宽
	uint32_t m_max_sector_y = 0;				// 屏高
	uint32_t m_map_max_size = 0;				// 地图最大屏幕数
	nine_map m_nine_map;						// 9屏关系
	nine_map m_direct_screen[_DIR_MAX];			// 正向变化五屏或者三屏
	nine_map m_reversedirect_screen[_DIR_MAX];	// 反向变化五屏或者三屏
	static nine_vector CONST_NINE_VECTOR;		// 静态变量，模仿protobuf

	// 杂项信息
private:
	uint32_t m_born_point = 0;		// 出生点跳转表id
	std::vector<std::pair<uint32_t, uint32_t>> m_config_born_list;	// 跳转点配置列表
	uint32_t m_revive_point = 0;	// 出生点跳转表
	uint32_t m_city_id = 0;			// 对应的城市

	map_area_map m_area_map;			// 区域信息
	monster_define_vec m_monster_vec;	// 区域明雷
	collect_define_vec m_collect_vec;	// 区域采集点
	king_war_define_vec m_king_war_vec;	// 王诚战npc
	dungeon_summon_dynamic_vec m_dungeon_summon_dynamic_vec;	// 副本限制召唤怪物
	dungeon_summon_delay_vec m_dungeon_summon_delay_vec;	// 副本定时召唤怪物
	map_patrol_ptr m_map_patrol_info = NULL;		// 巡逻怪物信息
};

typedef boost::weak_ptr<map_t> map_wptr;
typedef boost::shared_ptr<map_t> map_ptr;

#endif

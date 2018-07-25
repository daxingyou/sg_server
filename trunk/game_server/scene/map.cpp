#include "map.hpp"
#include <math.h>
#include "main/game_server_fwd.hpp"
#include "main/game_server_fwd.hpp"
#include "scene_manager.hpp"
#include "tblh/JumpTable.tbls.h"
#include "config/config_manager.hpp"

nine_vector map_t::CONST_NINE_VECTOR;

map_t::map_t(uint32_t map_id, proto::server::em_map_type type) : m_map_id(map_id), m_map_type(type)
{

}

map_t::~map_t()
{
	
}

bool map_t::load(MapTable* p_map, bool is_load_all /*= true*/)
{
	if (NULL == p_map)
	{
		log_error("p_map[%u] null error", m_map_id);
		return false;
	}

	// 重置
	clear();

	// 出生点信息
	//m_born_point = p_map->scene_born();
	const std::vector<std::pair<uint32_t, uint32_t>>& born_config_list = p_map->get_born_list();
	m_config_born_list.assign(born_config_list.begin(), born_config_list.end());

	// 加载复活点
	m_revive_point = p_map->revive_point();

	// 城市id
	m_city_id = p_map->scene_city();

	// 场景组id
	m_map_group_id = p_map->map_group_id();

	// 对于不是本场景地图不加载地图9屏等信息，一般在这个场景中也不会用到这些信息
	if (!is_load_all)
	{
		return true;
	}

	// 加载地图阻挡,区域信息
	char configPath[256] = { 0 };
    snprintf(configPath, sizeof(configPath), "%s/scene_name_%d.map", env::xml->map_path.c_str(), m_map_id);
	if (!load_map_block(configPath))
	{
		log_error("load map[%s] info error", configPath);
		return false;
	}

	// 设置格子信息
	m_max_sector_x = (m_max_x + SCREEN_WIDTH - 1) / SCREEN_WIDTH;
	m_max_sector_y = (m_max_y + SCREEN_HEIGHT - 1) / SCREEN_HEIGHT;

	m_map_max_size = m_max_sector_x * m_max_sector_y;

	const int32_t adjust[9][2] = { { 0, -1 },{ 1, -1 },{ 1, 0 },{ 1, 1 },{ 0, 1 },{ -1, 1 },{ -1, 0 },{ -1, -1 },{ 0, 0 } };

	// 循环建立9屏数据
	for (int32_t y = 0; y < (int32_t)m_max_sector_y; ++y)
	{
		for (int32_t x = 0; x < (int32_t)m_max_sector_x; ++x)
		{
			uint32_t s_key = (y * m_max_sector_x) + x;
			// 计算周围9屏关系
			{
				nine_vector nine;
				for (int32_t i = 0; i < 9; ++i)
				{
					int32_t s_x = x + adjust[i][0];
					int32_t s_y = y + adjust[i][1];
					if (s_x >= 0 && s_y >= 0 && s_x < (int32_t)m_max_sector_x && s_y < (int32_t)m_max_sector_y)
					{
						nine.push_back(s_y * m_max_sector_x + s_x);
					}
				}

				m_nine_map.insert(std::make_pair(s_key, nine));
			}

			// 计算正向变化五屏或者三屏
			for (int32_t dir = 0; dir < 8; ++dir)
			{
				int32_t start, end;
				nine_vector nine;

				if (1 == dir % 2) {
					//斜方向
					start = 6;
					end = 10;
				}
				else {
					//正方向
					start = 7;
					end = 9;
				}

				for (int32_t i = start; i <= end; i++) 
				{
					int32_t s_x = x + adjust[(i + dir) % 8][0];
					int32_t s_y = y + adjust[(i + dir) % 8][1];
					if (s_x >= 0 && s_y >= 0 && s_x < (int32_t)m_max_sector_x && s_y < (int32_t)m_max_sector_y)
					{
						nine.push_back(s_y * m_max_sector_x + s_x);
					}
				}

				m_direct_screen[dir].insert(std::make_pair(s_key, nine));
			}

			//计算反向变化五屏或者三屏
			for (int32_t dir = 0; dir < 8; ++dir)
			{
				int32_t start, end;
				nine_vector nine;

				if (1 == dir % 2) {
					//斜方向
					start = 2;
					end = 6;
				}
				else {
					//正方向
					start = 3;
					end = 5;
				}

				for (int32_t i = start; i <= end; i++)
				{
					int32_t s_x = x + adjust[(i + dir) % 8][0];
					int32_t s_y = y + adjust[(i + dir) % 8][1];
					if (s_x >= 0 && s_y >= 0 && s_x < (int32_t)m_max_sector_x && s_y < (int32_t)m_max_sector_y)
					{
						nine.push_back(s_y * m_max_sector_x + s_x);
					}
				}
				m_reversedirect_screen[dir].insert(std::make_pair(s_key, nine));
			}
		}
	}

	if (m_nine_map.empty())
	{
		log_error("map[%u] have no nine info!", m_map_id);
		return false;
	}

	// 加载区域信息
	char area_path[256] = { 0 };
    snprintf(area_path, sizeof(area_path), "%s/map/scene_%d.xml", env::xml->config_path.c_str(), m_map_id);
	if (!load_scene_info(area_path))
	{
		log_error("map[%u] load_scene_info error!", m_map_id);
		return false;
	}

	// 加载所有安全区信息
	std::vector<uint32_t> safe_areas;
	common::string_util_t::split<uint32_t>(p_map->scene_safe(), safe_areas, "$");
	if (!safe_areas.empty())
	{
		for (std::vector<uint32_t>::const_iterator citr = safe_areas.begin();
			citr != safe_areas.end(); ++citr)
		{
			scene_manager_t::add_safe_area_id(*citr);
		}
	}

	//SCENE_LOG("success load map[%u]", m_map_id);

	return true;
}

bool map_t::load_map_block(const char* filename)
{
	if (NULL == filename)
	{
		log_error("map[%u] error area name", m_map_id);
		return false;
	}

	FILE *fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		log_error("load map file[%s] failed", filename);
		return false;
	}

	map_file_header hdr;
	fread(&hdr, sizeof(hdr), 1, fp);
	//printf("\033[31m load[%s] width:%u height:%u\033[0m\n", filename, hdr.width, hdr.height);
	m_max_x = hdr.width;
	m_max_y = hdr.height;

	if (m_max_x == 0 || m_max_y == 0 || m_max_x > 2000 || m_max_y > 2000)
	{
		log_error("map[%u] map size[%u, %u] error", m_map_id, m_max_x, m_max_y);
		fclose(fp);
		return false;
	}

	//printf("\033[31m grid_tile_t size:[%u] \033[0m\n", (uint32_t)sizeof(grid_tile_t));

	// 设置容量大小
	m_grid_tiles.resize(m_max_x * m_max_y);
	fread(&m_grid_tiles[0], m_grid_tiles.size() * sizeof(grid_tile_t), 1, fp);

	fclose(fp);

	return true;
}

bool map_t::load_area_info(const boost::property_tree::ptree& pt)
{
	if (pt.size() == 0)
		return true;

	try
	{
		map_area_ptr p_area = NULL;
		for (const auto& area : pt)
		{
			if (area.first == "<xmlattr>")
			{
				uint32_t area_id = area.second.get<uint32_t>("id");
				p_area.reset(new map_area_t(area_id));
				if (NULL == p_area)
				{
					log_error("map[%u] area null", m_map_id);
					return false;
				}

				std::pair<map_area_map::iterator, bool> ret = m_area_map.insert(std::make_pair(area_id, p_area));
				if (ret.second == false)
				{
					log_error("map[%u] read area id[%u] again", m_map_id, area_id);
					return false;
				}

				p_area->set_trigger_min(area.second.get<uint32_t>("trigger_min"));
				p_area->set_trigger_max(area.second.get<uint32_t>("trigger_max"));
			}
			if (area.first == "monster")
			{
				for (const auto& monster : area.second)
				{
					uint32_t monster_id = monster.second.get<uint32_t>("id");
					uint32_t weight = monster.second.get<uint32_t>("weight");
					uint32_t task_id = monster.second.get<uint32_t>("task_id", 0);
					if (NULL != p_area)
					{
						p_area->add_dark_item(monster_id, weight, task_id);
					}
				}
			}
		}
	}
	catch (std::exception& e)
	{
		log_error("map[%u] read load_area_info error", m_map_id);
		return false;
	}

	return true;
}

bool map_t::read_monster_config(const boost::property_tree::ptree& pt, monster_common::monster_define_t* define)
{
	if (define == NULL)
	{
		log_error("define null error");
		return false;
	}

	try
	{
		// 这个几个配置一定会有
		define->id = pt.get<uint32_t>("id");
		define->pos_x = pt.get<float>("x");
		define->pos_y = pt.get<float>("y");
		define->dir = pt.get<uint32_t>("dir");
		define->type = pt.get<uint32_t>("type", 0);

		// ai配置 没有就设置默认
		uint32_t ai_id = pt.get<uint32_t>("ai", 0);
		if (ai_id != 0)
		{
			define->ai_id = ai_id;
			define->interval = pt.get<uint32_t>("interval", 0);

#if 0 // 暂时不读
			std::string str_paths = pt.get<std::string>("paths", "");
			if (!str_paths.empty())
			{
				std::vector<std::string> paths;
				common::string_util_t::split<std::string>(str_paths, paths, "$");

				std::vector<uint32_t> path_conf;
				monster_common::alert_path_t alert_path;

				for (uint32_t i = 0; i < paths.size(); ++i)
				{
					common::string_util_t::split<uint32_t>(paths[i], path_conf, ":");
					if (path_conf.size() == 3)
					{
						alert_path.pos_x = path_conf[0];
						alert_path.pos_y = path_conf[1];
						alert_path.stop_time = path_conf[2];
						define->paths.push_back(alert_path);
					}
				}
			}
#endif
		}
	}
	catch (std::exception& e)
	{
		log_error("map[%u] load monster data error", m_map_id);
		return false;
	}

	return true;
}

bool map_t::load_map_monsters(const boost::property_tree::ptree& pt)
{
	if (pt.size() == 0)
		return true;

	try
	{
		for (const auto& mob : pt)
		{
			if (mob.first == "<xmlattr>")
			{
				monster_common::monster_define_t define;
				if (!read_monster_config(mob.second, &define))
				{
					log_error("map[%u] read monster[%u] config error", m_map_id, define.id);
					return false;
				}

				m_monster_vec.push_back(define);
			}
		}
	}
	catch (std::exception& e)
	{
		log_error("map[%u] load_map_monsters error ", m_map_id);
		return false;
	}

	return true;
}

bool map_t::load_map_king_war(const boost::property_tree::ptree& pt)
{
	if (pt.size() == 0)
		return true;

	try
	{
		for (const auto& mob : pt)
		{
			if (mob.first == "<xmlattr>")
			{
				monster_common::king_war_monster king_war_define;
				if (!read_monster_config(mob.second, &king_war_define))
				{
					log_error("map[%u] read monster[%u] config error", m_map_id, king_war_define.id);
					return false;
				}

				king_war_define.hp = mob.second.get<uint32_t>("hp");
				king_war_define.area_id = mob.second.get<uint32_t>("area_id");
				king_war_define.king_war_type = mob.second.get<uint32_t>("king_type");

				m_king_war_vec.push_back(king_war_define);
			}
		}
	}
	catch (std::exception& e)
	{
		log_error("map[%u] load_map_king_war error ", m_map_id);
		return false;
	}

	return true;
}

bool map_t::load_map_collect_points(const boost::property_tree::ptree& pt)
{
	if (pt.size() == 0)
		return true;

	try
	{
		for (const auto& collect : pt)
		{
			if (collect.first == "<xmlattr>")
			{
				t_collect_define define;
				define.m_collect_id = collect.second.get<uint32_t>("id");
				define.m_x = collect.second.get<float>("x");
				define.m_y = collect.second.get<float>("y");

				m_collect_vec.push_back(define);
			}
		}
	}
	catch (std::exception& e)
	{
		log_error("map[%u] load_map_collect_points error ", m_map_id);
		return false;
	}

	return true;
}

bool map_t::load_map_summon_dynamic(const boost::property_tree::ptree& pt)
{
	if (pt.size() == 0)
		return true;

	try
	{
		for (const auto& mob : pt)
		{
			if (mob.first == "<xmlattr>")
			{
				monster_common::dungeon_summon_dynamic dyna_def;
				if (!read_monster_config(mob.second, &dyna_def))
				{
					log_error("map[%u] read monster[%u] config error", m_map_id, dyna_def.id);
					return false;
				}

				m_dungeon_summon_dynamic_vec.push_back(dyna_def);
			}
		}
	}
	catch (std::exception& e)
	{
		log_error("map[%u] load_map_summon_dynamic error ", m_map_id);
		return false;
	}

	return true;
}

bool map_t::load_map_summon_delay(const boost::property_tree::ptree& pt)
{
	if (pt.size() == 0)
		return true;

	try
	{
		for (const auto& mob : pt)
		{
			if (mob.first == "<xmlattr>")
			{
				monster_common::dungeon_summon_delay delay_def;
				if (!read_monster_config(mob.second, &delay_def))
				{
					log_error("map[%u] read monster[%u] config error", m_map_id, delay_def.id);
					return false;
				}

				delay_def.delay = mob.second.get<uint32_t>("delay", 0);
				m_dungeon_summon_delay_vec.push_back(delay_def);
			}
		}
	}
	catch (std::exception& e)
	{
		log_error("map[%u] load_map_summon_delay error ", m_map_id);
		return false;
	}

	return true;
}

bool map_t::load_map_patrol_config(const boost::property_tree::ptree& pt)
{
	if (pt.size() == 0)
		return true;

	try
	{
		for (const auto& patrol : pt)
		{
			if (patrol.first == "<xmlattr>")
			{
				m_map_patrol_info.reset(new map_patrol_t());
				if (m_map_patrol_info == NULL)
				{
					log_error("map[%u] patrol null", m_map_id);
					return false;
				}

				m_map_patrol_info->set_trigger_min(patrol.second.get<uint32_t>("trigger_min"));
				m_map_patrol_info->set_trigger_max(patrol.second.get<uint32_t>("trigger_max"));
			}
			if (patrol.first == "monster")
			{
				for (const auto& monster : patrol.second)
				{
					uint32_t monster_id = monster.second.get<uint32_t>("id");
					uint32_t weight = monster.second.get<uint32_t>("weight");
					if (NULL != m_map_patrol_info)
					{
						m_map_patrol_info->add_dark_item(monster_id, weight);
					}
				}
			}
		}
	}
	catch (std::exception& e)
	{
		log_error("map[%u] read load_map_patrol_config error", m_map_id);
		return false;
	}

	return true;
}

bool map_t::load_scene_info(const char* filename)
{
	if (NULL == filename)
	{
		log_error("map[%u] error area name", m_map_id);
		return false;
	}

	// 不是所有场景都有布怪文件
	if (access(filename, F_OK) != 0)
	{
		return true;
	}

	using boost::property_tree::ptree;
	ptree pt;

	try
	{
		read_xml(filename, pt);

		if (pt.size() == 0)
			return true;

		for (const auto& root : pt)
		{
			if (root.first == "map")
			{
				for (const auto& child : root.second)
				{
					// 区域暗雷信息
					if (child.first == "area")
					{
						if (!load_area_info(child.second))
							return false;
					}
					// 明雷怪信息
					else if (child.first == "static")
					{
						if (!load_map_monsters(child.second))
							return false;
					}
					// 王城战信息
					else if (child.first == "kingwar")
					{
						if (!load_map_king_war(child.second))
							return false;
					}
					// 采集信息
					else if (child.first == "collect")
					{
						if (!load_map_collect_points(child.second))
							return false;
					}
					else if (child.first == "dynamic")
					{
						if (!load_map_summon_dynamic(child.second))
							return false;
					} 
					else if (child.first == "delay")
					{
						if (!load_map_summon_delay(child.second))
							return false;
					}
					else
					{
						log_error("child[%s] not find", child.first.c_str());
						return false;
					}
				}
			}
			else if (root.first == "patrol")
			{
				if (!load_map_patrol_config(root.second))
					return false;
			}
		}
	}
	catch (std::exception& e)
	{
		log_error("\033[31mthrow error[%s]\033[0m\n", e.what());
		return false;
	}

	return true;

	//SCENE_LOG("success load map[%s] area info", filename);
}

void map_t::clear()
{
	m_max_x = 0;
	m_max_y = 0;
	m_map_max_size = 0;
	m_grid_tiles.clear();

	m_max_sector_x = 0;
	m_max_sector_y = 0;

	m_nine_map.clear();
	for (uint32_t i = 0; i < _DIR_MAX; ++i)
	{
		m_direct_screen[i].clear();
		m_reversedirect_screen[i].clear();
	}

	//m_scene_id_list.clear();

	m_born_point = 0;
	m_revive_point = 0;
	m_city_id = 0;

	m_area_map.clear();
	m_monster_vec.clear();
	m_collect_vec.clear();
	m_king_war_vec.clear();
	m_config_born_list.clear();
}

uint32_t map_t::convert_xy_2_map_key(float x, float y)
{
	uint32_t index_x = x / SCREEN_WIDTH;
	uint32_t index_y = y / SCREEN_HEIGHT;

	if (index_x >= m_max_sector_x || index_y >= m_max_sector_y)
	{
		return INVALID_INDEX;
	}

	return (index_y * m_max_sector_x) + index_x;
}

uint32_t map_t::get_born_point(const uint32_t level)
{
	for (auto& it : m_config_born_list)
	{
		if (level <= it.first)
		{
			return it.second;
		}
	}
	log_error("MapTable scene_born config error level[%d] find fail !!!",level);
	return 0;
}

map_area_ptr map_t::get_map_area_info(uint32_t id)
{
	map_area_map::iterator it = m_area_map.find(id);
	if (it != m_area_map.end())
		return it->second;

	return map_area_ptr();
}

const nine_vector& map_t::get_nine_keys(uint32_t index_key)
{
	nine_map::const_iterator it = m_nine_map.find(index_key);
	if (it != m_nine_map.end())
	{
		return it->second;
	}
	//如果出现异常返回0编号的屏索引
	log_error("map[%u] can not find key[%u]", m_map_id, index_key);
	return CONST_NINE_VECTOR;
}

// 一定保证dir 不是_DIR_NOT_CROSS
const nine_vector& map_t::get_direct_screen(uint32_t index_key, int32_t dir)
{
	nine_map::const_iterator it = m_direct_screen[dir].find(index_key);
	if (it != m_direct_screen[dir].end())
	{
		return it->second;
	}

	//如果出现异常返回0编号的屏索引
	log_error("map[%u] can not find direct_screen[%u]", m_map_id, index_key);
	return CONST_NINE_VECTOR;
}

// 一定保证dir 不是_DIR_NOT_CROSS
const nine_vector& map_t::get_reversedirect_screen(uint32_t index_key, int32_t dir)
{
	nine_map::const_iterator it = m_reversedirect_screen[dir].find(index_key);
	if (it != m_reversedirect_screen[dir].end())
	{
		return it->second;
	}

	//如果出现异常返回0编号的屏索引
	log_error("map[%u] can not find reversedirect_screen[%u]", m_map_id, index_key);
	return CONST_NINE_VECTOR;
}

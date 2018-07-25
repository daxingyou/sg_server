#ifndef __GAME_SERVER_FWD_HPP__
#define __GAME_SERVER_FWD_HPP__

#include "log.hpp"
#include "common/common_struct.hpp"
#include <boost/asio.hpp>

class game_server_t;

struct env
{
    static game_server_t* server;
    static common::game_xml_t* xml;
};

#define EQUIP_LOG(format, ...) if(env::xml->log_debug.equip){log_debug(format, ##__VA_ARGS__);}
#define CULTI_LOG(format, ...) if(env::xml->log_debug.hero){log_debug(format, ##__VA_ARGS__);}
#define TRADE_LOG(format, ...) if(env::xml->log_debug.trade){log_debug(format, ##__VA_ARGS__);}
#define TASK_LOG(format, ...) if(env::xml->log_debug.task){log_debug(format, ##__VA_ARGS__);}
#define SCENE_LOG(format, ...) if(env::xml->log_debug.scene){log_debug(format, ##__VA_ARGS__);}
#define ITEM_LOG(format, ...) if(env::xml->log_debug.item){log_debug(format, ##__VA_ARGS__);}
#define BOSS_LOG(format, ...) if(env::xml->log_debug.boss){log_debug(format, ##__VA_ARGS__);}
#define FIGHT_LOG(format, ...) if(env::xml->log_debug.fight){log_combat(format, ##__VA_ARGS__);}
#define TREASURE_LOG(format, ...) if(env::xml->log_debug.treasure){log_debug(format, ##__VA_ARGS__);}
#define PK_LOG(format, ...) if(env::xml->log_debug.pk){log_debug(format, ##__VA_ARGS__);}
#define ACHIEVE_LOG(format, ...) if(env::xml->log_debug.achieve){log_debug(format, ##__VA_ARGS__);}
#define RED_POINT_LOG(format, ...) if(env::xml->log_debug.red_point){log_debug(format, ##__VA_ARGS__);}
#define LIFEFORCE_LOG(format, ...) if(env::xml->log_debug.lifeforce){log_debug(format, ##__VA_ARGS__);}
#define NEW_GUIDE_LOG(format, ...) if(env::xml->log_debug.new_guide){log_debug(format, ##__VA_ARGS__);}
#define ROLE_BUFF_LOG(format, ...) if(env::xml->log_debug.role_buff){log_debug(format, ##__VA_ARGS__);}
#define ROLE_LOG(format, ...) if(env::xml->log_debug.role){log_debug(format, ##__VA_ARGS__);}
#define HERO_LOG(format, ...) if(env::xml->log_debug.hero){log_debug(format, ##__VA_ARGS__);}
#define DUNGEON_LOG(format, ...) if(env::xml->log_debug.dungeon){log_debug(format, ##__VA_ARGS__);}
#define KINGWAR_LOG(format, ...) if(env::xml->log_debug.kingwar){log_debug(format, ##__VA_ARGS__);}
#define SHOP_LOG(format, ...) if(env::xml->log_debug.shop){log_debug(format, ##__VA_ARGS__);}
#define MAIL_LOG(format, ...) if(env::xml->log_debug.mail){log_debug(format, ##__VA_ARGS__);}
#define GOODS_LOG(format, ...) if(env::xml->log_debug.goods){log_debug(format, ##__VA_ARGS__);}
#define FAMILY_LOG(format, ...) if(env::xml->log_debug.family){log_debug(format, ##__VA_ARGS__);}
#define ACTIVE_REWARD_LOG(format, ...) if(env::xml->log_debug.active_reward){log_debug(format, ##__VA_ARGS__);}
#define PAGE_LOG(format, ...) if(env::xml->log_debug.page){log_debug(format, ##__VA_ARGS__);}
#define EXPE_LOG(format, ...) if(env::xml->log_debug.expedition){log_debug(format, ##__VA_ARGS__);}
#define TROOP_LOG(format, ...) if(env::xml->log_debug.troop){log_debug(format, ##__VA_ARGS__);}
#define MONEY_LOG(format, ...) if(env::xml->log_debug.money){log_debug(format, ##__VA_ARGS__);}
#define ARENA_LOG(format, ...) if(env::xml->log_debug.arena){log_debug(format, ##__VA_ARGS__);}
#define TOWER_LOG(format, ...) if(env::xml->log_debug.tower){log_debug(format, ##__VA_ARGS__);}
#define MULTI_LOG(format, ...) if(env::xml->log_debug.multi){log_debug(format, ##__VA_ARGS__);}
#define LIANDAN_LOG(format, ...) if(env::xml->log_debug.liandan){log_debug(format, ##__VA_ARGS__);}
#define PATROL_HIRE_LOG(format, ...) if(env::xml->log_debug.patrol_hire){log_debug(format, ##__VA_ARGS__);}
#define ACTIVITY_LOG(format, ...) if(env::xml->log_debug.activity){log_warn(format, ##__VA_ARGS__);}
#define FRIEND_LOG(format, ...) if(env::xml->log_debug.friends){log_debug(format, ##__VA_ARGS__);}
#endif

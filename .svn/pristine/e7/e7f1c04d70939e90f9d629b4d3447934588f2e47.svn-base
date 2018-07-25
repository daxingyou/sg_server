#ifndef __CENTER_SERVER_FWD_HPP__
#define __CENTER_SERVER_FWD_HPP__

#include "log.hpp"
#include <boost/asio.hpp>
#include "common/common_fwd.hpp"

class center_server_t;

struct env
{
    static center_server_t* server;
	static common::center_xml_t* xml;
	// static struct ConnectionPool_S *conn_zdb_pool;
};

#define PK_LOG(format, ...) if(env::xml->log_debug.pk){log_debug(format, ##__VA_ARGS__);}
#define RANK_LOG(format, ...) if(env::xml->log_debug.rank){log_debug(format, ##__VA_ARGS__);}
#define ROLE_LOG(format, ...) if(env::xml->log_debug.role){log_debug(format, ##__VA_ARGS__);}
#define BOSS_LOG(format, ...) if(env::xml->log_debug.boss){log_debug(format, ##__VA_ARGS__);}
#define FRIEND_LOG(format, ...) if(env::xml->log_debug.friends){log_debug(format, ##__VA_ARGS__);}
#define KINGWAR_LOG(format, ...) if(env::xml->log_debug.kingwar){log_debug(format, ##__VA_ARGS__);}
#define MAIL_LOG(format, ...) if(env::xml->log_debug.mail){log_debug(format, ##__VA_ARGS__);}
#define SCENE_LOG(format, ...) if(env::xml->log_debug.scene){log_debug(format, ##__VA_ARGS__);}
#define SHOP_LOG(format, ...) if(env::xml->log_debug.shop){log_debug(format, ##__VA_ARGS__);}
#define GOODS_LOG(format, ...) if(env::xml->log_debug.goods){log_debug(format, ##__VA_ARGS__);}
#define FAMILY_LOG(format, ...) if(env::xml->log_debug.family){log_debug(format, ##__VA_ARGS__);}
#define TROOP_LOG(format, ...) if(env::xml->log_debug.troop){log_debug(format, ##__VA_ARGS__);}
#define EXPE_LOG(format, ...) if(env::xml->log_debug.expedition){log_debug(format, ##__VA_ARGS__);}
#define ITEM_LOG(format, ...) if(env::xml->log_debug.item){log_debug(format, ##__VA_ARGS__);}
#define REDBAG_LOG(format, ...) if(env::xml->log_debug.redbag){log_debug(format, ##__VA_ARGS__);}
#define OFFLINE_ARENA_LOG(format, ...) if(env::xml->log_debug.offline_arena){log_debug(format, ##__VA_ARGS__);}
#define COUNTRY_LOG(format, ...) if(env::xml->log_debug.country){log_debug(format, ##__VA_ARGS__);}
#endif


AUTOMAKE_OPTIONS = subdir-objects
INCLUDES=   -I/usr/local/include/zdb \
            -I$(top_srcdir)/common \
            -I$(top_srcdir)/network \
            -I$(top_srcdir)/protocpp \
            -I$(top_srcdir)/db_server

LDFLAGS  = -L/usr/local/lib

CXXFLAGS = -std=c++11
if IS_BUILD_DEBUG
    CXXFLAGS += -D_DEBUG -O0
    CXXFLAGS += -ggdb -gdwarf-2
    CXXFLAGS += -Werror
    CXXFLAGS += -Wall -Wextra -Wunreachable-code -Wsign-compare -Wfloat-equal -Wcast-align -Winline -Wcast-qual -Wundef -Wredundant-decls
    CXXFLAGS += -Wno-reorder -Wno-unused-parameter -Wno-ignored-qualifiers
    CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif
if IS_BUILD_VALGRIND
    CXXFLAGS += -D_DEBUG -O0
    CXXFLAGS += -ggdb -gdwarf-2
    CXXFLAGS += -Werror
    CXXFLAGS += -Wall -Wextra -Wunreachable-code -Wsign-compare -Wfloat-equal -Wcast-align -Winline -Wcast-qual -Wundef -Wredundant-decls
    CXXFLAGS += -Wno-reorder -Wno-unused-parameter -Wno-ignored-qualifiers
endif
if IS_BUILD_GPROF
    CXXFLAGS += -D_DEBUG -O0
    CXXFLAGS += -ggdb -gdwarf-2
    CXXFLAGS += -Werror
    CXXFLAGS += -Wall -Wextra -Wunreachable-code -Wsign-compare -Wfloat-equal -Wcast-align -Winline -Wcast-qual -Wundef -Wredundant-decls
    CXXFLAGS += -Wno-reorder -Wno-unused-parameter -Wno-ignored-qualifiers
    CXXFLAGS += -pg
endif
if IS_BUILD_RELEASE
    CXXFLAGS += -DNDEBUG -O2
    CXXFLAGS += -ggdb -gdwarf-2
endif
if IS_BUILD_ONLINE                  
    CXXFLAGS += -DNDEBUG -O2
    CXXFLAGS += -s
endif

bin_PROGRAMS = db_server

db_server_LDADD =   $(top_srcdir)/common/libcommon.a \
                    $(top_srcdir)/network/libnetwork.a \
                    $(top_srcdir)/protocpp/libproto.a \
                    -lredis \
                    -lprotobuf \
                    -lzdb \
                    -lboost_system \
                    -lboost_thread \
                    -lboost_regex \
					-lboost_locale
                    
db_server_SOURCES = center/center_msg_handler.cpp \
                    center/center_listener.cpp \
                    account/account_manager.cpp \
                    account/account_msg_handler.cpp \
                    account/account_connector.cpp \
                    main/main.cpp \
                    main/db_server.cpp \
                    game/game_manager.cpp \
                    game/game_listener.cpp \
                    game/game_msg_handler.cpp \
                    redis/item_data_manager.cpp \
                    redis/task_data_manager.cpp \
                    redis/role_data_manager.cpp \
                    redis/hero_data_manager.cpp \
                    redis/redis_data_manager.cpp \
                    redis/trade_item_data_manager.cpp \
                    redis/mount_data_manager.cpp \
                    redis/country_bounty_data_manager.cpp \
                    redis/activity_data_manager.cpp \
                    redis/arena_data_manager.cpp \
                    redis/achieve_data_manager.cpp \
                    redis/tower_data_manager.cpp \
                    redis/luckydraw_data_manager.cpp \
                    redis/dungeon_data_manager.cpp \
					redis/expedition_data_manager.cpp \
					redis/active_reward_data_manager.cpp \
					redis/random_shop_manager.cpp \
					redis/level_reward_manager.cpp \
                    config/config_manager.cpp \
					redis/level_reward_mail_manager.cpp \
					redis/business_act_data_manager.cpp \
					redis/liandan_data_manager.cpp \
					redis/family_skill_data_manager.cpp \
					redis/patrol_hire_data_manager.cpp \
					redis/equip_plan_data_manager.cpp \
					redis/general_event_data_manager.cpp\
					redis/recharge_data_manager.cpp\
					redis/office_data_manager.cpp\
					redis/redis_db.cpp 
                    
db_serverdir=$(prefix)/conf/

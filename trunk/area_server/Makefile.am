AUTOMAKE_OPTIONS = subdir-objects
INCLUDES =  -I/usr/local/include/zdb \
            -I$(top_srcdir)/common \
            -I$(top_srcdir)/network \
            -I$(top_srcdir)/protocpp \
            -I$(top_srcdir)/area_server

LDFLAGS = -L/usr/local/lib

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

bin_PROGRAMS = area_server

area_server_LDADD = $(top_srcdir)/common/libcommon.a \
                    $(top_srcdir)/network/libnetwork.a \
                    $(top_srcdir)/protocpp/libproto.a \
                    -lredis \
                    -lprotobuf \
                    -lzdb \
                    -lboost_system \
                    -lboost_thread \
                    -lboost_locale

area_server_SOURCES =   main/main.cpp \
                        main/area_server.cpp \
                        config/config_manager.cpp \
                        arena/arena_score_rank_mgr.cpp \
                        transfer/transfer_listener.cpp \
                        transfer/transfer_manager.cpp \
                        transfer/transfer.cpp \
                        transfer/transfer_msg_handler.cpp \
                        cross/cross_listener.cpp \
                        cross/cross_manager.cpp \
                        cross/cross_msg_handler.cpp \
                        test/cross_fight_test.cpp \
                        arena/arena_manager.cpp \
                        role/arena_role.cpp \
                        role/arena_role_manager.cpp \
                        arena/arena_ai.cpp \
                        world_cup/world_cup_battle.cpp \
                        world_cup/world_cup_manager.cpp \
                        world_cup/world_cup_bet.cpp \
						hero_strategy/hero_strategy_manager.cpp \
						redis/redis_area.cpp 

area_serverdir=$(prefix)/conf/

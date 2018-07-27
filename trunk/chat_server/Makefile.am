
AUTOMAKE_OPTIONS = subdir-objects
INCLUDES =   -I/usr/local/include/zdb \
            -I$(top_srcdir)/common \
            -I$(top_srcdir)/network \
            -I$(top_srcdir)/protocpp \
            -I$(top_srcdir)/chat_server
            
LDFLAGS = -L/usr/local/lib 

CXXFLAGS = -std=c++11 -Wno-error=unused-function
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

bin_PROGRAMS = chat_server

chat_server_LDADD = $(top_srcdir)/common/libcommon.a \
                    $(top_srcdir)/network/libnetwork.a \
                    $(top_srcdir)/protocpp/libproto.a \
                    -lprotobuf \
                    -lboost_system \
                    -lboost_thread

chat_server_SOURCES =   main/main.cpp \
                        main/chat_server.cpp \
                        game/game_listener.cpp \
                        game/game_msg_handler.cpp \
                        game/game_manager.cpp \
                        game/send_game_msg.cpp \
                        gate/gate_listener.cpp \
                        gate/gate_manager.cpp \
                        gate/gate_msg_handler.cpp \
                        transfer/transfer_connector.cpp \
                        transfer/transfer_msg_handler.cpp \
                        transfer/transfer_manager.cpp \
                        user/user_manager.cpp \
                        local_time/local_time_manager.cpp \
                        family/family.cpp \
                        family/family_manager.cpp \
                        country/country.cpp \
                        country/country_manager.cpp \
                        center/center_connector.cpp \
                        center/center_msg_handler.cpp \
                        config/config_manager.cpp \
						hero_strategy/hero_strategy_manager.cpp \
						video/video_connector.cpp

chat_serverdir=$(prefix)/conf/

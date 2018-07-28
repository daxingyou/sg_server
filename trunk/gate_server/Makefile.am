
AUTOMAKE_OPTIONS = subdir-objects
INCLUDES =  -I$(top_srcdir)/common \
            -I$(top_srcdir)/network \
            -I$(top_srcdir)/protocpp \
            -I$(top_srcdir)/gate_server \
            -I$(top_srcdir)/tblh

LDFLAGS =   -L/usr/local/lib

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

bin_PROGRAMS = gate_server

gate_server_LDADD = $(top_srcdir)/common/libcommon.a \
                    $(top_srcdir)/network/libnetwork.a \
                    $(top_srcdir)/protocpp/libproto.a \
                    -lprotobuf \
                    -lboost_system \
                    -lboost_thread \
					-lz \
					-lstdc++

gate_server_SOURCES =   main/main.cpp \
                        main/gate_server.cpp \
                        user/user.cpp \
                        user/user_listener.cpp \
                        user/user_manager.cpp \
                        user/user_msg_handler.cpp \
                        user/user_msg_checker.cpp \
                        login/login_connector.cpp \
                        login/login_msg_handler.cpp \
                        game/game_manager.cpp \
                        game/game_listener.cpp \
                        game/cross_connector.cpp \
                        game/game_msg_handler.cpp \
                        chat/chat_connector.cpp \
                        chat/chat_msg_handler.cpp \
                        center/center_connector.cpp \
                        center/center_msg_handler.cpp \
                        config/config_manager.cpp

gate_serverdir=$(prefix)/conf/

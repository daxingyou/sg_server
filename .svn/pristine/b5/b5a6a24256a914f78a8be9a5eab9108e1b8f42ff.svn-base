INCLUDES =  -I/usr/local/include/lua \
            -I/usr/local/include/tolua++ \
            -I$(top_srcdir)/common \
            -I$(top_srcdir)/network \
            -I$(top_srcdir)/protocpp \
            -I$(top_srcdir)/robot_server


LDFLAGS = -L/usr/local/lib -ldl

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

bin_PROGRAMS = robot_server

robot_server_LDADD =    $(top_srcdir)/common/libcommon.a \
                        $(top_srcdir)/network/libnetwork.a \
                        $(top_srcdir)/protocpp/libproto.a \
                        -llua \
                        -ltolua++ \
                        -lluabit \
                        -lprotobuf \
                        -lboost_system \
                        -lboost_thread \
                        -lboost_locale

robot_server_SOURCES =  main/main.cpp \
                        main/robot_server.cpp \
                        robot/robot.cpp \
                        robot/robot_manager.cpp \
                        robot/robot_msg_handler.cpp \
                        robot/name_manager.cpp \
                        robot_common/sTime.cpp \
                        robot_common/sMisc.cpp \
                        config/config_manager.cpp \
                        script/ToScript.cpp \
                        script/script_robot.cpp \
                        script/game_script_func.cpp
                        
robot_serverdir=$(prefix)/conf/

TOLUA = /usr/local/bin/tolua++

script/ToScript.cpp:script/ToScript.pkg @ROBOT_TOLUA_DEPEND@
	$(TOLUA) -o script/ToScript.cpp script/ToScript.pkg
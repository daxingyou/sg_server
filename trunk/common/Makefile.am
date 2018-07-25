
AUTOMAKE_OPTIONS = subdir-objects
INCLUDES=   -I/usr/local/include/zdb \
            -I/usr/local/include/lua \
            -I/usr/local/include/tolua++ \
            -I$(top_srcdir)/common

CXXFLAGS = -std=c++11 -Wno-error=format-security
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

noinst_LIBRARIES = libcommon.a

libcommon_a_LIBFLAGS = -L/usr/local/lib

libcommon_a_SOURCES =   log.cpp \
                        server_base.cpp \
                        assert.cpp \
                        exception.cpp \
                        random_uuid.cpp \
                        md5.cpp \
                        csv.cpp \
                        global_id.cpp \
                        http_request_parser.cpp \
                        config_mgr.cpp \
                        xls_reader/data_exception.cpp \
                        LuaState.cpp \
                        ScriptProxy.cpp \
                        ScriptManager.cpp \
                        redis.cpp \
						redis_unit.cpp \
						redis_srv.cpp \
						redis_cli.cpp \
						redis_client.cpp \
						rank.cpp \
                        timer_common.cpp \
                        time_manager.cpp \
                        DataManager.cpp \
                        Entry.cpp \
                        TblxFileHead.cpp \
                        func_time.cpp \
                        StringTool.cpp \
                        utility.cpp

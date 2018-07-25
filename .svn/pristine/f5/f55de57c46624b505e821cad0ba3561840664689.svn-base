INCLUDES = -I/usr/local/include/zdb \
           -I$(top_srcdir)/common \
           -I$(top_srcdir)/network \
           -I$(top_srcdir)/protocpp \
           -I$(top_srcdir)/log_server
           
LDFLAGS =   -L/usr/local/lib

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

bin_PROGRAMS = log_server

log_server_LDADD =      $(top_srcdir)/common/libcommon.a \
                        $(top_srcdir)/network/libnetwork.a \
                        $(top_srcdir)/protocpp/libproto.a \
                        -lprotobuf \
                        -lzdb \
                        -lboost_system \
                        -lboost_thread
                        
log_server_SOURCES =    main/main.cpp \
                        main/log_server.cpp \
                        main/log_data.cpp \
                        main/log_manager.cpp \
                        main/source_type_converter.cpp \
                        game/game_listener.cpp \
                        game/game_msg_handler.cpp \
                        center/center_listener.cpp \
                        center/center_msg_handler.cpp \
                        account/account_listener.cpp \
                        account/account_msg_handler.cpp

log_serverdir=$(prefix)/conf/

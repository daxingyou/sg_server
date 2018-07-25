INCLUDES =  -I/usr/local/include/zdb \
            -I$(top_srcdir)/common \
            -I$(top_srcdir)/network \
            -I$(top_srcdir)/protocpp \
            -I$(top_srcdir)/tblh \
            -I$(top_srcdir)/login_server
                       
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

bin_PROGRAMS = login_server

login_server_LDADD =    $(top_srcdir)/common/libcommon.a \
                        $(top_srcdir)/network/libnetwork.a \
                        $(top_srcdir)/protocpp/libproto.a \
                        -lredis \
                        -lprotobuf \
                        -lzdb \
                        -lboost_system \
                        -lboost_thread

login_server_SOURCES =  main/main.cpp \
                        main/login_server.cpp \
                        user/user_listener.cpp \
                        user/user_msg_handler.cpp \
                        user/user.cpp \
                        user/user_manager.cpp \
                        gate/gate_listener.cpp \
                        gate/gate_msg_handler.cpp \
                        gate/gate_manager.cpp \
                        account/account_connector.cpp

login_serverdir=$(prefix)/conf/

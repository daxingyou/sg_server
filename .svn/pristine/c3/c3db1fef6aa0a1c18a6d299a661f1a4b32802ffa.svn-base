INCLUDES =  -I/usr/local/include/zdb \
            -I$(top_srcdir)/common \
            -I$(top_srcdir)/network \
            -I$(top_srcdir)/protocpp \
            -I$(top_srcdir)/account_server
           
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

bin_PROGRAMS = account_server

account_server_LDADD =  $(top_srcdir)/common/libcommon.a \
                        $(top_srcdir)/network/libnetwork.a \
                        $(top_srcdir)/protocpp/libproto.a \
                        -lprotobuf \
                        -lzdb \
                        -lcurl \
                        -lboost_system \
                        -lboost_thread \
						-lboost_regex

account_server_SOURCES =    main/main.cpp \
                            main/account_server.cpp \
                            login/login.cpp \
                            login/login_listener.cpp \
                            login/login_manager.cpp \
                            login/login_msg_handler.cpp \
                            db/db_listener.cpp \
                            db/db_msg_handler.cpp \
                            log/log_connector.cpp \
                            log/log_unify_save.cpp \
                            log/log_wrapper.cpp \
                            channel/channel.cpp \
                            channel/channel_manager.cpp \
                            channel/curl_wrapper.cpp \
                            user/user_listener.cpp \
                            user/user_manager.cpp \
                            user/user_msg_handler.cpp\
							serverlist/server_info.cpp\
							serverlist/server_info_manager.cpp\
							serverlist/web_curl_wrapper.cpp
                           
account_serverdir=$(prefix)/conf/

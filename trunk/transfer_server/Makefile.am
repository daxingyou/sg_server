
AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS =   -I$(top_srcdir)/common \
                -I$(top_srcdir)/network \
                -I$(top_srcdir)/protocpp \
                -I$(top_srcdir)/transfer_server\
                -I$(top_srcdir)/config/protobuf_data
            
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

bin_PROGRAMS = transfer_server

transfer_server_LDADD = $(top_srcdir)/common/libcommon.a \
                        $(top_srcdir)/network/libnetwork.a \
                        $(top_srcdir)/protocpp/libproto.a \
                        -lprotobuf \
                        -lboost_system \
                        -lboost_thread
                        
transfer_server_SOURCES =   main/main.cpp \
                            main/transfer_server.cpp \
                            chat/chat_listener.cpp \
                            chat/chat_manager.cpp \
                            chat/chat.cpp \
                            channel/chat_channel.cpp \
                            channel/chat_channel_manager.cpp \
                            chat/chat_msg_handler.cpp \
                            config/config_manager.cpp \
                            area/area_connector.cpp \
                            area/area_msg_handler.cpp \
                            center/center_listener.cpp \
                            center/center_manager.cpp \
                            center/center_msg_handler.cpp
                            
transfer_serverdir=$(prefix)/conf/

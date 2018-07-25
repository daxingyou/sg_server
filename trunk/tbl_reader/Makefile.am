INCLUDES =  -I$(top_srcdir)/common 

LDFLAGS =   -L/usr/local/lib

CXXFLAGS = -std=c++11 -DNDEBUG -O2 -s

bin_PROGRAMS = tbl_reader

tbl_reader_LDADD =  $(top_srcdir)/common/libcommon.a \
                    -lprotobuf \
                    -lboost_system \
                    -lboost_thread
                    
tbl_reader_SOURCES =  main.cpp \
                      tbl_reader.cpp
                      
tbl_readerdir=$(prefix)/conf/
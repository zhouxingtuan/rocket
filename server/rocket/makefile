
LOCAL_LIB_PATH := ./

LEVEL_DB_LIB_PATH := ../../third/leveldb
LEVEL_DB_INC := ../../third/leveldb/include
LEVEL_DB_LIB := $(LEVEL_DB_LIB_PATH)/build/libleveldb.a

CRC32C_LIB_PATH := ../../third/crc32c
CRC32C_INC := ../../third/crc32c/include
CRC32C_LIB := $(CRC32C_LIB_PATH)/build/libcrc32c.a

SNAPPY_LIB_PATH := ../../third/snappy
SNAPPY_LIB := $(SNAPPY_LIB_PATH)/libsnappy.a

LUA_DIR := ../../third/LuaJIT
LUA_INC := $(LUA_DIR)/src
LUA_LIB := $(LUA_DIR)/src/libluajit.a

MYSQL_LIB := ../libmysql/libmysql.a
MYSQL_INC := ../libmysql
MYSQL_LIB_PATH := -L/usr/lib/mysql -L/usr/lib64/mysql

REDIS_LIB := ../libredis/libredis.a
REDIS_INC := ../libredis

HIREDIS_LIB := ../../third/hiredis/libhiredis.a
HIREDIS_INC := ../../third/hiredis

LUARAPIDJSON_LIB := ../../third/luarapidjson/libluarapidjson.a
LUARAPIDJSON_INC := ../../third/luarapidjson

TOLUA_LIB := ../tolua/libtolua.a
TOLUA_INC := ../tolua

LIB_LUA_LIB := ../liblua/liblua.a
LIB_LUA_INC := ../liblua

NET_LIB_PATH := ../libnet
NET_LIB := $(NET_LIB_PATH)/libnet.a

COMMON_LIB_PATH := ../libcommon
COMMON_LIB := $(COMMON_LIB_PATH)/libcommon.a

INC_PATH = -I./ -I$(COMMON_LIB_PATH) -I$(NET_LIB_PATH) -I$(LIB_LUA_INC) -I$(LUA_INC) -I$(CRC32C_INC) -I$(SNAPPY_LIB_PATH) -I$(LEVEL_DB_INC)

# static的依赖库要放在static的后面，比如luasocket依赖openssl
LIB_PATH = -L$(LOCAL_LIB_PATH) -L$(COMMON_LIB_PATH) -L$(NET_LIB_PATH) -L$(MYSQL_LIB_PATH) -L$(SNAPPY_LIB) -L$(CRC32C_LIB_PATH) -L/usr/local/lib
LIBS = -lmysqlclient -lssl -lcrypto -lrt -lstdc++ -lm -ldl -Wl,-rpath=./lib

# 静态库的引用需要注意顺序，最底层的静态库放在后面
STATIC_LIB := $(LIB_LUA_LIB) $(MYSQL_LIB) $(REDIS_LIB) $(HIREDIS_LIB) $(LUARAPIDJSON_LIB) $(TOLUA_LIB) $(LUA_LIB) $(LEVEL_DB_LIB) $(SNAPPY_LIB) $(CRC32C_LIB) $(NET_LIB) $(COMMON_LIB)

# 注意，这里的 -ldl 需要放在flag的最后才能生效
CXXFLAGS = -O2 -Wall -fPIC -Wstrict-overflow=3 -pthread -std=c++11  $(INC_PATH) $(LIB_PATH) $(LIBS)
CFLAGS = -O2 -Wall -fPIC -Wstrict-overflow=3 -pthread $(INC_PATH) $(LIB_PATH) $(LIBS)
CC = gcc
AR = ar
ARFLAGS = cr
DEBUG= -g -ggdb
RM = rm -f
BIN = ./
TARGET = rocket

OBJS = main.o rocket_handler.o main_handler.o
COBJS =

all: $(COBJS) $(OBJS) $(TARGET)

$(OBJS): %.o:%.cpp
	$(CC) $(DEBUG) -c $< -o $@ $(CXXFLAGS)

$(COBJS): %.o:%.c
	$(CC) $(DEBUG) -c $< -o $@ $(CFLAGS)

$(TARGET): $(OBJS) $(COBJS)
	$(CC) $(DEBUG) $(OBJS) $(COBJS) $(STATIC_LIB) $(LIB_PATH) $(LIBS) -o $(TARGET) $(CXXFLAGS)

clean:
	-$(RM) $(TARGET)
	-$(RM) *.so
	-$(RM) *.o
	-$(RM) *.log







LIB_NAME := liblua.a
DLL_NAME := liblua.so

LEVEL_DB_INC := ../../third/leveldb/include

CRC32C_INC := ../../third/crc32c/include

SNAPPY_LIB_PATH := ../../third/snappy

LUA_DIR := ../../third/LuaJIT
LUA_INC := $(LUA_DIR)/src

MYSQL_INC := ../libmysql
MYSQL_LIB_PATH := -L/usr/lib/mysql -L/usr/lib64/mysql

REDIS_INC := ../libredis

LUARAPIDJSON_INC := ../../third/luarapidjson

TOLUA_INC := ../tolua

COMMON_LIB_PATH := ../libcommon/

NET_LIB_PATH := ../libnet/

INC_PATH := -I./ -I$(LUA_DIR) -I$(TOLUA_INC) -I$(COMMON_LIB_PATH) -I$(NET_LIB_PATH) -I$(MYSQL_INC) -I$(REDIS_INC) -I$(LUARAPIDJSON_INC) -I$(LEVEL_DB_INC) -I$(CRC32C_INC) -I$(SNAPPY_LIB_PATH)
#LIB_PATH := -L$(COMMON_LIB_PATH)
LIB_PATH :=
LIBS = -lssl -lcrypto -lrt -lstdc++ -ldl -Wl,-rpath=./lib

# 静态库的引用需要注意顺序，最底层的静态库放在后面
STATIC_LIB :=

# 注意，这里的 -ldl 需要放在flag的最后才能生效
CXXFLAGS = -O2 -Wall -fPIC -pthread -std=c++11 $(INC_PATH) $(LIB_PATH) $(LIBS)
CFLAGS = -O2 -Wall -fPIC -pthread -std=gnu99 $(INC_PATH) $(LIB_PATH) $(LIBS)
CC = gcc
AR = ar
ARFLAGS = cr
DEBUG= -g -ggdb

OBJS = level.o script.o eproto.o md5.o liblua.o

COBJS = mime.o

all: $(COBJS) $(OBJS) $(LIB_NAME) $(DLL_NAME)

$(OBJS): %.o:%.cpp
	$(CC) $(DEBUG) -c $< -o $@ $(CXXFLAGS)

$(COBJS): %.o:%.c
	$(CC) $(DEBUG) -c $< -o $@ $(CFLAGS)

liblua.o:liblua.cpp liblua.h manual.hpp

$(LIB_NAME):$(OBJS) $(COBJS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(OBJS) $(COBJS)

$(DLL_NAME):$(OBJS) $(COBJS)
	$(CC) $(DEBUG) $(OBJS) $(COBJS) $(STATIC_LIB) $(CXXFLAGS) -shared -o $(DLL_NAME)

clean:
	-$(RM) $(LIB_NAME)
	-$(RM) $(DLL_NAME)
	-$(RM) *.o











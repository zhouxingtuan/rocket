
LUA_DIR := ../../third/LuaJIT
LUA_INC := $(LUA_DIR)/src

TOLUA_INC := ../tolua

LIB_NAME := libmysql.a
DLL_NAME := libmysql.so

INC_PATH = -I./ -I/usr/include/mysql -I$(LUA_INC) -I$(TOLUA_INC)

# static的依赖库要放在static的后面，比如luasocket依赖openssl
LIB_PATH = -L/usr/lib/mysql -L/usr/lib64/mysql -L/usr/local/lib
LIBS = -lmysqlclient -lstdc++ -ldl -Wl,-rpath=./lib

# 静态库的引用需要注意顺序，最底层的静态库放在后面
STATIC_LIB :=

# 注意，这里的 -ldl 需要放在flag的最后才能生效
CXXFLAGS = -O2 -Wall -fPIC -Wstrict-overflow=3 -pthread -std=c++11 $(INC_PATH) $(LIB_PATH) $(LIBS)
CFLAGS = -O2 -Wall -fPIC -Wstrict-overflow=3 -pthread $(INC_PATH) $(LIB_PATH) $(LIBS)
CC = gcc
AR = ar
ARFLAGS = cr
DEBUG= -g -ggdb

OBJS = dbmysql.o libmysql.o automysql.o

COBJS =

all: $(COBJS) $(OBJS) $(DLL_NAME) $(LIB_NAME)

$(OBJS): %.o:%.cpp
	$(CC) $(DEBUG) -c $< -o $@ $(CXXFLAGS)

$(COBJS): %.o:%.c
	$(CC) $(DEBUG) -c $< -o $@ $(CFLAGS)

$(LIB_NAME):$(OBJS) $(COBJS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(OBJS) $(COBJS)

$(DLL_NAME):$(OBJS) $(COBJS)
	$(CC) $(DEBUG) $(OBJS) $(COBJS) $(STATIC_LIB) $(CXXFLAGS) -shared -o $(DLL_NAME)

dbmysql.o:dbmysql.cpp dbmysql.h

libmysql.o:libmysql.cpp libmysql.h

automysql.o:automysql.cpp automysql.h manual.hpp

clean:
	-$(RM) $(LIB_NAME)
	-$(RM) $(DLL_NAME)
	-$(RM) *.so
	-$(RM) *.o



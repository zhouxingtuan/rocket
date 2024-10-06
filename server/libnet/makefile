
LIB_NAME := libnet.a
DLL_NAME := libnet.so

COMMON_LIB_PATH := ../libcommon/

INC_PATH := -I./ -I$(COMMON_LIB_PATH)
#LIB_PATH := -L$(COMMON_LIB_PATH)
LIB_PATH :=
LIBS = -lssl -lcrypto -lrt -lstdc++ -ldl -Wl,-rpath=./lib

# 注意，这里的 -ldl 需要放在flag的最后才能生效
CXXFLAGS = -O2 -Wall -fPIC -pthread -std=c++11 $(INC_PATH) $(LIB_PATH) $(LIBS)
CFLAGS = -O2 -Wall -fPIC -pthread -std=gnu99 $(INC_PATH) $(LIB_PATH) $(LIBS)
CC = gcc
AR = ar
ARFLAGS = cr
DEBUG= -g -ggdb

OBJS = epoll.o active.o listener.o websocket.o \
		http_common.o http.o https.o http_client.o https_client.o manager.o

COBJS = http_parser.o

all: $(COBJS) $(OBJS) $(LIB_NAME) $(DLL_NAME)

$(OBJS): %.o:%.cpp
	$(CC) $(DEBUG) -c $< -o $@ $(CXXFLAGS)

$(COBJS): %.o:%.c
	$(CC) $(DEBUG) -c $< -o $@ $(CFLAGS)

$(LIB_NAME):$(OBJS) $(COBJS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(OBJS) $(COBJS)

$(DLL_NAME):$(OBJS) $(COBJS)
	$(CC) $(DEBUG) $(OBJS) $(COBJS) $(CXXFLAGS) -shared -o $(DLL_NAME)

clean:
	-$(RM) $(LIB_NAME)
	-$(RM) $(DLL_NAME)
	-$(RM) *.o












LIB_NAME := libsnappy.a
DLL_NAME := libsnappy.so

INC_PATH := -I./
LIB_PATH :=
LIBS = -lssl -lcrypto -lrt -lstdc++ -ldl -Wl,-rpath=./lib

# 注意，这里的 -ldl 需要放在flag的最后才能生效
CXXFLAGS = -O2 -Wall -fPIC -pthread -std=c++11 $(INC_PATH) $(LIB_PATH) $(LIBS)
CFLAGS = -O2 -Wall -fPIC -pthread -std=gnu99 $(INC_PATH) $(LIB_PATH) $(LIBS)
CC = gcc
AR = ar
ARFLAGS = cr
DEBUG= -g -ggdb
BIN = ./

OBJS =

CCOBJS = snappy.o snappy-sinksource.o snappy-stubs-internal.o

COBJS =

all: $(COBJS) $(CCOBJS) $(OBJS) $(LIB_NAME) $(DLL_NAME)

$(OBJS): %.o:%.cpp
	$(CC) $(DEBUG) -c $< -o $@ $(CXXFLAGS)

$(CCOBJS): %.o:%.cc
	$(CC) $(DEBUG) -c $< -o $@ $(CXXFLAGS)

$(COBJS): %.o:%.c
	$(CC) $(DEBUG) -c $< -o $@ $(CFLAGS)

$(LIB_NAME):$(OBJS) $(CCOBJS) $(COBJS)
	$(AR) $(ARFLAGS) $(LIB_NAME) $(OBJS) $(CCOBJS) $(COBJS)
#	cp $(LIB_NAME) $(BIN)/$(LIB_NAME)

$(DLL_NAME):$(OBJS) $(COBJS)
	$(CC) $(DEBUG) $(OBJS) $(CCOBJS) $(COBJS) $(CXXFLAGS) -shared -o $(DLL_NAME)
#	$(RM) $(BIN)/$(DLL_NAME)
#	cp $(DLL_NAME) $(BIN)/$(DLL_NAME)

clean:
	-$(RM) $(LIB_NAME)
	-$(RM) $(DLL_NAME)
	-$(RM) *.o











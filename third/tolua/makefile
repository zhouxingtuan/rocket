CFLAGES = -O2 -I./include

LDFLAGES = -L./lib -ltolua++

CC = gcc

OBJSDIR = src

LIB = lib

BIN = bin

OBJS = $(OBJSDIR)/luac.o \
       $(OBJSDIR)/tolua_event.o \
       $(OBJSDIR)/tolua_push.o \
       $(OBJSDIR)/print.o \
       $(OBJSDIR)/lzio.o \
       $(OBJSDIR)/ltm.o \
       $(OBJSDIR)/lstring.o \
       $(OBJSDIR)/lopcodes.o \
       $(OBJSDIR)/lobject.o \
       $(OBJSDIR)/lmem.o \
       $(OBJSDIR)/lmathlib.o \
       $(OBJSDIR)/llex.o \
       $(OBJSDIR)/linit.o \
       $(OBJSDIR)/lgc.o \
       $(OBJSDIR)/ldump.o \
       $(OBJSDIR)/lfunc.o \
       $(OBJSDIR)/lvm.o \
       $(OBJSDIR)/ltable.o \
       $(OBJSDIR)/lparser.o \
       $(OBJSDIR)/lcode.o \
       $(OBJSDIR)/lstate.o \
       $(OBJSDIR)/loslib.o \
       $(OBJSDIR)/liolib.o \
       $(OBJSDIR)/ldo.o \
       $(OBJSDIR)/ldblib.o \
       $(OBJSDIR)/lauxlib.o \
       $(OBJSDIR)/lbaselib.o \
       $(OBJSDIR)/ltablib.o \
       $(OBJSDIR)/lundump.o \
       $(OBJSDIR)/tolua_is.o \
       $(OBJSDIR)/ldebug.o \
       $(OBJSDIR)/lapi.o \
       $(OBJSDIR)/lstrlib.o \
       $(OBJSDIR)/loadlib.o \
       $(OBJSDIR)/tolua_to.o \
       $(OBJSDIR)/tolua_map.o

LUA_O = $(OBJSDIR)/lua/lua.c

TOLUA_O = $(OBJSDIR)/tolua/tolua.c \
          $(OBJSDIR)/tolua/toluabind.c

 

TARGET = libtolua++.so

$(TARGET):$(OBJS)
	$(CC) -shared -o $(LIB)/$@ $^ -lm $(CFLAGES)

$(OBJS):%.o:%.c
	$(CC) $? -c -fPIC -o $@ $(CFLAGES)

 

lua:
	$(CC) -o $(BIN)/$@ $(LUA_O) $(CFLAGES) $(LDFLAGES)

tolua++:
	$(CC) -o $(BIN)/$@ $(TOLUA_O) $(CFLAGES) $(LDFLAGES)

 

install:
	mkdir -p /usr/local/bin /usr/local/include
	cd bin && install -p -m 0755 lua tolua++ /usr/local/bin
	cd include && install -p -m 0644 lua.h luaconf.h lualib.h lauxlib.h lua.hpp tolua++.h /usr/local/include
	cd lib && install -p -m 0644 libtolua++.so /lib
	ldconfig

uninstall:
	cd /usr/local/bin && $(RM) lua tolua++
	cd /usr/local/include && $(RM) lua.h luaconf.h lualib.h lauxlib.h lua.hpp tolua++.h
	cd /lib && $(RM) libtolua++.so

clean:
	$(RM) $(LIB)/$(TARGET) $(OBJS) $(BIN)/*







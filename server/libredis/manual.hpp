
#include "lua.hpp"
#include "tolua++.h"

#include "dbredis.h"

USING_NS_HIVE

#define TOLUA_RELEASE 1

#undef tolua_pushcppstring
#define tolua_pushcppstring(x,y) lua_pushlstring(x,y.c_str(),(size_t)y.length())

/* method: command of class  DBRedis */
static int tolua_liblua_DBRedis_command(lua_State* L)
{
	DBRedis* self = (DBRedis*)  tolua_tousertype(L,1,0);
	bool skipResult = ((bool)  tolua_toboolean(L,2,0));
	return self->command(L, skipResult);
}
static int tolua_liblua_DBRedis_appendCommand(lua_State* L)
{
	DBRedis* self = (DBRedis*)  tolua_tousertype(L,1,0);
	self->appendCommand(L);
	return 0;
}
static int tolua_liblua_DBRedis_getReply(lua_State* L)
{
	DBRedis* self = (DBRedis*)  tolua_tousertype(L,1,0);
	return self->getReply(L);
}
static void extendDBRedis(lua_State* L)
{
    lua_pushstring(L, "DBRedis");
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (lua_istable(L,-1))
    {
        tolua_function(L, "command", tolua_liblua_DBRedis_command);
        tolua_function(L, "appendCommand", tolua_liblua_DBRedis_appendCommand);
        tolua_function(L, "getReply", tolua_liblua_DBRedis_getReply);
    }
    lua_pop(L, 1);
}

static int tolua_libredis_manual_open(lua_State* L)
{
    lua_getglobal(L, "_G");
    if (lua_istable(L,-1))//stack:...,_G,
    {
        extendDBRedis(L);
    }
    lua_pop(L, 1);
    return 1;
}

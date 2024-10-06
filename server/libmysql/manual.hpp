
#include "lua.hpp"
#include "tolua++.h"

#include "dbmysql.h"

USING_NS_HIVE

#define TOLUA_RELEASE 1

#undef tolua_pushcppstring
#define tolua_pushcppstring(x,y) lua_pushlstring(x,y.c_str(),(size_t)y.length())

/* method: execute of class  DBMysql */
static int tolua_liblua_DBMysql_execute(lua_State* L)
{
	size_t length;
	DBMysql* self = (DBMysql*)  tolua_tousertype(L,1,0);
	const char* query = ((const char*)  lua_tolstring(L,2,&length));
	int queryNumber = ((int)  tolua_tonumber(L,3,0));
	bool skipResult = ((bool)  tolua_toboolean(L,4,0));
	return self->execute(L, query, length, queryNumber, skipResult);
}
/* method: escape of class  DBMysql */
static int tolua_liblua_DBMysql_escape(lua_State* L)
{
	size_t length;
	DBMysql* self = (DBMysql*)  tolua_tousertype(L,1,0);
	const char* ptr = ((const char*)  lua_tolstring(L,2,&length));
	return self->escape(L, ptr, length);
}
static void extendDBMysql(lua_State* L)
{
    lua_pushstring(L, "DBMysql");
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (lua_istable(L,-1))
    {
        tolua_function(L, "execute", tolua_liblua_DBMysql_execute);
        tolua_function(L, "escape", tolua_liblua_DBMysql_escape);
    }
    lua_pop(L, 1);
}

static int tolua_libmysql_manual_open(lua_State* L)
{
    lua_getglobal(L, "_G");
    if (lua_istable(L,-1))//stack:...,_G,
    {
        extendDBMysql(L);
    }
    lua_pop(L, 1);
    return 1;
}

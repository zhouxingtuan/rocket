//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 2017/5/14
// Time: 上午11:29
// To change this template use File | Settings | File Templates.
//

#include "lua.hpp"
#include "tolua++.h"

#include "net.h"
#include "md5.h"
#include "level.h"
#include "shared_buffer.h"

USING_NS_HIVE

#define TOLUA_RELEASE 1

#undef tolua_pushcppstring
#define tolua_pushcppstring(x,y) lua_pushlstring(x,y.c_str(),(int64)y.length())

/* method: getBuffer of class  MD5 */
static int tolua_liblua_MD5_getBuffer(lua_State* L)
{
	MD5* self = (MD5*)  tolua_tousertype(L,1,0);
	return self->getBuffer(L);
}
static void extendMD5(lua_State* L)
{
    lua_pushstring(L, "MD5");
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (lua_istable(L,-1))
    {
        tolua_function(L, "getBuffer", tolua_liblua_MD5_getBuffer);
    }
    lua_pop(L, 1);
}
static int tolua_liblua_md5Encode32To16(lua_State* L)
{
    const char* ori_str = ((const char*)tolua_tostring(L,1,0));
	return md5Encode32To16(L, ori_str);
}
static int tolua_liblua_md5Decode16To32(lua_State* L)
{
    const char* ori_str = ((const char*)tolua_tostring(L,1,0));
	return md5Decode16To32(L, ori_str);
}
static void extendMD5Function(lua_State* L)
{
    tolua_function(L, "md5Encode32To16", tolua_liblua_md5Encode32To16);
    tolua_function(L, "md5Decode16To32", tolua_liblua_md5Decode16To32);
}
static int tolua_liblua_db_open(lua_State* L){
    std::string name = ((const char*)  tolua_tostring(L,1,0));
    bool result = (bool)  db_open(name);
    lua_pushboolean(L,(int)result);
    return 1;
}
static int tolua_liblua_db_set(lua_State* L){
    size_t key_len;
    const char *key = lua_tolstring(L, 1, &key_len);
    size_t value_len;
    const char *value = lua_tolstring(L, 2, &value_len);
    bool result = db_set(std::string(key, key_len), std::string(value, value_len));
    lua_pushboolean(L,(int)result);
    return 1;
}
static int tolua_liblua_db_get(lua_State* L){
    size_t key_len;
    const char *key = lua_tolstring(L, 1, &key_len);
    std::string value;
    bool result = db_get(std::string(key, key_len), value);
    lua_pushboolean(L,(int)result);
    lua_pushlstring(L, value.data(), value.length());
    return 2;
}
static int tolua_liblua_db_del(lua_State* L){
    size_t key_len;
    const char *key = lua_tolstring(L, 1, &key_len);
    bool result = db_del(std::string(key, key_len));
    lua_pushboolean(L,(int)result);
    return 1;
}
static int tolua_liblua_db_batch(lua_State* L){
    bool result = db_batch(L);
    lua_pushboolean(L,(int)result);
    return 1;
}
static void extendLevel(lua_State* L)
{
    tolua_function(L, "db_open", tolua_liblua_db_open);
    tolua_function(L, "db_set", tolua_liblua_db_set);
    tolua_function(L, "db_get", tolua_liblua_db_get);
    tolua_function(L, "db_del", tolua_liblua_db_del);
    tolua_function(L, "db_batch", tolua_liblua_db_batch);
}

static int tolua_liblua_sb_create(lua_State* L){
    int slotNumber = ((int)  tolua_tonumber(L,1,0));
    bool result = (bool)  sb_create(slotNumber);
    lua_pushboolean(L,(int)result);
    return 1;
}
static int tolua_liblua_sb_set(lua_State* L){
    size_t key_len;
    const char *key = lua_tolstring(L, 1, &key_len);
    size_t value_len;
    const char *value = lua_tolstring(L, 2, &value_len);
    sb_set(std::string(key, key_len), std::string(value, value_len));
    return 0;
}
static int tolua_liblua_sb_get(lua_State* L){
    size_t key_len;
    const char *key = lua_tolstring(L, 1, &key_len);
    std::string value;
    bool result = sb_get(std::string(key, key_len), value);
    lua_pushboolean(L,(int)result);
    lua_pushlstring(L, value.data(), value.length());
    return 2;
}
static int tolua_liblua_sb_del(lua_State* L){
    size_t key_len;
    const char *key = lua_tolstring(L, 1, &key_len);
    bool result = sb_del(std::string(key, key_len));
    lua_pushboolean(L,(int)result);
    return 1;
}
static bool LuaSharedBufferUpdateFunction(void* data, const std::string& key, std::string& value){
    lua_State* L = (lua_State*)data;
    lua_pushlstring(L, value.data(), value.length());
    if( lua_pcall( L, 1, 1, 0 ) != 0 ){
		const char* msg = lua_tostring( L, -1 );
		if( msg == NULL ){
		    msg = "(error without message)";
		}
		LOG_ERROR("lua error:%s\n", msg);
		return false;
    }
    size_t vlen;
    const char *v = lua_tolstring(L, -1, &vlen);
    value.clear();
    value.append(v, vlen);
//    LOG_DEBUG("stack number=%d value=%s", lua_gettop(L), v);
    return true;
}
static int tolua_liblua_sb_update(lua_State* L){
    size_t klen;
    const char *k = lua_tolstring(L, 1, &klen);
    std::string key(k, klen);
    sb_update(key, LuaSharedBufferUpdateFunction, L);
    return 0;
}
static void extendSharedBuffer(lua_State* L)
{
    tolua_function(L, "sb_create", tolua_liblua_sb_create);
    tolua_function(L, "sb_set", tolua_liblua_sb_set);
    tolua_function(L, "sb_get", tolua_liblua_sb_get);
    tolua_function(L, "sb_del", tolua_liblua_sb_del);
    tolua_function(L, "sb_update", tolua_liblua_sb_update);
}

/* method: readData of class  File */
static int tolua_liblua_File_readData(lua_State* L)
{
	File* self = (File*)  tolua_tousertype(L,1,0);
	int64 offset = ((int64)  tolua_tonumber(L,2,0));
	size_t length = ((size_t)  tolua_tonumber(L,3,0));
	char* pData = new char[length];
	if( self->readData(offset, pData, length) ){
	    lua_pushlstring(L, (const char*)pData, length);
	}else{
	    lua_pushnil(L);
	}
	delete[] pData;
	return 1;
}
/* method: fastReadData of class  File */
static int tolua_liblua_File_fastReadData(lua_State* L)
{
	int64 mapLength = 0;
	int64 mapOffset = 0;
	File* self = (File*)  tolua_tousertype(L,1,0);
	int64 offset = ((int64)  tolua_tonumber(L,2,0));
	int64 length = ((size_t)  tolua_tonumber(L,3,0));
	char* pData = self->fastOpenRead(offset, length, mapLength, mapOffset);
	if(NULL != pData){
        lua_pushlstring(L, (const char*)(pData + (offset - mapOffset)), length);
        self->fastCloseRead(pData, mapLength);
	}else{
	    lua_pushnil(L);
	}
	return 1;
}
static void extendFile(lua_State* L)
{
    lua_pushstring(L, "File");
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (lua_istable(L,-1))
    {
        tolua_function(L, "readData", tolua_liblua_File_readData);
        tolua_function(L, "fastReadData", tolua_liblua_File_fastReadData);
    }
    lua_pop(L, 1);
}

static int tolua_liblua_manual_open(lua_State* L)
{
    lua_getglobal(L, "_G");
    if (lua_istable(L,-1))//stack:...,_G,
    {
        extendMD5(L);
        extendMD5Function(L);
        extendLevel(L);
        extendSharedBuffer(L);
        extendFile(L);
    }
    lua_pop(L, 1);
    return 1;
}


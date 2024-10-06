/*
** Lua binding: autoredis
** Generated automatically by tolua++-1.0.92 on Tue Dec 26 12:48:21 2017.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_autoredis_open (lua_State* tolua_S);

#include "manual.hpp"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_DBRedis (lua_State* tolua_S)
{
 DBRedis* self = (DBRedis*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"DBRedis");
}

/* method: new of class  DBRedis */
#ifndef TOLUA_DISABLE_tolua_autoredis_DBRedis_new00
static int tolua_autoredis_DBRedis_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"DBRedis",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   DBRedis* tolua_ret = (DBRedis*)  Mtolua_new((DBRedis)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"DBRedis");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  DBRedis */
#ifndef TOLUA_DISABLE_tolua_autoredis_DBRedis_new00_local
static int tolua_autoredis_DBRedis_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"DBRedis",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   DBRedis* tolua_ret = (DBRedis*)  Mtolua_new((DBRedis)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"DBRedis");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  DBRedis */
#ifndef TOLUA_DISABLE_tolua_autoredis_DBRedis_delete00
static int tolua_autoredis_DBRedis_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBRedis",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBRedis* self = (DBRedis*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isAvailable of class  DBRedis */
#ifndef TOLUA_DISABLE_tolua_autoredis_DBRedis_isAvailable00
static int tolua_autoredis_DBRedis_isAvailable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBRedis",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBRedis* self = (DBRedis*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isAvailable'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isAvailable();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isAvailable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: connectRedis of class  DBRedis */
#ifndef TOLUA_DISABLE_tolua_autoredis_DBRedis_connectRedis00
static int tolua_autoredis_DBRedis_connectRedis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBRedis",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBRedis* self = (DBRedis*)  tolua_tousertype(tolua_S,1,0);
  const char* ip = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned short port = (( unsigned short)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'connectRedis'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->connectRedis(ip,port);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'connectRedis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: closeRedis of class  DBRedis */
#ifndef TOLUA_DISABLE_tolua_autoredis_DBRedis_closeRedis00
static int tolua_autoredis_DBRedis_closeRedis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBRedis",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBRedis* self = (DBRedis*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'closeRedis'", NULL);
#endif
  {
   self->closeRedis();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'closeRedis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: identifyRedis of class  DBRedis */
#ifndef TOLUA_DISABLE_tolua_autoredis_DBRedis_identifyRedis00
static int tolua_autoredis_DBRedis_identifyRedis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBRedis",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBRedis* self = (DBRedis*)  tolua_tousertype(tolua_S,1,0);
  const char* pwd = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'identifyRedis'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->identifyRedis(pwd);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'identifyRedis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_autoredis_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"DBRedis","DBRedis","",tolua_collect_DBRedis);
  #else
  tolua_cclass(tolua_S,"DBRedis","DBRedis","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"DBRedis");
   tolua_function(tolua_S,"new",tolua_autoredis_DBRedis_new00);
   tolua_function(tolua_S,"new_local",tolua_autoredis_DBRedis_new00_local);
   tolua_function(tolua_S,".call",tolua_autoredis_DBRedis_new00_local);
   tolua_function(tolua_S,"delete",tolua_autoredis_DBRedis_delete00);
   tolua_function(tolua_S,"isAvailable",tolua_autoredis_DBRedis_isAvailable00);
   tolua_function(tolua_S,"connectRedis",tolua_autoredis_DBRedis_connectRedis00);
   tolua_function(tolua_S,"closeRedis",tolua_autoredis_DBRedis_closeRedis00);
   tolua_function(tolua_S,"identifyRedis",tolua_autoredis_DBRedis_identifyRedis00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_autoredis (lua_State* tolua_S) {
 return tolua_autoredis_open(tolua_S);
};
#endif


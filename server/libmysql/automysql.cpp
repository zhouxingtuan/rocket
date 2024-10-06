/*
** Lua binding: automysql
** Generated automatically by tolua++-1.0.92 on Tue Dec 26 14:37:15 2017.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_automysql_open (lua_State* tolua_S);

#include "manual.hpp"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_DBMysql (lua_State* tolua_S)
{
 DBMysql* self = (DBMysql*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"DBMysql");
}

/* method: new of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_new00
static int tolua_automysql_DBMysql_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   DBMysql* tolua_ret = (DBMysql*)  Mtolua_new((DBMysql)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"DBMysql");
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

/* method: new_local of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_new00_local
static int tolua_automysql_DBMysql_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   DBMysql* tolua_ret = (DBMysql*)  Mtolua_new((DBMysql)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"DBMysql");
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

/* method: delete of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_delete00
static int tolua_automysql_DBMysql_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBMysql* self = (DBMysql*)  tolua_tousertype(tolua_S,1,0);
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

/* method: isAvailable of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_isAvailable00
static int tolua_automysql_DBMysql_isAvailable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBMysql* self = (DBMysql*)  tolua_tousertype(tolua_S,1,0);
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

/* method: connectMySql of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_connectMySql00
static int tolua_automysql_DBMysql_connectMySql00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBMysql* self = (DBMysql*)  tolua_tousertype(tolua_S,1,0);
  const char* ip = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* user = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* pwd = ((const char*)  tolua_tostring(tolua_S,4,0));
  const char* db = ((const char*)  tolua_tostring(tolua_S,5,0));
  unsigned short port = (( unsigned short)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'connectMySql'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->connectMySql(ip,user,pwd,db,port);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'connectMySql'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: closeMysql of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_closeMysql00
static int tolua_automysql_DBMysql_closeMysql00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBMysql* self = (DBMysql*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'closeMysql'", NULL);
#endif
  {
   self->closeMysql();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'closeMysql'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: pingMysql of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_pingMysql00
static int tolua_automysql_DBMysql_pingMysql00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBMysql* self = (DBMysql*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'pingMysql'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->pingMysql();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'pingMysql'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLastAutoID of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_getLastAutoID00
static int tolua_automysql_DBMysql_getLastAutoID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBMysql* self = (DBMysql*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLastAutoID'", NULL);
#endif
  {
   long long int tolua_ret = ( long long int)  self->getLastAutoID();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLastAutoID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAutoCommit of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_setAutoCommit00
static int tolua_automysql_DBMysql_setAutoCommit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBMysql* self = (DBMysql*)  tolua_tousertype(tolua_S,1,0);
  bool on = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAutoCommit'", NULL);
#endif
  {
   self->setAutoCommit(on);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAutoCommit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: commit of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_commit00
static int tolua_automysql_DBMysql_commit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBMysql* self = (DBMysql*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'commit'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->commit();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'commit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rollback of class  DBMysql */
#ifndef TOLUA_DISABLE_tolua_automysql_DBMysql_rollback00
static int tolua_automysql_DBMysql_rollback00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DBMysql",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DBMysql* self = (DBMysql*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rollback'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->rollback();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rollback'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_automysql_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"DBMysql","DBMysql","",tolua_collect_DBMysql);
  #else
  tolua_cclass(tolua_S,"DBMysql","DBMysql","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"DBMysql");
   tolua_function(tolua_S,"new",tolua_automysql_DBMysql_new00);
   tolua_function(tolua_S,"new_local",tolua_automysql_DBMysql_new00_local);
   tolua_function(tolua_S,".call",tolua_automysql_DBMysql_new00_local);
   tolua_function(tolua_S,"delete",tolua_automysql_DBMysql_delete00);
   tolua_function(tolua_S,"isAvailable",tolua_automysql_DBMysql_isAvailable00);
   tolua_function(tolua_S,"connectMySql",tolua_automysql_DBMysql_connectMySql00);
   tolua_function(tolua_S,"closeMysql",tolua_automysql_DBMysql_closeMysql00);
   tolua_function(tolua_S,"pingMysql",tolua_automysql_DBMysql_pingMysql00);
   tolua_function(tolua_S,"getLastAutoID",tolua_automysql_DBMysql_getLastAutoID00);
   tolua_function(tolua_S,"setAutoCommit",tolua_automysql_DBMysql_setAutoCommit00);
   tolua_function(tolua_S,"commit",tolua_automysql_DBMysql_commit00);
   tolua_function(tolua_S,"rollback",tolua_automysql_DBMysql_rollback00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_automysql (lua_State* tolua_S) {
 return tolua_automysql_open(tolua_S);
};
#endif


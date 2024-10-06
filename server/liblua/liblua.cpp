/*
** Lua binding: liblua
** Generated automatically by tolua++-1.0.92 on Fri Oct  4 00:24:01 2024.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_liblua_open (lua_State* tolua_S);

#include "manual.hpp"
#include "level.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_MD5 (lua_State* tolua_S)
{
 MD5* self = (MD5*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_File (lua_State* tolua_S)
{
 File* self = (File*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_RequestData (lua_State* tolua_S)
{
 RequestData* self = (RequestData*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_MapFile (lua_State* tolua_S)
{
 MapFile* self = (MapFile*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"MD5");
 tolua_usertype(tolua_S,"MapFile");
 tolua_usertype(tolua_S,"File");
 tolua_usertype(tolua_S,"Manager");
 tolua_usertype(tolua_S,"RequestData");
 tolua_usertype(tolua_S,"Handler");
}

/* function: setMaxLogSize */
#ifndef TOLUA_DISABLE_tolua_liblua_setMaxLogSize00
static int tolua_liblua_setMaxLogSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int logSize = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   setMaxLogSize(logSize);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMaxLogSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getMaxLogSize */
#ifndef TOLUA_DISABLE_tolua_liblua_getMaxLogSize00
static int tolua_liblua_getMaxLogSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  getMaxLogSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMaxLogSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: setLogLevel */
#ifndef TOLUA_DISABLE_tolua_liblua_setLogLevel00
static int tolua_liblua_setLogLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int level = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   setLogLevel(level);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLogLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getLogLevel */
#ifndef TOLUA_DISABLE_tolua_liblua_getLogLevel00
static int tolua_liblua_getLogLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  getLogLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLogLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getTimeString */
#ifndef TOLUA_DISABLE_tolua_liblua_getTimeString00
static int tolua_liblua_getTimeString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  getTimeString();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTimeString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getTimeStringUS */
#ifndef TOLUA_DISABLE_tolua_liblua_getTimeStringUS00
static int tolua_liblua_getTimeStringUS00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  getTimeStringUS();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTimeStringUS'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getTimeStringUSFile */
#ifndef TOLUA_DISABLE_tolua_liblua_getTimeStringUSFile00
static int tolua_liblua_getTimeStringUSFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  getTimeStringUSFile();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTimeStringUSFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: writeCppLog */
#ifndef TOLUA_DISABLE_tolua_liblua_writeCppLog00
static int tolua_liblua_writeCppLog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* str = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   writeCppLog(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeCppLog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: getCppLogFileName */
#ifndef TOLUA_DISABLE_tolua_liblua_getCppLogFileName00
static int tolua_liblua_getCppLogFileName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   const char* tolua_ret = (const char*)  getCppLogFileName();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCppLogFileName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: writeRemoteLog */
#ifndef TOLUA_DISABLE_tolua_liblua_writeRemoteLog00
static int tolua_liblua_writeRemoteLog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* fileName = ((const char*)  tolua_tostring(tolua_S,1,0));
  const char* str = ((const char*)  tolua_tostring(tolua_S,2,0));
  {
   writeRemoteLog(fileName,str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeRemoteLog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: djb_hash */
#ifndef TOLUA_DISABLE_tolua_liblua_djb_hash00
static int tolua_liblua_djb_hash00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* str = ((const char*)  tolua_tostring(tolua_S,1,0));
  unsigned int length = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   unsigned int tolua_ret = ( unsigned int)  djb_hash(str,length);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'djb_hash'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_time_us */
#ifndef TOLUA_DISABLE_tolua_liblua_get_time_us00
static int tolua_liblua_get_time_us00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   long long tolua_ret = ( long long)  get_time_us();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_time_us'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_time_ms */
#ifndef TOLUA_DISABLE_tolua_liblua_get_time_ms00
static int tolua_liblua_get_time_ms00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   long long tolua_ret = ( long long)  get_time_ms();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_time_ms'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_time_s */
#ifndef TOLUA_DISABLE_tolua_liblua_get_time_s00
static int tolua_liblua_get_time_s00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   long long tolua_ret = ( long long)  get_time_s();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_time_s'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_time_seed */
#ifndef TOLUA_DISABLE_tolua_liblua_get_time_seed00
static int tolua_liblua_get_time_seed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
    int tolua_ret = (  int)  get_time_seed();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_time_seed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_next_time_us */
#ifndef TOLUA_DISABLE_tolua_liblua_get_next_time_us00
static int tolua_liblua_get_next_time_us00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  long long timeCount = (( long long)  tolua_tonumber(tolua_S,1,0));
  {
   long long tolua_ret = ( long long)  get_next_time_us(timeCount);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_next_time_us'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: get_next_time_ms */
#ifndef TOLUA_DISABLE_tolua_liblua_get_next_time_ms00
static int tolua_liblua_get_next_time_ms00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  long long timeCount = (( long long)  tolua_tonumber(tolua_S,1,0));
  {
   long long tolua_ret = ( long long)  get_next_time_ms(timeCount);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_next_time_ms'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: convert_ip2str */
#ifndef TOLUA_DISABLE_tolua_liblua_convert_ip2str00
static int tolua_liblua_convert_ip2str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int ip = (( unsigned int)  tolua_tonumber(tolua_S,1,0));
  {
   const char* tolua_ret = (const char*)  convert_ip2str(ip);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'convert_ip2str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: convert_str2ip */
#ifndef TOLUA_DISABLE_tolua_liblua_convert_str2ip00
static int tolua_liblua_convert_str2ip00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* ip = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   unsigned int tolua_ret = ( unsigned int)  convert_str2ip(ip);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'convert_str2ip'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: url_encode */
#ifndef TOLUA_DISABLE_tolua_liblua_url_encode00
static int tolua_liblua_url_encode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* str = ((const char*)  tolua_tostring(tolua_S,1,0));
  unsigned int length = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  url_encode(str,length);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'url_encode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: url_decode */
#ifndef TOLUA_DISABLE_tolua_liblua_url_decode00
static int tolua_liblua_url_decode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* str = ((const char*)  tolua_tostring(tolua_S,1,0));
  unsigned int length = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  url_decode(str,length);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'url_decode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: crc32 */
#ifndef TOLUA_DISABLE_tolua_liblua_crc3200
static int tolua_liblua_crc3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  unsigned int crc = ((unsigned int)  tolua_tonumber(tolua_S,1,0));
  unsigned const char* data = ((unsigned const char*)  tolua_tostring(tolua_S,2,0));
  long long count = (( long long)  tolua_tonumber(tolua_S,3,0));
  {
   unsigned int tolua_ret = (unsigned int)  crc32(crc,data,count);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'crc32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: snappy_compress */
#ifndef TOLUA_DISABLE_tolua_liblua_snappy_compress00
static int tolua_liblua_snappy_compress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* input = ((const char*)  tolua_tostring(tolua_S,1,0));
  long long input_length = (( long long)  tolua_tonumber(tolua_S,2,0));
  std::string compressed = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   long long tolua_ret = ( long long)  snappy_compress(input,input_length,compressed);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)compressed);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'snappy_compress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: snappy_uncompress */
#ifndef TOLUA_DISABLE_tolua_liblua_snappy_uncompress00
static int tolua_liblua_snappy_uncompress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* compressed = ((const char*)  tolua_tostring(tolua_S,1,0));
  long long compressed_length = (( long long)  tolua_tonumber(tolua_S,2,0));
  std::string uncompressed = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  snappy_uncompress(compressed,compressed_length,uncompressed);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)uncompressed);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'snappy_uncompress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: broadcast of class  Handler */
#ifndef TOLUA_DISABLE_tolua_liblua_Handler_broadcast00
static int tolua_liblua_Handler_broadcast00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Handler",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Handler* self = (Handler*)  tolua_tousertype(tolua_S,1,0);
  const char* ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int length = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'broadcast'", NULL);
#endif
  {
   self->broadcast(ptr,length);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'broadcast'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send of class  Handler */
#ifndef TOLUA_DISABLE_tolua_liblua_Handler_send00
static int tolua_liblua_Handler_send00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Handler",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Handler* self = (Handler*)  tolua_tousertype(tolua_S,1,0);
  const char* ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int length = ((int)  tolua_tonumber(tolua_S,3,0));
  unsigned short serverIndex = ((unsigned short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->send(ptr,length,serverIndex);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: startTimer of class  Handler */
#ifndef TOLUA_DISABLE_tolua_liblua_Handler_startTimer00
static int tolua_liblua_Handler_startTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Handler",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Handler* self = (Handler*)  tolua_tousertype(tolua_S,1,0);
  unsigned int callbackID = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  long long timeCount = (( long long)  tolua_tonumber(tolua_S,3,0));
  long long fromTime = (( long long)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'startTimer'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->startTimer(callbackID,timeCount,fromTime);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'startTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeTimer of class  Handler */
#ifndef TOLUA_DISABLE_tolua_liblua_Handler_removeTimer00
static int tolua_liblua_Handler_removeTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Handler",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Handler* self = (Handler*)  tolua_tousertype(tolua_S,1,0);
  unsigned int handle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeTimer'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->removeTimer(handle);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: changeTimerCallbackID of class  Handler */
#ifndef TOLUA_DISABLE_tolua_liblua_Handler_changeTimerCallbackID00
static int tolua_liblua_Handler_changeTimerCallbackID00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Handler",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Handler* self = (Handler*)  tolua_tousertype(tolua_S,1,0);
  unsigned int handle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int callbackID = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'changeTimerCallbackID'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->changeTimerCallbackID(handle,callbackID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'changeTimerCallbackID'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: changeTimer of class  Handler */
#ifndef TOLUA_DISABLE_tolua_liblua_Handler_changeTimer00
static int tolua_liblua_Handler_changeTimer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Handler",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Handler* self = (Handler*)  tolua_tousertype(tolua_S,1,0);
  unsigned int handle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int callbackID = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
  long long timeCount = (( long long)  tolua_tonumber(tolua_S,4,0));
  long long fromTime = (( long long)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'changeTimer'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->changeTimer(handle,callbackID,timeCount,fromTime);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'changeTimer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTimerLeft of class  Handler */
#ifndef TOLUA_DISABLE_tolua_liblua_Handler_getTimerLeft00
static int tolua_liblua_Handler_getTimerLeft00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Handler",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Handler* self = (Handler*)  tolua_tousertype(tolua_S,1,0);
  unsigned int handle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTimerLeft'", NULL);
#endif
  {
   long long tolua_ret = ( long long)  self->getTimerLeft(handle);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTimerLeft'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setNextFrameActive of class  Handler */
#ifndef TOLUA_DISABLE_tolua_liblua_Handler_setNextFrameActive00
static int tolua_liblua_Handler_setNextFrameActive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Handler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Handler* self = (Handler*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setNextFrameActive'", NULL);
#endif
  {
   self->setNextFrameActive();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setNextFrameActive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getManager of class  Handler */
#ifndef TOLUA_DISABLE_tolua_liblua_Handler_getManager00
static int tolua_liblua_Handler_getManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Handler",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Handler* self = (Handler*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getManager'", NULL);
#endif
  {
   Manager* tolua_ret = (Manager*)  self->getManager();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Manager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_new00
static int tolua_liblua_RequestData_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   RequestData* tolua_ret = (RequestData*)  Mtolua_new((RequestData)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"RequestData");
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

/* method: new_local of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_new00_local
static int tolua_liblua_RequestData_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   RequestData* tolua_ret = (RequestData*)  Mtolua_new((RequestData)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"RequestData");
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

/* method: delete of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_delete00
static int tolua_liblua_RequestData_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RequestData* self = (RequestData*)  tolua_tousertype(tolua_S,1,0);
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

/* method: initialize of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_initialize00
static int tolua_liblua_RequestData_initialize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RequestData* self = (RequestData*)  tolua_tousertype(tolua_S,1,0);
  const char* method = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* url = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* body = ((const char*)  tolua_tostring(tolua_S,4,0));
  unsigned int body_length = (( unsigned int)  tolua_tonumber(tolua_S,5,0));
  unsigned int callbackID = (( unsigned int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initialize'", NULL);
#endif
  {
   self->initialize(method,url,body,body_length,callbackID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initialize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPhonyIP of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_setPhonyIP00
static int tolua_liblua_RequestData_setPhonyIP00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RequestData* self = (RequestData*)  tolua_tousertype(tolua_S,1,0);
  unsigned int phonyIP = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPhonyIP'", NULL);
#endif
  {
   self->setPhonyIP(phonyIP);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPhonyIP'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setServerHandle of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_setServerHandle00
static int tolua_liblua_RequestData_setServerHandle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RequestData* self = (RequestData*)  tolua_tousertype(tolua_S,1,0);
  unsigned int serverHandle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setServerHandle'", NULL);
#endif
  {
   self->setServerHandle(serverHandle);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setServerHandle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setKeepAlive of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_setKeepAlive00
static int tolua_liblua_RequestData_setKeepAlive00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RequestData* self = (RequestData*)  tolua_tousertype(tolua_S,1,0);
  bool isKeepAlive = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setKeepAlive'", NULL);
#endif
  {
   self->setKeepAlive(isKeepAlive);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setKeepAlive'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLocalHost of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_setLocalHost00
static int tolua_liblua_RequestData_setLocalHost00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RequestData* self = (RequestData*)  tolua_tousertype(tolua_S,1,0);
  const char* localhost = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLocalHost'", NULL);
#endif
  {
   self->setLocalHost(localhost);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLocalHost'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: appendHeader of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_appendHeader00
static int tolua_liblua_RequestData_appendHeader00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RequestData* self = (RequestData*)  tolua_tousertype(tolua_S,1,0);
  const char* field = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* value = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'appendHeader'", NULL);
#endif
  {
   self->appendHeader(field,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'appendHeader'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clearData of class  RequestData */
#ifndef TOLUA_DISABLE_tolua_liblua_RequestData_clearData00
static int tolua_liblua_RequestData_clearData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RequestData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RequestData* self = (RequestData*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clearData'", NULL);
#endif
  {
   self->clearData();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clearData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: closeConnect of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_closeConnect00
static int tolua_liblua_Manager_closeConnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int handle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'closeConnect'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->closeConnect(handle);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'closeConnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: openWebSocket of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_openWebSocket00
static int tolua_liblua_Manager_openWebSocket00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  const char* url = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned int callbackID = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int serverHandle = (( unsigned int)  tolua_tonumber(tolua_S,4,0));
  unsigned int phonyIP = (( unsigned int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'openWebSocket'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->openWebSocket(url,callbackID,serverHandle,phonyIP);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'openWebSocket'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendWebSocket of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_sendWebSocket00
static int tolua_liblua_Manager_sendWebSocket00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int handle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  const char* ptr = ((const char*)  tolua_tostring(tolua_S,3,0));
  int length = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendWebSocket'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->sendWebSocket(handle,ptr,length);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendWebSocket'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: bindConnect of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_bindConnect00
static int tolua_liblua_Manager_bindConnect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int handle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  unsigned int serverHandle = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
  unsigned int callbackID = (( unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'bindConnect'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->bindConnect(handle,serverHandle,callbackID);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bindConnect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sendHttpRequest of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_sendHttpRequest00
static int tolua_liblua_Manager_sendHttpRequest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"RequestData",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int handle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  RequestData* pRequest = ((RequestData*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sendHttpRequest'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->sendHttpRequest(handle,pRequest);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sendHttpRequest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: responseHttp of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_responseHttp00
static int tolua_liblua_Manager_responseHttp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  unsigned int handle = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
  const char* ptr = ((const char*)  tolua_tostring(tolua_S,3,0));
  int length = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'responseHttp'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->responseHttp(handle,ptr,length);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'responseHttp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRequestData of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_getRequestData00
static int tolua_liblua_Manager_getRequestData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRequestData'", NULL);
#endif
  {
   RequestData* tolua_ret = (RequestData*)  self->getRequestData();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"RequestData");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRequestData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerHandle of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_getServerHandle00
static int tolua_liblua_Manager_getServerHandle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerHandle'", NULL);
#endif
  {
   unsigned int tolua_ret = (unsigned int)  self->getServerHandle();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerHandle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGroupIndex of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_getGroupIndex00
static int tolua_liblua_Manager_getGroupIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGroupIndex'", NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->getGroupIndex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGroupIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerType of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_getServerType00
static int tolua_liblua_Manager_getServerType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerType'", NULL);
#endif
  {
   unsigned char tolua_ret = (unsigned char)  self->getServerType();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getServerIndex of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_getServerIndex00
static int tolua_liblua_Manager_getServerIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getServerIndex'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->getServerIndex();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getServerIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setServerType of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_setServerType00
static int tolua_liblua_Manager_setServerType00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  unsigned char type = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setServerType'", NULL);
#endif
  {
   self->setServerType(type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setServerType'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setGroupIndex of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_setGroupIndex00
static int tolua_liblua_Manager_setGroupIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  unsigned char groupIndex = ((unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGroupIndex'", NULL);
#endif
  {
   self->setGroupIndex(groupIndex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGroupIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: initHttpsCertificate of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_initHttpsCertificate00
static int tolua_liblua_Manager_initHttpsCertificate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  const char* publicKey = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* privateKey = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'initHttpsCertificate'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->initHttpsCertificate(publicKey,privateKey);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'initHttpsCertificate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setActiveNextFrameEnd of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_setActiveNextFrameEnd00
static int tolua_liblua_Manager_setActiveNextFrameEnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
  bool active = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setActiveNextFrameEnd'", NULL);
#endif
  {
   self->setActiveNextFrameEnd(active);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setActiveNextFrameEnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isActiveNextFrameEnd of class  Manager */
#ifndef TOLUA_DISABLE_tolua_liblua_Manager_isActiveNextFrameEnd00
static int tolua_liblua_Manager_isActiveNextFrameEnd00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Manager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Manager* self = (Manager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isActiveNextFrameEnd'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isActiveNextFrameEnd();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isActiveNextFrameEnd'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_new00
static int tolua_liblua_File_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   File* tolua_ret = (File*)  Mtolua_new((File)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"File");
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

/* method: new_local of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_new00_local
static int tolua_liblua_File_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   File* tolua_ret = (File*)  Mtolua_new((File)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"File");
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

/* method: delete of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_delete00
static int tolua_liblua_File_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
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

/* method: openFile of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_openFile00
static int tolua_liblua_File_openFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
  bool createNew = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'openFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->openFile(createNew);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'openFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: closeFile of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_closeFile00
static int tolua_liblua_File_closeFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'closeFile'", NULL);
#endif
  {
   self->closeFile();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'closeFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: touchFile of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_touchFile00
static int tolua_liblua_File_touchFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
  const char* checkHead = ((const char*)  tolua_tostring(tolua_S,2,0));
  int checkLength = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'touchFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->touchFile(checkHead,checkLength);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'touchFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeData of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_writeData00
static int tolua_liblua_File_writeData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
  long long offset = (( long long)  tolua_tonumber(tolua_S,2,0));
  const char* pData = ((const char*)  tolua_tostring(tolua_S,3,0));
  long long length = (( long long)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeData'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->writeData(offset,pData,length);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: fastWriteData of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_fastWriteData00
static int tolua_liblua_File_fastWriteData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
  long long offset = (( long long)  tolua_tonumber(tolua_S,2,0));
  const char* pData = ((const char*)  tolua_tostring(tolua_S,3,0));
  long long length = (( long long)  tolua_tonumber(tolua_S,4,0));
  bool clearAll = ((bool)  tolua_toboolean(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fastWriteData'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->fastWriteData(offset,pData,length,clearAll);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fastWriteData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: renameFile of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_renameFile00
static int tolua_liblua_File_renameFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
  const char* newName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'renameFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->renameFile(newName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'renameFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: removeFile of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_removeFile00
static int tolua_liblua_File_removeFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
  const char* fileName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->removeFile(fileName);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'removeFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setFileName of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_setFileName00
static int tolua_liblua_File_setFileName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setFileName'", NULL);
#endif
  {
   self->setFileName(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setFileName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFileName of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_getFileName00
static int tolua_liblua_File_getFileName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFileName'", NULL);
#endif
  {
   const std::string tolua_ret = (const std::string)  self->getFileName();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFileName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFileLength of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_getFileLength00
static int tolua_liblua_File_getFileLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFileLength'", NULL);
#endif
  {
   long long tolua_ret = ( long long)  self->getFileLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFileLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getExpandSize of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_getExpandSize00
static int tolua_liblua_File_getExpandSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getExpandSize'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getExpandSize();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getExpandSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setExpandSize of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_setExpandSize00
static int tolua_liblua_File_setExpandSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
  int exp = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setExpandSize'", NULL);
#endif
  {
   self->setExpandSize(exp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setExpandSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getFileHandle of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_getFileHandle00
static int tolua_liblua_File_getFileHandle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getFileHandle'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getFileHandle();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getFileHandle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: fileSeek of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_fileSeek00
static int tolua_liblua_File_fileSeek00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
  long long offset = (( long long)  tolua_tonumber(tolua_S,2,0));
  int seek = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fileSeek'", NULL);
#endif
  {
   long long tolua_ret = ( long long)  self->fileSeek(offset,seek);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fileSeek'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readTell of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_readTell00
static int tolua_liblua_File_readTell00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readTell'", NULL);
#endif
  {
   long long tolua_ret = ( long long)  self->readTell();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readTell'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeTell of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_writeTell00
static int tolua_liblua_File_writeTell00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeTell'", NULL);
#endif
  {
   long long tolua_ret = ( long long)  self->writeTell();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeTell'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: flush of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_flush00
static int tolua_liblua_File_flush00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'flush'", NULL);
#endif
  {
   int tolua_ret = (int)  self->flush();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'flush'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isOpen of class  File */
#ifndef TOLUA_DISABLE_tolua_liblua_File_isOpen00
static int tolua_liblua_File_isOpen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"File",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  File* self = (File*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isOpen'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isOpen();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isOpen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MapFile */
#ifndef TOLUA_DISABLE_tolua_liblua_MapFile_new00
static int tolua_liblua_MapFile_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MapFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MapFile* tolua_ret = (MapFile*)  Mtolua_new((MapFile)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MapFile");
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

/* method: new_local of class  MapFile */
#ifndef TOLUA_DISABLE_tolua_liblua_MapFile_new00_local
static int tolua_liblua_MapFile_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MapFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MapFile* tolua_ret = (MapFile*)  Mtolua_new((MapFile)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MapFile");
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

/* method: delete of class  MapFile */
#ifndef TOLUA_DISABLE_tolua_liblua_MapFile_delete00
static int tolua_liblua_MapFile_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MapFile",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MapFile* self = (MapFile*)  tolua_tousertype(tolua_S,1,0);
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

/* method: syncFile of class  MapFile */
#ifndef TOLUA_DISABLE_tolua_liblua_MapFile_syncFile00
static int tolua_liblua_MapFile_syncFile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MapFile",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MapFile* self = (MapFile*)  tolua_tousertype(tolua_S,1,0);
  bool saveAll = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool asyncSave = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'syncFile'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->syncFile(saveAll,asyncSave);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'syncFile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: syncBlock of class  MapFile */
#ifndef TOLUA_DISABLE_tolua_liblua_MapFile_syncBlock00
static int tolua_liblua_MapFile_syncBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MapFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MapFile* self = (MapFile*)  tolua_tousertype(tolua_S,1,0);
  long long offset = (( long long)  tolua_tonumber(tolua_S,2,0));
  unsigned long long length = (( unsigned long long)  tolua_tonumber(tolua_S,3,0));
  bool asyncSave = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'syncBlock'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->syncBlock(offset,length,asyncSave);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'syncBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: moveTo of class  MapFile */
#ifndef TOLUA_DISABLE_tolua_liblua_MapFile_moveTo00
static int tolua_liblua_MapFile_moveTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MapFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MapFile* self = (MapFile*)  tolua_tousertype(tolua_S,1,0);
  long long to = (( long long)  tolua_tonumber(tolua_S,2,0));
  long long offset = (( long long)  tolua_tonumber(tolua_S,3,0));
  unsigned long long length = (( unsigned long long)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'moveTo'", NULL);
#endif
  {
   self->moveTo(to,offset,length);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'moveTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: markDirtyBlock of class  MapFile */
#ifndef TOLUA_DISABLE_tolua_liblua_MapFile_markDirtyBlock00
static int tolua_liblua_MapFile_markDirtyBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MapFile",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MapFile* self = (MapFile*)  tolua_tousertype(tolua_S,1,0);
  long long offset = (( long long)  tolua_tonumber(tolua_S,2,0));
  unsigned long long length = (( unsigned long long)  tolua_tonumber(tolua_S,3,0));
  bool mark = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'markDirtyBlock'", NULL);
#endif
  {
   self->markDirtyBlock(offset,length,mark);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'markDirtyBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  MD5 */
#ifndef TOLUA_DISABLE_tolua_liblua_MD5_new00
static int tolua_liblua_MD5_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MD5",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MD5* tolua_ret = (MD5*)  Mtolua_new((MD5)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MD5");
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

/* method: new_local of class  MD5 */
#ifndef TOLUA_DISABLE_tolua_liblua_MD5_new00_local
static int tolua_liblua_MD5_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MD5",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MD5* tolua_ret = (MD5*)  Mtolua_new((MD5)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MD5");
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

/* method: delete of class  MD5 */
#ifndef TOLUA_DISABLE_tolua_liblua_MD5_delete00
static int tolua_liblua_MD5_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MD5",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MD5* self = (MD5*)  tolua_tousertype(tolua_S,1,0);
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

/* method: update of class  MD5 */
#ifndef TOLUA_DISABLE_tolua_liblua_MD5_update00
static int tolua_liblua_MD5_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MD5",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MD5* self = (MD5*)  tolua_tousertype(tolua_S,1,0);
  const char* ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  unsigned int length = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'update'", NULL);
#endif
  {
   self->update(ptr,length);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getString of class  MD5 */
#ifndef TOLUA_DISABLE_tolua_liblua_MD5_getString00
static int tolua_liblua_MD5_getString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MD5",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MD5* self = (MD5*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getString'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getString();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: md5sum */
#ifndef TOLUA_DISABLE_tolua_liblua_md5sum00
static int tolua_liblua_md5sum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* ptr = ((const char*)  tolua_tostring(tolua_S,1,0));
  unsigned int length = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  {
   const char* tolua_ret = (const char*)  md5sum(ptr,length);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'md5sum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_liblua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"setMaxLogSize",tolua_liblua_setMaxLogSize00);
  tolua_function(tolua_S,"getMaxLogSize",tolua_liblua_getMaxLogSize00);
  tolua_function(tolua_S,"setLogLevel",tolua_liblua_setLogLevel00);
  tolua_function(tolua_S,"getLogLevel",tolua_liblua_getLogLevel00);
  tolua_function(tolua_S,"getTimeString",tolua_liblua_getTimeString00);
  tolua_function(tolua_S,"getTimeStringUS",tolua_liblua_getTimeStringUS00);
  tolua_function(tolua_S,"getTimeStringUSFile",tolua_liblua_getTimeStringUSFile00);
  tolua_function(tolua_S,"writeCppLog",tolua_liblua_writeCppLog00);
  tolua_function(tolua_S,"getCppLogFileName",tolua_liblua_getCppLogFileName00);
  tolua_function(tolua_S,"writeRemoteLog",tolua_liblua_writeRemoteLog00);
  tolua_function(tolua_S,"djb_hash",tolua_liblua_djb_hash00);
  tolua_function(tolua_S,"get_time_us",tolua_liblua_get_time_us00);
  tolua_function(tolua_S,"get_time_ms",tolua_liblua_get_time_ms00);
  tolua_function(tolua_S,"get_time_s",tolua_liblua_get_time_s00);
  tolua_function(tolua_S,"get_time_seed",tolua_liblua_get_time_seed00);
  tolua_function(tolua_S,"get_next_time_us",tolua_liblua_get_next_time_us00);
  tolua_function(tolua_S,"get_next_time_ms",tolua_liblua_get_next_time_ms00);
  tolua_function(tolua_S,"convert_ip2str",tolua_liblua_convert_ip2str00);
  tolua_function(tolua_S,"convert_str2ip",tolua_liblua_convert_str2ip00);
  tolua_function(tolua_S,"url_encode",tolua_liblua_url_encode00);
  tolua_function(tolua_S,"url_decode",tolua_liblua_url_decode00);
  tolua_function(tolua_S,"crc32",tolua_liblua_crc3200);
  tolua_function(tolua_S,"snappy_compress",tolua_liblua_snappy_compress00);
  tolua_function(tolua_S,"snappy_uncompress",tolua_liblua_snappy_uncompress00);
  tolua_cclass(tolua_S,"Handler","Handler","",NULL);
  tolua_beginmodule(tolua_S,"Handler");
   tolua_function(tolua_S,"broadcast",tolua_liblua_Handler_broadcast00);
   tolua_function(tolua_S,"send",tolua_liblua_Handler_send00);
   tolua_function(tolua_S,"startTimer",tolua_liblua_Handler_startTimer00);
   tolua_function(tolua_S,"removeTimer",tolua_liblua_Handler_removeTimer00);
   tolua_function(tolua_S,"changeTimerCallbackID",tolua_liblua_Handler_changeTimerCallbackID00);
   tolua_function(tolua_S,"changeTimer",tolua_liblua_Handler_changeTimer00);
   tolua_function(tolua_S,"getTimerLeft",tolua_liblua_Handler_getTimerLeft00);
   tolua_function(tolua_S,"setNextFrameActive",tolua_liblua_Handler_setNextFrameActive00);
   tolua_function(tolua_S,"getManager",tolua_liblua_Handler_getManager00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"RequestData","RequestData","",tolua_collect_RequestData);
  #else
  tolua_cclass(tolua_S,"RequestData","RequestData","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"RequestData");
   tolua_function(tolua_S,"new",tolua_liblua_RequestData_new00);
   tolua_function(tolua_S,"new_local",tolua_liblua_RequestData_new00_local);
   tolua_function(tolua_S,".call",tolua_liblua_RequestData_new00_local);
   tolua_function(tolua_S,"delete",tolua_liblua_RequestData_delete00);
   tolua_function(tolua_S,"initialize",tolua_liblua_RequestData_initialize00);
   tolua_function(tolua_S,"setPhonyIP",tolua_liblua_RequestData_setPhonyIP00);
   tolua_function(tolua_S,"setServerHandle",tolua_liblua_RequestData_setServerHandle00);
   tolua_function(tolua_S,"setKeepAlive",tolua_liblua_RequestData_setKeepAlive00);
   tolua_function(tolua_S,"setLocalHost",tolua_liblua_RequestData_setLocalHost00);
   tolua_function(tolua_S,"appendHeader",tolua_liblua_RequestData_appendHeader00);
   tolua_function(tolua_S,"clearData",tolua_liblua_RequestData_clearData00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Manager","Manager","",NULL);
  tolua_beginmodule(tolua_S,"Manager");
   tolua_function(tolua_S,"closeConnect",tolua_liblua_Manager_closeConnect00);
   tolua_function(tolua_S,"openWebSocket",tolua_liblua_Manager_openWebSocket00);
   tolua_function(tolua_S,"sendWebSocket",tolua_liblua_Manager_sendWebSocket00);
   tolua_function(tolua_S,"bindConnect",tolua_liblua_Manager_bindConnect00);
   tolua_function(tolua_S,"sendHttpRequest",tolua_liblua_Manager_sendHttpRequest00);
   tolua_function(tolua_S,"responseHttp",tolua_liblua_Manager_responseHttp00);
   tolua_function(tolua_S,"getRequestData",tolua_liblua_Manager_getRequestData00);
   tolua_function(tolua_S,"getServerHandle",tolua_liblua_Manager_getServerHandle00);
   tolua_function(tolua_S,"getGroupIndex",tolua_liblua_Manager_getGroupIndex00);
   tolua_function(tolua_S,"getServerType",tolua_liblua_Manager_getServerType00);
   tolua_function(tolua_S,"getServerIndex",tolua_liblua_Manager_getServerIndex00);
   tolua_function(tolua_S,"setServerType",tolua_liblua_Manager_setServerType00);
   tolua_function(tolua_S,"setGroupIndex",tolua_liblua_Manager_setGroupIndex00);
   tolua_function(tolua_S,"initHttpsCertificate",tolua_liblua_Manager_initHttpsCertificate00);
   tolua_function(tolua_S,"setActiveNextFrameEnd",tolua_liblua_Manager_setActiveNextFrameEnd00);
   tolua_function(tolua_S,"isActiveNextFrameEnd",tolua_liblua_Manager_isActiveNextFrameEnd00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"BLOCK_SIZE",BLOCK_SIZE);
  tolua_constant(tolua_S,"MAX_EXPAND_BLOCK_SIZE",MAX_EXPAND_BLOCK_SIZE);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"File","File","",tolua_collect_File);
  #else
  tolua_cclass(tolua_S,"File","File","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"File");
   tolua_function(tolua_S,"new",tolua_liblua_File_new00);
   tolua_function(tolua_S,"new_local",tolua_liblua_File_new00_local);
   tolua_function(tolua_S,".call",tolua_liblua_File_new00_local);
   tolua_function(tolua_S,"delete",tolua_liblua_File_delete00);
   tolua_function(tolua_S,"openFile",tolua_liblua_File_openFile00);
   tolua_function(tolua_S,"closeFile",tolua_liblua_File_closeFile00);
   tolua_function(tolua_S,"touchFile",tolua_liblua_File_touchFile00);
   tolua_function(tolua_S,"writeData",tolua_liblua_File_writeData00);
   tolua_function(tolua_S,"fastWriteData",tolua_liblua_File_fastWriteData00);
   tolua_function(tolua_S,"renameFile",tolua_liblua_File_renameFile00);
   tolua_function(tolua_S,"removeFile",tolua_liblua_File_removeFile00);
   tolua_function(tolua_S,"setFileName",tolua_liblua_File_setFileName00);
   tolua_function(tolua_S,"getFileName",tolua_liblua_File_getFileName00);
   tolua_function(tolua_S,"getFileLength",tolua_liblua_File_getFileLength00);
   tolua_function(tolua_S,"getExpandSize",tolua_liblua_File_getExpandSize00);
   tolua_function(tolua_S,"setExpandSize",tolua_liblua_File_setExpandSize00);
   tolua_function(tolua_S,"getFileHandle",tolua_liblua_File_getFileHandle00);
   tolua_function(tolua_S,"fileSeek",tolua_liblua_File_fileSeek00);
   tolua_function(tolua_S,"readTell",tolua_liblua_File_readTell00);
   tolua_function(tolua_S,"writeTell",tolua_liblua_File_writeTell00);
   tolua_function(tolua_S,"flush",tolua_liblua_File_flush00);
   tolua_function(tolua_S,"isOpen",tolua_liblua_File_isOpen00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MapFile","MapFile","File",tolua_collect_MapFile);
  #else
  tolua_cclass(tolua_S,"MapFile","MapFile","File",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MapFile");
   tolua_function(tolua_S,"new",tolua_liblua_MapFile_new00);
   tolua_function(tolua_S,"new_local",tolua_liblua_MapFile_new00_local);
   tolua_function(tolua_S,".call",tolua_liblua_MapFile_new00_local);
   tolua_function(tolua_S,"delete",tolua_liblua_MapFile_delete00);
   tolua_function(tolua_S,"syncFile",tolua_liblua_MapFile_syncFile00);
   tolua_function(tolua_S,"syncBlock",tolua_liblua_MapFile_syncBlock00);
   tolua_function(tolua_S,"moveTo",tolua_liblua_MapFile_moveTo00);
   tolua_function(tolua_S,"markDirtyBlock",tolua_liblua_MapFile_markDirtyBlock00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MD5","MD5","",tolua_collect_MD5);
  #else
  tolua_cclass(tolua_S,"MD5","MD5","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MD5");
   tolua_function(tolua_S,"new",tolua_liblua_MD5_new00);
   tolua_function(tolua_S,"new_local",tolua_liblua_MD5_new00_local);
   tolua_function(tolua_S,".call",tolua_liblua_MD5_new00_local);
   tolua_function(tolua_S,"delete",tolua_liblua_MD5_delete00);
   tolua_function(tolua_S,"update",tolua_liblua_MD5_update00);
   tolua_function(tolua_S,"getString",tolua_liblua_MD5_getString00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"md5sum",tolua_liblua_md5sum00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_liblua (lua_State* tolua_S) {
 return tolua_liblua_open(tolua_S);
};
#endif



#include "dbmysql.h"
#include "automysql.h"
#include "manual.hpp"

extern "C" {

int luaopen_libmysql (lua_State* tolua_S){
    luaopen_automysql(tolua_S);
    tolua_libmysql_manual_open(tolua_S);
    DBMysql::onInitialize();
    return 1;
}

}



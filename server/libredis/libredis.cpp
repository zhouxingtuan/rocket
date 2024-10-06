
#include "dbredis.h"
#include "autoredis.h"
#include "manual.hpp"

extern "C" {

int luaopen_libredis (lua_State* tolua_S){
    luaopen_autoredis(tolua_S);
    tolua_libredis_manual_open(tolua_S);
    return 1;
}

}



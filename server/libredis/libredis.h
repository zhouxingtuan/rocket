//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 2017/5/14
// Time: 上午11:25
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__libredis__
#define __hive__libredis__

#include "lua.hpp"

extern "C"{
/* Exported function */
int luaopen_libredis (lua_State* tolua_S);
}

#endif

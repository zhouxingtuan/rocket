//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 2017/5/14
// Time: 上午11:25
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__libmysql__
#define __hive__libmysql__

#include "lua.hpp"

extern "C"{
/* Exported function */
int luaopen_libmysql (lua_State* tolua_S);
}

#endif

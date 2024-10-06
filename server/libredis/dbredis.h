//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/12/6
// Time: 下午10:52
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__dbredis__
#define __hive__dbredis__

#include "lua.hpp"
#include "tolua++.h"
#include "hiredis.h"
#include <string.h>
#include <vector>

#define NS_HIVE_BEGIN namespace hive {
#define NS_HIVE_END };
#define USING_NS_HIVE using namespace hive;

NS_HIVE_BEGIN

// 统一所有数据类型的使用
typedef char 						int8;
typedef short 					int16;
typedef int 						int32;
typedef long long   				int64;
typedef unsigned char 			uint8;
typedef unsigned short  			uint16;
typedef unsigned int    			uint32;
typedef unsigned long long      uint64;

/*--------------------------------------------------------------------*/

typedef const char* ConstCharPtr;

class DBRedis
{
public:
	typedef std::vector<size_t> SizeTVector;
	typedef std::vector<ConstCharPtr> ConstCharPtrVector;
public:
    DBRedis(void);
    ~DBRedis(void);

    bool isAvailable(void);
	bool connectRedis(const char* ip, uint16 port);
	void closeRedis(void);
	bool identifyRedis(const char* pwd);
	int command(lua_State* tolua_S, bool skipResult);
	void appendCommand(lua_State* tolua_S);
	int getReply(lua_State* tolua_S);

    int executeCommandArgv(lua_State* tolua_S, int argc, const char **argv, const size_t *argvlen);
    bool executeCommandArgvWithoutReturn(int argc, const char **argv, const size_t *argvlen);
    inline bool checkReplyStatus(redisReply* reply);
    void checkRedisReplyData(lua_State* tolua_S, redisReply* reply);
private:
	redisContext* m_pContext;
	SizeTVector m_argvlen;
	ConstCharPtrVector m_argv;
};

NS_HIVE_END

#endif

//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/12/6
// Time: 下午10:52
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__dbmysql__
#define __hive__dbmysql__

#include "lua.hpp"
#include "tolua++.h"
#include <mysql/mysql.h>
//thread libs
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <string>
#include <atomic>

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

class DBMysql
{
public:
    DBMysql(void);
    ~DBMysql(void);

    static void onInitialize(void);

    bool isAvailable(void);
	bool connectMySql(const char* ip, const char* user, const char* pwd, const char* db, uint16 port);
	void closeMysql(void);
	bool pingMysql(void);
	int64 getLastAutoID(void);
	void setAutoCommit(bool on);
	bool commit(void);
	bool rollback(void);

	bool execute(const char* query, int queryNumber, bool skipResult){ return false; }
	int execute(lua_State* tolua_S, const char* query, uint32 length, int queryNumber, bool skipResult);

	const char* escape(const char* ptr){ return ptr; }
	int escape(lua_State* tolua_S, const char* ptr, uint32 length);
private:

    int executeQuery(lua_State* tolua_S, const char* query, unsigned int length, int queryNumber);
    bool executeQueryWithoutReturn(const char* query, unsigned int length);
    void parseResult(lua_State* tolua_S, MYSQL_RES* result);
    void parseOther(lua_State* tolua_S, MYSQL_RES* result);
private:
	MYSQL* m_pMysql;
	std::vector<char> m_buffer;
//	Buffer* m_pBuffer;
};

NS_HIVE_END

#endif

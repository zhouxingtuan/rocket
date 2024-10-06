//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/5/29
// Time: 上午9:08
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__script__
#define __hive__script__

#include "common.h"
#include "net.h"
#include "lua.hpp"
#include "tolua++.h"

NS_HIVE_BEGIN

class Script
{
public:
    Script(void);
    virtual ~Script(void);

	virtual void setState(lua_State* pState);
	inline lua_State * getState( void ) { return m_pState; }
	inline bool requireFile(const std::string& file){
		std::string exec = "require (\"" + file + "\")";
		return executeText(exec.c_str(), exec.length(), NULL);
	}
	inline bool executeFile( const char * script_file_path ){
		if( luaL_loadfile(m_pState, script_file_path ) == 0 ){
			if( lua_resume( m_pState, 0 ) == 0 ){
				return true;
			}
		}
		outputError();
		return false;
	}
	inline bool executeText( const char * script, const int script_length, const char * comment = NULL ){
		if( luaL_loadbuffer( m_pState, script, script_length, comment ) == 0 ){
			if( lua_pcall( m_pState, 0, 0, 0 ) == 0 ){
				return true;
			}
		}
		outputError();
		return false;
	}
    inline void luaCall( int parameter_number, int result_count = 0 ){
        if( lua_pcall( m_pState, parameter_number, result_count, 0 ) != 0 ){
            outputError();
        }
    }
    // Lua堆栈操作函数
    inline void pushFunction(const char * function_name){
        lua_settop(m_pState, 0);
        lua_getglobal(m_pState, function_name);
    }
    inline void pushNumber(double number){
        lua_pushnumber(m_pState,(lua_Number)number);
    }
    inline void pushString(const char* ptr, int64 length){
		lua_pushlstring(m_pState, ptr, length);
    }
    inline void pushString(const std::string& str){
        lua_pushlstring(m_pState, str.c_str(), str.length());
    }
    inline void pushBool(bool b){
        lua_pushboolean(m_pState, b);
    }
    inline void pushNil(void){
        lua_pushnil(m_pState);
    }
    inline void pushTable(uint32 table_size){
        lua_createtable(m_pState, 0, table_size);
    }
    inline void setTableElement(void){
        lua_rawset(m_pState,-3);
    }
    inline void pushArray(uint32 array_size){
        lua_createtable(m_pState, array_size, 0);
    }
    inline void setArrayElement(int index){
        lua_rawseti(m_pState, -2, index);
    }
    inline void pushUserData(void* ptr, const char* class_name){
        tolua_pushusertype(m_pState, (void*)ptr, class_name);
    }
	// 执行方法
	inline void executeFunction(int result_count){
		int param_count = lua_gettop(m_pState) - 1;
//		LOG_DEBUG("param_count=%d result_count=%d", param_count, result_count);
		luaCall(param_count, result_count);
	}
	inline void setGlobal(const char* global_name){
        lua_setglobal(m_pState, global_name);
	}

	// 判断栈中数据类型
	inline bool isNil(int index){
		int t = lua_type(m_pState, index);
		return (t == LUA_TNIL);
	}
	inline bool isNumber(int index){
		int t = lua_type(m_pState, index);
		return (t == LUA_TNUMBER);
	}

	// 从Lua函数返回结果中拿出数据
	inline double getDouble(int index, double value=0){
		return ((double)tolua_tonumber(m_pState, index, value));
	}
	inline int64 getInt64(int index, int value=0){
		return ((int64)tolua_tonumber(m_pState, index, value));
	}
	inline uint64 getUInt64(int index, int value=0){
		return ((uint64)tolua_tonumber(m_pState, index, value));
	}
	inline int getInt(int index, int value=0){
		return ((int)tolua_tonumber(m_pState, index, value));
	}
	inline uint32 getUInt(int index, int value=0){
		return ((uint32)tolua_tonumber(m_pState, index, value));
	}
	inline const char* getString(int index, size_t* length){
		return ((const char*)lua_tolstring(m_pState, index, (size_t*)length));
	}
	inline bool getBool(int index, bool value=false){
		return ((bool)tolua_toboolean(m_pState, index, value));
	}
	inline void outputError(void){
		const char* msg = lua_tostring( m_pState, -1 );
		if( msg == NULL )
			msg = "(error without message)";
		LOG_ERROR("lua error:%s\n", msg);
	}
	inline bool checkUpdateFlag(void){
		bool result = !m_update_flag.test_and_set();
		return result;
	}
	inline void clearUpdateFlag(void){
		m_update_flag.clear();
	}
protected:
	lua_State* m_pState;
	std::atomic_flag m_update_flag;
};//end class Script

NS_HIVE_END

#endif

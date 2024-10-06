
local config = class("config")

config.logLevel = "debug"	-- { "trace", "debug", "info", "warn", "error", "fatal" }

-- 设置C++日志打印等级
--	#define LOG_LEVEL_DEBUG 0
--	#define LOG_LEVEL_INFO 1
--	#define LOG_LEVEL_ERROR 2
config.cppLogLevel = 0

config.handlerFile = "logic"

-- 热更新的key，匹配才可以发起更新
config.hotUpdateKey = "e3A7f341aF0c0b328c945B4fc3946270"

config.requirePath = {
    
}

return config

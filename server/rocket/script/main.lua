--
-- Created by IntelliJ IDEA.
-- User: AppleTree
-- Date: 2017/5/20
-- Time: 下午1:55
-- To change this template use File | Settings | File Templates.
--

local _G = _G
local os = os
local io = io
local type = type
local rawset = rawset
local rawget = rawget
local tostring = tostring
local debug = debug
local error_function = print
local print_error = function(msg, traceMsg)
	error_function("----------------------------------------")
	error_function("LUA ERROR: " .. tostring(msg) .. "\n")
	error_function(traceMsg)
	error_function("----------------------------------------")
end
local register_error
function set_register_error_function(callback)
	register_error = callback
end

-- traceback
function __G__TRACKBACK__(msg)
	local traceMsg = debug.traceback()
	print_error(msg, traceMsg)
	if register_error ~= nil then
		return register_error(msg, traceMsg)
	end
end
local __G__TRACKBACK__ = __G__TRACKBACK__
local xpcall = xpcall

-- 预定义
local rpc
local g_pNode = g_pNode
local djb_hash = djb_hash
local getTimeStringUS = getTimeStringUS
local setLogLevel = setLogLevel
local pManager = g_pNode:getManager()
local h = pManager:getServerHandle()
local t = getTimeStringUS()
local arr = { t, h }
local str = table.concat(arr, "_")
local seed = djb_hash(str, #str)
local seed_str = string.format("index=%s current seed %s", h, seed)
math.randomseed(seed)

local require_path = {
	"base";
	"pb";
}
local function initEnvironment()
	-- 初始化环境目录参数
	local result,env = pcall(require, "server_env")
	if not result then
		env = "online"
	end
	table.insert(require_path, env)
	-- 设置文件加载环境
	local package = package
	package.path = "./?.lua;./script/?.lua;"
	package.cpath = "./?.so;./lib/?.so;"
	for _,p in ipairs(require_path) do
		p = "script/"..p
		package.path = package.path .. "./"..p.."/?.lua;"
		package.cpath = package.cpath .. "./"..p.."/?.so;"
	end

	-- 设置C++日志打印等级
--	#define LOG_LEVEL_DEBUG 0
--	#define LOG_LEVEL_INFO 1
--	#define LOG_LEVEL_ERROR 2
	setLogLevel(0)
	-- 设置Lua日志打印等级
	local log = require("log")
	log.level = "debug"

	log_info("main current environment", env)
	log_info("main current random seed", seed_str)

	error_function = log_error

	-- 初始化加载模块
	for _,p in ipairs(require_path) do
		p = "script."..p..".init"
		pcall(require, p)
	end

	rpc = require("rpc")
end

local function checkForbidFunction()
	-- ============== 重置系统函数库，不支持上层调用 =====================
	local call_global_pwd = "e3A7f341aF0c0b328c945B4fc3946270"
	-- 这里只能执行一次，所以需要一个全局变量记录是否已经执行
	local g_isForbidFunction = rawget(_G, "g_isForbidFunction")
	if not g_isForbidFunction then
		rawset(_G, "g_isForbidFunction", true)
		local formNewFunction = function(name)
			local func = function(...)
				log_error(name, "is not supported:", ...)
			end
			return func
		end
		-- os.exit -> g_os_exit
		local changeFunction = function(obj, objName, name)
			local func = obj[name]
			obj[name] = formNewFunction(objName.."."..name)
			local g_func = function(pwd, ...)
				if pwd == call_global_pwd then
					func(...)
				end
			end
			-- 记录全局可用函数
			rawset(_G, "g_"..objName.."_"..name, g_func)
		end
		-- 屏蔽部分os方法
		local nameArray = {"exit"; "execute"; "remove"; "rename"; "tmpname";}
		for _,name in pairs(nameArray) do
			changeFunction(os, "os", name)
		end
		-- 屏蔽所有io的方法
		local ioNameArray = {}
		for k,v in pairs(io) do
			if type(v) == "function" then
				table.insert(ioNameArray, k)
			end
		end
		for _,name in pairs(ioNameArray) do
			changeFunction(io, "io", name)
		end
	end
	-- ===================================================================
end

function onTimerUpdate(callbackID, timerHandle)
	local timeCount = -1
	xpcall(function()
		timeCount = rpc:onTimerUpdate(callbackID, timerHandle)
	end, __G__TRACKBACK__)
	return (timeCount or -1)
end
function onOpenConnect(handle, serverHandle, callbackID)
	return xpcall(function() return rpc:onOpenConnect(handle, serverHandle, callbackID) end, __G__TRACKBACK__)
end
function onCloseConnect(handle, serverHandle, callbackID)
	return xpcall(function() return rpc:onCloseConnect(handle, serverHandle, callbackID) end, __G__TRACKBACK__)
end
function onHandleBuffer(sourceServerIndex, buffer)
	return xpcall(function() return rpc:onHandleBuffer(sourceServerIndex, buffer) end, __G__TRACKBACK__)
end
function onReceiveWebSocket(handle, serverHandle, ip, buffer)
	return xpcall(function() return rpc:onReceiveWebSocket(handle, serverHandle, ip, buffer) end, __G__TRACKBACK__)
end
function onReceiveHttp(handle, serverHandle, ip, method, url, body, headers)
	return xpcall(function() return rpc:onReceiveHttp(handle, serverHandle, ip, method, url, body, headers) end, __G__TRACKBACK__)
end
function onHttpResponse(handle, serverHandle, callbackID, statusCode, isRequestOK, body, headers)
	return xpcall(function() return rpc:onHttpResponse(handle, serverHandle, callbackID, statusCode, isRequestOK, body, headers) end, __G__TRACKBACK__)
end
function onNextFrameActive()
	return xpcall(function() return rpc:onNextFrameActive() end, __G__TRACKBACK__)
end
function onInitialize(pNode, moduleName)
	local p = "script/"..moduleName
	package.path = package.path .. "./"..p.."/?.lua;"
	package.cpath = package.cpath .. "./"..p.."/?.so;"
	p = "script."..moduleName..".init"
	pcall(require, p)
	log_info("onInitialize", moduleName, "package.path", package.path)
	-- 设置config中需要加入到环境的路径
	local config = require("config")
	table.insert(require_path, moduleName)
	config.base_require_path = require_path
	-- 设置Lua日志打印等级
	local log = require("log")
	log.level = config.logLevel or "trace"
	-- 设置C++日志打印等级
	--	#define LOG_LEVEL_DEBUG 0
	--	#define LOG_LEVEL_INFO 1
	--	#define LOG_LEVEL_ERROR 2
	local cppLogLevel = config.cppLogLevel or 0
	setLogLevel(cppLogLevel)
	return xpcall(function() return rpc:onInitialize(pNode, moduleName) end, __G__TRACKBACK__)
end

-- 禁用一些函数
checkForbidFunction()
-- 初始化环境变量配置
initEnvironment()



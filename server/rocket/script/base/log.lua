--
-- Created by IntelliJ IDEA.
-- User: AppleTree
-- Date: 2017/5/25
-- Time: 下午10:50
-- To change this template use File | Settings | File Templates.
--

local ipairs = ipairs
local select = select
local type = type
local math = math
local table = table
local tostring = tostring
local debug = debug
local string = string
local _G = _G
local rawset = rawset
local rawget = rawget
local pcall = pcall
local io = io
local os = os
local getTimeStringUS = getTimeStringUS
local getCppLogFileName = getCppLogFileName
local writeRemoteLog = writeRemoteLog
local maxinteger = 9007199254740991
local mininteger = -maxinteger

local dump = require("dump")
local rapidjson = require("rapidjson")

local log = rawget(_G, "log") or {}

log.level = log.level or "trace"
function log:resetFileName()
	log.fileName = log.fileName or getCppLogFileName()
end
log:resetFileName()

local modes = { "trace", "debug", "info", "warn", "error", "fatal" }

function log.dispatch(callback)
	log.callback = callback
end

local levels = {}
for i,name in ipairs(modes) do
	levels[name] = i
end

local log_tostring = function(...)
	local t = {}
	local total = select('#', ...)
	for i = 1, total do
		local x = select(i, ...)
		local tx = type(x)
		if tx == "table" then
			local ret,s = pcall(rapidjson.encode, x)
			if not ret then
				s = dump(x, 10, true, nil)
				s = "\n"..s.."\n"
			end
			table.insert(t, s)
		elseif tx == "number" then
			if math.floor(x) == x and x < maxinteger and x > mininteger then
				table.insert(t, string.format("%d", x))
			else
				table.insert(t, string.format("%f", x))
			end
		elseif tx == "string" then
			table.insert(t, x)
		else
			table.insert(t, tostring(x))
		end
	end
	return table.concat(t, " ")
end

for i,name in ipairs(modes) do
	local nameupper = name:upper()
	log[name] = function(...)
		-- Return early if we're below the log level
		if i < levels[log.level] then
			return
		end

		local msg = log_tostring(...)
		local info = debug.getinfo(2, "Sl")
		local lineinfo = info.short_src .. ":" .. info.currentline
		local str = string.format("[%-6s%s][%s] %s\n", nameupper, getTimeStringUS(), lineinfo, msg)

		-- Output to log file
		local callback = log.callback
		if callback then
			callback(str)
		else
			writeRemoteLog(log.fileName, str)
		end
	end
	rawset(_G, "log_"..name, log[name])
end

rawset(_G, "log", log)

return log

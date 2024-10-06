--
-- Created by IntelliJ IDEA.
-- User: AppleTree
-- Date: 16/11/26
-- Time: 下午10:51
-- To change this template use File | Settings | File Templates.
--

local type = type
local pairs = pairs
local ipairs = ipairs
local os = os
local math = math
local table = table
local tonumber = tonumber
local string = string
local next = next

local util = {}

local function getExtentionIndex(fileName)
	local length = #fileName
	local index
	for k=length,1,-1 do
		local c = string.sub(fileName, k, k)
		if c == "." then
			index = k
			break
		end
	end
	return index
end
local function splitFileNameExtention(fileName)
	local length = #fileName
	local index = getExtentionIndex(fileName)
	if not index then
		return
	end
	local name = string.sub(fileName, 1, index - 1)
	local ext = string.sub(fileName, index, length)
	return name,ext
end
local function splitFileName(fileName)
	local index = getExtentionIndex(fileName)
	if not index then
		return fileName
	end
	local name = string.sub(fileName, 1, index - 1)
	return name
end
local function splitExtention(fileName)
	local length = #fileName
	local index = getExtentionIndex(fileName)
	if not index then
		return
	end
	local ext = string.sub(fileName, index, length)
	return ext
end
util.getExtentionIndex = getExtentionIndex
util.splitFileNameExtention = splitFileNameExtention
util.splitFileName = splitFileName
util.splitExtention = splitExtention

local function toHex(number)
	return string.format("0x%x", number)
end
util.toHex = toHex

local shuffle = function(tb)
	if not tb then return end
	local cnt = #tb
	if cnt <= 1 then
		return
	end
	for i=1,cnt do
		local j = math.random(i,cnt)
		tb[i],tb[j] = tb[j],tb[i]
	end
end
util.shuffle = shuffle

local copy = function(res)
	local lookup_table = {}
	local function _copy(object)
		if type(object) ~= "table" then
			return object
		elseif lookup_table[object] then
			return lookup_table[object]
		end
		local new_table = {}
		lookup_table[object] = new_table
		for key, value in pairs(object) do
			new_table[_copy(key)] = _copy(value)
		end
		return new_table
	end
	return _copy(res)
end
util.copy = copy
local copyTo = function(res, to)
	local lookup_table = {}
	local function _copy(object, to_object)
		if type(object) ~= "table" then
			return object
		elseif lookup_table[object] then
			return lookup_table[object]
		end
		to_object = to_object or {}
		-- 处理数组的拷贝情况，先清空 to_object 多余的数据
		local objLen = #object
		local toObjLen = #to_object
		if objLen < toObjLen then
			for k=objLen+1,toObjLen do
				to_object[k] = nil
			end
		end
		lookup_table[object] = to_object
		for key, value in pairs(object) do
			to_object[_copy(key)] = _copy(value, to_object[key])
		end
		return to_object
	end
	return _copy(res, to)
end
util.copyTo = copyTo
local rawCopyTo = function(res, to)
	local function _copy(object, to_object)
		if type(object) ~= "table" then
			return object
		end
		to_object = to_object or {}
		-- 处理数组的拷贝情况，先清空 to_object 多余的数据
		local objLen = #object
		local toObjLen = #to_object
		if objLen < toObjLen then
			for k=objLen+1,toObjLen do
				to_object[k] = nil
			end
		end
		for key, value in pairs(object) do
			to_object[_copy(key)] = _copy(value, to_object[key])
		end
		return to_object
	end
	return _copy(res, to)
end
util.rawCopyTo = rawCopyTo
local dataBaseCopyTo = function(res, to)
	local function _copy(object, to_object)
		if type(object) ~= "table" then
			return object
		end
		-- 有一些数据字段拷贝的时候，是同一张表，不需要执行拷贝
		if object == to_object then
			return to_object
		end
		-- 处理更新的 object 为空表的情况，在这种情况下，不再继续深入拷贝，而是直接赋值，删除目的地表内部数值
		if next(object) == nil then
			return object
		end
		to_object = to_object or {}
		-- 处理数组的拷贝情况，先清空 to_object 多余的数据
		local objLen = #object
		local toObjLen = #to_object
		if objLen < toObjLen then
			for k=objLen+1,toObjLen do
				to_object[k] = nil
			end
		end
		for key, value in pairs(object) do
			to_object[key] = _copy(value, to_object[key])
		end
		return to_object
	end
	return _copy(res, to)
end
util.dataBaseCopyTo = dataBaseCopyTo

local split = function(value, mark)
	local arr = {}
	local fromn = 1
	local token
	local startn, endn = string.find(value, mark, fromn)
	while startn ~= nil do
		token = string.sub( value, fromn, startn-1 )
		table.insert(arr, token)
		fromn = endn + 1
		startn, endn = string.find(value, mark, fromn)
	end
	token = string.sub( value, fromn, #value )
	table.insert(arr, token)
	return arr
end
util.split = split
local splitNoEmpty = function(value, mark)
	local arr = {}
	local fromn = 1
	local token
	local startn, endn = string.find(value, mark, fromn)
	while startn ~= nil do
		if fromn < startn then
			token = string.sub( value, fromn, startn-1 )
			table.insert(arr, token)
		end
		fromn = endn + 1
		startn, endn = string.find(value, mark, fromn)
	end
	if fromn < #value then
		token = string.sub( value, fromn, #value )
		table.insert(arr, token)
	end
	return arr
end
util.splitNoEmpty = splitNoEmpty
-- 文件偏移从0开始，Lua数组从1开始，所以在计算结果出来之后需要换算减1
local splitFileOffset = function(value, mark)
	local arr = {}
	local fromn = 1
	local token
	local startn, endn = string.find(value, mark, fromn)
	while startn ~= nil do
		token = string.sub( value, fromn, startn-1 )
		local info = {
			token = token;
			offset = fromn - 1;
		}
		table.insert(arr, info)
		fromn = endn + 1
		startn, endn = string.find(value, mark, fromn)
	end
	token = string.sub( value, fromn, #value )
	local info = {
		token = token;
		offset = fromn - 1;
	}
	table.insert(arr, info)
	return arr
end
util.splitFileOffset = splitFileOffset
local function url_encode(s)
	s = string.gsub(s, "([^%w%.%- ])", function(c) return string.format("%%%02X", string.byte(c)) end)
	return string.gsub(s, " ", "+")
end
util.url_encode = url_encode
local function url_decode(s)
	s = string.gsub(s, '%%(%x%x)', function(h) return string.char(tonumber(h, 16)) end)
	return s
end
util.url_decode = url_decode
local parse_url = function(url)
	local params = {}
	if url == nil then
		return params
	end
	local _,pose = string.find(url, "?")
	if pose and pose + 1 < #url then
		local param_str = string.sub(url, pose+1, #url)
		local params_pair = split(param_str, "&")
		for _,p in ipairs(params_pair) do
			local kv = split(p, "=")
			if #kv == 2 then
				params[kv[1]] = url_decode(kv[2])
			end
		end
	end
	return params
end
util.parse_url = parse_url

local count = function(tab)
	local c = 0
	for _,_ in pairs(tab) do
		c = c + 1
	end
	return c
end
util.count = count
local isEmpty = function(tab)
	if tab == nil or type(tab) ~= "table" then
		return true
	end

	for _,_ in pairs(tab) do
		return false
	end
	return true
end
util.isEmpty = isEmpty

-- 该方法可以将中间有nil值的参数完全解开
local function param_unpack(t, s)
	s = s or 1
	local max = 0
	for i,_ in pairs(t) do
		if i > max then
			max = i
		end
	end
	local function up(t, i)
		if i < max then
			return t[i],up(t,i+1)
		else
			return t[i]
		end
	end
	return up(t,s)
end
util.unpack = param_unpack

local merge = function(des, res)
	local lookup_table = {}
	local _merge, _mergeTable
	_mergeTable = function(des_table, res_table)
		for k, v in pairs(res_table) do
			if type(k) == "table" then
				_merge(des_table, copy(k), v)
			else
				_merge(des_table, k, v)
			end
		end
	end
	_merge = function(dest, key, value)
		if type(value) ~= "table" then
			dest[key] = value
			return
		elseif lookup_table[value] then
			dest[key] = lookup_table[value]
			return
		end
		local dest_table = dest[key]
		if dest_table == nil or type(dest_table) ~= "table" then
			dest_table = {}
			dest[key] = dest_table
		end
		lookup_table[value] = dest_table
		_mergeTable(dest_table, value)
	end
	_mergeTable(des, res)
end
util.merge = merge

local checkType = function(obj, objtype)
	if type(objtype) ~= "string" then
		return false
	end

	if type(obj) ~= objtype then
		return false
	end

	return true
end

util.checkType = checkType

local getFileNameOrPath = function( strpath, bname)  
    local ts = string.reverse(strpath)  
    local _, param2 = string.find(ts, "/")  -- 这里以"/"为例  
	local len = string.len(strpath) 
    local m = len - param2 + 1     
    local result  
    if bname then  
        result = string.sub(strpath, m+1, len)   
    else  
        result = string.sub(strpath, 1, m-1)   
    end  
  
    return result  
end  
util.getFileNameOrPath = getFileNameOrPath

local existArrayValue = function (tb, value)
	if not checkType(tb, "table") then
		return
	end

	for i,v in ipairs(tb) do
		if v == value then
			return true, i
		end
	end
end
util.existArrayValue = existArrayValue

local existHashValue = function (tb, value)
	if not checkType(tb, "table") then
		return
	end

	for k,v in pairs(tb) do
		if v == value then
			return true, k
		end
	end
end
util.existHashValue = existHashValue

local function get_timestamp(start_date, start_time)
	local date_arr = util.split(start_date, "-")
	local time_arr = util.split(start_time, ":")
	local year = tonumber(date_arr[1])
	local month = tonumber(date_arr[2])
	local day = tonumber(date_arr[3])
	local hour = tonumber(time_arr[1])
	local min = tonumber(time_arr[2])
	local sec = tonumber(time_arr[3])
	local new_t = { year=year, month=month, day=day, hour=hour, min=min, sec=sec }
	local active_time = os.time(new_t)
	local today_second = hour*3600 + min*60 + sec
	return active_time,today_second
end
util.get_timestamp = get_timestamp
local function get_today_seconds(start_time)
	local time_arr = util.split(start_time, ":")
	local hour = tonumber(time_arr[1])
	local min = tonumber(time_arr[2])
	local sec = tonumber(time_arr[3])
	local today_second = hour*3600 + min*60 + sec
	return today_second
end
util.get_today_seconds = get_today_seconds
local php_week_to_lua_wday = {
	[1] = 2;
	[2] = 3;
	[3] = 4;
	[4] = 5;
	[5] = 6;
	[6] = 7;
	[7] = 1;
}
util.php_week_to_lua_wday = php_week_to_lua_wday

return util

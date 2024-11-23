--
-- Created by IntelliJ IDEA.
-- User: AppleTree
-- Date: 15/4/10
-- Time: 下午2:28
-- To change this template use File | Settings | File Templates.
--

local _G = _G
local string = string
local pairs = pairs
local ipairs = ipairs
local table = table
local type = type
local unpack = unpack
local os = os
local tostring = tostring
local log_debug = log_debug
local log_info = log_info
local log_error = log_error
local queue = require("queue")

log_debug("require module libmysql")
--require("libmysql")
local DBMysql = DBMysql

local mysql = class("mysql")
-- 创建使用new，销毁使用dtor函数
function mysql:ctor(skipTimer)
	-- 设定计时器
	local m_pDBMysql = DBMysql()
	self.m_pDBMysql = m_pDBMysql
	if not skipTimer then
        log_debug("mysql start timer check")
        --local timer = require("timer")
        --self.m_coInfo = timer:schedule(function()
        --    self:tryPing()
        --    self:checkToSave()
        --end, 10000, true)
	end
	self.m_hash_queue = queue.new()
end
function mysql:dtor()
	self:release()
	--local m_coInfo = self.m_coInfo
	--if m_coInfo ~= nil then
	--	self.m_coInfo = nil
     --   local timer = require("timer")
	--	timer:unschedule(m_coInfo)
	--end
	self.m_hash_queue = nil
end
function mysql:getDataBaseName()
	local m_connParam = self.m_connParam
	if m_connParam == nil then
		return
	end
	return m_connParam[4]
end
function mysql:isAvailable()
    return self.m_pDBMysql:isAvailable()
end
function mysql:release()
	local m_pDBMysql = self.m_pDBMysql
	if m_pDBMysql ~= nil then
		m_pDBMysql:closeMysql()
		self.m_pDBMysql = nil
        log_info("mysql close mysql", table.concat(self.m_connParam, " "))
    else
        log_info("mysql m_pDBMysql==nil", table.concat(self.m_connParam, " "))
	end
end
function mysql:tryPing()
--	log_debug("tryPing processing 1")
	self:checkReconnect()
--	log_debug("tryPing processing 2")
	local m_pDBMysql = self.m_pDBMysql
	local isAvaliable = m_pDBMysql:isAvailable()
	if not isAvaliable then
		log_error("pingMysql not avaliable", isAvaliable)
		return
	end
--	log_debug("tryPing m_pDBMysql isAvaliable", isAvaliable)
	local result = m_pDBMysql:pingMysql()
--	log_debug("tryPing pingMysql result", result)
	if not result then
		log_debug("pingMysql failed result", result)
		return self:checkReconnect()
	end
end
function mysql:checkReconnect()
	local m_pDBMysql = self.m_pDBMysql
	local isAvaliable = m_pDBMysql:isAvailable()
	if not isAvaliable then
		log_debug("m_pDBMysql isAvailable", isAvaliable)
		local m_connParam = self.m_connParam
		if m_connParam ~= nil then
            self.m_connParam = nil
			log_debug("checkReconnect processing")
			return self:connect(unpack(m_connParam))
		else
			log_error("checkReconnect m_connParam is nil")
		end
		return
	end
end
function mysql:checkToSave()
	-- 把缓存的数据全部保存
	local m_hash_queue = self.m_hash_queue
	local sqlArr = m_hash_queue:copyArray()
	if #sqlArr > 0 then
		if self:executeQuery(sqlArr, true) then
			m_hash_queue:clear()
		end
	end
end
function mysql:isEqual(newParam, oldParam)
	if oldParam == nil then
		return false
	end
	for k,v in pairs(newParam) do
		if oldParam[k] ~= v then
			return false
		end
	end
	return true
end
-- 公共方法
function mysql:connect(ip, user, pwd, db, port)
    local newParam = {ip, user, pwd, db, port }
    log_debug("mysql connect", newParam)
    local oldParam = self.m_connParam
    local m_pDBMysql = self.m_pDBMysql
    if self:isEqual(newParam, oldParam) then
		log_debug("current equal param, no connect processing")
        return m_pDBMysql:isAvailable()
	end
	log_debug("mysql connect processing", newParam)
    self.m_connParam = newParam
	local result = m_pDBMysql:connectMySql(ip, user, pwd, db, port)
	log_debug("connectMySql", result)
	if not result then
		log_error("connect mysql failed", ip, user, pwd, db, port, "result", result)
	else
		local sqlArr = { "SET NAMES utf8mb4;" }
		local resultArr = self:executeQuery(sqlArr, true)
		log_debug("connect mysql OK", ip, user, pwd, db, port, "SET NAMES utf8mb4", resultArr)
	end
	return result
end
-- 查询mysql语句，skipResult为false或者nil则直接执行
function mysql:query(sql, skipResult)
	if skipResult then
		return self:queryHash(sql)
	else
		return self:executeQuery({sql}, skipResult)
	end
end
-- 请求操作不会立即执行，而是缓存到一定量，或者超时之后执行
function mysql:queryHash(sql)
	local m_hash_queue = self.m_hash_queue
	m_hash_queue:push(sql)
	if m_hash_queue:count() > 100 then
		self:checkToSave()
	end
	return true
end
-- 获取上一个操作自增的ID值
function mysql:getLastAutoID()
	return self.m_pDBMysql:getLastAutoID()
end
-- 对数据进行转码操作
function mysql:escape(s)
	return self.m_pDBMysql:escape(s)
end
-- 更新一条数据
function mysql:update(table_name, id_name, data, skipResult)
	local sql = self:getUpdateSql(table_name, id_name, data)
--	log_debug("update sql:", sql)
	if skipResult then
		return self:queryHash(sql)
	end
	return self:executeQuery({sql}, skipResult)
end
-- 更新多条数据
function mysql:mupdate(tableArr, skipResult)
	if skipResult then
		for _,info in ipairs(tableArr) do
			local table_name = info.table_name
			local id_name = info.id_name
			local dataArr = info.data_arr
			for _,data in ipairs(dataArr) do
				local sql = self:getUpdateSql(table_name, id_name, data)
--				log_debug("mupdate sql:", sql)
				self:queryHash(sql)
			end
		end
		return true
	end
	local sqlArr = {}
	for _,info in ipairs(tableArr) do
		local table_name = info.table_name
		local id_name = info.id_name
		local dataArr = info.data_arr
		for _,data in ipairs(dataArr) do
			local sql = self:getUpdateSql(table_name, id_name, data)
			table.insert(sqlArr, sql)
		end
	end
	return self:executeQuery(sqlArr, skipResult)
end
-- 插入一条数据
function mysql:insert(table_name, data, skipResult)
	local sql = self:getInsertSql(table_name, data)
	if skipResult then
		return self:queryHash(sql)
	end
	return self:executeQuery({sql}, skipResult)
end
-- 插入多条数据
function mysql:minsert(tableArr, skipResult)
	if skipResult then
		for _,info in ipairs(tableArr) do
			local table_name = info.table_name
			--			local id_name = info.id_name
			local dataArr = info.data_arr
			for _,data in ipairs(dataArr) do
				local sql = self:getInsertSql(table_name, data)
				self:queryHash(sql)
			end
		end
		return true
	end
	local sqlArr = {}
	for _,info in ipairs(tableArr) do
		local table_name = info.table_name
		--			local id_name = info.id_name
		local dataArr = info.data_arr
		for _,data in ipairs(dataArr) do
			local sql = self:getInsertSql(table_name, data)
			table.insert(sqlArr, sql)
		end
	end
	return self:executeQuery(sqlArr, skipResult)
end
-- 插入一条数据
function mysql:replace(table_name, data, skipResult)
    local sql = self:getReplaceSql(table_name, data)
    if skipResult then
        return self:queryHash(sql)
    end
    return self:executeQuery({sql}, skipResult)
end
-- 插入多条数据
function mysql:mreplace(tableArr, skipResult)
    if skipResult then
        for _,info in ipairs(tableArr) do
            local table_name = info.table_name
            --			local id_name = info.id_name
            local dataArr = info.data_arr
            for _,data in ipairs(dataArr) do
                local sql = self:getReplaceSql(table_name, data)
                self:queryHash(sql)
            end
        end
        return true
    end
    local sqlArr = {}
    for _,info in ipairs(tableArr) do
        local table_name = info.table_name
        --			local id_name = info.id_name
        local dataArr = info.data_arr
        for _,data in ipairs(dataArr) do
            local sql = self:getReplaceSql(table_name, data)
            table.insert(sqlArr, sql)
        end
    end
    return self:executeQuery(sqlArr, skipResult)
end
-- 查询一条数据
function mysql:select(table_name, keyName, keyValue, columnNames)
	local sql = self:getSelectSql(table_name, keyName, keyValue, columnNames)
	return self:executeQuery({sql}, false)
end
function mysql:selectByKeyMap(table_name, keyMap, columnNames)
	local sql = self:getSelectSqlByKeyMap(table_name, keyMap, columnNames)
	return self:executeQuery({sql}, false)
end
-- 查询多条数据
function mysql:mselect(queryArr)
	local sqlArr = {}
	for _,query in ipairs(queryArr) do
		local table_name = query.table_name
		local keyName = query.key_name
		local keyValue = query.key_value
		local columnNames = query.column_names
		local sql = self:getSelectSql(table_name, keyName, keyValue, columnNames)
		table.insert(sqlArr, sql)
	end
	return self:executeQuery(sqlArr, false)
end
-- 删除一条数据
function mysql:delete(table_name, keyName, keyValue, skipResult)
	local sql = self:getDeleteSql(table_name, keyName, keyValue)
	return self:executeQuery({sql}, skipResult)
end
-- 删除多条数据
function mysql:mdelete(tableArr, skipResult)
	if skipResult then
		for _,info in ipairs(tableArr) do
			local table_name = info.table_name
			local keyName = info.key_name
			local valueArr = info.value_arr
			for _,keyValue in ipairs(valueArr) do
				local sql = self:getDeleteSql(table_name, keyName, keyValue)
				self:queryHash(sql)
			end
		end
		return true
	end
	local sqlArr = {}
	for _,info in ipairs(tableArr) do
		local table_name = info.table_name
		local keyName = info.key_name
		local valueArr = info.value_arr
		for _,keyValue in ipairs(valueArr) do
			local sql = self:getDeleteSql(table_name, keyName, keyValue)
			table.insert(sqlArr, sql)
		end
	end
	return self:executeQuery(sqlArr, skipResult)
end
-- 多个操作数据发送过来
function mysql:moperate(operateArr)
	-- 操作类型，1新加； 2更新； 3删除
	local sqlArr = {}
	for _,op in ipairs(operateArr) do
		local sql
		local opType = op[1]
		if opType == 1 then
			local table_name = op[2]
			local data = op[3]
			sql = self:getReplaceSql(table_name, data)
		elseif opType == 2 then
			local table_name = op[2]
			local data = op[3]
			sql = self:getReplaceSql(table_name, data)
		elseif opType == 3 then
			local table_name = op[2]
			local keyName = op[3]
			local keyValue = op[4]
			sql = self:getDeleteSql(table_name, keyName, keyValue)
		end
		if sql then
			table.insert(sqlArr, sql)
		end
	end
	return self:executeQuery(sqlArr, false)
end
-- 多个操作，使用原始操作完成：新增（insert）、更新（update）、删除（delete）
function mysql:moperateOrigin(operateArr)
	-- 操作类型，1新加； 2更新； 3删除
	local sqlArr = {}
	for _,op in ipairs(operateArr) do
		local sql
		local opType = op[1]
		if opType == 1 then
			local table_name = op[2]
			local data = op[3]
			sql = self:getInsertSql(table_name, data)
		elseif opType == 2 then
			local table_name = op[2]
			local data = op[3]
			local id_name = op[4]
			sql = self:getUpdateSql(table_name, id_name, data)
		elseif opType == 3 then
			local table_name = op[2]
			local keyName = op[3]
			local keyValue = op[4]
			sql = self:getDeleteSql(table_name, keyName, keyValue)
		end
		if sql then
			table.insert(sqlArr, sql)
		end
	end
	return self:executeQuery(sqlArr, false)
end
-- 获取查询语句
function mysql:getUpdateSql(table_name, id_name, data)
	local m_pDBMysql = self.m_pDBMysql
	local id = data[id_name]
	local kvArr = {}
	local kv
	for k,v in pairs(data) do
        if k ~= id_name then
            local v_type = type(v)
            if v_type == "string" then
                v = m_pDBMysql:escape(v) or v     -- 需要确保字符串编码的安全
                v = "\""..v.."\""
			elseif v_type == "number" then
				v = string.format("%d", v)
            end
            kv = "`".. k .. "`=" .. v
            table.insert(kvArr, kv)
        end
	end
	local sql
	if type(id) == "number" then
		sql = "update `%s` set %s where `%s`=%d;"
	else
		sql = "update `%s` set %s where `%s`=\"%s\";"
	end
	local valuestr = table.concat(kvArr, ",")
	sql = string.format( sql, table_name, valuestr, id_name, id )
	return sql
end
function mysql:getInsertSql(table_name, data)
	local m_pDBMysql = self.m_pDBMysql
	local keys = {}
	local values = {}
	for k,v in pairs(data) do
		local v_type = type(v)
		table.insert(keys, "`"..k.."`")
		if v_type == "string" then
			v = m_pDBMysql:escape(v) or v    -- 需要确保字符串编码的安全
			v = "\""..v.."\""
		elseif v_type == "number" then
			v = string.format("%d", v)
		end
		table.insert(values, v)
	end
	local sql = "insert into `%s` (%s) values (%s);";
	local keystr = table.concat(keys, ",")
	local valuestr = table.concat(values, ",")
	sql = string.format( sql, table_name, keystr, valuestr )
	return sql
end
function mysql:getReplaceSql(table_name, data)
    local m_pDBMysql = self.m_pDBMysql
    local keys = {}
    local values = {}
    for k,v in pairs(data) do
        local v_type = type(v)
        table.insert(keys, "`"..k.."`")
        if v_type == "string" then
            v = m_pDBMysql:escape(v) or v    -- 需要确保字符串编码的安全
            v = "\""..v.."\""
		elseif v_type == "number" then
			v = string.format("%d", v)
        end
        table.insert(values, v)
    end
    local sql = "replace into `%s` (%s) values (%s);";
    local keystr = table.concat(keys, ",")
    local valuestr = table.concat(values, ",")
    sql = string.format( sql, table_name, keystr, valuestr )
    return sql
end
function mysql:getSelectSqlOther(path_name, keyName, keyValue, columnNames)
    local m_pDBMysql = self.m_pDBMysql
    local sql
    if columnNames == nil or #columnNames == 0 then
        sql = "select * from"
    else
        local columns = {}
        for _,name in ipairs(columnNames) do
            table.insert(columns, "`"..name.."`")
        end
        sql = "select " .. table.concat(columns, ",") .. " from"
    end
    local key_value_type = type(keyValue)
    if key_value_type == "nil" then
        sql = string.format("%s %s;", sql, path_name)
    elseif key_value_type == "number" then
        sql = string.format("%s %s where `%s`=%d;", sql, path_name, keyName, keyValue)
    else
        keyValue = m_pDBMysql:escape(keyValue) or keyValue    -- 需要确保字符串编码的安全
        sql = string.format("%s %s where `%s`=\"%s\";", sql, path_name, keyName, keyValue)
    end
    return sql
end
function mysql:getSelectSql(table_name, keyName, keyValue, columnNames)
    if string.find(table_name, "%.") ~= nil then
        return self:getSelectSqlOther(table_name, keyName, keyValue, columnNames)
    end
	local m_pDBMysql = self.m_pDBMysql
	local sql
	if columnNames == nil or #columnNames == 0 then
		sql = "select * from"
	else
		local columns = {}
		for _,name in ipairs(columnNames) do
			table.insert(columns, "`"..name.."`")
		end
		sql = "select " .. table.concat(columns, ",") .. " from"
	end
	local key_value_type = type(keyValue)
	if key_value_type == "nil" then
		sql = string.format("%s `%s`;", sql, table_name)
	elseif key_value_type == "number" then
		sql = string.format("%s `%s` where `%s`=%d;", sql, table_name, keyName, keyValue)
	else
		keyValue = m_pDBMysql:escape(keyValue) or keyValue    -- 需要确保字符串编码的安全
		sql = string.format("%s `%s` where `%s`=\"%s\";", sql, table_name, keyName, keyValue)
	end
	return sql
end
function mysql:getSelectSqlByKeyMapOther(table_name, keyMap, columnNames)
	keyMap = keyMap or {}
	local m_pDBMysql = self.m_pDBMysql
	local sql
	if columnNames == nil or #columnNames == 0 then
		sql = "select * from"
	else
		local columns = {}
		for _,name in ipairs(columnNames) do
			table.insert(columns, "`"..name.."`")
		end
		sql = "select " .. table.concat(columns, ",") .. " from"
	end
	local conditionArr = {}
	for keyName,keyValue in pairs(keyMap) do
		local key_value_type = type(keyValue)
		local cond
		if key_value_type == "number" then
			cond = string.format("`%s`=%d", keyName, keyValue)
		else
			keyValue = m_pDBMysql:escape(keyValue) or keyValue    -- 需要确保字符串编码的安全
			cond = string.format("`%s`=\"%s\"", keyName, keyValue)
		end
		table.insert(conditionArr, cond)
	end
	if #conditionArr == 0 then
		sql = string.format("%s %s;", sql, table_name)
	elseif #conditionArr == 1 then
		local condStr = conditionArr[1]
		sql = string.format("%s %s where %s;", sql, table_name, condStr)
	else
		local condStr = table.concat(conditionArr, " and ")
		sql = string.format("%s %s where %s;", sql, table_name, condStr)
	end
	return sql
end
function mysql:getSelectSqlByKeyMap(table_name, keyMap, columnNames)
	if string.find(table_name, "%.") ~= nil then
		return self:getSelectSqlByKeyMapOther(table_name, keyMap, columnNames)
	end
	keyMap = keyMap or {}
	local m_pDBMysql = self.m_pDBMysql
	local sql
	if columnNames == nil or #columnNames == 0 then
		sql = "select * from"
	else
		local columns = {}
		for _,name in ipairs(columnNames) do
			table.insert(columns, "`"..name.."`")
		end
		sql = "select " .. table.concat(columns, ",") .. " from"
	end
	local conditionArr = {}
	for keyName,keyValue in pairs(keyMap) do
		local key_value_type = type(keyValue)
		local cond
		if key_value_type == "number" then
			cond = string.format("`%s`=%d", keyName, keyValue)
		else
			keyValue = m_pDBMysql:escape(keyValue) or keyValue    -- 需要确保字符串编码的安全
			cond = string.format("`%s`=\"%s\"", keyName, keyValue)
		end
		table.insert(conditionArr, cond)
	end
	if #conditionArr == 0 then
		sql = string.format("%s `%s`;", sql, table_name)
	elseif #conditionArr == 1 then
		local condStr = conditionArr[1]
		sql = string.format("%s `%s` where %s;", sql, table_name, condStr)
	else
		local condStr = table.concat(conditionArr, " and ")
		sql = string.format("%s `%s` where %s;", sql, table_name, condStr)
	end
	return sql
end
function mysql:getDeleteSql(table_name, keyName, keyValue)
	local m_pDBMysql = self.m_pDBMysql
	local sql
	if type(keyValue) == "number" then
		sql = "delete from `%s` where `%s`=%d;";
	else
		keyValue = m_pDBMysql:escape(keyValue) or keyValue    -- 需要确保字符串编码的安全
		sql = "delete from `%s` where `%s`=\"%s\";";
	end
	sql = string.format( sql, table_name, keyName, keyValue )
	return sql
end
function mysql:executeQuery(sqlArr, skipResult)
	local queryNumber = #sqlArr
	if queryNumber == 0 then
		log_error("queryNumber == 0")
		return false
	end
	skipResult = skipResult or false
	local query = table.concat(sqlArr)
	local result = self.m_pDBMysql:execute(query, queryNumber, skipResult)
	log_debug("skipResult", skipResult, "queryNumber", queryNumber, query, "result", result)
	return result
end

return mysql


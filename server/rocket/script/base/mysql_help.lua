--
-- Created by IntelliJ IDEA.
-- User: zxt
-- Date: 2018/1/5
-- Time: 11:53
-- To change this template use File | Settings | File Templates.
--

local pairs = pairs
local ipairs = ipairs
local next = next
local table = table
local string = string
local type = type
local log_debug = log_debug
local log_error = log_error

local mysql_help = class("mysql_help")

function mysql_help:convert(resultArr)
--    local newArr = {}
--    for _,sqlResArr in ipairs(resultArr) do
----        local columns = sqlResArr[1]
--        local sqlData = {}
--        table.insert(newArr, sqlData)
--        for i=1,#sqlResArr do
--            local column_data = sqlResArr[i]
--            table.insert(sqlData, column_data)
--        end
--    end
--    return newArr
    return resultArr
end
-- 2021.4.6 底层C++传递的row数据，已经转换成key-value模式
function mysql_help:convertHash(resultArr, hashKeyName)
	local newArr = {}
	if type(hashKeyName) == "string" then
		for _,sqlResArr in ipairs(resultArr) do
			local sqlData = {}
			table.insert(newArr, sqlData)
			for i=1,#sqlResArr do
				local column_data = sqlResArr[i]
				local hashKey = column_data[hashKeyName]
				sqlData[hashKey] = column_data
			end
		end
	else
		for k,sqlResArr in ipairs(resultArr) do
			local sqlData = {}
			table.insert(newArr, sqlData)
			local keyName = hashKeyName[k]
			for i=1,#sqlResArr do
				local column_data = sqlResArr[i]
				local hashKey = column_data[keyName]
				sqlData[hashKey] = column_data
			end
		end
	end
	return newArr
end
function mysql_help:convertOld(resultArr)
    local newArr = {}
    for _,sqlResArr in ipairs(resultArr) do
        local columns = sqlResArr[1]
        local sqlData = {}
        table.insert(newArr, sqlData)
        for i=2,#sqlResArr do
            local arr = sqlResArr[i]
            local column_data = {}
            table.insert(sqlData, column_data)
            for j,v in pairs(arr) do
                local key = columns[j]
                column_data[key] = v
            end
        end
    end
    return newArr
end
function mysql_help:convertHashOld(resultArr, hashKeyName)
    local newArr = {}
    if type(hashKeyName) == "string" then
        for _,sqlResArr in ipairs(resultArr) do
            local columns = sqlResArr[1]
            local sqlData = {}
            table.insert(newArr, sqlData)
            for i=2,#sqlResArr do
                local arr = sqlResArr[i]
                log_debug("arr", arr)
                local column_data = {}
                for j,key in ipairs(columns) do
                    local value = arr[j]
                    column_data[key] = value
                end
--                for j,v in pairs(arr) do
--                    local key = columns[j]
--                    column_data[key] = v
--                end
                local hashKey = column_data[hashKeyName]
                sqlData[hashKey] = column_data
            end
        end
    else
        for k,sqlResArr in ipairs(resultArr) do
            local columns = sqlResArr[1]
            local sqlData = {}
            table.insert(newArr, sqlData)
            local keyName = hashKeyName[k]
            for i=2,#sqlResArr do
                local arr = sqlResArr[i]
                log_debug("arr", arr)
                local column_data = {}
                for j,key in ipairs(columns) do
                    local value = arr[j]
                    column_data[key] = value
                end
--                for j,v in pairs(arr) do
--                    local key = columns[j]
--                    column_data[key] = v
--                end
                local hashKey = column_data[keyName]
                sqlData[hashKey] = column_data
            end
        end
    end
    return newArr
end

function mysql_help:getTableSchema(mysqlObj, db_name, table_names)
    if #table_names == 0 then
        return {}
    end
    local column_names = { "column_name", "data_type", "character_maximum_length" }
    local table_name = "Information_schema.columns"
    local sqlArr = {}
    for _,name in ipairs(table_names) do
        local keyMap = {
            table_Name = name;
            table_Schema = db_name;
        }
        local sql = mysqlObj:getSelectSqlByKeyMap(table_name, keyMap, column_names)
        table.insert(sqlArr, sql)
    end
    local resultArr = mysqlObj:executeQuery(sqlArr)
    log_debug("getTableSchema", db_name, "sqlArr", sqlArr, "resultArr", resultArr)
--    local queryArr = {}
--    for _,name in ipairs(table_names) do
--        local query = {
--            table_name = table_name;
--            key_name = "table_Name";
--            key_value = name;
--            column_names = column_names;
--        }
--        table.insert(queryArr, query)
--    end
--    local resultArr = mysqlObj:mselect(queryArr)
    resultArr = mysql_help:convertHash(resultArr, "column_name")
    for _,sqlRes in ipairs(resultArr) do
        for _,info in pairs(sqlRes) do
            info.data_type = string.upper(info.data_type)
            if info.data_type == "CHAR" or info.data_type == "VARCHAR" then
                info.data_type = info.data_type.."("..info.character_maximum_length..")"
            end
        end
    end
    return resultArr
end
function mysql_help:compareTable(table_name, cfgInfo, dbInfo)
    local connectParam = function(info)
        local str = "`"..info.column_name.."` "..info.data_type
        if info.character then
            str = str .. " CHARACTER SET "..info.character
        end
        if info.auto_increment then
            str = str .. " AUTO_INCREMENT"
        end
        return str
    end
    -- 这是一个新创建的表
    if dbInfo == nil or next(dbInfo) == nil then
        local paramArr = {}
        local primary_key
        local cfgInfoArr = {}
        for _,info in pairs(cfgInfo) do
            table.insert(cfgInfoArr, info)
        end
        table.sort(cfgInfoArr, function(a, b) return a.index < b.index end)
        for _, info in ipairs(cfgInfoArr) do
            if info.primary_key then
                primary_key = info.column_name
            end
            table.insert(paramArr, connectParam(info))
        end
        local paramStr = table.concat(paramArr, ",")
        local sql
        if primary_key then
            sql = "create table `"..table_name.."` ("..paramStr..",PRIMARY KEY (`"..primary_key.."`));"
        else
            sql = "create table `"..table_name.."` ("..paramStr..");"
        end
        local sqlArr = { sql }
        return sqlArr
    end
    -- 查看对旧表是否有更新
    local change = {}
    local new = {}
    for k,info in pairs(cfgInfo) do
        if dbInfo[k] == nil then
            new[k] = connectParam(info)
        else
            local oldinfo = dbInfo[k]
            if info.data_type ~= oldinfo.data_type then
                change[k] = connectParam(info)
            end
        end
    end
    local sqlArr = {}
    for k,v in pairs(new) do
        local sql = "alter table `"..table_name.."` add "..v..";"
        table.insert(sqlArr, sql)
    end
    for k,v in pairs(change) do
        local sql = "alter table `"..table_name.."` modify "..v..";"
        table.insert(sqlArr, sql)
    end
    if #sqlArr == 0 then
        return nil
    end
    return sqlArr
end
function mysql_help:compareDB(db_name, cfgDB, curDB)
    local sqlArr = {}
    for table_name,cfgInfo in pairs(cfgDB) do
        local dbInfo = curDB[table_name]
        local arr = mysql_help:compareTable(table_name, cfgInfo, dbInfo)
        if arr then
            for _,sql in ipairs(arr) do
                table.insert(sqlArr, sql)
            end
        end
    end
    return sqlArr
end
function mysql_help:updateMysql(mysqlObj, db_name, cfgDB)
    local table_names = {}
    for table_name,_ in pairs(cfgDB) do
        table.insert(table_names, table_name)
    end
    if #table_names == 0 then
        log_debug("updateMysql no table found")
        return
    end
    local resultArr = mysql_help:getTableSchema(mysqlObj, db_name, table_names)
    local curDB = {}
    for index,table_name in ipairs(table_names) do
        curDB[table_name] = resultArr[index]
    end
--    log_debug("updateMysql table schema:", curDB)
    local sqlArr = mysql_help:compareDB(db_name, cfgDB, curDB)
    log_debug("updateMysql compare result:", sqlArr)
    if #sqlArr > 0 then
        local result = mysqlObj:executeQuery(sqlArr, false)
        log_debug("updateMysql update mysql result:", result)
    end
end

return mysql_help

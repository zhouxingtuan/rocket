--
-- Created by IntelliJ IDEA.
-- User: zxt
-- Date: 2018/1/4
-- Time: 14:49
-- To change this template use File | Settings | File Templates.
--

local TINYINT = "TINYINT"
local SMALLINT = "SMALLINT"
local INT = "INT"
local BIGINT = "BIGINT"
local FLOAT = "FLOAT"
local DOUBLE = "DOUBLE"
local VARCHAR = "VARCHAR"
local BLOB = "BLOB"

local protobuf_to_mysql = {
    double = DOUBLE;
    float = FLOAT;
    int32 = INT;
    int64 = BIGINT;
    uint32 = INT;
    uint64 = BIGINT;
    sint32 = INT;
    sint64 = BIGINT;
    fixed32 = INT;
    fixed64 = BIGINT;
    sfixed32 = INT;
    sfixed64 = BIGINT;
    bool = TINYINT;
    string = VARCHAR;
    bytes = BLOB;
}

local class = require("class")
local dump = require("dump")
local util = require("util")

local parser_sql = class("parser_sql")

function parser_sql:ctor(path, route)
    self.m_path = path
    self.m_route = route
    self.m_package = nil
    self.m_error_code = nil
    self.m_message_stack = {}
    self.m_full_message = {}
    self.m_current_message = {}
    self.m_final_message = {}
end
function parser_sql:transfer(file)
    local parser = require("parser")
    local obj = parser.new(self.m_path)
    local name,data = obj:getFileNameAndData(file)
    local lines = obj:splitLines(data)
    local linesArr = obj:splitEmptyMark(lines)
    self:findPackage(linesArr)
    if self.m_package == nil then
        print("can not find package name for file", file)
        return
    end
    self:parseLines(linesArr)
    local sql_buf = self:generateSQL()
    local save_name = "mysql_"..name..".lua"
    self:setFileData(save_name, sql_buf)
end
function parser_sql:parseLines(linesArr)
    for _,arr in ipairs(linesArr) do
        self:findAnyThingInLine(arr)
        if self.m_error_code ~= nil then
            return
        end
    end
end
function parser_sql:findAnyThingInLine(arr)
    local first = arr[1]
    first = first:lower()
    if first == "message" then
        self:pushMessage(arr[2], first)
    elseif first == "{" then
        -- message or enum begin
    elseif first == "}" then
        self:popMessage()
    elseif first == "required" then
        self:checkElementSingle(arr, 2)
    elseif first == "optional" then
        self:checkElementSingle(arr, 2)
    elseif first == "repeated" then
        self:checkElementArray(arr)
    elseif first == "map" then
        self:checkElementMap(arr)
    elseif first == "enum" then
        self:pushMessage(arr[2], first)
    elseif first == "package" then
        self.m_package = arr[2]
        print("package_name", self.m_package)
    else
        local info = self:topMessage()
        if info and info.type == "enum" then
            self:pushEnum(arr[1], arr[2])
        else
            local data_type = protobuf_to_mysql[first]
            if data_type then
                self:checkElementSingle(arr, 1)
            else
                local msg_info = self:findMessage(first)
                if msg_info then
                    self:checkElementSingle(arr, 1)
                else
                    print("current first string is not valid", first)
                end
            end
        end
    end
end
function parser_sql:checkElementSingle(arr, startIndex)
    local info = self:topMessage()
    local type_name = arr[startIndex]
    local column_name = arr[startIndex+1]
    local index = tonumber(arr[startIndex+2])
    local desc = arr[startIndex+3]
    if not index then
        print("current message", info.name, "index is not a number", type(index), index)
        self.m_error_code = 1
        return
    end
    -- find normal data type
    local data_type = protobuf_to_mysql[type_name]
    if data_type == nil then
        print("current message", info.name, "data_type can not found", type(index), index)
        self.m_error_code = 2
        return
    end
    local auto_increment
    if string.find(data_type, INT) and string.find(desc, "auto") then
        auto_increment = true
    end
    if data_type == "INT" then
        data_type = self:generateIntType(desc)
    end
    local character
    if type_name == "string" then
        data_type = self:generateStringType(desc)
        local character_map = {
            "utf8mb4";
            "utf8";
            "latin1";
            "gbk";
        }
        for _,cname in ipairs(character_map) do
            if string.find(desc, cname) then
                character = cname
                break
            end
        end
    elseif type_name == "bytes" then
        data_type = self:generateBytesType(desc)
    end
    self:pushElement(index, column_name, data_type, character, auto_increment)
end
function parser_sql:generateIntType(desc)
    local arr = util.split(desc, " ")
    local n
    for _,s in ipairs(arr) do
        n = tonumber(s)
        if n then
            break
        end
    end
    n = n or 4
    if n == 1 then
        return "TINYINT"
    elseif n == 2 then
        return "SMALLINT"
    elseif n == 3 then
        return "MEDIUMINT"
    elseif n == 4 then
        return "INT"
    end
end
function parser_sql:generateStringType(desc)
    local arr = util.split(desc, " ")
    local n
    for _,s in ipairs(arr) do
        n = tonumber(s)
        if n then
            break
        end
    end
    n = n or 128
    if n <= 32 then
        return "CHAR("..n..")"
    elseif n < 65536 then
        return "VARCHAR("..n..")"
    elseif n < 16777216 then
        return "MEDIUMTEXT"
    else
        return "LONGTEXT"
    end
end
function parser_sql:generateBytesType(desc)
    local arr = util.split(desc, " ")
    local n
    for _,s in ipairs(arr) do
        n = tonumber(s)
        if n then
            break
        end
    end
    n = n or 128
    if n < 256 then
        return "TINYBLOB"
    elseif n < 65536 then
        return "BLOB"
    elseif n < 16777216 then
        return "MEDIUMBLOB"
    else
        return "LONGBLOB"
    end
end
function parser_sql:checkElementArray(arr)
    print("we dont support array for mysql")
    self.m_error_code = 3
end
function parser_sql:checkElementMap(arr)
    print("we dont support map for mysql")
    self.m_error_code = 4
end
function parser_sql:pushEnum(key, value)
    local info = self:topMessage()
    value = tonumber(value)
    if not value then
        print("current enum", info.name, "value is not a number", type(value), value)
        self.m_error_code = 5
        return
    end
    -- check data
    if info.name_hash[key] ~= nil then
        print("current enum", info.name, "has the same name for key", key)
        self.m_error_code = 6
        return
    end
    if info.index_hash[value] ~= nil then
        print("current enum", info.name, "has the same index", key, value)
        self.m_error_code = 7
        return
    end
    info.name_hash[key] = value
    info.index_hash[value] = key
    local param = {key, value}
    table.insert(info.elements, param)
end
function parser_sql:pushElement(index, column_name, data_type, character, auto_increment)
    local info = self:topMessage()
    -- check data
    if info.name_hash[column_name] ~= nil then
        print("current message", info.name, "has the same name for key", column_name)
        self.m_error_code = 8
        return
    end
    if info.index_hash[index] ~= nil then
        print("current message", info.name, "has the same index", index)
        self.m_error_code = 9
        return
    end
    local param = {column_name, data_type}
    info.name_hash[column_name] = param
    info.index_hash[index] = param
    table.insert(info.elements, param)
    local param = { index=index, column_name=column_name, data_type=data_type, character=character, auto_increment=auto_increment, }
    if index == 1 then
        param.primary_key = true
    elseif index == 2 then
        param.group_key = true
    end
    info.elements_sql[column_name] = param
--    table.insert(info.elements_sql, param)
end
function parser_sql:pushMessage(name, type)
    local info = {
        type = type;
        name = name;
        elements = {};
        elements_sql = {};
        name_hash = {};
        index_hash = {};
        full_name = nil;
        path_name = nil;
    }
    self:setCurrentMessage(name, info)
    local m_message_stack = self.m_message_stack
    m_message_stack[#m_message_stack+1] = info
    if #m_message_stack > 1 then
        self.m_error_code = 10
        print("current message should be one level")
        return
    end
end
function parser_sql:popMessage()
    local m_message_stack = self.m_message_stack
    if m_message_stack[#m_message_stack] == nil then
        self.m_error_code = 11
        print("popMessage error current stack is empty")
        return
    end
    m_message_stack[#m_message_stack] = nil
end
function parser_sql:topMessage()
    local m_message_stack = self.m_message_stack
    return m_message_stack[#m_message_stack]
end
function parser_sql:setCurrentMessage(name, info)
    local full_name,path_name,table_name = self:getFullName(name, false)
    info.full_name = full_name
    info.path_name = path_name
    if info.type == "message" then
        self.m_full_message[full_name] = info.elements
    end
    self.m_current_message[path_name] = info
    self.m_final_message[table_name] = info.elements_sql
end
function parser_sql:getCurrentMessage()
    local full_name,path_name = self:getFullName(nil, false)
    return self.m_current_message[path_name]
end
function parser_sql:getFullMessage()
    local full_name,path_name = self:getFullName(nil, false)
    return self.m_full_message[full_name]
end
function parser_sql:findMessage(name)
    local full_name,path_name = self:getFullName(name, true)
    return self.m_current_message[path_name]
end
function parser_sql:getFullName(name, skipTop)
    local arr = {}
    local m_message_stack = self.m_message_stack
    local stop_index = #m_message_stack
    if skipTop then
        stop_index = stop_index - 1
    end
    for k=1,stop_index do
        table.insert(arr, m_message_stack[k].name)
    end
    if name and name ~= "" then
        table.insert(arr, name)
    end
    local path_name
    if #arr > 1 then
        path_name = table.concat(arr, ".")
    else
        path_name = arr[1]
    end
    local full_name
    if self.m_package ~= nil then
        full_name = self.m_package.."."..path_name
    else
        full_name = path_name
    end
    return full_name,path_name,arr[1]
end
function parser_sql:generateSQL()
    local tab = {
        [self.m_package] = self.m_final_message;
    }
    local data = dump(tab, 10, true, nil)
    data = string.format([[
local data = %s
return data
    ]], data)
    return data
end
function parser_sql:setFileData(file, buf)
    local file_path
    if self.m_route then
        file_path = self.m_path.."/"..self.m_route.."/"..file
    else
        file_path = self.m_path.."/"..file
    end
    print("set file data", file_path)
    local f = io.open(file_path, "wb")
    f:write(buf)
    f:close()
end
function parser_sql:findPackage(linesArr)
    for _,arr in ipairs(linesArr) do
        if arr[1] == "package" then
            self.m_package = arr[2]
            print("package_name", self.m_package)
            return
        end
    end
    print("can not find package name")
end

return parser_sql

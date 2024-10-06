
local pairs = pairs
local table = table

local map = class("map")

function map:ctor()
    self.m_count = 0
    self.m_data = {}
end

function map:dtor()
    self:clear()
end

function map:clear()
    self.m_data = {}
    self.m_count = 0
end

function map:set(k, v)
    local m_data = self.m_data
    if nil == m_data[k] then
        self.m_count = self.m_count + 1
    elseif nil == v then
        self.m_count = self.m_count - 1
    end
    m_data[k] = v
end
function map:get(k)
    return self.m_data[k]
end
function map:insert(k, v)
    return self:set(k, v)
end
function map:remove(k)
    local m_data = self.m_data
    if nil ~= m_data[k] then
        m_data[k] = nil
        self.m_count = self.m_count - 1
        return true
    end
    return false
end

function map:getValue(k)
    return self.m_data[k]
end

function map:getData()
    return self.m_data
end

function map:exist(k)
    return (nil ~= self.m_data[k] and true) or false
end

function map:size()
    return self.m_count
end
function map:isEmpty()
    return (self.m_count == 0)
end
function map:getArray()
    local tb = {}
    for _,v in pairs(self.m_data) do
        table.insert(tb,v)
    end
    return tb
end

return map


--
-- Created by IntelliJ IDEA.
-- User: Administrator
-- Date: 2023/7/13
-- Time: 11:55
-- To change this template use File | Settings | File Templates.
--

local log_error = log_error

local math = math
local ipairs = ipairs

local random_helper = class("random_helper")

function random_helper:ctor(array)
    self.m_array = array
    self.m_count = 0
    self.m_indexMap = {}
end
function random_helper:randomNext()
    local m_array = self.m_array
    local total = #m_array
    if self.m_count >= total then
        return
    end
    self.m_count = self.m_count + 1
    local index = math.random(1, total)
    local m_indexMap = self.m_indexMap
    if m_indexMap[index] then
        if math.random(1, 2) ==  1 then
            for k=index+1,total do
                if not m_indexMap[k] then
                    m_indexMap[k] = true
                    return m_array[k]
                end
            end
            for k=index-1,1,-1 do
                if not m_indexMap[k] then
                    m_indexMap[k] = true
                    return m_array[k]
                end
            end
        else
            for k=index-1,1,-1 do
                if not m_indexMap[k] then
                    m_indexMap[k] = true
                    return m_array[k]
                end
            end
            for k=index+1,total do
                if not m_indexMap[k] then
                    m_indexMap[k] = true
                    return m_array[k]
                end
            end
        end
        -- should not come here
        log_error("random_helper:randomNext error")
    else
        m_indexMap[index] = true
        return m_array[index]
    end
end

return random_helper

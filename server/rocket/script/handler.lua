--
-- Created by IntelliJ IDEA.
-- User: AppleTree
-- Date: 2017/11/26
-- Time: 下午9:35
-- To change this template use File | Settings | File Templates.
--

--local collectgarbage = collectgarbage
--local coroutine = coroutine
local string = string
local tostring = tostring
local log_debug = log_debug
local log_error = log_error

local rpc = require("rpc")
--local eproto = require("eproto")
local handler = class("handler")

-- onInitialize函数只在创建的时候调用一次
function handler:onInitialize()
    self.m_timerMap = {}
end
function handler:onInitializeEnd()

end
function handler:onHotUpdate()

end
function handler:onHttpGet(response, rpcReq)

end
-- ---------------------------------------------------------------
-- 设定timer
function handler:startTimer(componentName, timerId, obj, funcName, timeout, loop, fromTime)
    local timerKey = componentName .. "_" .. timerId
    local timerInfo = self.m_timerMap[timerKey]
    if timerInfo then
        if loop and timerInfo.obj == obj and timerInfo.funcName == funcName and timerInfo.timeout == timeout and timerInfo.loop == loop and timerInfo.fromTime == fromTime then
            return
        end
        -- 停止旧的timer
        self:stopTimerByKey(timerKey)
    end
    timerInfo = {
        obj = obj;
        componentName = componentName;
        timerId = timerId;
        funcName = funcName;
        timeout = timeout;
        loop = loop;
        fromTime = fromTime;
    }
    self.m_timerMap[timerKey] = timerInfo
    if loop then
        local callbackID = rpc:scheduleLoopTimer(function()
            return self:onTimerCallback(timerKey)
        end, timeout, fromTime)
        timerInfo.callbackID = callbackID
    else
        local result,callbackID,coInfo = rpc:schedule(function()
            self.m_timerMap[timerKey] = nil
            return self:onTimerCallback(timerKey)
        end, timeout, loop, fromTime)
        timerInfo.coInfo = coInfo
    end
end
function handler:stopTimer(componentName, timerId)
    local timerKey = componentName .. "_" .. timerId
    self:stopTimerByKey(timerKey)
end
function handler:stopTimerByKey(timerKey)
    local timerInfo = self.m_timerMap[timerKey]
    if timerInfo then
        self.m_timerMap[timerKey] = nil
        if timerInfo.loop then
            rpc:stopLoopTimer(timerInfo.callbackID)
        else
            local coInfo = timerInfo.coInfo
            rpc:remove(coInfo)
        end
    end
end
function handler:onTimerCallback(timerKey)
    local timerInfo = self.m_timerMap[timerKey]
    if timerInfo then
        local obj = timerInfo.obj
        local funcName = timerInfo.funcName
        local func = obj[funcName]
        if func then
            return func(obj, timerInfo.timerId)
        end
    end
end

return handler

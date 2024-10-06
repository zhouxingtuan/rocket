--
-- Created by IntelliJ IDEA.
-- User: zxt
-- Date: 2018/1/16
-- Time: 11:37
-- To change this template use File | Settings | File Templates.
--

local type = type
local strSub = string.sub
local strLen = string.len

local sensitive = rawget(_G, "sensitive") or {
    _tree = {}
}
rawset(_G, "sensitive", sensitive)

local _maskWord = '*'
local _tree = sensitive._tree
local function _word2Tree(root, word)
    if strLen(word) == 0 then return end

    local function _byte2Tree(r, ch, tail)
        if tail then
            if type(r[ch]) == 'table' then
                r[ch].isTail = true
            else
                r[ch] = true
            end
        else
            if r[ch] == true then
                r[ch] = { isTail = true }
            else
                r[ch] = r[ch] or {}
            end
        end
        return r[ch]
    end

    local tmpparent = root
    local len = strLen(word)
    for i=1, len do
        if tmpparent == true then
            tmpparent = { isTail = true }
        end
        tmpparent = _byte2Tree(tmpparent, strSub(word, i, i), i==len)
    end
end
local function _detect(parent, word, idx)
    local len = strLen(word)

    local ch = strSub(word, 1, 1)
    local child = parent[ch]

    if not child then
    elseif type(child) == 'table' then
        if len > 1 then
            if child.isTail then
                return _detect(child, strSub(word, 2), idx+1) or idx
            else
                return _detect(child, strSub(word, 2), idx+1)
            end
        elseif len == 1 then
            if child.isTail == true then
                return idx
            end
        end
    elseif (child == true) then
        return idx
    end
    return false
end
function sensitive:parse(file_name)
    if type(file_name) == "string" then
        local words = require(file_name)
        for _, word in pairs(words) do
            _word2Tree(_tree, word)
        end
        package.loaded[file_name]  = nil
    else
        local words = file_name
        for _, word in pairs(words) do
            _word2Tree(_tree, word)
        end
    end
end
function sensitive:replace(s)
    if type(s) ~= 'string' then return end
    local i = 1
    local len = strLen(s)
    local word, idx, tmps
    while true do
        word = strSub(s, i)
        idx = _detect(_tree, word, i)

        if idx then
            tmps = strSub(s, 1, i-1)
            for j=1, idx-i+1 do
                tmps = tmps .. _maskWord
            end
            s = tmps .. strSub(s, idx+1)
            i = idx+1
        else
            i = i + 1
        end
        if i > len then
            break
        end
    end
    return s
end
function sensitive:detect(s)
    if type(s) ~= 'string' then return end
    local i = 1
    local len = strLen(s)
    local word, idx, tmps
    while true do
        word = strSub(s, i)
        idx = _detect(_tree, word, i)

        if idx then
            return strSub(s, i, idx)
        else
            i = i + 1
        end
        if i > len then
            break
        end
    end
end
return sensitive

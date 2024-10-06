--
-- Created by IntelliJ IDEA.
-- User: AppleTree
-- Date: 15/4/6
-- Time: 下午9:43
-- To change this template use File | Settings | File Templates.
--

---- global module
local _G = _G
local type = type
local pairs = pairs
local error = error
local string = string
local pcall = pcall
local tostring = tostring
local setmetatable = setmetatable
local getmetatable = getmetatable
local rawset = rawset
local require = require
local debug = debug
-----------------------------------------------------------------------------
-- class define
-----------------------------------------------------------------------------
-- 记录创建过的类；下面的操作避免重复加载的时候也可以不清掉以前的数据
local classes = _G.classes or {}
rawset(_G, "classes", classes)

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
local function splitFileName(fileName)
	local index = getExtentionIndex(fileName)
	if not index then
		return fileName
	end
	local name = string.sub(fileName, 1, index - 1)
	return name
end
local function clone(object, to_object)
	local lookup_table = {}
	local function _copy(object, to_object)
		if type(object) ~= "table" then
			return object
		elseif lookup_table[object] then
			return lookup_table[object]
		end
		local new_table = to_object or {}
		lookup_table[object] = new_table
		for key, value in pairs(object) do
			new_table[_copy(key)] = _copy(value)
		end
		return setmetatable(new_table, getmetatable(object))
	end
	return _copy(object, to_object)
end
-- 这里将会修改class的定义，原始定义在extern.lua中；这里会记录创建过的类的类型
--Create an class.
local function class(defineClassName, super)
	-- 获取当前类声明的目录，使用这个目录拼接当前定义的类名称，作为最终类名称；
	-- 通过这个方式有效防止不同的目录定义相同的类名称，导致出现冲突；
	local info = debug.getinfo(2, "S")
	local filePath = splitFileName(info.short_src)
	local classname = filePath .. ":" .. defineClassName
	log_debug("class define", classname)

	-- 查找是否已经有创建过的类，将会直接返回
	if type(super) == "string" then
--		super = classes[super]
		super = require("super")
	end
	local cls = classes[classname]

	local superType = type(super)

	-- 这里做一个处理，让外部可以通过字符串来实现继承，前提是另外一个类已经被加载?（感觉也没必要）
	if type(classname) ~= "string" then
		error("classname mast be string type : "..tostring(classname))
	end
	-- 这里需要报错，提醒外面传进来的类型是错误类型
	if superType ~= "function" and superType ~= "table" then
		if superType ~= "nil" then
			error("the super of class are not function,table or nil : "..tostring(classname).."-->"..tostring(superType)..", "..debug.traceback())
		end
		superType = nil
		super = nil
	end
	if superType == "function" or (super and super.__ctype == 1) then
		-- inherited from native C++ Object
		cls = cls or {}
		if superType == "table" then
			-- copy fields from super
			for k,v in pairs(super) do cls[k] = v end
			cls.__create = super.__create
			cls.super    = super
		else
			cls.__create = super
		end
		cls.ctor    = function() end
		cls.__cname = classname
		cls.__ctype = 1
		function cls.new(...)
			-- 传入的初始化参数是个个类的初始化数据
			local instance = cls.__create(...)
			-- copy fields from class to native object
			for k,v in pairs(cls) do instance[k] = v end
			instance.class = cls
			instance:ctor(...)

			return instance
		end
	else
		-- inherited from Lua Object
		if super then
			if cls == nil then
				cls = clone(super)
--				cls = setmetatable({}, super)
			else
				for k,v in pairs(super) do cls[k] = v end
			end
			cls.super = super
		else
			if cls == nil then
				cls = {ctor = function() end}
			end
		end

		cls.__cname = classname
		cls.__ctype = 2 -- lua
		cls.__index = cls
--		cls.__index = function(_, key)
--			local v = cls[key]
--			if v then
--				return v
--			end
--			if super then
--				return super.__index(_, key)
--			end
--		end

		function cls.new(...)
			local instance = setmetatable({}, cls)
			instance.class = cls
			instance:ctor(...)

			return instance
		end
	end

	-- 记录创建的类数据
	classes[classname] = cls

	return cls
end
rawset(_G, "class", class)

-----------------------------------------------------------------------------
-- global variable control
-- 全局变量（_G表中的值）在这里之后将会被控制存取
-----------------------------------------------------------------------------
local gm = {}
-- 凡是通过class注册的类，或者模块，都可以通过_G查询到
gm.__index = function(_, key)
	local c = classes[key]
	if c == nil then
		error("you can't __index global variable(for right value): xxx="..tostring(key)..", "..debug.traceback())
	end
	return c
end
-- 禁止程序员使用_G表，全局变量全部禁止使用；程序员通过subclass或者module可以声明一个模块
gm.__newindex = function(_, key, value)
	if classes[key] then
		return
	end
	error("you can't __newindex global variable(for left value): "..tostring(key).."="..tostring(value)..", "..debug.traceback())
end
setmetatable(_G, gm)

return class



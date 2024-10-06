--
-- Created by IntelliJ IDEA.
-- User: AppleTree
-- Date: 2017/6/3
-- Time: 下午2:51
-- To change this template use File | Settings | File Templates.
--

-- 支持 pb, mp, json 三种协议
-- eproto, msgpack, json
-- level 等级区分三种
-- 0 为公共接口，无需权限
-- 5 为客户端访问权限
-- 9 为服务器访问权限

local proto = {}

proto.files = {
	"pb_rocket";
	--"pb_login";
}
-- msgLevel, msgID, msgSource, requestName, responseName
-- 特殊的请求参数： objFuncName
proto.messages = {
	{0, 1000, "json", "", ""}; -- 热更新命令
	{9, 1001, "json", "", ""}; -- 热更新执行
	{0, 1002, "json", "", "", "Register"}; -- 注册
}

return proto



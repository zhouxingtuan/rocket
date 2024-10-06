# rocket
rocket is a multi thread web server.
rocket 是一个多线程架构的web服务器

link https://github.com/zhouxingtuan/rocket

# rocket 的特点
    (1) 类似nginx的处理web的方式，主线程accept连接，之后分给其它子线程处理业务逻辑，单进程可高效处理高并发
    (2) 使用LuaJIT作为脚本语言，封装了Lua的协程处理异步请求
    (3) 添加了levelDB作为本地持久化存储，接口 db_open、db_set、db_get、db_del、db_batch
    (4) 添加了shared_buffer作为多线程共享数据，支持原子update操作，接口 sb_create、sb_set、sb_get、sb_del、sb_update
    (5) 子线程间可以实现通信，接口 rpc:send、rpc:broadcast
    (6) 线程可以与其它服务进程节点通过WebSocket建立连接，组建服务网络；也可以通过http请求来与其它服务通信rpc:http
    (7) 封装好的定时器 handler:startTimer(componentName, timerId, obj, funcName, timeout, loop, fromTime)
    (8) 一套完整的Lua类系统和业务逻辑处理封装
    (9) 提供连接MySQL数据库和Redis数据库的封装
    (10) 源代码模块划分清晰，代码量少，便于阅读、学习和修改

# 当前测试进度
    由于重构的代码，还没有经过完整测试；测试进度目前60%左右
    release目录下面有已经编译好的可执行文件，CentOS7、ubuntu22、ubuntu24

# 商业合作以及支持
    appletreezxt@qq.com


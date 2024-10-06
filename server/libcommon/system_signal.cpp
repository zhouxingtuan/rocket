//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 17/3/18
// Time: 上午9:19
// To change this template use File | Settings | File Templates.
//

#include <map>
#include <string>
#include <stdio.h>
#include "system_signal.h"
#include "log.h"

NS_HIVE_BEGIN

typedef struct SignalHandleInfo {
	std::string name;
	SignalCallback callback;
	bool quit;
}SignalHandleInfo;
typedef std::map<int, SignalHandleInfo> SignalHandleInfoMap;
class SignalHandleObject
{
public:
    SignalHandleObject(void){

    }
    ~SignalHandleObject(void){

    }

    SignalHandleInfoMap m_signalMap;
};

static SignalHandleObject* g_pSignalHandleObject = NULL;
//static SignalHandleInfoMap g_signalMap;

void handleSignal(int sig){
	LOG_ERROR("handleSignal called sig=%d", sig);
	if(NULL == g_pSignalHandleObject){
	    LOG_ERROR("handleSignal NULL == g_pSignalHandleObject sig=%d", sig);
	    return;
	}
	SignalHandleInfoMap::iterator itCur = g_pSignalHandleObject->m_signalMap.find(sig);
	if( itCur != g_pSignalHandleObject->m_signalMap.end() ){
		LOG_ERROR("signal name=%s", itCur->second.name.c_str());
		if( NULL != itCur->second.callback ){
			itCur->second.callback(sig);
		}
		if( itCur->second.quit ){
			LOG_ERROR("handleSignal exit sig=%d name=%s", sig, itCur->second.name.c_str());
			sleep(1);
			exit(1);
		}
	}else{
		LOG_ERROR("handleSignal unregister sig=%d", sig);
		sleep(1);
		exit(1);
	}
}

void signalExit(int sig, const char* name, SignalCallback cb){
	if(NULL == g_pSignalHandleObject){
	    LOG_ERROR("signalExit NULL == g_pSignalHandleObject sig=%d", sig);
	    return;
	}
	SignalHandleInfo info;
	info.name = name;
	info.callback = cb;
	info.quit = true;
	g_pSignalHandleObject->m_signalMap[sig] = info;
	signal(sig, handleSignal);
}
void signalHandle(int sig, const char* name, SignalCallback cb){
	if(NULL == g_pSignalHandleObject){
	    LOG_ERROR("signalHandle NULL == g_pSignalHandleObject sig=%d", sig);
	    return;
	}
	SignalHandleInfo info;
	info.name = name;
	info.callback = cb;
	info.quit = false;
	g_pSignalHandleObject->m_signalMap[sig] = info;
	signal(sig, handleSignal);
}

// SIGKILL 和 SIGSTOP 无法捕获，也不能被忽略
void defaultSignalHandle(SignalCallback coreCb, SignalCallback exitCb, SignalCallback handleCb){
    if(NULL == g_pSignalHandleObject){
        g_pSignalHandleObject = new SignalHandleObject();
    }
	// 忽略某些信号的处理
	SIGNAL_IGNORE(SIGPIPE);
	SIGNAL_IGNORE(SIGCHLD);

	if(NULL != coreCb){
		// 指针错误信号监听
		SIGNAL_EXIT(SIGSEGV, coreCb);
	}

	// 只响应kill命令，其它的只打印不处理（2020.10.19 阿里云操作系统产生SIGABRT但是不产生core文件）
	SIGNAL_HANDLE(SIGILL, handleCb);
	SIGNAL_HANDLE(SIGTRAP, handleCb);
	SIGNAL_HANDLE(SIGABRT, handleCb);
	SIGNAL_HANDLE(SIGBUS, handleCb);
	SIGNAL_HANDLE(SIGFPE, handleCb);
	SIGNAL_HANDLE(SIGIOT, handleCb);
	SIGNAL_HANDLE(SIGINT, handleCb);
	SIGNAL_HANDLE(SIGQUIT, handleCb);
	SIGNAL_HANDLE(SIGXCPU, handleCb);
	SIGNAL_HANDLE(SIGXFSZ, handleCb);
	SIGNAL_EXIT(SIGTERM, exitCb);
	SIGNAL_HANDLE(SIGTSTP, handleCb);
	SIGNAL_HANDLE(SIGTTIN, handleCb);
	SIGNAL_HANDLE(SIGTTOU, handleCb);

	// 监听并打印信号或处理
//	SIGNAL_EXIT(SIGILL, exitCb);
//	SIGNAL_EXIT(SIGTRAP, exitCb);
//	SIGNAL_EXIT(SIGABRT, exitCb);
//	SIGNAL_EXIT(SIGBUS, exitCb);
//	SIGNAL_EXIT(SIGFPE, exitCb);
//	SIGNAL_EXIT(SIGIOT, exitCb);
//	SIGNAL_EXIT(SIGINT, exitCb);
//	SIGNAL_EXIT(SIGQUIT, exitCb);
//	SIGNAL_EXIT(SIGXCPU, exitCb);
//	SIGNAL_EXIT(SIGXFSZ, exitCb);
//	SIGNAL_EXIT(SIGTERM, exitCb);
//	SIGNAL_EXIT(SIGTSTP, exitCb);
//	SIGNAL_EXIT(SIGTTIN, exitCb);
//	SIGNAL_EXIT(SIGTTOU, exitCb);

	// 只监听打印，不处理
	SIGNAL_HANDLE(SIGALRM, handleCb);
	SIGNAL_HANDLE(SIGHUP, handleCb);
	SIGNAL_HANDLE(SIGPOLL, handleCb);
	SIGNAL_HANDLE(SIGPROF, handleCb);
	SIGNAL_HANDLE(SIGSYS, handleCb);
	SIGNAL_HANDLE(SIGVTALRM, handleCb);
	SIGNAL_HANDLE(SIGUSR1, handleCb);
	SIGNAL_HANDLE(SIGUSR2, handleCb);
	SIGNAL_HANDLE(SIGIO, handleCb);
}

NS_HIVE_END

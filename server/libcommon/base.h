//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/5/28
// Time: 下午12:53
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__base__
#define __hive__base__

#define NS_HIVE_BEGIN namespace hive {
#define NS_HIVE_END };
#define USING_NS_HIVE using namespace hive;

//c libs
#include <assert.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>  //atof函数
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <inttypes.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <fcntl.h>			/* nonblocking */
#include <arpa/inet.h>		/* inet(3) functions */

//std libs
#include <atomic>
//#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>

////epoll & net libs
//#include <errno.h>
//#include <netdb.h>
//#include <sys/types.h>		/* basic system data types */
//#include <sys/socket.h>		/* basic socket definitions */
//#include <netinet/in.h>		/* sockaddr_in{} and other Internet defns */
//#include <arpa/inet.h>		/* inet(3) functions */
//#include <sys/epoll.h>		/* epoll function */
//#include <fcntl.h>			/* nonblocking */

#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)

//thread libs
#include <pthread.h>
#include <semaphore.h>


#define SAFE_DELETE(ptr) if(ptr != NULL){ delete ptr; ptr = NULL; }
#define SAFE_DELETE_ARRAY(ptr) if(ptr != NULL){ delete [] ptr; ptr = NULL; }
#define SAFE_RETAIN(ptr) if(ptr != NULL){ ptr->retain(); }
#define SAFE_RELEASE(ptr) if(ptr != NULL){ ptr->release(); ptr = NULL; }

NS_HIVE_BEGIN

// 统一所有数据类型的使用
typedef char 						int8;
typedef short 					int16;
typedef int 						int32;
typedef long long   				int64;
typedef unsigned char 			uint8;
typedef unsigned short  			uint16;
typedef unsigned int    			uint32;
typedef unsigned long long      uint64;

#define MAX_INT64 9223372036854775807
#define MIN_INT64 -9223372036854775808
#define MAX_INT32 2147483647
#define MIN_INT32 -2147483648
/*--------------------------------------------------------------------*/
// 拥有同步互斥量的类
class Sync
{
protected:
	pthread_mutex_t m_mutex;
public:
	Sync(void){
		pthread_mutex_init(&m_mutex, NULL);
	}
	virtual ~Sync(void){
		pthread_mutex_destroy(&m_mutex);
	}
	inline void lock(void){
		pthread_mutex_lock(&m_mutex);
	}
	inline void rlock(void){
		pthread_mutex_lock(&m_mutex);
	}
	inline void unlock(void){
		pthread_mutex_unlock(&m_mutex);
	}
};
// 读写锁权限分离的类
class SyncRW
{
protected:
    pthread_rwlock_t m_rwlock;
public:
	SyncRW(void){
		pthread_rwlock_init(&m_rwlock, NULL);
	}
	virtual ~SyncRW(void){
		pthread_rwlock_destroy(&m_rwlock);
	}
	inline void lock(void){
		pthread_rwlock_wrlock(&m_rwlock);
	}
	inline void rlock(void){
		pthread_rwlock_rdlock(&m_rwlock);
	}
	inline void unlock(void){
		pthread_rwlock_unlock(&m_rwlock);
	}
};
/*--------------------------------------------------------------------*/
// 引用计数基类

class RefObject
{
protected:
	std::atomic<int> m_referenceCount;
	union{
	    int m_padding;
	    int m_length;
	    int m_cursor;
//	    unsigned int m_handle;
	};
public:
	RefObject(void) : m_referenceCount(0), m_padding(0){}
	RefObject(int referenceCount) : m_referenceCount(referenceCount), m_padding(0){}
	virtual ~RefObject(void){}
	inline void release(void){
		if( std::atomic_fetch_sub_explicit(&m_referenceCount, 1, std::memory_order_relaxed) == 1 ){
			delete this;
		}
	}
	inline void retain(void){
		std::atomic_fetch_add_explicit(&m_referenceCount, 1, std::memory_order_relaxed);
	}
	inline int getRefCount(void){ return (int)m_referenceCount; }
	inline int getPadding(void){ return m_padding; }
	inline void setPadding(int value){ m_padding = value; }
};
/*--------------------------------------------------------------------*/
// 线程基类
class Thread
{
public:
	typedef void*(*ThreadCallback)(void* pData);
protected:
	pthread_t m_pThread;
public:
	Thread(void) : m_pThread(0){}
	virtual ~Thread(void){}
	bool startThread(void);
	void cancelThread(void);
	pthread_t getThreadHandle(void){ return m_pThread; }
	virtual int threadFunction(void) = 0;

	static pthread_t staticThread(ThreadCallback start_rtn, void *arg);
private:
	static void* staticThreadFunction(void* pData);
};
/*--------------------------------------------------------------------*/

#define DEFINE_INSTANCE_H(T)\
static T* getInstance(void);\
static T* createInstance(void);\
static void destroyInstance(void);\


#define DEFINE_INSTANCE_CPP(T)\
static T* g_p##T = NULL;\
T* T::getInstance(void){\
	return g_p##T;\
}\
T* T::createInstance(void){\
	if(g_p##T == NULL){\
		g_p##T = new T();\
		SAFE_RETAIN(g_p##T)\
	}\
	return g_p##T;\
}\
void T::destroyInstance(void){\
    SAFE_RELEASE(g_p##T)\
}\


NS_HIVE_END

#endif

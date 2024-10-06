//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/10/12
// Time: 下午9:12
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__timer__
#define __hive__timer__

#include "base.h"

NS_HIVE_BEGIN

#define EPOLL_DEFAULT_WAIT_TIME 10000

//#ifdef WIN32
inline int64 get_time_us(void){
	std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
	return (int64)std::chrono::duration_cast<std::chrono::microseconds>(p.time_since_epoch()).count();
}
inline int64 get_time_ms(void){
	std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
	return (int64)std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch()).count();
}
inline int64 get_time_s(void){
	std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
	return (int64)std::chrono::duration_cast<std::chrono::seconds>(p.time_since_epoch()).count();
}
inline int32 get_time_seed(void){
	int64 ts = get_time_s();
	ts = ts % MAX_INT32;
	return (int32)ts;
}
//#else
//inline int64 get_time_us(void){
//	timeval t;
//	gettimeofday( &t, NULL );
//	return (int64)(1000000*(int64)t.tv_sec + (int64)t.tv_usec);
//}
//inline int64 get_time_ms(void){
//	timeval t;
//	gettimeofday( &t, NULL );
//	return (int64)(1000*(int64)t.tv_sec + (int64)t.tv_usec/1000);
//}
//#endif
inline int64 get_next_time_us(int64 timeCount){
	return (get_time_us() + (int64)(1000*timeCount));
}
inline int64 get_next_time_ms(int64 timeCount){
	return (get_time_ms() + (int64)(timeCount));
}

class TimerObject;
class TimerList;
class Timer;

class TimerObject
{
	friend class TimerList;
	friend class Timer;
public:
	TimerObject(void) : m_index(0), m_condition(0), m_pTimerList(NULL), m_pPre(NULL), m_pNext(NULL){}
	virtual ~TimerObject(void){ clearTimer(); }

	virtual bool isActive(void) const { return (NULL != m_pTimerList); }
	virtual bool setTimer(int64 timeCount, int64 fromTime, Timer* pTimer);
	virtual bool clearTimer(void);
	virtual int64 timerCallback(void) = 0;

	inline int64 getCondition(void) const { return m_condition; }
	inline int64 getLeftTime(void) const{
		int64 currentTime = get_time_ms();
		int64 condition = this->getCondition();
		if( condition > currentTime ){
			return (int64)(condition - currentTime);
		}
		return 0;
	}
	inline uint64 getIndex(void) const { return m_index; }
	inline int64 getNextCondition(int64 timeCount){
		int64 currentTime = get_time_ms();
		if(timeCount == 0){
			return currentTime;
		}
		int64 condition = this->getCondition();
		if(condition == 0){
			return currentTime + timeCount;
		}
		do{
			condition += timeCount;
			if(condition > currentTime){
				return condition;
			}
		}while(true);
		return currentTime;
	}
private:
	uint64			m_index;		// Timer的序号
	int64 			m_condition;	// 调用的条件
	TimerList* 		m_pTimerList;	// 链表
	TimerObject* 	m_pPre;			// 前一个指针
	TimerObject* 	m_pNext;		// 下一个指针
};

typedef std::pair<TimerObject*, TimerList*> TimerObjectListPair;
typedef std::vector<TimerObjectListPair> TimerObjectVector;

class TimerList : public RefObject
{
	friend class TimerObject;
	friend class Timer;
public:
	TimerList(void) : RefObject(), m_timeCount(0), m_pHead(NULL), m_pTail(NULL), m_pTimer(NULL) {}
	virtual ~TimerList(void){}

	inline int64 getLeftTime(void){
		if(NULL != m_pHead){
			return m_pHead->getLeftTime();
		}
		return 0;
	}
	inline int64 getHeadCondition(void){
		if(NULL != m_pHead){
			return m_pHead->getCondition();
		}
		return MAX_INT64;
	}
private:
	void update(int64 currentTime, TimerObjectVector& objects);
	void add(TimerObject* pObject, int64 fromTime);
	bool remove(TimerObject* pObject);
	void setTimer(Timer* pTimer){ m_pTimer = pTimer; }
	inline bool isEmpty(void) const { return (NULL == m_pHead); }
	inline void setTimeCount(int64 timeCount) { m_timeCount = timeCount; }
	inline int64 getTimeCount(void) const { return m_timeCount; }
	inline void clearList(void){ m_pHead = NULL; m_pTail = NULL; }
private:
	int64 m_timeCount;		// 当前链表的时间
	TimerObject* m_pHead;	// 头部指针
	TimerObject* m_pTail;	// 尾部指针
	Timer* m_pTimer;		// Timer管理器的指针
};

class Timer : public RefObject
{
	friend class TimerObject;
	friend class TimerList;
public:
	typedef std::map<int64, TimerList*> TimerListMap;
	Timer(void);
	virtual ~Timer(void);

	void update(void);
	inline int64 getWaitTimeout(void){
		if( MAX_INT64 == m_waitTimeout ){
//    		return -1;
    		return EPOLL_DEFAULT_WAIT_TIME;   // default wait timeout
    	}
    	int64 currentTime = get_time_ms();
    	if( m_waitTimeout > currentTime ){
    		return (int64)(m_waitTimeout - currentTime);
    	}else{
    		return 0;
    	}
	}
	inline void recordWaitTimeout(int64 condition){
		if( condition < m_waitTimeout ){
			m_waitTimeout = condition;
		}
	}
	inline uint32 getTimerListSize(void) const { return (uint32)m_timerListMap.size(); }
private:
	void checkRemoveList(void);
	bool timerAdd(TimerObject* pObject, int64 timeCount, int64 fromTime);
	uint64 getTimerIndex(void) { return ++m_timerIndex; }
	TimerList* findTimerList(int64 timeCount){
		TimerListMap::iterator itCur = m_timerListMap.find(timeCount);
		if( itCur != m_timerListMap.end() ){
			return itCur->second;
		}
		return NULL;
	}
	void releaseTimerList(void);
private:
	TimerListMap m_timerListMap;	// 用于查找对应的timerList
	uint64 m_timerIndex;			// TimerObject的序号
	int64 m_waitTimeout;			// 最近一次超时时间
};

NS_HIVE_END

#endif

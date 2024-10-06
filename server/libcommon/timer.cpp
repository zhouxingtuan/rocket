//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/10/12
// Time: 下午9:13
// To change this template use File | Settings | File Templates.
//

#include "timer.h"
#include "log.h"

NS_HIVE_BEGIN

bool TimerObject::setTimer(int64 timeCount, int64 fromTime, Timer* pTimer){
    if(NULL == pTimer){
        LOG_ERROR("setTimer failed NULL == pTimer timeCount=%lld", timeCount);
        return false;
    }
	return pTimer->timerAdd(this, timeCount, fromTime);
}
bool TimerObject::clearTimer(void){
	if(NULL != m_pTimerList){
		return m_pTimerList->remove(this);
	}
	return false;
}

void TimerList::update(int64 currentTime, TimerObjectVector& objects){
	TimerObject* pTemp;
	TimerObject* pObject = m_pHead;
	while( NULL != pObject && currentTime >= pObject->m_condition ){
		pTemp = pObject;
		pObject = pObject->m_pNext;	// 检查下一个节点

		objects.push_back(std::make_pair(pTemp, this));
		pTemp->m_pTimerList = NULL;
		pTemp->m_pPre = NULL;
		pTemp->m_pNext = NULL;
	};
	m_pHead = pObject;	// 之前的节点全部出队列
	if(NULL == m_pHead){
		m_pTail = NULL;
	}else{
		m_pHead->m_pPre = NULL;
	}
}
void TimerList::add(TimerObject* pObject, int64 fromTime){
	this->remove(pObject);		// 移除出旧的列表
	pObject->m_index = m_pTimer->getTimerIndex();
	int64 currentTime = get_time_ms();
	if(fromTime < currentTime){
		pObject->m_condition = currentTime + m_timeCount;
	}else{
		pObject->m_condition = fromTime;
	}
//	pObject->m_condition = get_next_time_ms(m_timeCount);
	pObject->m_pTimerList = this;
	pObject->m_pPre = NULL;
	pObject->m_pNext = NULL;
	if( NULL == m_pHead ){
		m_pHead = pObject;
		m_pTail = pObject;
		return;
	}
	m_pTail->m_pNext = pObject;
	pObject->m_pPre = m_pTail;
	m_pTail = pObject;
}
bool TimerList::remove(TimerObject* pObject){
	TimerList* pTimerList = pObject->m_pTimerList;
	if(NULL == pTimerList){
		return false;
	}
	TimerObject* pPre = pObject->m_pPre;
	TimerObject* pNext = pObject->m_pNext;
	if(pObject == pTimerList->m_pHead){
		if(pObject == pTimerList->m_pTail){
			pTimerList->m_pHead = NULL;
			pTimerList->m_pTail = NULL;
		}else{
			pTimerList->m_pHead = pNext;
			pNext->m_pPre = NULL;
		}
	}else{
		if(pObject == pTimerList->m_pTail){
			pTimerList->m_pTail = pPre;
			pPre->m_pNext = NULL;
		}else{
			pPre->m_pNext = pNext;
			pNext->m_pPre = pPre;
		}
	}
	// 清理数据
	pObject->m_pTimerList = NULL;
	pObject->m_pPre = NULL;
	pObject->m_pNext = NULL;
	return true;
}

Timer::Timer() : RefObject(), m_timerIndex(0), m_waitTimeout(MAX_INT64) {

}
Timer::~Timer(){
    releaseTimerList();
}
bool object_sort_function(const TimerObjectListPair& a, const TimerObjectListPair& b){
	return a.first->getIndex() < b.first->getIndex();
}
void Timer::update(void){
	int64 currentTime = get_time_ms();
	// 还没有到更新时间，这里不做更新操作
	if(m_waitTimeout > currentTime){
		return;
	}
	m_waitTimeout = MAX_INT64;
	TimerObjectVector objects;
	for(auto &pair : m_timerListMap ){
//		fprintf(stderr, "update check timeCount=%lld\n", pair.second->getTimeCount());
		pair.second->update(currentTime, objects);
		this->recordWaitTimeout(pair.second->getHeadCondition());
	}
	if(objects.size() > 0){
		std::sort(objects.begin(), objects.end(), object_sort_function);
		TimerObject* pObject;
		TimerList* pTimerList;
		int64 timeCount;
		for(auto &objListPair : objects){
			pObject = objListPair.first;
			pTimerList = objListPair.second;

			timeCount = pObject->timerCallback();
//            LOG_DEBUG("Timer::update timeCount=%lld pObject=%x pObject->isActive()=%d", timeCount, pObject, pObject->isActive());
			if(timeCount >= 0 && !pObject->isActive()){
				int64 fromTime = pObject->getNextCondition(timeCount);
				if(pTimerList->getTimeCount() == timeCount){
					pTimerList->add(pObject, fromTime);
					this->recordWaitTimeout(pObject->m_condition);
				}else{
					this->timerAdd(pObject, timeCount, fromTime);
				}
			}
		}
		checkRemoveList();
//		fprintf(stderr, "m_waitTimeout - get_time_ms() = %lld\n", m_waitTimeout - get_time_ms());
	}
}
void Timer::checkRemoveList(void){
	TimerList* pTimerList;
	TimerListMap::iterator itCur = m_timerListMap.begin();
	for(;itCur != m_timerListMap.end();){
		pTimerList = itCur->second;
		if( pTimerList->isEmpty() ){
			pTimerList->clearList();
			pTimerList->release();
			m_timerListMap.erase(itCur++);
		}else{
			++itCur;
		}
	}
}
bool Timer::timerAdd(TimerObject* pObject, int64 timeCount, int64 fromTime){
	// 是否添加到旧的列表
	TimerList* pTimerList = pObject->m_pTimerList;
	if( NULL != pTimerList ){
		// 判断只有在当前列表下，才可以直接添加
		if( this == pTimerList->m_pTimer && pTimerList->getTimeCount() == timeCount ){
			pTimerList->add(pObject, fromTime);
			this->recordWaitTimeout(pObject->m_condition);
			return true;
		}
		// 不添加就从旧的链表删除
		pTimerList->remove(pObject);
	}
	// 找到timeCount对应的list，存在就直接添加到list中
	pTimerList = findTimerList(timeCount);
	if( NULL != pTimerList ){
		pTimerList->add(pObject, fromTime);
		this->recordWaitTimeout(pObject->m_condition);
		return true;
	}
	// 不存在就获取新的list添加进去
	pTimerList = new TimerList();
	pTimerList->retain();
	pTimerList->setTimer(this);
	pTimerList->setTimeCount(timeCount);
	pTimerList->add(pObject, fromTime);
	m_timerListMap.insert(std::make_pair(timeCount, pTimerList));
	this->recordWaitTimeout(pObject->m_condition);
	return true;
}
void Timer::releaseTimerList(void){
    for(auto &pair : m_timerListMap ){
        pair.second->release();
    }
    m_timerListMap.clear();
}

NS_HIVE_END

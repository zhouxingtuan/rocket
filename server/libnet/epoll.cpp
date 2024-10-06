
#include "epoll.h"

NS_HIVE_BEGIN
/*--------------------------------------------------------------------*/
EpollObject::EpollObject(void) : TimerObject(),
    m_pEpoll(NULL),
    m_timerCallback(NULL),
    m_handle(0),
    m_serverHandle(0),
    m_callbackID(0),
    m_fd(-1),
    m_ip(0),
    m_port(0), m_isNeedEncrypt(false), m_isInEpoll(false) {
//    LOG_DEBUG("EpollObject new %p", this);
}
EpollObject::~EpollObject(void){
//    LOG_DEBUG("EpollObject delete %p", this);
    closeSocket();
}
int64 EpollObject::timerCallback(void){
    if(NULL != m_timerCallback){
        return m_timerCallback(this);
    }
    return -1;
}
void EpollObject::epollRemove(void){
	m_pEpoll->objectRemove(this);
}
bool EpollObject::setTimeout(int64 timeCount, EpollObjectTimerFunction callback){
    if( setTimer(timeCount, 0, m_pEpoll->getTimer())){
        m_timerCallback = callback;
        return true;
    }
    return false;
}
int EpollObject::epollRemoveWithoutClose(void){
    int fd = m_fd;
    if( fd >= 0 ){
        m_pEpoll->objectRemoveWithoutClose(this);
    }
    return fd;
}
void EpollObject::closeSocket(void){
//    LOG_DEBUG("EpollObject closeSocket handle=%u m_fd=%d", getHandle(), m_fd);
    if( m_fd >= 0 ){
        close( m_fd );
        m_fd = -1;
    }
}
void EpollObject::clearData(void){
    closeSocket();
    clearTimer();
}
/*--------------------------------------------------------------------*/
Epoll::Epoll(void) : RefObject(), m_epollfd(-1), m_activeNextFrameEnd(false), m_pTimer(NULL)
{
    m_pTimer = new Timer();
    m_pTimer->retain();
}
Epoll::~Epoll(void){
    closeEpoll();
    SAFE_RELEASE(m_pTimer);
}
void Epoll::update(void){
    EpollObject* pObject;
    int64 timeout;
	pid_t tid = gettid();
	uint32 handle;
    uint32 events;
    int nfds;
    char state = 0;

    LOG_INFO("Epoll thread start tid=%d", (int)tid);
	while(1){
	    if(0 == state && m_activeNextFrameEnd){
            m_activeNextFrameEnd = false;
            state = 1;
	    }else{
	        state = 0;
            timeout = m_pTimer->getWaitTimeout();   // 毫秒级别
            nfds = epoll_wait(m_epollfd, m_events, MAX_WAIT_EVENT_SIZE, timeout);
            if(nfds > 0){
                for(int i=0; i<nfds; ++i){
                    struct epoll_event& evt = m_events[i];
                    events = evt.events;
    //                pObject = (EpollObject*)evt.data.ptr;
                    handle = evt.data.u32;
                    pObject = getObject(handle);
                    if(NULL == pObject){
                        LOG_ERROR("can not find object for handle=%u", handle);
    //                    LOG_ERROR("can not find object for pObject=%p", pObject);
                        continue;
                    }
                    checkConnectObject(pObject, events);
                }
            }
            m_pTimer->update();
	    }
        if(1 == state){
            onNextFrameActive();
        }
	}
    LOG_INFO("Epoll thread end tid=%d", (int)tid);
}
void Epoll::checkConnectObject(EpollObject* pObject, uint32 events){
	if(pObject->getSocketFD() < 0){
		return;
	}
    if( pObject->epollActive(events) ){
        return;
    }
    if(events & EPOLLIN){
        if( !pObject->epollIn() ){
            return;
        }
    }
    if(events & EPOLLOUT){
        if( !pObject->epollOut() ){
            return;
        }
    }else if(events & EPOLLERR){
//        LOG_DEBUG("socket error and remove handle=%u", pObject->getHandle());
        pObject->epollRemove();
        return;
    }
    // 在这里，该连接有可能已经被移除
    if(pObject->getSocketFD() >= 0){
        pObject->epollCheck();
    }
}
bool Epoll::objectAddLT(EpollObject* pObject, uint32 events){
    int fd = pObject->getSocketFD();
    if(fd < 0){
        return false;
    }
    uint32 handle = pObject->getHandle();
    if(handle == 0){
        LOG_ERROR("objectAddLT handle == 0");
        return false;
    }
    struct epoll_event ev;
    ev.events = events;
    ev.data.u32 = handle;
//    ev.data.ptr = pObject;
    if( epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &ev) == -1){
        LOG_ERROR("objectAddLT epoll_ctl failed handle=%u", handle);
//        LOG_ERROR("objectAddLT epoll_ctl failed pObject=%p", pObject);
        return false;
    }
    pObject->setIsInEpoll(true);
    return true;
}
// EPOLLIN | EPOLLOUT | EPOLLET
bool Epoll::objectAdd(EpollObject* pObject, uint32 events){
    int fd = pObject->getSocketFD();
    if(fd < 0){
        LOG_ERROR("objectAdd fd < 0");
        return false;
    }
    uint32 handle = pObject->getHandle();
    if(handle == 0){
        LOG_ERROR("objectAdd handle == 0");
        return false;
    }
    struct epoll_event ev;
    ev.events = events | EPOLLET;
    ev.data.u32 = handle;
//    ev.data.ptr = pObject;
    if( epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &ev) == -1){
        LOG_ERROR("objectAdd epoll_ctl failed handle=%u", handle);
//        LOG_ERROR("objectAdd epoll_ctl failed pObject=%p", pObject);
        return false;
    }
    pObject->setIsInEpoll(true);
    return true;
}
void Epoll::objectChange(EpollObject* pObject, uint32 events){
    int fd = pObject->getSocketFD();
    if(fd < 0){
        return;
    }
    uint32 handle = pObject->getHandle();
    if(handle == 0){
        LOG_ERROR("objectChange handle == 0");
        return;
    }
    struct epoll_event ev;
    ev.events = events | EPOLLET;
//    ev.data.ptr = pObject;
    ev.data.u32 = handle;
    epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &ev);
}
void Epoll::objectRemove(EpollObject* pObject){
    int fd = pObject->getSocketFD();
    if(pObject->isInEpoll()){
        pObject->setIsInEpoll(false);
        epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL);
//        int ret = epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL);
//        LOG_DEBUG("objectRemove pObject=%p ret=%d(0 is success)", pObject, ret);
    }
    if(fd >= 0){
        onCloseConnect(pObject);
        pObject->closeSocket();
        onRemoveConnect(pObject);
    }
}
void Epoll::objectRemoveWithoutClose(EpollObject* pObject){
    if(pObject->isInEpoll()){
        pObject->setIsInEpoll(false);
        int fd = pObject->getSocketFD();
        epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, NULL);
        pObject->setSocketFD(-1);
        onRemoveConnect(pObject);
    }
}
bool Epoll::createEpoll(void){
//    m_epollfd = epoll_create(MAX_WAIT_EVENT_SIZE);
    m_epollfd = epoll_create1(0);
	return true;
}
void Epoll::closeEpoll(void){
    if( m_epollfd >= 0 ){
        close(m_epollfd);
        m_epollfd = -1;
    }
}
/*--------------------------------------------------------------------*/
NS_HIVE_END

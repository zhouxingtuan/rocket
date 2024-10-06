//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/11/22
// Time: 下午10:00
// To change this template use File | Settings | File Templates.
//

#include "active.h"

NS_HIVE_BEGIN

Active::Active(void) : EpollObject() {

}
Active::~Active(void){
	closeEvent();
}

bool Active::epollActive(uint32 events){
	this->readEvent();
	return true;
}
bool Active::epollIn(void){
    return true;
}
bool Active::epollOut(void){
    return true;
}
void Active::epollRemove(void){
//    m_pEpoll->objectRemove(this);
}
void Active::epollCheck(void){

}
int Active::readEvent(void){
    int64 value = 0;
    read(m_fd, &value, sizeof(int64));
	this->onActive(value);
	return 0;
}
void Active::closeEvent(void){
	if(m_fd >= 0){
		getEpoll()->objectRemove(this);
		close(m_fd);
		m_fd = -1;
	}
}
bool Active::createEvent(void){
	m_fd = eventfd(0, 0);
	if (m_fd == -1){
		LOG_ERROR("m_fd == -1");
		goto ACTIVE_CREATE_FAILED;
	}
	if (!set_non_blocking(m_fd)){
		LOG_ERROR("set_non_blocking m_fd=%d failed", m_fd);
		goto ACTIVE_CREATE_FAILED;
	}
	if (!getEpoll()->objectAddLT(this, EPOLLIN)){
		LOG_ERROR("objectAddLT failed");
		goto ACTIVE_CREATE_FAILED;
	}
	return true;
ACTIVE_CREATE_FAILED:
	closeEvent();
	return false;
}
bool Active::active(int64 value){
//		int64 value = 1;
    int nwrite = write(m_fd, &value, sizeof(int64));
    if(nwrite != sizeof(int64)){
        LOG_ERROR("write failed m_fd=%d", m_fd);
        return false;
    }
//		fprintf(stderr, "--Active::wakeup m_fd write %lld\n", value);
    return true;
}

NS_HIVE_END
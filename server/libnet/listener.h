//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/7/9
// Time: 上午11:39
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__listener__
#define __hive__listener__

#include "epoll.h"

NS_HIVE_BEGIN

class Listener;

typedef void (*AcceptSocketFunction)(int fd, uint32 ip, uint16 port, Listener* pListener);

#define CONN_TYPE_LISTENER 1

#define MAX_LISTEN_SIZE 100000

class Listener : public EpollObject
{
public:
	Listener(void);
	virtual ~Listener(void);

	// from EpollObject
	virtual bool epollActive(uint32 events);
	virtual bool epollIn(void);
	virtual bool epollOut(void);
	virtual void epollRemove(void);
	virtual void epollCheck(void);

    static uint8 objectType(void){
        return CONN_TYPE_LISTENER;
    }
    virtual uint8 getType(void){ return this->objectType(); }

	bool openSocket(void);
	void setAcceptSocketFunction(AcceptSocketFunction pFunc){ m_acceptSocketFunction = pFunc; }
	AcceptSocketFunction getAcceptSocketFunction(void){ return m_acceptSocketFunction; }
	void resetData(void);
protected:
	AcceptSocketFunction m_acceptSocketFunction;
};

NS_HIVE_END

#endif

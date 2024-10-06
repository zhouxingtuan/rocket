//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/11/22
// Time: 下午10:00
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__active__
#define __hive__active__

#include "epoll.h"
#include <sys/eventfd.h>

NS_HIVE_BEGIN

class Active : public EpollObject
{
public:
	Active(void);
	virtual ~Active(void);

	// from EpollObject
	virtual bool epollActive(uint32 events);
	virtual bool epollIn(void);
	virtual bool epollOut(void);
	virtual void epollRemove(void);
	virtual void epollCheck(void);

	virtual void onActive(int64 value) = 0;

    static uint8 objectType(void){
        return 0;
    }
    virtual uint8 getType(void){ return this->objectType(); }

    bool createEvent(void);
    bool active(int64 value);
protected:
	int readEvent(void);
	void closeEvent(void);
protected:

};

NS_HIVE_END

#endif
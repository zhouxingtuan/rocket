#ifndef __hive__epoll__
#define __hive__epoll__

//epoll & net libs
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>		/* basic system data types */
#include <sys/socket.h>		/* basic socket definitions */
#include <netinet/in.h>		/* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>		/* inet(3) functions */
#include <sys/epoll.h>		/* epoll function */
#include <fcntl.h>			/* nonblocking */

#include "base.h"
#include "timer.h"
#include "object_pool.h"

NS_HIVE_BEGIN

#define READ_BUFFER_SIZE 8192

#define MAX_WAIT_EVENT_SIZE 1024
#define EPOLL_WAIT_FLAG -1

inline const char* convert_ip2str(uint32 ip){
    struct in_addr addr;
    addr.s_addr = ip;
    return inet_ntoa(addr);
}
inline uint32 convert_str2ip(const char* ip){
    return (uint32)inet_addr(ip);
}

inline bool set_non_blocking(int fd){
    if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1){
  		return false;
    }
    return true;
}

class Epoll;
class EpollObject;
typedef int64 (*EpollObjectTimerFunction)(EpollObject* pObject);
/*--------------------------------------------------------------------*/
typedef struct ServerHandle{
    union{
        struct{
            unsigned int serverIndex : 16;      // 服务index，最多65535个
            unsigned int serverType : 8;        // 服务类型，最多255个
            unsigned int groupIndex : 8;        // 服务的分组index，最多255个
        };
        unsigned int handle : 32;
    };
public:
    ServerHandle(unsigned int h){ this->handle = h; }
    ServerHandle(void) : handle(0) {}
    ~ServerHandle(void){}

    inline unsigned int getServerIndex(void) const { return this->serverIndex; }
    inline unsigned int getServerType(void) const { return this->serverType; }
    inline unsigned int getGroupIndex(void) const { return this->groupIndex; }
    inline unsigned int getHandle(void) const { return this->handle; }
    inline void setServerIndex(unsigned int index){ this->serverIndex = index; }
    inline void setServerType(unsigned int type){ this->serverType = type; }
    inline void setGroupIndex(unsigned int groupIndex){ this->groupIndex = groupIndex; }
    inline bool operator==(unsigned int h) const { return (this->handle == h); }
    inline bool operator==(const ServerHandle& h) const{ return (this->handle == h.handle); }
    inline bool operator!=(unsigned int h) const { return (this->handle != h); }
    inline bool operator!=(const ServerHandle& h) const{ return (this->handle != h.handle); }
    inline bool operator<(unsigned int h) const { return (this->handle < h); }
    inline bool operator<(const ServerHandle& h) const { return (this->handle < h.handle); }
    inline ServerHandle& operator=(unsigned int h){ this->handle = h; return *this; }
    inline ServerHandle& operator=(ServerHandle& h){ this->handle = h.handle; return *this; }
}ServerHandle;
/*--------------------------------------------------------------------*/
class EpollObject : public TimerObject
{
protected:
    Epoll* m_pEpoll;
    EpollObjectTimerFunction m_timerCallback;

    ObjectHandle m_handle;
    ServerHandle m_serverHandle;
    uint32 m_callbackID;
	int m_fd;

    uint32 m_ip;
    uint16 m_port;
    bool m_isNeedEncrypt;			// 是否需要解密
    bool m_isInEpoll;
public:
	EpollObject(void);
	virtual ~EpollObject(void);

    // from TimerObject
    virtual int64 timerCallback(void);

	// 对象被epoll激活时调用的函数，用于Listener
	virtual bool epollActive(uint32 events) = 0;
	// 处理数据读
	virtual bool epollIn(void) = 0;
	// 处理数据写
	virtual bool epollOut(void) = 0;
	// 处理移除socket
	virtual void epollRemove(void);
	// 处理socket最后的状态
	virtual void epollCheck(void) = 0;

    static uint8 objectType(void){
        return 0;
    }
    virtual uint8 getType(void){ return this->objectType(); }
    virtual void setParam(void){  }
    virtual void clearData(void);

	int epollRemoveWithoutClose(void);  // remove without close
	void closeSocket(void);

    bool setTimeout(int64 timeCount, EpollObjectTimerFunction callback);
    void setEpoll(Epoll* pEpoll){ m_pEpoll = pEpoll; }
    Epoll* getEpoll(void){ return m_pEpoll; }
	inline int getSocketFD(void) const { return m_fd; }
	inline void setSocketFD(int fd){ m_fd = fd; }
	inline void setSocket(uint32 ip, uint16 port){
        m_ip = ip;
        m_port = port;
	}
    inline void setIP(uint32 ip){ m_ip = ip; }
	inline void setPort(uint16 port){ m_port = port; }
    inline uint32 getIP(void) const { return m_ip; }
	inline uint16 getPort(void) const { return m_port; }
	inline bool isNeedEncrypt(void) const { return m_isNeedEncrypt; }
	inline void setIsNeedEncrypt(bool need) { m_isNeedEncrypt = need; }
	inline void setIsInEpoll(bool isIn) { m_isInEpoll = isIn; }
	inline bool isInEpoll(void) const { return m_isInEpoll; }

    inline void setServerHandle(uint32 serverHandle){ m_serverHandle = serverHandle; }
    inline uint32 getServerHandle(void){ return m_serverHandle.handle; }
    inline void setCallbackID(uint32 callbackID){ m_callbackID = callbackID; }
    inline uint32 getCallbackID(void){ return m_callbackID; }
    inline unsigned int getHandle(void){ return m_handle.handle; }
    inline void setHandle(unsigned int handle){ m_handle = handle; }
    inline ObjectHandle getHandleObject(void){ return m_handle; }
    inline void setHandleObject(ObjectHandle handle){ m_handle = handle; }
};
/*--------------------------------------------------------------------*/
class Epoll : public RefObject
{
protected:
    int m_epollfd;
    bool m_activeNextFrameEnd;
    Timer* m_pTimer;
    struct epoll_event m_events[MAX_WAIT_EVENT_SIZE];
public:
    Epoll(void);
    virtual ~Epoll(void);

    virtual EpollObject* getObject(uint32 handle) = 0;
    virtual void onCloseConnect(EpollObject* pObject) = 0;
    virtual void onNextFrameActive(void) = 0;
    virtual void onRemoveConnect(EpollObject* pObject) = 0;

    void update(void);

	bool objectAddLT(EpollObject* pObject, uint32 events=EPOLLIN);
	// EPOLLIN | EPOLLOUT | EPOLLET
	bool objectAdd(EpollObject* pObject, uint32 events=EPOLLIN);
	void objectChange(EpollObject* pObject, uint32 events);
	void objectRemove(EpollObject* pObject);
    void checkConnectObject(EpollObject* pObject, uint32 events);
    void objectRemoveWithoutClose(EpollObject* pObject);

    void setActiveNextFrameEnd(bool active){ m_activeNextFrameEnd = active; }
    bool isActiveNextFrameEnd(void){ return m_activeNextFrameEnd; }
    Timer* getTimer(void){ return m_pTimer; }
    void setTimer(Timer* pTimer){ m_pTimer = pTimer; }
protected:
    bool createEpoll(void);
    void closeEpoll(void);
};
/*--------------------------------------------------------------------*/


NS_HIVE_END

#endif
#ifndef __hive__manager__
#define __hive__manager__

#include "common.h"
#include "epoll.h"
#include "active.h"
#include "listener.h"
#include "http_parser.h"
#include "http_common.h"
#include "http.h"
#include "https.h"
#include "http_client.h"
#include "https_client.h"

#include <openssl/conf.h>

NS_HIVE_BEGIN

#define HTTP_HANDLE_TIMEOUT 30000
#define HTTP_CHECK_TIMEOUT 12000

/*--------------------------------------------------------------------*/
#define OBJECT_TYPE_EPOLL_ACTIVE 8
class EpollActive : public Active
{
public:
    EpollActive(void) : Active(){}
    virtual ~EpollActive(void){}
    // from TimerObject
    virtual int64 timerCallback(void){ return -1; }
    // from Active
    virtual void onActive(int64 value);

    static uint8 objectType(void){
        return OBJECT_TYPE_EPOLL_ACTIVE;
    }
    virtual uint8 getType(void){ return this->objectType(); }
protected:
};
/*--------------------------------------------------------------------*/
#define OBJECT_TYPE_TIMER_HANDLER 9
class Handler;
class TimerHandler : public TimerObject
{
protected:
    Handler* m_pHandler;
    ObjectHandle m_handle;
    uint32 m_callbackID;
public:
    TimerHandler(void);
    virtual ~TimerHandler(void);

	// from TimerObject
	virtual int64 timerCallback(void);

    static uint8 objectType(void){
        return OBJECT_TYPE_TIMER_HANDLER;
    }
    virtual uint8 getType(void){ return this->objectType(); }
    virtual void setParam(void){}
    virtual void clearData(void){}
    inline void setCallbackID(uint32 callbackID){ m_callbackID = callbackID; }
    inline uint32 getCallbackID(void){ return m_callbackID; }
    inline unsigned int getHandle(void){ return m_handle.handle; }
    inline void setHandle(unsigned int handle){ m_handle = handle; }
	void setHandler(Handler* pHandler){ m_pHandler = pHandler; }
	Handler* getHandler(void){ return m_pHandler; }
};

/*--------------------------------------------------------------------*/
class Manager;
class Handler
{
protected:
    Manager* m_pManager;
public:
    Handler(void) : m_pManager(NULL) {}
    virtual ~Handler(void) {}

    virtual int64 onTimerUpdate(uint32 callbackID, uint32 handle) = 0;		// 更新定时器
    virtual void onNextFrameActive(void) = 0;
    virtual void onOpenConnect(EpollObject* pObject) = 0;
    virtual void onCloseConnect(EpollObject* pObject) = 0;
    virtual void onReceiveWebSocket(HttpCommon* pObject, const char* ptr, int length) = 0;
    virtual void onReceiveHttp(Http* pObject) = 0;
    virtual void onHttpResponse(HttpClient* pObject, const char* ptr, int length, bool isRequestOK) = 0;
    virtual void onHandleBuffer(unsigned short serverIndex, Buffer* pBuffer) = 0;
    virtual bool handleOpenHttp(int fd, uint32 ip, uint16 port) = 0;
    virtual bool handleOpenHttps(int fd, uint32 ip, uint16 port) = 0;

    // 线程间通信
    virtual void broadcast(const char* ptr, int length) = 0;
    virtual bool send(const char* ptr, int length, unsigned short serverIndex) = 0;

    uint32 startTimer(uint32 callbackID, int64 timeCount, int64 fromTime);	// 开始一个计时器
    bool removeTimer(uint32 handle);						// 移除计时器
    bool changeTimerCallbackID(uint32 handle, uint32 callbackID);		// 更改计时器的回调ID
    bool changeTimer(uint32 handle, uint32 callbackID, int64 timeCount, int64 fromTime);		// 更改计时器的时间
    int64 getTimerLeft(uint32 handle);						// 获取计时器剩余时间

    void setNextFrameActive(void);
    void setManager(Manager* pManager){ m_pManager = pManager; }
    Manager* getManager(void){ return m_pManager; }
};
/*--------------------------------------------------------------------*/
typedef struct ManagerEvent{
    union {
        Buffer* pBuffer;
        struct{
            int fd;
            uint32 ip;
        };
    };
    uint16 port;
    bool isNeedEncrypt;
    char operateType;       // 0 无 1 Buffer 2 Accept 3 Http 4 Https
    uint32 serverHandle;
    ManagerEvent(void) : pBuffer(NULL), port(0), isNeedEncrypt(false), operateType(0), serverHandle(0){}
}ManagerEvent;
/*--------------------------------------------------------------------*/
typedef std::vector<ManagerEvent> ManagerEventVector;
class Manager : public Epoll, public Sync
{
protected:
	static SSL_CTX* m_pSSLCTX;
	static SSL_CTX* m_pClientSSLCTX;
    static std::string m_password;						// 服务间连接验证密码
	static std::string m_publicKey;					// 公共密钥
	static std::string m_privateKey;					// 私钥
	static std::string m_httpTimeoutResponse;          // http超时回复
	static std::string m_httpClientTimeoutResponse;    // http client超时回复

    ManagerEventVector m_eventQueue;
    ManagerEventVector m_eventQueueSwap;
    Handler* m_pHandler;
    ObjectPool* m_pObjectPool;
    EpollActive* m_pActive;
    http_parser_settings m_settings;
    RequestData m_tempRequestData;
	ServerHandle m_serverHandle;                // 当前所属的服务
public:
    Manager(void);
    virtual ~Manager(void);

	static void onAcceptHttp(int fd, uint32 ip, uint16 port, Listener* pListener);
	static void onAcceptHttps(int fd, uint32 ip, uint16 port, Listener* pListener);

    // from Epoll
    virtual EpollObject* getObject(uint32 handle);
    virtual void onCloseConnect(EpollObject* pObject);
    virtual void onNextFrameActive(void);
    virtual void onRemoveConnect(EpollObject* pObject);

    void sendHandleBuffer(unsigned short serverIndex, Buffer* pBuffer);
    void transferHttp(int fd, uint32 ip, uint16 port);
    void transferHttps(int fd, uint32 ip, uint16 port);
    bool closeConnect(uint32 handle);
	uint32 openHttpListener(const std::string& ip, uint16 port);
	uint32 openHttpsListener(const std::string& ip, uint16 port);
	uint32 openListener(const std::string& ip, uint16 port, AcceptSocketFunction pFunc, bool isNeedEncrypt);
	uint32 openWebSocket(const char* url, uint32 callbackID, uint32 serverHandle, uint32 phonyIP);
	bool sendWebSocket(uint32 handle, const char* ptr, int length);
	bool bindConnect(uint32 handle, uint32 serverHandle, uint32 callbackID);
	uint32 sendHttpRequest(uint32 handle, RequestData *pRequest);
    bool responseHttp(uint32 handle, const char* ptr, int length);

    void onOpenConnect(EpollObject* pObject);
    void onReceiveWebSocket(HttpCommon* pObject, const char* ptr, int length);
    void onReceiveHttp(Http* pObject);
    void onHttpResponse(HttpClient* pObject, const char* ptr, int length, bool isRequestOK);

    bool active(int64 value);
    void onActive(int64 value);
    void initialize(void);
    void destroy(void);
    void addManagerEvent(const ManagerEvent& event);

    inline http_parser_settings* getSettings(void) { return &m_settings; }
	inline void setPassword(const std::string& pwd){ m_password = pwd; }
	inline const std::string& getPassword(void) const { return m_password; }
	inline void setHttpTimeoutResponse(const std::string& response){ m_httpTimeoutResponse = response; }
	inline const std::string& getHttpTimeoutResponse(void) const { return m_httpTimeoutResponse; }
	inline void setHttpClientTimeoutResponse(const std::string& response){ m_httpClientTimeoutResponse = response; }
	inline const std::string& getHttpClientTimeoutResponse(void) const { return m_httpClientTimeoutResponse; }
	inline void setPublicKey(const std::string& key){ m_publicKey = key; }
	inline const std::string& getPublicKey(void) const { return m_publicKey; }
	inline void setPrivateKey(const std::string& key){ m_privateKey = key; }
	inline const std::string& getPrivateKey(void) const { return m_privateKey; }
	inline SSL_CTX* getSSLCTX(void){ return m_pSSLCTX; }
	inline SSL_CTX* getClientSSLCTX(void){ return m_pClientSSLCTX; }
	void setHandler(Handler* pHandler){ m_pHandler = pHandler; }
	Handler* getHandler(void){ return m_pHandler; }
	ObjectPool* getObjectPool(void){ return m_pObjectPool; }
	RequestData* getRequestData(void){ return &m_tempRequestData; }
	unsigned int getServerHandle(void){ return m_serverHandle.handle; }
	void setServerHandle(unsigned char groupIndex, unsigned char serverType, unsigned short serverIndex){
	    m_serverHandle.setGroupIndex(groupIndex);
	    m_serverHandle.setServerType(serverType);
	    m_serverHandle.setServerIndex(serverIndex);
	}
	unsigned char getGroupIndex(void){ return m_serverHandle.getGroupIndex();}
	unsigned char getServerType(void){ return m_serverHandle.getServerType(); }
	unsigned short getServerIndex(void){ return m_serverHandle.getServerIndex(); }
    void setServerIndex(unsigned short index){ m_serverHandle.setServerIndex(index); }
    void setServerType(unsigned char type){ m_serverHandle.setServerType(type); }
    void setGroupIndex(unsigned char groupIndex){ m_serverHandle.setGroupIndex(groupIndex); }

	bool initHttpsCertificate(const char* publicKey, const char* privateKey);
protected:
    void releaseManagerEvent(ManagerEventVector& eventQueue);
    void openHttp(int fd, uint32 ip, uint16 port);
    void openHttps(int fd, uint32 ip, uint16 port);

    void initializeHttp(void);
    void initializeOpenSSL(void);
    void initializeHttps(void);
};
/*--------------------------------------------------------------------*/
NS_HIVE_END

#endif

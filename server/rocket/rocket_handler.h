#ifndef __hive__rocket_handler__
#define __hive__rocket_handler__

#include "common.h"
#include "net.h"
#include "script.h"

NS_HIVE_BEGIN

class MainHandler;
class RocketHandler : public Handler, public Thread
{
protected:
    MainHandler* m_pMainHandler;
    Script* m_pScript;
    static std::string m_scriptFileName;
    static std::string m_moduleName;
public:
    RocketHandler(void);
    virtual ~RocketHandler(void);

    // from Handler
    virtual int threadFunction(void);

    // from Handler
    virtual int64 onTimerUpdate(uint32 callbackID, uint32 handle);
    virtual void onNextFrameActive(void);
    virtual void onOpenConnect(EpollObject* pObject);
    virtual void onCloseConnect(EpollObject* pObject);
    virtual void onReceiveWebSocket(HttpCommon* pObject, const char* ptr, int length);
    virtual void onReceiveHttp(Http* pObject);
    virtual void onHttpResponse(HttpClient* pObject, const char* ptr, int length, bool isRequestOK);

    virtual void onHandleBuffer(unsigned short serverIndex, Buffer* pBuffer);
    virtual bool handleOpenHttp(int fd, uint32 ip, uint16 port);
    virtual bool handleOpenHttps(int fd, uint32 ip, uint16 port);

    // 线程间通信
    virtual void broadcast(const char* ptr, int length);
    virtual bool send(const char* ptr, int length, unsigned short serverIndex);

    static void setScriptFileName(const std::string& scriptFileName){
        m_scriptFileName = scriptFileName;
    }
    static void setModuleName(const std::string& moduleName){
        m_moduleName = moduleName;
    }
    bool initializeScript(const std::string& scriptFileName, const std::string& moduleName);

    void setMainHandler(MainHandler* pHandler){ m_pMainHandler = pHandler; }
    MainHandler* getMainHandler(void){ return m_pMainHandler; }
};

NS_HIVE_END

#endif



#ifndef __hive__main_handler__
#define __hive__main_handler__

#include "common.h"
#include "net.h"
#include "rocket_handler.h"

NS_HIVE_BEGIN

class MainHandler : public Handler
{
protected:
    std::vector<Manager*> m_managerArray;
    int m_nextManagerIndex;
public:
    MainHandler(void);
    virtual ~MainHandler(void);
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

    virtual void broadcast(const char* ptr, int length){}
    virtual bool send(const char* ptr, int length, unsigned short serverIndex){ return false; }

    void broadcast(const char* ptr, int length, uint16 sourceServerIndex);
    bool send(const char* ptr, int length, uint16 sourceServerIndex, uint16 destinationServerIndex);
    void initialize(std::vector<RocketHandler*>& vecThread, std::string& private_key, std::string& public_pem);
    void startLoop(void);
protected:
    Manager* getNextManager(void);
};

NS_HIVE_END


#endif

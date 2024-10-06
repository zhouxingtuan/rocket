
#include "main_handler.h"

NS_HIVE_BEGIN

MainHandler::MainHandler(void) : Handler()
    , m_nextManagerIndex(-1)
    {

}
MainHandler::~MainHandler(void){
    Manager* pManager;
    Handler* pHandler;
    for(int i=0; i<(int)m_managerArray.size(); ++i){
        pManager = m_managerArray[i];
        if(i > 0){
            pHandler = pManager->getHandler();
            if(NULL != pHandler){
                delete pHandler;
            }
        }
        if(NULL != pManager){
            delete pManager;
        }
    }
    m_managerArray.clear();
    SAFE_DELETE(m_pManager);
}

int64 MainHandler::onTimerUpdate(uint32 callbackID, uint32 handle){
    return -1;
}
void MainHandler::onNextFrameActive(void){

}
void MainHandler::onOpenConnect(EpollObject* pObject){

}
void MainHandler::onCloseConnect(EpollObject* pObject){
//    LOG_DEBUG("onCloseConnect handle=%u", pObject->getHandle());
}
void MainHandler::onReceiveWebSocket(HttpCommon* pObject, const char* ptr, int length){

}
void MainHandler::onReceiveHttp(Http* pObject){

}
void MainHandler::onHttpResponse(HttpClient* pObject, const char* ptr, int length, bool isRequestOK){

}
void MainHandler::onHandleBuffer(unsigned short serverIndex, Buffer* pBuffer){

}
bool MainHandler::handleOpenHttp(int fd, uint32 ip, uint16 port){
    Manager* pManager = getNextManager();
    if(NULL == pManager){
        return false;
    }
    pManager->transferHttp(fd, ip, port);
    return true;
}
bool MainHandler::handleOpenHttps(int fd, uint32 ip, uint16 port){
    Manager* pManager = getNextManager();
    if(NULL == pManager){
        return false;
    }
    pManager->transferHttps(fd, ip, port);
    return true;
}
// 服务间通信
void MainHandler::broadcast(const char* ptr, int length, uint16 sourceServerIndex){
    Buffer* pBuffer = new Buffer();
    pBuffer->retain();
    pBuffer->write(ptr, length, 0);
    for(int i=0; i<(int)m_managerArray.size(); ++i){
        m_managerArray[i]->sendHandleBuffer(sourceServerIndex, pBuffer);
    }
    pBuffer->release();
}
bool MainHandler::send(const char* ptr, int length, uint16 sourceServerIndex, uint16 destinationServerIndex){
    if(destinationServerIndex >= m_managerArray.size()){
        return false;
    }
    Buffer* pBuffer = new Buffer();
    pBuffer->retain();
    pBuffer->write(ptr, length, 0);
    m_managerArray[destinationServerIndex]->sendHandleBuffer(sourceServerIndex, pBuffer);
    pBuffer->release();
    return true;
}

void MainHandler::initialize(std::vector<RocketHandler*>& vecThread, std::string& private_key, std::string& public_pem){
    Manager* pManager;
    RocketHandler* pThreadHandler;
    if(getManager() != NULL){
        return;
    }
    pManager = new Manager();
    pManager->setHandler(this);
    pManager->setPrivateKey(private_key);
    pManager->setPublicKey(public_pem);
    pManager->setServerIndex(0);
    pManager->initialize();
    setManager(pManager);
    int threadNumber = (int)vecThread.size();
    m_managerArray.resize(threadNumber, NULL);
    for(int index=0; index < threadNumber; ++index){
        pThreadHandler = vecThread[index];
        pThreadHandler->setMainHandler(this);
        pManager = new Manager();
        pManager->setHandler(pThreadHandler);
        pThreadHandler->setManager(pManager);
        pManager->setServerIndex(index);
        pManager->initialize();
        m_managerArray[index] = pManager;
    }
}
void MainHandler::startLoop(void){
    getManager()->update();
}
Manager* MainHandler::getNextManager(void){
    if(m_managerArray.size() <= 0){
        return NULL;
    }
    m_nextManagerIndex++;
    if(m_nextManagerIndex >= (int)m_managerArray.size()){
        m_nextManagerIndex = 0;
    }
    return m_managerArray[m_nextManagerIndex];
}

NS_HIVE_END

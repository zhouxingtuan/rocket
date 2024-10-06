
#include "rocket_handler.h"
#include "main_handler.h"

NS_HIVE_BEGIN

std::string RocketHandler::m_scriptFileName = "";
std::string RocketHandler::m_moduleName = "";

RocketHandler::RocketHandler(void) : Handler(), Thread(), m_pMainHandler(NULL), m_pScript(NULL) {

}
RocketHandler::~RocketHandler(void){
    SAFE_DELETE(m_pScript);
}
int64 RocketHandler::onTimerUpdate(uint32 callbackID, uint32 handle){
	m_pScript->pushFunction("onTimerUpdate");
	m_pScript->pushNumber(callbackID);
	m_pScript->pushNumber(handle);
	m_pScript->executeFunction(1);
	int64 timeCount = -1;
	if(m_pScript->isNumber(1)){
		timeCount = m_pScript->getInt64(1);
	}else{
		LOG_ERROR("onTimerUpdate callbackID=%d return not number", callbackID);
	}
	return timeCount;
}
void RocketHandler::onNextFrameActive(void){
    m_pScript->pushFunction("onNextFrameActive");
    m_pScript->executeFunction(0);
}
void RocketHandler::onOpenConnect(EpollObject* pObject){
    uint32 handle = pObject->getHandle();
    uint32 serverHandle = pObject->getServerHandle();
    uint32 callbackID = pObject->getCallbackID();
	m_pScript->pushFunction("onOpenConnect");
	m_pScript->pushNumber(handle);
	m_pScript->pushNumber(serverHandle);
	m_pScript->pushNumber(callbackID);
	m_pScript->executeFunction(0);
}
void RocketHandler::onCloseConnect(EpollObject* pObject){
    uint32 handle = pObject->getHandle();
    uint32 callbackID = pObject->getCallbackID();
    if(0 == callbackID){
        LOG_DEBUG("onCloseConnect 0 == callbackID handle=%u", handle);
        return;
    }
    uint32 serverHandle = pObject->getServerHandle();
	m_pScript->pushFunction("onCloseConnect");
	m_pScript->pushNumber(handle);
	m_pScript->pushNumber(serverHandle);
	m_pScript->pushNumber(callbackID);
	m_pScript->executeFunction(0);
}
// 需要把数据拷贝走并记录
void RocketHandler::onReceiveWebSocket(HttpCommon* pObject, const char* ptr, int length){
    uint32 handle = pObject->getHandle();
    uint32 serverHandle = pObject->getServerHandle();
    uint32 ip = pObject->getIP();
    m_pScript->pushFunction("onReceiveWebSocket");
    m_pScript->pushNumber(handle);
    m_pScript->pushNumber(serverHandle);
    m_pScript->pushNumber(ip);
    m_pScript->pushString(ptr, length);
    m_pScript->executeFunction(0);
}
void RocketHandler::onReceiveHttp(Http* pObject){
    uint32 handle = pObject->getHandle();
    uint32 serverHandle = pObject->getServerHandle();
    uint32 ip = pObject->getIP();
    const char* url = pObject->getUrl();
    const char* body = pObject->getBody();
    int bodyLength = pObject->getBodyLength();
    int urlLength = pObject->getUrlLength();
//    LOG_DEBUG("onReceiveHttp handle=%u url=%s urlLength=%d bodyLength=%d body=%s",
//        pObject->getHandle(), std::string(url, urlLength).c_str(), urlLength, bodyLength, std::string(body, bodyLength).c_str());
    m_pScript->pushFunction("onReceiveHttp");
    m_pScript->pushNumber(handle);
    m_pScript->pushNumber(serverHandle);
    m_pScript->pushNumber(ip);
    m_pScript->pushString(http_method_str((http_method)pObject->getParser()->method));
    m_pScript->pushString(url, urlLength);
    m_pScript->pushString(body, bodyLength);
    HStringMap& temp = pObject->getHeaders();
    m_pScript->pushTable((uint32)temp.size());
    for( auto &p : temp ){
        m_pScript->pushString(p.first.toString());
        m_pScript->pushString(p.second.toString());
        m_pScript->setTableElement();
    }
    m_pScript->executeFunction(0);
}
// 需要把数据拷贝走并记录
void RocketHandler::onHttpResponse(HttpClient* pObject, const char* ptr, int length, bool isRequestOK){
    uint32 handle = pObject->getHandle();
    uint32 serverHandle = pObject->getServerHandle();
    uint32 callbackID = pObject->getCallbackID();
    m_pScript->pushFunction("onHttpResponse");
    m_pScript->pushNumber(handle);
    m_pScript->pushNumber(serverHandle);
    m_pScript->pushNumber(callbackID);
    m_pScript->pushBool(isRequestOK);
    m_pScript->pushString(ptr, length);
    HStringMap& temp = pObject->getHeaders();
    m_pScript->pushTable((uint32)temp.size());
    for( auto &p : temp ){
        m_pScript->pushString(p.first.toString());
        m_pScript->pushString(p.second.toString());
        m_pScript->setTableElement();
    }
    m_pScript->executeFunction(0);
}

void RocketHandler::onHandleBuffer(unsigned short serverIndex, Buffer* pBuffer){
    m_pScript->pushFunction("onHandleBuffer");
    m_pScript->pushNumber(serverIndex);
    m_pScript->pushString(pBuffer->data(), pBuffer->size());
    m_pScript->executeFunction(0);
}
bool RocketHandler::handleOpenHttp(int fd, uint32 ip, uint16 port){

    return false;
}
bool RocketHandler::handleOpenHttps(int fd, uint32 ip, uint16 port){

    return false;
}
// 服务间通信
void RocketHandler::broadcast(const char* ptr, int length){
    getMainHandler()->broadcast(ptr, length, getManager()->getServerIndex());
}
bool RocketHandler::send(const char* ptr, int length, unsigned short serverIndex){
    return getMainHandler()->send(ptr, length, getManager()->getServerIndex(), serverIndex);
}

bool RocketHandler::initializeScript(const std::string& scriptFileName, const std::string& moduleName){
    m_pScript = new Script();
    m_pScript->setState(NULL);
    m_pScript->pushUserData(this, "Handler");
    m_pScript->setGlobal("g_pNode");
	if( !m_pScript->requireFile(scriptFileName) ){
	    LOG_ERROR("m_pScript requireFile failed file=%s", scriptFileName.c_str());
	    return false;
	}
	m_pScript->pushFunction("onInitialize");
	m_pScript->pushUserData(this, "Handler");
	m_pScript->pushString(moduleName);
	m_pScript->executeFunction(0);
	return true;
}
int RocketHandler::threadFunction(void){
    initializeScript(m_scriptFileName, m_moduleName);
    getManager()->update();
    return 0;
}

NS_HIVE_END

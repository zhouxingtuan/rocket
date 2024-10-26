
#include "manager.h"

NS_HIVE_BEGIN
/*--------------------------------------------------------------------*/
static pthread_mutex_t *g_lock_cs = NULL;
//static long *lock_count = NULL;
pthread_t pthreads_thread_id(void){
    pthread_t ret;
    ret=pthread_self();
    return(ret);
}
void pthreads_locking_callback(int mode, int type, const char *file, int line){
    if (mode & CRYPTO_LOCK){
        pthread_mutex_lock(&(g_lock_cs[type]));
//        lock_count[type]++;
    }else{
        pthread_mutex_unlock(&(g_lock_cs[type]));
    }
}
/*--------------------------------------------------------------------*/
void EpollActive::onActive(int64 value){
    ((Manager*)getEpoll())->onActive(value);
}
/*--------------------------------------------------------------------*/
TimerHandler::TimerHandler(void) : TimerObject(), m_pHandler(NULL), m_handle(0), m_callbackID(0) {

}
TimerHandler::~TimerHandler(void){

}
int64 TimerHandler::timerCallback(void){
    int64 timeCount = m_pHandler->onTimerUpdate(m_callbackID, m_handle.handle);
	// 如果在 m_callbackFunction 中重新设定了当前定时器， 那么 isActive 为true
	if(timeCount < 0 && !this->isActive()){
		m_pHandler->getManager()->getObjectPool()->destroy<TimerHandler>(this);
	}
	return timeCount;
}
/*--------------------------------------------------------------------*/
// 开始一个计时器
uint32 Handler::startTimer(uint32 callbackID, int64 timeCount, int64 fromTime){
    TimerHandler* pTimerHandler = getManager()->getObjectPool()->create<TimerHandler>();
    if(NULL == pTimerHandler){
        LOG_ERROR("startTimer failed callbackID=%u timeCount=%lld fromTime=%lld", callbackID, timeCount, fromTime);
        return 0;
    }
    pTimerHandler->setHandler(this);
    pTimerHandler->setCallbackID(callbackID);
    pTimerHandler->setTimer(timeCount, fromTime, getManager()->getTimer());
    return pTimerHandler->getHandle();
}
// 移除计时器
bool Handler::removeTimer(uint32 handle){
    TimerHandler* pTimerHandler = getManager()->getObjectPool()->getObject<TimerHandler>(handle);
    if(NULL == pTimerHandler){
        LOG_ERROR("removeTimer failed handle=%u", handle);
        return false;
    }
    pTimerHandler->clearTimer();
    getManager()->getObjectPool()->destroy<TimerHandler>(pTimerHandler);
    return true;
}
// 更改计时器的回调ID
bool Handler::changeTimerCallbackID(uint32 handle, uint32 callbackID){
    TimerHandler* pTimerHandler = getManager()->getObjectPool()->getObject<TimerHandler>(handle);
    if(NULL == pTimerHandler){
        LOG_ERROR("changeTimerCallbackID failed handle=%u callbackID=%u", handle, callbackID);
        return false;
    }
    pTimerHandler->setCallbackID(callbackID);
    return true;
}
// 更改计时器的时间
bool Handler::changeTimer(uint32 handle, uint32 callbackID, int64 timeCount, int64 fromTime){
    TimerHandler* pTimerHandler = getManager()->getObjectPool()->getObject<TimerHandler>(handle);
    if(NULL == pTimerHandler){
        LOG_ERROR("changeTimer failed handle=%u callbackID=%u timeCount=%lld fromTime=%lld", handle, callbackID, timeCount, fromTime);
        return false;
    }
    pTimerHandler->setTimer(timeCount, fromTime, getManager()->getTimer());
    return true;
}
// 获取计时器剩余时间
int64 Handler::getTimerLeft(uint32 handle){
    TimerHandler* pTimerHandler = getManager()->getObjectPool()->getObject<TimerHandler>(handle);
    if(NULL == pTimerHandler){
        LOG_ERROR("getTimerLeft failed handle=%u", handle);
        return false;
    }
	return pTimerHandler->getLeftTime();
}
void Handler::setNextFrameActive(void){
    getManager()->setActiveNextFrameEnd(true);
}
/*--------------------------------------------------------------------*/
static char g_httpTimeoutResponse[] = "HTTP/1.1 200 OK\r\n"
                                      "Connection: close\r\n"
                                      "Content-Type: application/json\r\n"
                                      "Content-Length: 37\r\n"
                                      "\r\n"
                                      "{\"error_code\":-1,\"msg\":\"request timeout\"}";
static char g_httpClientTimeoutResponse[] = "{\"error_code\":-1,\"msg\":\"request timeout\"}";
SSL_CTX* Manager::m_pSSLCTX = NULL;
SSL_CTX* Manager::m_pClientSSLCTX = NULL;
std::string Manager::m_password = std::string("bd8c53d8c5a077d955ca16fa3677c3c8");
std::string Manager::m_publicKey = std::string("");
std::string Manager::m_privateKey = std::string("");
std::string Manager::m_httpTimeoutResponse = std::string(g_httpTimeoutResponse);
std::string Manager::m_httpClientTimeoutResponse = std::string(g_httpClientTimeoutResponse);
Manager::Manager(void) : Epoll(),
    m_pHandler(NULL),
    m_pObjectPool(NULL),
    m_pActive(NULL),
//    m_pSSLCTX(NULL), m_pClientSSLCTX(NULL),
    m_serverHandle(0){

}
Manager::~Manager(void){
    destroy();
}

void Manager::onAcceptHttp(int fd, uint32 ip, uint16 port, Listener* pListener){
    ((Manager*)pListener->getEpoll())->openHttp(fd, ip, port);
}
void Manager::onAcceptHttps(int fd, uint32 ip, uint16 port, Listener* pListener){
    ((Manager*)pListener->getEpoll())->openHttps(fd, ip, port);
}

// from Epoll
EpollObject* Manager::getObject(uint32 handle){
    return m_pObjectPool->getObject<EpollObject>(handle);
}
void Manager::onCloseConnect(EpollObject* pObject){
//    LOG_DEBUG("onCloseConnect handle=%u", pObject->getHandle());
    m_pHandler->onCloseConnect(pObject);
}
void Manager::onNextFrameActive(void){
    m_pHandler->onNextFrameActive();
}
void Manager::onRemoveConnect(EpollObject* pObject){
//    LOG_DEBUG("onRemoveConnect handle=%u", pObject->getHandle());
    // 对应连接创建时的 create
    m_pObjectPool->destroy<EpollObject>(pObject);
}

void Manager::sendHandleBuffer(unsigned short serverIndex, Buffer* pBuffer){
    pBuffer->retain();  // 出队时需要retain
    ManagerEvent event;
    event.operateType = 1;
    event.pBuffer = pBuffer;
    event.serverHandle = serverIndex;
    addManagerEvent(event);
}
void Manager::transferHttp(int fd, uint32 ip, uint16 port){
    ManagerEvent event;
    event.operateType = 3;
    event.fd = fd;
    event.ip = ip;
    event.port = port;
//    LOG_DEBUG("transferHttp fd=%d ip=%u port=%d", fd, ip, port);
    addManagerEvent(event);
}
void Manager::transferHttps(int fd, uint32 ip, uint16 port){
    ManagerEvent event;
    event.operateType = 4;
    event.fd = fd;
    event.ip = ip;
    event.port = port;
    addManagerEvent(event);
}
void Manager::addManagerEvent(const ManagerEvent& event){
    lock();
    m_eventQueue.push_back(event);
    unlock();
    m_pActive->active(1);
}
bool Manager::closeConnect(uint32 handle){
	EpollObject* pObject = getObject(handle);
	if(NULL == pObject){
		return false;
	}
	LOG_DEBUG("closeConnect handle=%u", handle);
    pObject->epollRemove();
    return true;
}
uint32 Manager::openHttpListener(const std::string& ip, uint16 port){
	return this->openListener(ip, port, Manager::onAcceptHttp, false);
}
uint32 Manager::openHttpsListener(const std::string& ip, uint16 port){
	return this->openListener(ip, port, Manager::onAcceptHttps, false);
}
uint32 Manager::openListener(const std::string& ip, uint16 port, AcceptSocketFunction pFunc, bool isNeedEncrypt){
//	LOG_DEBUG("try to open Listener ip=%s port=%d", ip, port);
	uint32 handle = 0;
	uint32 ip_number = convert_str2ip(ip.c_str());
	Listener* pListener = m_pObjectPool->create<Listener>();
	pListener->setEpoll(this);
	pListener->setSocket(ip_number, port);
	pListener->setAcceptSocketFunction(pFunc);
	pListener->setIsNeedEncrypt(isNeedEncrypt);
    handle = pListener->getHandle();
	do{
        if( !pListener->openSocket() ){
//            pListener->epollRemove();
            m_pObjectPool->destroy<Listener>(pListener);
            handle = 0;
            LOG_ERROR("Listener openSocket failed");
            break;
        }
        if( !objectAdd(pListener, EPOLLIN) ){
//            pListener->epollRemove();
            m_pObjectPool->destroy<Listener>(pListener);
            handle = 0;
            LOG_ERROR("Listener objectAdd to epoll failed");
            break;
        }
        LOG_INFO("openListener handle=%u fd=%d ip=%s port=%d", handle, pListener->getSocketFD(), convert_ip2str(pListener->getIP()), pListener->getPort());
	}while(0);
	return handle;
}

void Manager::onOpenConnect(EpollObject* pObject){
    LOG_DEBUG("onOpenConnect handle=%u", pObject->getHandle());
    m_pHandler->onOpenConnect(pObject);
}
void Manager::onReceiveWebSocket(HttpCommon* pObject, const char* ptr, int length){
    m_pHandler->onReceiveWebSocket(pObject, ptr, length);
}
void Manager::onReceiveHttp(Http* pObject){
    m_pHandler->onReceiveHttp(pObject);
}
void Manager::onHttpResponse(HttpClient* pObject, const char* ptr, int length, bool isRequestOK){
    m_pHandler->onHttpResponse(pObject, ptr, length, isRequestOK);
}

bool Manager::active(int64 value){
    return m_pActive->active(value);
}
void Manager::onActive(int64 value){
    // 处理当前收到的消息
    lock();
    m_eventQueue.swap(m_eventQueueSwap);
    unlock();
//    LOG_DEBUG("m_eventQueueSwap size=%d", (int)m_eventQueueSwap.size());
    for(int i=0; i < (int)m_eventQueueSwap.size(); ++i){
        ManagerEvent& event = m_eventQueueSwap[i];
//        LOG_DEBUG("onActive operateType=%d", event.operateType);
        switch(event.operateType){
            case 1:{
                if(NULL != event.pBuffer){
                    m_pHandler->onHandleBuffer(event.serverHandle, event.pBuffer);
                    event.pBuffer->release();   // 对应入队时的retain
                }
                break;
            }
            case 2:{
                break;
            }
            case 3:{
                openHttp(event.fd, event.ip, event.port);
                break;
            }
            case 4:{
                openHttps(event.fd, event.ip, event.port);
                break;
            }
        }
    }
    m_eventQueueSwap.clear();
}

void Manager::openHttp(int fd, uint32 ip, uint16 port){
//	LOG_DEBUG("fd=%d ip=%s port=%d", fd, convert_ip2str(ip), port);
    if(m_pHandler->handleOpenHttp(fd, ip, port)){
        return;
    }
	if( !set_non_blocking(fd) ){
	    close(fd);
		LOG_ERROR("openHttp set none block failed fd=%d ip=%s port=%d", fd, convert_ip2str(ip), port);
		return;
	}
	Http* pHttp = m_pObjectPool->create<Http>();
	pHttp->setEpoll(this);
	pHttp->initialize();
	pHttp->setSocketFD(fd);
	pHttp->setSocket(ip, port);
	pHttp->setState(HTTP_STATE_READ);
//	LOG_DEBUG("openHttp pHttp=%p handle=%u", pHttp, pHttp->getHandle());
    pHttp->setTimeout(HTTP_HANDLE_TIMEOUT, HttpCommon::httpConnectTimeout);
	// 读取数据
	pHttp->epollIn();
}
void Manager::openHttps(int fd, uint32 ip, uint16 port){
//	LOG_DEBUG("fd=%d ip=%s port=%d", fd, convert_ip2str(ip), port);
    if(m_pHandler->handleOpenHttps(fd, ip, port)){
        return;
    }
	Https* pHttps = m_pObjectPool->create<Https>();
	pHttps->setEpoll(this);
	pHttps->initialize();
	pHttps->setSocketFD(fd);
	pHttps->setSocket(ip, port);
	pHttps->setState(HTTP_STATE_READ);
//    LOG_DEBUG("openHttps pHttps=%p", pHttps);
    if(!pHttps->bindSSL(getSSLCTX())){
//        pHttps->epollRemove();
        m_pObjectPool->destroy<Https>(pHttps);
        LOG_ERROR("bind ssl failed fd=%d", pHttps->getSocketFD());
        return;
    }
    pHttps->setSSLState(HTTPS_SSL_CONNECTING);
    pHttps->startConnecting(true);
}
uint32 Manager::openWebSocket(const char* url, uint32 callbackID, uint32 serverHandle, uint32 phonyIP){
    m_tempRequestData.clearData();
    m_tempRequestData.m_method = "POST";
    m_tempRequestData.m_url = url;
//    m_tempRequestData.m_callbackID = callbackID;
//    m_tempRequestData.m_serverHandle = serverHandle;
    m_tempRequestData.splitURL();
    const std::string &requestType = m_tempRequestData.m_requestType;
    HttpClient *pObject;
    if(requestType == "ws"){
        pObject = m_pObjectPool->create<HttpClient>();
    }else if(requestType == "wss"){
        pObject = m_pObjectPool->create<HttpsClient>();
    }else{
        LOG_ERROR("openWebSocket error url=%s callbackID=%u serverHandle=%u", url, callbackID, serverHandle);
        return 0;
    }
    pObject->setEpoll(this);
    pObject->initialize();
    pObject->setIP(phonyIP);
    pObject->setCallbackID(callbackID);
    pObject->setServerHandle(serverHandle);
    uint32 handle = pObject->getHandle();
//    LOG_DEBUG("openWebSocket handle=%u pObject=%p callbackID=%d", handle, pObject, m_tempRequestData.m_callbackID);
    int socketFD = 0;
    if(!pObject->connectServer(&m_tempRequestData, socketFD)){
        m_pObjectPool->destroy<HttpClient>(pObject);
        LOG_ERROR("openWebSocket connectServer failed requestType=%s", requestType.c_str());
        return 0;
    }
	pObject->setSocketFD(socketFD);
	pObject->setState(HTTP_STATE_WRITE);

    if(requestType == "ws"){
        if( !set_non_blocking(socketFD) ){
            m_pObjectPool->destroy<HttpClient>(pObject);
            LOG_ERROR("openWebSocket ws set_non_blocking failed");
            return 0;
        }
        std::string requestData;
        m_tempRequestData.joinUpgradeData(requestData);
//        LOG_DEBUG("requestData=\n%s", requestData.c_str());
        pObject->setTimeout(HTTP_HANDLE_TIMEOUT, HttpCommon::httpConnectTimeout);
        pObject->responseRequestNormal(requestData.c_str(), requestData.length());
    }else if(requestType == "wss"){
	    HttpsClient *pHttpsClient = (HttpsClient*)pObject;
        if( !pHttpsClient->bindSSL(getClientSSLCTX()) ){
            m_pObjectPool->destroy<HttpClient>(pObject);
            LOG_ERROR("bind ssl failed fd=%d requestType=%s host=%s", socketFD, requestType.c_str(), m_tempRequestData.m_requestHost.c_str());
            return 0;
        }
        std::string requestData;
        m_tempRequestData.joinUpgradeData(requestData);
//        LOG_DEBUG("requestData=\n%s \n", requestData.c_str());
        pObject->responseAppend(requestData.c_str(), requestData.length());
        pHttpsClient->setSSLState(HTTPS_SSL_CONNECTING);
        pHttpsClient->startConnecting(true);
    }
//    else{
//        LOG_ERROR("not handle requestType=%s host=%s", requestType.c_str(), m_tempRequestData.m_requestHost.c_str());
//    }

    return handle;
}
bool Manager::sendWebSocket(uint32 handle, const char* ptr, int length){
    HttpCommon *pObject;
//    EpollObject* pEpollObject = getObject(handle);
    pObject = m_pObjectPool->getObject<HttpCommon>(handle);
    if(NULL == pObject){
        LOG_ERROR("sendWebSocket failed NULL == pObject handle=%u", handle);
        return false;
    }
    if(!pObject->isUpgrade()){
        LOG_ERROR("sendWebSocket failed isUpgrade false handle=%u", handle);
        return false;
    }
    pObject->responseRequestUpgrade(ptr, length);
    return true;
}
uint32 Manager::sendHttpRequest(uint32 handle, RequestData *pRequest){
    HttpClient *pObject = NULL;
    const std::string &requestType = pRequest->m_requestType;
    int socketFD = 0;
    bool isNeedConnect = false;
    if(handle > 0){
        EpollObject* pEpollObject = getObject(handle);
        if(NULL != pObject){
            if(!(pObject->getType() == CONN_TYPE_HTTP_CLIENT || pObject->getType() == CONN_TYPE_HTTPS_CLIENT)){
                LOG_ERROR("sendHttpRequest object type error type=%d", pObject->getType());
                return 0;
            }
            pObject = (HttpClient*)pEpollObject;
        }
//        pObject = (HttpClient*)m_pObjectPool->getObject<EpollObject>(handle);
    }
    if(NULL == pObject){
        if(requestType == "http"){
            pObject = m_pObjectPool->create<HttpClient>();
        }else if(requestType == "https"){
            pObject = m_pObjectPool->create<HttpsClient>();
        }else{
            LOG_ERROR("sendHttpRequest error handle=%u", handle);
            return 0;
        }
        pObject->setEpoll(this);
        pObject->initialize();
        handle = pObject->getHandle();
        isNeedConnect = true;
    }
    pObject->setIP(pRequest->m_phonyIP);
    pObject->setCallbackID(pRequest->m_callbackID);
    pObject->setServerHandle(pRequest->m_serverHandle);

//    LOG_DEBUG("sendHttpRequest handle=%u pObject=%p callbackID=%d", handle, pObject, pRequest->m_callbackID);
    if(isNeedConnect){
        if(!pObject->connectServer(pRequest, socketFD)){
            m_pObjectPool->destroy<HttpClient>(pObject);
            LOG_ERROR("sendHttpRequest connectServer failed requestType=%s", requestType.c_str());
            return 0;
        }
        pObject->setSocketFD(socketFD);
    }

	pObject->setState(HTTP_STATE_WRITE);
    if(pRequest->m_isKeepAlive){
        pObject->setUpgradeState(HTTP_UPGRADE_KEEP_ALIVE);
    }
	if(requestType == "http"){
	    if(isNeedConnect){
            if( !set_non_blocking(socketFD) ){
                m_pObjectPool->destroy<HttpClient>(pObject);
                LOG_ERROR("sendHttpRequest http set_non_blocking failed");
                return 0;
            }
	    }
        std::string requestData;
        pRequest->joinData(requestData);
//        LOG_DEBUG("requestData=\n%s", requestData.c_str());
        pObject->setTimeout(HTTP_HANDLE_TIMEOUT, HttpCommon::httpConnectTimeout);
        pObject->responseRequestNormal(requestData.c_str(), requestData.length());
	}else if(requestType == "https"){
        if(isNeedConnect){
            HttpsClient *pHttpsClient = (HttpsClient*)pObject;
            if( !pHttpsClient->bindSSL(getClientSSLCTX()) ){
                m_pObjectPool->destroy<HttpClient>(pObject);
                LOG_ERROR("bind ssl failed fd=%d requestType=%s host=%s", socketFD, requestType.c_str(), pRequest->m_requestHost.c_str());
                return 0;
            }
            std::string requestData;
            pRequest->joinData(requestData);
    //        LOG_DEBUG("requestData=\n%s \n", requestData.c_str());
            pObject->responseAppend(requestData.c_str(), requestData.length());
            pHttpsClient->setSSLState(HTTPS_SSL_CONNECTING);
            pHttpsClient->startConnecting(true);
        }else{
            std::string requestData;
            pRequest->joinData(requestData);
    //        LOG_DEBUG("requestData=\n%s", requestData.c_str());
            pObject->setTimeout(HTTP_HANDLE_TIMEOUT, HttpCommon::httpConnectTimeout);
            pObject->responseRequestNormal(requestData.c_str(), requestData.length());
        }
    }
//    else{
//        LOG_ERROR("not handle requestType=%s host=%s", requestType.c_str(), pRequest->m_requestHost.c_str());
//    }

    return handle;
}
bool Manager::bindConnect(uint32 handle, uint32 serverHandle, uint32 callbackID){
    EpollObject* pObject = getObject(handle);
    if(NULL == pObject){
        return false;
    }
    pObject->setServerHandle(serverHandle);
    pObject->setCallbackID(callbackID);
    return true;
}
bool Manager::responseHttp(uint32 handle, const char* ptr, int length){
    EpollObject* pObject = getObject(handle);
    if(NULL == pObject){
        return false;
    }
    if(!(pObject->getType() == CONN_TYPE_HTTP || pObject->getType() == CONN_TYPE_HTTPS)){
        LOG_ERROR("responseHttp object type error type=%d", pObject->getType());
        return false;
    }
    ((HttpClient*)pObject)->responseRequestNormal(ptr, length);
    return true;
}

void Manager::initialize(void){
    if(m_epollfd >= 0){
        LOG_ERROR("Manager already initialize");
        return;
    }
    if( !createEpoll() ){
        LOG_ERROR("Manager createEpoll failed");
        return;
    }
    m_eventQueue.clear();
    m_eventQueueSwap.clear();
    m_pObjectPool = new ObjectPool();
//    m_pActive = new EpollActive();
    m_pActive = m_pObjectPool->create<EpollActive>();
    m_pActive->setEpoll(this);
    if(!m_pActive->createEvent()){
//        SAFE_DELETE(m_pActive);
        m_pObjectPool->destroy<EpollActive>(m_pActive);
        LOG_ERROR("Manager m_pActive createEvent failed");
        return;
    }
    initializeHttp();
    initializeOpenSSL();
    initializeHttps();
}
void Manager::destroy(void){
//    SAFE_DELETE(m_pActive);
    m_pObjectPool->destroy<EpollActive>(m_pActive);
    SAFE_DELETE(m_pObjectPool);
    closeEpoll();
    releaseManagerEvent(m_eventQueue);
    releaseManagerEvent(m_eventQueueSwap);
//	if(NULL != m_pSSLCTX){
//		SSL_CTX_free(m_pSSLCTX);
//		m_pSSLCTX = NULL;
//	}
//	if(NULL != m_pClientSSLCTX){
//	    SSL_CTX_free(m_pClientSSLCTX);
//	    m_pClientSSLCTX = NULL;
//	}
//    if(NULL != g_lock_cs){
//        OPENSSL_free(g_lock_cs);
//        g_lock_cs = NULL;
//    }
}
void Manager::releaseManagerEvent(ManagerEventVector& eventQueue){
    for(int i=0; i < (int)eventQueue.size(); ++i){
        ManagerEvent& event = eventQueue[i];
        switch(event.operateType){
            case 1:{
                if(NULL != event.pBuffer){
                    event.pBuffer->release();
                }
                break;
            }
            case 2:
            case 3:
            case 4:{
                close(event.fd);
                break;
            }
        }
    }
    eventQueue.clear();
}
void Manager::initializeHttp(void){
	http_parser_settings_init(&m_settings);
	m_settings.on_message_begin 	= HttpCommon::onMessageBegin;
	m_settings.on_url 				= HttpCommon::onUrl;
	m_settings.on_header_field 		= HttpCommon::onHeaderField;
	m_settings.on_header_value 		= HttpCommon::onHeaderValue;
	m_settings.on_headers_complete 	= HttpCommon::onHeadersComplete;
	m_settings.on_body 				= HttpCommon::onBody;
	m_settings.on_message_complete 	= HttpCommon::onMessageComplete;
}
void Manager::initializeOpenSSL(void){
    if(NULL != m_pClientSSLCTX){
        return;
    }
#if OPENSSL_VERSION_NUMBER >= 0x10100003L
    LOG_INFO("initializeOpenSSL load in new");
    if (OPENSSL_init_ssl(OPENSSL_INIT_LOAD_CONFIG, NULL) == 0){
        LOG_ERROR("OPENSSL_init_ssl() failed!");
        return;
    }
    /*
    * OPENSSL_init_ssl() may leave errors in the error queue
    * while returning success
    */
    ERR_clear_error();
#else
    LOG_INFO("initializeOpenSSL load in old");
    OPENSSL_config(NULL);
#endif

	/* SSL 库初始化 */
	SSL_library_init();
	/* 载入所有 SSL 错误消息 */
	SSL_load_error_strings();
	/* 载入所有 SSL 算法 */
	OpenSSL_add_all_algorithms();

	// 初始化http所需
	if(NULL == m_pClientSSLCTX){
        m_pClientSSLCTX = SSL_CTX_new(SSLv23_client_method());
//        m_pClientSSLCTX = SSL_CTX_new(TLS_client_method());
        if(NULL == m_pClientSSLCTX){
            LOG_ERROR("m_pClientSSLCTX SSL_CTX_new failed");
            ERR_print_errors_fp(stderr);
        }else{
            LOG_INFO("m_pClientSSLCTX SSL_CTX_new OK");
        }
	}
}
void Manager::initializeHttps(void){
    if(NULL != m_pSSLCTX){
        return;
    }
	// 初始化https数据
	/* SSL 库初始化 */
//	SSL_library_init();
	/* 载入所有 SSL 算法 */
//	OpenSSL_add_all_algorithms();
	/* 载入所有 SSL 错误消息 */
//	SSL_load_error_strings();
	/* 以 SSLV2 和 V3 标准兼容方式产生一个 SSL_CTX ,即 SSLContentText*/
	if(NULL == m_pSSLCTX){
	    m_pSSLCTX = SSL_CTX_new(SSLv23_server_method());
//	    m_pSSLCTX = SSL_CTX_new(TLS_server_method());
		/* 也可以用 SSLv2_server_method() 或 SSLv3_server_method() 单独表示 V2 或 V3标准 */
    	if (m_pSSLCTX == NULL) {
    	    LOG_ERROR("m_pSSLCTX SSL_CTX_new failed");
    		ERR_print_errors_fp(stderr);
    	}else{
    		LOG_INFO("m_pSSLCTX SSL_CTX_new OK");
    	}
	}
	const std::string& publicKey = getPublicKey();
	const std::string& privateKey = getPrivateKey();
    if(!publicKey.empty() && !privateKey.empty()){
        if(!initHttpsCertificate(publicKey.c_str(), privateKey.c_str())){
            LOG_ERROR("epoll init https key error");
        }
    }
//	if( !publicKey.empty() && !privateKey.empty() && !initHttpsCertificate(publicKey.c_str(), privateKey.c_str()) ){
//		LOG_ERROR("epoll init https key error");
//	}
    g_lock_cs = (pthread_mutex_t*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
//    lock_count = (long*)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(long));
    for (int i=0; i<CRYPTO_num_locks(); i++){
//        lock_count[i]=0;
        pthread_mutex_init(&(g_lock_cs[i]),NULL);
    }
    CRYPTO_set_id_callback((unsigned long (*)())pthreads_thread_id);
    CRYPTO_set_locking_callback((void(*)(int mode, int type, const char *file, int line))pthreads_locking_callback);
}
bool Manager::initHttpsCertificate(const char* publicKey, const char* privateKey){
//    LOG_DEBUG("initHttpsCertificate called publicKey=%s privateKey=%s", publicKey, privateKey);
	if (m_pSSLCTX == NULL) {
		LOG_ERROR("m_pSSLCTX == NULL");
		return false;
	}
	/* 载入用户的数字证书, 此证书用来发送给客户端。 证书里包含有公钥 */
	if (SSL_CTX_use_certificate_file(m_pSSLCTX, publicKey, SSL_FILETYPE_PEM) <= 0) {
    	ERR_print_errors_fp(stderr);
    	return false;
    }
    /* 载入用户私钥 */
    if (SSL_CTX_use_PrivateKey_file(m_pSSLCTX, privateKey, SSL_FILETYPE_PEM) <= 0) {
    	ERR_print_errors_fp(stderr);
    	return false;
    }
//    if (SSL_CTX_use_PrivateKey_file_pass(ctx, "certs/sslserverkey.pem", "123456") <= 0){
//        ERR_print_errors_fp(stderr);
//        return false;
//    }
    /* 检查用户私钥是否正确 */
    if (!SSL_CTX_check_private_key(m_pSSLCTX)) {
    	ERR_print_errors_fp(stderr);
    	return false;
    }
//    LOG_DEBUG("initHttpsCertificate OK publicKey=%s privateKey=%s", publicKey, privateKey);
    return true;
}
/*--------------------------------------------------------------------*/
NS_HIVE_END

//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/11/2
// Time: 下午11:26
// To change this template use File | Settings | File Templates.
//

#include "http_common.h"
#include "manager.h"

NS_HIVE_BEGIN

HttpCommon::HttpCommon(void) : EpollObject(),
    m_readBuffer(HTTP_DEFAULT_READ_BUFFER_SIZE),
    m_writeBuffer(HTTP_DEFAULT_WRITE_BUFFER_SIZE),
    m_httpState(HTTP_STATE_IDLE),
    m_parseState(HTTP_PARSE_INIT),
    m_sslState(HTTPS_SSL_INIT),
	m_upgradeState(HTTP_UPGRADE_NONE),
	m_readOffset(0) {
	m_readBuffer.clear();
	m_writeBuffer.clear();
}
HttpCommon::~HttpCommon(void){

}

int64 HttpCommon::httpConnectTimeout(EpollObject* pObject){
    // 回复超时消息
    Manager* pManager = (Manager*)pObject->getEpoll();
    uint8 objectType = pObject->getType();
    switch(objectType){
        case CONN_TYPE_HTTP:
        case CONN_TYPE_HTTPS:
        {
            const std::string& response = pManager->getHttpTimeoutResponse();
            ((HttpCommon*)pObject)->responseRequestNormal(response.c_str(), response.length());
            break;
        }
        case CONN_TYPE_HTTP_CLIENT:
        case CONN_TYPE_HTTPS_CLIENT:
        {
            const std::string& response = pManager->getHttpClientTimeoutResponse();
            pManager->onHttpResponse((HttpClient*)pObject, response.c_str(), response.length(), false);
            break;
        }
    }

    pObject->epollRemove();
    return -1;
}

int HttpCommon::onMessageBegin(http_parser* pParser){
	((HttpCommon*)pParser->data)->setParseState(HTTP_PARSE_MESSAGE_BEGIN);
	return 0;
}
int HttpCommon::onHeadersComplete(http_parser* pParser){
	((HttpCommon*)pParser->data)->recordHeadersComplete();
	return 0;
}
int HttpCommon::onMessageComplete(http_parser* pParser){
	((HttpCommon*)pParser->data)->setParseState(HTTP_PARSE_MESSAGE_END);
	return 0;
}
int HttpCommon::onUrl(http_parser* pParser, const char* at, size_t length){
	return ((HttpCommon*)pParser->data)->recordUrl(at, length);
}
int HttpCommon::onHeaderField(http_parser* pParser, const char* at, size_t length){
	return ((HttpCommon*)pParser->data)->recordHeaderField(at, length);
}
int HttpCommon::onHeaderValue(http_parser* pParser, const char* at, size_t length){
	return ((HttpCommon*)pParser->data)->recordHeaderValue(at, length);
}
int HttpCommon::onBody(http_parser* pParser, const char* at, size_t length){
	return ((HttpCommon*)pParser->data)->recordBody(at, length);
}
void HttpCommon::recordHeadersComplete(void){
    // 记录最后一个key - value
    if(!m_parseHeader.isEmpty()){
        HString key(getBufferOffsetPtr(m_parseHeader.field.offset), m_parseHeader.field.length);
        HString value(getBufferOffsetPtr(m_parseHeader.value.offset), m_parseHeader.value.length);
        m_headersMap.insert(std::make_pair(key, value));
//        LOG_DEBUG("HttpCommon::recordHeadersComplete key=%s value=%s", key.toString().c_str(), value.toString().c_str());
    }
    setParseState(HTTP_PARSE_HEADER_COMPLETE);
}
int HttpCommon::recordUrl(const char* at, size_t length){
	if(HTTP_PARSE_URL != getParseState()){
		m_url.offset = this->getOffsetAt(at);
		m_url.length = (int)length;
		this->setParseState(HTTP_PARSE_URL);
	}else{
		m_url.length += (int)length;
	}
	return 0;
}
int HttpCommon::recordHeaderField(const char* at, size_t length){
//	LOG_DEBUG("HttpCommon::recordHeaderField offset=%d length=%d", (int)this->getOffsetAt(at), (int)length);
	if(HTTP_PARSE_HEADER_FIELD != getParseState()){
	    // 记录上一个key - value
        if(!m_parseHeader.isEmpty()){
            HString key(getBufferOffsetPtr(m_parseHeader.field.offset), m_parseHeader.field.length);
            HString value(getBufferOffsetPtr(m_parseHeader.value.offset), m_parseHeader.value.length);
            m_headersMap.insert(std::make_pair(key, value));
//            LOG_DEBUG("HttpCommon::recordHeaderValue key=%s value=%s", key.toString().c_str(), value.toString().c_str());
        }
        // 开启下一个key
		m_parseHeader.field.offset = this->getOffsetAt(at);
		m_parseHeader.field.length = (int)length;
		this->setParseState(HTTP_PARSE_HEADER_FIELD);
	}else{
		m_parseHeader.field.length += (int)length;
	}
	return 0;
}
int HttpCommon::recordHeaderValue(const char* at, size_t length){
//	LOG_DEBUG("HttpCommon::recordHeaderValue offset=%d length=%d", (int)this->getOffsetAt(at), (int)length);
	if(HTTP_PARSE_HEADER_VALUE != getParseState()){
		m_parseHeader.value.offset = this->getOffsetAt(at);
		m_parseHeader.value.length = (int)length;

		this->setParseState(HTTP_PARSE_HEADER_VALUE);
	}else{
		m_parseHeader.value.length += (int)length;
	}
	return 0;
}
int HttpCommon::recordBody(const char* at, size_t length){
//	fprintf(stderr, "HttpCommon::recordBody offset=%d length=%d\n", this->getOffsetAt(at), length);
	if(HTTP_PARSE_BODY != getParseState()){
		m_body.offset = this->getOffsetAt(at);
		m_body.length = (int)length;
		this->setParseState(HTTP_PARSE_BODY);
	}else{
		m_body.length += (int)length;
	}
	return 0;
}
bool HttpCommon::epollActive(uint32 events){
	// 检查是读事件还是写事件；根据当前状态执行下一个处理动作
	if( this->getState() >= HTTP_STATE_DESTROY ){
//		LOG_DEBUG("state=%d >= HTTP_STATE_DESTROY=%d", getState(), HTTP_STATE_DESTROY);
		this->epollRemove();
		return true;
	}
	if(events & EPOLLIN){
		this->setState(HTTP_STATE_READ);
		this->epollIn();
	}
	if(events & EPOLLOUT){
		this->setState(HTTP_STATE_WRITE);
		this->epollOut();
	}else if(events & EPOLLERR){
//		LOG_DEBUG("EPOLLERR");
		this->epollRemove();
		return true;
	}
	return true;
}
bool HttpCommon::epollIn(void){
    size_t nparsed;
    int parse_size;
	int result;
    bool bReceiveMessage = false;
	do{
		result = readSocket();
		if(result == 0){
			if(this->isUpgrade()){
	            // 已经升级，开始解析websocket数据
	            HttpCommon* pHttp = this;
	            CharVector* pBuffer = getReadBuffer();
	            bool client_frame = (this->getType() == CONN_TYPE_HTTP || this->getType() == CONN_TYPE_HTTPS);
	            result = loop_parse_ws_frames(
	                [&pHttp](std::vector<char>& output){
	                    char* ptr = output.data();
	                    int length = (int)output.size();
                        ((Manager*)pHttp->getEpoll())->onReceiveWebSocket(pHttp, ptr, length);
	                },
	                [&pHttp](size_t offset){
	                    pHttp->setReadOffset((int)offset);
	                },
	                client_frame,
	                *pBuffer
	            );
	            if(result == 0){
	                continue;
	            }else{
	                break;
	            }
			}
			else{
	            parse_size = this->getParseSize();
	            nparsed = http_parser_execute(this->getParser(), ((Manager*)getEpoll())->getSettings(), this->getParseOffsetPtr(), parse_size);
	            if (this->getParser()->upgrade) {
//	                LOG_DEBUG("current accept websocket handle=%u", getHandle());
	                // 回复对方，当前支持升级
	                result = onHttpUpgrade();
	                if(result != 0){
	                    break;
	                }
	                // 清理头部数据
	                this->clearReadBuffer();
	                this->setReadOffset(0);
//	                this->clearHeaders();
	//                result = 0;
	                break;
	            }else if(nparsed != (size_t)parse_size){
	                result = -1;
	                LOG_ERROR("nparsed(%d) != (size_t)parse_size(%d) (int)m_readBuffer.size()=%d getReadOffset()=%d",
	                    (int)nparsed, (int)parse_size, (int)m_readBuffer.size(), getReadOffset());
	                break;
	            }
//	            LOG_DEBUG("nparsed(%d) != (size_t)parse_size(%d) (int)m_readBuffer.size()=%d getReadOffset()=%d",
//	                (int)nparsed, (int)parse_size, (int)m_readBuffer.size(), getReadOffset());
	            // 一旦解析结束，调用处理请求的callback
	            if(HTTP_PARSE_MESSAGE_END == getParseState()){
//	                std::string ConnectionFlag = getHeader("Connection");
//	                int ret = http_should_keep_alive(this->getParser());
	                if( !isUpgrade() ){
	                    std::string Connection = getHeader("Connection");
//	                    LOG_DEBUG("Connection=%s handle=%u", Connection.c_str(), getHandle());
	                    if(Connection == "keep-alive" || Connection == "Keep-Alive"){
                            LOG_DEBUG("setUpgradeState HTTP_UPGRADE_KEEP_ALIVE handle=%u", getHandle());
                            this->setUpgradeState(HTTP_UPGRADE_KEEP_ALIVE);
	                    }else{
	                        this->setUpgradeState(HTTP_UPGRADE_NONE);
	                    }
	                }
	                this->setState(HTTP_STATE_READ_DONE);
	                this->setReadOffset(0);
	                //回调处理收到请求
	                result = 0;
	                bReceiveMessage = true;
//                    this->onReadHttpEnd();
	                break;
	            }else{
	                this->setReadOffset((int)getReadBuffer()->size());
	            }
			}
		}
	}while(result == 0);
	if(result < 0){
		this->setState(HTTP_STATE_DESTROY);
	}else if(result > 0){
		this->setState(HTTP_STATE_READ_WAIT);
	}else{
		// do nothing
	}
    // 不管成功还是失败，都会检查删除
	if( this->getState() >= HTTP_STATE_DESTROY ){
//		LOG_DEBUG("state=%d >= HTTP_STATE_DESTROY=%d", getState(), HTTP_STATE_DESTROY);
		this->epollRemove();
		return false;
	}else{
        if(this->getLeftTime() < HTTP_CHECK_TIMEOUT){
//            LOG_DEBUG("setTimer handle=%u", getHandle());
            this->setTimeout(HTTP_HANDLE_TIMEOUT, HttpCommon::httpConnectTimeout);
        }
//        LOG_DEBUG("state=%d HTTP_STATE_DESTROY=%d", getState(), HTTP_STATE_DESTROY);
        checkEpollState();
	}
	if(bReceiveMessage){
	    this->onReadHttpEnd();
	}
	return true;
}
bool HttpCommon::epollOut(void){
	int result;
	do{
		result = writeSocket();
		if( result < 0 ){
			this->setState(HTTP_STATE_DESTROY);
			break;
		}
		if( result == 0 ){
            onWriteHttpEnd();
			break;
		}
		// result > 0
		this->setState(HTTP_STATE_WRITE_WAIT);
		break;
	}while(0);
    // 不管成功还是失败，都会检查删除
	if( this->getState() >= HTTP_STATE_DESTROY ){
//		LOG_DEBUG("state=%d >= HTTP_STATE_DESTROY=%d", getState(), HTTP_STATE_DESTROY);
		this->epollRemove();
		return false;
	}else{
//	    LOG_DEBUG("state=%d < HTTP_STATE_DESTROY=%d", getState(), HTTP_STATE_DESTROY);
        if(this->getLeftTime() < HTTP_CHECK_TIMEOUT){
//            LOG_DEBUG("setTimer handle=%u", getHandle());
            this->setTimeout(HTTP_HANDLE_TIMEOUT, HttpCommon::httpConnectTimeout);
        }
//        LOG_DEBUG("state=%d HTTP_STATE_DESTROY=%d", getState(), HTTP_STATE_DESTROY);
        checkEpollState();
	}
	return true;
}

bool HttpCommon::checkEpollState(void){
    if(this->isUpgrade()){
        if(getWriteBuffer()->size() > 0){
            return setEpollState(EPOLLIN | EPOLLOUT);
        }else{
            return setEpollState(EPOLLIN);
        }
    }else{
        if(this->getState() < HTTP_STATE_READ_DONE){
//            LOG_DEBUG("EPOLLIN handle=%u", getHandle());
            return setEpollState(EPOLLIN);
        }else if(this->getState() == HTTP_STATE_WRITE_WAIT){
//            LOG_DEBUG("EPOLLOUT handle=%u", getHandle());
            return setEpollState(EPOLLOUT);
        }else{
//            LOG_DEBUG("setEpollState nothing");
            return true;
        }
    }
}
bool HttpCommon::setEpollState(uint32 events){
//    LOG_DEBUG("state=%d HTTP_STATE_DESTROY=%d", getState(), HTTP_STATE_DESTROY);
	if( this->isInEpoll() ){
		getEpoll()->objectChange(this, events);
	}else{
		if( !getEpoll()->objectAdd(this, events) ){
		    LOG_ERROR("epoll objectAdd failed handle=%u", getHandle());
			this->setState(HTTP_STATE_DESTROY);
		}else{
			setIsInEpoll(true);
		}
	}
	return true;
}
void HttpCommon::checkEpollRemove(void){
	if( this->isInEpoll() ){
		getEpoll()->objectRemove(this);
		this->setIsInEpoll(false);
	}
}
void HttpCommon::responseRequestUpgrade(const char* ptr, int length){
//	LOG_DEBUG("responseRequest length=%d", length);
	CharVector* pBuffer = this->getWriteBuffer();
	bool client_frame = (this->getType() == CONN_TYPE_HTTP_CLIENT || this->getType() == CONN_TYPE_HTTPS_CLIENT);

    encode_ws_frame_body(*pBuffer, ptr, length, client_frame);

	this->epollOut();
}
void HttpCommon::responseRequestNormal(const char* ptr, int length){
    CharVector* pBuffer = this->getWriteBuffer();
    pBuffer->insert(pBuffer->end(), ptr, ptr+length);
    this->epollOut();
}
// 追加一个请求字符串
void HttpCommon::responseAppend(const char* ptr, int length){
	CharVector* pBuffer = this->getWriteBuffer();
	pBuffer->insert(pBuffer->end(), ptr, ptr+length);
}
void HttpCommon::findTransferIP(void){
	std::string realIP = getHeader("X-Real-IP");
	if( !realIP.empty() ){
		this->setIP(convert_str2ip(realIP.c_str()));
	}
}
std::string HttpCommon::getHeader(const std::string& key){
    HStringMap::iterator itCur = m_headersMap.find(HString(key));
    if(itCur != m_headersMap.end()){
        return itCur->second.toString();
    }
    return std::string("");
}
char* HttpCommon::getHeader(const std::string& key, int& length){
    HStringMap::iterator itCur = m_headersMap.find(HString(key));
    if(itCur != m_headersMap.end()){
        length = itCur->second.length();
        return itCur->second.c_str();
    }
    return NULL;
}
int HttpCommon::readSocket(void){
	char recvBuffer[READ_BUFFER_SIZE];
//	char* recvBuffer = MainHandler::getInstance()->getLineQueue()->getCurrentWorker()->getReadBuffer();
	int nread;
	nread = read(this->getSocketFD(), recvBuffer, READ_BUFFER_SIZE);
    if(nread < 0){
        switch(errno){
        case EINTR: return 1; 	// 读数据失败，处理信号中断
        case EAGAIN: return 2;	// 可以下次重新调用
        default: return -1;
        }
        return -1;
    }else if(nread == 0){
        return -1;
    }
	CharVector* pBuffer = getReadBuffer();
	this->setReadOffset((int)pBuffer->size());
	pBuffer->insert(pBuffer->end(), recvBuffer, recvBuffer + nread);
//	LOG_DEBUG("getReadBuffer=%s", pBuffer->data());
	return 0;
}
int HttpCommon::writeSocket(void){
    int nwrite;
    nwrite = write(this->getSocketFD(), this->getWriteOffsetPtr(), this->getWriteSize());
    if(nwrite < 0){
        switch(errno){
        case EINTR: return 1; // 写数据失败，处理信号中断
        case EAGAIN:    // 可以下次重新调用
//            fprintf(stderr, "write EAGAIN capture\n");
            return 2;
        default:
//            LOG_DEBUG("write error and return -1 nwrite=%d errno=%d write size=%d", nwrite, errno, this->getWriteSize());
            return -1;
        }
        return -1;
    }
    moveWriteOffset(nwrite);
    if( this->isWriteEnd() ){
        clearWriteBuffer();
    	return 0;
    }
    return 3;	// 一次没能写完，等待写
}
void HttpCommon::clearData(void){
    EpollObject::clearData();
	this->checkEpollRemove();
	this->clearState();
	this->setParseState(HTTP_PARSE_INIT);
	this->setSSLState(HTTPS_SSL_INIT);
	this->setUpgradeState(HTTP_UPGRADE_NONE);
	this->setReadOffset(0);
//	LOG_DEBUG("clearData handle=%u", getHandle());
	this->clearReadBuffer();
	this->clearWriteBuffer();
	this->m_url.reset();
	this->m_body.reset();
	this->m_parseHeader.reset();
	this->m_headersMap.clear();
}
void HttpCommon::keepAliveReset(char state){
    this->setState(state);
    this->setParseState(HTTP_PARSE_INIT);
    this->setReadOffset(0);
	this->clearReadBuffer();
	this->clearWriteBuffer();
	this->m_url.reset();
	this->m_body.reset();
	this->m_parseHeader.reset();
	this->m_headersMap.clear();
}
void HttpCommon::initialize(void){
	http_parser* pParser = getParser();
	switch(getType()){
	    case CONN_TYPE_HTTP:
	    case CONN_TYPE_HTTPS:{
            http_parser_init(pParser, HTTP_REQUEST);
	        break;
	    }
	    case CONN_TYPE_HTTP_CLIENT:
	    case CONN_TYPE_HTTPS_CLIENT:{
            http_parser_init(pParser, HTTP_RESPONSE);
            break;
	    }
	    default:{
            http_parser_init(pParser, HTTP_BOTH);
	        break;
	    }
	}
	pParser->data = this;
	this->setParseState(HTTP_PARSE_INIT);
}



NS_HIVE_END

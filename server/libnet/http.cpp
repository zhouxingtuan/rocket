
#include "http.h"
#include "manager.h"

NS_HIVE_BEGIN

Http::Http(void) : HttpCommon() {
//    setType(CONN_TYPE_HTTP);
}
Http::~Http(void){

}
void Http::onReadHttpEnd(void){
	// if this is a preflight request
	if(HTTP_OPTIONS == this->getParser()->method){
		static char options_response[] = {"HTTP/1.1 200 OK\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Access-Control-Allow-Methods: POST,GET,OPTIONS\r\n"
                    "Access-Control-Allow-Headers: *\r\n"
                    "Content-Type: text/html; charset=utf-8\r\n"
                    "Content-Length: 0\r\n"
                    "Accept-Ranges: bytes\r\n"
                    "\r\n"
                    ""};
//		LOG_DEBUG("options_response length=%d data=\n%s", (int)sizeof(options_response), options_response);
		this->responseRequestNormal(options_response, sizeof(options_response));
		return;
	}
	// check if this connect is transfer from nginx
	this->findTransferIP();
	// dispatch the packet
	((Manager*)this->getEpoll())->onReceiveHttp(this);
}
void Http::onWriteHttpEnd(void){
    if(this->isKeepAlive()){
        // 需要重置连接状态
        this->keepAliveReset(HTTP_STATE_READ);
        // 重新计时，超过这个时间则关闭连接
        this->setTimeout(HTTP_HANDLE_TIMEOUT, HttpCommon::httpConnectTimeout);
//        LOG_DEBUG("onWriteHttpEnd isKeepAlive");
    }else if(this->isUpgrade()){
//        LOG_DEBUG("onWriteHttpEnd isUpgrade");
    }else{
        this->setState(HTTP_STATE_DESTROY);
//        LOG_DEBUG("onWriteHttpEnd HTTP_STATE_DESTROY");
    }
//    if( !this->isUpgrade() ){
//        this->setState(HTTP_STATE_DESTROY);
//    }
}
int Http::onHttpUpgrade(void){
    std::string accept_key = getHeader(SEC_WEBSOCKET_KEY_HEADER);
    if(accept_key.empty()){
        accept_key = getHeader(SEC_WEBSOCKET_KEY_HEADER_LOWER_CASE);
    }
    if(accept_key.empty()){
        LOG_ERROR("upgrade but can not find Sec-WebSocket-Key handle=%u", getHandle());
        return -1;
    }
    std::string accept_protocol = getHeader(SEC_WEBSOCKET_PROTOCOL_HEADER);
    if(accept_protocol.empty()){
        accept_protocol = getHeader(SEC_WEBSOCKET_PROTOCOL_HEADER_LOWER_CASE);
    }
    findTransferIP();
    this->setUpgradeState(HTTP_UPGRADE_WEB_SOCKET);
    std::vector<char> response = accept_upgrade_response(accept_key, accept_protocol);
//    LOG_DEBUG("onHttpUpgrade length=%d response=\n%s", (int)response.size(), std::string(response.data(), response.size()).c_str());
    this->responseRequestNormal(response.data(), response.size());
    return 0;
}

NS_HIVE_END

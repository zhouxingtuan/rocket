
#include "http_client.h"
#include "manager.h"

NS_HIVE_BEGIN

RequestData::RequestData(void) : RefObject(),
    m_body(HTTP_DEFAULT_WRITE_BUFFER_SIZE),
    m_callbackID(0), m_phonyIP(0), m_serverHandle(0), m_isKeepAlive(false)
    {
    m_body.clear();
}

RequestData::~RequestData(void){

}

void RequestData::clearData(void){
    m_requestType.clear();
    m_requestHost.clear();
    m_requestPort.clear();
    m_requestRoute.clear();
    m_method.clear();
    m_url.clear();
    m_headers.clear();
    m_body.clear();
    m_callbackID = 0;
    m_phonyIP = 0;
    m_serverHandle = 0;
    m_isKeepAlive = false;
}
void RequestData::initialize(const char* method, const char* url, const char* body, uint32 body_length, uint32 callbackID){
    m_method = method;
    m_url = url;
    m_callbackID = callbackID;
    m_body.insert(m_body.end(), body, body + body_length);
    splitURL();
}
void RequestData::initialize(const std::string& method, const std::string& url, const std::string& body, uint32 callbackID){
    m_method = method;
    m_url = url;
    m_callbackID = callbackID;
    m_body.insert(m_body.end(), body.c_str(), body.c_str() + body.length());
    splitURL();
}

void RequestData::splitURL(std::string& requestType, std::string& requestHost, std::string& requestPort, std::string& requestRoute){
    size_t type_pos = m_url.find_first_of("://");
    if( type_pos == std::string::npos ){
        requestType = "http";
        type_pos = 0;
    }else{
        requestType = m_url.substr(0, type_pos);	// pos, len
        type_pos += 3;
    }
    size_t addr_pos = m_url.find("/", type_pos);
    if(addr_pos == std::string::npos ){
        requestHost = m_url.substr(type_pos);
        requestRoute = "/";
    }else{
        requestHost = m_url.substr(type_pos, addr_pos - type_pos);
        requestRoute = m_url.substr(addr_pos);
    }
    size_t port_pos = requestHost.find(":", 0);
    if(port_pos == std::string::npos ){
        // do nothing
    }else{
        requestPort = requestHost.substr(port_pos + 1);
        requestHost = requestHost.substr(0, port_pos);
    }
//    LOG_DEBUG("requestType=%s requestHost=%s requestPort=%s requestRoute=%s", requestType.c_str(), requestHost.c_str(), requestPort.c_str(), requestRoute.c_str());
}
void RequestData::joinUpgradeData(std::string& requestData){
    requestData.clear();
    requestData.append(m_method + " " + m_requestRoute + " " + "HTTP/1.1\r\n");
    requestData.append("Host: " + m_requestHost + "\r\n");
    requestData.append("Upgrade: websocket\r\n");
    requestData.append("Connection: Upgrade\r\n");
    requestData.append("Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n");
    for(auto &str : m_headers){
        requestData.append(str + "\r\n");
    }
    requestData.append("Content-Length: " + std::to_string((int)m_body.size()) + "\r\n");
    requestData.append("\r\n");
    requestData.append(std::string(m_body.data(), m_body.size()));
}
void RequestData::joinData(const std::string& requestRoute, std::string& requestData){
    requestData.clear();
    requestData.append(m_method + " " + requestRoute + " " + "HTTP/1.1\r\n");
    if(m_isKeepAlive){
        requestData.append("Connection: keep-alive\r\n");
    }
//    else{
//        requestData.append("Connection: close\r\n");
//    }
    requestData.append("Host: " + m_requestHost + "\r\n");
    requestData.append("Accept: */*\r\n");
    for(auto &str : m_headers){
        requestData.append(str + "\r\n");
    }
    if(m_method == "GET"){
        requestData.append("\r\n");
        return;
    }
    requestData.append("Content-Length: " + std::to_string((int)m_body.size()) + "\r\n");
    requestData.append("\r\n");
    requestData.append(std::string(m_body.data(), m_body.size()));
}
void RequestData::appendHeader(const std::string& field, const std::string& value){
    std::string header;
    header.append(field);
    header.append(": ");
    header.append(value);
    m_headers.push_back(header);
}
void RequestData::appendHeader(const char* field, const char* value){
    std::string header;
    header.append(field);
    header.append(": ");
    header.append(value);
    m_headers.push_back(header);
}


HttpClient::HttpClient(void) : HttpCommon()
//    , m_callbackID(0)
    {
//    setType(CONN_TYPE_HTTP_CLIENT);
}
HttpClient::~HttpClient(void){

}
void HttpClient::onReadHttpEnd(void){
    ((Manager*)getEpoll())->onHttpResponse(this, this->getBody(), this->getBodyLength(), true);
    if(this->isKeepAlive()){
        // 需要重置连接状态
        this->keepAliveReset(HTTP_STATE_WRITE);
        // 重新计时，超过这个时间则关闭连接
        this->setTimeout(HTTP_HANDLE_TIMEOUT, HttpCommon::httpConnectTimeout);
    }else if(this->isUpgrade()){

    }else{
        this->setState(HTTP_STATE_DESTROY);
    }
//    if( !this->isUpgrade() ){
//        this->setState(HTTP_STATE_DESTROY);
//    }
}
void HttpClient::onWriteHttpEnd(void){
    this->setState(HTTP_STATE_READ_WAIT);
}
int HttpClient::onHttpUpgrade(void){
    std::string accept_key = getHeader(SEC_WEBSOCKET_ACCEPT_HEADER);
    if(accept_key != "s3pPLMBiTxaQ9kYGzzhZRbK+xOo="){
        LOG_ERROR("Sec-WebSocket-Accept is not valid handle=%u", getHandle());
        return -1;
    }
    this->setUpgradeState(HTTP_UPGRADE_WEB_SOCKET);
    ((Manager*)getEpoll())->onOpenConnect(this);
    return 0;
}
bool HttpClient::connectServer(RequestData* pRequest, int &fd){
    static char port_str[8] = "80";
    static char ssl_port_str[8] = "443";
	struct addrinfo hints;
	struct addrinfo *res = NULL;
	struct addrinfo *ptr = NULL;
	const char* port_ptr = NULL;
	int result;
    fd = -1;
	if( !pRequest->m_requestPort.empty() ){
	    port_ptr = pRequest->m_requestPort.c_str();
	}else{
	    if(pRequest->m_requestType == "https" || pRequest->m_requestType == "wss"){
            port_ptr = ssl_port_str;
	    }else{
	        port_ptr = port_str;
	    }
	}
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;
	result = getaddrinfo(pRequest->m_requestHost.c_str(), port_ptr, &hints, &res);
	if (result) {
		if(res){
			freeaddrinfo(res);	// 记得释放
		}
		return false;
	}
	for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
		fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (fd < 0) {
			continue;
		}
		if (connect(fd, res->ai_addr, res->ai_addrlen) < 0) {
			close(fd);
			fd = -1;
			continue;
		}
//		m_ip = ((sockaddr_in*)(res->ai_addr))->sin_addr.s_addr;
		break;	// we got one conn
	}
	freeaddrinfo(res);	// 记得释放
	if (fd < 0) {
		return false;
	}
	return true;
}

NS_HIVE_END

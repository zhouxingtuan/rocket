
#ifndef __hive__http_client__
#define __hive__http_client__

#include "http_common.h"

NS_HIVE_BEGIN

typedef std::vector<std::string> StringVector;
class RequestData : public RefObject
{
public:
	RequestData(void);
	virtual ~RequestData(void);

    void initialize(const char* method, const char* url, const char* body, uint32 body_length, uint32 callbackID);
    void initialize(const std::string& method, const std::string& url, const std::string& body, uint32 callbackID);

    void setPhonyIP(uint32 phonyIP){ m_phonyIP = phonyIP; }
    void setServerHandle(uint32 serverHandle){ m_serverHandle = serverHandle; }
    void setKeepAlive(bool isKeepAlive){ m_isKeepAlive = isKeepAlive; }
    void setLocalHost(const char* localhost){ m_localHost = localhost; }

    void splitURL(void){
        splitURL(m_requestType, m_requestHost, m_requestPort, m_requestRoute);
    }
    void splitURL(std::string& requestType, std::string& requestHost, std::string& requestPort, std::string& requestRoute);
    void joinUpgradeData(std::string& requestData);
    void joinData(std::string& requestData){
        joinData(m_requestRoute, requestData);
    }
    void joinData(const std::string& requestRoute, std::string& requestData);

	void appendHeader(const std::string& field, const std::string& value);
	void appendHeader(const char* field, const char* value);

	void clearData(void);
public:
	std::string m_localHost;
	std::string m_requestType;
	std::string m_requestHost;
	std::string m_requestPort;
	std::string m_requestRoute;
	std::string m_method;
	std::string m_url;
	StringVector m_headers;
	CharVector m_body;
	uint32 m_callbackID;
	uint32 m_phonyIP;
	uint32 m_serverHandle;
	bool m_isKeepAlive;
};

class HttpClient : public HttpCommon
{
public:

public:
	HttpClient(void);
	virtual ~HttpClient(void);

    // from HttpCommon
	virtual void onReadHttpEnd(void);
	virtual void onWriteHttpEnd(void);
	virtual int onHttpUpgrade(void);

    static uint8 objectType(void){
        return CONN_TYPE_HTTP_CLIENT;
    }
    virtual uint8 getType(void){ return this->objectType(); }

    bool connectServer(RequestData* pRequest, int &fd);
};

NS_HIVE_END

#endif

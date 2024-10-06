
#ifndef __hive__https_client__
#define __hive__https_client__

#include "http_client.h"

NS_HIVE_BEGIN

class HttpsClient : public HttpClient
{
public:
    HttpsClient(void);
    virtual ~HttpsClient(void);

	// from EpollObject
	virtual bool epollActive(uint32 events);
	// from Http
	virtual int readSocket(void);
	virtual int writeSocket(void);
	// from PoolObject
	virtual void clearData(void);

    static uint8 objectType(void){
        return CONN_TYPE_HTTPS_CLIENT;
    }
    virtual uint8 getType(void){ return this->objectType(); }

	bool bindSSL(SSL_CTX* ctx);
	void startConnecting(bool changeEpoll);
protected:
	void destroySSL(void);
protected:
	SSL *m_pSSL;
};

NS_HIVE_END

#endif

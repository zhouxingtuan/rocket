//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 17/1/23
// Time: 下午6:27
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__https__
#define __hive__https__

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "http.h"


NS_HIVE_BEGIN

class Https : public Http
{
public:
	Https(void);
	virtual ~Https(void);

	// from EpollObject
	virtual bool epollActive(uint32 events);
	// from Http
	virtual int readSocket(void);
	virtual int writeSocket(void);
	// from PoolObject
	virtual void clearData(void);

    static uint8 objectType(void){
        return CONN_TYPE_HTTPS;
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

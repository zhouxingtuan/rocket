
#ifndef __hive__http__
#define __hive__http__

#include "http_common.h"

NS_HIVE_BEGIN

class Http : public HttpCommon
{
public:
	Http(void);
	virtual ~Http(void);

    // from HttpCommon
	virtual void onReadHttpEnd(void);
	virtual void onWriteHttpEnd(void);
    virtual int onHttpUpgrade(void);

    static uint8 objectType(void){
        return CONN_TYPE_HTTP;
    }
    virtual uint8 getType(void){ return this->objectType(); }
protected:

};

NS_HIVE_END

#endif

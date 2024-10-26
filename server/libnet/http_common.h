//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/11/2
// Time: 下午11:26
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__http_common__
#define __hive__http_common__

#include "http_parser.h"
#include "websocket.h"
#include "epoll.h"
#include "util.h"
#include "timer.h"
#include "hstring.h"


NS_HIVE_BEGIN

//#define HTTP_DEFAULT_READ_BUFFER_SIZE 256
//#define HTTP_DEFAULT_WRITE_BUFFER_SIZE 256
#define HTTP_DEFAULT_READ_BUFFER_SIZE 240
#define HTTP_DEFAULT_WRITE_BUFFER_SIZE 240

#define HTTP_STATE_IDLE 		0		// 空闲状态
#define HTTP_STATE_READ 		1		// 正在处理读状态
#define HTTP_STATE_READ_WAIT 	2		// 正在epoll中等待读
#define HTTP_STATE_READ_DONE 	3		// 已经读取完毕
#define HTTP_STATE_WAIT		    4		// 正在挂起等待发送
#define HTTP_STATE_WRITE	 	5		// 正在处理写状态
#define HTTP_STATE_WRITE_WAIT 	6		// 正在epoll中等待写
#define HTTP_STATE_WRITE_DONE 	7		// 已经写入完毕
#define HTTP_STATE_DESTROY	 	8		// 已经被标记为删除，Handler执行时会返回

#define HTTP_PARSE_INIT 			0
#define HTTP_PARSE_MESSAGE_BEGIN 	1
#define HTTP_PARSE_URL 			    2
#define HTTP_PARSE_HEADER_FIELD 	3
#define HTTP_PARSE_HEADER_VALUE 	4
#define HTTP_PARSE_HEADER_COMPLETE  5
#define HTTP_PARSE_BODY 			6
#define HTTP_PARSE_MESSAGE_END	 	7

#define HTTPS_SSL_INIT          0
#define HTTPS_SSL_CONNECTING    1
#define HTTPS_SSL_IDENTIFIED    2

#define HTTP_UPGRADE_NONE 0
#define HTTP_UPGRADE_KEEP_ALIVE 1
#define HTTP_UPGRADE_WEB_SOCKET 2

typedef struct HttpValueRecorder{
	int offset;
	int length;
	HttpValueRecorder(void) : offset(0), length(0){}
	~HttpValueRecorder(void){}
	inline void reset(void){
		offset = 0;
		length = 0;
	}
}HttpValueRecorder;

typedef struct HttpHeaderRecorder{
	HttpValueRecorder field;
	HttpValueRecorder value;
	HttpHeaderRecorder(void) : field(), value(){}
	~HttpHeaderRecorder(void){}
	inline void reset(void){
		field.reset();
		value.reset();
	}
	inline bool isEmpty(void){
	    if(0 == field.length){
	        return true;
	    }
	    return false;
	}
}HttpHeaderRecorder;

#define CONN_TYPE_HTTP 4
#define CONN_TYPE_HTTPS 5
#define CONN_TYPE_HTTP_CLIENT 6
#define CONN_TYPE_HTTPS_CLIENT 7

//typedef std::vector<HttpHeaderRecorder> HttpHeaderRecorderVector;
typedef std::unordered_map<HString, HString> HStringMap;
typedef std::vector<char> CharVector;
class HttpCommon : public EpollObject
{
protected:
	http_parser m_parser;
	CharVector m_readBuffer;
	CharVector m_writeBuffer;
	char m_httpState;
	char m_parseState;
	char m_sslState;
	char m_upgradeState;

	int m_readOffset;
	HttpValueRecorder m_url;
	HttpValueRecorder m_body;
	HttpHeaderRecorder m_parseHeader;
    HStringMap m_headersMap;
public:
	HttpCommon(void);
	virtual ~HttpCommon(void);

    static int64 httpConnectTimeout(EpollObject* pObject);

	static int onMessageBegin(http_parser* pParser);
	static int onHeadersComplete(http_parser* pParser);
	static int onMessageComplete(http_parser* pParser);
	static int onUrl(http_parser* pParser, const char* at, size_t length);
	static int onHeaderField(http_parser* pParser, const char* at, size_t length);
	static int onHeaderValue(http_parser* pParser, const char* at, size_t length);
	static int onBody(http_parser* pParser, const char* at, size_t length);

	int recordUrl(const char* at, size_t length);
	int recordHeaderField(const char* at, size_t length);
	int recordHeaderValue(const char* at, size_t length);
	int recordBody(const char* at, size_t length);
	void recordHeadersComplete(void);

	// from EpollObject
	virtual bool epollActive(uint32 events);
	virtual bool epollIn(void);
	virtual bool epollOut(void);
	virtual void epollCheck(void){}

    // from PoolObject
	virtual void clearData(void);

    virtual void keepAliveReset(char state);

	virtual void onReadHttpEnd(void) = 0;
	virtual void onWriteHttpEnd(void) = 0;
	virtual int onHttpUpgrade(void) = 0;



	void responseRequestUpgrade(const char* ptr, int length);	// 返回请求字符串
	void responseAppend(const char* ptr, int length);    // 追加一个请求字符串
	void responseRequestNormal(const char* ptr, int length);

	virtual int readSocket(void);
	virtual int writeSocket(void);

	inline char getState(void) {
		return m_httpState;
	}
	inline void setState(char state) {
		if( HTTP_STATE_DESTROY != m_httpState ){
			m_httpState = state;
		}
	}
	inline void clearState(void) { m_httpState = HTTP_STATE_IDLE; }
    inline void clearReadBuffer(void){
        CharVector temp(HTTP_DEFAULT_READ_BUFFER_SIZE, 0);
        m_readBuffer.swap(temp);
        m_readBuffer.clear();
    }
    inline void clearWriteBuffer(void){
        CharVector temp(HTTP_DEFAULT_WRITE_BUFFER_SIZE, 0);
        m_writeBuffer.swap(temp);
        m_writeBuffer.clear();
    }
	inline CharVector* getReadBuffer(void){ return &m_readBuffer; }
	inline CharVector* getWriteBuffer(void){ return &m_writeBuffer; }
	inline void setParseState(char state){ m_parseState = state; }
	inline char getParseState(void) const { return m_parseState; }
	inline void setSSLState(char state){ m_sslState = state; }
	inline char getSSLState(void) const { return m_sslState; }
	inline char* getUrl(void) { return (getReadBuffer()->data() + m_url.offset); }
	inline int getUrlLength(void) const { return m_url.length; }
	inline char* getBody(void) { return (getReadBuffer()->data() + m_body.offset); }
	inline int getBodyLength(void) const { return m_body.length; }
	inline char* getBufferOffsetPtr(int offset) { return (getReadBuffer()->data() + offset); }
	HStringMap& getHeaders(void) { return m_headersMap; };
    void findTransferIP(void);
    std::string getHeader(const std::string& key);
    char* getHeader(const std::string& key, int& length);
	inline void setReadOffset(int offset) { m_readOffset = offset; }
	inline int getReadOffset(void) const { return m_readOffset; }
	inline void moveWriteOffset(int move) {
	    if(move == (int)getWriteBuffer()->size()){
	        getWriteBuffer()->clear();
	    }else{
	        getWriteBuffer()->erase(getWriteBuffer()->begin(), getWriteBuffer()->begin() + move);
	    }
	}
	inline char* getParseOffsetPtr(void) { return (getReadBuffer()->data() + m_readOffset); }
	inline int getParseSize(void) const { return ((int)m_readBuffer.size() - m_readOffset); }
	inline char* getWriteOffsetPtr(void) { return getWriteBuffer()->data(); }
	inline bool isWriteEnd(void) const { return (0 == (int)m_writeBuffer.size()); }
	inline int getWriteSize(void) const { return ((int)m_writeBuffer.size()); }
	inline void setUpgradeState(char upgradeState) { m_upgradeState = upgradeState; }
	inline char getUpgradeState(void) const { return m_upgradeState; }
	inline bool isUpgrade(void) const { return m_upgradeState == HTTP_UPGRADE_WEB_SOCKET; }
	inline bool isKeepAlive(void) const { return m_upgradeState == HTTP_UPGRADE_KEEP_ALIVE; }
	bool checkEpollState(void);
	bool setEpollState(uint32 events);
	void initialize(void);
	unsigned int getStatusCode(void){ return m_parser.status_code; }
	inline http_parser* getParser(void){ return &m_parser; }
	inline int getOffsetAt(const char* at){ return (int)(at - getReadBuffer()->data()); }
	void checkEpollRemove(void);
};

NS_HIVE_END

#endif

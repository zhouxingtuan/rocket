//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 17/1/23
// Time: 下午6:27
// To change this template use File | Settings | File Templates.
//

#include "https.h"
#include "manager.h"

NS_HIVE_BEGIN

Https::Https(void) : Http(), m_pSSL(NULL) {
//	setType(CONN_TYPE_HTTPS);
}
Https::~Https(void){
	destroySSL();
}
bool Https::epollActive(uint32 events){
    // 如果是SSL验证状态
    if(getSSLState() == HTTPS_SSL_CONNECTING){
        this->startConnecting(false);
        return true;
    }
    return HttpCommon::epollActive(events);
}
int Https::readSocket(void){
	char recvBuffer[READ_BUFFER_SIZE];
	int nread;
	nread = SSL_read(m_pSSL, recvBuffer, READ_BUFFER_SIZE);
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
	return 0;
}
int Https::writeSocket(void){
    int nwrite;
    nwrite = SSL_write(m_pSSL, this->getWriteOffsetPtr(), this->getWriteSize());
    if(nwrite < 0){
        switch(errno){
        case EINTR: return 1; // 写数据失败，处理信号中断
        case EAGAIN:    // 可以下次重新调用
//            fprintf(stderr, "write EAGAIN capture\n");
            return 2;
        default: return -1;
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
bool Https::bindSSL(SSL_CTX* ctx){
    destroySSL();
	/* 基于 ctx 产生一个新的 SSL*/
    m_pSSL = SSL_new(ctx);
    if(NULL == m_pSSL){
        LOG_ERROR("bindSSL NULL == m_pSSL handle=%u", getHandle());
        return false;
    }
    /* 将连接用户的 socket 加入到 SSL*/
    SSL_set_fd(m_pSSL, this->getSocketFD());

    SSL_set_accept_state(m_pSSL);

    if(!set_non_blocking(getSocketFD())){
        LOG_ERROR("set_non_blocking failed handle=%u", getHandle());
        return false;
    }

    return true;
}
void Https::startConnecting(bool changeEpoll){
    int acceptResult = SSL_do_handshake(m_pSSL);
    if(acceptResult == 1){
//        LOG_DEBUG("startConnecting SSL_do_handshake OK");
        this->setSSLState(HTTPS_SSL_IDENTIFIED);
        // 验证成功
        this->setTimeout(HTTP_HANDLE_TIMEOUT, HttpCommon::httpConnectTimeout);
        // 读取数据
        this->epollIn();
        return;
    }
    int connState = -1;
    int iErrCode = SSL_get_error(m_pSSL, acceptResult);
    switch (iErrCode) {
        case SSL_ERROR_NONE: {
//            LOG_DEBUG("startConnecting SSL_ERROR_NONE https OK");
            connState = 0;
            break;
        };
        case SSL_ERROR_WANT_READ: {
//            LOG_DEBUG("startConnecting SSL_ERROR_WANT_READ https read");
            connState = 0;
            break;
        };
        case SSL_ERROR_WANT_WRITE: {
//            LOG_DEBUG("startConnecting SSL_ERROR_WANT_READ https write");
            connState = 0;
            break;
        };
        case SSL_ERROR_SYSCALL: {
//            LOG_DEBUG("startConnecting SSL_ERROR_SYSCALL https");
            if (EINTR == errno){
                connState = 0;
            }else{
                connState = 1;
            }
            break;
        };
        case SSL_ERROR_ZERO_RETURN:{
//            LOG_DEBUG("startConnecting SSL_ERROR_ZERO_RETURN https close");
            connState = 2;
            break;
        };
        default: {
//            LOG_DEBUG("startConnecting errorCode=%d https error", iErrCode);
            connState = 3;
            break;
        };
    };
    if(connState == 0){
        if(changeEpoll){
            this->setEpollState(EPOLLIN | EPOLLOUT);
        }
    }else{
        this->epollRemove();
    }
}
void Https::clearData(void){
	destroySSL();
	HttpCommon::clearData();
}
void Https::destroySSL(void){
	if(NULL != m_pSSL){
		/* 关闭 SSL 连接 */
		SSL_shutdown(m_pSSL);
        /* 释放 SSL*/
        SSL_free(m_pSSL);
        m_pSSL = NULL;
	}
}

NS_HIVE_END

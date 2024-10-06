//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 16/7/9
// Time: 上午11:39
// To change this template use File | Settings | File Templates.
//

#include "listener.h"
#include "manager.h"

NS_HIVE_BEGIN

Listener::Listener(void) : EpollObject(),
	m_acceptSocketFunction(NULL)
	{

}
Listener::~Listener(void){

}
bool Listener::epollActive(uint32 events){
	struct sockaddr_in cliaddr;
	socklen_t socklen = sizeof(struct sockaddr_in);
	int fd;
	while( (fd = accept(getSocketFD(), (struct sockaddr*)&cliaddr, &socklen)) > 0 ){
		// 这里接收到新的连接，处理新的连接事件
//		uint32 ip_number = cliaddr.sin_addr.s_addr;
//		struct in_addr addr;
//		addr.s_addr = ip_number;
//		LOG_INFO("ip_number=%lld ipn=%s ip=%s", (long long int)ip_number, inet_ntoa(addr), inet_ntoa(cliaddr.sin_addr));
		m_acceptSocketFunction(fd, (uint32)cliaddr.sin_addr.s_addr, cliaddr.sin_port, this);
	};
	if( fd == -1 ){
		switch(errno){
			case EAGAIN:
			case EINTR:
			case ECONNABORTED:
			case EPROTO:
				epollCheck();
				break;
			default:
				// 这里说明socket出Bug了，需要进行处理；移除这个Listener
				LOG_ERROR("Listener accept failed errno=%d", errno);
				epollRemove();
				break;
		}
	}else{
	    LOG_ERROR("Listener epollActive not handle fd=%d", fd);
	    close(fd);
	    epollCheck();
	}
	return true;
}
bool Listener::epollIn(void){
	return true;
}
bool Listener::epollOut(void){
	return true;
}
void Listener::epollRemove(void){
	LOG_ERROR("Listener epollRemove ip=%s port=%d", convert_ip2str(getIP()), getPort());
	// 如果有接口监听失效，直接退出进程
//	sleep(1);
//    exit(1);
}
void Listener::epollCheck(void){
//	fprintf(stderr, "--Listener::epollCheck\n");
	getEpoll()->objectChange(this, EPOLLIN);
}
void Listener::resetData(void){
	closeSocket();
}
bool Listener::openSocket(void){
    int opt = 1;    // 是否复用标记 0不复用 1复用
    int fd;
    struct sockaddr_in servaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    bzero(&servaddr, socklen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = this->getIP(); //inet_addr( this->getIP() );//htonl(INADDR_ANY);
    servaddr.sin_port = htons( this->getPort() );
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
		LOG_ERROR("failed ip(%s) port(%d)", convert_ip2str(this->getIP()), this->getPort());
		return false;
    }
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//    if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1){
    if( !set_non_blocking(fd) ){
		LOG_ERROR("setNonBlocking to listen socket failed ip(%s) port(%d)", convert_ip2str(this->getIP()), this->getPort());
		goto LISTEN_FAILED;
    }
    if(bind(fd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) == -1){
		LOG_ERROR("bind listen socket error ip(%s) port(%d)", convert_ip2str(this->getIP()), this->getPort());
		goto LISTEN_FAILED;
    }
    if(listen(fd, MAX_LISTEN_SIZE) == -1){
  		LOG_ERROR("socket listen error ip(%s) port(%d)", convert_ip2str(this->getIP()), this->getPort());
		goto LISTEN_FAILED;
    }
//    LOG_INFO("OK ip=%s port=%d", convert_ip2str(this->getIP()), this->getPort());
    this->setSocketFD(fd);
    return true;
LISTEN_FAILED:
    close(fd);
    return false;
}

NS_HIVE_END

#include "Socket.h"

#include <netinet/tcp.h>
#include <string.h>

#include "InetAddress.h"
#include "utils.h"

void Socket::bindAddress(const InetAddress &localaddr) {
  int ret = ::bind(sockfd_, (sockaddr *)localaddr.getSockAddr(),
                  sizeof(sockaddr_in));
  perror_if(ret!=0,"bind");
}

void Socket::listen() {
  int ret = ::listen(sockfd_, 1024);
    
  perror_if(ret!=0,"listen");
}

int Socket::accept(InetAddress *peeraddr) {
  sockaddr_in addr;
  socklen_t len = sizeof(addr);
  ::memset(&addr, 0, sizeof(addr));
  // int connfd = ::accept(sockfd_,(sockaddr *)&addr,&len);
  int connfd =
      ::accept4(sockfd_, (sockaddr *)&addr, &len,
                SOCK_NONBLOCK | SOCK_CLOEXEC);  // 配合epoll边缘触发设置为非阻塞
  if (connfd >= 0) {
    peeraddr->setSockAddr(addr);
  }
  return connfd;
}

void Socket::shutdownWrite() {
  int ret = ::shutdown(sockfd_, SHUT_WR);
  perror_if(ret < 0,"shutdown write");
}

void Socket::setTcpNoDelay(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

void Socket::setReuseAddr(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

void Socket::setReusePort(bool on) {
  int optval = on ? 1:0;
  ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEPORT,&optval,sizeof(optval));
}

void Socket::setKeepAlive(bool on){
  int optval = on ? 1:0;
  ::setsockopt(sockfd_,SOL_SOCKET,SO_KEEPALIVE,&optval,sizeof(optval));
}

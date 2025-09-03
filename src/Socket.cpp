#include "Socket.h"

#include <netinet/tcp.h>
#include <string.h>

#include "InetAddress.h"
#include "temp_utils.h"

void Socket::bindAddress(const InetAddress &localaddr) {
  if (0 != ::bind(sockfd_, (sockaddr *)localaddr.getSockAddr(),
                  sizeof(sockaddr_in))) {
    char message[100];
    sprintf(message, "bind sockfd:%d fail\n", sockfd_);
    error_handling(message);
  }
}

void Socket::listen() {
  if (::listen(sockfd_, 1024) != 0) {
    char message[100];
    sprintf(message, "listen sockfd:%d fail\n", sockfd_);
    error_handling(message);
  }
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
  if (::shutdown(sockfd_, SHUT_WR) < 0) {
    error_handling("shutdownWrite error");
  }
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

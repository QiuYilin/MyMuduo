#pragma once
#include <netinet/tcp.h>
#include <unistd.h>
#include "utils.h"

class InetAddr;  // 前置声明

/// @brief 抽象封装socket文件描述符
class Socket {
 public:
  Socket() : sockfd_(::socket(AF_INET, SOCK_STREAM, 0)) {
    perror_if(sockfd_ == -1, "socket");
  }

  /// @brief
  /// @param sockfd
  explicit Socket(int sockfd) : sockfd_(sockfd) {
    perror_if(sockfd_ == -1, "socket(int fd)");
  }

  ~Socket() {
    if (sockfd_ != -1) {
      ::close(sockfd_);
    }
  }

  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  /// @brief 获取fd
  /// @return
  int fd() const { return sockfd_; }

  /// @brief 包装bind函数
  /// @param localaddr
  void bindAddress(const InetAddr &localaddr);

  /// @brief 包装listen()函数
  void listen();
  /// @brief 包装accept()函数
  /// @param peeraddr
  /// @return
  int accept(InetAddr *peeraddr);
  /// @brief 包装半关闭shutdown()函数
  void shutdownWrite();

  /// @brief 包装NODELAY选项
  /// @param on
  void setTcpNoDelay(bool on);

  /// @brief 包装ReuseAddr选项
  ///        允许一个套接字强制绑定到一个已被其他套接字使用的端口
  ///        这对于需要重启并绑定到相同端口的服务器应用程序非常有用
  /// @param on
  void setReuseAddr(bool on);

  /// @brief 包裝ReusePort选项
  ///        允许同一主机上的多个套接字绑定到相同的端口号
  ///        对于在多个线程或进程之间负载均衡传入连接非常有用
  /// @param on
  void setReusePort(bool on);

  /// @brief 包装SO_KEEPALIVE选项
  ///        如果另一端没有响应，则认为连接已经断开并关闭
  ///        这对于检测网络中失效的对等方非常有用
  /// @param on
  void setKeepAlive(bool on);

  void setNonblock();

 private:
  const int sockfd_;
};
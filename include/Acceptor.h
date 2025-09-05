#pragma once
#include <functional>

#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"

class EventLoop;

class Acceptor {
 public:
  using NewConnectionCallback = std::function<void(int sockfd)>;

  Acceptor(const InetAddress& listenAddr, EventLoop* eventLoop);
  ~Acceptor();

  void setNewconnectionCallback(const NewConnectionCallback& cb) {
    NewConnectionCallback_ = cb;
  }

  void listen();

 private:
  void handleRead();
  EventLoop* loop_;
  Socket acceptSocket_;
  Channel acceptChannel_;

  NewConnectionCallback NewConnectionCallback_;
  bool listen_;
};
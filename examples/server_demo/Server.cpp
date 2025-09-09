#include "Server.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

const int READ_BUFFER = 1024;

void setNonblock(int sockfd) {
  int flag = fcntl(sockfd, F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(sockfd, F_SETFL, flag);
}

Server::Server(const InetAddress& listenAddr, EventLoop* eventloop)
    : loop_(eventloop),
      acceptor_(std::make_unique<Acceptor>(listenAddr, loop_)) {
  auto cb = [this](int sockfd) { newConnection(sockfd); };
  acceptor_->setNewconnectionCallback(cb);
}

Server::~Server() {
  for (auto& item : connections_) {
    ConnectionPtr conn(item.second);
    item.second.reset();
    conn->connectDestroyed();
  }
}

void Server::newConnection(int sockfd) {
  setNonblock(sockfd);
  auto conn = std::make_shared<Connection>(loop_, sockfd);
  printf("newconnection make_shared user_count= %ld\n", conn.use_count());
  connections_[sockfd] = conn;
  printf("connections_[sockfd] = conn user_count = %ld\n", conn.use_count());

  conn->setMessageCallback(messageCallback_);
  conn->setCloseCallback([this](const ConnectionPtr& connection) {
    removeConnection(connection);
  });
}

void Server::removeConnection(const ConnectionPtr& conn) {
  auto n = connections_.erase(conn->fd());
  assert(n == 1);
  conn->connectDestroyed();
}
#pragma once

#include <Channel.h>
#include <Epoll.h>
#include <Socket.h>
#include <utils.h>
#include <EventLoop.h>
#include <InetAddress.h>
#include <Acceptor.h>
#include <memory>
#include <Buffer.h>

class Server{
public:
    Server(const InetAddress& serverAddr,EventLoop* eventloop);
    ~Server();
    void handleEvent(Channel*ch);
    void newConnection(int sockfd);
private:
    EventLoop* loop_;
    std::unique_ptr<Acceptor> acceptor_;
    Buffer inputBuffer_;
};

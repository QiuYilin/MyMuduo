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
#include "Connection.h"
#include <map>

class Server{
public:
    using connectionMap = std::map<int,ConnectionPtr>;
    Server(const InetAddress& serverAddr,EventLoop* eventloop);
    ~Server();
    void setMessageCallback(const MessageCallback& cb){
        messageCallback_ = cb;
    }
    
private:
    void newConnection(int sockfd);
    void removeConnection(const ConnectionPtr& conn);
    EventLoop* loop_;
    std::unique_ptr<Acceptor> acceptor_;
    connectionMap connections_;
    MessageCallback messageCallback_;
};

#pragma once

#include <Channel.h>
#include <Epoll.h>
#include <Socket.h>
#include <utils.h>
#include <EventLoop.h>
#include <InetAddress.h>

class Server{
public:
    Server(const InetAddress& serverAddr,EventLoop* eventloop);
    ~Server();
    void handleEvent(Channel*ch);
    void newConnection(Socket* serv_sock);
private:
    EventLoop* loop_;
    Socket *serv_socket_;
    Channel *serv_channel_;
};

#include "Server.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

const int READ_BUFFER = 1024;

void setNonblock(int sockfd){
    int flag = fcntl(sockfd,F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, flag);
}

Server::Server(const InetAddress& listenAddr,EventLoop *eventloop):loop_(eventloop),acceptor_(std::make_unique<Acceptor>(listenAddr,loop_)){
    auto cb = [this](int sockfd){newConnection(sockfd);};
    acceptor_->setNewconnectionCallback(cb);
}

Server::~Server(){
}

void Server::handleEvent(Channel* ch){
    int fd = ch->Fd();
    char buf[READ_BUFFER];
    memset(buf,0,sizeof(buf));

    ssize_t bytes_read = read(fd,buf,sizeof(buf));
    if(bytes_read>0){
        printf("client fd %d says: %s\n",fd,buf);
        write(fd,buf,bytes_read);
    }
    else if(bytes_read == -1){
        perror("read");
    }
    else if(bytes_read ==0){
        printf("client fd %d disconnected\n",fd);
        loop_->removeChannel(ch);
        close(fd);
    }
}

void Server::newConnection(int sockfd){
    setNonblock(sockfd);
    Channel *channel = new Channel(loop_,sockfd);
    auto cb = [this,channel](){handleEvent(channel);};
    channel->setReadCallback(cb);
    channel->enableReading();
}
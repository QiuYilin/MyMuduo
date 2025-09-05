#include "Server.h"
#include <string.h>
#include <unistd.h>

const int READ_BUFFER = 1024;

Server::Server(const InetAddress& listenAddr,EventLoop *eventloop):loop_(eventloop),serv_socket_(new Socket){
    serv_socket_->bindAddress(listenAddr);
    serv_socket_->listen();
    serv_socket_->setNonblock();

    serv_channel_=new Channel(loop_,serv_socket_->fd());
    auto cb = [this](){newConnection(serv_socket_);};
    serv_channel_->setCallback(cb);
    serv_channel_->enableReading();
}

Server::~Server(){
    if(serv_socket_){
        delete serv_socket_;
    }
    if(serv_channel_){
        delete serv_channel_;
    }
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

void Server::newConnection(Socket *serv_sock){
    InetAddress cliaddr;
    Socket* cli_socket = new Socket(serv_sock->accept(&cliaddr));
    cli_socket->setNonblock();
    Channel *channel = new Channel(loop_,cli_socket->fd());
    auto cb = [this,channel](){handleEvent(channel);};
    channel->setCallback(cb);
    channel->enableReading();
}
#include "Connection.h"

#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"

Connection::Connection(EventLoop* loop, int sockfd)
    : loop_(loop),
      state_(StateE::kConnecting),
      socket_(std::make_unique<Socket>(sockfd)),
      channel_(std::make_unique<Channel>(loop, sockfd)) {
  channel_->setReadCallback([this]() { handleRead(); });
  channel_->setWriteCallback([this]() { handleWrite(); });
}

void Connection::send(Buffer* message) {}

void Connection::send(const void* message,int len){

}

void Connection::send(const std::string& message){
    if(state_==StateE::kDisconnected
    ){
        return;
    }

    bool faultError = false;
    ssize_t nwrote = 0;
    ssize_t reamining = message.size();
    if(!channel_->isWrite() && outputBuffer_.readableBytes()==0){
        nwrote = ::write(fd(),message.data(),message.size());
        if(nwrote >= 0){
            reamining = message.size() - nwrote;
            if(reamining == 0){

            }
        }
        else{
            nwrote =0;
            if(errno != EWOULDBLOCK){
                faultError = true;
            }
        }
    }

    assert(reamining <= message.size());
    if(!faultError && reamining > 0){
        outputBuffer_.append(message.data() + nwrote,reamining);
        if(!channel_->isWrite()){
            channel_->enableWriting();
        }
    }
}

void Connection::connectEstablished(){
    assert(state_ == StateE::kConnecting);
    setState(StateE::kConnected);
    channel_->enableReading();
}

void Connection::connectDestroyed(){
    if(state_ == StateE::kConnected){
        setState(StateE::kDisconnected);
        channel_->disableAll();
    }
    channel_->remove();
}

void Connection::handleRead(){
    int savedErrno = 0;
    auto n = inputBuffer_.readFd(fd(),&savedErrno);
    if(n > 0){
        messageCallback_(shared_from_this(),&inputBuffer_);
    }
    else if(n==0){
        handleClose();
    }
    else{
        printf("readFd error\n");
    }
}

void Connection::handleWrite(){
    if(!channel_->isWrite()){
        printf("Connection fd = %d is down,no more writing\n",channel_->Fd());
        return;
    }
    auto n = ::write(fd(),outputBuffer_.peek(),outputBuffer_.readableBytes());
    if(n > 0){
        outputBuffer_.retrieve(n);
        if(outputBuffer_.readableBytes() == 0){
            channel_->disableWriting();
        }
        else{
            printf("read to write more data\n");
        }
    }
    else{
        printf("handleWrite error\n");
    }
}

void Connection::handleClose(){
    assert(state_ == StateE::kConnected || state_ == StateE::kDisconnecting);
    setState(StateE::kDisconnected);
    channel_->disableAll();

    ConnectionPtr guardThis(shared_from_this());  
    printf("Connection::handleClose() guardThis(shared_from_this())�� user_count= %ld\n", guardThis.use_count());
    closeCallback_(guardThis);
}
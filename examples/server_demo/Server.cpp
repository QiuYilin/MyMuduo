#include"Server.h"
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

Server::Server(const InetAddr& listenAddr, EventLoop* eventloop)
	:loop_(eventloop)
	,ipPort_(listenAddr.toIpPort())
	,acceptor_(std::make_unique<Acceptor>(listenAddr,loop_))
{
	auto cb = [this](int sockfd,const InetAddr& peerAddr){newConnection(sockfd, peerAddr); };
	acceptor_->setNewconnectionCallback(cb);
}

Server::~Server()
{
	for (auto& item : connections_) {
		ConnectionPtr conn(item.second);
		item.second.reset();
		//printf("~Server() connections_.reset() later  user_count= %ld\n", item.second.use_count());

		conn->connectDestroyed();
	}
}

void Server::newConnection(int sockfd, const InetAddr& peerAddr)
{
	sockets::setNonblock(sockfd);

	InetAddr localAddr(sockets::getLocalAddr(sockfd));

	auto conn = std::make_shared<Connection>(loop_, sockfd, localAddr,peerAddr);
	connections_[sockfd] = conn;
	//printf("connections_[sockfd] = conn �� user_count= %ld\n", conn.use_count());


	conn->setMessageCallback(messageCallback_);
	conn->setCloseCallback([this](const ConnectionPtr& connection) {removeConnection(connection); });
	conn->setConnectionCallback(connectionCallback_);
	conn->connectEstablished();
}

void Server::removeConnection(const ConnectionPtr& conn)
{
	auto n = connections_.erase(conn->fd());	

	assert(n == 1);
	conn->connectDestroyed();
}



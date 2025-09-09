#include <Epoll.h>
#include <Socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <utils.h>
#include "Server.h"

void onMessage(const ConnectionPtr& conn, Buffer* buf) {
	std::string msg(buf->retrieveAllAsString());
	printf("onMessage() %ld bytes reveived:%s\n", msg.size(), msg.c_str());

	conn->send(msg);
}

int main()
{
	InetAddr servAddr(10000);
	EventLoop loop;
	Server server(servAddr, &loop);
	server.setMessageCallback([=](const ConnectionPtr& conn, Buffer* buf) {onMessage(conn, buf); });
	server.setConnectionCallback([](const ConnectionPtr& conn) {
		if (conn->connected()) {
			printf("new client fd %d ip:port: %s  connected..\n", conn->fd(), conn->peerAddress().toIpPort().c_str());
		}
		});
	loop.loop();

	return 0;
}


#include <Epoll.h>
#include <Socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <utils.h>
#include "Server.h"

const int MAXSIZE = 1024;

int main()
{
	InetAddress servAddr(10000);
	EventLoop loop;
	Server server(servAddr, &loop);
	loop.loop();

	return 0;
}


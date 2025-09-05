#include <Channel.h>
#include <Epoll.h>
#include <InetAddress.h>
#include <Socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <utils.h>

const int READ_BUFFER = 1024;
const int MAXSIZE = 1024;
void handleEvent(Channel* ch, Epoll& poll) {
  int fd = ch->Fd();
  char buf[READ_BUFFER];
  memset(buf, 0, sizeof(buf));
  ssize_t bytes_read = read(fd, buf, sizeof(buf));
  if (bytes_read > 0) {
    printf("client fd %d says: %s\n", fd, buf);
    write(fd, buf, bytes_read);
  } else if (bytes_read == -1) {
    perror_if(1, "read");
  } else if (bytes_read == 0) {
    printf("client fd %d disconnected\n", fd);
    poll.del(ch);
    close(fd);
  }
}

int main() {
  Socket serv_socket;
  InetAddress saddr(8080);

  serv_socket.bindAddress(saddr);
  serv_socket.listen();
  serv_socket.setNonblock();

  Epoll poll;
  Channel serv_channel(&poll, serv_socket.fd());
  serv_channel.enableReading();

  while (1) {
    std::vector<Channel*> activeChannel;
    poll.epoll_wait(activeChannel);
    int nums = activeChannel.size();
    for (int i = 0; i < nums; i++) {
      Channel* ch = activeChannel[i];
      if (ch->Fd() == serv_socket.fd()) {
        InetAddress addr;
        Socket* cli_socket = new Socket(serv_socket.accept(&addr));

        cli_socket->setNonblock();
        auto cliChannel = new Channel(&poll, cli_socket->fd());
        cliChannel->enableReading();
      } else if (ch->Revent() & EPOLLIN) {
        handleEvent(ch, poll);
      }
    }
  }
  return 0;
}
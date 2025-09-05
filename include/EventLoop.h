#pragma once
#include <vector>

class Channel;
class Epoll;

/// @brief 相对于以前的Reactor模式 将eventloop从Epoll中分离出来 用户只需要使用eventloop Epoll负责底层调用
class EventLoop {
 public:
  using channelList = std::vector<Channel*>;
  EventLoop();
  ~EventLoop();
  EventLoop(const EventLoop&) = delete;
  EventLoop& operator=(const EventLoop&) = delete;

  void loop();
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);

 public:
  Epoll* ep_;
  channelList activeChannels_;
};
#pragma once
#include <functional>

class EventLoop;
class Channel {
 public:
  using ReadEventCallback = std::function<void()>;
  Channel(EventLoop *loop, int fd);

  /// @brief 设置关心的events
  /// @param events
  void setEvents(int events);

  /// @brief 返回events
  /// @return
  int Event() const;
  void setRevents(int events);
  int Revent() const;

  /// @brief 包装epoll添加fd的操作
  void enableReading();
  bool isInEpoll();
  void setInEpoll(bool in);
  int Fd() const;

  void setCallback(ReadEventCallback cb) {
    readCallback_=std::move(cb);
  }

  void handleEvent();

 private:
  EventLoop* loop_;
  int fd_;          // 每个Channel都负责一个fd
  int events_;      // fd关心的事件
  int revents_;     // 目前该fd活动的事件
  bool isInEpoll_;  // 表示fd是否在epoll中，即是否正在监听

  ReadEventCallback readCallback_;
};
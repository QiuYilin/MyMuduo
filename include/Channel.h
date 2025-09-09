#pragma once
#include <functional>
#include <unistd.h>
#include <fcntl.h>
#include "Epoll.h"

class EventLoop;
class Channel {
 public:
  //using ReadEventCallback = std::function<void()>;
  using EventCallback = std::function<void()>;

  Channel(EventLoop *loop, int fd);

  /// @brief 设置关心的events
  /// @param events
  void setEvents(int events);
  /// @brief 返回events
  /// @return
  int Event() const;
  void setRevents(int events);
  int Revent() const;

  bool isInEpoll();
  void setInEpoll(bool in);
  int Fd() const;

  void setReadCallback(EventCallback cb) {
    readCallback_=std::move(cb);
  }
  void setWriteCallback(EventCallback cb){
    writeCallback_=std::move(cb);
  }
  void setCloseCallback(EventCallback cb){
    CloseCallback_=std::move(cb);
  }

  /// @brief 包装epoll添加fd的操作
  void enableReading(){
    events_ |= (EPOLLIN|EPOLLPRI);
    update();
  }
  void disableReading(){
    events_ &= ~(EPOLLIN|EPOLLPRI);
    update();
  }
  void enableWriting(){
    events_ |= EPOLLOUT;
    update();
  }
  void disableWriting(){
    events_&=~EPOLLOUT;
    update();
  }
  void disableAll(){
    events_=0;
    update();
  }

  bool isNoneEvent() const{
    return events_ ==0;
  }
  bool isWrite() const{
    return events_ & EPOLLOUT;
  }
  bool isRead() const{
    return events_&(EPOLLIN|EPOLLPRI);
  }

  void handleEvent();
  void remove();

 private:
  void update();
  EventLoop* loop_;
  int fd_;          // 每个Channel都负责一个fd
  int events_;      // fd关心的事件
  int revents_;     // 目前该fd活动的事件
  bool isInEpoll_;  // 表示fd是否在epoll中，即是否正在监听

  EventCallback readCallback_;
  EventCallback writeCallback_;
  EventCallback CloseCallback_;
};
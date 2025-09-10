#pragma once
#include<functional>
#include<unistd.h>
#include<fcntl.h>
#include<memory>
#include"Epoll.h"
class EventLoop;

class Channel
{
public:
	using ReadEventCallback = std::function<void()>;
	using EventCallback = std::function<void()>;


public:
	//Channel(Epoll* ep, int fd);
	Channel(EventLoop* loop, int fd);


	void setEvents(int events);
	int Event()const;
	void setRevents(int events);
	int Revent()const;

	bool isInEpoll();
	void setInEpoll(bool in);
	int Fd()const;


	void setReadCallback(EventCallback cb) { readCallback_ = std::move(cb); }
	void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
	void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
	void setErrorCallback(EventCallback cb){ errorCallback_ = std::move(cb); }


	void enableReading() { events_ |= (EPOLLIN | EPOLLPRI); update(); }
	
	void disableReading() { events_ &= ~(EPOLLIN | EPOLLPRI); update(); }
	void enableWriting() { events_ |= EPOLLOUT; update(); }
	void disableWriting() { events_ &= ~EPOLLOUT; update(); }
	void disableAll() {events_ = 0; update();}

	bool isNoneEvent()const { return events_ == 0; }
	bool isWrite()const {return events_ & EPOLLOUT; }
	bool isRead()const {return events_& (EPOLLIN | EPOLLPRI); }

	void handleEvent();
	void remove();

	/// @brief 监测Connection是否存活a
	/// @param  
	void tie(const std::shared_ptr<void>&);
private:
	void update();
	void handleEventWithGuard();
private:
	EventLoop* loop_;

	int fd_;
	int events_;
	int revents_;
	bool isInEpoll_;

	std::weak_ptr<void> tie_;//监测Connection是否存活b
	bool tied_;//监测Connection是否存活c
	
	EventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback closeCallback_;
	EventCallback errorCallback_;
};
#pragma once
#include<vector>
#include<memory>
#include<atomic>
#include<thread>
#include<mutex>
#include <sys/eventfd.h>
#include<functional>
class Channel;
class Epoll;

class EventLoop
{
public:
	using Functor = std::function<void()>;//转移到eventloop的回调函数
	using channelList=std::vector<Channel*>;
public:
	EventLoop();
	~EventLoop();

	void loop();
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);

	/// @brief 判断该线程是否是该IO线程
	/// @return 
	bool isInLoopThread()const { return threadId_ == std::this_thread::get_id(); }
  /// @brief 该函数先判断此时刻的线程是否是该IO线程，若是就直接执行cb回调任务函数；若不是，就放到该eventloop中的任务队列来等待或立刻执行
  /// @param cb 
  void runInLoop(Functor cb);
	void queueInLoop(Functor cb);

	/// @brief 用来唤醒loop所在线程 向wakeupfd写一个数据 wakeupChannel就发生读事件，当前
	void wakeup();


	void quit();	
private:
	void doPendingFunctors();	
  /// @brief 用于响应wake up
  void handleRead();	



	std::thread::id threadId_;//IO线程的线程id在构造函数中使用std::this_thread::get_id()获得id
	std::atomic_bool quit_;

	std::atomic_bool callingPendingFunctors_;//标识当前loop是否有需要执行的回调操作

	std::unique_ptr<Epoll> ep_;
	channelList activeChannels_;

	int wakeupFd_;
	std::unique_ptr<Channel> wakeupChannel_;

	std::vector<Functor> pendingFunctors_;//存储loop需要执行的所有回调任务函数
	std::mutex mutex_; //保护pendingFunctors_
};
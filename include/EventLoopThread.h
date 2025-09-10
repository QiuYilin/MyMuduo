#pragma once
#include<thread>
#include<mutex>
#include<condition_variable>

class EventLoop;
/// @brief 封装了EventLoop和Thread
class EventLoopThread
{
public:
	EventLoopThread();
	~EventLoopThread();

	/// @brief 启动IO线程函数中的loop循环，返回IO线程中创建的EventLoop
	/// @return 
	EventLoop* startLoop();

private:
	/// @brief IO线程函数 创建EventLoop局部对象，运行loop循环
	void threadFunc();

	EventLoop* loop_;//绑定的EventLoop对象指针

	std::thread thread_;
	std::mutex mutex_;
	std::condition_variable cond_;
};
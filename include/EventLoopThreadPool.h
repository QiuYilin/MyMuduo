#pragma once
#include<vector>
#include<memory>
#include<functional>

class EventLoop;
class EventLoopThread;
/// @brief 时间循环线程池类
class EventLoopThreadPool
{
public:
	EventLoopThreadPool(EventLoop* base);
	~EventLoopThreadPool();

	void setThreadNum(int numThreads) { numThreads_ = numThreads; }
    /// @brief 启动线程池
    void start();

	EventLoop* getNextLoop();

	bool started()const { return started_; }
private:
	EventLoop* baseLoop_;
	bool started_;
	int numThreads_;
	int next_;
	std::vector<std::unique_ptr<EventLoopThread>> threads_;
	std::vector<EventLoop*> loops_;
};
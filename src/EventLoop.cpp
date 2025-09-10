#include"EventLoop.h"
#include"Channel.h"
#include"Epoll.h"
#include<signal.h>

int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0)
	{
		printf("eventfd error:%d\n", errno);
	}
	return evtfd;
}

class IgnoreSigPipe
{
public:
	IgnoreSigPipe()
	{
		::signal(SIGPIPE, SIG_IGN);
	}
};

IgnoreSigPipe initObj;

EventLoop::EventLoop()
	:threadId_(std::this_thread::get_id())
	,quit_(false)
	, callingPendingFunctors_(false)
	,ep_(std::make_unique<Epoll>())
	,wakeupFd_(createEventfd())
	,wakeupChannel_(std::make_unique<Channel>(this,wakeupFd_))
{
	wakeupChannel_->setReadCallback([this]() {handleRead(); });
	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
	wakeupChannel_->disableAll();
	wakeupChannel_->remove();
	::close(wakeupFd_);
}

void EventLoop::loop()
{
	quit_ = false;
	while (!quit_) {
		activeChannels_.clear();
		ep_->epoll_wait(activeChannels_,10000);
		for (auto& active : activeChannels_) {
			active->handleEvent();
		}

		doPendingFunctors();	
	}
}
void EventLoop::updateChannel(Channel* channel)
{
	ep_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
	ep_->del(channel);
}

void EventLoop::quit() {
	quit_ = true;
}


void EventLoop::runInLoop(Functor cb)
{
	if (isInLoopThread()) {
		cb();
	}
	else {
		queueInLoop(std::move(cb));
	}
}

void EventLoop::queueInLoop(Functor cb)
{
	{
		std::lock_guard<std::mutex> lock(mutex_);
		pendingFunctors_.push_back(std::move(cb));
	}

	if (!isInLoopThread() || callingPendingFunctors_) {
		wakeup();
	}
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = write(wakeupFd_, &one, sizeof(one));
	if (n != sizeof(one)){
		printf("EventLoop wakeup write %lu bytes instead of 8 \n", n);
	}
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	auto n = ::read(wakeupFd_, &one, sizeof(one));
	if (n != sizeof(one)){
		printf("EventLoop::handleRead() reads %lu bytes \n", n);
	}
}

void EventLoop::doPendingFunctors()	
{
	std::vector<Functor> functors;
	callingPendingFunctors_ = true;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		functors.swap(pendingFunctors_);
	}

	for (const auto& functor : functors) {
		functor();	
	}

	callingPendingFunctors_ = false;
}



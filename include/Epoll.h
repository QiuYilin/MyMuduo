#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;

/// @brief 包装epoll()函数
class Epoll{
public:
    /// @brief 包装epoll_create
    Epoll();
    /// @brief 删除所有事件
    ~Epoll();

    /// @brief 
    /// @param ch 
    void updateChannel(Channel* ch);

    /// @brief 
    /// @param ch 
    void del(Channel* ch);
    /// @brief 包装epoll_wait
    /// @param active 
    /// @param timeout 
    void epoll_wait(std::vector<Channel*>& active, int timeout=10);
private:
    int epfd_;
    struct epoll_event* events_;
};
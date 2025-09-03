#pragma once
#include <sys/epoll.h>
#include <vector>

/// @brief 包装epoll()函数
class Epoll{
public:
    /// @brief 包装epoll_create
    Epoll();
    /// @brief 删除所有事件
    ~Epoll();
    Epoll(const Epoll& other) = delete;
    Epoll& operator=(const Epoll& other)=delete;

    /// @brief 包装epoll_ctl
    /// @param sockfd 
    /// @param events 
    /// @param op 
    void update(int sockfd,int events,int op);

    /// @brief 额外包装epoll_ctl中删除文件描述符的操作
    /// @param fd 
    void epoll_delete(int fd);

    /// @brief 包装epoll_wait
    /// @param active 
    /// @param timeout 
    void epoll_wait(std::vector<epoll_event>& active, int timeout=10);
private:
    int epfd_;
    struct epoll_event* events_;
};
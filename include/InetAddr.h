#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

/// @brief 抽象包装ipv4地址类型
class InetAddr
{
public:
    /// @brief 通过端口号和ip地址构造bind()需要的地址结构
    /// @param port  端口号
    /// @param ip ip地址
    explicit InetAddr(uint16_t port = 0, std::string ip = "127.0.0.1");
    /// @brief 直接通过sockadrr_in构造bind()需要的地址结构
    /// @param addr socket api ipv4所需的基本地址结构
    explicit InetAddr(const sockaddr_in &addr)
        : addr_(addr)
    {
    }

    /// @brief 获取ip地址
    /// @return 
    std::string toIp() const;
    /// @brief 获取ip:port字符串
    /// @return 
    std::string toIpPort() const;
    /// @brief 获取端口号
    /// @return 
    uint16_t toPort() const;

    /// @brief 获取sockaddr_in
    /// @return 
    const sockaddr_in *getSockAddr() const { return &addr_; }
    /// @brief 直接设置sockaddr_in
    /// @param addr 
    void setSockAddr(const sockaddr_in &addr) { addr_ = addr; }

private:
    sockaddr_in addr_;
};
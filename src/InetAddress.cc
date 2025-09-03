#include "temp_utils.h"
#include "InetAddress.h"

#include <string.h>
#include <strings.h>

InetAddress::InetAddress(uint16_t port, std::string ip) {
  ::memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_port = ::htons(port);
  struct in_addr addr;
  memset(&addr, 0, sizeof(addr));
  if (::inet_aton(ip.c_str(), &addr)) {
    addr_.sin_addr.s_addr = ::inet_addr(ip.c_str());
  }
  else{
    error_handling("Invalid ip address!");
  }
}

std::string InetAddress::toIp() const {
  char buf[64] = {0};
  ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
  return buf;
}

std::string InetAddress::toIpPort() const {
  char buf[64] = {0};
  ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
  size_t end = ::strlen(buf);
  uint16_t port = ::ntohs(addr_.sin_port);
  sprintf(buf + end, ":%u", port);
  return buf;
}

uint16_t InetAddress::toPort() const { return ::ntohs(addr_.sin_port); }
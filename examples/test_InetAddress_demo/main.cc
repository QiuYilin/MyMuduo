#include <iostream>
#include "InetAddress.h"
int main()
{
    InetAddress addr(8080);
    std::cout << addr.toIpPort() << std::endl;
}
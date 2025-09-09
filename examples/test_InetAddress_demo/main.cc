#include <iostream>
#include "InetAddr.h"
int main()
{
    InetAddr addr(8080);
    std::cout << addr.toIpPort() << std::endl;
}
#pragma once
#include <stdio.h>
#include <stdlib.h>

void error_handling(char *message){//TODO 线程不安全
    fputs(message,::stderr);
    fputc('\n',::stderr);
    exit(1);
}
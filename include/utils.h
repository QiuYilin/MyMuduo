#pragma once
#include <stdio.h>
#include <stdlib.h>

inline void perror_if(bool condition,const char* errorMessage){
    if(condition){
        perror(errorMessage);
        exit(1);
    }
}
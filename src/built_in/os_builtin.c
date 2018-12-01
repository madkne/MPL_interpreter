//
// Created by amindelavar on 11/30/2018.
//
#include <MPL/system.h>

//************************************************
void _OS_TYPE__exit(int32 i) {
  __syscall_exit(i);
}


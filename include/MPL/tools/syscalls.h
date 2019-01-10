//
// Created by amindelavar on 11/30/2018.
//

#ifndef MPL_SYSCALLS_H
#define MPL_SYSCALLS_H
void __syscall_exit(int32 i);
String __syscall_hostname();
long_int __syscall_pid();
String __syscall_abspath(String path);
long_int __syscall_unix_time();
String __syscall_datetime(uint8 type);
#endif //MPL_SYSCALLS_H

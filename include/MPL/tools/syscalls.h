//
// Created by amindelavar on 11/30/2018.
//

#ifndef MPL_SYSCALLS_H
#define MPL_SYSCALLS_H
String __syscall_hostname();
String __syscall_homedir();
long_int __syscall_pid();
String __syscall_abspath(String path);
long_int __syscall_unix_time();
String __syscall_datetime(uint8 type);
String __syscall_read_input();
int32 __syscall_read_file(String path, str_list *lines, Boolean skip_empty_lines);
Boolean __syscall_write_file(String path,String s);
String  __syscall_get_line(FILE *fp);
Boolean __syscall_mkdir(String path,Boolean is_make_parents);
uint32 __syscall_rand(uint32 min, uint32 max);
Boolean __syscall_binary_copy(String src, String dst);
#endif //MPL_SYSCALLS_H

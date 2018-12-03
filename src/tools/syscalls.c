//
// Created by amindelavar on 11/30/2018.
//

#include <MPL/system.h>

void __syscall_exit(int32 i) {
  if (is_programmer_debug >= 1) {
    AppStartedClock = clock() - AppStartedClock;
    double time_taken = ((double) AppStartedClock) / CLOCKS_PER_SEC; // in seconds
    //exit state
    String exit_state = 0, unit = 0;
    if (i == 0)str_init(&exit_state, "EXIT_SUCCESS");
    else str_init(&exit_state, "EXIT_FAILURE");
    if (time_taken == 0) {
      time_taken += 0.000001;
      //printf("BAD\n");
    }
    if (time_taken >= 60) {
      time_taken /= 60;
      str_init(&unit, "minutes");
    } else str_init(&unit, "seconds");
    printf("Process finished during %.6f %s with exit code %i (%s)\n", time_taken, unit, i, exit_state);
  }
  exit(i);
}

//******************************************
String __syscall_hostname() {
  #if WINDOWS_PLATFORM == true
  return getenv("COMPUTERNAME");
  #elif LINUX_PLATFORM == true
  return getenv("HOSTNAME");
  #endif
  return "<unknown>";
}
//******************************************
long_int __syscall_pid() {
  #if WINDOWS_PLATFORM == true
  return (long_int) GetCurrentProcessId();
  #elif LINUX_PLATFORM == true
  //TODO
  #endif
  return 0;
}

//******************************************
String __syscall_abspath(String path) {
  //uint8 resolved_path[PATH_MAX+1];
  #if LINUX_PLATFORM == 1
  String abs_path=0;
  abs_path=realpath(path, 0);
  //printf("\n%s=>%s\n",path,abs_path);
  return abs_path;
  #elif WINDOWS_PLATFORM == 1
  char tmp[BufferSize];
  String abs_path = 0;
  GetFullPathName(path, BufferSize, tmp, 0);
  str_init(&abs_path, tmp);
  if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(abs_path) && GetLastError() == ERROR_FILE_NOT_FOUND) return 0;
  //printf("\nSSS::%s=>%s\n",path,abs_path);
  return abs_path;
  #endif
  return 0;
}

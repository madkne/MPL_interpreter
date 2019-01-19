//
// Created by amindelavar on 11/30/2018.
//

#include <MPL/system.h>

void __syscall_exit(int32 i) {
  //=>store all session entries to database in disk
  if(!flush_session_entries()){
    //TODO:error
  }
  //=>show exit message if in program debug
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
//******************************************
long_int __syscall_unix_time() {
  return (long_int) time(NULL);
}
//******************************************
String __syscall_datetime(uint8 type) {
  time_t t = time(NULL);
  struct tm tim = *localtime(&t);
  //char s[64];
  String ret = 0;
  //date and time : 2018-6-23 3:45:12
  if (type == 1) {
    ret = malloc(6 * sizeof(int) + 5);
    sprintf(ret, "%i-%i-%i %i:%i:%i", tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min,
            tim.tm_sec);
  }

  //strftime(s, sizeof(s), "%c", tm);
  return ret;
}
//******************************************
String __syscall_read_input() {
  String ret = "";
  uint8 c = 0;
  for (;;) {
    c = fgetc(stdin);
    if (c == EOF || c == '\n'/*ENTER*/) break;
    ret = char_append(ret, c);
  }
  return ret;
}
//******************************************
int32 __syscall_read_file(String path, str_list *lines, Boolean skip_empty_lines) {
  FILE *fp = fopen(path, "r");
  if (fp == NULL)return -1;
  // printf ("PATH:%s\n", path);
  int32 lines_co = 0;
  for (;;) {
    String line = 0;
    for (;;) {
      int32 buf = fgetc(fp);
      if (buf == EOF) {
        if (line[0] != 0)str_list_append(&(*lines), line, lines_co++);
        goto ENDOFFILE;
      }
      if (buf == 0)continue;
      if ('\n' == buf || '\r' == buf)break;
      line = char_append(line, buf);
//      printf("-----%s\n", line);
    }
    if (skip_empty_lines) {
      line = str_trim_space(line);
      if (line == 0)continue;
    }
    //printf ("LI:%s\n", line);
    str_list_append(&(*lines), line, lines_co++);
  }
  ENDOFFILE:
  fclose(fp);
  return lines_co;

}
//******************************************
Boolean __syscall_write_file(String path, String s) {
  FILE *fp = fopen(path, "w");
  if (fp == NULL)return false;
  //=>write to file
  fwrite(s, sizeof(uint8), str_length(s), fp);
  fclose(fp);
  return true;
}



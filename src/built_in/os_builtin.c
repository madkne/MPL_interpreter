//
// Created by amindelavar on 11/30/2018.
//
#include <MPL/system.h>

//************************************************
void _OS_TYPE__exit(int32 i) {
  __syscall_exit(i);
}
//************************************************
/**
 * print any value or variable and return true if successful
 * @param items
 * @param len
 * @return Boolean
 * @samples print(s,"\n")||print({4,8.7},"er"+"AS",67.5};
 */
Boolean _OS_TYPE__print(str_list items, uint32 len) {
//  printf("!!!!:%s(%i)\n",print_str_list(items,len),len);
  for (uint32 i = 0; i < len; i++) {
    printf(replace_control_chars(items[i]));
  }

  return true;
}

//************************************************
String _OS_TYPE__input(String type) {
  if (str_equal(type, READ_CHAR_INPUT_TYPE)) {
    return str_trim_space(char_to_str(fgetc(stdin)));
  } else if (str_equal(type, READ_LINE_INPUT_TYPE)) {
    return __syscall_read_input();
  }
  return 0;
}
//************************************************
String _OS_TYPE__shell(String command) {
  //system(command);
  String ret = 0;
  FILE *fp;
  uint8 line[1035];
  /* Open the command for reading. */
  fp = _popen(command, "r");
  if (fp == 0) {
    exception_user_handler(ERROR_ID, "bad_command", "Failed to run command", "shell");
    return 0;
  }
  /* Read the output a line at a time - output it. */
  while (fgets(line, sizeof(line) - 1, fp) != NULL) {
    ret = str_append(ret, line);
  }
  /* close */
  pclose(fp);
  if (str_length(ret) < 2 && (ret[0] < 32 || ret[0] > 126)) {
    exception_user_handler(ERROR_ID, "bad_output", "Failed to get output of command", "shell");
    return 0;
  }

  return ret;
}
//************************************************
String _OS_TYPE__time() {
  return str_from_long_int(__syscall_unix_time());
}
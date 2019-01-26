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
  str_list lines = 0;
  //=>store errors of command in stderr.txt
  command = str_multi_append(command, " 2>", interpreter_tmp_path, "stderr.txt", 0, 0);
  /* Open the command for reading. */
  fp = popen(command, "r");
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
//  printf("QQ:%s$\n",__syscall_get_line(stderr));
  //=>raise an error when stdout is empty
  if (str_length(ret) < 2 || (ret[0] < 32 || ret[0] > 126)) {
    //=>if stdout is empty, then get errors from stderr.txt
    int32 lines_len = __syscall_read_file(str_append(interpreter_tmp_path, "stderr.txt"), &lines, true);
//    printf("EEE:%s=>%i\n", str_append(interpreter_tmp_path, "stderr.txt"), lines_len);
    ret = 0;
    if (lines_len <= 0)return 0;
    for (uint32 i = 0; i < lines_len; i++) {
      ret = str_append(ret, lines[i]);
      if (i + 1 < lines_len)ret = char_append(ret, '\n');
    }
    if (ret != 0)exception_user_handler(ERROR_ID, "bad_output", "Failed to get output of command", "shell");
  }

  return ret;
}
//************************************************
String _OS_TYPE__time() {
  return str_from_long_int(__syscall_unix_time());
}
//************************************************
String _OS_TYPE__rand(String min, String max) {
  //=>if min or max are negative numbers
  if (min[0] == '-' || max[0] == '-') {
    exception_user_handler(ERROR_ID, "bad_inputs", "Failed to generate a random number in negative interval", "rand");
    return "-1";
  }
  //=>determine sub type of min and max
  uint8 type = set_type_of_math(&min, &max);
  //=>if sub type is int(i) or huge(h)
  if (type == 'i' || type == 'h')return str_from_long_int(__syscall_rand(str_to_long_int(min), str_to_long_int(max)));
  //=>if sub type is float(f)
  if (type == 'f') {
    double f1 = str_to_double(min);
    double f2 = str_to_double(max);
    return str_from_long_int(__syscall_rand((uint32) f1, (uint32) f2));
  }
  exception_user_handler(ERROR_ID,
                         "bad_inputs",
                         "Failed to generate a random number between min and max inputs",
                         "rand");
  return "-1";
}
//************************************************
String _OS_TYPE__argvs() {
  String ret = 0;
  if (argvs_len == 0)return "null";
  ret = char_append(ret, '{');
  for (uint32 i = 0; i < argvs_len; i++) {
    ret = str_append(ret, convert_to_string(program_argvs[i]));
    if (i + 1 < argvs_len)ret = char_append(ret, ',');
  }
  ret = char_append(ret, '}');
  return ret;
}
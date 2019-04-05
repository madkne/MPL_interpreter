//
// Created by amindelavar on 11/30/2018.
//
#include <MPL/system.h>

//************************************************
void _OS_TYPE__exit(int32 i) {
  exit_runtime(i);
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
//************************************************
/**
 * display on console a formatted text
 * @param types
 * @param items
 * @param len
 * @return Boolean
 */
Boolean _OS_TYPE__printf(str_list types, str_list items, uint32 items_len) {
//  printf("!!!!:%s(%i)\n", print_str_list(items, items_len), items_len);
  //init vars
  String text = items[0];
  uint32 len = str_length(text);
  String ret = 0, format = 0, retf = 0;
  uint32 items_ind = 1;
  //---------------------------
  for (uint32 i = 0; i < len; i++) {
    //=>if was %%
    if (text[i] == '%' && i + 1 < len && text[i + 1] == '%') {
      ret = char_append(ret, text[i]);
      i++;
      continue;
    }
    if (text[i] == '%' && (i == 0 || text[i - 1] != '\\') && i + 1 < len) {
      format = 0, retf = 0;
      uint32 j;
      for (j = i + 1; j < len; j++) {
        if ((text[j] >= '0' && text[j] <= '9') || text[j] == '.' || text[j] == 's' || text[j] == 'i' || text[j] == 'f'
            || text[j] == 'h' || text[j] == 'b')
          format = char_append(format, text[j]);
        else break;
      }
      i = j - 1;
      if (items_ind >= items_len) {
        exception_user_handler(ERROR_ID, "not_enough_inputs", "count of parameters is not enough", "printf");
        return false;
      }
      if (!_OS_TYPE__printf_format(format, items[items_ind++], types[items_ind], &retf)) {
        exception_user_handler(ERROR_ID,
                               "bad_entry_format",
                               str_multi_append("entry \'",
                                                format, "\' or value \'",
                                                items[items_ind - 1],
                                                "\' not valid", 0), "printf");
        return false;
      }
//      printf("Format:%s=>%s\n", format, retf);
      ret = str_append(ret, retf);
      continue;
    } else ret = char_append(ret, text[i]);
  }
  //=>display ret
  printf(replace_control_chars(ret));
  return true;
}
//************************************************
Boolean _OS_TYPE__printf_format(String f, String s, String type, String *ret) {
  /**
 * %s:string(hello)                     ---OK---
 * %i:integer(455)                      ---OK---
 * %f:float(4.656)                      ---OK---
 * %h:huge(45.99999..)                  ---OK---
 * %b:bool(true)                        ---OK---
 * %%:%                                 ---OK---
 * %3s:hello=>$hel$                     ---OK---
 * %10s:hello=>$     hello$             ---OK---
 * %3i:4557.7=>455$                     ---OK---
 * %5i:4557.7=>04557                    ---OK---
 * %2.3f:456.4645=>45.464               ---OK---
 * %4.3f:456.4645=>0456.464             ---OK---
 * %.3h(%0.3h:BAD):324.99999=>324.999   ---OK---
 * %2.h:324.99999=>32.99999             ---OK---
 * %2.5h:345.67=>34.67000               ---OK---
   */
  //init vars
  uint32 flen = str_length(f);
  uint32 slen = str_length(s);
  uint32 ico = 0;//=>count of integer digits
  uint32 aico = 0; //=>count of extra integer digits
  uint32 fco = 0;//=>count of float digits
  uint32 afco = 0; //=>count of extra float digits
  //----------------------------
  //=>if format is bool
  if (str_ch_equal(f, 'b'))(*ret) = str_from_bool(str_to_bool(s));
    //=>if format is str
  else if (str_has_suffix(f, "s")) {
    //=>check input is valid as a string
    if (!str_equal(type, "str"))return false;
    //=>if has number limit
    String tmp = str_substring(f, 0, flen - 1);
    if (tmp == 0)ico = slen;
    else ico = str_to_long_int(tmp);
    if (ico > slen) {
      aico = ico - slen;
      ico = slen;
    }
    //=>set limit and create result
    (*ret) = str_substring(s, 0, ico);
    //=>set extra whitespaces with aico
    if (aico > 0) {
      String tmp1 = 0;
      for (uint32 i = 0; i < aico; i++)tmp1 = char_append(tmp1, ' ');
      (*ret) = str_append(tmp1, (*ret));
    }
    return true;
  }
    //=>if format is num
  else if (str_has_suffix(f, "i") || str_has_suffix(f, "f") || str_has_suffix(f, "h")) {
    //=>check input is valid as a num
    if (!str_is_num(s))return false;
    uint8 num_type = f[flen - 1];
    //=>convert input based on num_type
    if (num_type == 'i')(*ret) = resize_to_int(s);
    else if (num_type == 'f')(*ret) = resize_to_float(s);
    //=>if not has any limit
    if (flen == 1) {
      (*ret) = s;
      return true;
    }
      //=>if has number limit,then split ico,fco
    else {
      //=>remove sub type from end of format
      f = str_substring(f, 0, flen - 1);
      flen--;
      str_list tmp = 0, tmp1 = 0;
      uint32 ll = char_split(f, '.', &tmp, false);
      if (ll > 2 || (num_type == 'i' && ll == 2))return false;
      uint32 ll1 = char_split(s, '.', &tmp1, true);
      uint32 ll1_1 = str_length(tmp1[0]);
      uint32 ll1_2 = (ll1 == 2) ? str_length(tmp1[1]) : 0;
      //=>determine ico,fco
      ico = (tmp[0] != 0) ? str_to_long_int(tmp[0]) : ll1_1;
      if (ll == 2) fco = (tmp[1] != 0) ? str_to_long_int(tmp[1]) : ll1_2;
      else fco = 0;
//      if (ll == 2)printf("SSSS:%i,%i(%s)[%i](%i,%i)\n", ico, fco, tmp[1], ll, ll1_1, ll1_2);
      //=>detemine aico,afco
      if (ll1_1 < ico) {
        aico = ico - ll1_1;
        ico = ll1_1;
      }
      if (ll1_2 < fco) {
        afco = fco - ll1_2;
        fco = ll1_2;
      }
      //=>set limit and create int,float segments
      String integer_s = str_substring(tmp1[0], 0, ico);
      String float_s = (ll1 == 2) ? str_substring(tmp1[1], 0, fco) : 0;
      //=>set extra zeros with aico,afco
      if (aico > 0) {
        String tmp2 = 0;
        for (uint32 i = 0; i < aico; i++)tmp2 = char_append(tmp2, '0');
        integer_s = str_append(tmp2, integer_s);
      }
      if (afco > 0) {
        String tmp2 = 0;
        for (uint32 i = 0; i < afco; i++)tmp2 = char_append(tmp2, '0');
        float_s = str_append(float_s, tmp2);
      }
      //=>create final result
      (*ret) = integer_s;
      if (float_s != 0)(*ret) = str_multi_append((*ret), ".", float_s, 0, 0, 0);
      return true;
    }
  }
  return false;
}
//************************************************
String _OS_TYPE__date_english(long_int timestamp, String format) {
/**
 * format:
 * #y : 2019
 * #m : 1..12
 * #M : Jan,Feb,..
 * #d : 1..31
 * #D : 0..365
 * #h : 1..12
 * #H : 0..23
 * #i : 0..59
 * #s : 0..59
 * #t : AM,PM
 * #w : 1..7
 * #W : Sun,Fri,..
 * #z : (Win32)Iran Standard Time
 * #Z : (Win32) (UTC+03:30) Tehran
 */
  //=>init vars
  String timezone = 0;
  String days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  //------------------------------------
  //=>get time struct
  time_t t = timestamp;
  struct tm tim = *localtime(&t);
  //=>set #y format
  format = str_replace(format, "#y", str_from_int32(tim.tm_year + 1900), -1);
  //=>set #m format
  format = str_replace(format, "#m", str_from_int32(tim.tm_mon + 1), -1);
  //=>set #M format
  format = str_replace(format, "#M", months[tim.tm_mon], -1);
  //=>set #d format
  format = str_replace(format, "#d", str_from_int32(tim.tm_mday), -1);
  //=>set #D format
  format = str_replace(format, "#D", str_from_int32(tim.tm_yday), -1);
  //=>set #h format
  format = str_replace(format, "#h", str_from_int32(_OS_TYPE__get_12_from_24hours((uint8) tim.tm_hour)), -1);
  //=>set #H format
  format = str_replace(format, "#H", str_from_int32(tim.tm_hour), -1);
  //=>set #i format
  format = str_replace(format, "#i", str_from_int32(tim.tm_min), -1);
  //=>set #s format
  format = str_replace(format, "#s", str_from_int32(tim.tm_sec), -1);
  //=>set #t format
  if (tim.tm_hour > 12) format = str_replace(format, "#t", "PM", -1);
  else format = str_replace(format, "#t", "AM", -1);
  //=>set #w format
  format = str_replace(format, "#w", str_from_int32(tim.tm_wday), -1);
  //=>set #W format
  format = str_replace(format, "#W", days[tim.tm_wday], -1);
  //=>set #z format
  if (str_indexof(format, "#z", 0) != -1) {
    //=>Get the timezone info.
    #if WINDOWS_PLATFORM == true
    timezone = __syscall_reg_value(HKEY_LOCAL_MACHINE,
                                   "SYSTEM\\CurrentControlSet\\Control\\TimeZoneInformation", "TimeZoneKeyName");
    #elif LINUX_PLATFORM == true
    //TODO
    #endif
    format = str_replace(format, "#z", timezone, -1);
  }
  //=>set #Z format
  if (str_indexof(format, "#Z", 0) != -1) {
    //=>Get the timezone info.
    #if WINDOWS_PLATFORM == true
    String tmp1 = __syscall_reg_value(HKEY_LOCAL_MACHINE,
                                      "SYSTEM\\CurrentControlSet\\Control\\TimeZoneInformation", "TimeZoneKeyName");
    String timezone = __syscall_reg_value(HKEY_LOCAL_MACHINE,
                                          str_append("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\",
                                                     tmp1), "Display");
    #elif LINUX_PLATFORM == true
    //TODO
    #endif
    format = str_replace(format, "#Z", timezone, -1);
  }
  return format;
}
//************************************************
uint8 _OS_TYPE__get_12_from_24hours(uint8 h24) {
  if (h24 == 0)return 12;
  if (h24 < 13)return h24;
  return h24 - 12;
}
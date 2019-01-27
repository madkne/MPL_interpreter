//
// Created by amindelavar on 12/2/2018.
//
#include<MPL/system.h>

Boolean edit_magic_macro(uint8 type, String key, String value) {
  mama *tmp1 = entry_table.mama_start;
  if (tmp1 == 0) return false;
//  printf("SSSS:%s=>%s\n",key,value);
  for (;;) {
    if (tmp1->type == type && str_equal(tmp1->key, key)) {
      str_init(&tmp1->value, value);
      return true;
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return false;
}
//***********************************
Boolean edit_magic_config(String key, String value, String val_type) {
//  printf("$CONFIG:%s=>%s(%s)\n",key,value,val_type);
  //=>ErrorsState or WarningsState
  Boolean is_err = false, not_exist = false;
  if ((is_err = str_equal(key, "ErrorsState")) || str_equal(key, "WarningsState")) {
    if (!str_equal(val_type, "str")) {
      //TODO:error
      return false;
    }
    //=>remove quotations if value is string
    value = str_reomve_quotations(value, val_type);
    //=>ErrorsState change
    if (is_err) {
      if (str_equal(value, "fatal"))errors_mode = FATAL_ID;
      else if (str_equal(value, "error"))errors_mode = ERROR_ID;
      else not_exist = true;
    }
      //=>WarningsState change
    else {
      if (str_equal(value, "warning"))warnings_mode = WARNING_ID;
      else if (str_equal(value, "error"))warnings_mode = ERROR_ID;
      else if (str_equal(value, "fatal"))warnings_mode = FATAL_ID;
      else not_exist = true;
    }
    //=>if value is not valid
    if (not_exist) {
      //TODO:error
      return false;
    }
//    printf("@WW:%s(%i):%i,%i\n",value,is_err,errors_mode,warnings_mode);
  }
  //----------------------------------------
  //=>MaxHugeDivideSteps or MaxHugeDecimalNumbers
  Boolean divide = false, decimal = false;
  int32 value_int = 0;
  if ((divide = str_equal(key, "MaxHugeDivideSteps")) || (decimal = str_equal(key, "MaxHugeDecimalNumbers"))) {
    if (!str_equal(val_type, "num") || determine_type_num(value) != 'i' || !str_is_int32(value)
        || (value_int = str_to_int32(value)) < 1) {
      //TODO:error
      return false;
    }
    //=>MaxHugeDivideSteps change
    if (divide) max_steps_estimate_huge = (uint8) value_int;
      //=>MaxHugeDecimalNumbers change
    else if (decimal) max_decimal_has_huge = (uint32) value_int;
  }

  //----------------------------------------
  //=SessionMode,HelpArgumentMode,OverwriteBuiltinMode,DebugMode,PackageMode,AccessVariablesMode,BuildMode
  if (str_has_suffix(key, "Mode")) {
    if (!str_equal(val_type, "bool")) {
      //TODO:error
      return false;
    }
    Boolean value_bool = str_to_bool(value);
    if (str_equal(key, "SessionMode"))session_mode = value_bool;
    else if (is_real_mpl && str_equal(key, "BuildMode"))build_mode = value_bool;
    else if (str_equal(key, "HelpArgumentMode"))help_argv_mode = value_bool;
    else if (str_equal(key, "OverwriteBuiltinMode"))overwrite_builtin_mode = value_bool;
    else if (str_equal(key, DEBUG_MODE))debug_mode = value_bool;
    //TODO:PackageMode,AccessVariablesMode
  }
  //----------------------------------------
  //=>ExportByteCode,ExportLogFile,SessionDatabasePath,ExportBuildFile
  Boolean exportb = false, exportl = false, dbpath = false, exportbu = false;
  if ((exportb = str_equal(key, "ExportByteCode")) || (exportl = str_equal(key, "ExportLogFile"))
      || (dbpath = str_equal(key, "SessionDatabasePath")) || (exportbu = str_equal(key, "ExportBuildFile"))) {
    if (!str_equal(val_type, "str")) {
      //TODO:error
      return false;
    }
    //=>remove quotations if value is string
    value = str_reomve_quotations(value, val_type);
    //=>convert path to absolute path
    value = convert_mplpath_to_abspath(value);
    if (exportb)bytecode_path = value;
    else if (exportl)logfile_path = value;
    else if (exportbu)buildfile_path = value;
    else if (dbpath)sessiondb_path = value;
  }
  //----------------------------------------
  //=>AppIcon
  if (str_indexof(key, "App", 0) == 0) {
    if (!str_equal(val_type, "str")) {
      //TODO:error
      return false;
    }
    //=>remove quotations if value is string
    value = str_reomve_quotations(value, val_type);
    if (str_equal(key, "AppIcon")) {
      value = convert_mplpath_to_abspath(value);
      String ret = 0;
      for (uint32 i = 0; i < str_length(value); i++) {
        if (value[i] == '\\') {
          ret = char_append(ret, '/');
          continue;
        }
        ret = char_append(ret, value[i]);
      }
      value = ret;
      #if WINDOWS_PLATFORM == true
      if (!str_has_suffix(value, ".ico"))value = str_append(value, ".ico");
      #elif LINUX_PLATFORM == true
      if(!str_has_suffix(value,".png"))value=str_append(value,".png");
      #endif
    }
  }
//  printf("CONFIG:%s(%s)=>%s\n", key, val_type, value);
  //=>edit main $con[key]
  return edit_magic_macro(CONFIG_MAGIC_MACRO_TYPE, key, value);
}
//***********************************
void print_magic_macros(uint8 wh) {
  printf("***MAGIC __CONFIG:***\n");
  mama *tmp1 = entry_table.mama_start;
  for (;;) {
    if ((wh == 0 || tmp1->type == wh)) {
      printf("[%i:%i]- %s:%s(%s)\n", tmp1->id, tmp1->type, tmp1->key, tmp1->value, tmp1->value_type);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  printf("---------------------\n");
}

//***********************************
void init_magic_define() {
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "ERROR", str_from_int32(ERROR_ID));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "WARNING", str_from_int32(WARNING_ID));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "str", "OSType", OS_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "str", "OSArch", OS_ARCH);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "str", "PathSeparator", char_to_str(OS_SEPARATOR));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "EOF", str_from_int32(EOF));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "str", "EOL", new_line_char);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "IntSize", str_from_int32(MAX_INT_LEN));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "FloatSize", str_from_int32(MAX_FLOAT_LEN));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE,
              "str",
              "MplVersion",
              str_multi_append(VERSION_NAME, "-", VERSION, "-", str_from_long_int(VERSION_NUMBER), 0));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "str", "AppPath", str_from_path(project_root));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "str", "MplPath", str_from_path(interpreter_path));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "LeftDirect", LEFT_DIRECT);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "RightDirect", RIGHT_DIRECT);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "BothDirect", BOTH_DIRECT);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "ReadChar", READ_CHAR_INPUT_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "ReadLine", READ_LINE_INPUT_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "PID", str_from_long_int(__syscall_pid()));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "num", "StartedTime", AppStartedTime);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "str", "HostName", __syscall_hostname());
}

//***********************************
void init_magic_config() {
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "ErrorsState", "error");//[OK] fatal
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "WarningsState", "warning");//[OK] error,fatal
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "num", "MaxHugeDivideSteps", str_from_int32(max_steps_estimate_huge)); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "num", "MaxHugeDecimalNumbers", str_from_int32(max_decimal_has_huge)); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "SessionMode", "true"); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "HelpArgumentMode", "false"); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "OverwriteBuiltinMode", "false"); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", DEBUG_MODE, "false"); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "BuildMode", "false"); //[OK]
//  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "PackageMode", "false");
//  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "AccessVariablesMode", "false");
//  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "NameSpace", 0);
//  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "ExportByteCode", 0); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "ExportBuildFile", 0); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "ExportLogFile", 0); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "SessionDatabasePath", 0); //[OK]

  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "AppVersion", "1.0.0.0"); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "AppName", main_source_name); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "AppLicense", LICENCE); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "AppCreator", "MPL-Interpreter"); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "AppIcon", 0); //[OK]
  //  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "SafeMode", "false");
  //  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "OptimizeMode", "false");
}
//***********************************
/**
 * store all session entries in session database in disk
 * @return Boolean
 */
Boolean flush_session_entries() {
  //init vars
  String filename;
  String result = 0;
  utst *sesutf8_start = 0;
  utst *sesutf8_end = 0;
  Boolean is_exist = false;
  //--------------------------
  /**
   * SESSION DATABASE STRUCTURE(V1):
    SESSION_HEADER_FLAG
    VERSION_NUMBER
    SESSION_HEADER_START
    #key,type,value_len
    value(encoded)
    SESSION_HEADER_END
    //if exist utf8 strings,then continue
    SESSION_HEADER_START
    id;max_bytes;utf8_str

   */
  //=>determine database path
  if (sessiondb_path == 0)
    filename = str_multi_append(project_root, char_to_str(OS_SEPARATOR), SESSION_DEFAULT_FILENAME, 0, 0, 0);
  else filename = convert_mplpath_to_abspath(sessiondb_path);
  //=>append header to result
  result = str_multi_append(SESSION_HEADER_FLAG, "\n", str_from_int32(VERSION_NUMBER), "\n", SESSION_HEADER_START, 0);
  //=>write all session entries to result
  mama *tmp1 = entry_table.mama_start;
  for (;;) {
    if (tmp1->type == SESSION_MAGIC_MACRO_TYPE) {
      is_exist = true;
      result = str_multi_append(result, "\n#", tmp1->key, ",", tmp1->value_type, ",");
      result = str_multi_append(result,
                                str_from_int32(str_length(tmp1->value)),
                                "\n", MPLV1_encode(tmp1->value),
                                0,
                                0);
      //=>if value is utf8 string
      if (str_indexof(tmp1->value, UTF8_ID_LABEL, 0) == 0) {
        long_int uid = str_to_long_int(str_substring(tmp1->value, UTF8_ID_LBL_LEN, 0));
        utst *q = (utst *) malloc(sizeof(utst));
        if (q == 0) return false;
        (*q) = get_utst(uid);
        q->next = 0;
        if (sesutf8_start == 0) { sesutf8_start = sesutf8_end = q; }
        else {
          sesutf8_end->next = q;
          sesutf8_end = q;
        }
      }
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  //=>if not exist any session entries
  if (!is_exist)return true;
  //=>append end of main section
  result = char_append(result, '\n');
  result = str_append(result, SESSION_HEADER_END);
  //=>append utf8 entries
  if (sesutf8_start == 0)return true;
  result = str_multi_append(result, "\n", SESSION_HEADER_START, "\n", 0, 0);
  utst *tmp2 = sesutf8_start;
  for (;;) {
//      printf("DDDS:%i,%s\n", tmp2->id, utf8_to_unicode_str(tmp2->utf8_string));
    result = str_multi_append(result,
                              str_from_long_int(tmp2->id),
                              ";",
                              str_from_int32(tmp2->max_bytes_per_char),
                              ";",
                              utf8_to_bytes_string(tmp2->utf8_string));
    result = char_append(result, '\n');
    tmp2 = tmp2->next;
    if (tmp2 == 0) break;
  }
  //=>encode all result
  for (uint32 i = 0; i < str_length(result); i++) {
    result[i] += 1;
    result[i] = ~result[i];
  }
  //=>flush to db file
  __syscall_write_file(filename, result);
  //=>return true
  return true;
}
//***********************************
/**
 * load all session entries from session database to mama and return a number that shows state
 * @return int8
 */
int8 load_session_entries() {
  //init vars
  String filename;
  String output = 0;
  uint32 out_len = 0;
  FILE *fp;
  String buf = 0;
  uint8 section_counter = 0;
  uint32 header_version_number = 0;
  uint32 keys_start_ind = 0, keys_end_ind = 0, session_keys = 0, utf8_session_keys_len = 0,
      tmp_utf8_session_keys_len = 0;
  Boolean is_utf8_header = false;
  str_list utf8_session_keys = 0;
  //--------------------------
  //=>determine database path
  if (sessiondb_path == 0)
    filename = str_multi_append(project_root, char_to_str(OS_SEPARATOR), SESSION_DEFAULT_FILENAME, 0, 0, 0);
  else filename = convert_mplpath_to_abspath(sessiondb_path);
  //=>open database if exist as a binary file
  fp = fopen(filename, "r");
  if (fp == NULL) return false;
  //=>get every bytes of database file and decode it and then store to output
  for (;;) {
    int32 c = fgetc(fp);
    if (c == EOF)break;
    c = ~c;
    c -= 1;
    if (c == 0)break;
    out_len++;
    output = char_append(output, c);
//    printf("WW(%i)>>%s\n", c, output);
  }
  fclose(fp);
//  printf("output(%i):%s\n", out_len, output);
  //=>check header for any error and get version_number
  for (uint32 i = 0; i < out_len; i++) {
    if (output[i] == '\n') {
      section_counter++;
      if (section_counter == 1 && !str_equal(buf, SESSION_HEADER_FLAG))return -1;
      else if (section_counter == 2 && !str_is_int32(buf))return -1;
      else if (section_counter == 2)header_version_number = str_to_long_int(buf);
      else if (section_counter == 3 && (!str_equal(buf, SESSION_HEADER_START) || output[i + 1] != '#')) return -1;
      else if (section_counter == 3) {
        keys_start_ind = i + 1;
        break;
      }
      buf = 0;
      continue;
    }
    buf = char_append(buf, output[i]);
  }
//  printf("Session{vernum:%i}\n", header_version_number);
  //=>get all keys and values from output
  buf = 0;
  for (uint32 i = keys_start_ind; i < out_len; i++) {
    if (output[i] == '\n') {
      if (session_keys > 0 && str_equal(buf, SESSION_HEADER_END)) {
        keys_end_ind = i + 1;
        break;
      }
      if (buf[0] != '#')return -2;
      str_list ret = 0;
      session_keys++;
      uint32 retlen = char_split(buf, ',', &ret, true);
      if (retlen != 3) return -2;
      ret[0] = str_substring(ret[0], 1, 0);
      uint32 vallen = (uint32) str_to_long_int(ret[2]);
      String value = str_substring(output, i + 1, i + 1 + vallen);
      value = MPLV1_decode(value);
      //=>add to $ses magic macro
      if (str_indexof(value, UTF8_ID_LABEL, 0) == 0)
        str_list_append(&utf8_session_keys, ret[0], utf8_session_keys_len++);
      add_to_mama(SESSION_MAGIC_MACRO_TYPE, ret[1], ret[0], value);
//      printf("@EE:%s(%s)[%i]:%s$(%i)\n", ret[0], ret[1], vallen, value, utf8_session_keys_len);
      i += 1 + vallen;
      buf = 0;
      continue;
    }
    buf = char_append(buf, output[i]);
  }
  //=>if exist utf8 strings, then get all
  if (keys_end_ind == 0 || keys_end_ind + 4 >= out_len)return true;
  buf = 0;
  for (uint32 i = keys_end_ind; i < out_len; i++) {
    if (output[i] == '\n') {
//      printf("DDDDD:%s[%c],%i\n",buf,output[keys_end_ind],utf8_session_keys_len);
      if (!is_utf8_header && str_equal(buf, SESSION_HEADER_START))is_utf8_header = true;
      else if (!is_utf8_header)return -3;
      else if (buf == 0) break;
      else {
        str_list ret = 0;
        uint32 retlen = char_split(buf, ';', &ret, true);
        if (retlen != 3 || tmp_utf8_session_keys_len > utf8_session_keys_len) return -3;
        long_int utf8_id = add_to_utst(0, utf8_encode_bytes(ret[2]), str_to_int32(ret[1]));
        String utf8_str = str_append(UTF8_ID_LABEL, str_from_long_int(utf8_id));
        edit_magic_macro(SESSION_MAGIC_MACRO_TYPE, utf8_session_keys[tmp_utf8_session_keys_len++], utf8_str);
//        printf("@UTF:%s(%s)=>%s:%s\n", ret[0], ret[1], utf8_session_keys[utf8_session_keys_len + 1], ret[2]);
      }
      buf = 0;
      continue;
    }
    buf = char_append(buf, output[i]);
  }
  return true;
}
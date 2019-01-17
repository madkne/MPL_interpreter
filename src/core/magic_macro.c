//
// Created by amindelavar on 12/2/2018.
//
#include<MPL/system.h>

Boolean edit_magic_macro(uint8 type, String key, String value) {
  mama *tmp1 = entry_table.mama_start;
  if (tmp1 == 0) return false;
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
  //=>remove quotations if value is string
  value = str_reomve_quotations(value, val_type);
  //=>ErrorsState or WarningsState
  Boolean is_err = false, not_exist = false;
  if ((is_err = str_equal(key, "ErrorsState")) || str_equal(key, "WarningsState")) {
    if (!str_equal(val_type, "str")) {
      //TODO:error
      return false;
    }
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
  //=>MaxHugeDivideSteps or MaxHugeDecimalNumbers or TabSize
  Boolean divide = false, decimal = false, tabsize = false;
  int32 value_int = 0;
  if ((divide = str_equal(key, "MaxHugeDivideSteps")) || (decimal = str_equal(key, "MaxHugeDecimalNumbers"))
      || (tabsize = str_equal(key, "TabSize"))) {
    if (!str_equal(val_type, "num") || determine_type_num(value) != 'i' || !str_is_int32(value)
        || (value_int = str_to_int32(value)) < 1) {
      //TODO:error
      return false;
    }
    //=>MaxHugeDivideSteps change
    if (divide) max_steps_estimate_huge = (uint8) value_int;
      //=>MaxHugeDecimalNumbers change
    else if (decimal) max_decimal_has_huge = (uint32) value_int;
      //=>TabSize change
    else if (tabsize) {
      tab_size_int = (uint8) value_int;
      tab_size = 0;
      for (uint8 i = 0; i < tab_size_int; i++) tab_size = char_append(tab_size, ' ');
      if (tab_size_int == 6)using_custom_tab = false;
      else using_custom_tab = true;
    }
  }

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
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, "str", "AppPath", project_root);
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
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "num", "TabSize", str_from_int32(tab_size_int)); //[OK]
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "SessionMode", "true");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "HelpArgumentMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "OverwriteBuiltinMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", DEBUG_MODE, "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "PackageMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "AccessVariablesMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "RunOnlyOS", 0);//windows,linux
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "RunOnlyArch", 0);//x86,x64
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "NameSpace", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "ExportByteCode", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "ExportLogFile", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "SessionDatabasePath", 0);

  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "AppVersion", "1.0.0");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "AppName", main_source_name);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "AppLicense", LICENCE);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "str", "AppCreator", "MPL-Interpreter");
  //  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "SafeMode", "false");
  //  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, "bool", "OptimizeMode", "false");
}
//***********************************
/**
 * store all session entries in session database in disk
 * @return Boolean
 */
Boolean flush_session_entries(){
return true;
}
//***********************************
/**
 * load all session entries from session database to mama
 * @return Boolean
 */
Boolean load_session_entries(){
  return true;
}
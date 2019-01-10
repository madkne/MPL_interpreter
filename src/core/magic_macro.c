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
void print_magic_macros(uint8 wh) {
  printf("***MAGIC __CONFIG:***\n");
  mama *tmp1 = entry_table.mama_start;
  for (;;) {
    if ((wh == 0 || tmp1->type == wh)) {
      printf("[%i:%i]- %s:%s(%c)\n", tmp1->id, tmp1->type, tmp1->key, tmp1->value, tmp1->sub_type);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  printf("---------------------\n");
}

//***********************************
void init_magic_define() {
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ERROR", str_from_int32(ERROR_ID));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "WARNING", str_from_int32(WARNING_ID));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "OSType", OS_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "OSArch", OS_ARCH);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "PathSeparator", char_to_str(OS_SEPARATOR));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "EOF", str_from_int32(EOF));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "EOL", new_line_char);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "IntSize", str_from_int32(MAX_INT_LEN));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "FloatSize", str_from_int32(MAX_FLOAT_LEN));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "MplVersion", str_multi_append(VERSION_NAME, "-", VERSION, 0, 0, 0));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "AppPath", project_root);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "LeftDirect", LEFT_DIRECT);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "RightDirect", RIGHT_DIRECT);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "BothDirect", BOTH_DIRECT);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ReadChar", READ_CHAR_INPUT_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ReadLine", READ_LINE_INPUT_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "PID", str_from_long_int(__syscall_pid()));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "StartedTime", AppStartedTime);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "HostName", __syscall_hostname());
}

//***********************************
void init_magic_config() {
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "ErrorsState", str_from_int32(ERROR_ID)); //fatals
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "WarningsState", str_from_int32(WARNING_ID));//errors,fatals
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "ExportLogFile", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "MaxHugeDivideSteps", str_from_int32(max_steps_estimate_huge));
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "MaxHugeDecimalNumbers", str_from_int32(max_decimal_has_huge));
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "TabSize", str_from_int32(tab_size_int));
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppVersion", "1.0.0");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppName", main_source_name);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppLicense", LICENCE);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppCreator", "MPL-Interpreter");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "SafeMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "SessionMode", "true");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "HelpArgumentMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "OptimizeMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "OverwriteBuiltinMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', DEBUG_MODE, "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "ExportByteCode", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "RunOnlyOS", 0);//windows,linux
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "RunOnlyArch", 0);//x86,x64
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "NameSpace", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "SessionDatabasePath", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "PackageMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "AccessVariablesMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "NameSpace", 0);
}
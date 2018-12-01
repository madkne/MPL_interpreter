//
// Created by amindelavar on 9/19/2018.
//
#include <MPL/system.h>

//***********************************
uint32 call_built_in_funcs(String func_name, str_list params, str_list partypes, uint32 params_len, str_list *returns) {
  //-----------------init vars
  uint32 func_id = 0, returns_len = 0;
  uint8 func_type = 0;
  str_list argvs = 0;
  long_int var0_ind = 0;
  //-----------------find function
  bifs *st = entry_table.bifs_start;
  for (;;) {
    //if func_name is true
    if (str_equal(st->func_name, func_name)) {
      //convert func_params to std
      str_list func_params = 0;
      uint32 func_params_len = char_split(st->params, '|', &func_params, true);
      for (uint32 i = 0; i < func_params_len; i++) {
        str_list seg = 0;
        uint32 seg_len = char_split(func_params[i], ';', &seg, true);
        if (seg_len > 1)func_params[i] = str_multi_append(seg[0], ";var;", seg[1], 0, 0, 0);
        else func_params[i] = str_multi_append(seg[0], ";var;0", 0, 0, 0, 0);
      }
//		  printf("#####@@:%s,%s,[%i,%i]\n",print_str_list (func_params, func_params_len),print_str_list ( partypes, params_len),func_params_len,params_len);
      if (is_exact_function(func_params, func_params_len, partypes, params_len, true)) {
        func_id = st->id;
        func_type = st->type;
        //printf("-----------@@@\n");
        break;
      }
    }
    st = st->next;
    if (st == 0) break;
  }

  //-----------------call function
//  printf ("WWW@@WWW:%s,%s,%s[%i,%i]\n", func_name, print_str_list (params, params_len), print_str_list (partypes, params_len), func_id, func_type);
  if (func_id == 0 || func_type == 0)return 0;
  //convert vars to values
  for (uint32 i = 0; i < params_len; i++) {
    str_list p1 = 0;
    char_split(partypes[i], ';', &p1, false);
    //is var
    if (str_ch_equal(p1[2], '1') || str_ch_equal(p1[2], '2')) {
//      printf("###argv%i:(VAR)%s\n", i, partypes[i]);
      String var_val = return_value_var_complete(str_to_long_int(p1[1]));
      if (p1[3] == 0 || str_ch_equal(p1[3], '0') || str_ch_equal(p1[3], '1') && var_val != 0 && var_val[0] != '{')
        var_val = str_reomve_quotations(var_val, p1[0]);
      str_list_append(&argvs, var_val, i);
      //if param[0] is var, store its index
      if (i == 0) var0_ind = str_to_long_int(p1[1]);
    }
      //is value
    else {
      String ret1 = 0;
      uint8 ret2 = 0;
      calculate_value_of_var(params[i], p1[0], &ret1, &ret2);
      ret1 = str_reomve_quotations(ret1, p1[0]);
//        printf("###argv%i:%s(%s)=>%s\n",i,params[i],p1[0],ret1);
      str_list_append(&argvs, ret1, i);
    }
  }
//print_struct(PRINT_STRUCT_DES_ST);
//  printf ("@BUILT-IN:%s@%i@;%s\n", print_str_list (params, params_len),params_len, print_str_list (argvs, params_len));
  //---------------------MPL Function Calls----------------------
  if (func_type == MPL_BUILT_IN_TYPE) {
    if (func_id == 1/*len*/) {}
    else if (func_id == 2/*print*/) {
      Boolean ret = _MPL_TYPE__print(argvs, params_len);
      str_list_append(&(*returns), str_from_bool(ret), returns_len++);
    } else if (func_id == 3/*typeof*/) {
      str_list p = 0;
      char_split(partypes[0], ';', &p, false);
      String ret = _MPL_TYPE__typeof(argvs[0], p[0]);
      str_list_append(&(*returns), convert_to_string(ret), returns_len++);
      // printf ("Result of \'typeof\':%s[%s]=>%s\n", argvs[0], p[0], ret);
    } else if (func_id == 4/*input*/) {
      String ret = _MPL_TYPE__input(argvs[0]);
      str_list_append(&(*returns), convert_to_string(ret), returns_len++);
      //printf ("Result of \'input\':%s=>%s\n", argvs[0], ret);
    } else if (func_id == 15/*push*/) {
      str_list p = 0;
      char_split(partypes[1], ';', &p, false);
      uint32 ret = _MPL_TYPE__push(var0_ind, argvs[1], p[0], argvs[2]);
      str_list_append(&(*returns), str_from_long_int((long_int) ret), returns_len++);
      //printf ("Result of \'input\':%s=>%s\n", argvs[0], ret);
    } else if (func_id == 21/*var_type*/) {
      str_list p = 0;
      char_split(partypes[0], ';', &p, false);
      String ret = p[0];
      str_list_append(&(*returns), convert_to_string(ret), returns_len++);
      // printf ("Result of \'var_type\':%s[%s]=>%s\n", argvs[0], p[0], ret);
    }
    //TODO:complete
  }
    //---------------------DATA Function Calls---------------------
  else if (func_type == DATA_BUILT_IN_TYPE) {
    //TODO:complete
  }
    //---------------------OS Function Calls-----------------------
  else if (func_type == OS_BUILT_IN_TYPE) {
    if (func_id == 1/*exit*/) {
      _OS_TYPE__exit(str_to_int32(argvs[0]));
    }
  }
  //-----------------return

  return returns_len;
}
//***********************************

//***********************************
void init_built_in_funcs() {
  add_to_bifs(0, 0, 0, 0, 0);
  /**
   * a=bool|str|num|struct|var[0] : value
   * aa=bool[?,..]|str[?,..]|num[?,..]|struct[?,..] : var
   * aa..=bool[?,..]|str[?,..]|num[?,..]|struct[?,..] : var,var,..
   */
  //----------------------------------mpl built_in
  add_to_bifs(1, MPL_BUILT_IN_TYPE, "len", "aa", "num;?");
  add_to_bifs(2, MPL_BUILT_IN_TYPE, "print", "aa..", "bool"); //=>[OK]
  add_to_bifs(3, MPL_BUILT_IN_TYPE, "typeof", "a", "str"); //=>[OK]
  add_to_bifs(4, MPL_BUILT_IN_TYPE, "input", "num", "str"); //=>[OK]
  add_to_bifs(5, MPL_BUILT_IN_TYPE, "error_handle", "num|str|str", "bool");
  add_to_bifs(6, MPL_BUILT_IN_TYPE, "config_all", 0, "str;?,2");
  add_to_bifs(7, MPL_BUILT_IN_TYPE, "define_all", 0, "str;?,2");
  add_to_bifs(8, MPL_BUILT_IN_TYPE, "define_isset", "str", "bool");
  add_to_bifs(9, MPL_BUILT_IN_TYPE, "embed_run", "str;?", "num");
  add_to_bifs(10, MPL_BUILT_IN_TYPE, "argvs", 0, "str;?");
  add_to_bifs(11, MPL_BUILT_IN_TYPE, "session_all", 0, "str;?,2");
  add_to_bifs(12, MPL_BUILT_IN_TYPE, "session_isset", "str", "bool");
  add_to_bifs(13, MPL_BUILT_IN_TYPE, "crop", "aa|num|num", "aa");
  add_to_bifs(14, MPL_BUILT_IN_TYPE, "search", "aa|a|num", "num");
  add_to_bifs(15, MPL_BUILT_IN_TYPE, "push", "aa|a|num", "num");
  add_to_bifs(16, MPL_BUILT_IN_TYPE, "pop", "aa|num", "num");
  add_to_bifs(17, MPL_BUILT_IN_TYPE, "del", "aa", "bool");
  add_to_bifs(18, MPL_BUILT_IN_TYPE, "mpl_execute", "str", "str");
  add_to_bifs(19, MPL_BUILT_IN_TYPE, "trace_var", "aa", "bool");
  add_to_bifs(20, MPL_BUILT_IN_TYPE, "trace_func", "aa", "bool");
  add_to_bifs(21, MPL_BUILT_IN_TYPE, "var_type", "aa", "str"); //=>[OK]
  //----------------------------------data types built_in
  add_to_bifs(1, DATA_BUILT_IN_TYPE, "str_split", "str|str", "str;?");
  add_to_bifs(2, DATA_BUILT_IN_TYPE, "str_replace", "str|str|str", "bool");
  add_to_bifs(3, DATA_BUILT_IN_TYPE, "to_num", "a|bool", "num");
  add_to_bifs(4, DATA_BUILT_IN_TYPE, "to_bool", "a", "bool");
  add_to_bifs(5, DATA_BUILT_IN_TYPE, "to_str", "a", "str");
  add_to_bifs(6, DATA_BUILT_IN_TYPE, "at", "a|num", "a");
  add_to_bifs(7, DATA_BUILT_IN_TYPE, "str_at", "str|num|str", "bool");
  add_to_bifs(8, DATA_BUILT_IN_TYPE, "str_crop", "str|num|num", "str");
  add_to_bifs(9, DATA_BUILT_IN_TYPE, "str_indexof", "str|str|num", "str");
  add_to_bifs(10, DATA_BUILT_IN_TYPE, "str_uppercase", "str", "str");
  add_to_bifs(11, DATA_BUILT_IN_TYPE, "str_lowercase", "str", "str");
  add_to_bifs(12, DATA_BUILT_IN_TYPE, "str_trim", "str|num", "str");
  add_to_bifs(13, DATA_BUILT_IN_TYPE, "is_num", "a", "bool");
  add_to_bifs(14, DATA_BUILT_IN_TYPE, "is_bool", "a", "bool");
  add_to_bifs(15, DATA_BUILT_IN_TYPE, "is_str", "a", "bool");
  add_to_bifs(16, DATA_BUILT_IN_TYPE, "str_contains", "str|str", "bool");
  add_to_bifs(17, DATA_BUILT_IN_TYPE, "str_join", "str|str", "str");
  add_to_bifs(18, DATA_BUILT_IN_TYPE, "bool_switch", "bool", "bool");
  add_to_bifs(19, DATA_BUILT_IN_TYPE, "str_reverse", "str", "str");
  add_to_bifs(20, DATA_BUILT_IN_TYPE, "to_chars", "a", "str;?");
  add_to_bifs(21, DATA_BUILT_IN_TYPE, "base_convert", "num|num|bool", "str");
  //----------------------------------os built_in
  add_to_bifs(1, OS_BUILT_IN_TYPE, "exit", "num", "bool");
  add_to_bifs(2, OS_BUILT_IN_TYPE, "mkdir", "str", "bool");
  add_to_bifs(3, OS_BUILT_IN_TYPE, "sell", "str", "str");
  add_to_bifs(4, OS_BUILT_IN_TYPE, "time", 0, "num");
  add_to_bifs(5, OS_BUILT_IN_TYPE, "fopen", "str|str", "num");
  add_to_bifs(6, OS_BUILT_IN_TYPE, "fclose", "num", "bool");
  add_to_bifs(7, OS_BUILT_IN_TYPE, "fwrite", "num|str", "bool");
  add_to_bifs(8, OS_BUILT_IN_TYPE, "fread", "num|str|num", "str");
  add_to_bifs(9, OS_BUILT_IN_TYPE, "finfo", "str", "str;?");
  add_to_bifs(10, OS_BUILT_IN_TYPE, "proc_start", "str|str;?", "bool");
  add_to_bifs(11, OS_BUILT_IN_TYPE, "abspath", "str", "str");
  add_to_bifs(12, OS_BUILT_IN_TYPE, "copy", "str|str", "bool");
  add_to_bifs(13, OS_BUILT_IN_TYPE, "exist", "str", "bool");
  add_to_bifs(14, OS_BUILT_IN_TYPE, "remove", "str", "bool");
  add_to_bifs(15, OS_BUILT_IN_TYPE, "scandir", "str|num", "str;?");
  add_to_bifs(16, OS_BUILT_IN_TYPE, "runtime_info", "num", "str;?");
  add_to_bifs(17, OS_BUILT_IN_TYPE, "shutdown", "num", "bool");
  add_to_bifs(18, OS_BUILT_IN_TYPE, "rand", "num|num", "num");
}

//***********************************
void init_built_in_defines() {
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ERROR", str_from_int32(ERROR_ID));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "WARNING", str_from_int32(WARNING_ID));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "FATAL", str_from_int32(FATAL_ID));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "OSType", OS_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "OSArch", OS_ARCH);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "PathSeparator", char_to_str(OS_SEPARATOR));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "EOF", str_from_int32(EOF));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "IntSize", str_from_int32(MAX_INT_LEN));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "FloatSize", str_from_int32(MAX_FLOAT_LEN));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "MplVersion", str_multi_append(VERSION_NAME, "-", VERSION, 0, 0, 0));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "AppPath", project_root);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "LeftDirect", LEFT_DIRECT);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "RightDirect", RIGHT_DIRECT);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "BothDirect", BOTH_DIRECT);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ReadChar", READ_CHAR_INPUT_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ReadLine", READ_LINE_INPUT_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "ReadAll", READ_ALL_INPUT_TYPE);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "CpuInfo", "1");
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "MemoryInfo", "2");
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "OSInfo", "3");
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "PID", str_from_long_int(get_pid()));
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 'i', "StartedTime", AppStartedTime);
  add_to_mama(DEFINE_MAGIC_MACRO_TYPE, 's', "HostName", get_host_name());
}

//***********************************
void init_default_configs() {
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "ErrorsMode", str_from_int32(ERROR_ID)); //fatals
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "WarningsMode", str_from_int32(WARNING_ID));//errors,fatals
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "SetLogfile", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "DebugMode", "0");//0..5
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "MaxHugeDivideSteps", str_from_int32(max_steps_estimate_huge));
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'i', "TabSize", str_from_int32(tab_size_int));
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "EnableSafeMode", "true");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppVersion", "1.0.0");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppName", main_source_name);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppLicense", "free");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "EnableSession", "true");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "SaveSessionOnExit", "true");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "AutoCreateDocs", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "EnableHelpArgument", "true");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "OptimizeMode", "false");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 'b', "EnableAboutArgument", "true");
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "SetByteCode", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "RunOnlyOS", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "RunOnlyArch", 0);
  add_to_mama(CONFIG_MAGIC_MACRO_TYPE, 's', "AppCreator", "MPL-Interpreter");
}
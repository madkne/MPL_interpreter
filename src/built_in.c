//
// Created by amindelavar on 9/19/2018.
//
#include <MPL/system.h>

//***********************************
Boolean check_built_in_module_function(String S_params, str_list partypes, uint32 params_len) {
  //convert func_params to std
  str_list func_params = 0;
  uint32 func_params_len = char_split(S_params, '|', &func_params, true);
  for (uint32 i = 0; i < func_params_len; i++) {
    str_list seg = 0;
    uint32 seg_len = char_split(func_params[i], ';', &seg, true);
    if (seg_len > 1)func_params[i] = str_multi_append(seg[0], ";var;", seg[1], 0, 0, 0);
    else func_params[i] = str_multi_append(seg[0], ";var;0", 0, 0, 0, 0);
  }
//		  printf("#####@@:%s,%s,[%i,%i]\n",print_str_list (func_params, func_params_len),print_str_list ( partypes, params_len),func_params_len,params_len);
  if (is_exact_function(func_params, func_params_len, partypes, params_len, true)) {
    //printf("-----------@@@\n");
    return true;
  }
  return false;
}
//***********************************
void convert_built_in_module_vars_to_values(str_list partypes,
                                            str_list params,
                                            uint32 params_len,
                                            str_list *argvs,
                                            long_int *var0_ind) {
  for (uint32 i = 0; i < params_len; i++) {
    str_list p1 = 0;
    char_split(partypes[i], ';', &p1, false);
    //is var
    if (str_ch_equal(p1[2], '1') || str_ch_equal(p1[2], '2')) {
      String var_val = 0;
//      printf("###argv%i:%s(VAR)%s\n", i, params[i], partypes[i]);
      //=>if var not has any index
      if (str_ch_equal(p1[3], '_')) var_val = return_value_var_complete(str_to_long_int(p1[1]));
        //=> else if var has index
      else {
        long_int po_id = get_Mvar(str_to_long_int(p1[1])).pointer_id;
        long_int data_id = get_data_memory_id(po_id, p1[3]);
        var_val = __return_value_var_complete(data_id);
      }
      if (str_ch_equal(p1[3], '_') || str_ch_equal(p1[3], '1') && var_val != 0 && var_val[0] != '{')
        var_val = str_reomve_quotations(var_val, p1[0]);
      //=>if string is utf8
      if (p1[0][0] == 's' && str_indexof(var_val, UTF8_ID_LABEL, 0) == 0) {
        var_val = utf8_to_bytes_string(get_utst_by_label(var_val).utf8_string);
      }
      str_list_append(&(*argvs), var_val, i);
      //if param[0] is var, store its index
      if (i == 0) (*var0_ind) = str_to_long_int(p1[1]);
    }
      //is value
    else {
      String ret1 = 0;
      ret1 = return_value_from_expression(params[i], p1[0]);
      //=>if string is utf8
      if (p1[0][0] == 's' && str_indexof(ret1, UTF8_ID_LABEL, 0) == 0) {
        ret1 = utf8_to_bytes_string(get_utst_by_label(ret1).utf8_string);
      } else ret1 = str_reomve_quotations(ret1, p1[0]);
//      printf("###argv%i:%s(%s)=>%s\n", i, params[i], p1[0], ret1);
      str_list_append(&(*argvs), ret1, i);
    }
  }
}
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
      Boolean is_exist = check_built_in_module_function(st->params, partypes, params_len);
      if (is_exist) {
        func_id = st->id;
        func_type = st->type;
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
  convert_built_in_module_vars_to_values(partypes, params, params_len, &argvs, &var0_ind);
//print_struct(PRINT_STRUCT_DES_ST);
//  printf("@BUILT-IN:%s@%i@;%s\n", print_str_list(params, params_len), params_len, print_str_list(argvs, params_len));
  //---------------------MPL Function Calls----------------------
  if (func_type == MPL_BUILT_IN_TYPE) {
    if (func_id == _MPL_LEN) {
      //TODO:
    } else if (func_id == _MPL_TYPEOF) {
      str_list p = 0;
      char_split(partypes[0], ';', &p, false);
      String ret = _MPL_TYPE__typeof(argvs[0], p[0]);
      str_list_append(&(*returns), convert_to_string(ret), returns_len++);
      // printf ("Result of \'typeof\':%s[%s]=>%s\n", argvs[0], p[0], ret);
    } else if (func_id == _MPL_PUSH) {
      str_list p = 0;
      char_split(partypes[1], ';', &p, false);
      uint32 ret = _MPL_TYPE__push(var0_ind, argvs[1], p[0], argvs[2]);
      str_list_append(&(*returns), str_from_long_int((long_int) ret), returns_len++);
      //printf ("Result of \'input\':%s=>%s\n", argvs[0], ret);
    } else if (func_id == _MPL_VAR_TYPE) {
      str_list p = 0;
      char_split(partypes[0], ';', &p, false);
      String ret = p[0];
      str_list_append(&(*returns), convert_to_string(ret), returns_len++);
      // printf ("Result of \'var_type\':%s[%s]=>%s\n", argvs[0], p[0], ret);
    } else if (func_id == _MPL_ERROR_HANDLE) {
      Boolean ret = _MPL_TYPE__error_handle(str_to_int32(argvs[0]),
                                            argvs[1], argvs[2]);
      str_list_append(&(*returns), convert_to_string(str_from_bool(ret)), returns_len++);
    }
    //TODO:complete
  }
    //---------------------DATA Function Calls---------------------
  else if (func_type == DATA_BUILT_IN_TYPE) {
    if (func_id == _DATA_TNUM) {
      //TODO
    } else if (func_id == _DATA_AND) {
      String ret = _DATA_TYPE__bit_operations('a', argvs[0], argvs[1]);
      str_list_append(&(*returns), ret, returns_len++);
    } else if (func_id == _DATA_OR) {
      String ret = _DATA_TYPE__bit_operations('o', argvs[0], argvs[1]);
      str_list_append(&(*returns), ret, returns_len++);
    } else if (func_id == _DATA_XOR) {
      String ret = _DATA_TYPE__bit_operations('x', argvs[0], argvs[1]);
      str_list_append(&(*returns), ret, returns_len++);
    } else if (func_id == _DATA_NOT) {
      String ret = _DATA_TYPE__bit_operations('n', argvs[0], 0);
      str_list_append(&(*returns), ret, returns_len++);
    } else if (func_id == _DATA_RSHIFT) {
      String ret = _DATA_TYPE__bit_operations('r', argvs[0], argvs[1]);
      str_list_append(&(*returns), ret, returns_len++);
    } else if (func_id == _DATA_LSHIFT) {
      String ret = _DATA_TYPE__bit_operations('l', argvs[0], argvs[1]);
      str_list_append(&(*returns), ret, returns_len++);
    }
    //TODO:complete
  }
    //---------------------OS Function Calls-----------------------
  else if (func_type == OS_BUILT_IN_TYPE) {
    if (func_id == _OS_EXIT) {
      _OS_TYPE__exit(str_to_int32(argvs[0]));
    } else if (func_id == _OS_PRINT) {
      Boolean ret = _OS_TYPE__print(argvs, params_len);
      str_list_append(&(*returns), str_from_bool(ret), returns_len++);
    } else if (func_id == _OS_INPUT) {
      String ret = _OS_TYPE__input(argvs[0]);
      str_list_append(&(*returns), convert_to_string(ret), returns_len++);
      //printf ("Result of \'input\':%s=>%s\n", argvs[0], ret);
    } else if (func_id == _OS_SHELL) {
      String ret = _OS_TYPE__shell(argvs[0]);
      str_list_append(&(*returns), convert_to_string(ret), returns_len++);
    } else if (func_id == _OS_TIME) {
      String ret = _OS_TYPE__time();
      str_list_append(&(*returns), ret, returns_len++);
    }else if (func_id == _OS_RAND) {
      String ret = _OS_TYPE__rand(argvs[0],argvs[1]);
//      printf("RAND:%s,%s=>%s\n",argvs[0],argvs[1],ret);
      str_list_append(&(*returns), ret, returns_len++);
    }else if (func_id == _OS_ARGVS) {
      String ret=_OS_TYPE__argvs();
//      printf("ARGVS:%s\n",ret);
      str_list_append(&(*returns), ret, returns_len++);
    }
    //TODO:complete
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
  add_to_bifs(_MPL_LEN, MPL_BUILT_IN_TYPE, "len", "aa", "num;?");
  add_to_bifs(_MPL_VAR_TYPE, MPL_BUILT_IN_TYPE, "type", "aa", "str"); //=>[OK]
  add_to_bifs(_MPL_TYPEOF, MPL_BUILT_IN_TYPE, "typeof", "a", "str"); //=>[OK]
  add_to_bifs(_MPL_ERROR_HANDLE, MPL_BUILT_IN_TYPE, "ErrHandle", "num|str|str", "bool"); //=>[OK]
  add_to_bifs(_MPL_CONFIG_ALL, MPL_BUILT_IN_TYPE, "ConAll", 0, "str;?,2");
  add_to_bifs(_MPL_DEFINE_ALL, MPL_BUILT_IN_TYPE, "DefAll", 0, "str;?,2");
  add_to_bifs(_MPL_DEFINE_ISSET, MPL_BUILT_IN_TYPE, "DefIsset", "str", "bool");
  add_to_bifs(_MPL_EMBED_RUN, MPL_BUILT_IN_TYPE, "EmbedRun", "str;?", "num");
  add_to_bifs(_MPL_SESSION_ALL, MPL_BUILT_IN_TYPE, "SesAll", 0, "str;?,2");
  add_to_bifs(_MPL_SESSION_ISSET, MPL_BUILT_IN_TYPE, "SesIsset", "str", "bool");
  add_to_bifs(_MPL_CROP, MPL_BUILT_IN_TYPE, "crop", "aa|num|num", "aa");
  add_to_bifs(_MPL_PUSH, MPL_BUILT_IN_TYPE, "push", "aa|a|num", "num");//=>[OK]
  add_to_bifs(_MPL_POP, MPL_BUILT_IN_TYPE, "pop", "aa|num", "num");
  add_to_bifs(_MPL_DEL, MPL_BUILT_IN_TYPE, "del", "aa", "bool");
  add_to_bifs(_MPL_MPL_EXECUTE, MPL_BUILT_IN_TYPE, "exec", "str", "str");
  add_to_bifs(_MPL_ECHO, MPL_BUILT_IN_TYPE, "echo", "aa..", "bool");
  add_to_bifs(_MPL_DEFINE_UNSET, MPL_BUILT_IN_TYPE, "DefUnset", "str", "bool");
  add_to_bifs(_MPL_SESSION_UNSET, MPL_BUILT_IN_TYPE, "SesUnset", "str", "bool");
  //----------------------------------data types built_in
  add_to_bifs(_DATA_TNUM, DATA_BUILT_IN_TYPE, "tnum", "a|bool", "num");
  add_to_bifs(_DATA_TBOOL, DATA_BUILT_IN_TYPE, "tbool", "a", "bool");
  add_to_bifs(_DATA_TSTR, DATA_BUILT_IN_TYPE, "tstr", "a", "str");
  add_to_bifs(_DATA_TCHARS, DATA_BUILT_IN_TYPE, "tchars", "a", "str;?");
  add_to_bifs(_DATA_AT, DATA_BUILT_IN_TYPE, "at", "a|num", "a");
  add_to_bifs(_DATA_INTO, DATA_BUILT_IN_TYPE, "into", "str|num|str", "bool");
  add_to_bifs(_DATA_INUM, DATA_BUILT_IN_TYPE, "inum", "a", "bool");
  add_to_bifs(_DATA_IBOOL, DATA_BUILT_IN_TYPE, "ibool", "a", "bool");
  add_to_bifs(_DATA_ISTR, DATA_BUILT_IN_TYPE, "istr", "a", "bool");
  add_to_bifs(_DATA_XOR, DATA_BUILT_IN_TYPE, "xor", "num|num", "num");//=>[OK]
  add_to_bifs(_DATA_AND, DATA_BUILT_IN_TYPE, "and", "num|num", "num");//=>[OK]
  add_to_bifs(_DATA_OR, DATA_BUILT_IN_TYPE, "or", "num|num", "num");//=>[OK]
  add_to_bifs(_DATA_NOT, DATA_BUILT_IN_TYPE, "not", "num", "num");//=>[OK]
  add_to_bifs(_DATA_RSHIFT, DATA_BUILT_IN_TYPE, "rshift", "num|num", "num");//=>[OK]
  add_to_bifs(_DATA_LSHIFT, DATA_BUILT_IN_TYPE, "lshift", "num|num", "num");//=>[OK]
  //----------------------------------os built_in
  add_to_bifs(_OS_EXIT, OS_BUILT_IN_TYPE, "exit", "num", "bool"); //=>[OK]
  add_to_bifs(_OS_PRINT, OS_BUILT_IN_TYPE, "print", "aa..", "bool"); //=>[OK]
  add_to_bifs(_OS_PRINTF, OS_BUILT_IN_TYPE, "printf", "str|aa..", "bool");
  add_to_bifs(_OS_INPUT, OS_BUILT_IN_TYPE, "input", "num", "str"); //=>[OK]
  add_to_bifs(_OS_SHELL, OS_BUILT_IN_TYPE, "shell", "str", "str"); //=>[OK]
  add_to_bifs(_OS_TIME, OS_BUILT_IN_TYPE, "time", 0, "num"); //=>[OK]
  add_to_bifs(_OS_RAND, OS_BUILT_IN_TYPE, "rand", "num|num", "num"); //[OK]
  add_to_bifs(_OS_ARGVS, OS_BUILT_IN_TYPE, "argvs", 0, "str;?");
}


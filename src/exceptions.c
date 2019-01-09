//
// Created by amindelavar on 9/19/2018.
//
#include<MPL/system.h>

void init_exceptions_list_data() {
  entry_table.exceptions_count = 0;
  entry_table.exli_start = 0;
  //-----------------------------------------
  define_new_exception(0, FATAL_ID, 0, 0, 0);
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
  //DebuggerError
  define_new_exception(1, ERROR_ID, "not_enough_params", DebuggerError, "your input has not enough parameters");
  define_new_exception(2, ERROR_ID, "not_exist_source", DebuggerError, "'!1@1!' source file is not a part of this program");
  define_new_exception(3, ERROR_ID, "invalid_line_number", DebuggerError, "'!1@1!' is invalid as line number of a source file");
  define_new_exception(4, ERROR_ID, "not_execute_line_code", DebuggerError, "in line number'!1@1!' in '!2@2!' file, not exist any executable instruction");
  define_new_exception(5, ERROR_ID, "unknown_command", DebuggerError, "'!1@1!' is unknown command for mdebug");
  define_new_exception(6, ERROR_ID, "not_find_breakpoint", DebuggerError, "not find any breakpoints in line'!1@1!' of '!2@2!' file");
  define_new_exception(7, ERROR_ID, "undefined_var", DebuggerError, "'!1@1!' as a variable in current line is undefined");
  define_new_exception(8, ERROR_ID, "can_not_run", DebuggerError, "line '!1@1!' completed and could not run again!");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
  //CommandError
  define_new_exception(1, FATAL_ID, "bad_exit", CommandError, "returned 1 exit status");
  define_new_exception(2, ERROR_ID, "unknown_opt", CommandError, "unknown option '!1@1!'");
  define_new_exception(3, FATAL_ID, "not_exist_file", CommandError, "can't open file '!1@1!',No such file.");
  define_new_exception(4, ERROR_ID, "not_exist_docs_file", CommandError, "can not open docs file for '!1@1!'");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//ImportError
  define_new_exception(1, ERROR_ID, "bad_place_using_utf8", ImportError,
                       "using UTF-8 in bad place or bad character in `!1@1!`");
  define_new_exception(2, ERROR_ID, "not_open_file", ImportError, "can't open file '!1@1!'");
  define_new_exception(3, ERROR_ID, "file_not_support_utf8", ImportError, "'!1@1!' file not support UTF-8");
  define_new_exception(4, ERROR_ID, "import_syntax_error", ImportError,
                       "A syntax error occurs for '!1@1!' import instruction");
  define_new_exception(5, ERROR_ID, "import_not_support_protocol", ImportError,
                       "not support the protocol in this path '!1@1!' import instruction");
  define_new_exception(6, ERROR_ID, "can_not_load_module", ImportError, "can not load a module in '!1@1!' path");
  define_new_exception(7, ERROR_ID, "not_support_module", ImportError, "mpl not support '!1@1!' as a module");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//InterruptedError
  define_new_exception(1, ERROR_ID, "zero_division", InterruptedError, "division by zero in '!1@1!' expression");
  define_new_exception(2, ERROR_ID, "wrong_remainder", InterruptedError,
                       "remainder(%%) is not determined for '!1@1!' numbers and expressions");
  define_new_exception(3, ERROR_ID, "out_of_range_index", InterruptedError,
                       "'!1@1!' out of range for '!2@2!' variable");
  define_new_exception(4, ERROR_ID, "out_of_range_break", InterruptedError, "'break' instruction parameter is not between 1 .. !1@1!");
  define_new_exception(5,
                       ERROR_ID,
                       "out_of_range_integer",
                       InterruptedError,
                       "result of '!1@1!' expression is out of range of integer");
  define_new_exception(6,
                       ERROR_ID,
                       "out_of_range_float",
                       InterruptedError,
                       "result of '!1@1!' expression is out of range of float");
  define_new_exception(7,
                       ERROR_ID,
                       "array_index_out_of_range",
                       InterruptedError,
                       "Index Array '!1@1!' is out of range from '0' to '!2@2!'");
  define_new_exception(8, ERROR_ID, "not_using_next_break", InterruptedError, "not using next and break instructions outside of a loop structure");
  define_new_exception(9, WARNING_ID, "array_index_underflow", InterruptedError, "Index Array '!1@1!' is underflow");
  define_new_exception(10,
                       ERROR_ID,
                       "array_index_overflow",
                       InterruptedError,
                       "Index Array '!1@1!' is overflow from '!2@2!'");

  define_new_exception(11, ERROR_ID, "review_array_out_of_range", InterruptedError,
                       "review array can not assign '!1@1!' to more than '!2@2!' variables");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//SyntaxError
  define_new_exception(1, ERROR_ID, "non_dec_stat", SyntaxError,
                       "'!1@1!' is non-declaration statement outside a body");
  define_new_exception(2, FATAL_ID, "invalid_name_block", SyntaxError,
                       "'!1@1!' is not a valid name for a function or package");
  define_new_exception(3, FATAL_ID, "redeclared_package", SyntaxError, "'!1@1!' redeclared as a package before");
  define_new_exception(4, FATAL_ID, "param_def_val", SyntaxError,
                       "'!1@1!' as a '!2@2!' function parameter can not have any default value");
  define_new_exception(5, ERROR_ID, "reiterative_attr", SyntaxError, "'!1@1!' reiterative before as a attribute");
  define_new_exception(6, FATAL_ID, "not_end_acod", SyntaxError, "expected '}' at end of input");
  define_new_exception(7, FATAL_ID, "not_start_acod", SyntaxError, "expected '{' at start of input");
  define_new_exception(8, ERROR_ID, "not_defined_struct", SyntaxError, "'!1@1!' struct can not defined in a structure");
  define_new_exception(9, FATAL_ID, "define_func_in", SyntaxError, "'!1@1!' is defined into another function");
  define_new_exception(10, ERROR_ID, "redeclared_var", SyntaxError,
                       "'!1@1!' redeclared as a variable before in this function or package");
  define_new_exception(11, ERROR_ID, "redeclared_func", SyntaxError,
                       "'!1@1!' redeclared as a function before with same parameters");
  define_new_exception(12, ERROR_ID, "wrong_def_var", SyntaxError,
                       "'!1@1!' is a wrong expression for variable declaration");
  define_new_exception(13, ERROR_ID, "invalid_name_var", SyntaxError, "'!1@1!' is not a valid name for a variable");
  define_new_exception(14, ERROR_ID, "invalid_next_inst", SyntaxError, "'next' instruction not accept any parameters");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//ValueError
  define_new_exception(1, ERROR_ID, "val_def_var", ValueError,
                       "'!1@1!' cannot define as type '!2@2!' in variable declaration");
  define_new_exception(2, ERROR_ID, "invalid_exp_val", ValueError, "'!1@1!' is invalid as a '!2@2!' expression");
  define_new_exception(3, ERROR_ID, "not_alloc_val", ValueError, "'!1@1!' can not get '!2@2!' as itself value");
  define_new_exception(4, ERROR_ID, "not_equal_vals_vars", ValueError,
                       "count of variables is not equal with count of initialization values");
  define_new_exception(5, ERROR_ID, "not_valid_val_param", ValueError,
                       "'!1@1!' is not a valid value as a function parameter");
  define_new_exception(6, ERROR_ID, "not_defined_array", ValueError,
                       "'!1@1!' defined by one value and its assigned value is an array");
  define_new_exception(7, ERROR_ID, "unknown_magic_macro", ValueError,
                       "'!1@1!' is an unknown magic macro");
  define_new_exception(8, ERROR_ID, "not_defined_mm_key", ValueError,
                       "'!1@1!' not defined in '!2@2!' magic macro");
  define_new_exception(9, ERROR_ID, "reinitialized_in__define_mm", ValueError,
                       "'!1@1!' was defined in '__define' magic macro. you can not Re-initialized it");
  define_new_exception(10, ERROR_ID, "not_global__config_mm", ValueError,
                       "you can not edit '__config' magic macro items inside of a function");
  define_new_exception(11, ERROR_ID, "not_float_array_index", ValueError,
                       "Array index can not be as a float or huge number");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//NotExistError
  define_new_exception(1, ERROR_ID, "not_exist_func", NotExistError,
                       "'!1@1!' with '!2@2!' parameters, not exist as a function");
  define_new_exception(2, ERROR_ID, "wrong_type_var", NotExistError,
                       "'!1@1!' is not a valid type for variable declaration");
  define_new_exception(3, ERROR_ID, "not_found_main", NotExistError,
                       "Not found 'main()' function in your source files");
  define_new_exception(4, ERROR_ID, "not_exist_var", NotExistError, "'!1@1!' is not exist as a variable");
  define_new_exception(5, ERROR_ID, "not_exist_struct_name", NotExistError, "Not exist name for struct");
  define_new_exception(6,
                       ERROR_ID,
                       "not_exist__config_mm",
                       NotExistError,
                       "Not exist any items by name '!1@1!' in '__config' magic macro");
  define_new_exception(7,
                       ERROR_ID,
                       "not_exist_func_param",
                       NotExistError,
                       "Not exist '!1@1!' as a parameter for '!2@2!' function");
  define_new_exception(8, ERROR_ID, "not_exist_mod_func", NotExistError,
                       "not found '!1@1!' function in '!2@2!' module");
  define_new_exception(9, ERROR_ID, "not_load_module", NotExistError,
                       "'!1@1!' module not load in memory and not available for using");
  //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//RuntimeError
  define_new_exception(1, ERROR_ID, "unknown_instruction", RuntimeError, "'!1@1!' is not a valid instruction");
  define_new_exception(2,
                       ERROR_ID,
                       "incorrect_global_inst",
                       RuntimeError,
                       "'!1@1!' is an invalid instruction in global");
  define_new_exception(3,
                       ERROR_ID,
                       "missing_func_param",
                       RuntimeError,
                       "'!1@1!' is a parameter of '!2@2!' function and must be in calling this function");
}

int8 exception_handler(String lbl_err, const char func_occur[], String rep1, String rep2) {
//  printf("FFFFFFFFFF:%s\n",lbl_err);
  //---------------------init vars
  int8 ret_num = WARNING_ID;
  long_int excep_id = 0;
  String func_occur_err = str_from_const_char(func_occur);
  //---------------------search in exceptions_list
  for (long_int i = 0; i < entry_table.exceptions_count; i++) {
    exli st = get_exli(i);
    if (str_equal(lbl_err, st.lbl)) {
      //fmt.Println("SSSSS:", st)
      ret_num = st.type;
      excep_id = i;
      break;
    }
  }
  exli st = get_exli(excep_id);
  //---------------------search for manage structure
  String manage_exception_store = 0;
  stst *tmp1 = entry_table.stst_start;
  //print_struct(PRINT_STRUCTURES_STACK_ST);
  if (tmp1 != 0) {
    for (;;) {
      // printf("##SSSSS:%i,%s\n", tmp1->type, tmp1->extra);
      if (tmp1->type == MANAGE_STRU_ID && tmp1->fin == entry_table.cur_fin && tmp1->sid == entry_table.cur_sid) {
        str_init(&manage_exception_store, tmp1->extra);
        break;
      }
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
  }
  //---------------------print error;
  if (manage_exception_store == 0 && excep_id > 0) {
    ret_num = print_error(entry_table.Rline, st.lbl, entry_table.Rsrc, rep1, rep2, func_occur_err);
  }
    //---------------------manage structure
  else if (manage_exception_store != 0 && excep_id > 0) {
    //---------modes
    switch (ret_num) {
      case ERROR_ID: ret_num = errors_mode;
        break;
      case WARNING_ID: ret_num = warnings_mode;
        break;
    }
//    printf("@@@@:%i,%s\n",ret_num,manage_exception_store);
    if (ret_num == CANCEL_ID) {
      return ret_num;
    }
    //---------if manage_exception_store not 'null'
    if (!str_equal(manage_exception_store, "null")) {
      uint8 sub = 0;
      String struct_id = calculate_struct_expression(create_exli_struct(st, rep1, rep2), "exception", &sub);
      print_vaar(get_stde(str_to_long_int(struct_id)).st);
      _MPL_TYPE__push(find_index_var_memory(str_to_long_int(manage_exception_store)),
                      struct_id,
                      "exception",
                      RIGHT_DIRECT);
    }
    //---------if was error
    if (ret_num == ERROR_ID) {
      //printf("@@@@WWWWW:\n");
      //cur_sid = last_sid
      //cur_order = last_order
      entry_table.is_occur_error_exception = true;
    }
      //---------if was fatal
    else if (ret_num == FATAL_ID) {
      __syscall_exit(EXIT_FAILURE);
    }
  }

  //---------------------return
  return ret_num;
}

//**************************************************************
exli search_lbl_exli(String lbl) {
  exli null = {0, 0, 0, 0, 0};
  exli *tmp1 = entry_table.exli_start;
  for (;;) {
    if (str_equal(tmp1->lbl, lbl)) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  return null;
}

//**************************************************************
exli get_exli(long_int i) {
  exli null = {0, 0, 0, 0, 0};
  long_int counter = 0;
  exli *tmp1 = entry_table.exli_start;
  for (;;) {
    if (i == counter) {
      return (*tmp1);
    }
    tmp1 = tmp1->next;
    counter++;
    if (tmp1 == 0) break;
  }
  return null;
}
//**************************************************************
String create_exli_struct(exli s, String rep1, String rep2) {
  String ret = 0;
//replace rep1,rep2
  s.text = str_replace(s.text, "!1@1!", rep1, -1);
  s.text = str_replace(s.text, "!2@2!", rep2, -1);
  //create struct value
  //struct::exception(num id,str msg,str group,num type,str src,num line)
  ret = str_append("struct(", str_from_long_int((long_int) s.id));
  ret = char_append(ret, ',');
  ret = str_multi_append(ret, "\"", s.text, "\"", 0, 0);
  ret = char_append(ret, ',');
  ret = str_multi_append(ret, "\"", exceptions_group[s.group], "\"", 0, 0);
  ret = char_append(ret, ',');
  ret = str_append(ret, str_from_int32((int32) s.type));
  ret = char_append(ret, ',');
  ret = str_multi_append(ret, "\"", entry_table.Rsrc, "\"", 0, 0);
  ret = char_append(ret, ',');
  ret = str_append(ret, str_from_int32((int32) entry_table.Rline));

  return ret;
}
//**************************************************************
void define_new_exception(uint32 id, int8 type, String lbl, uint8 group, String text) {
  //id;type;group;label;text
  exli *q;
  q = (exli *) malloc(sizeof(exli));
  if (q == 0) return;
  q->id = id;
  q->type = type;
  q->group = group;
  str_init(&q->lbl, lbl);
  str_init(&q->text, text);
  q->next = 0;
  entry_table.exceptions_count++;
  if (entry_table.exli_start == 0)
    entry_table.exli_start = entry_table.exli_end = q;
  else {
    entry_table.exli_end->next = q;
    entry_table.exli_end = q;
  }
}

//**************************************************************
int8 print_error(long_int line_err, String name_err, String file_err, String rep1, String rep2, String occur_func) {
  uint8 group_err = RuntimeError;
  int8 type_err = ERROR_ID;
  String text_err = 0;
  long_int id_err = 0;
  //---------------------search in exceptions_list
  exli search = search_lbl_exli(name_err);
  id_err = search.id;
  text_err = search.text;
  type_err = search.type;
  group_err = search.group;

  //----------------------modes
  switch (type_err) {
    case ERROR_ID: type_err = errors_mode;
      break;
    case WARNING_ID: type_err = warnings_mode;
      break;
  }
  if (type_err == CANCEL_ID) {
    return type_err;
  }
  //printf("%s,%s,%i,\n%s\n",rep1,rep2,group_err,print_str_list(exceptions_group,14));
  //----------------------replace items
  text_err = str_replace(text_err, "!1@1!", rep1, -1);
  text_err = str_replace(text_err, "!2@2!", rep2, -1);
  //-----------------------print exception
  String exception_msg = malloc(7 + str_length(file_err) + (2 * sizeof(long_int)) + str_length(text_err) +
      str_length(exceptions_group[group_err]) + 1);
  if (line_err == 0)
    sprintf(exception_msg, "%s : %s (%s::Errno%li)", exceptions_type[-type_err], text_err,
            exceptions_group[group_err], id_err);
  else
    sprintf(exception_msg, "%s[%s:%li] %s (%s::Errno%li)", exceptions_type[-type_err], file_err, line_err, text_err,
            exceptions_group[group_err], id_err);
  if (is_programmer_debug >= 1) {
    //y, mo, d := time.Now().Date()
    // h, mi, s := time.Now().Clock()
    //time_now := fmt.Sprintf("%v:%v:%v - %v.%v.%v", h, mi, s, y, mo, d)
    // exception_msg += new_line_char + fmt.Sprintf("Occurs in:[%s] [%s]", occur_func, time_now)
    printf("%s {occur_func:%s}\n", exception_msg, occur_func);
  } else {
    printf("%s\n", exception_msg);
  }

  if (logfile_path != 0) {
    //append_to_file(project_root+separator+is_set_logfile+".log", exception_msg+new_line_char+"---------------------------"+new_line_char)
    //msg("&YYYYY", project_root+separator+is_set_logfile+".log")
  }
  //-----------------------handle fatal
  if (type_err == FATAL_ID) {
    __syscall_exit(EXIT_FAILURE);
  }
  return type_err;
}


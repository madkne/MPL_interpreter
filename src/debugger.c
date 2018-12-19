//
// Created by amindelavar on 12/2/2018.
//

#include <MPL/system.h>
/**
 * mdebug commands:
 //=>debug line 5 of main source file
 * [OK]main:5
 //=>run current instruction in line 5 and then stop!
 * [OK]run                  (r)
 //=>run current instruction in line 5 and then continue
 * [OK]continue             (cn)
 //=>go to next instruction
 * [OK]next                 (nx)
 //=>print values of selection variables
 * [OK]print:var1,var2      (p:var1,var2)
 //=>print values of important registers of program
 * [OK]print:registers      (p:r)
 //=>print all breakpoints of program
 * [OK]print:breakpoints    (p:b)
 * //=>print all of the memory
 * [OK]print:memory         (p:m)
 //=>print all source file paths
 * [OK]print:sources        (p:s)
 //=>cancel debug mode and quit from mdebug
 * [OK]cancel               (c)
 //=>exit from mdebug and program
 * [OK]exit                 (e)
 //=>create new breakpoint like main:15
 * [OK]new                  (n)
 * //=>set a breakpoint in first line that an error is occur
 * [..]new:error                (n:e)
 * //=>set a breakpoint in first line that an warning is occur
 * [..]new:warning              (n:w)
 * //=>clear current breakpoint
 * [OK]clear                (cl)
 * //=>clear all breakpoints
 * [OK]clear:all            (cl:a)
 * //=>determine current instruction is what type of instructions
 * whatis                   (ws)
 *
 */


Boolean start_debugger() {
  printf("MPL Debugger [mdebug]:))\n");
  printf("For more information about commands, type \"mpl -h debugger\" after exit the program by command \"exit\"\n");
  //----------------set new breakpoint
  String source_file = 0;
  uint32 source_line = 0;
  set_new_breakpoint(&source_file, &source_line);
  append_debr(source_line, source_file);
  return true;
}

//*****************************************
void set_new_breakpoint(String *source_file, uint32 *source_line) {
//  printf("***%s\n",print_str_list(entry_table.sources_list, entry_table.sources_list_len));
  printf("%sCreate a new breakpoint (src:line)\n", DEBUG_QUESTION);
  for (;;) {
    printf(DEBUG_ANSWER);
    //-----------read input
    String input = read_input();
    //-----------check params
    str_list params = 0;
    uint32 params_len = char_split(input, ':', &params, true);
    //-----------exit command
    if (params_len == 1 && str_equal(params[0], "exit")/*exit the program*/)
      __syscall_exit(EXIT_SUCCESS);
      //-----------sources command
    else if (params_len == 1 && str_equal(params[0], "sources"/*display all sources*/)) {
      display_all_source_paths();
      continue;
    }
      //-----------cancel command
    else if (params_len == 1 && str_equal(params[0], "cancel")/*cancel debug mode*/) {
      edit_magic_macro(CONFIG_MAGIC_MACRO_TYPE, DEBUG_MODE, "false");
      entry_table.debr_start = 0;
      entry_table.debr_len = 0;
      break;
    }
    if (params_len != 2) {
      print_error(0, "not_enough_params", "stdin", 0, 0, "set_new_breakpoint");
      continue;
    }
    //-----------set source file
    //validate source path
    String ext = 0;
    if (str_indexof(params[0], ".", 0) == -1)str_init(&ext, ".mpl");
    (*source_file) = str_multi_append(project_root, char_to_str(OS_SEPARATOR), validate_path(params[0]), ext, 0, 0);
    if (!str_search(entry_table.sources_list, (*source_file), entry_table.sources_list_len)) {
      print_error(0, "not_exist_source", "stdin", (*source_file), 0, "set_new_breakpoint");
      continue;
    }
    //-----------set line number
    int32 tmp = str_to_int32(params[1]);
    if (!str_to_int32(params[1]) || tmp < 1) {
      print_error(0, "invalid_line_number", "stdin", params[1], 0, "set_new_breakpoint");
      continue;
    }
    (*source_line) = tmp;
    //-----------check line number is valid
    if (get_instru_by_source((*source_file), (*source_line)).id == 0) {
      print_error(0, "not_execute_line_code", "stdin", params[1], (*source_file), "set_new_breakpoint");
      continue;
    }
//    print_struct(PRINT_INSTRU_ST);

    break;
  }
}
//*****************************************
uint32 translate_debug_symbols(String s, str_list *pars) {
//-----------check params
  str_list params = 0;
  uint32 params_len = char_split(s, ':', &params, true);
  Boolean is_short = false;
  //-----------analyzing params
  for (uint32 i = 0; i < params_len; i++) {
    //run command
    if (i == 0 && str_ch_equal(params[i], 'r'))str_init(&params[i], "run");
      //continue command
    else if (i == 0 && str_equal(params[i], "cn"))str_init(&params[i], "continue");
      //next command
    else if (i == 0 && str_equal(params[i], "nx"))str_init(&params[i], "next");
      //cancel command
    else if (i == 0 && str_ch_equal(params[i], 'c'))str_init(&params[i], "cancel");
      //exit command
    else if (i == 0 && str_ch_equal(params[i], 'e'))str_init(&params[i], "exit");
      //print command
    else if (i == 0 && str_ch_equal(params[i], 'p')) {
      str_init(&params[i], "print");
      is_short = true;
    } else if (i > 0 && str_equal(params[0], "print") && is_short) {
      if (i == 1 && str_ch_equal(params[i], 'r'))str_init(&params[i], "registers");
      else if (i == 1 && str_ch_equal(params[i], 'b'))str_init(&params[i], "breakpoints");
      else if (i == 1 && str_ch_equal(params[i], 'm'))str_init(&params[i], "memory");
      else if (i == 1 && str_ch_equal(params[i], 's'))str_init(&params[i], "sources");
    }
      //new command
    else if (i == 0 && str_ch_equal(params[i], 'n')) {
      str_init(&params[i], "new");
      is_short = true;
    }
      //clear command
    else if (i == 0 && str_equal(params[i], "cl")) {
      str_init(&params[i], "clear");
      is_short = true;
    } else if (i > 0 && str_equal(params[0], "clear") && is_short) {
      if (i == 1 && str_ch_equal(params[i], 'a'))str_init(&params[i], "all");
    }
  }
  //-----------return
  (*pars) = params;
  return params_len;
}
//*****************************************
void on_breakpoint_interrupt(String code, Boolean is_running) {
  entry_table.debug_is_run = false;
  entry_table.debug_is_next = false;
  code=get_origin_code(code);
  if (is_running) printf("(Running:%i) %s\n", entry_table.Rline, code);
  else printf("(Completed:%i) %s\n", entry_table.Rline, code);
  printf("%sEnter your command (For skip type \"continue\")\n", DEBUG_QUESTION);
  for (;;) {
    printf(DEBUG_ANSWER);
    //-----------read input
    String input = read_input();
    //-----------check params
    str_list params = 0;
    uint32 params_len = translate_debug_symbols(input, &params);
    //-----------exit command
    if (str_equal(params[0], "exit")/*exit from program*/)
      __syscall_exit(EXIT_SUCCESS);
      //-----------run command
    else if (str_equal(params[0], "run")/*run current line*/) {
      if (!is_running) {
        print_error(0,
                    "can_not_run", "stdin",
                    str_from_long_int((long_int) entry_table.Rline), 0, "on_breakpoint_interrupt");
        continue;
      }
      entry_table.debug_is_run = true;
      break;
    }
      //-----------next command
    else if (str_equal(params[0], "next")/*go to next instruction*/) {
      entry_table.debug_is_next = true;
      break;
    }
      //-----------cancel command
    else if (str_equal(params[0], "cancel")/*cancel debug mode*/) {
      edit_magic_macro(CONFIG_MAGIC_MACRO_TYPE, DEBUG_MODE, "false");
      entry_table.debr_start = 0;
      entry_table.debr_len = 0;
      break;
    }
      //-----------clear commands
    else if (str_equal(params[0], "clear")/*cancel debug mode*/) {
      if (params_len == 1 && !delete_debr(entry_table.Rline, entry_table.Rsrc)) {
        print_error(0,
                    "not_find_breakpoint",
                    "stdin",
                    str_from_long_int((long_int) entry_table.Rline),
                    entry_table.Rsrc,
                    "on_breakpoint_interrupt");
        continue;
      } else if (params_len == 2 && str_equal(params[1], "all")) {
        entry_table.debr_start = 0;
        entry_table.debr_len = 0;
      }
    }
      //-----------new commands
    else if (str_equal(params[0], "new")/*create new breakpoint*/) {
      String path = 0;
      uint32 line = 0;
      if (params_len == 1) set_new_breakpoint(&path, &line);
      //TODO
      append_debr(line, path);
    }
      //-----------print commands
    else if (str_equal(params[0], "print")/*print info*/) {
      if (params_len == 1) {
        //TODO
      } else {
        if (str_equal(params[1], "breakpoints"))display_all_breakpoints();
        else if (str_equal(params[1], "sources"))display_all_source_paths();
        else if (str_equal(params[1], "memory"))show_memory(0);
        else if (str_equal(params[1], "registers"))display_all_registers();
        else display_value_vars(params[1]);
      }
    }
      //-----------continue command
    else if (str_equal(params[0], "continue")/*continue until next breakpoint*/)break;
      //-----------unknown command
    else {
      print_error(0, "unknown_command", "stdin", input, 0, "on_breakpoint_interrupt");
      continue;
    }

  }
}
//*****************************************
String get_origin_code(String code){
  String ret=0;
  if(code[0]=='@'){
    blst s=search_lbl_stru(code);
    ret=str_multi_append(return_type_structure(s.type),"(",str_join(s.params,s.params_len,";"),")",0,0);
  }else{
    ret=code;
  }

  return ret;
}
//*****************************************
void display_all_breakpoints() {
  debr *tmp1 = entry_table.debr_start;
  if (tmp1 == 0) {
    printf("(NULL) breakpoints!\n");
    return;
  }
  uint32 counter = 0;
  for (;;) {
    if (tmp1->source_path == 0)continue;
    String cur = 0;
    str_init(&cur, " ");
    if (entry_table.Rline == tmp1->line_number && str_equal(entry_table.Rsrc, tmp1->source_path))
      str_init(&cur,
               "(current)");
    printf("Breakpoint %i: [%s] - [%i] %s\n", ++counter, tmp1->source_path, tmp1->line_number, cur);
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
}
//*****************************************
void display_all_source_paths() {
//  printf("***%s\n", print_str_list(entry_table.sources_list, entry_table.sources_list_len));
  if (entry_table.sources_list_len == 0) {
    printf("(NULL) source codes!\n");
    return;
  }
  String cur = 0;
  for (uint32 i = 0; i < entry_table.sources_list_len; i++) {
    str_init(&cur, " ");
    if (str_equal(entry_table.Rsrc, entry_table.sources_list[i]))str_init(&cur, "(current)");
    String trim_path = str_substring(str_replace(entry_table.sources_list[i], project_root, "", -1), 1, 0);
    printf("SourceCode %i: %s \t[%s] %s\n", i + 1, trim_path, entry_table.sources_list[i], cur);
  }
}
//*****************************************
void display_all_registers() {
  printf("fin(%i) , ", entry_table.cur_fin);
  printf("fid(%i) , ", entry_table.cur_fid);
  printf("pfin(%i) , ", entry_table.parent_fin);
  printf("order(%i) , ", entry_table.cur_order);
  printf("sid(%i)\n", entry_table.cur_sid);
}
//*****************************************
void display_value_vars(String vars) {
  //-----------check params
  str_list params = 0;
  uint32 params_len = char_split(vars, ',', &params, true);
  for (uint32 i = 0; i < params_len; i++) {
    long_int id = return_var_id(params[i], 0);
    if (id == 0) {
      print_error(0, "undefined_var", "stdin", params[i], 0, "display_value_vars");
      continue;
    }
    long_int var_ind = find_index_var_memory(id);
    Mvar var = get_Mvar(var_ind);
    String val = return_value_var_complete(var_ind);

    printf("%i) %s %s =\n\t%s\n", i + 1, get_datas(var.type_var).name, var.name, val);
  }
}
//*****************************************
//*****************************************
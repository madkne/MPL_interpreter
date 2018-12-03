//
// Created by amindelavar on 12/2/2018.
//

#include <MPL/system.h>
/**
 * mdebug commands:
 * [OK]main:5       //=>debug line 5 of main source file
 * run              //=>run current instruction in line 5 and then stop!
 * [OK]continue     //=>run current instruction in line 5 and then continue
 * next             //=>go to next instruction
 * print:var1,var2  //=>print values of selection variables
 * print:registers  //=>print values of important registers of program
 * [OK]print:breakpoints //=>print all breakpoints of program
 * [OK]print:memory     //=>print all of the memory
 * [OK]print:sources    //=>print all source file paths
 * [OK]cancel       //=>cancel debug mode and quit from mdebug
 * [OK]exit         //=>exit from mdebug and program
 * [OK]new          //=>create new breakpoint like main:15
 * new:error    //=>set a breakpoint in first line that an error is occur
 * new:warning //=>set a breakpoint in first line that an warning is occur
 * [OK]clear            //=>clear current breakpoint
 * [OK]clear:all        //=>clear all breakpoints
 * whatis     //=>determine current instruction is what type of instructions
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
    if (params_len == 1 && str_equal(params[0], "exit")/*exit the program*/)
      __syscall_exit(EXIT_SUCCESS);
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
void on_breakpoint_interrupt(String code) {
  printf("(Running:%i) %s\n", entry_table.Rline, code);
  printf("%sEnter your command (For skip type \"continue\")\n", DEBUG_QUESTION);
  for (;;) {
    printf(DEBUG_ANSWER);
    //-----------read input
    String input = read_input();
    //-----------check params
    str_list params = 0;
    uint32 params_len = char_split(input, ':', &params, true);
    //-----------exit command
    if (str_equal(params[0], "exit")/*exit from program*/)
      __syscall_exit(EXIT_SUCCESS);
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
        print_error(0, "not_find_breakpoint", "stdin",entry_table.Rline, entry_table.Rsrc, "on_breakpoint_interrupt");
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
  String cur = 0;
  for (uint32 i = 0; i < entry_table.sources_list_len; i++) {
    str_init(&cur, " ");
    if (str_equal(entry_table.Rsrc, entry_table.sources_list[i]))str_init(&cur, "(current)");
    printf("SourceCode %i: %s %s\n", i + 1, entry_table.sources_list[i], cur);
  }
}
//*****************************************
//*****************************************
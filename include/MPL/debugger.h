//
// Created by amindelavar on 12/2/2018.
//

#ifndef MPL_DEBUGGER_H
#define MPL_DEBUGGER_H
Boolean start_debugger();
void set_new_breakpoint(String *source_file, uint32 *source_line);
uint32 translate_debug_symbols(String s,str_list *pars);
void on_breakpoint_interrupt(String code,Boolean is_running);
String get_origin_code(String code);
void display_all_breakpoints();
void display_all_source_paths();
void display_all_registers();
void display_value_vars(String vars);
#endif //MPL_DEBUGGER_H

//
// Created by amindelavar on 12/2/2018.
//

#ifndef MPL_DEBUGGER_H
#define MPL_DEBUGGER_H
Boolean start_debugger();
void set_new_breakpoint(String *source_file, uint32 *source_line);
void on_breakpoint_interrupt(String code);
void display_all_breakpoints();
void display_all_source_paths();
#endif //MPL_DEBUGGER_H

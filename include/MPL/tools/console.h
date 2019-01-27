//
// Created by amindelavar on 1/26/2019.
//

#ifndef MPL_CONSOLE_H
#define MPL_CONSOLE_H

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

Boolean console_enable_color_mode();
void console_color_red();
void console_color_yellow();
void console_color_bold();
void console_color_underline();
void console_color_reverse();
void console_color_reset();
#endif //MPL_CONSOLE_H

//
// Created by amindelavar on 1/26/2019.
//

#include <MPL/system.h>

Boolean console_enable_color_mode() {
  #if WINDOWS_PLATFORM == true
  // Set output mode to handle virtual terminal sequences
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) return false;
  DWORD dwMode = 0;
  if (!GetConsoleMode(hOut, &dwMode))return false;
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if (!SetConsoleMode(hOut, dwMode))return false;
  #endif
}
void console_color_red() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[1;31m");
  #endif
}
//********************************
void console_color_yellow() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[1;33m");
  #endif
}
//********************************
void console_color_bold() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[1m");
  #endif
}
//********************************
void console_color_underline() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[4m");
  #endif
}
//********************************
/**
 * Swaps foreground and background colors
 */
void console_color_reverse() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[7m");
  #endif
}
//********************************
void console_color_reset() {
  #if WINDOWS_PLATFORM == true
  wprintf(L"\033[0m");
  #endif
}


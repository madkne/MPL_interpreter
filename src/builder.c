//
// Created by amindelavar on 1/20/2019.
//

#include <MPL/system.h>

void start_builder() {
  printf("MPL Program Builder [mbuild]:))\n");
  printf("For more information about commands, type \"mpl -h builder\" in command-line\n");
  //=>reset global vars
  is_programmer_debug = 0;
  entry_table.Rline = 0;
  entry_table.Rsrc = 0;
  edit_magic_config("BuildMode", "false", "bool");
  //=>level 1 : check exist builder lib,compiler,linker,resource
  printf("#check for requirements:\n");
  if (!builder_check_exist())return;
}

//*********************************************
Boolean builder_check_exist() {
  //=>check exist compiler
  printf("------------checking compiler:\n");
  //=>check gcc
  if (str_indexof(_OS_TYPE__shell("gcc -v"), "gcc version", 0) == -1) {
    //TODO:error
    printf("B#ERR3679\n");
    return false;
  }
  printf("\t[OK]\n");
  //=>check exist linker
  printf("------------checking linker:\n");
  //=>check ld
  if (str_indexof(_OS_TYPE__shell("ld -v"), "GNU ld", 0) == -1) {
    //TODO:error
    printf("B#ERR3678\n");
    return false;
  }
  printf("\t[OK]\n");
  //=>check exist resource
  printf("------------checking resource:\n");
  //=>check windres
  if (str_indexof(_OS_TYPE__shell("windres -V"), "GNU windres", 0) == -1) {
    //TODO:error
    printf("B#ERR3677\n");
    return false;
  }
  printf("\t[OK]\n");
  printf("------------checking %s:\n", BUILDER_LIB_NAME);
  //=>check builder library
  if (fopen(str_multi_append(interpreter_path, os_separator, BUILDER_LIB_NAME, 0, 0, 0), "rb") == NULL) {
    //TODO:error
    printf("B#ERR3676\n");
    return false;
  }
  printf("\t[OK]\n");
}
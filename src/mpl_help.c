//
// Created by amindelavar on 11/22/2018.
//
#include <MPL/system.h>
/***
 * mpl -h [OK]
 * mpl -h usage [OK]
 * mpl -h -l (list all of groups) [OK]
 * mpl -h keywords (introduction about this group) [OK]
 * mpl -h keywords -l [OK]
 * mpl -h keywords return (doc about 'return' 'keyword') [OK]
 * mpl -h keywords return -l (list all of parts of text) [OK]
 * mpl -h keywords return intro (only shows 'intro' part of 'return' text) [OK]
 */
//************************************************
String help_groups[] = {
    "keywords",
    "built_in",
    "exceptions",
    "debugger",
    "get_started",
    "about"
};
//************************************************
String help_get_started_subjects[] = {
    "whats_new",
    "topics",
    "samples",
    "features"
};
//************************************************
String help_about_subjects[] = {
    "license",
    "more",
    "README.md",
    "credits"
};

Boolean mpl_help_starter(str_list argvs, uint32 argvs_len) {
  //help || usage || -l || [group] || [group] -l || [group] [subject] -l || [group] [subject] [part]
  //  printf ("--Help:%s\n", print_str_list (argvs, argvs_len));
  //=>print main help
  if (argvs_len == 0) mpl_help_main();
    //=>print help usage
  else if (argvs_len == 1 && str_equal(argvs[0], "usage")) mpl_help_usage();
    //=>print anything
  else {
    //init vars
    String abs_mpl = get_mpl_dir_path(); //=>absolute path of mpl execute
    String sp = char_to_str(OS_SEPARATOR); //=>platform file path separator
    String docs_dir = str_multi_append(abs_mpl, sp, "docs", sp, 0, 0); //=>absolute path of 'docs' dir
    String not_exist = 0;
    uint8 footer[300]; //=>footer texts for some docs pages
    footer[0] = 0;
    uint8 header[300]; //=>header texts for all docs pages
    header[0] = 0;
//    String header_sp = "___|____________________";
    //---------------------------------------------
    //=>just list of all groups
    if (argvs_len == 1 && str_equal(argvs[0], "-l")/*list of groups*/) {
      printf("mpl:)) offline documentation groups:\n%s\n", header_full);
      for (uint8 i = 0; i < StrArraySize (help_groups); i++)
        DRAW_TABLE_ROW(i + 1, help_groups[i])
    }
      //=>select a group whitout any parameters, so show group intro
    else if (argvs_len == 1 && str_search(help_groups, argvs[0], StrArraySize (help_groups))/*intro of the group*/) {
      if (mpl_help_print_article(str_multi_append(docs_dir, argvs[0], sp, "index.mdo", 0, 0), "#intro")
          == -1)
        str_init(&not_exist, argvs[0]);
      sprintf(footer, "For getting list of all subjects of \'%s\' group, Type \'mpl -h %s -l\'", argvs[0], argvs[0]);
    }
      //=>list of all subjects of a group selected
    else if (argvs_len == 2 && str_search(help_groups, argvs[0], StrArraySize (help_groups))
        && str_equal(argvs[1], "-l")/*list of all subjects*/) {
      printf("mpl:)) offline documentation '%s' subjects:\n", argvs[0]);
      //=>if not exist list of subjects
      if (mpl_help_print_article(str_multi_append(docs_dir, argvs[0], sp, "index.mdo", 0, 0), "#subjects") == -1)
        str_init(&not_exist, argvs[0]);
    }
      //=>list of all parts of a subject selected from a group selected
    else if (argvs_len == 3 && str_search(help_groups, argvs[0], StrArraySize (help_groups))
        && str_equal(argvs[2], "-l")/*list of all parts of the subject*/) {
      printf("mpl:)) offline documentation '%s' (%s) parts:\n", argvs[0], argvs[1]);
      str_list lines = 0;
      int32
      //=>read all lines of subject file
          lines_co = read_lines_from_file(str_multi_append(docs_dir, argvs[0], sp, argvs[1], ".mdo", 0), &lines, true);
      uint32 co = 0;
      //=>if not exist subject file
      if (lines_co == -1)str_init(&not_exist, argvs[1]);
      else {
        for (uint32 i = 0; i < lines_co; i++)
          if (lines[i][0] == '#') {
            if ((co > 0 && co % 3 == 0))printf("\n");
            printf("%-20s \t", str_substring(lines[i], 1, 0));
            co++;
          }
        printf("\n");
      }
    }
      //=>show a specific part of a subject or all parts
    else if ((argvs_len == 2 || argvs_len == 3)
        && str_search(help_groups,
                      argvs[0],
                      StrArraySize (help_groups))/*show a subject text by a part or all parts*/) {
      String part = 0;
      if (argvs_len == 3)str_init(&part, argvs[2]);
      if (mpl_help_print_article(str_multi_append(docs_dir, argvs[0], sp, argvs[1], ".mdo", 0), part) == -1)
        str_init(&not_exist, argvs[1]);
      sprintf(footer,
              "For getting list of all parts of \'%s\' subject, Type \'mpl -h %s %s -l\'",
              argvs[0],
              argvs[1],
              argvs[1]);
    }

    //-----------------------if not exist file
    if (footer[0] != 0) DRAW_TABLE_FOOTER(footer);
    if (not_exist != 0) {
      print_error(0, "not_exist_docs_file", "stdin", not_exist, 0, "mpl_help_starter");
      return false;
    }
  }
  return true;
}
//************************************************
void mpl_help_usage() {
  printf("usage for mpl: mpl [option] --OR-- mpl [path] [arg]...\n");
  printf("usage for mpl-docs: mpl -h [group] [subject] [part]\n");
  printf("Try 'mpl -h' for more information.\n");
}
//************************************************
void mpl_help_main() {
  printf("\t In The Name Of ALLAH\n\t----------------------\n");
  printf("%s[%s-%s] build:[%s %s-%s] arch:[%s] under:[%s]\n", APP_NAME, VERSION_NAME, VERSION, OS_BUILD,
         BUILD_DATE, BUILD_TIME, OS_ARCH, LICENCE);
  printf("usage: mpl [option] --OR-- mpl [path] [arg]...\n");
  printf("Options:\n");
  printf("-v	:Display interpreter version information\n");
  printf("-h	:Display information about anything. \n\n");
  printf("usage(mpl-docs): mpl -h [group] [subject] [part]\n");
  printf(
      "You can type \'mpl -h -l\' to get list of all groups.\nAnd you can type \'mpl -h [group] -l\' to get list of all subjects of [group].\n");
  printf("-------------------------------------\n");
  printf("%s\n", SLOGAN);
  printf("See '%s' for more details.\n", OFFICIAL_WEBSITE);
}
//************************************************
int8 mpl_help_print_article(String path, String part) {
  if (part != 0 && part[0] != '#')part = str_multi_append("#", part, 0, 0, 0, 0);
  str_list lines = 0;
  uint32 c1 = 0;//=>list items counter
  uint32 c2 = 0;
  Boolean is_range = false;//=>if browse a specific part or all part
  Boolean is_exist = false;//=>if exist the article
  if (part == 0)is_range = true;//if not any specific part, then browse all parts
  //=>read all lines from [file].mdo and store it in 'lines' str_list
  int32 lines_co = read_lines_from_file(path, &lines, true);
  //=>check if occur an exception when reading file or is END OF FILE(EOF)
  if (lines_co == -1)return -1;
  //=>browse all nodes of 'lines'
  for (uint32 i = 0; i < lines_co; i++) {
    //=>check if this line is start of specific part
    if (!is_range && str_equal(lines[i], part)) {
      is_range = true;
      continue;
    }
      //=>if exist specific part, so ignore the other parts
    else if (part != 0 && is_range && lines[i][0] == '#')break;
    //=>if in specific part or start of browse all parts
    if (is_range) {
      Boolean is_part_header = false;
      //=>if this line is a part header
      if (lines[i][0] == '#') {
        is_part_header = true;
        printf("%s\n=> | %s\n%s\n", header_full, str_substring(lines[i], 1, 0), header_sp);
      }
      //=>list items format
      if (lines[i][0] == '*') {
        if (c1 == 0)printf("%s\n", header_full);
        DRAW_TABLE_ROW(c1 + 1, str_substring(lines[i], 1, 0));
        c1++;
      } else if (lines[i][0] == '-') {
        c2++;
        printf("%i.  %s \n", c2, str_substring(lines[i], 1, 0));
      } else if (!is_part_header) {
        //=>every line format
        if (c1 == 0)printf("   | ");
        printf("%s\n", lines[i]);
        c1 = c2 = 0;
      }
      is_exist = true;
    }
  }
  if (!is_exist) {
    printf("\n=====): Not Found '%s' Part :(=====\n\n", part);
    return 1;
  }
  return 0;
}

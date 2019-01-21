//
// Created by amindelavar on 11/22/2018.
//

#ifndef MPL_MPL_HELP_H
#define MPL_MPL_HELP_H

//*******************************defines
#define header_full   "________________________"
#define header_sp     "___|____________________"
#define DRAW_TABLE_ROW(ID, ROW) printf("   |\n%3i| %s\n%s\n", ID, ROW,header_sp);
#define DRAW_TABLE_FOOTER(ROW) printf("%s\n   |\n:::| %s\n%s\n",header_full, ROW,header_sp);
#define MPL_LICENCE_VIEW_COMMAND  "mpl -h about license"
//*******************************arrays
String help_groups[10];
String help_get_started_subjects[3];
String help_about_subjects[4];

//*******************************functions
Boolean mpl_help_starter(str_list argvs, uint32 argvs_len);
void mpl_help_usage();
void mpl_help_version();
void mpl_help_info();
void mpl_help_main();
int8 mpl_help_print_article(String path, String part,Boolean generate_list);
void mpl_help_article(String params[3], String *not_exist);
void mpl_help_list(uint8 type, String params[3], String *not_exist);
#endif //MPL_MPL_HELP_H

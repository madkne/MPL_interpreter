//
// Created by amindelavar on 11/22/2018.
//

#ifndef MPL_MPL_HELP_H
#define MPL_MPL_HELP_H

String help_groups[6];
String help_get_started_subjects[4];
String help_about_subjects[4];

Boolean mpl_help_starter (str_list argvs, uint32 argvs_len);
void mpl_help_usage ();
int8 mpl_help_print_article (String path, String part);
#endif //MPL_MPL_HELP_H

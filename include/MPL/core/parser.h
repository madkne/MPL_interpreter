//
// Created by amindelavar on 9/19/2018.
//

#ifndef MPL_PARSER_H
#define MPL_PARSER_H

#define GET_ORDER_ID(x1, x2, x3) ((x1*max_size_id*max_size_id)+(x2*max_size_id)+x3)

//********************************
Boolean analyze_source_code();

void manage_import_keywords(uint32 *i);

void manage_structs(uint32 *i);

void manage_normal_instructions(uint32 *i);

void manage_functions(uint32 *i);

void manage_structures(uint32 *i, String lbl);

long_int get_last_id_active_is_in_stru();

void get_all_tokens();


#endif //MPL_PARSER_H

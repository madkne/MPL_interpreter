//
// Created by amindelavar on 9/22/2018.
//

#ifndef MPL_BUILT_IN_H
#define MPL_BUILT_IN_H

uint32 call_built_in_funcs (String func_name, str_list params, str_list partypes, uint32 params_len, str_list *returns);
void init_built_in_funcs ();
void init_built_in_defines ();
void init_default_configs ();

#endif //MPL_BUILT_IN_H

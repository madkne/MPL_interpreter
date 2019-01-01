//
// Created by amindelavar on 9/22/2018.
//

#ifndef MPL_BUILT_IN_H
#define MPL_BUILT_IN_H

Boolean check_built_in_module_function(String S_params, str_list partypes, uint32 params_len);
void convert_built_in_module_vars_to_values(str_list partypes,str_list params,uint32 params_len,str_list *argvs,long_int *var0_ind);
uint32 call_built_in_funcs(String func_name, str_list params, str_list partypes, uint32 params_len, str_list *returns);
void init_built_in_funcs();

#endif //MPL_BUILT_IN_H

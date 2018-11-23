//
// Created by amindelavar on 9/23/2018.
//

#ifndef MPL_RUN_MGR_H
#define MPL_RUN_MGR_H

uint8 labeled_instruction (String code);

String define_vars (String inst);

String alloc_magic_macros (String exp);

String function_call (String exp);

String init_calling_function (String pname, String fname, str_list params, uint32 param_len, String index);
void garbage_collector (uint8 type);
Boolean
is_exact_function (str_list func_params, uint32 func_params_len, str_list type_params, uint32 params_len, Boolean is_built_in);

int32 set_function_parameters (String pack_name, String func_name, str_list pars, uint32 pars_len);

uint32 determine_type_name_func_parameters (str_list params, uint32 params_len, str_list *ret);
Boolean function_return (String exp);
int8 vars_allocation (String exp);
#endif //MPL_RUN_MGR_H

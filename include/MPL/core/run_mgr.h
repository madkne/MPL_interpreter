//
// Created by amindelavar on 9/23/2018.
//

#ifndef MPL_RUN_MGR_H
#define MPL_RUN_MGR_H

uint8 labeled_instruction(String code);

String define_vars(String inst);

String alloc_magic_macros(String exp);

String function_call(String exp);

String init_calling_function(uint8 func_type,
                             String pname,
                             String fname,
                             str_list params,
                             uint32 param_len,
                             String index);
void garbage_collector(uint8 type);
Boolean
is_exact_function(str_list func_params,
                  uint32 func_params_len,
                  str_list type_params,
                  uint32 params_len,
                  Boolean is_built_in);
int8 run_package_module_function(uint8 func_type, String pack_name, String func_name, str_list pars, uint32 pars_len);
int8 set_function_parameters(String func_name, str_list pars, uint32 pars_len);

uint32 determine_type_name_func_parameters(str_list params, uint32 params_len, str_list *ret);
Boolean function_return(String exp);
Boolean vars_allocation(String exp);
Boolean vars_assign_allocation(Boolean valid_var,
                               Boolean is_array,
                               String alloc,
                               String origin_indexes,
                               long_int origin_pointer_id);
Boolean vars_swap_allocation(Mvar origin,String alloc, long_int origin_po_val);

String vars_allocation_short(String exp);
Boolean check_post_short_alloc();
Boolean do_show_allocation(String var_name, Boolean is_plusplus);
Boolean init_structures(String exp);
Boolean structure_MANAGE(long_int st_id, String value);
Boolean structure_CONDITION(long_int st_id, uint8 type, String value);
Boolean structure_LOOP(long_int st_id, uint8 type, str_list params);
int8 structure_loop_run_header(str_list insts, uint32 insts_len, uint8 part, uint32 *array_review_ind);
uint32 structure_split_segments(String part, str_list *segments);
uint8 labeled_loop_instruction(String code, uint8 part);
Boolean structure_loop_next_break(String code);
uint32 init_loop_review_array(String code, long_int *array_id, longint_list *alloc_ids, longint_list *alloc_indexes);
int8 structure_loop_review_array(uint32 review_number,
                                 long_int array_id,
                                 longint_list alloc_ids,
                                 longint_list alloc_inds,
                                 uint32 alloc_len);
#endif //MPL_RUN_MGR_H

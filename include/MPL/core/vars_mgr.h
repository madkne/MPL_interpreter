//
// Created by amindelavar on 9/19/2018.
//

#ifndef MPL_VARS_MGR_H
#define MPL_VARS_MGR_H

Boolean is_valid_name (String name, Boolean is_array);

void return_name_index_var (String s, Boolean is_empty_index, String *name, String *index);
String simplification_var_index (String index);
uint8 define_vars_analyzing (String inst, def_var_s vars_store[]);

uint8 determine_sub_type_var (String *value, String main_type);

long_int return_var_id (String var_name, String var_index);

String return_value_var_complete (long_int var_index);

uint32 return_total_array_rooms (String index);

String calculate_struct_expression (String value, String type_var, uint8 *sub_type);

uint8 return_size_value_dimensions (String val, int32 indexes[], String *first_item);

vaar_en return_value_dimensions (String value, String type_var, int32 indexes[], uint8 indexes_len);

String determine_value_type (String val);

long_int determine_struct_type (String s, long_int struct_fid, String *struct_name);

void calculate_value_of_var (String value, String type, String *ret_value, uint8 *ret_subtype);

String calculate_boolean_expression (String exp, uint8 *sub_type);

String return_true_false (String exp);

String determine_unknown_value (String value, uint8 *sub_type);

String format_string_expression (String exp);

void calculate_string_expression (String exp, String *value, uint8 *sub_type);

void calculate_math_expression (String exp, uint8 target_type, String *retval, uint8 *rettype);
String calculate_two_numbers (String num1, String num2, uint8 op, uint8 type_exp);
int32 balance_huge_numbers (String num1, String num2, uint8 type_nums, String *ret_num1, String *ret_num2);
int32
determine_max_min_huge_numbers (String num1, String num2, uint8 type_nums, String *mmax, String *mmin, uint8 *final_type);
String sum_huge_numbers (String num1, String num2, Boolean is_neg);
String muliply_huge_numbers (String num1, String num2);
String divide_huge_numbers (String num1, String num2);
String is_radix_need_convert (String buf, uint8 type_exp);
String convert_radixes_to_dec (String base, uint8 radix, uint8 type_exp);
String resize_to_int (String str_val);
String resize_to_float (String str_val);
uint8 priority_math_type (uint8 type_exp, uint8 tmp1);
String remove_unused_parenthesis (String value);
uint8 set_type_of_math (String *num11, String *num22);
uint8 determine_type_num (String num);
Boolean is_array_var (long_int id, Boolean is_id);
String return_default_value (String type);
String create_null_array (String type, int32 indexes[], uint8 indexes_len);
Boolean alloc_array_var (long_int var_ind, String array_val, String type);
Boolean alloc_struct_var (datas type_datas, long_int pointer_index, vaar_en struct_id);
#endif //MPL_VARS_MGR_H

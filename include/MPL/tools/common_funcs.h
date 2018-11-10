#ifndef __COMMON_FUNCS_H
#define __COMMON_FUNCS_H    1

String read_input ();

String get_host_name ();

long_int get_pid ();

String get_absolute_path (String path);

Boolean switch_bool (Boolean b);

void msg (String format, ...);

void longint_list_delete_first (longint_list *s, uint32 len);

void longint_list_append (longint_list *s, uint32 len, long_int n);

uint32 longint_list_join (longint_list *s, longint_list a1, uint32 a1_len, longint_list a2, uint32 a2_len);

void longint_list_init (longint_list *s, longint_list val, uint32 len);

String longint_list_print (longint_list s, uint32 len);

double int32_power (double base, int32 power);

String get_current_datetime (uint8 type);

String print_str_list (str_list s, uint32 len);

void print_struct (uint8 which);

uint32 source_paths_search (str_utf8 path);

utst return_utf8_string_value (String s);

void str_list_append (str_list *s, String s1, uint32 len);

void str_list_init (str_list *s, str_list val, uint32 len);

void str_list_delete_first (str_list *s, uint32 len);

uint32 str_list_join (str_list *s, str_list a1, uint32 a1_len, str_list a2, uint32 a2_len);

Boolean str_list_equal (str_list s1, uint32 len1, str_list s2, uint32 len2);

void utf8_str_list_append (utf8_str_list *s, str_utf8 s1, uint32 len);

String remove_incorrect_pars (String str, int32 *invalid_pars);

FILE *utf8_file_open (String filename, String mode);

String replace_in_expression (String exp, String rep, int32 start, int32 end, Boolean remove_pars, Boolean is_trim);

String make_valid_double (String s);

String limit_decimal_huge_numbers (String s);

Boolean is_equal_arrays_indexes (String s1, String s2);

Boolean has_two_limiting (String s, uint8 l1, uint8 l2, Boolean ignore_space);
String generate_return_var_name (String name, uint32 *co);
String replace_control_chars(String val);
#endif /*__COMMON_FUNCS_H*/
#ifndef __COMMON_FUNCS_H
#define __COMMON_FUNCS_H    1


String read_input();
String get_absolute_path(String path);
Boolean switch_bool(Boolean b);
void msg(String format,...);

long_int get_max_unsigned_size(uint8 bytes);
uint32 long_int_append(long_int **s,long_int n);
uint32 long_int_size(long_int *s);
String get_current_datetime(uint8 type);

String print_str_list(str_list s,uint32 len);
void print_struct(uint8 which);

uint32 source_paths_search(str_utf8 path);
utst return_utf8_string_value(String s);

void str_list_append(str_list *s, String s1, uint32 len);
Boolean str_list_equal(str_list s1, uint32 len1, str_list s2, uint32 len2);
void utf8_str_list_append(utf8_str_list *s, str_utf8 s1, uint32 len);

FILE * utf8_file_open(String filename, String mode);
#endif /*__COMMON_FUNCS_H*/
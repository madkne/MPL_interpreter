#ifndef __STRINGS_H
#define __STRINGS_H    1

#define StrArraySize(x) (sizeof(x)/sizeof(String))

//*************************************
Boolean str_equal(String s1, String s2);

String convert_to_string(String s);

String str_reomve_quotations(String s, String type);

uint32 str_length(String s);

String str_append(String s1, String s2);

void str_init(String *s, String val);

String str_replace(String s, String old_s, String new_s, int8 num);

Boolean str_search(str_list list, String s, uint32 size);

int32 str_to_int32(String s);

long_int str_to_long_int(String s);

Boolean str_is_int32(String s);

Boolean str_is_num(String str);

String str_from_long_int(long_int num);

String str_reverse(String s);

String str_trim_space(String s);

uint32 str_list_size(str_list s);

void str_empty(String *s);

Boolean str_is_empty(String s);

void str_swap(String *s1, String *s2);

String str_substring(String s, uint32 start, uint32 end);

int32 str_indexof(String s, String s1, uint32 start);
int32 char_last_indexof(String s,uint8 ch);
String str_to_upper_case(String text);

String str_to_lower_case(String text);

String str_from_int32(int32 x);
String str_from_bool(Boolean x);
String str_from_int64(int64 x);
Boolean str_to_bool(String s);
double str_to_double(String s);

String str_from_double(double n, uint8 afterpoint);

String str_multi_append(String s, String s1, String s2, String s3, String s4, String s5);

void str_to_utf8(str_utf8 *ret, String val);

String str_from_const_char(const char s[]);

String str_trim_optimized_boolean(String str);

String str_trim_number(String str);

String str_trim_last_float(String str);

Boolean str_has_suffix(String s, String find);

String str_make_negative_number(String s);

void str_detachment_float(String s, String *s1, String *s2);
String str_join(str_list s, uint32 len, String delimiter);
//***********************************************************************************
String char_backspace(String s);
uint8 char_to_uint8(uint8 c, Boolean *has_err);
String char_join(str_list s, uint8 sp, uint32 size, Boolean is_remove_empty);

uint32 char_split(String s, uint8 sp, str_list *ret, Boolean is_remove_empty);

String char_append(String s, uint8 c);

Boolean str_ch_equal(String s1, uint8 s2);

Boolean char_search(uint8 list[], uint8 c);

int32 char_search_index(uint8 list[], uint8 c);

uint32 char_search_count(String s, uint8 c);

String char_to_str(uint8 c);

#endif /*__STRINGS_H*/
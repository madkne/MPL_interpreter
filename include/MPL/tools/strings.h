#ifndef __STRINGS_H
#define __STRINGS_H    1

#define StrArraySize(x) (sizeof(x)/sizeof(String))
//*************************************
Boolean str_equal(String s1,String s2);
uint32 str_length(String s);
String str_append(String s1,String s2);
void str_swap(String *s1,String *s2);
void str_init(String *s,String val);
String str_replace(String s,String old_s,String new_s,int8 num);
Boolean str_search(str_list list,String s,uint32 size);
int32 str_to_int32(String s);
Boolean str_is_int32(String s);
String str_from_long_int(long_int num);
String str_reverse(String s);
String str_trim_space(String s);
void str_list_init(str_list *s,str_list val,uint32 len);
uint32 str_list_size(str_list s);
void str_empty(String *s);
Boolean str_is_empty(String s);
String str_substring(String s,uint32 start,uint32 end);
int32 str_indexof(String s,String s1,uint32 start);
String str_multi_append(String s, String s1, String s2, String s3, String s4, String s5);
void str_to_utf8(str_utf8 *ret,String val);

String char_join(str_list s,uint8 sp,uint32 size,Boolean is_remove_empty);
uint32 char_split(String s,uint8 sp,str_list *ret,Boolean is_remove_empty);
String char_append(String s,uint8 c);
Boolean str_ch_equal(String s1, uint8 s2);
Boolean char_search(uint8 list[],uint8 c);
String char_to_str(uint8 c);


#endif /*__STRINGS_H*/
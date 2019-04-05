//
// Created by amindelavar on 11/30/2018.
//

#ifndef MPL_OS_BUILTIN_H
#define MPL_OS_BUILTIN_H

void _OS_TYPE__exit(int32 i);
Boolean _OS_TYPE__print(str_list items, uint32 len);
String _OS_TYPE__input(String type);
String _OS_TYPE__shell(String command);
String _OS_TYPE__time();
String _OS_TYPE__rand(String min,String max);
String _OS_TYPE__argvs();
Boolean _OS_TYPE__printf(str_list types,str_list items, uint32 items_len);
Boolean _OS_TYPE__printf_format(String f,String s,String type,String *ret);
String _OS_TYPE__date_english(long_int timestamp,String format);
uint8 _OS_TYPE__get_12_from_24hours(uint8 h24);
#endif //MPL_OS_BUILTIN_H

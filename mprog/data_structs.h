//
// Created by amindelavar on 11/25/2018.
//

#ifndef MPL_DATA_STRUCTS_H
#define MPL_DATA_STRUCTS_H

//----------------------------typedef
typedef char int8;
typedef unsigned char uint8;
typedef int int32;
typedef unsigned int uint32;
typedef unsigned char *String;
typedef unsigned char **str_list;
typedef uint32 **utf8_str_list;
typedef uint32 *str_utf8;
typedef unsigned long long int long_int;
typedef long_int *longint_list;
typedef unsigned char Boolean;
//----------------------------define
#define   ERROR_ID        -1
#define   FATAL_ID        -2
//----------------------------arrays
String main_argvs[11];

#endif //MPL_DATA_STRUCTS_H

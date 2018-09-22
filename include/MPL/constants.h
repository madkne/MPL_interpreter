//
// Created by amindelavar on 9/19/2018.
//

#ifndef MPL_CONSTANTS_H
#define MPL_CONSTANTS_H


//**************************************************define compiler constants
#define  APP_NAME                       "Mini Portable Language"
#define  VERSION                        "0.0.46"               //1.10.100
#define  VERSION_NAME                   "BETA"
#define  SLOGAN                         "The solution to common problems of programmers" //راه حل کارهای متداول برنامه
// نویسان
#define  BUILD_DATE                     __DATE__
#define  BUILD_TIME                     __TIME__
#define  FIRST_BUILD                    "2018.9.19"
#define  LICENCE                        "Apache 2.0"
#define  OFFICIAL_WEBSITE               "http://mpl-lang.ir"
#define  CORE_CODE_LINES                0
#define  LANGUAGE_CREATOR               "Mohammad Amin Delavar Khalafi"
#define  CONFIDENCE_LEVEL               5
//**************************************************define
#if x64_OS == 1
#define OS_ARCH                         "x64"
#elif x86_OS == 1
#define OS_ARCH                         "x86"
#endif
#define   MAX_INT_LEN                   7
#define   MAX_FLOAT_LEN                 12
#define   true                          1
#define   false                         0
#define   max_array_dimensions          5
#define   max_var_alloc_instruction     20
#define   max_into_in_structures        20
#define   MAX_OPENED_FILES              20
#define   END_OF_FILE                  -10
#define   BUFSIZE                       4096
#define   SYSTEM_STRING_DEFINE          "SYS__STR_"

//------------------------------------------
#define   ERROR_ID                     -2
#define   WARNING_ID                   -3
#define   FATAL_ID                     -1
#define   CANCEL_ID                     0
//------------------------------------------
#define   UNKNOWN_LBL_INST              0
#define   STRUCTURE_LBL_INST            1
#define   DEF_VARS_LBL_INST             2
#define   ALLOC_VARS_LBL_INST           3
#define   FUNC_CALL_LBL_INST            4
#define   ALLOC_SHORT_LBL_INST          5
#define   RETURN_LBL_INST               6
#define   PACK_INIT_LBL_INST            7
#define   CONST_CALL_LBL_INST           8
#define   NEXT_BREAK_LBL_INST           9
#define   LOGIC_CALC_LBL_INST           10
#define   REVIEW_ARRAY_LBL_INST         11
//------------------------------------------
#define   ImportError                   0
#define   MahlibError                   1
#define   SyntaxError                   2
#define   InterruptedError              3
#define   ValueError                    4
#define   NotExistError                 5
#define   RuntimeError                  6
#define   IOError                       7
#define   NotAccessError                8
#define   OSError                       9
#define   OverflowError                 10
#define   TypeError                     11
#define   FloatingPointError            12
#define   CommandError                  13
//------------------------------------------
#define IMPORT_FILE                     1
#define IMPORT_LIBRARY                  2
#define IMPORT_EMBEDDED                 3
#define IMPORT_MODULE                   4
//------------------------------------------
#define STDIN_SOURCE_PATH               0
//------------------------------------------
#define PRINT_IMPORT_ST                 1
#define PRINT_EXCEPTIONS_ST             2
#define PRINT_UTF8_ST                   3
#define PRINT_MAIN_SOURCE_ST            4
#define PRINT_TOKENS_SOURCE_ST          5
#define PRINT_FUNC_ST                   6
#define PRINT_INSTRU_ST                 8
#define PRINT_STRU_ST                   9
#define PRINT_VIRTUAL_MEMORY_ST         10
//------------------------------------------
#define   BOOL_SUB_TYPE_ID              1
#define   STR_SUB_TYPE_ID               2
#define   INT_SUB_TYPE_ID               3
#define   FLOAT_SUB_TYPE_ID             4
#define   HUGE_SUB_TYPE_ID              5
//------------------------------------------used in parser-determine type of statements and block codes
#define   FUNC_BLOCK_ID                 1
#define   LOOP_STRU_ID                  2
#define   IF_STRU_ID                    3
#define   ELIF_STRU_ID                  4
#define   ELSE_STRU_ID                  5
#define   MANAGE_STRU_ID                6
#define   SWITCH_STRU_ID                7
//------------------------------------------used in data_defined,runtime-determine type data types that is main or struct
#define   MAIN_DATA_TYPE                1
#define   STRUCT_DATA_TYPE              2
//------------------------------------------used in built_in-determine type of built-in functions
#define   MPL_BUILT_IN_TYPE             1
#define   DATA_BUILT_IN_TYPE            2
#define   OS_BUILT_IN_TYPE              3
//**************************************************typedef
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
#if LINUX_PLATFORM == 1 && x64_OS == 1
typedef __int64_t           int64;
typedef __uint64_t          uint64;
#elif LINUX_PLATFORM == 1 && x86_OS == 1
typedef unsigned long int   uint64;
typedef signed long int     int64;
#elif WINDOWS_PLATFORM == 1 && x64_OS == 1
typedef unsigned long int   uint64;
typedef signed long int     int64;
#elif WINDOWS_PLATFORM == 1 && x86_OS == 1
typedef unsigned long int uint64;
typedef signed long int int64;
#else
typedef unsigned long int uint64;
typedef signed long int int64;
#endif

typedef long int num_int;
typedef double num_float;

typedef unsigned char *String;
typedef unsigned char **str_list;
typedef uint32 **utf8_str_list;
typedef uint32 *str_utf8;
typedef uint64 long_int;
typedef unsigned char Boolean;

#endif //MPL_CONSTANTS_H

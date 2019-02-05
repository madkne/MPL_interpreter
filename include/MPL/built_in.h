//
// Created by amindelavar on 9/22/2018.
//

#ifndef MPL_BUILT_IN_H
#define MPL_BUILT_IN_H

//******************************defines
//------------------mpl function defines
#define _MPL_LEN                1
#define _MPL_VAR_TYPE           2
#define _MPL_TYPEOF             3
#define _MPL_PUSH               4
#define _MPL_POP                5
#define _MPL_DEL                6
#define _MPL_CROP               7
#define _MPL_ERROR_HANDLE       8
#define _MPL_CONFIG_ALL         9
#define _MPL_DEFINE_ALL         10
#define _MPL_DEFINE_ISSET       11
#define _MPL_SESSION_ALL        12
#define _MPL_SESSION_ISSET      13
#define _MPL_EMBED_RUN          14
#define _MPL_MPL_EXECUTE        15
#define _MPL_ECHO               16
#define _MPL_DEFINE_UNSET       17
#define _MPL_SESSION_UNSET      18
//------------------data function defines
#define _DATA_TNUM              1
#define _DATA_TBOOL             2
#define _DATA_TSTR              3
#define _DATA_TARRAY            4
#define _DATA_AT                5
#define _DATA_INTO              6
#define _DATA_INUM              7
#define _DATA_IBOOL             8
#define _DATA_XOR               10
#define _DATA_AND               11
#define _DATA_OR                12
#define _DATA_NOT               13
#define _DATA_RSHIFT            14
#define _DATA_LSHIFT            15
//------------------os function defines
#define _OS_EXIT                1
#define _OS_PRINT               2
#define _OS_PRINTF              3
#define _OS_INPUT               4
#define _OS_SHELL               5
#define _OS_TIME                6
#define _OS_RAND                7
#define _OS_ARGVS               8
//******************************functions
Boolean check_built_in_module_function(String S_params, str_list partypes, uint32 params_len);
void convert_built_in_module_vars_to_values(str_list partypes,str_list params,uint32 params_len,str_list *argvs,long_int *var0_ind);
uint32 call_built_in_funcs(String func_name, str_list params, str_list partypes, uint32 params_len, str_list *returns);
void init_built_in_funcs();

#endif //MPL_BUILT_IN_H

//
// Created by amindelavar on 12/24/2018.
//

#ifndef MPL_FS_INTERFACE_H
#define MPL_FS_INTERFACE_H

//--------------------------------constants
#define FS_VERSION_INTERFACE                     "1.4"
//--------------------------------variables
mofu *fs_start;
mofu *fs_end;
//--------------------------------functions
int32 run_fs_mod_funcs(String func_name, str_list params, str_list pars_type, uint32 params_len, str_list *rets_mod);
void init_fs_module_funcs();
#endif //MPL_FS_INTERFACE_H

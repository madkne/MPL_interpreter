//
// Created by amindelavar on 12/24/2018.
//

#ifndef MPL_PUBLIC_MOD_H
#define MPL_PUBLIC_MOD_H

//******************************************constants
#define EXPORT                          __declspec(dllexport)
//******************************************typedef
#if WINDOWS_PLATFORM == true
typedef String (WINAPI *FUNCTYPE1)(void);
typedef String (WINAPI *FUNCTYPE2)(String);
#endif
//******************************************functions
int32 call_module_funcs(String mod_name,
                        String func_name,
                        str_list params,
                        str_list pars_type,
                        uint32 params_len,
                        str_list *rets_mod);
Boolean load_module_file(String path, uint32 line, String src);
uint32 return_module_id(String module_name);
void init_module_file_funcs(uint8 module_id);


String call_module_type2(String func_name,uint32 mpl_module_id,String s);

#endif //MPL_PUBLIC_MOD_H

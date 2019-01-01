//
// Created by amindelavar on 12/24/2018.
//

#include<MPL/system.h>
//***********************************************
int32 call_module_funcs(String mod_name,
                        String func_name,
                        str_list params,
                        str_list pars_type,
                        uint32 params_len,
                        str_list *rets_mod) {
  //-----------------------init vars
  uint32 rets_mod_len = 0;
  int32 ret_func_response = 0;
  //-----------------------check errors
  uint8 mod_id = return_module_id(mod_name);
  if (mod_id == 0) return 0;
  else if (mpl_modules_instance[mod_id] == 0) {
    exception_handler("not_load_module", __func__, mod_name, 0);
    return -1;
  }
  //-----------------------find module interface
  if (mod_id == FS_MODULE_ID) {
    ret_func_response = run_fs_mod_funcs(func_name, params, pars_type, params_len, &(*rets_mod));
  }
  //-----------------------return
  if (ret_func_response > 0)rets_mod_len = ret_func_response;
  return rets_mod_len;
}
//***********************************************
Boolean load_module_file(String path, uint32 line, String src) {
  //if module supported by mpl
  String mod_name = return_file_name_extension_path(path, 0);
  uint32 mpl_module_id = return_module_id(mod_name);
  if (mpl_module_id == 0) {
    print_error(line, "not_support_module", src, mod_name, 0, "load_module_file");
    return false;
  }
  //printf("@@:%s,%s\n", path, mod_name);
  //-----------------------------------
  #if WINDOWS_PLATFORM == 1
  // Get a handle to the DLL module.
  HINSTANCE hinstLib = LoadLibrary(TEXT(path));
  // If the handle is not valid
  if (hinstLib == NULL) {
    print_error(line, "can_not_load_module", src, path, 0, "load_module_file");
    return false;
  }
  //save module instance
  mpl_modules_instance[mpl_module_id] = hinstLib;
  //init module functions
  init_module_file_funcs(mpl_module_id);
  return true;
  //-----------------------------------
  #elif LINUX_PLATFORM == 1
  return true;
  #endif
}
//***********************************************
uint32 return_module_id(String module_name) {
  if (str_equal(module_name, "fs")) return FS_MODULE_ID;
  else if (str_equal(module_name, "os")) return OS_MODULE_ID;
  else if (str_equal(module_name, "sqlite3")) return SQLITE3_MODULE_ID;
  else if (str_equal(module_name, "math")) return MATH_MODULE_ID;
  else if (str_equal(module_name, "mgt")) return MGT_MODULE_ID;
  else if (str_equal(module_name, "strs")) return STRS_MODULE_ID;
  return 0;
}
//***********************************************
void init_module_file_funcs(uint8 module_id) {
  if (module_id == FS_MODULE_ID) init_fs_module_funcs();
  //TODO
}
//***********************************************
String call_module_type2(String func_name,uint32 mpl_module_id,String s){
  #if WINDOWS_PLATFORM==true
  FUNCTYPE2 Proc = (FUNCTYPE2) GetProcAddress(mpl_modules_instance[mpl_module_id],func_name);
  if (NULL != Proc){
    return (String)Proc(s);
  }
  #endif
  //TODO:error
  printf("ERR34\n");
  return 0;
}

//***********************************************

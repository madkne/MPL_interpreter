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
  } else if (mod_id == SQLITE_MODULE_ID) {
    ret_func_response = run_sqlite_mod_funcs(func_name, params, pars_type, params_len, &(*rets_mod));
//    printf("DDFSS:%s\n",*rets_mod[0]);
  }
  //-----------------------return
  if (ret_func_response > 0)rets_mod_len = ret_func_response;
  return rets_mod_len;
}
//***********************************************
Boolean load_module_file(String path, uint32 line, String src) {
  //if module supported by mpl
  String mod_name = return_file_name_extension_path(path, 0,true);
  uint32 mpl_module_id = return_module_id(mod_name);
  if (mpl_module_id == 0) {
    print_error(line, "not_support_module", src, mod_name, path, "load_module_file");
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
  str_list_append(&installed_modules,path,installed_modules_len++);
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
  else if (str_equal(module_name, "sqlite")) return SQLITE_MODULE_ID;
  else if (str_equal(module_name, "math")) return MATH_MODULE_ID;
  else if (str_equal(module_name, "mgt")) return MGT_MODULE_ID;
  else if (str_equal(module_name, "strs")) return STRS_MODULE_ID;
  return 0;
}
//***********************************************
void init_module_file_funcs(uint8 module_id) {
  if (module_id == FS_MODULE_ID) init_fs_module_funcs();
  else if (module_id == SQLITE_MODULE_ID) init_sqlite_module_funcs();
  //TODO
}
//***********************************************
String call_module_type1(String func_name,uint32 mpl_module_id){
  #if WINDOWS_PLATFORM == true
  FUNCTYPE1 Proc = (FUNCTYPE1) GetProcAddress(mpl_modules_instance[mpl_module_id], func_name);
  if (NULL != Proc) return (String) Proc();
  #endif
  //TODO:error
  printf("M#ERR343:%s,%i\n",func_name,mpl_module_id);
  return 0;
}
//***********************************************
/**
 * a function that can call a module function with a String and return a String
 * mod(String):String
 * @param func_name
 * @param mpl_module_id
 * @param s => get String
 * @return String
 */
String call_module_type2(String func_name, uint32 mpl_module_id, String s) {
  #if WINDOWS_PLATFORM == true
  FUNCTYPE2 Proc = (FUNCTYPE2) GetProcAddress(mpl_modules_instance[mpl_module_id], func_name);
  if (NULL != Proc) return (String) Proc(s);
  #endif
  //TODO:error
  printf("M#ERR34\n");
  return 0;
}
//***********************************************
/**
 * a function that can call a module function with a int32 and return a String
 * mod(int32):String
 * @param func_name
 * @param mpl_module_id
 * @param i => get int32
 * @return String
 */
String call_module_type3(String func_name, uint32 mpl_module_id,int32 i) {
  #if WINDOWS_PLATFORM == true
  FUNCTYPE3 Proc = (FUNCTYPE3) GetProcAddress(mpl_modules_instance[mpl_module_id], func_name);
  if (NULL != Proc) return (String) Proc(i);
  #endif
  //TODO:error
  printf("M#ERR374\n");
  return 0;
}
//***********************************************
/**
 * a function that can call a module function with a String and return a Boolean
 * mod(String):Boolean
 * @param func_name
 * @param mpl_module_id
 * @param s => get String
 * @return Boolean
 */
Boolean call_module_type4(String func_name, uint32 mpl_module_id,String s) {
  #if WINDOWS_PLATFORM == true
  FUNCTYPE4 Proc = (FUNCTYPE4) GetProcAddress(mpl_modules_instance[mpl_module_id], func_name);
  if (NULL != Proc) return (Boolean) Proc(s);
  #endif
  //TODO:error
  printf("M#ERR399\n");
  return 0;
}

//***********************************************
void append_to_mofu(uint32 id,
                    uint8 mod_id,
                    String func_name,
                    String params,
                    String returns) {
  /**
  * a=bool|str|num|struct|var[0] : value
  * aa=bool[?,..]|str[?,..]|num[?,..]|struct[?,..] : var
  * aa..=bool[?,..]|str[?,..]|num[?,..]|struct[?,..] : var,var,..
  */
  //----------------------------------init vars
  uint8 par_len = 0, ret_len = 0;
  mofu *q;
  //----------------------------------analyzing params,returns
  par_len = char_search_count(params, ';');
  ret_len = char_search_count(returns, ';');
  //----------------------------------append to mofu
  q = (mofu *) malloc(sizeof(mofu));
  if (q == 0) return;
  q->id = id;
  q->mod_id = mod_id;
  q->params_len = par_len;
  q->returns_len = ret_len;
  str_init(&q->func_name, func_name);
  str_init(&q->params, params);
  str_init(&q->returns, returns);
  q->next = 0;
  //=>if module is fs
  if (mod_id == FS_MODULE_ID) {
    if (fs_start == 0)
      fs_start = fs_end = q;
    else {
      fs_end->next = q;
      fs_end = q;
    }
  }
    //=>if module is sqlite3
  else if (mod_id == SQLITE_MODULE_ID) {
    if (sqlite_start == 0)
      sqlite_start = sqlite_end = q;
    else {
      sqlite_end->next = q;
      sqlite_end = q;
    }
  }
}

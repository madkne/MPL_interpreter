//
// Created by amindelavar on 12/24/2018.
//

#include<MPL/system.h>

//***************************************
mofu *fs_start = 0;
mofu *fs_end = 0;
//***************************************
int32 run_fs_mod_funcs(String func_name, str_list params, str_list pars_type, uint32 params_len, str_list *rets_mod) {
  //-----------------------init vars
  uint32 rets_mod_len = 0;
  uint32 func_id = 0;
  str_list argvs = 0;
  long_int var0_ind = 0;
  //-----------------------search for fs function
  mofu *st = fs_start;
  for (;;) {
    //if func_name is true
    if (str_equal(st->func_name, func_name)) {
      Boolean is_exist = check_built_in_module_function(st->params, pars_type, params_len);
      if (is_exist) {
        func_id = st->id;
        break;
      }
    }
    st = st->next;
    if (st == 0) break;
  }
  //-----------------------check and convert
  if (func_id == 0)return 0;
  //convert vars to values
  convert_built_in_module_vars_to_values(pars_type, params, params_len, &argvs, &var0_ind);
//  printf("SSSS:%i,%s,%s\n", func_id, st->func_name, st->params);
  //-----------------------call function
  if (func_id == 1/*version*/) {
    str_list_append(&(*rets_mod), convert_to_string(FS_VERSION_INTERFACE), rets_mod_len++);
  } else if (func_id == 2/*LibVersion*/) {
    String ret = call_module_type1("fs_mod__LibVersion", FS_MODULE_ID);
    str_list_append(&(*rets_mod), convert_to_string(ret), rets_mod_len++);
  } else if (func_id == 3/*AbsPath*/) {
    String path = convert_mplpath_to_abspath(argvs[0]);
    String ret = __syscall_abspath(path);
    str_list_append(&(*rets_mod), convert_to_string(ret), rets_mod_len++);
  } else if (func_id == 4/*mkdir*/) {
    String path = convert_mplpath_to_abspath(argvs[0]);
    Boolean ret = call_module_type4("fs_mod__mkdir", FS_MODULE_ID, path);
    str_list_append(&(*rets_mod), str_from_bool(ret), rets_mod_len++);
  }
  //-----------------------return
  return rets_mod_len;
}
//***************************************
void init_fs_module_funcs() {
  append_to_mofu(0, FS_MODULE_ID, 0, 0, 0);
  append_to_mofu(1, FS_MODULE_ID, "version", 0, "str");
  append_to_mofu(2, FS_MODULE_ID, "LibVersion", 0, "str");
  append_to_mofu(3, FS_MODULE_ID, "AbsPath", "str", "str");
  append_to_mofu(4, FS_MODULE_ID, "mkdir", "str", "bool");
}

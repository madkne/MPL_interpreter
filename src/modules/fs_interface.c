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
//  mofu *tmp1 = fs_start;
//  if (tmp1 == 0) printf("--NULL fs functions--\n");
//  else
//    for (;;) {
//      if (tmp1->id != 0)
//        printf("FS_func[%i]:%s %s\n", tmp1->id, tmp1->func_name, tmp1->params);
//      tmp1 = tmp1->next;
//      if (tmp1 == 0) break;
//    }
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
  convert_built_in_module_vars_to_values(pars_type,params,params_len,&argvs,&var0_ind);
//  printf("SSSS:%i,%s,%s\n", func_id, st->func_name, st->params);
  //-----------------------call function
  if(func_id==1/*version*/){
    //TODO:
  }else if(func_id==2/*abspath*/){
    #if WINDOWS_PLATFORM==true
      String ret=call_module_type2("fs_mod__abspath",FS_MODULE_ID,"sd");
      printf("abspath:%s=>%s\n",argvs[0],ret);
    #endif
  }
  //-----------------------return
  return rets_mod_len;
}
//***************************************
void init_fs_module_funcs() {
  append_fs_mofu(0, FS_MODULE_ID, 0, 0, 0);
  append_fs_mofu(1, FS_MODULE_ID, "Version", 0, "str");
  append_fs_mofu(2, FS_MODULE_ID, "abspath", "str", "str");
}
//***************************************
void append_fs_mofu(uint32 id,
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
  if (fs_start == 0) {
    fs_start = fs_end = q;
  } else {
    fs_end->next = q;
    fs_end = q;
  }
}
//
// Created by amindelavar on 11/9/2018.
//
#include <MPL/system.h>

String _MPL_TYPE__len(String partype) {
  //=>init vars
  str_list partype1 = 0;
  str_list store = 0;
  uint32 storelen = 0;
  //--------------------------
  //=>split partype to partype1
  char_split(partype, ';', &partype1, true);
  //=>if is a value
  if (str_ch_equal(partype1[2], 0)) {
    storelen = char_split(partype1[3], ',', &store, true);
  }
    //=>if is var or ref var
  else {
    long_int po_id = get_Mvar(str_to_long_int(partype1[1])).pointer_id;
    storelen = return_array_dimensions(po_id, &store);

  }
  //=>return!
  if (storelen == 0)return "null";
  return str_multi_append("{", str_join(store, storelen, ","), "}", 0, 0, 0);
}
//************************************************
/**
 * process just atomic values like: num,str,bool and for struct return null. in public it return 's' for str or 'b' for bool
 * @param value
 * @param type
 * @return String
 * @samples typeof(45.78)=>'f'||typeof(s[1])=>'s'
 */
String _MPL_TYPE__typeof(String value, String type) {
  if (str_equal(type, "num")) {
    return char_to_str(determine_type_num(value));
  } else if (str_equal(type, "str"))return "s";
  else if (str_equal(type, "bool")) return "b";
  return "null";
}
//************************************************
/**
 * push to a variable a value(str,num,struct,..) and return var items count
 * @param var_ind
 * @param value
 * @param val_type
 * @param wh
 * @return uint32
*/
uint32 _MPL_TYPE__push(long_int var_ind, String value, String val_type, String wh) {
  if (var_ind == 0) {
    //TODO:error
    return 0;
  }
//  printf("***rt:%i,%s,%s\n", var_ind, value, wh);
  //get Mvar
  Mvar var = get_Mvar(var_ind);
  //----------------------calculate index of var
  str_list c1 = 0;
  uint32 c1_len = return_array_dimensions(var.pointer_id, &c1);
  uint32 var_len = (uint32) str_to_long_int(c1[0]);
  //check if var has one dimension
  if (c1_len > 1) {
    //TODO:error
    return 0;
  }
  //----------------------if value type and var type is not equal
  String var_type = get_datas(var.type_var).name;
  if (!str_equal(val_type, var_type) && !str_equal(val_type, "null")) {
    //TODO:error
    return var_len;
  }
  //----------------------get first room pointer id
  long_int first_po_id = var.pointer_id;
  String first_room = get_Mpoint(first_po_id).data;
  //determine if struct type
  Boolean is_struct = false;
  if (!str_search(basic_types, var_type, StrArraySize(basic_types))) is_struct = true;
  //----------------------create new value pointer
  long_int new_pointer_id = 0;
  if (is_struct) {
//    print_vaar(get_stde(str_to_long_int(value)).st);
    new_pointer_id = set_struct_node_Mpoint(get_stde(str_to_long_int(value)).st);
  } else {
    uint8 sub_type = '0';
    if (str_equal(val_type, "num"))sub_type = determine_type_num(value);
    else if (str_equal(val_type, "str"))sub_type = 's';
    else if (str_equal(val_type, "bool")) sub_type = 'b';
    new_pointer_id = add_to_pointer_memory(value, sub_type);
  }
  //----------------------start of pushing
  if (str_equal(wh, LEFT_DIRECT)) {
    edit_Mpoint(first_po_id,
                str_multi_append(str_from_long_int(new_pointer_id), ";", first_room, 0, 0, 0), 0, true, false);
    var_len++;
  } else if (str_equal(wh, RIGHT_DIRECT)) {
    //printf("****%i,%s,%i\n\n", first_po_ind, first_room, new_pointer_id);
    edit_Mpoint(first_po_id,
                str_multi_append(first_room, ";", str_from_long_int(new_pointer_id), 0, 0, 0), 0, true, false);
    var_len++;
  } else {
    //TODO:warning
  }
  return var_len;
}
//************************************************
/**
 * crop an array by min,max numbers and return croped var or null
 * @param var_ind
 * @param min
 * @param max
 * @return string
*/
String _MPL_TYPE__crop(long_int var_ind, uint32 min, uint32 max) {
  //=>if min>max, then return null
  if (min > max) return "null";
  //=>if min is zero
  if (min == 0) {
    //TODO:error
    printf("ERR_mb5768888\n");
    return "null";
  }
  //=>init vars
  str_list ret = 0, pointers;
  uint32 ret_len = 0;
  //----------------------------------
//  printf("***rt:%i,%s,%s\n", var_ind, value, wh);
  //=>get Mvar
  Mvar var = get_Mvar(var_ind);
  //=>get dimensions of var
  str_list c1 = 0;
  uint32 c1_len = return_array_dimensions(var.pointer_id, &c1);
  //=>check if var has one dimension,no more!
  if (c1_len > 1) {
    //TODO:error
    return 0;
  }
  //=>get length of var
  uint32 var_len = (uint32) str_to_long_int(c1[0]);
  //=>check max is smaller or equal with var length,then ignore it!
  if (max > var_len)max = var_len;
  //=>check if min>max,then return null!
  if (min > max) return "null";
  //=>get list of room pointers
  long_int po_id = var.pointer_id;
  char_split(get_Mpoint(po_id).data, ';', &pointers, true);
  //=>collect all nodes between min,max
  for (uint32 i = min - 1; i < max; i++) {
    str_list_append(&ret, __return_value_var_complete(str_to_long_int(pointers[i])), ret_len++);
  }
  //=>return if ret is an array
  if (ret_len > 1)return str_multi_append("{", str_join(ret, ret_len, ","), "}", 0, 0, 0);
  //=>return if exist a value
  if (ret_len == 1)return ret[0];
  //=>else return null
  return "null";
}

//************************************************
/**
 * get var_index of variable and delete var and removes it from VM and then return true
 * @param var_ind
 * @return Boolean
 */
Boolean _MPL_TYPE__del(long_int var_ind) {
  return delete_full_memory_var(var_ind, true);
}
//************************************************
Boolean _MPL_TYPE__error_handle(int8 err_type, String err_name, String err_des) {
  return exception_user_handler(err_type, err_name, err_des, get_func_by_id(entry_table.cur_fid).lbl);
}
//************************************************
/**
 * get a variable or value and return its main type like for a num value or variable return 'num' and return its indexes as {"num","1"}
 * @param value
 * @param type
 * @return String
 * @samples var_type(45.78)=>{"num","1"}||var_type(s)=>{"str",1}||var_type(struct(57.8,"rt"))=>{"st1","1"} ||var_type({{4,6},{6,9},{9,9}}=>{"num","3,2"}
 */
String _MPL_TYPE__var_type(String value, String type) {
  return type;
}
//************************************************
String _MPL_TYPE__mm_all(uint8 type) {
  //=>init vars
  String ret = 0;
  str_list tmp = 0;
  uint32 tmplen = 0;
  //---------------------------
  //=>get start point of mama
  mama *tmp1 = entry_table.mama_start;
  //=>if no any date, so return null
  if (tmp1 == 0) return "null";
  //=>store all mm keys to tmp str_list
  for (;;) {
    if (tmp1->type == type) {
      str_list_append(&tmp, convert_to_string(tmp1->key), tmplen++);
    }
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  //=>create ret and return it!
  ret = str_multi_append("{", str_join(tmp, tmplen, ","), "}", 0, 0, 0);
  return ret;
}
//************************************************
Boolean _MPL_TYPE__mm_isset(uint8 type, String key) {
  mama ret = get_mama(type, key);
  if (ret.id == 0)return false;
  return true;
}
//************************************************
Boolean _MPL_TYPE__exec(String cmd) {
  Boolean is_done = true;
  cmd = replace_control_chars(cmd);
  if (str_equal(cmd, "null")) return true;
  while ((cmd = str_trim_space(cmd)) != 0) {
    uint8 state = labeled_instruction(cmd);
    if (is_programmer_debug >= 1) {
      printf("@###############INST_EXEC(fid:%i,sid:%i,order:%i,state:%i,fin:%i,line:%i):\n%s\n", entry_table.cur_fid,
             entry_table.cur_sid, entry_table.Rorder, state, entry_table.cur_fin, entry_table.Rline, cmd);
    }
    //---------------------analyzing all states
    if (state == UNKNOWN_LBL_INST) {
      is_done = false;
      if (str_equal(cmd, "null")) return true;
      exception_handler("unknown_instruction", __func__, cmd, "");
    } else if (state == DEF_VARS_LBL_INST) cmd = define_vars(cmd);
    else if (state == ALLOC_MAGIC_MACROS_LBL_INST)cmd = alloc_magic_macros(cmd);
    else if (state == FUNC_CALL_LBL_INST)cmd = function_call(cmd);
    else if (state == ALLOC_VARS_LBL_INST) return vars_allocation(cmd);
    else if (state == ALLOC_SHORT_LBL_INST) cmd = vars_allocation_short(cmd);
    else if (state == NEXT_BREAK_LBL_INST) return structure_loop_next_break(cmd);
    else is_done = false;
    if (str_equal(cmd, BAD_CODE)) is_done = false;
    else if (entry_table.is_occur_error_exception) is_done = false;
    //********************
    if (!is_done) break;
  }
  return is_done;
}


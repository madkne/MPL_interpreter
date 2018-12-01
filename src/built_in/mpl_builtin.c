//
// Created by amindelavar on 11/9/2018.
//
#include <MPL/system.h>

//************************************************
/**
 * print any value or variable and return true if successful
 * @param items
 * @param len
 * @return Boolean
 * @samples print(s,"\n")||print({4,8.7},"er"+"AS",67.5};
 */
Boolean _MPL_TYPE__print(str_list items, uint32 len) {
//  printf("!!!!:%s(%i)\n",print_str_list(items,len),len);
  for (uint32 i = 0; i < len; i++) {
    printf(replace_control_chars(items[i]));
  }

  return true;
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
String _MPL_TYPE__input(String type) {
  if (str_equal(type, READ_CHAR_INPUT_TYPE)) {
    return str_trim_space(char_to_str(fgetc(stdin)));
  } else if (str_equal(type, READ_LINE_INPUT_TYPE)) {
    return read_input();
  }
  return 0;
}
//************************************************
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
  uint32 c1_len = return_var_dimensions(var_ind, &c1);
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
  //----------------------get first room pointer index
  long_int first_po_ind = find_index_pointer_memory(var.pointer_id);
  String first_room = get_Mpoint(first_po_ind).data;
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
    edit_Mpoint(first_po_ind,
                str_multi_append(str_from_long_int(new_pointer_id), ";", first_room, 0, 0, 0),
                0,
                true,
                false);
    var_len++;
  } else if (str_equal(wh, RIGHT_DIRECT)) {
    //printf("****%i,%s,%i\n\n", first_po_ind, first_room, new_pointer_id);
    edit_Mpoint(first_po_ind,
                str_multi_append(first_room, ";", str_from_long_int(new_pointer_id), 0, 0, 0),
                0,
                true,
                false);
    var_len++;
  } else {
    //TODO:warning
  }
  return var_len;
}
//************************************************
//************************************************
//************************************************
/**
 * get a variable or value and return its main type like for a num value or variable return just 'num'
 * @param value
 * @param type
 * @return String
 * @samples var_type(45.78)=>'num'||var_type(s)=>'str'||var_type(struct(57.8,"rt"))=>'st1'
 */
String _MPL_TYPE__var_type(String value, String type) {
  return type;
}


//
// Created by amindelavar on 1/9/2019.
//

#include <MPL/system.h>

String _DATA_TYPE__bit_operations(uint8 type, String n1, String n2) {
  int32 ret = 0;
  int32 n22 = 0;
  int32 n11 = str_to_int32(n1);
  if (n2 != 0) n22 = str_to_int32(n2);
  if (type == 'a')ret = n11 & n22;
  else if (type == 'o')ret = n11 | n22;
  else if (type == 'x')ret = n11 ^ n22;
  else if (type == 'n')ret = ~n11;
  else if (type == 'r')ret = n11 >> n22;
  else if (type == 'l')ret = n11 << n22;
  return str_from_int32(ret);
}
//***********************************
String _DATA_TYPE__at(String value, String ind) {
  value = replace_control_chars(value);
  int32 indd = str_to_int32(ind);
  if (indd < 0 || indd >= str_length(value)) {
    exception_user_handler(ERROR_ID, "bad_index", "index not valid", "at");
    return 0;
  }
  return set_valid_control_chars(char_to_str(value[indd]));
}
//***********************************
String _DATA_TYPE__into(String value, String ind, String rep) {
  //=>set valid value,rep
  value = replace_control_chars(value);
  rep = replace_control_chars(rep);
  //=>calc length of value,rep
  uint32 vallen = str_length(value);
  uint32 replen = str_length(rep);
  //=>convert ind to integer
  int32 indd = str_to_int32(ind);
  //=>if indd is not valid as index
  if (indd < 0 || indd >= vallen) {
    exception_user_handler(ERROR_ID, "bad_index", "index not valid", "into");
    return 0;
  }
  //=>get substring of value before indd
  String before = str_substring(value, 0, indd);
  //=>get after of value minus rep
  uint32 st = (vallen - indd) - replen; //count rest chars of value
  String after = 0;
  if (st > 0) {
    st = vallen - st;
    after = str_substring(value, st, vallen);
  }
  //=>return result
  String ret = str_multi_append(before, rep, after, 0, 0, 0);
  return set_valid_control_chars(ret);

}
//************************************************
String _DATA_TYPE__tnum(String type, String value) {
  if (str_equal(type, "str") && str_is_num(value)) return value;
  if (str_equal(type, "bool")) {
    if (value[0] == 't')return "1";
    return "0";
  }
  if (str_equal(type, "num"))return value;
  exception_user_handler(ERROR_ID, "bad_input", "can not convert input to a num value", "tnum");
  return "0";
}
//************************************************
String _DATA_TYPE__tstr(String type, String value) {
  if (str_equal(type, "str") || str_equal(type, "num") || str_equal(type, "bool"))
    return value;
  exception_user_handler(ERROR_ID, "bad_input", "can not convert input to a str value", "tstr");
  return "";
}
//************************************************
Boolean _DATA_TYPE__tbool(String type, String value) {
  if (str_equal(type, "num")) {
    int32 num = str_to_int32(value);
    if (num == 0)return false;
    return true;
  }
  if (str_equal(type, "str") && str_is_bool(value)) return str_to_bool(value);
  if (str_equal(type, "bool"))return str_to_bool(value);

  exception_user_handler(ERROR_ID, "bad_input", "can not convert input to a bool value", "tbool");
  return false;
}
//************************************************
String _DATA_TYPE__tarray(String type, String value) {
  //=>init vars
  String ret = 0;
  uint32 len = 0;
  //-------------------------------
  if (str_equal(type, "bool")) {
    if (value[0] == 't')str_init(&ret, "\"t\"");
    str_init(&ret, "\"f\"");
  } else if (str_equal(type, "str")) {
    value = replace_control_chars(value);
    len = str_length(value);
    for (uint32 i = 0; i < len; i++) {
      String tmp = set_valid_control_chars(char_to_str(value[i]));
      ret = str_multi_append(ret, "\"", tmp, "\"", 0, 0);
      if (i + 1 < len)ret = char_append(ret, ',');
    }
//    printf("FFFFF:%s=>%s\n", value, ret);
  } else if (str_equal(type, "num")) {
    len = str_length(value);
    for (uint32 i = 0; i < len; i++) {
      ret = str_multi_append(ret, "\"", char_to_str(value[i]), "\"", 0, 0);
      if (i + 1 < len)ret = char_append(ret, ',');
    }
  } else {
    exception_user_handler(ERROR_ID, "bad_input", "can not convert input to str array", "tarray");
  }
  return str_multi_append("{", ret, "}", 0, 0, 0);
}
//************************************************
Boolean _DATA_TYPE__inum(String value) {
  if (str_is_num(value))return true;
  return false;
}
//************************************************
String _DATA_TYPE__dbslah(String val) {
  uint32 len = str_length(val);
  if (len < 1)return val;
  String ret = 0;
  for (uint32 i = 0; i < len; i++) {
    if (val[i] == '\\' && i + 1 < len) {
      ret = str_append(ret, "\\\\");
    } else ret = char_append(ret, val[i]);
  }
//  printf("Slash:%s=>%s\n", val, ret);
  return ret;
}
//************************************************
Boolean _DATA_TYPE__ibool(String value) {
  if (str_is_bool(value))return true;
  return false;
}
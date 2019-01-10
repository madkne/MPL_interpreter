//
// Created by amindelavar on 1/9/2019.
//

#include <MPL/system.h>

String _DATA_TYPE__bit_operations(uint8 type,String n1, String n2) {
  int32 ret=0;
  int32 n22=0;
  int32 n11 = str_to_int32(n1);
  if(n2!=0) n22 = str_to_int32(n2);
  if(type=='a')ret= n11 & n22;
  else if(type=='o')ret= n11 | n22;
  else if(type=='x')ret= n11 ^ n22;
  else if(type=='n')ret= ~n11;
  else if(type=='r')ret= n11 >> n22;
  else if(type=='l')ret= n11 << n22;
  return str_from_int32(ret);
}

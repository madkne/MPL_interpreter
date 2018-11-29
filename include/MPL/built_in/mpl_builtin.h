//
// Created by amindelavar on 11/9/2018.
//

#ifndef MPL_MPL_BUILTIN_H
#define MPL_MPL_BUILTIN_H

Boolean _MPL_TYPE__print (str_list items, uint32 len);
String _MPL_TYPE__typeof (String value, String type);
String _MPL_TYPE__input (String type);


uint32 _MPL_TYPE__push(long_int var_ind, String value,String val_type, String wh);

String _MPL_TYPE__var_type (String value, String type);
#endif //MPL_MPL_BUILTIN_H

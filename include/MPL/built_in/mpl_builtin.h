//
// Created by amindelavar on 11/9/2018.
//

#ifndef MPL_MPL_BUILTIN_H
#define MPL_MPL_BUILTIN_H

String _MPL_TYPE__typeof (String value, String type);


uint32 _MPL_TYPE__push(long_int var_ind, String value,String val_type, String wh);
Boolean _MPL_TYPE__error_handle(int8 err_type,String err_name,String err_des);
String _MPL_TYPE__var_type (String value, String type);
#endif //MPL_MPL_BUILTIN_H

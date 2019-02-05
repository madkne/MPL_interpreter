//
// Created by amindelavar on 1/9/2019.
//

#ifndef MPL_DATA_BUILTIN_H
#define MPL_DATA_BUILTIN_H

String _DATA_TYPE__bit_operations(uint8 type, String n1, String n2);
String _DATA_TYPE__at(String value, String ind);
String _DATA_TYPE__into(String value, String ind, String rep);
String _DATA_TYPE__tnum(String type, String value);
String _DATA_TYPE__tstr(String type, String value);
Boolean _DATA_TYPE__tbool(String type, String value);
String _DATA_TYPE__tarray(String type, String value);
Boolean _DATA_TYPE__inum(String value);
Boolean _DATA_TYPE__ibool(String value);
#endif //MPL_DATA_BUILTIN_H

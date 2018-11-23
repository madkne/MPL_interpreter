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
Boolean _MPL_TYPE__print (str_list items, uint32 len)
{
  for (uint32 i = 0; i < len; i++)
	{
	  printf (replace_control_chars (items[i]));
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
String _MPL_TYPE__typeof (String value, String type)
{
  if (str_equal (type, "num"))
	{
	  return char_to_str (determine_type_num (value));
	}
  else if (str_equal (type, "str"))return "s";
  else if (str_equal (type, "bool")) return "b";
  return "null";
}
//************************************************
String _MPL_TYPE__input (String type)
{
  if (str_equal (type, READ_CHAR_INPUT_TYPE))
	{
	  return str_trim_space (char_to_str (fgetc (stdin)));
	}
  else if (str_equal (type, READ_LINE_INPUT_TYPE))
	{
	  return read_input ();
	}
  return 0;
}
//************************************************
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
String _MPL_TYPE__var_type (String value, String type)
{
  return type;
}


//
// Created by amindelavar on 11/9/2018.
//
#include <MPL/system.h>

Boolean _MPL_TYPE__print (str_list items, uint32 len)
{
  for (uint32 i = 0; i < len; i++)
	{
	  printf (replace_control_chars (items[i]));
	}

  return true;
}


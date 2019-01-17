//
// Created by amindelavar on 12/2/2018.
//

#ifndef MPL_MAGIC_MACRO_H
#define MPL_MAGIC_MACRO_H

Boolean edit_magic_macro(uint8 type,String key,String value);
Boolean edit_magic_config(String key,String value,String val_type);
void print_magic_macros(uint8 wh);
void init_magic_define();
void init_magic_config();
Boolean flush_session_entries();
Boolean load_session_entries();
#endif //MPL_MAGIC_MACRO_H

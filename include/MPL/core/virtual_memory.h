//
// Created by amindelavar on 9/22/2018.
//

#ifndef MPL_VIRTUAL_MEMORY_H
#define MPL_VIRTUAL_MEMORY_H

void init_memory();

void append_Mvar(Mvar s);

void append_Mpoint(Mpoint s);

Mvar get_Mvar(long_int i);

Mpoint get_Mpoint(long_int i);

Mpoint return_var_memory_value(String var_name);

long_int get_data_memory_index(long_int pointer_id, String index_var);

long_int
set_memory_var(long_int fin, long_int sid, String name, String value_var, String type_var, Boolean is_create_var);

long_int
add_to_var_memory(long_int pointer_id, long_int fin, long_int sid, long_int type_var, String name, String extra);

long_int add_to_pointer_memory(String data, uint8 type_data);

void show_memory(uint8 wh);
Boolean delete_pointer_memory(long_int id);
long_int find_index_pointer_memory(long_int id);

long_int find_index_var_memory(long_int id);

long_int return_var_ind_pointer_id(long_int pointer_id);

#endif //MPL_VIRTUAL_MEMORY_H

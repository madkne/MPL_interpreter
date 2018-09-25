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

long_int find_index_pointer_memory(long_int id);

long_int find_index_var_memory(long_int id);

long_int return_var_ind_pointer_id(long_int pointer_id);

#endif //MPL_VIRTUAL_MEMORY_H

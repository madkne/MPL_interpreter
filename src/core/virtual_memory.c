//
// Created by amindelavar on 9/22/2018.
//

#include <MPL/system.h>

void init_memory() {
	Mvar tmp1 = {0, 0, 0, 0, 0, 0, 0, 0};
	append_Mvar(tmp1);
	Mpoint tmp2 = {0, 0, '0'};
	append_Mpoint(tmp2);
	Mpoint tmp3 = {0, 0, 'v'};
	append_Mpoint(tmp3);
}

//*****************************
void append_Mvar(Mvar s) {
	Mvar *q;
	q = (Mvar *) malloc(sizeof(Mvar));
	if (q == 0) return;
	q->id = s.id;
	q->func_index = s.func_index;
	q->pointer_id = s.pointer_id;
	q->stru_index = s.stru_index;
	q->type_var = s.type_var;
	str_init(&q->name, s.name);
	str_init(&q->extra, s.extra);
	q->next = 0;
	if (entry_table.var_memory_start == 0)
		entry_table.var_memory_start = entry_table.var_memory_end = q;
	else {
		entry_table.var_memory_end->next = q;
		entry_table.var_memory_end = q;
	}
}

//*****************************
void append_Mpoint(Mpoint s) {
	Mpoint *q;
	q = (Mpoint *) malloc(sizeof(Mpoint));
	if (q == 0) return;
	q->id = s.id;
	q->type_data = s.type_data;
	str_init(&q->data, s.data);
	q->next = 0;
	if (entry_table.pointer_memory_start == 0)
		entry_table.pointer_memory_start = entry_table.pointer_memory_end = q;
	else {
		entry_table.pointer_memory_end->next = q;
		entry_table.pointer_memory_end = q;
	}
}
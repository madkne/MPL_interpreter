//
// Created by amindelavar on 9/22/2018.
//

#include <MPL/system.h>

void init_memory() {
	entry_table.var_memory_start = 0;
	entry_table.pointer_memory_start = 0;
	entry_table.var_mem_id = 0;
	entry_table.pointer_mem_id = 0;
	entry_table.var_mem_len = 0;
	entry_table.pointer_mem_len = 0;
	//----------------------------
	Mvar tmp1 = {0, 0, 0, 0, 0, 0, 0, 0};
	append_Mvar(tmp1);
	Mpoint tmp2 = {0, 0, '0'};
	append_Mpoint(tmp2);
	Mpoint tmp3 = {0, 0, 'v'};
	append_Mpoint(tmp3);
}

//****************************************************
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
	entry_table.var_mem_len++;
	if (entry_table.var_memory_start == 0)
		entry_table.var_memory_start = entry_table.var_memory_end = q;
	else {
		entry_table.var_memory_end->next = q;
		entry_table.var_memory_end = q;
	}
}

//****************************************************
void delete_Mvar(long_int i) {
	long_int counter = 0;
	Mvar *first = 0;
	Mvar *last = entry_table.var_memory_start;
	for (;;) {
		if (counter == i) {
			Mvar *next = last->next;
			free(last);
			if (i == 0) entry_table.var_memory_start = next; //if was first node
			else first->next = next;
			if (next == 0)entry_table.var_memory_end = first; //if was last node
			break;
		}
		first = last;
		last = last->next;
		if (last == 0) break;
		counter++;
	}
	entry_table.var_mem_len--;
}

//****************************************************
void append_Mpoint(Mpoint s) {
	Mpoint *q;
	q = (Mpoint *) malloc(sizeof(Mpoint));
	if (q == 0) return;
	q->id = s.id;
	q->type_data = s.type_data;
	str_init(&q->data, s.data);
	q->next = 0;
	entry_table.pointer_mem_len++;
	if (entry_table.pointer_memory_start == 0)
		entry_table.pointer_memory_start = entry_table.pointer_memory_end = q;
	else {
		entry_table.pointer_memory_end->next = q;
		entry_table.pointer_memory_end = q;
	}
}

//****************************************************
void delete_Mpoint(long_int i) {
	long_int counter = 0;
	Mpoint *first = 0;
	Mpoint *last = entry_table.pointer_memory_start;
	for (;;) {
		if (counter == i) {
			Mpoint *next = last->next;
			free(last);
			if (i == 0) entry_table.pointer_memory_start = next; //if was first node
			else first->next = next;
			if (next == 0)entry_table.pointer_memory_end = first; //if was last node
			break;
		}
		first = last;
		last = last->next;
		if (last == 0) break;
		counter++;
	}
	entry_table.pointer_mem_len--;
}

//****************************************************
Mvar get_Mvar(long_int i) {
	Mvar null = {0, 0, 0, 0, 0, 0, 0, 0};
	long_int counter = 0;
	Mvar *tmp1 = entry_table.var_memory_start;
	for (;;) {
		if (i == counter) return (*tmp1);
		tmp1 = tmp1->next;
		counter++;
		if (tmp1 == 0) break;
	}
	return null;
}

//****************************************************
Mpoint get_Mpoint(long_int i) {
	Mpoint null = {0, 0, 0, 0};
	long_int counter = 0;
	Mpoint *tmp1 = entry_table.pointer_memory_start;
	for (;;) {
		if (i == counter) return (*tmp1);
		tmp1 = tmp1->next;
		counter++;
		if (tmp1 == 0) break;
	}
	return null;
}

//****************************************************
/**
 * get a name(and index) of var and return a Mpoint struct of var by its index
 * @param var_name
 * @return Mpoint
 */
Mpoint return_var_memory_value(String var_name) {
	/**
	1- s[1,9]
	2- st1
	Samples:
	1- num nu=d1[0,1]+d1[1,0]
	2- str st2={"Amin",st1[0]*2+"!"}
	*/
	//msg("&VVVVV:", var_name)
	//**************************define variables
	String var_index = "0";
	uint8 type_var = '0';
	long_int pointer_id = 0;
	Mpoint failed = {0, 0, '0', 0};
	//var tmp_name = var_name
	//**************************start analyzing
	//**************************get name and index
	return_name_index_var(var_name, false, &var_name, &var_index);
	//**************************get value of var
	//----------------search for variable
	long_int tmpx = return_var_id(var_name, var_index);
	long_int real_id = find_index_var_memory(tmpx);
	//msg("&XXXXX:", var_name, var_index, real_id)
	//show_memory(0)
	if (real_id == 0) {
		//msg("CXCXC:", var_name, "=>", value_var)
		return failed;
	}
	
	pointer_id = get_Mvar(real_id).pointer_id;
	//msg("III:", pointer_id)
	//----------------search for data in index
	long_int data_ind = get_data_memory_index(pointer_id, var_index);
	//**************************return
	//msg("CXCXC:", var_name, "=>", value_var)
	return get_Mpoint(data_ind);
}
//****************************************************
/**
 * get a pointer_id of var and index of var and return an index of Mpoint
 * @param pointer_id
 * @param index_var
 * @return long_int
 */
long_int get_data_memory_index(long_int pointer_id, String index_var) {
	//**************************define variables
	long_int ret_index = 0;
	long_int basic_pointer = pointer_id;
	//**************************start analyzing
	//msg("&III:", pointer_id, index_var)
	//show_memory(40)
	//----------------search for data in index
	str_list tmp1 = 0;
	uint32 tmp1_len = char_split(index_var, ',', &tmp1, true);
	for (uint32 i = 0; i < tmp1_len + 1; i++) {
		long_int po_ind = find_index_pointer_memory(pointer_id);
		if (po_ind == 0) {
			return ret_index;
		}
		//msg("RTE:", pointer_memory[po_ind], po_ind)
		uint32 user_ind = 0;
		if (i < tmp1_len) {
			user_ind = str_to_int32(tmp1[i]);
		}
		Mpoint mpoint = get_Mpoint(po_ind);
		if (mpoint.type_data == 'p') {
			str_list tmp2 = 0;
			uint32 tmp2_len = char_split(mpoint.data, ';', &tmp2, true);
			if (user_ind >= tmp2_len || user_ind < 0) {
				String var_name = 0;
				str_init(&var_name, get_Mvar(return_var_ind_pointer_id(basic_pointer)).name);
				exception_handler("out_of_range_index", "get_data_memory_index", index_var, var_name);
				return 0;
			}
			pointer_id = str_to_long_int(tmp2[user_ind]);
		} else if (mpoint.type_data != 'v' && mpoint.type_data != 'p' || mpoint.type_data != '0') {
			//msg("&YY", i, tmp1, len(tmp1), index_var)
			if (i < tmp1_len) {
				String var_name = 0;
				str_init(&var_name, get_Mvar(return_var_ind_pointer_id(basic_pointer)).name);
				exception_handler("out_of_range_index", "get_data_memory_index", index_var, var_name);
				return 0;
			}
			return po_ind;
			//msg("CCXXXX:", user_ind, pointer_memory[po_ind].data_ids, data_ind)
		}
	}
	return 0;
}

//****************************************************
long_int
set_memory_var(long_int fin, long_int sid, String name, String value_var, String type_var, Boolean is_create_var) {
	/**
	1- str st1=fi.name
	2- str st2=fg //fg={"sd","ER"} ---OK---
	3- str r=er ---OK---
	4- str u="RT"*2 ---OK---
	5- num j=j1 //num j1=56d => j=56d
	*/
	//printf("&@@@:%s,%i,%s\n", name, return_var_id(name, "0"), value_var);
	//*******************************************if value is a variable
	if (is_valid_name(value_var, true)) {
		String al_name = 0, al_index = 0;
		return_name_index_var(value_var, true, &al_name, &al_index);
		//msg("&!!!", value_var, al_index)
		//----------if is a variable
		if (al_index == 0) {
			//msg("&NNNN", value_var, return_var_id(value_var, "0"), cur_fin)
			long_int ret0 = return_var_id(value_var, "0");
			if (ret0 == 0) {
				exception_handler("not_exist_var", __func__, value_var, "");
				return 0;
			}
			//show_memory(40)
			value_var = return_value_var_complete(find_index_var_memory(ret0));
			//msg("&NNN+++",value_var,find_index_var_memory(return_var_id(value_var, "0")))
		}
		//----------if is a data variable
		if (al_index != 0) {
			//msg("&UUU")
			uint8 ty = '0';
			Mpoint point = return_var_memory_value(value_var);
			value_var = point.data;
			if (point.type_data == 's') {
				value_var = convert_to_string(value_var);
			} else if (point.type_data == 'p' || point.type_data == '0') {
				return 0;
			}
		}
	}
	//msg("&IIIII", value_var)
	//*******************************************define variables
	value_var = str_trim_space(value_var);
	vaar_en vals_array = {0, 0, 0};
	//int32 set_indexes[MAX_ARRAY_DIMENSIONS + 1];
	int32 max_indexes[MAX_ARRAY_DIMENSIONS];//get user indexes
	Boolean is_multi_array = false, is_struct = false;
	uint8 indexes_len = 0;//count of user indexes dimensions
	//*******************************************if type is a struct
	if (search_datas(type_var, entry_table.cur_fid, false).id != 0 &&
			!str_search(basic_types, type_var, StrArraySize(basic_types))) {
		is_struct = true;
	}
	//*******************************************analyzing name of variable
	String tmp_name = 0, tmp_ind = 0;
	return_name_index_var(name, true, &tmp_name, &tmp_ind);
	//printf("CCCCCCC:%s=>%s,%s\n",name,tmp_name, tmp_ind);
	//is_multi array
	if (tmp_ind != 0 && !str_equal(tmp_ind, "0")) {
		is_multi_array = true;
		str_list indexes;
		indexes_len = (uint8) char_split(tmp_ind, ',', &indexes, true);
		if (indexes_len > MAX_ARRAY_DIMENSIONS) {
			//TODO:error
			return 0;
		}
		//******************add indexes to max_indexes
		for (uint32 i = 0; i < indexes_len; i++) {
			max_indexes[i] = str_to_int32(indexes[i]);
		}
	}
	str_init(&name, tmp_name);
	//printf("CCCCCCCC:%s,%i,%s\n",name,return_var_id(name, 0),value_var);
	//show_memory(1);
	if (return_var_id(name, 0) > 0) {
		exception_handler("redeclared_var", __func__, name, "");
		return 0;
	}
	//*******************************************analyzing values of variable
	//is multi array
	if (is_multi_array) {
		vals_array = return_value_dimensions(value_var, type_var, max_indexes, indexes_len);
		if (vals_array.start == 0) {
			printf("###########failed1\n");
			return 0;
		}
	}
		//is single data
	else {
		uint32 val_len = str_length(value_var);
		if (val_len > 2 && value_var[0] == '{' && value_var[val_len - 1] == '}') {
			exception_handler("not_defined_array", __func__, name, "");
			return 0;
		}
		//printf("DDDDDDDDD:%s,%s\n", value_var,type_var);
		String main_value = 0;
		uint8 sub_type = '0';
		calculate_value_of_var(value_var, type_var, &main_value, &sub_type);
		if (sub_type == '0') {
			printf("###########failed2\n");
			return 0;
		}
		indexes_len = 1;
		max_indexes[0] = 1;
		//-----
		vaar s = {1, sub_type, main_value, "0", 0};
		append_vaar(s, &vals_array);
		//TODO:
	}
	//print_vaar(vals_array);
	//*******************************************determine type of var
	datas data_type = search_datas(type_var, entry_table.cur_fid, false);
	//printf("#$EEEE:%s,%i,%s\n",type_var,data_type.id,data_type.name);
	//*******************************************add to memory
	//if len(name) > 0 && name[0] == '@'
	//{
	//printf("=================SET_VAR:\n%s %s(fin:%i,sid:%i,fid:%i)\n%s\n=================\n", type_var, name, fin, sid,entry_table.cur_fid, value_var);
	//}
	//******************add to pointer_memory
	longint_list pointers_id = 0;
	uint32 pointers_id_len = 0;
	//-------------create data pointers (last nodes)
	vaar *st = vals_array.start;
	for (;;) {
		if (is_struct) {
			longint_list tmp_po_id = 0, struct_po_id = 0;
			uint32 tmp_po_id_len = 0, struct_po_id_len = 0;
			long_int stde_id = str_to_long_int(st->value);
			stde s = get_stde(stde_id);
			vaar *tmp1 = s.st.start;
			uint32 co = 0;
			int32 tmp_max_indexes[MAX_ARRAY_DIMENSIONS];
			uint8 tmp_indexes_len = 0;
			if (tmp1 != 0) {
				for (;;) {
					//when switch to next item:create last node and create parent pointers
					if (tmp1 == 0 || tmp1->data_id == co + 1) {
						co++;
						long_int po_id = set_parent_nodes_Mpoint(tmp_max_indexes, tmp_indexes_len, tmp_po_id,
								tmp_po_id_len);
						//printf("@WWW:%i,%i,%i\n", po_id, tmp_max_indexes[0], tmp_indexes_len);
						longint_list_append(&struct_po_id, struct_po_id_len++, po_id);
						tmp_po_id = 0;
						tmp_po_id_len = 0;
						tmp_indexes_len = 0;
						if (tmp1 == 0) break;
					}
					//first time for any items:get max_indexes
					if (tmp_indexes_len == 0) {
						str_list ret1 = 0;
						tmp_indexes_len = (uint8) char_split(tmp1->index, ';', &ret1, true);
						for (uint8 b = 0; b < tmp_indexes_len; b++) {
							tmp_max_indexes[b] = str_to_int32(ret1[b]);
						}
					}
					//when select an item:create last node for item and save its id
					if (tmp1->data_id == co) {
						long_int po = add_to_pointer_memory(tmp1->value, tmp1->sub_type);
						longint_list_append(&tmp_po_id, tmp_po_id_len++, po);
					}
					//go to next item
					tmp1 = tmp1->next;
				}
				//create struct node as a single room
				String struct_val = 0;
				for (uint32 i = 0; i < struct_po_id_len; ++i) {
					//printf("RRRE:%i\n", struct_po_id[i]);
					struct_val = str_append(struct_val, str_from_long_int(struct_po_id[i]));
					if (i + 1 < struct_po_id_len)struct_val = char_append(struct_val, ';');
				}
				long_int po = add_to_pointer_memory(struct_val, 'l');
				longint_list_append(&pointers_id, pointers_id_len++, po);
			}
		} else {
			long_int po = add_to_pointer_memory(st->value, st->sub_type);
			longint_list_append(&pointers_id, pointers_id_len++, po);
		}
		st = st->next;
		if (st == 0) break;
	}
	//-------------create other pointers (parent nodes)
	long_int main_pointer_id = set_parent_nodes_Mpoint(max_indexes, indexes_len, pointers_id, pointers_id_len);
	//******************add to var_memory
	if (is_create_var) {
		return add_to_var_memory(main_pointer_id, fin, sid, data_type.id, name, 0);
	} else {
		return main_pointer_id;
	}
	//*******************************************
	//msg("&&&", value_var, first_pointer, pid, fin, sid, class_type, pack_type, name, prop)
	
	return 0;
}

//****************************************************
long_int
set_parent_nodes_Mpoint(int32 max_indexes[], uint8 indexes_len, longint_list pointers_id, uint32 pointers_id_len) {
	long_int main_pointer_id = 0;
	for (int32 j = indexes_len - 1; j >= 0; --j) {
		//calculate how many pointers_id set in each next pointers
		uint32 room_members = max_indexes[j];
		uint32 rooms_count = pointers_id_len / room_members;
		longint_list tmp_pointers_id = 0;
		uint32 tmp_pointers_id_len = 0;
		//=======
		for (uint32 i = 0; i < rooms_count; ++i) {
			String pointers_list = 0;
			for (uint32 k = 0; k < room_members; ++k) {
				pointers_list = str_append(pointers_list, str_from_long_int(pointers_id[0]));
				longint_list_delete_first(&pointers_id, pointers_id_len--);
				if (k + 1 < room_members)pointers_list = char_append(pointers_list, ';');
			}
			long_int po = add_to_pointer_memory(pointers_list, 'p');
			longint_list_append(&tmp_pointers_id, tmp_pointers_id_len++, po);
		}
		//=======
		if (rooms_count == 1) {
			main_pointer_id = tmp_pointers_id[0];
			break;
		}
		longint_list_init(&pointers_id, tmp_pointers_id, tmp_pointers_id_len);
		pointers_id_len = tmp_pointers_id_len;
	}
	return main_pointer_id;
}

//****************************************************
long_int
add_to_var_memory(long_int pointer_id, long_int fin, long_int sid, long_int type_var, String name, String extra) {
	entry_table.var_mem_id++;
	Mvar var = {entry_table.var_mem_id, pointer_id, sid, fin, type_var, 0, 0};
	str_init(&var.name, name);
	if (extra != 0)str_init(&var.extra, extra);
	append_Mvar(var);
	return entry_table.var_mem_id;
}

//*****************************************************
long_int add_to_pointer_memory(String data, uint8 type_data) {
	entry_table.pointer_mem_id++;
	Mpoint point = {entry_table.pointer_mem_id, 0, type_data};
	str_init(&point.data, data);
	append_Mpoint(point);
	return entry_table.pointer_mem_id;
}

//****************************************************
/**
 * display Mvar,Mpoint or both in console by details
 * @param wh
 */
void show_memory(uint8 wh) {
	if (wh == 40) {
		wh = 0;
	} else if (is_programmer_debug < 2) {
		return;
	}
	//*************show var_memory
	printf("+++++++++++++++++++++++++\n");
	if (wh == 0 || wh == 1 || wh == 3) {
		printf("-------var_memory-------\n");
		for (long_int i = 0; i < entry_table.var_mem_len; i++) {
			Mvar st = get_Mvar(i);
			if (wh != 3 && st.name == 0) {
				continue;
			}
			printf("VAR(id:%i,fin:%i,sid:%i,pointer:%i,type:%i)\n\t%s,{extra:%s}\n=====================\n", st.id,
					st.func_index, st.stru_index, st.pointer_id, st.type_var, st.name, st.extra);
		}
	}
	//*************show pointer_memory
	if (wh == 0 || wh == 2) {
		printf("-------pointer_memory-------\n");
		for (long_int i = 0; i < entry_table.pointer_mem_len; i++) {
			Mpoint st = get_Mpoint(i);
			printf("POINTER(id:%i,Type:%c)%s\n", st.id, st.type_data, st.data);
		}
	}
	printf("+++++++++++++++++++++++++\n");
}

//****************************************************
/**
 * get index of a Mvar node and delete it (if you permission it) and all its pointers
 * @param var_ind
 * @param is_del_var
 * @return Boolean
 */
Boolean delete_full_memory_var(long_int var_ind, Boolean is_del_var) {
	//******************init vars
	longint_list del_ids = 0;
	uint32 del_ids_len = 0;
	//******************analyzing
	longint_list_append(&del_ids, del_ids_len++, get_Mvar(var_ind).pointer_id);
	//msg("&DEL:", var_memory[var_id].name, var_memory[var_id].id)
	for (;;) {
		if (del_ids_len == 0) {
			break;
		}
		//-----------------
		long_int real_id = find_index_pointer_memory(del_ids[0]);
		Mpoint st = get_Mpoint(real_id);
		if (st.type_data == 'p') {
			str_list tmp1 = 0;
			uint32 tmp1_len = char_split(st.data, ';', &tmp1, true);
			for (uint32 i = 0; i < tmp1_len; i++) {
				longint_list_append(&del_ids, del_ids_len++, str_to_long_int(tmp1[i]));
			}
		}
		//-----------------
		delete_pointer_memory(del_ids[0]);
		longint_list_delete_first(&del_ids, del_ids_len--);
		
	}
	if (is_del_var) {
		delete_Mvar(var_ind);
	}
	//******************return
	return true;
}

//**************************************************************
/**
 * delete a Mpoint node by its id
 * @param id
 * @return Boolean
 */
Boolean delete_pointer_memory(long_int id) {
	for (long_int i = 0; i < entry_table.pointer_mem_len; i++) {
		if (get_Mpoint(i).id == id) {
			delete_Mpoint(i);
		}
	}
	return true;
}

//**************************************************************
/**
 * get an id and search for index it in Mpoint
 * @param id
 * @return long_int
 */
long_int find_index_pointer_memory(long_int id) {
	for (long_int i = 0; i < entry_table.pointer_mem_len; i++) {
		if (get_Mpoint(i).id == id) {
			return i;
		}
	}
	return 0;
}

//**************************************************************
/**
 * delete a Mvar node by its id
 * @param id
 * @return Boolean
 */
Boolean delete_var_memory(long_int id) {
	for (long_int i = 0; i < entry_table.var_mem_len; i++) {
		if (get_Mvar(i).id == id) {
			delete_Mvar(i);
		}
	}
	return true;
}

//**************************************************************
/**
 * get an id and search for index it in Mvar
 * @param id
 * @return long_int
 */
long_int find_index_var_memory(long_int id) {
	for (long_int i = 0; i < entry_table.var_mem_len; i++) {
		if (get_Mvar(i).id == id) {
			return i;
		}
	}
	return 0;
}

//****************************************************
/**
 * get a pointer_id of var and return an index in Mvar
 * @param pointer_id
 * @return long_int
 */
long_int return_var_ind_pointer_id(long_int pointer_id) {
	for (long_int i = 0; i < entry_table.var_mem_len; i++) {
		if (get_Mvar(i).pointer_id == pointer_id) {
			return i;
		}
	}
	return 0;
}
//****************************************************
//func get_data_memory_index(pointer_id long_int, index_var string) (long_int, string)
//****************************************************
//func edit_index_room_var(new_value, index_var string, real_id long_int, new_type byte) bool
//****************************************************
//func copy_memory_var(var_index long_int, new_name string, pid, fin long_int) long_int
//****************************************************
//func return_var_dimensions(var_ind long_int) []string
//****************************************************
//func review_create_array_from(po_id long_int) (string,byte,bool)
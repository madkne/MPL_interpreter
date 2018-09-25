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
	//msg("&@@@:", name, return_var_id(name+"%", "0"), cur_fin, value_var)
	//*******************************************if name is already exist
	/*if(return_var_id(name + "%", "0") > 0)
	{
		return return_var_id(name + "%", "0")
	}*/
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
				exception_handler("not_exist_var", "set_memory_var:120", value_var, "");
				return 0;
			}
			//show_memory(40)
			value_var = return_value_var_complete(find_index_var_memory(ret0))
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
	var
	max_indexes, set_indexes[max_array_dimensions + 1]
	int var
	class_type long_int = 0
	var is_multi_array_fix = false
	//*******************************************analyzing name of variable
	if strings.Contains(name, "[")
	{
		tmp_name, tmp_ind:=return_name_index_var(name, true);
		//msg("&NNN",tmp_name,tmp_ind)
		if tmp_ind != ""
		{
			is_multi_array_fix = true
			indexes:=strings.Split(tmp_ind, ",")
			//******************add indexes to max_indexes
			for
			i:=1;
			i < len(max_indexes);
			i++
			{
				if len(indexes) <= i - 1
				{
					break
				}
				tmp, _:=strconv.ParseUint(indexes[i - 1], 10, 64)
				max_indexes[i] =
				int(tmp)
			}
			name = tmp_name
		}
		
	}
	//*******************************************analyzing values of variable
	if !is_multi_array_fix && len(value_var) > 1 && value_var[0] == '{'
	{
		var
		is_done = true
		vals_array, max_indexes, is_done = return_value_dimensions(value_var, type_var)
		//msg("&YY", vals_array)
		if !is_done
		{
			return 0
		}
	} else if !is_multi_array_fix
	{
		if return_var_id(name, "") > 0
		{
			exception_handler("redeclared_var", "set_memory_var", name, "")
			return 0
		}
		//msg("&BBB",name,value_var)
		tmp_val, _, tmp_type := fitting_value(value_var, type_var, "byte")
		max_indexes[1] = 1
		//msg("&XVAL:", name, value_var, tmp_val, type_var, string(tmp_type))
		vals_array = append(vals_array, vals_array_struct
		{ tmp_val, "0", tmp_type, 0 })
		if tmp_type == '0'
		{
			return 0
		}
		//msg(max_indexes)
		
		//msg("&QQQQ", vals_array)
	}
	//msg("&header", 'i', 'd', vals_array)
	//*******************************************determine type of var
	class_type = search_in_classes(type_var)
	//msg(vals_array, class_type, set_indexes)
	//*******************************************add to memory
	if len(name) > 0 && name[0] == '@'
	{
		//fmt.Printf("=================SET_VAR:\n%s %s%v(cid:%v,fin:%v,sid:%v,fin:%v)\n%v\n=================\n", type_var, name, prop, pid, fin, sid, cur_fin, value_var)
	}
	
	//******************add to pointer_memory as data
	//-------------get count of dimension of array
	var
	count_dem, tmp2 = 0, 1
	for
	{
		if max_indexes[tmp2] == 0
		{
			break
		}
		count_dem++
		tmp2++
	}
	//-------------allocate all array rooms
	var
	is_break = false
	//msg("FFF:", count_dem)
	set_indexes[count_dem - 1] = -1
	for
	{
		//-------------determine index
		var
		ind_str = ""
		set_indexes[count_dem - 1]++
		for
		v := count_dem - 1;
		v >= 0;
		v--
		{
			if set_indexes[v] == max_indexes[v + 1] && v > 0
			{
				set_indexes[v - 1]++
				set_indexes[v] = 0
			} else if v == 0 && set_indexes[v] == max_indexes[v + 1]
			{
				is_break = true
				break
			}
		}
		if is_break
		{
			break
		}
		//-------------create ind_str
		for
		b := 0;
		b < count_dem;
		b++
		{
			ind_str += strconv.FormatInt(int64(set_indexes[b]), 10)
			if b < count_dem - 1
			{
				ind_str += ","
			}
		}
		//-------------allocate
		var
		is_exist, val_ind = false, 0
		
		data, _, flag := fitting_value("", type_var, "default")
		for
		b := 0;
		b < len(vals_array);
		b++
		{
			if vals_array[b].index == ind_str
			{
				is_exist = true
				val_ind = b
				data = vals_array[b].value
				flag = vals_array[b].type_val
			}
		}
		
		data, _, _ = fitting_value(data, string(flag), "str_mem")
		//msg("&YY", data, string(flag), type_var, name)
		data_id := add_to_pointer_memory(data, flag)
		if is_exist
		{
			vals_array[val_ind].data_id = data_id
		} else {
			vals_array = append(vals_array, vals_array_struct
			{ data, ind_str, flag, data_id })
		}
		//msg("&HHHH", vals_array)
		//-------------sorting
		for
		{
			var
			is_change = false
			for
			b := 0;
			b < len(vals_array);
			b++
			{
				if b + 1 < len(vals_array) && vals_array[b + 1].data_id < vals_array[b].data_id
				{
					is_change = true
					tmp1 := vals_array[b + 1].data_id
					tmp2 := vals_array[b + 1].index
					tmp3 := vals_array[b + 1].type_val
					tmp4 := vals_array[b + 1].value
					vals_array[b + 1].data_id = vals_array[b].data_id
					vals_array[b + 1].index = vals_array[b].index
					vals_array[b + 1].type_val = vals_array[b].type_val
					vals_array[b + 1].value = vals_array[b].value
					vals_array[b].data_id = tmp1
					vals_array[b].index = tmp2
					vals_array[b].type_val = tmp3
					vals_array[b].value = tmp4
				}
			}
			if !is_change
			{
				break
			}
		}
		//msg("-YYYY:", ind_str)
		
	}
	//msg("&&",vals_array[0].value, class_type)
	//*******************************************search for errors&warnings
	
	//******************add to pointer_memory
	//-------------create pointers
	first_pointer := add_to_pointer_memory("", 'p')
	var
	last_pointers[]
	long_int
	last_pointers = append(last_pointers, first_pointer)
	for
	i := 1;
	i <= count_dem - 1;
	i++
	{
		var
		tmp_pointers[]
		long_int
		for
		b := 0;
		b < max_indexes[i] * len(last_pointers);
		b++
		{
			tmp_p := add_to_pointer_memory("", 'p')
			tmp_pointers = append(tmp_pointers, tmp_p)
		}
		count_per := len(tmp_pointers) / len(last_pointers)
		var
		start_ar = 0
		for
		b := 0;
		b < len(last_pointers);
		b++
		{
			ind := find_index_pointer_memory(last_pointers[b])
			for
			c := start_ar;
			c < start_ar + count_per;
			c++
			{
				pointer_memory[ind].data += strconv.FormatInt(int64(tmp_pointers[c]), 10)
				if c + 1 < start_ar + count_per
				{
					pointer_memory[ind].data += ";"
				}
			}
			start_ar = start_ar + count_per
		}
		last_pointers = last_pointers[0:0]
		last_pointers = tmp_pointers
	}
	//-------------allocate last pointers with vals_array
	count_per := len(vals_array) / len(last_pointers)
	var
	start_ar = 0
	for
	i := 0;
	i < len(last_pointers);
	i++
	{
		//msg("IIII:", i, last_pointers[i])
		ind := find_index_pointer_memory(last_pointers[i])
		for
		c := start_ar;
		c < start_ar + count_per;
		c++
		{
			pointer_memory[ind].data += strconv.FormatInt(int64(vals_array[c].data_id), 10)
			if c + 1 < start_ar + count_per
			{
				pointer_memory[ind].data += ";"
			}
		}
		start_ar = start_ar + count_per
	}
	//******************add to var_memory
	if is_create_var
	{
		if pack_type == 0
		{
			pid = 0
		}
		if len(prop) > 2 && prop[2]
		{
			var
			extra = strconv.FormatInt(int64(cur_pid), 10) + ";" + strconv.FormatInt(int64(cur_pid_instance), 10) + ";" +
					strconv.FormatInt(int64(cur_fid), 10)
			name += segments_split + extra
		}
		final_var_id := add_to_var_memory(first_pointer, pid, fin, sid, class_type, pack_type, name, prop)
		return final_var_id
	}else{
		return first_pointer
	}
	//*******************************************
	//msg("&&&", value_var, first_pointer, pid, fin, sid, class_type, pack_type, name, prop)
	//show_memory(40)
	
	
}
//****************************************************
/**
delete_full_memory_var
get a var_id as var_memory index
*/
Boolean delete_full_memory_var(long_int var_id, Boolean is_del_var) {
	//******************init vars
	/*	var
		del_ids[]
		long_int
		//******************analyzing
		del_ids = append(del_ids, var_memory[var_id].pointer_id)
		//msg("&DEL:", var_memory[var_id].name, var_memory[var_id].id)
		for
		{
			if len(del_ids) == 0
			{
				break
			}
			//-----------------
			real_id := find_index_pointer_memory(del_ids[0])
			st := pointer_memory[real_id]
			if st.type_data == 'p'
			{
				tmp1 := strings.Split(st.data, ";")
				for
				i := 0;
				i < len(tmp1);
				i++
				{
					tmp2, _ := strconv.ParseInt(tmp1[i], 10, 64)
					del_ids = append(del_ids, long_int(tmp2))
				}
			}
			//-----------------
			delete_pointer_memory(del_ids[0])
			del_ids = del_ids[1:]
			
		}
		if (is_del_var) {
			delete_var_memory(var_memory[var_id].id)
		}*/
	//******************return
	return true;
}

//****************************************************
//**************************************************************
//[OK]
Boolean delete_pointer_memory(long_int id) {
	/*var tmp[]
	pointer_memory_struct
	for i := 0; i <
	len(pointer_memory);
	i++ {
	if pointer_memory[i].id != id {
	tmp = append(tmp, pointer_memory[i])
	}
	}
	pointer_memory = tmp*/
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
//[OK]
Boolean delete_var_memory(long_int id) {
	/*var tmp[]
	var_memory_struct
	for i := 0; i <
	len(var_memory);
	i++ {
	if var_memory[i].id != id {
	tmp = append(tmp, var_memory[i])
	}
	}
	var_memory = tmp*/
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
//****************************************************
//****************************************************
//****************************************************
//****************************************************
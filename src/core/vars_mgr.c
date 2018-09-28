//
// Created by amindelavar on 9/19/2018.
//

#include <MPL/system.h>


Boolean is_valid_name(String name, Boolean is_array) {
	//fmt.Println("WWWWWWW:", name)
	//var sp=[]byte{',','!','#','$','%','^','&','*','(',')','-','=','+','/','\\','.','?',':',';','~'}
	uint32 name_len = str_length(name);
	if (name_len < 1) {
		return false;
	}
	//******************************is mahdi keyword
	if (str_search(keywords, name, StrArraySize(keywords))) {
		return false;
	}
	//******************************split index , name
	String name_v = 0, index = 0, buf = 0, extra = 0;
	Boolean is_string = false, is_end = false;
	uint32 pars = 0;
	if (is_array) {
		for (uint32 i = 0; i < name_len; i++) {
			//------------------check is string
			if (name[i] == '\"' && (i == 0 || name[i - 1] != '\\')) {
				is_string = switch_bool(is_string);
			}
			//------------------count pars
			if (!is_string && name[i] == '[')
				pars++;
			else if (!is_string && name[i] == ']')
				pars--;
			
			//------------------set name_v
			if (!is_string && name[i] == '[' && name_v == 0 && pars == 1) {
				name_v = str_trim_space(buf);
				buf = 0;
				continue;
			}
			//------------------set index
			if (!is_string && name[i] == ']' && name_v != 0 && pars == 0) {
				index = str_trim_space(buf);
				buf = 0;
				is_end = true;
				continue;
				//break
			}
			//------------------append to buf
			buf = char_append(buf, name[i]);
		}
		//------------------set extra :k1[0]+3
		if (!is_string && is_end && buf != 0) {
			str_init(&extra, buf);
		}
		if (name_v == 0) {
			str_init(&name_v, buf);
		}
		
	} else {
		str_init(&name_v, name);
	}
	//msg("&%%%", name, name_v, index, extra)
	//******************************check for extra
	if (extra != 0) {
		return false;
	}
	//******************************check for name_v
	if (str_length(name_v) > 0 &&
			((name_v[0] >= '0' && name_v[0] <= '9') || (name_v[0] == '_' && str_equal(interpreter_level, "parse")))) {
		return false;
	}
	for (uint32 i = 0; i < str_length(name_v); i++) {
		if ((name_v[i] >= '0' && name_v[i] <= '9') || (name_v[i] >= 'A' && name_v[i] <= 'Z') ||
				(name_v[i] >= 'a' && name_v[i] <= 'z') || name_v[i] == '_' || (name_v[i] == '@' && i == 0)) {
			continue;
		} else {
			return false;
		}
	}
	//******************************check for index
	if (is_array && index == 0) {
		//return false
	}
	return true;
}


//*********************************************************
void return_name_index_var(String s, Boolean is_empty_index, String *name, String *index) {
	/**
	error messages:
	- array dems is larger than max_array_dimension
	*/
	//**************************define variables
	String word = 0;
	uint8 bra = 0;
	*name = 0, *index = 0;
	if (!is_empty_index) {
		*index = "0";
	}
	
	//**************************start analyzing
	uint32 len = str_length(s);
	//----------------finding name and index of var
	for (uint32 i = 0; i < len; i++) {
		if (s[i] == '[')bra++;
		else if (s[i] == ']')bra--;
		//----------
		if ((s[i] == '[' && bra == 1) || (s[i] == ']' && bra == 0) || i + 1 == len) {
			if (s[i] == '[' || i + 1 == len && (*name) == 0) {
				if (i + 1 == len) {
					word = char_append(word, s[i]);
				}
				str_init(&*(name), word);
			} else if (name != 0 && s[i] == ']') {
				str_init(&(*index), word);
				break;
			}
			word = 0;
		} else {
			word = char_append(word, s[i]);
		}
	}
	(*index) = str_trim_space(*index);
	//printf("@@@2:%s=>%s#,%s#\n", s, *name, *index);
}

//********************************************
uint8 define_vars_analyzing(String inst, def_var_s vars_store[]) {
	/**
	1- num x1,x2,str x3 								---OK---
	2- num x1,x2=(3+5)*x3 								---OK---
	3- num x1,x2,str x3=0x3A4D,(5+7)/2,("we"*10)  		---OK---
	4- num x1=45 										---OK---
	5- bool x1=true&&(f==45) 							---OK---
	6- num x1[2,2]={{5,7.8},{1,7}}	 					---OK---
	7- num x1=x2[1,2]*2 								---OK---
	8- str st2[2]={"Amin",st1[0]*2+"!"} 				---OK---
	9- str q1[?,5] 										---OK---
	---------------------SAMPLES---------------------
	//str ss,num b1,b2="WE",-(2 ^(-3.0 +1))*3,0x345+3  //b1=-(2 ^(-3 +1))*3 =-0.75
	//num x1,x2,str x3=0x3A4D,(5+7)/2,"Hello "+("Amin "*3)+"!"
	//str x1, x3="Hello "+("Amin "*3)+"!",{"Amin","Ali","Reza"}
	//__define["x1"]="Hello "+("Amin "*3)+"!"
	//num zz[2,3]={{4+6,7.84,(9^2)-1},{8,888,1000}}
	//bool x1=true&&!((45-1)>=45) //x1=true
	*/
	//printf("DDDDDDDDDDD:%s\n",inst);
	//*******************************************is null
	inst = str_trim_space(inst);
	if (str_is_empty(inst)) return 0;
	String word = 0, last_type = 0, data_type = 0;
	Boolean is_string = false, is_equal = false;
	int16 vars_counter = -1, vals_counter = -1, pars = 0, bra = 0;
	//*******************************************determine variables with their values
	uint32 len = str_length(inst);
	for (uint32 i = 0; i < len; i++) {
		//------------------check is string
		if (inst[i] == '\"' && (i == 0 || inst[i - 1] != '\\')) {
			is_string = switch_bool(is_string);
		}
		//------------------continue if ' '
		if (!is_string && i + 1 < len && inst[i] == ' ' && (inst[i + 1] == '(' || inst[i + 1] == '[')) {
			continue;
		}
		//------------------count bra
		if (!is_string) {
			switch (inst[i]) {
				case '[':
					bra++;
					break;
				case ']':
					bra--;
					break;
			}
		}
		//------------------determine data type
		if (!is_string && !is_equal && word != 0 && inst[i] == ' ' &&
				(is_valid_name(word, false) || str_equal(word, "vars") || str_equal(word, "num") ||
						str_equal(word, "str") || str_equal(word, "bool"))) {
			str_init(&data_type, word);
			word = 0;
		}
		//------------------store a variable
		if (!is_string && !is_equal && pars == 0 && (inst[i] == ',' || inst[i] == '=' || i + 1 == len) &&
				data_type != 0 && bra == 0) {
			if (i + 1 == len) {
				word = char_append(word, inst[i]);
			}
			word = str_trim_space(word);
			if (!is_valid_name(word, true)) {
				print_error(entry_table.line_number, "invalid_name_var", entry_table.cur_ascii_source_path, word, "",
						"define_vars_analyzing");
				return 0;
			}
			
			if (word != 0) {
				
				//add new record to ret
				vars_counter++;
				return_name_index_var(word, false, &vars_store[vars_counter].name_var,
						&vars_store[vars_counter].index_var);
				str_init(&vars_store[vars_counter].main_type, data_type);
				//printf("IIIII:%s[%i]=>%s\n",word,vars_counter,data_type);
				vars_store[vars_counter].sub_type = 0;
				vars_store[vars_counter].value_var = 0;
				vars_store[vars_counter].sid = entry_table.current_sid;
				vars_store[vars_counter].fid = entry_table.current_fid;
				//printf("&SDSS:%s,%s,%s=>\n", word, data_type,ret.name_var);
				str_init(&last_type, data_type);
			}
		}
		//------------------allocate values to variables
		//msg("VVVV:", word, "|MMM:", pars)
		if (is_equal && !is_string && (word != 0 || i + 1 == len) &&
				((inst[i] == ',' && pars == 0) || (i + 1 == len && pars < 2)) && bra == 0) {
			if (i + 1 == len) {
				word = char_append(word, inst[i]);
			}
			/*if vals_counter > vars_counter {
				//msg("&SSS", vals_counter, vars_counter)
				exception_handler("wrong_def_var", "define_vars_analyzing", word, "")
			} else */
			if (inst[i] == '}') pars--;
			vals_counter++;
			//printf("UUU:%s,%s,%i\n", word,vars_store[vals_counter].main_type,vals_counter);
			word = str_trim_space(word);
			String main_type = vars_store[vals_counter].main_type;
			if (word[0] == '{' && !str_equal(main_type, "bool") && !str_equal(main_type, "str")) {
				String buf = 0;
				uint8 sub = INT_SUB_TYPE_ID;
				for (uint32 j = 0; j < str_length(word); j++) {
					if (buf != 0 && (word[j] == '{' || word[j] == '}' || word[j] == ',')) {
						uint8 tmp = determine_sub_type_var(&buf, main_type);
						if (tmp == HUGE_SUB_TYPE_ID) {
							sub = tmp;
							break;
						} else if (tmp > sub)sub = tmp;
						buf = 0;
						continue;
					}
					buf = char_append(buf, word[j]);
				}
				vars_store[vals_counter].sub_type = sub;
			} else {
				vars_store[vals_counter].sub_type = determine_sub_type_var(&word, vars_store[vals_counter].main_type);
			}
			str_init(&vars_store[vals_counter].value_var, word);
			word = 0;
			
		}
		
		//------------------append to word
		Boolean empty_word = false;
		if (!is_string && !is_equal && (inst[i] == ' ' || inst[i] == '=' || inst[i] == ',')) {
			empty_word = true;
		} else if (!is_string && is_equal && pars == 0 && inst[i] == ',') {
			empty_word = true;
		}
		if (empty_word && bra == 0) {
			word = 0;
			//------------------is_equal
			if (inst[i] == '=') is_equal = true;
		} else {
			word = char_append(word, inst[i]);
		}
		//------------------count pars
		if (!is_string && is_equal) {
			switch (inst[i]) {
				case '{':
					pars++;
					break;
				case '}':
					pars--;
					break;
			}
		}
	}
	//**************alloc one value to many variables
	//TODO:
	if (vals_counter == 0 && vars_counter > 0) {
		for (uint8 i = 1; i <= vars_counter; i++) {
			/*if vars_store[0].type_var != vars_store[i].type_var {
				exception_handler("not_alloc_val", "define_vars_analyzing", vars_store[i].name_var, vars_store[0].value_var)
				return -1, vars_store
			}*/
			//str_init(&vars_store[i].value_var , vars_store[0].value_var);
		}
	}
	//**************if values is less than vars
	/*if (vals_counter>0&&vars_counter>vals_counter){
		for (uint8 i=0;i<=vars_counter;i++){
			if vars_store[i].value_var==""{
				exception_handler("not_equal_vals_vars", "define_vars_analyzing:433","","")
				return -1, vars_store
			}
		}
	}*/
	//TODO:
	//**************if values is more than vars
	/*if is_runtime&&vars_counter<vals_counter{
		for i:=0;i<=vals_counter;i++{
			if vars_store[i].name_var==""{
				exception_handler("not_equal_vals_vars", "define_vars_analyzing:442","","")
				return -1, vars_store
			}
		}
	}*/
	//**************return
	//printf("CCCC:%i:%s\n",vars_counter,vars_store[0].name_var);
	for (uint8 i = 0; i < vars_counter + 1; i++) {
		def_var_s tmp3 = vars_store[i];
		if (tmp3.sub_type == 0) tmp3.sub_type = determine_sub_type_var(&tmp3.value_var, tmp3.main_type);
		//printf("WWWSSS(%i):%s(%i)-%s[%s]=%s\n", i, tmp3.main_type, tmp3.sub_type, tmp3.name_var,tmp3.index_var, tmp3.value_var);
	}
	return vars_counter + 1;
}


//*********************************************************
uint8 determine_sub_type_var(String *value, String main_type) {
	//----------------------------------if main_type is str or bool
	if (str_equal(main_type, "str")) return STR_SUB_TYPE_ID;
	else if (str_equal(main_type, "bool")) return BOOL_SUB_TYPE_ID;
	//-----------------------------------if main_type is num
	Boolean is_point = false;
	//-----------------if defined sub type
	String val = 0;
	str_init(&val, (*value));
	uint32 len = str_length(val);
	if (len > 2)
		for (uint8 i = 2; i < StrArraySize(sub_types); i++) {
			uint32 size = str_length(sub_types[i]);
			if (len - size == 0) continue;
			String word = str_substring(val, len - size, 0);
			//printf("DDDD:%s=>%s,%s,%i\n",val,word,sub_types[i],len-size);
			if (word != 0 && len - size - 1 > 0 && val[len - size - 1] == '.' && str_equal(word, sub_types[i])) {
				(*value) = str_substring(val, 0, len - size - 1);
				return i + 1;
			}
		}
	//-----------------check num attrs
	for (uint32 i = 0; i < len; i++) {
		if (val[i] == '.') {
			is_point = true;
			break;
		}
	}
	
	if (!is_point && len < MAX_INT_LEN) return INT_SUB_TYPE_ID;
	if (is_point && len < MAX_FLOAT_LEN) return FLOAT_SUB_TYPE_ID;
	
	return HUGE_SUB_TYPE_ID;
}

//*********************************************************
long_int return_var_id(String var_name, String var_index) {
	//******************
	for (long_int i = 0; i < entry_table.var_mem_len; i++) {
		Mvar st = get_Mvar(i);
		//******************method1:search with cur_fin : function var
		if (st.func_index == entry_table.cur_fin && str_equal(st.name, var_name)) {
			return st.id;
		}
		//******************method2:search with cur_fin=0 : global var
		if (st.func_index == 0 && str_equal(st.name, var_name)) {
			return st.id;
		}
		
	}
	//******************return failed
	//show_memory(0)
	//println("WWW:", cur_fin)
	//show_prev_fins_array()
	//os.Exit(1)
	return 0;
}
//*********************************************************
/**
 * get an index of Mvar and return all values of this var
 * @param var_index
 * @return String
 */
String return_value_var_complete(long_int var_index) {
	//******************init vars
	if (var_index == 0) {
		
		return 0;
	}
	long_int po_id = get_Mvar(var_index).pointer_id;
	String final_value = 0;
	longint_list store_po;
	uint32 items_count = 0, store_po_len = 0;
	//show_memory(0)
	
	//******************analyzing
	for (;;) {
		if (store_po_len > 0) {
			po_id = store_po[0];
			longint_list_delete_first(&store_po, store_po_len--);
		}
		if (po_id == 0 && store_po_len == 0) break;
		if (po_id == 0) {
			final_value = char_append(final_value, '}');
			if (store_po_len > 0 && store_po[0] != 0) {
				final_value = char_append(final_value, ',');
				items_count++;
			}
			continue;
		}
		long_int po_ind = find_index_pointer_memory(po_id);
		if (po_ind == 0) break;
		//msg("RTE:", pointer_memory[po_ind])
		Mpoint mpoint = get_Mpoint(po_ind);
		if (mpoint.type_data == 'p') {
			str_list st;
			uint32 count = char_split(mpoint.data, ';', &st, true);
			final_value = char_append(final_value, '{');
			longint_list tmp_store = 0;
			uint32 tmp_store_len = 0;
			for (uint32 b = 0; b < count; b++) {
				longint_list_append(&tmp_store, tmp_store_len++, str_to_long_int(st[b]));
			}
			longint_list_append(&tmp_store, tmp_store_len++, 0);
			longint_list tmp1;
			uint32 tmp1_len = store_po_len;
			longint_list_init(&tmp1, store_po, store_po_len);
			longint_list_init(&store_po, tmp_store, tmp_store_len);
			store_po_len = tmp_store_len;
			for (uint32 n = 0; n < tmp1_len; n++) {
				longint_list_append(&store_po, store_po_len, tmp1[n]);
			}
			//msg("####QQQ:\n", store_po)
			
		} else if (mpoint.type_data != 'p') {
			if (mpoint.type_data == 's') {
				str_init(&mpoint.data, convert_to_string(mpoint.data));
			} else if (mpoint.type_data == 'i' || mpoint.type_data == 'f' || mpoint.type_data == 'h') {
				mpoint.data = char_append(mpoint.data, mpoint.type_data);
			}
			final_value = str_append(final_value, mpoint.data);
			if (store_po_len > 0 && store_po[0] != 0) {
				final_value = char_append(final_value, ',');
				items_count++;
			}
			//msg("&CCXXXX:", var_memory[var_index].name, data, string(typ))
		}
	}
	final_value = char_append(final_value, '}');
	if (items_count == 0 && str_length(final_value) > 1) {
		final_value = str_substring(final_value, 1, str_length(final_value) - 1);
	}
	//msg("&*****$$$$COPY:", final_value, var_memory[var_index].pointer_id, var_memory[var_index].name, var_index)
	return final_value;
}
//*********************************************************
/**
 * get a string from max index of a var like "2,4,5" and return sum its indexs like 40
 * @param index
 * @return uint32
 */
uint32 return_total_array_rooms(String index) {
	str_list ind = 0;
	uint32 ret = 1;
	uint8 count = (uint8) char_split(index, ',', &ind, true);
	for (uint8 i = 0; i < count; ++i) {
		int32 num = str_to_int32(ind[i]);
		ret *= num;
	}
	return ret;
}
//*********************************************************
/**
 * get an array value of var and analyze its items and return list of its items for set im memory
 * @param value
 * @param type_var
 * @param indexes
 * @param indexes_len
 * @return vaar_en
 */
vaar_en return_value_dimensions(String value, String type_var, int32 indexes[], uint8 indexes_len) {
	/**
	samples:
	- {{0b001010,78,0x56},{-456.88678,9999,0.99}}
	- {{3,-8,9.78},{(243^34)/4+56.5,8f,null},{0x45,0o34,0b01011},{-3454,45h,12.4i}}
	*/
	//--------------------init vars
	Boolean is_string = false;
	vaar_en vals_array = {0, 0, 0};
	vaar_en null = {0, 0, 0};
	uint8 pars = 0/*count of parenthesis*/, bras = 0/*count of brackets*/, acos = 0/*count of acolads*/, count_d = 0/*count of dimensions */;
	String word = 0;
	uint32 max_count = 0;
	value = str_trim_space(value);
	str_list entries = 0;
	uint32 entries_len = 0;
	str_list indexes_list = 0;
	uint32 indexes_list_len = 0;
	uint32 child_count = 0;
	int32 cur_ind[indexes_len];
	uint32 total_rooms = 1;
	if (indexes_len == 0)return vals_array;
	//--------------------get total rooms
	for (uint8 j = 0; j < indexes_len; ++j) {
		total_rooms *= indexes[j]; //calc total_rooms
		cur_ind[j] = 0; //reset cur_ind
	}
	//--------------------create all indexes
	for (uint32 k = 0; k < total_rooms; ++k) {
		String index = 0;
		if (k > 0)
			for (int16 j = indexes_len - 1; j >= 0; j--) {
				cur_ind[j]++;
				if (cur_ind[j] >= indexes[j])cur_ind[j] = 0;
				else break;
			}
		for (int r = 0; r < indexes_len; r++) {
			index = str_append(index, str_from_long_int((long_int) cur_ind[r]));
			if (r + 1 < indexes_len)index = char_append(index, ',');
		}
		str_list_append(&indexes_list, index, indexes_list_len++);
		//printf("index[%i]:%s\n", k, index);
	}
	//--------------------search for values
	str_list_append(&entries, value, entries_len++);
	//--------
	for (uint8 i = 0; i < indexes_len; i++) {
		str_list tmp_entries = 0;
		uint32 tmp_entries_len = 0;
		for (uint32 b = 0; b < entries_len; b++) {
			uint32 ent_len = str_length(entries[b]);
			//------
			if (entries[b][0] == '{' && entries[b][ent_len - 1] == '}') {
				str_init(&entries[b], str_substring(entries[b], 1, ent_len - 1));
			} else {
				//TODO:error
				//printf("ERROR:%s\n",entries[b]);
				return null;
			}
			//------
			pars = 0, bras = 0, acos = 0;
			String buf = 0;
			//------
			for (uint32 c = 0; c < ent_len; c++) {
				//========check is string
				if (entries[b][c] == '\"' && (i == 0 || entries[b][c - 1] != '\\')) {
					is_string = switch_bool(is_string);
				}
				//========count pars,bras,acos
				if (!is_string) {
					if (entries[b][c] == '(')pars++;
					else if (entries[b][c] == ')')pars--;
					else if (entries[b][c] == '[')bras++;
					else if (entries[b][c] == ']')bras--;
					else if (entries[b][c] == '{')acos++;
					else if (entries[b][c] == '}')acos--;
				}
				//========split segments
				if (!is_string && pars == 0 && bras == 0 && acos == 0 && (entries[b][c] == ',' || c + 1 == ent_len)) {
					str_list_append(&tmp_entries, buf, tmp_entries_len++);
					//----last step:register in vals_array
					if (i + 1 == indexes_len) {
						String sec_type = determine_value_type(buf);
						if (!str_equal(sec_type, type_var) && !str_equal(sec_type, "null")) {
							exception_handler("val_def_var", "set_memory_var", buf, type_var);
							return null;
						}
						//-----
						String main_value = 0;
						uint8 sub_type = '0';
						calculate_value_of_var(buf, type_var, &main_value, &sub_type);
						//-----
						vaar s = {child_count, sub_type, main_value, indexes_list[child_count], 0};
						child_count++;
						append_vaar(s, &vals_array);
						//printf("@@##R:%s\n", buf);
					}
					buf = 0;
					continue;
				}
				//========append to buf
				buf = char_append(buf, entries[b][c]);
			}
		}
		//printf("QQQQQQQ:%i,%s\n", i, print_str_list(tmp_entries, tmp_entries_len));
		//------
		str_list_init(&entries, tmp_entries, tmp_entries_len);
		entries_len = tmp_entries_len;
	}
	return vals_array;
}
//*********************************************************
/**
 * get value and determine that what is its type like bool,num
 * @param val
 * @return String
 */
String determine_value_type(String val) {
	//****************init vars
	Boolean is_string = false;
	String final_type = 0, word = 0;
	str_init(&final_type, "num");
	//****************analyzing
	if (str_equal(val, "null")) {
		return "null";
	}
	if (str_equal(val, "true") || str_equal(val, "false")) {
		return "bool";
	}
	uint32 len = str_length(val);
	for (uint32 i = 0; i < len; i++) {
		//------------------check is string
		if (val[i] == '\"' && (i == 0 || val[i - 1] != '\\')) {
			is_string = switch_bool(is_string);
			str_init(&final_type, "str");
		}
		//------------------is boolean operators
		String tmp1 = 0;
		if (!is_string && i + 1 < len) {
			tmp1 = char_append(tmp1, val[i]);
			tmp1 = char_append(tmp1, val[i + 1]);
		}
		if (!is_string && tmp1 != 0 && str_search(boolean_operators, tmp1, StrArraySize(boolean_operators))) {
			str_init(&final_type, "bool");
			break;
		}
		if (!is_string && (str_equal(word, "true") || str_equal(word, "false"))) {
			str_init(&final_type, "bool");
			break;
		}
		
		//------------------append to word
		if (!is_string && char_search_count(words_splitter, val[i])) {
			word = 0;
		} else {
			word = char_append(word, val[i]);
		}
	}
	
	//****************return
	return final_type;
}

//*********************************************************
/**
 * get a value string and its type like num and then if need simplify value and return sub_type of value
 * @param value
 * @param type
 * @param ret_value
 * @param ret_subtype
 */
void calculate_value_of_var(String value, String type, String *ret_value, uint8 *ret_subtype) {
	//printf("&YYYY:%s,%s\n", value, type);
	if (str_equal(type, "null")) {
		(*ret_subtype) = 'n';
		str_init(&(*ret_value), "null");
	} else if (str_equal(type, "num")) {
		calculate_math_expression(value, '_', &(*ret_value), &(*ret_subtype));
	} else if (str_equal(type, "str")) {
		calculate_string_expression(value, &(*ret_value), &(*ret_subtype));
	}
	//TODO:
	/*
	case type_var == "bool":
		value_var = calculate_boolean_expression(value_var)
	sub_type = 'b'
	case search_in_classes(type_var) > 0:
		//msg("XXXX:", type_var, value_var)
		if value_var == "" || value_var == "null"
	{
		value_var = "null"
	} else if is_digit_string(value_var)
	{
		var
		num_type
		byte = 'i'
		value_var, num_type = calculate_math_expression(value_var, '_')
		sub_type = num_type
	}
	*/
	
	//tmp_type, tmp_val := calculate_value_of_var(type_val, value, false)
	/*if len(value) > 0 && (value[len(value)-1] == 'h' || value[len(value)-1] == 'f' || value[len(value)-1] == 'i') {
		tmp_type = determine_type_num(value)
		if tmp_type == 'i' && len(tmp_val) > int_len {
			tmp_val = resize_to_int(tmp_val)
		} else if tmp_type == 'f' && len(tmp_val) > float_len {
			tmp_val = resize_to_float(tmp_val)
		}
	}*/
}

//*********************************************************
String format_string_expression(String exp) {
	/**
	1- str st2="Hi %st1% ! you are %d1[1,0]%\n" ---OK---
	2- str st2="Hi \%st1\% ! you are %d1[1,0]%\n" ---OK---
	*/
	//msg("QQQQ:", exp)
	//***********************define variables
	String final_exp = 0, word = 0;
	str_init(&final_exp, exp);
	Boolean is_format = false, is_change = false;
	int32 start = 0, end = 0;
	//***********************start analyzing
	while (true) {
		uint32 final_len = str_length(final_exp);
		is_change = false;
		for (uint32 i = 0; i < final_len; i++) {
			if (final_exp[i] == '%' && (i == 0 || final_exp[i - 1] != '\\')) {
				is_format = switch_bool(is_format);
				if (is_format) {
					start = i;
				} else {
					//printf("&###:%s\n", word /*, final_exp, exp*/);
					end = i;
					Mpoint po = return_var_memory_value(word);
					if (po.type_data != '0') {
						//String tmp3=0 = final_exp
						final_exp = str_multi_append(str_substring(final_exp, 0, start), po.data,
								str_substring(final_exp, end + 1, 0), 0, 0, 0);
						//printf("PP::%s,%s\n", word, final_exp);
						is_change = true;
						break;
					}
				}
				word = 0;
				continue;
			}
			//---------------append to word
			if (is_format) {
				word = char_append(word, final_exp[i]);
			}
		}
		if (!is_change) {
			break;
		}
	}
	//***********************return
	//printf("####::%s\n", final_exp);
	return final_exp;
}

//*********************************************************
void calculate_string_expression(String exp, String *value, uint8 *sub_type) {
	/**
	1- "Ind: %i[0]%\n" ---OK---
	2- "Hello "+sd[0,1] ---OK---
	3- ""+"Hi" ---OK---
	*/
	//**************************define variables
	String final_exp = 0, buf = 0, str1 = 0, str2 = 0;
	//str_utf8 ustr1 = 0, ustr2 = 0;
	uint8 op = 0, type = 's';
	Boolean is_string = false, is_str1 = false;
	int32 pars = 0, bra = 0, starting = -1, ending = -1;
	//var byt byte = '0'
	//**************************start analyzing
	exp = remove_unused_parenthesis(exp);
	//msg("&UUUU:", exp)
	if (str_equal(exp, "null")) {
		str_init(&(*value), "\"\"");
		(*sub_type) = 's';
		return;
	} else if (str_equal(exp, "\"\\\"")) {
		str_init(&(*value), exp);
		(*sub_type) = 's';
		return;
	}
	exp = char_append(exp, '=');
	//******************
	for (int32 i = 0; i < str_length(exp); i++) {
		//---------------check is string
		if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\')) {
			is_string = switch_bool(is_string);
		}
		//---------------count pars
		if (!is_string) {
			if (exp[i] == ' ') continue;
			else if (exp[i] == '(' && bra == 0) {
				pars++;
				str1 = 0, str2 = 0, op = 0, buf = 0;
				starting = -1;
				continue;
			} else if (exp[i] == ')')pars--;
			else if (exp[i] == '[')bra++;
			else if (exp[i] == ']')bra--;
		}
		uint32 buf_len = str_length(buf);
		//---------------allocate str1
		if (!is_string && exp[i] == '+' && buf != 0 && op == 0 && !is_str1 && bra == 0) {
			Boolean is_valid_val = false;
			if (buf_len >= 2 && buf[0] == '\"' && buf[buf_len - 1] == '\"') {
				str1 = str_substring(buf, 1, buf_len - 1);
				is_valid_val = true;
				if (str_indexof(str1, UTF8_ID_LABEL, 0) == 0) {
					type = 'u';
					long_int uid = str_to_long_int(str_substring(str1, UTF8_ID_LBL_LEN, 0));
					str1 = utf8_to_bytes_string(get_utst(uid).utf8_string);
					//utf8_str_print("aaaaaa",ustr,true);
				}
			} else {
				Mpoint point = return_var_memory_value(buf);
				str_init(&str1, point.data);
				if (point.type_data == 's' || point.type_data == 'u') {
					is_valid_val = true;
				}
				if (point.type_data == 'u') {
					type = 'u';
					long_int uid = str_to_long_int(str_substring(str1, UTF8_ID_LBL_LEN, 0));
					str1 = utf8_to_bytes_string(get_utst(uid).utf8_string);
				}
				//msg("&CCDDD:", str1, buf, string(tmp1))
			}
			//***************
			if (!is_valid_val) {
				exception_handler("invalid_exp_val", "calculate_string_expression:136", buf, "str");
				str_init(&(*value), 0);
				(*sub_type) = '0';
				return;
			}
			//msg("&G1", str1)
			is_str1 = true;
			op = exp[i];
			buf = 0;
			continue;
		}
		//---------------allocate str2
		if (!is_string && (exp[i] == '=' || exp[i] == ')' || exp[i] == '+') && op != 0 && is_str1 && str2 == 0 &&
				buf != 0 && bra == 0) {
			ending = i;
			Boolean is_valid_val = false;
			if (buf_len >= 2 && buf[0] == '\"' && buf[buf_len - 1] == '\"') {
				str2 = str_substring(buf, 1, buf_len - 1);
				is_valid_val = true;
				if (str_indexof(str2, UTF8_ID_LABEL, 0) == 0) {
					type = 'u';
					long_int uid = str_to_long_int(str_substring(str2, UTF8_ID_LBL_LEN, 0));
					str2 = utf8_to_bytes_string(get_utst(uid).utf8_string);
					//utf8_str_print("aaaaaa",ustr,true);
				}
			} else if (str_is_num(buf) && (exp[i] == '=' || exp[i] == ')')) {
				str_init(&str2, buf);
				is_valid_val = true;
				if (str_indexof(str2, UTF8_ID_LABEL, 0) == 0) {
					type = 'u';
					long_int uid = str_to_long_int(str_substring(str2, UTF8_ID_LBL_LEN, 0));
					str2 = utf8_to_bytes_string(get_utst(uid).utf8_string);
					//utf8_str_print("aaaaaa",ustr,true);
				}
			} else {
				Mpoint point = return_var_memory_value(buf);
				str_init(&str2, point.data);
				if (point.type_data == 's') {
					is_valid_val = true;
				}
				if (point.type_data == 'u') {
					type = 'u';
					long_int uid = str_to_long_int(str_substring(str2, UTF8_ID_LBL_LEN, 0));
					str2 = utf8_to_bytes_string(get_utst(uid).utf8_string);
				}
				//msg("&KKKK:", buf, str2)
			}
			//msg("&G2", str1, str2, buf)
			if (!is_valid_val) {
				exception_handler("invalid_exp_val",__func__, buf, "str");
				str_init(&(*value), 0);
				(*sub_type) = '0';
				return;
			}
			//*************analyzing str1&str2 or ustr1&ustr2
			if (str_indexof(str1, "%", 0) > -1) {
				str1 = format_string_expression(str1);
				//printf("DDDDD:%s,%s\n",str1,str2);
			}
			if (str_indexof(str2, "%", 0) > -1) {
				str2 = format_string_expression(str2);
			}
			
			//msg("&G2", str1, str2, buf)
			//*************calculate str1&str2
			String result = 0;
			if (op == '+') {
				result = str_multi_append("\"", str1, str2, "\"", 0, 0);
			}
			//printf("#RRRR:%s,%s,%s[%s,%i,%i]\n",str1,str2,result,exp,starting,ending);
			//*************replace the result
			String s1 = 0, s2 = 0, expression = 0;
			s1 = str_trim_space(str_substring(exp, 0, starting));
			s2 = str_trim_space(str_substring(exp, ending, 0));
			if (str_length(s1) > 0 && s1[str_length(s1) - 1] == '(' && s2[0] == ')') {
				s1 = str_substring(s1, 0, str_length(s1) - 1);
				s2 = str_substring(s2, 1, 0);
			}
			expression = str_multi_append(s1, result, s2, 0, 0, 0);
			//printf("$RRRR:%s,%s=>%s\n",s1,s2,expression);
			str_init(&exp, expression);
			//printf("##DDDDDD:%s,%s,%s,%s\n",result,exp,str1,str2);
			//*************end calculate
			op = 0, str1 = 0, str2 = 0, buf = 0;
			is_str1 = false;
			starting = -1, pars = 0;
			i = -1;
			continue;
		}
		//---------------end of calculation
		if (!is_string && buf != 0 && !is_str1 && str2 == 0 && op == 0 && exp[i] == '=') {
			Boolean is_str_style = false;
			Boolean is_valid_val = false;
			//printf("&HHH:%s\n",buf);
			if (buf_len > 1 && buf[0] == '\"' && buf[buf_len - 1] == '\"') {
				is_str_style = true;
				is_valid_val = true;
				if (str_indexof(buf, UTF8_ID_LABEL, 0) == 0) {
					type = 'u';
					long_int uid = str_to_long_int(str_substring(buf, UTF8_ID_LBL_LEN, 0));
					buf = utf8_to_bytes_string(get_utst(uid).utf8_string);
					//utf8_str_print("aaaaaa",ustr,true);
				}
			} else if (is_valid_name(buf, true)) {
				String buf1 = 0;
				str_init(&buf1, buf);
				Mpoint point = return_var_memory_value(buf);
				str_init(&buf, point.data);
				is_str_style = false;
				if (point.type_data == 's' || point.type_data == 'n' || point.type_data == 'u') {
					is_valid_val = true;
				} else {
					//msg("&Str", buf, string(tmp1), is_valid_val)
					str_init(&buf, buf1);
				}
				if (point.type_data == 'u') {
					type = 'u';
					long_int uid = str_to_long_int(str_substring(buf, UTF8_ID_LBL_LEN, 0));
					buf = utf8_to_bytes_string(get_utst(uid).utf8_string);
				}
			}
			if (!is_valid_val) {
				exception_handler("invalid_exp_val", __func__, buf, "str");
				str_init(&(*value), 0);
				(*sub_type) = '0';
				return;
			}
			//msg("&AAAA:", buf)
			if (str_indexof(buf, "%", 0) > -1) {
				buf = format_string_expression(buf);
			}
			//---------------if utf8
			String utf8_buf = 0;
			if (!is_str_style) {
				if (type == 'u') str_init(&utf8_buf, buf);
				buf = str_multi_append("\"", buf, "\"", 0, 0, 0);
			} else if (type == 'u') {
				str_init(&utf8_buf, str_substring(buf, 1, buf_len - 1));
			}
			if (type == 'u') {
				//printf("QQ##A:%s\n",utf8_buf);
				str_utf8 utf8 = utf8_encode_bytes(utf8_buf);
				//utf8_str_print("XXXX",utf8,true);
				utst tmp1 = {++entry_table.utf8_strings_id, utf8, utf8_str_max_bytes(utf8, false)};
				append_utst(tmp1);
				buf = str_multi_append(UTF8_ID_LABEL, str_from_long_int(entry_table.utf8_strings_id), 0, 0, 0, 0);
			}
			//---------------break
			str_init(&final_exp, buf);
			break;
		}
		//---------------append to buffer
		buf = char_append(buf, exp[i]);
		if (starting == -1) {
			starting = i;
		}
	}
	//**************************return final_exp
	str_init(&(*value), final_exp);
	(*sub_type) = type;
	return;
}
//*********************************************************
/**
 * get an math expression with a target type and calculate it and return it.
 * @param exp
 * @param target_type
 * @param retval
 * @param rettype
 */
void calculate_math_expression(String exp, uint8 target_type, String *retval, uint8 *rettype) {
	//***********************define variables
	String final_exp = 0;
	str_init(&final_exp, "0");
	int32 pars = 0, bra = 0, starting = -1, ending = -1;
	String num1 = 0, num2 = 0, buf = 0;
	uint8 op = 0, type_exp = target_type, ty1 = '_', ty2 = '_';
	//***********************start analyzing
	exp = str_trim_space(exp);
	//----------------if is '0'||null||'?'
	if (str_equal(exp, "0") || str_equal(exp, "null") || str_equal(exp, "?")) {
		if (type_exp == '_') {
			type_exp = 'i';
		}
		if (str_equal(exp, "?")) {
			str_init(&final_exp, "?");
		}
		str_init(&(*retval), final_exp);
		(*rettype) = type_exp;
		return;
	}
	//----------------if is ()
	exp = remove_unused_parenthesis(exp);
	uint32 len_exp = str_length(exp);
	
	if (len_exp == 1 && (exp[0] < '0' || exp[0] > '9')) {
		str_init(&(*retval), "0");
		(*rettype) = 'i';
		return;
	}
	exp = char_append(exp, '=');
	len_exp++;
	//printf("&UUU:%s,%c,%i\n", exp, type_exp,len_exp);
	//******************
	for (uint32 i = 0; i < len_exp; i++) {
		if (exp[i] == ' ')continue;
		else if (exp[i] == '(' && bra == 0) {
			pars++;
			op = 0, num1 = 0, num2 = 0, buf = 0;
			starting = -1;
			continue;
		} else if (exp[i] == ')')pars--;
		else if (exp[i] == '[')bra++;
		else if (exp[i] == ']')bra--;
		//---------------allocate num1
		if (char_search(single_operators, exp[i]) && buf != 0 && num1 == 0 && op == 0 && bra == 0) {
			//msg("&UYYY:", buf)
			Boolean is_valid_val = false;
			buf = remove_unused_parenthesis(buf);
			if (str_is_num(buf)) {
				str_init(&num1, buf);
				is_valid_val = true;
				ty1 = set_type_of_math(&num1, 0);
			} else {
				Mpoint mpoint = return_var_memory_value(buf);
				if (mpoint.type_data == 'i' || mpoint.type_data == 'f' || mpoint.type_data == 'h') {
					str_init(&num1, mpoint.data);
					is_valid_val = true;
					type_exp = priority_math_type(type_exp, mpoint.type_data);
					ty1 = type_exp;
				}
			}
			//*************
			if (!is_valid_val) {
				exception_handler("invalid_exp_val", __func__, buf, "num");
				str_init(&(*retval), "0");
				(*rettype) = '0';
				return;
			}
			//*************
			
			op = exp[i];
			//printf("&Buf1:%s,%s,%c,%c\n", buf, num1, type_exp,op);
			buf = 0;
			//msg("&EEEE:", num1, op)
			continue;
		}
		//---------------allocate num2
		if ((char_search(single_operators, exp[i]) || exp[i] == '=' || exp[i] == ')') && buf != 0 && num2 == 0 &&
				num1 != 0 && op != 0 && bra == 0) {
			buf = str_trim_space(buf);
			buf = remove_unused_parenthesis(buf);
			Boolean is_valid_val = false;
			ending = i;
			if (str_is_num(buf)) {
				str_init(&num2, buf);
				is_valid_val = true;
				ty2 = set_type_of_math(&num2, 0);
			} else {
				Mpoint mpoint = return_var_memory_value(buf);
				
				if (mpoint.type_data == 'i' || mpoint.type_data == 'f' || mpoint.type_data == 'h') {
					str_init(&num2, mpoint.data);
					is_valid_val = true;
					type_exp = priority_math_type(type_exp, mpoint.type_data);
					ty2 = type_exp;
				}
				//msg("##NUM2:", num2, buf, cur_fin)
			}
			//*************
			if (!is_valid_val) {
				exception_handler("invalid_exp_val", "calculate_math_expression", buf, "num");
				str_init(&(*retval), "0");
				(*rettype) = '0';
				return;
			}
			//*************detrmine type_exp
			if (ty1 == '_' || ty2 == '_') {
				//msg("&&&", num1, num2)
				uint8 tmp23 = '_';
				tmp23 = set_type_of_math(&num1, &num2);
				type_exp = priority_math_type(type_exp, tmp23);
			} else {
				type_exp = ty1;
				type_exp = priority_math_type(type_exp, ty2);
			}
			//msg("&TYpe", num1, num2, string(type_exp), string(ty1), string(ty2))
			//**************analyze num1&num2
			//msg("&&BBB",buf)
			if (str_indexof(num1, "0x", 0) >= 0 || str_indexof(num1, "0b", 0) >= 0 || str_indexof(num1, "0o", 0) >= 0) {
				num1 = is_radix_need_convert(num1, type_exp);
			}
			if (str_indexof(num2, "0x", 0) >= 0 || str_indexof(num2, "0b", 0) >= 0 || str_indexof(num2, "0o", 0) >= 0) {
				num2 = is_radix_need_convert(num2, type_exp);
			}
			//*************calculate num1&num2
			String result = 0;
			str_init(&result, "0");
			
			result = calculate_two_numbers(num1, num2, op, type_exp);
			//*************resize result
			if (type_exp == 'i' && (str_length(result) > MAX_INT_LEN || str_indexof(result, ".", 0) >= 0)) {
				result = resize_to_int(result);
			} else if (type_exp == 'f' && str_length(result) > MAX_FLOAT_LEN) {
				result = resize_to_float(result);
			}
			//msg("&VVV", num1, num2, op, string(type_exp), result)
			//*************replace the result
			String s1 = 0, s2 = 0, expression = 0;
			s1 = str_trim_space(str_substring(exp, 0, starting));
			s2 = str_trim_space(str_substring(exp, ending, 0));
			//printf("&Buf2:%s,%s,%s,%c,%c=>%s||%s:(%s,%s)[%i,%i]\n", buf, num2, num1,op,type_exp,result,exp,s1,s2,starting,ending);
			uint32 s1_len = str_length(s1);
			if (s1_len > 0 && s1[s1_len - 1] == '(' && s2[0] == ')') {
				s1 = str_substring(s1, 0, s1_len - 1);
				s2 = str_substring(s2, 1, 0);
			}
			expression = str_multi_append(s1, result, s2, 0, 0, 0);
			str_init(&exp, expression);
			
			//*************end calculate
			//msg("DDDD:", starting)
			//fmt.Printf("CALC:%s%s%s=%s\n\t%s\n", num1, op, num2, result, exp)
			op = 0, num1 = 0, num2 = 0, buf = 0;
			starting = -1, pars = 0;
			i = -1;
			continue;
		}
		//---------------end of calculation
		if (buf != 0 && num1 == 0 && num2 == 0 && op == 0 && exp[i] == '=') {
			Boolean is_valid_val = false;
			//printf("&www:%s=>%s\n",exp, buf);
			//*************detrmine type_exp
			if (type_exp == '_' && !is_valid_name(buf, true)) {
				type_exp = set_type_of_math(&buf, 0);
				if (type_exp != '0') {
					is_valid_val = true;
				}
				//msg("&UUUUU", buf, string(type_exp), is_valid_val)
			}
			//msg("&&BBB",buf)
			if (is_valid_name(buf, true)) {
				str_init(&buf, return_var_memory_value(buf).data);
				//msg("&%%%%:", buf)
			}
			if (str_indexof(buf, "0x", 0) > -1 || str_indexof(buf, "0b", 0) > -1 || str_indexof(buf, "0o", 0) > -1) {
				buf = is_radix_need_convert(buf, type_exp);
			}
			
			//msg("&NNN",buf,is_ok,string(type_exp))
			if (str_is_num(buf)) {
				is_valid_val = true;
			} else if (str_equal(buf, "null")) {
				is_valid_val = true;
				str_init(&buf, "0");
			}
			//*************
			if (!is_valid_val) {
				exception_handler("invalid_exp_val", "calculate_math_expression:889", buf, "num");
				str_init(&(*retval), "0");
				(*rettype) = '0';
				return;
			}
			//*************resize buf
			uint32 buf_len = str_length(buf);
			if (type_exp == 'i' && (buf_len > MAX_INT_LEN || str_indexof(buf, ".", 0) > 0)) {
				buf = resize_to_int(buf);
			} else if (type_exp == 'f' && buf_len > MAX_FLOAT_LEN) {
				buf = resize_to_float(buf);
			}
			str_init(&final_exp, buf);
			break;
		}
		//---------------append to buf
		buf = char_append(buf, exp[i]);
		if (starting == -1) {
			starting = i;
		}
	}
	//***********************return final_exp,type_exp
	//printf("RESULT:%s=>%s[%c]\n",exp, final_exp, type_exp);
	if (type_exp == '_') {
		type_exp = 'i';
	}
	//-----------------return all
	str_init(&(*retval), final_exp);
	(*rettype) = type_exp;
}

//*********************************************************
/**
 * get two string numbers and based of operator and sub_type(i,f,h),calculate their and return result of it
 * @param num1
 * @param num2
 * @param op
 * @param type_exp
 * @return String
 */
String calculate_two_numbers(String num1, String num2, uint8 op, uint8 type_exp) {
	String result = 0;
	str_init(&result, "0");
	//msg("&$$$", num1, num2, op, string(type_exp))
	//*********************************search for errors and warnings
	if (op == '/' && str_equal(num2, "0")) {
		exception_handler("zero_division", "calculate_two_numbers", str_multi_append(num1, "/", num2, 0, 0, 0), "");
		return "0";
	} else if (op == '%' && (type_exp == 'f' || type_exp == 'h')) {
		//com_type, _, _ := fitting_value(string(type_exp), "", "com_type")
		//exception_handler("wrong_remainder", "calculate_two_numbers", com_type, "")
		//TODO:error
		return "0";
	}
	
	//*********************************calculate num1,num2
	//----------------int
	if (type_exp == 'i') {
		num1 = resize_to_int(num1);
		num2 = resize_to_int(num2);
		int32 n1 = str_to_int32(num1);
		int32 n2 = str_to_int32(num2);
		
		if (op == '/' && n2 == 0) {
			exception_handler("zero_division", "calculate_two_numbers", str_multi_append(num1, "/", num2, 0, 0, 0), "");
			return "0";
		}
		//msg("&TTT:", num1, n1, num2, n2)
		switch (op) {
			case '+':
				result = str_from_int32(n1 + n2);
				break;
			case '-':
				result = str_from_int32(n1 - n2);
				break;
			case '*':
				result = str_from_int32(n1 * n2);
				break;
			case '/':
				result = str_from_int32(n1 / n2);
				break;
			case '%':
				result = str_from_int32(n1 % n2);
				break;
			case '^': {
				double ret = int32_power((double) n1, n2);
				if (ret > MAX_INT_NUMBER) {
					exception_handler("out_of_range_integer", "calculate_two_numbers",
							str_multi_append(num1, "^", num2, 0, 0, 0), "");
					return "0";
				}
				//printf("DDDDDDD:%li\n",ret);
				result = str_from_int32((int32) ret);
				break;
			}
		}
	}
		//----------------float
	else if (type_exp == 'f') {
		num_float n1 = str_to_double(num1);
		num_float n2 = str_to_double(num2);
		switch (op) {
			case '+':
				result = str_from_double(n1 + n2, (uint8) str_length(num1) / 2);
				break;
			case '-':
				result = str_from_double(n1 - n2, (uint8) str_length(num1) / 2);
				break;
			case '*':
				result = str_from_double(n1 * n2, (uint8) str_length(num1));
				break;
			case '/':
				result = str_from_double(n1 / n2, (uint8) str_length(num1));
				break;
			case '^': {
				double ret = int32_power(n1, (int32) n2);
				if (ret > MAX_FLOAT_NUMBER) {
					exception_handler("out_of_range_float", "calculate_two_numbers",
							str_multi_append(num1, "^", num2, 0, 0, 0), "");
					return "0";
				}
				//printf("DDDDDDD:%f\n",ret);
				result = str_from_double(ret, (uint8) (str_length(num1) * 2));
				break;
			}
		}
	}
	//----------------huge
	//TODO:
	/*else if (type_exp == 'h') {
		//msg("&HHH:", num1, num2, op)
		switch {
			case op == "+":
				result = sum_huge_numbers(num1, num2)
			case op == "-":
				result = sum_huge_numbers(num1, "-" + num2)
			case op == "*":
				result = muliply_huge_numbers(num1, num2)
			case op == "/":
				result = divide_huge_numbers(num1, num2)
			case op == "^":
				num2, _ := detachment_float_string(num2)
				n2, _ := strconv.ParseInt(num2, 10, 64)
				var
				is_neg = false
				var
				b
				int64 = 0
				var
				res = "1"
				if n2 < 0
				{
					is_neg = true
					n2 = -n2
				}
				
				for
				b = 0;
				b < n2;
				b++
				{
					res = muliply_huge_numbers(res, num1)
				}
				if is_neg
				{
					res = divide_huge_numbers("1", res)
				}
				result = res
		}
}*/
	
	return result;
}

//*********************************************************
/**
 * determine a string is a radix number and then convert it to decimal number and return it
 * @param buf
 * @param type_exp
 * @return String
 */
String is_radix_need_convert(String buf, uint8 type_exp) {
	Boolean is_neg = false;
	if (str_length(buf) > 1 && (buf[0] == '-' || buf[0] == '+')) {
		if (buf[0] == '-') {
			is_neg = true;
		}
		buf = str_substring(buf, 1, 0);
	}
	if (str_indexof(buf, "0b", 0) == 0)
		buf = convert_radixes_to_dec(buf, 2, type_exp);
	else if (str_indexof(buf, "0o", 0) == 0)
		buf = convert_radixes_to_dec(buf, 8, type_exp);
	else if (str_indexof(buf, "0x", 0) == 0)
		buf = convert_radixes_to_dec(buf, 16, type_exp);
	
	if (is_neg) {
		buf = str_multi_append("-", buf, 0, 0, 0, 0);
	}
	return buf;
}

//*********************************************************
/**
 * get a string base and convert it based on radix number and type char and then return a string decimal
 * @param base
 * @param radix
 * @param type_exp
 * @return String
 */
String convert_radixes_to_dec(String base, uint8 radix, uint8 type_exp) {
	//msg("&^^", base, radix)
	String final = 0, int_s = 0, float_s = 0;
	uint8 start_x, after_point = 1;
	Boolean is_point = false, is_base = false;
	double res_ii = 0, res_ff = 0;
	String nmb = 0;
	switch (radix) {
		case 2:
			start_x = 'b';
			str_init(&nmb, "01");
			break;
		case 8:
			start_x = 'o';
			str_init(&nmb, "01234567");
			break;
		case 16:
			start_x = 'x';
			str_init(&nmb, "0123456789abcdef");
			break;
	}
	
	//*******************
	if (base[0] == '-') {
		final = char_append(final, '-');
	}
	base = str_to_lower_case(base);
	//printf("JJJJ:%s\n",base);
	//*******************split int and float of radix number to int_s,float_s
	uint32 len_base = str_length(base);
	for (uint32 i = 0; i < len_base; i++) {
		if (base[i] == '.') {
			is_point = true;
			continue;
		}
		if (base[i] == start_x && i > 0 && base[i - 1] == '0') {
			is_base = true;
			continue;
		}
		if (!is_point && is_base && char_search(nmb, base[i])) {
			int_s = char_append(int_s, base[i]);
		} else if (is_point && is_base && char_search(nmb, base[i])) {
			float_s = char_append(float_s, base[i]);
		}
	}
	
	//*******************convert int_s to decimal
	int32 b = str_length(int_s) - 1;
	for (uint32 i = 0; i < str_length(int_s); i++) {
		double n3 = int32_power(radix, b);
		b--;
		
		int32 n4 = char_search_index(nmb, int_s[i]);
		res_ii += (n4 * n3);
	}
	//******************convert float_s radix to decimal
	if (type_exp != 'i' && type_exp != '_' && type_exp != '0') {
		b = -1;
		for (uint32 i = 0; i < str_length(float_s); i++) {
			double n3 = int32_power(radix, b);
			b--;
			int32 n4 = char_search_index(nmb, float_s[i]);
			res_ff += (n4 * n3);
			//printf("GGGGG:%i,%i,%f#%i,%f=>%f\n",radix, b,n3,n4,n4 * n3,res_ff);
		}
		res_ii += res_ff;
		switch (radix) {
			case 2:
				after_point = (uint8) str_length(float_s);
				break;
			case 8:
				after_point = (uint8) str_length(float_s);
				break;
			case 16:
				after_point = (uint8) str_length(float_s) * 2;
				break;
		}
	}
	//*****************
	String fin = str_from_double(res_ii, after_point);
	//printf("CCCCC:%s,%s=>%f,%i,%s\n", int_s, float_s, res_ii, str_length(float_s), fin);
	final = str_append(final, fin);
	return final;
}

//*********************************************************
/**
 * get a string number and resize it to integer number
 * @param str_val
 * @return String
 */
String resize_to_int(String str_val) {
	String fin_res = 0;
	for (uint32 i = 0; i < str_length(str_val); i++) {
		if (str_val[i] == '.' || str_length(fin_res) > MAX_INT_LEN) {
			break;
		}
		fin_res = char_append(fin_res, str_val[i]);
	}
	return fin_res;
}

//**************************************************************
/**
 * get a string number and resize it to float number
 * @param str_val
 * @return String
 */
String resize_to_float(String str_val) {
	String fin_res = 0;
	uint32 count = 0;
	for (uint32 i = 0; i < str_length(str_val); i++) {
		if (count >= MAX_FLOAT_LEN) {
			break;
		}
		if (str_val[i] != '-' && str_val[i] != '+' && str_val[i] != '.') {
			count++;
		}
		fin_res = char_append(fin_res, str_val[i]);
	}
	if (fin_res[str_length(fin_res) - 1] == '.') {
		fin_res = char_append(fin_res, '0');
	}
	return fin_res;
}

/**
 * get two sub_types and comparison and priority for their
 * @param type_exp
 * @param tmp1
 * @return uint8
 */
uint8 priority_math_type(uint8 type_exp, uint8 tmp1) {
	if (tmp1 == 'h' || type_exp == 'h')
		return 'h';
	if (tmp1 == 'f' || type_exp == 'f')
		return 'f';
	if (tmp1 == 'i' && type_exp == 'i')
		return 'i';
	return 'i';
}
//*********************************************************
/**
 * get expression string and remove its unused parenthesis
 * @param value
 * @return String
 */
String remove_unused_parenthesis(String value) {
	/**
		- (56+6) => 56+6
		- ((5+5)+6) => (5+5)+6
		- ((6-7)*(6-6)) => (6-7)*(6-6)
		- (5-7)*(6*9) => 5-7)*(6*9)
		*/
	uint32 len = str_length(value);
	if (len > 2 && value[0] == '(' && value[len - 1] == ')') {
		Boolean is_remove = false, is_string = false, is_br = false;
		uint8 pars = 0;
		for (uint32 i = 0; i < len; i++) {
			if (value[i] == '\"' && (i == 0 || value[i - 1] != '\\')) {
				is_string = switch_bool(is_string);
			}
			
			if (!is_string && value[i] == '(')pars++;
			else if (!is_string && value[i] == ')') {
				pars--;
				if (i + 1 == len && pars == 0) {
					is_remove = true;
					is_br = true;
				} else if (i + 1 < len && pars == 0) {
					is_br = true;
				}
			}
			if (is_br) {
				break;
			}
		}
		if (is_remove) {
			value = str_substring(value, 1, len - 1);
		}
	}
	return value;
}

//*********************************************************
/**
 * get num1,num2 and determine their sub_type and if need change num1,num2
 * @param num11
 * @param num22
 * @return uint8
 */
uint8 set_type_of_math(String *num11, String *num22) {
	String num1 = 0;
	String num2 = 0;
	str_init(&num1, (*num11));
	if (num22 != 0)str_init(&num2, (*num22));
	
	uint32 num1_len = str_length(num1);
	uint32 num2_len = (num22 != 0) ? str_length(num2) : 0;
	uint8 type_exp = '0';
	Boolean is_hex1 = false, is_hex2 = false;
	if (num1_len > 2 && num1[0] == '0' && num1[1] == 'x') {
		is_hex1 = true;
	} else if (num2_len > 2 && num2[0] == '0' && num2[1] == 'x') {
		is_hex2 = true;
	}
	uint8 num2_t = '0';
	uint8 num1_t = determine_type_num(num1);
	if (num2 != 0) {
		num2_t = determine_type_num(num2);
	}
	//msg("&XXX", string(type_exp), num1, num2, string(num1_t), string(num2_t))
	if (num1_t == '0' || (num2_t == '0' && num2 != 0)) {
		str_init(&(*num11), num1);
		if (num22 != 0)str_init(&(*num22), num2);
		return '0';
	} else if (num1_t == 'h' || num2_t == 'h')
		type_exp = 'h';
	else if (num1_t == 'f' || num2_t == 'f')
		type_exp = 'f';
	else type_exp = 'i';
	if (num1_len > 0 && !is_hex1 &&
			((num1[num1_len - 1] == 'i' && type_exp == 'i') || (num1[num1_len - 1] == 'f' && type_exp == 'f') ||
					(num1[num1_len - 1] == 'h' && type_exp == 'h'))) {
		num1 = str_substring(num1, 0, num1_len - 1);
	}
	
	if (num2_len > 0 && !is_hex2 &&
			((num2[num2_len - 1] == 'i' && type_exp == 'i') || (num2[num2_len - 1] == 'f' && type_exp == 'f') ||
					(num2[num2_len - 1] == 'h' && type_exp == 'h'))) {
		num2 = str_substring(num2, 0, num2_len - 1);
	}
	//printf("DDFFFF:%s,%i\n", num1, num2_len);
	//msg("&XXX", string(type_exp), num1, num2, string(num1_t), string(num2_t))
	str_init(&(*num11), num1);
	if (num22 != 0)str_init(&(*num22), num2);
	return type_exp;
}

//*********************************************************
/**
 * get a num and determine its sub_type and return 'i','f','h' or '0'
 * @param num
 * @return uint8
 */
uint8 determine_type_num(String num) {
	//**********************define
	uint8 num_type = '0';
	uint32 len = str_length(num);
	//var is_neg=false
	if (len > 3 && (num[0] == '-' || num[0] == '+') && num[1] == '0' &&
			(num[2] == 'b' || num[2] == 'o' || num[2] == 'x')) {
		if (num[0] == '-') {
			//is_neg=true
		}
		num = str_substring(num, 1, 0);
	}
	//-------------------------------
	if (len == 0) {
		return num_type;
	}
	Boolean is_hex = false;
	if (len > 2 && str_indexof(num, "0x", 0) == 0) {
		is_hex = true;
	}
	//**********************analyze hexdecimal
	if (is_hex) {
		int32 ind = str_indexof(num, ".", 0);
		if (ind == -1) {
			return 'd';
		} else {
			return 'i';
		}
	}
	//**********************analyze 1
	if (len > 1 && !is_hex && (num[len - 2] >= '0' && num[len - 2] <= '9')) {
		switch (num[len - 1]) {
			case 'i':
				return 'i';
			case 'f':
				return 'f';
			case 'h':
				return 'h';
		}
	}
	//**********************analyze 2
	if (!str_is_num(num)) {
		return '0';
	}
	int32 before = 0, after = 0, max_before = 0, max_after = 0;
	Boolean is_point = false;
	for (uint32 i = 0; i < len; i++) {
		if (num[i] == '.') {
			is_point = true;
			continue;
		}
		if (num[i] == ',') {
			if (after > max_after) {
				max_after = after;
			}
			if (before > max_before) {
				max_before = before;
			}
			after = 0, before = 0;
			is_point = false;
			continue;
		}
		if (!is_point && num[i] >= '0' && num[i] <= '9') {
			before++;
		} else if (is_point && num[i] >= '0' && num[i] <= '9') {
			after++;
		}
	}
	if (after > max_after) {
		max_after = after;
	}
	if (before > max_before) {
		max_before = before;
	}
	//*************
	if (max_after == 0 && max_before < MAX_INT_LEN)
		return 'i';
	if (max_after > 0 && max_before + max_after < MAX_FLOAT_LEN)
		return 'f';
	else
		return 'h';
	//return '0';
	
}
//*********************************************************
//*********************************************************



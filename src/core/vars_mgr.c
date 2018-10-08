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
	if (str_equal(interpreter_level, "parse"))str_init(&entry_table.Rsrc, entry_table.cur_ascii_source_path);
	//*******************************************is null
	inst = str_trim_space(inst);
	if (str_is_empty(inst)) return 0;
	String word = 0, last_type = 0, data_type = 0;
	Boolean is_string = false, is_equal = false, is_struct = false;
	int16 vars_counter = -1, vals_counter = -1, pars = 0, bras = 0, acos = 0;
	//*******************************************determine variables with their values
	uint32 len = str_length(inst);
	for (uint32 i = 0; i < len; i++) {
		//------------------check is string
		if (inst[i] == '\"' && (i == 0 || inst[i - 1] != '\\')) {
			is_string = switch_bool(is_string);
		}
		//------------------check is struct
		if (pars == 0 && inst[i] == '(' && str_equal(word, "struct")) {
			is_struct = true;
		} else if (pars == 1 && inst[i] == ')') {
			is_struct = false;
		}
		//------------------continue if ' '
		if (!is_string && i + 1 < len && inst[i] == ' ' && (inst[i + 1] == '(' || inst[i + 1] == '[')) {
			continue;
		}
		//------------------count bras,pars,acos
		if (!is_string) {
			if (inst[i] == '(')pars++;
			else if (inst[i] == ')')pars--;
			else if (inst[i] == '{')acos++;
			else if (inst[i] == '}')acos--;
			else if (inst[i] == '[')bras++;
			else if (inst[i] == ']')bras--;
			else if (inst[i] == '=')is_equal = true;
		}
		//------------------determine data type
		if (!is_string && !is_equal && word != 0 && inst[i] == ' ' &&
				(is_valid_name(word, false) || str_equal(word, "vars") || str_equal(word, "num") ||
						str_equal(word, "str") || str_equal(word, "bool")) &&
				(str_equal(interpreter_level, "parse") || search_datas(word, 0, true).id != 0)) {
			//printf("EEEEDDDDDDD:%s\n",word);
			str_init(&data_type, word);
			word = 0;
			continue;
		}
		//------------------store a variable
		//printf("QQ!!:%s\n",word);
		if (!is_string && pars == 0 && acos == 0 && bras == 0 &&
				((!is_equal && (inst[i] == ',' || i + 1 == len)) || inst[i] == '=') && data_type != 0) {
			if (i + 1 == len) {
				word = char_append(word, inst[i]);
			}
			//printf("DDDD@#DDDDDDD:%s;%s;%s\n",inst,data_type, word);
			word = str_trim_space(word);
			if (!is_valid_name(word, true)) {
				exception_handler("invalid_name_var", __func__, word, "");
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
				vars_store[vars_counter].sid = entry_table.cur_sid;
				vars_store[vars_counter].fid = entry_table.cur_fid;
				//printf("&SDSS:%s,%s,%s=>\n", word, data_type,ret.name_var);
				str_init(&last_type, data_type);
			}
			word = 0;
			continue;
		}
		//------------------allocate values to variables
		//msg("VVVV:", word, "|MMM:", pars)
		if (is_equal && !is_string && (word != 0 || i + 1 == len) &&
				((inst[i] == ',' && acos == 0) || (i + 1 == len && acos < 2)) && pars == 0 && bras == 0) {
			if (i + 1 == len) {
				word = char_append(word, inst[i]);
			}
			if (inst[i] == '}') acos--;
			else if (acos > 0)continue;
			/*if vals_counter > vars_counter {
				//msg("&SSS", vals_counter, vars_counter)
				exception_handler("wrong_def_var", "define_vars_analyzing", word, "")
			} else */
			
			vals_counter++;
			//printf("UUU:%s,%s,%i\n", word,vars_store[vals_counter].main_type,vals_counter);
			word = str_trim_space(word);
			String main_type = vars_store[vals_counter].main_type;
			vars_store[vals_counter].sub_type = '0';
			str_init(&vars_store[vals_counter].value_var, word);
			word = 0;
		}
		
		//------------------append to word
		word = char_append(word, inst[i]);
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
	Mvar mvar = get_Mvar(var_index);
	long_int po_id = mvar.pointer_id;
	String final_value = 0;
	str_list store_po;
	uint32 items_count = 0, store_po_len = 0;
	//printf("!!!!!#################:%i,%i,%s\n",var_index,po_id,mvar.name);
	//show_memory(0)
	//printf("-----------------------VAR_COMPLETEL:%s %i:\n", mvar.name, mvar.type_var);
	//******************analyzing
	for (;;) {
		//printf("Store_Po:%s\n", print_str_list(store_po, store_po_len));
		if (store_po_len > 0) {
			//end of array or struct
			if (str_equal(store_po[0], "p") || str_equal(store_po[0], "l")) {
				if (str_equal(store_po[0], "p"))final_value = char_append(final_value, '}');
				else final_value = char_append(final_value, ')');
				str_list_delete_first(&store_po, store_po_len--);
				if (store_po_len > 0 && !str_equal(store_po[0], "p") && !str_equal(store_po[0], "l")) {
					final_value = char_append(final_value, ',');
					//items_count++;
				} else if (store_po_len == 0)break;
				continue;
			} else {
				po_id = str_to_long_int(store_po[0]);
				str_list_delete_first(&store_po, store_po_len--);
			}
		}
		if (po_id == 0 && store_po_len == 0) break;
		long_int po_ind = find_index_pointer_memory(po_id);
		if (po_ind == 0) break;
		//msg("RTE:", pointer_memory[po_ind])
		Mpoint mpoint = get_Mpoint(po_ind);
		//if type is a pointer or a struct
		if (mpoint.type_data == 'p' || mpoint.type_data == 'l') {
			str_list st;
			uint8 end_node = '0';
			uint32 count = char_split(mpoint.data, ';', &st, true);
			if (mpoint.type_data == 'p' && count > 1) {
				final_value = char_append(final_value, '{');
				end_node = 'p';
			} else if ((mpoint.type_data == 'l')) {
				final_value = str_append(final_value, "struct(");
				end_node = 'l';
			}
			str_list tmp_store = 0;
			uint32 tmp_store_len = 0;
			for (uint32 b = 0; b < count; b++) {
				str_list_append(&tmp_store, st[b], tmp_store_len++);
			}
			if (end_node != '0') {
				str_list_append(&tmp_store, char_to_str(end_node), tmp_store_len++);
			}
			store_po_len = str_list_join(&store_po, tmp_store, tmp_store_len, store_po, store_po_len);
		}
			//if type is a data
		else {
			if (mpoint.type_data == 's') {
				str_init(&mpoint.data, convert_to_string(mpoint.data));
			} else if (mpoint.type_data == 'i' || mpoint.type_data == 'f' || mpoint.type_data == 'h') {
				mpoint.data = char_append(mpoint.data, mpoint.type_data);
			}
			final_value = str_append(final_value, mpoint.data);
			if (store_po_len > 0 && !str_equal(store_po[0], "p") && !str_equal(store_po[0], "l")) {
				final_value = char_append(final_value, ',');
				//items_count++;
			}
			//msg("&CCXXXX:", var_memory[var_index].name, data, string(typ))
		}
	}
	/*if (items_count == 0 && str_length(final_value) > 1) {
		final_value = str_substring(final_value, 1, str_length(final_value) - 1);
	}*/
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
 * get a struct value and its type and create a collection of nodes for struct and return it
 * @param value
 * @param type_var
 * @return vaar_en
 */
String calculate_struct_expression(String value, String type_var, uint8 *sub_type) {
	/**
	 * struct(b,y[0,1],"gh",null)
	 * struct(struct(5.6,{true,false}),true||false,bn,{{6,8},{8.8,0x56}})
	 * struct() => null all entries
	 */
	(*sub_type) = 'l';
	stde struct_node = {++entry_table.stde_id, type_var, {0, 0, 0}};
	//printf("####:%s,%s\n",value,type_var);
	uint32 val_len = str_length(value);
	String buf = 0;
	Boolean is_string = false, is_struct = false;
	str_list st_values = 0;
	uint32 st_values_len = 0;
	int32 pars = 0, acos = 0, bras = 0;
	//-------------------- get struct items
	datas s = search_datas(type_var, 0, true);
	str_list st_params = s.params;
	uint32 st_params_len = s.params_len;
	//-------------------- split struct params
	//printf("!!#:%s,%s\n", value, type_var);
	for (uint32 i = 0; i < val_len; i++) {
		//========is string
		if (value[i] == '\"' && (i == 0 || value[i - 1] != '\\')) {
			is_string = switch_bool(is_string);
		}
		//========count pars,bras,acos
		if (!is_string) {
			if (value[i] == '(')pars++;
			else if (value[i] == ')')pars--;
			else if (value[i] == '[')bras++;
			else if (value[i] == ']')bras--;
			else if (value[i] == '{')acos++;
			else if (value[i] == '}')acos--;
		}
		//========is struct
		if (!is_string && value[i] == '(' && pars == 1 && bras == 0 && acos == 0 &&
				str_equal(str_trim_space(buf), "struct")) {
			is_struct = true;
			buf = 0;
			continue;
		}
		//========get a value
		if (!is_string && is_struct && bras == 0 && acos == 0 &&
				((pars == 1 && value[i] == ',') || (value[i] == ')' && pars == 0))) {
			//printf("$$#$#:%s\n", buf);
			str_list_append(&st_values, str_trim_space(buf), st_values_len++);
			buf = 0;
			if (value[i] == ')')is_struct = false;
			continue;
		}
		//========append to buf
		buf = char_append(buf, value[i]);
	}
	//--------------------check errors by struct params
	//TODO:
	//--------------------create vals_array
	for (uint32 j = 0; j < st_values_len; ++j) {
		uint32 val_len = str_length(st_values[j]);
		str_list attrs = 0;
		uint32 attrs_len = char_split(st_params[j], ';', &attrs, false);
		//printf("ZZZZ:%s\n",st_values[j]);
		//-----is a var
		if (is_valid_name(st_values[j], true) && return_var_id(st_values[j], "0") > 0) {
			String ret0 = return_value_var_complete(find_index_var_memory(return_var_id(st_values[j], "0")));
			//printf("$$$$8:%s=>%s\n", st_values[j], ret0);
			str_init(&st_values[j], ret0);
			val_len = str_length(st_values[j]);
		}
		//-----is an array
		if (val_len > 2 && st_values[j][0] == '{' && st_values[j][val_len - 1] == '}') {
			int32 max_indexes[MAX_ARRAY_DIMENSIONS];//get user indexes
			uint8 indexes_len = return_size_value_dimensions(st_values[j], max_indexes);
			/*TODO:chck error by difference
			 str_list indexes = 0;
			uint32 indexes_len = char_split(attrs[2], ',', &indexes, true);
			*/
			vaar_en s = return_value_dimensions(st_values[j], attrs[0], max_indexes, indexes_len);
			//print_vaar(s);
			vaar *tmp1 = s.start;
			if (tmp1 != 0) {
				for (;;) {
					tmp1->data_id = (long_int) j;
					tmp1->index = 0;
					for (uint8 i = 0; i < indexes_len; ++i) {
						tmp1->index = str_append(tmp1->index, str_from_int32(max_indexes[i]));
						if (i + 1 < indexes_len) tmp1->index = char_append(tmp1->index, ';');
					}
					append_vaar((*tmp1), &struct_node.st);
					tmp1 = tmp1->next;
					if (tmp1 == 0) break;
				}
			}
			//printf("!OOOOO:%s,%s\n", st_values[j], print_str_list(attrs, attrs_len));
		}
			//is a value
		else {
			String main_value = 0;
			uint8 sub_type = '0';
			calculate_value_of_var(st_values[j], attrs[0], &main_value, &sub_type);
			//-----
			//printf("OOOOO:%s,%s,%c,%s\n", st_values[j], main_value, sub_type, print_str_list(attrs, attrs_len));
			vaar s = {j, sub_type, main_value, "1", 0};
			append_vaar(s, &struct_node.st);
		}
	}
	//--------------------
	append_stde(struct_node);
	printf("@STRUCT:%s=>[%i]%s\n", value, st_values_len, print_str_list(st_values, st_values_len));
	//print_vaar(struct_node.st);
	return str_from_long_int(struct_node.id);
}

//*********************************************************
/**
 * get a value of var like a value or an array of values and determine its indexes and count of its dimensions
 * @param val
 * @param indexes
 * @return uint8
 */
uint8 return_size_value_dimensions(String val, int32 indexes[]) {
	str_list entries = 0;
	uint32 entries_len = 0;
	str_list_append(&entries, val, entries_len++);
	uint8 count = 0;
	uint32 last_max = 1;
	for (;;) {
		str_list tmp_entries = 0;
		uint32 tmp_entries_len = 0;
		for (uint32 b = 0; b < entries_len; b++) {
			uint32 ent_len = str_length(entries[b]);
			//------
			if (entries[b][0] == '{' && entries[b][ent_len - 1] == '}') {
				str_init(&entries[b], str_substring(entries[b], 1, ent_len - 1));
			}/* else {
				//TODO:error
				//printf("ERROR:%s\n",entries[b]);
				return 0;
			}*/
			//------
			int32 pars = 0, bras = 0, acos = 0;
			String buf = 0;
			Boolean is_string = false, is_struct = false;
			//------
			for (uint32 c = 0; c < ent_len; c++) {
				//========check is string
				if (entries[b][c] == '\"' && (c == 0 || entries[b][c - 1] != '\\')) {
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
				//========is struct
				if (!is_string && entries[b][c] == '(' && pars == 1 && bras == 0 && acos == 0 &&
						str_equal(str_trim_space(buf), "struct")) {
					is_struct = true;
					buf = 0;
					continue;
				} else if (!is_string && is_struct && entries[b][c] == ')' && pars == 0 && bras == 0 && acos == 0) {
					is_struct = false;
				}
				//========split segments
				if (!is_string && !is_struct && pars == 0 && bras == 0 && acos == 0 &&
						(entries[b][c] == ',' || c + 1 == ent_len)) {
					if (c + 1 == ent_len)buf = char_append(buf, entries[b][c]);
					str_list_append(&tmp_entries, str_trim_space(buf), tmp_entries_len++);
					buf = 0;
					continue;
				}
				//========append to buf
				buf = char_append(buf, entries[b][c]);
			}
		}
		//------
		if (tmp_entries_len / last_max == 1 && count > 0)break;
		str_list_init(&entries, tmp_entries, tmp_entries_len);
		entries_len = tmp_entries_len;
		//printf("QQQQQQQ:%i,%s\n", count, print_str_list(tmp_entries, tmp_entries_len));
		indexes[count++] = tmp_entries_len / last_max;
		last_max = tmp_entries_len;
		if (count == MAX_ARRAY_DIMENSIONS)break;
	}
	return count;
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
	 - {struct(5,"hjj"),struct(57878,"amin")}
	*/
	//--------------------init vars
	Boolean is_string = false, is_struct = false;
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
			is_struct = false;
			//------
			for (uint32 c = 0; c < ent_len; c++) {
				//========check is string
				if (entries[b][c] == '\"' && (c == 0 || entries[b][c - 1] != '\\')) {
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
				//========is struct
				if (!is_string && entries[b][c] == '(' && pars == 1 && bras == 0 && acos == 0 &&
						str_equal(str_trim_space(buf), "struct")) {
					is_struct = true;
				} else if (!is_string && is_struct && entries[b][c] == ')' && pars == 0 && bras == 0 && acos == 0) {
					is_struct = false;
				}
				//========split segments
				if (!is_string && !is_struct && pars == 0 && bras == 0 && acos == 0 &&
						(entries[b][c] == ',' || c + 1 == ent_len)) {
					str_list_append(&tmp_entries, buf, tmp_entries_len++);
					//printf("E$RR:%s\n", buf);
					//----last step:register in vals_array
					if (i + 1 == indexes_len) {
						String sec_type = determine_value_type(buf);
						if (!str_equal(sec_type, type_var) && !str_equal(sec_type, "null") &&
								!str_equal(sec_type, "struct")) {
							exception_handler("val_def_var", __func__, buf, type_var);
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
	//****************if a var
	if (is_valid_name(val, true)) {
		String name = 0, index = 0;
		return_name_index_var(val, true, &name, &index);
		long_int ret_id = return_var_id(name, 0);
		if (ret_id > 0) {
			return get_datas(get_Mvar(find_index_var_memory(ret_id)).type_var).name;
		}
	}
	//****************analyzing
	if (str_indexof(str_trim_space(val), "struct", 0) == 0)return "struct";
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
	uint8 sub_type = '0';
	//----------------if is a variable
	if (is_valid_name(value, true)) {
		Mpoint m = return_var_memory_value(value);
		if (m.id > 0) {
			(*ret_value) = str_reomve_quotations(m.data, type);
			(*ret_subtype) = m.type_data;
			return;
		}
	}
	//printf("&YYYY:%s,%s,%i\n", value, type,m.id);
	//----------------
	if (str_equal(type, "num")) {
		calculate_math_expression(value, '_', &(*ret_value), &(*ret_subtype));
	} else if (str_equal(type, "str")) {
		calculate_string_expression(value, &(*ret_value), &(*ret_subtype));
	} else if (str_equal(type, "bool")) {
		(*ret_value) = calculate_boolean_expression(value, &sub_type);
		(*ret_subtype) = sub_type;
	} else if (search_datas(type, entry_table.cur_fid, false).id != 0) {
		(*ret_value) = calculate_struct_expression(value, type, &sub_type);
		(*ret_subtype) = sub_type;
	} else {
		(*ret_subtype) = 'n';
		str_init(&(*ret_value), "null");
	}
	//TODO:
	/*
	sub_type = 'b'
	case search_in_classes(type_var) > 0:
		//msg("XXXX:", type_var, value_var)
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
String calculate_boolean_expression(String exp, uint8 *sub_type) {
	/**
	1- (true&&false)||true ---OK---
	2- !(f==34) ---OK---
	3- !(true||false) ---OK---
	4- !true&&true ---OK---
	5- true~~false ---OK---
	6- (buf!=""&&exp=="tr")~~!(exp=="DF") ---OK---
	7- (true~~((45-1)<=45))||false ---OK---
	*/
	//msg("&BOOL11:", exp)
	//***********************define variables
	String EXP = 0;
	str_init(&EXP, exp);
	String final_exp = 0;
	str_init(&final_exp, "false");
	int32 pars = 0, bra = 0, starting = -1, ending = -1;
	String op = 0, bool1 = 0, bool2 = 0, buf = 0, word_f = "";
	Boolean is_string = false, is_break = false, is_func = false, is_change = false;
	(*sub_type) = 'b';
	//***********************start analyzing
	if (str_equal(exp, "true") || str_equal(exp, "false")) {
		return exp;
	}
	if (str_trim_space(exp) == 0 || str_equal(exp, "null")) {
		return "false";
	}
	exp = remove_unused_parenthesis(exp);
	exp = str_multi_append("(", exp, ")", 0, 0, 0);
	//***********************Level 1 : analyzing and convert expressions to true,false
	for (;;) {
		is_change = false;
		for (uint32 i = 0; i < str_length(exp); i++) {
			uint32 exp_len = str_length(exp);
			is_break = false;
			//---------------check is string
			if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\')) {
				is_string = switch_bool(is_string);
			}
			//---------------count bra,pras
			if (!is_string) {
				switch (exp[i]) {
					case ' ':
						continue;
						break;
					case '(': {
						pars++;
						word_f = str_trim_space(word_f);
						if (is_valid_name(word_f, false)) is_func = true;
						break;
					}
					case ')':
						pars--;
						break;
					case '[':
						bra++;
						break;
					case ']':
						bra--;
						break;
				}
			}
			//---------------get &&,||,~~
			String tmp1 = 0;
			if (!is_string && i + 1 < exp_len) {
				tmp1 = char_append(tmp1, exp[i]);
				tmp1 = char_append(tmp1, exp[i + 1]);
				//fmt.Println("ZZZZ:", tmp1, search_in_slice(boolean_operators, tmp1))
				
			}
			//---------------get an expression
			if (!is_string && ((exp[i] == ')' && pars == 0) ||
					(tmp1 != 0 && str_search(boolean_operators, tmp1, StrArraySize(boolean_operators))) ||
					i + 1 == exp_len) && bra == 0) {
				if (str_length(buf) > 2 && buf[0] == '!' && buf[1] == '(') {
					starting += 1;
					buf = str_substring(buf, 1, 0);
				}
				if (is_func && exp[i] == ')' && pars == 0) {
					buf = char_append(buf, exp[i]);
					is_func = false;
				}
				if (buf != 0 && (i + 1 == exp_len || (buf[0] == '(' && exp[i] == ')'))) {
					buf = char_append(buf, exp[i]);
				}
				
				if (buf != 0 && (str_search(boolean_operators, tmp1, StrArraySize(boolean_operators)) ||
						(buf[0] != '(' && exp[i] == ')' && pars == 0))) {
					//msg("&$$$:", tmp1, buf)
					ending = i;
				} else {
					ending = i + 1;
				}
				buf = str_trim_space(buf);
				//msg("&&&&&&&:", buf, trim_sep_string(buf), starting, ending)
				//search for true_false
				if (!str_equal(str_trim_optimized_boolean(buf), "true") &&
						!str_equal(str_trim_optimized_boolean(buf), "false") && buf != 0) {
					int32 invalid_pars = 0;
					buf = remove_incorrect_pars(buf, &invalid_pars);
					starting += invalid_pars;
					if (starting < 0) {
						starting = 0;
					}
					String ret0 = return_true_false(buf);
					//fmt.Printf("POP:***%s***%s=>%s\n", exp, buf, ret0)
					//*************replace the result
					if (starting > 0 && (exp[starting] == '&' || exp[starting] == '|' || exp[starting] == '~')) {
						starting++;
					}
					exp = replace_in_expression(exp, ret0, starting, ending, true, true);
					//*************end
					i = -1;
					//msg("&FFF:", s1, s2, exp)
					pars = 0;
					is_change = true;
				}
				//search for true,false
				if (str_search(boolean_operators, tmp1, StrArraySize(boolean_operators)) && i > -1) {
					i += 1;
				}
				if (i > -1 && buf != 0) {
					//msg("&@@@:", tmp1, buf)
					if (str_equal(buf, "!true"))
						str_init(&buf, "false");
					else if (str_equal(buf, "!false"))
						str_init(&buf, "true");
					
					is_change = true;
					if (starting == 0 && ending > exp_len - 2) {
						is_change = false;
					}
					exp = replace_in_expression(exp, buf, starting, ending, true, true);
				}
				buf = 0;
				is_break = true;
				starting = -1, ending = -1;
				break;
			}
			//---------------append to buf
			if (!is_break) {
				buf = char_append(buf, exp[i]);
				
				if (starting == -1) {
					starting = i;
				}
			}
			//---------------append to word_f
			if (!is_break && !is_string && char_search_index(words_splitter, exp[i]) && exp[i] != ' ') {
				word_f = 0;
			} else {
				word_f = char_append(word_f, exp[i]);
			}
			//fg++
			/*if (fg == 10) {
				break;
			}*/
			
		}
		if (!is_change) {
			break;
		}
	}
	//msg("&BOOLEAN:", exp)
	//***********************Level 2 : analyzing and calculate true/false expression
	if (str_equal(exp, "true") || str_equal(exp, "false")) {
		return exp;
	}
	//printf("#EEEE:%s,%s\n", EXP, exp);
	//---------------init vars
	buf = 0;
	starting = -1, ending = -1;
	is_string = false;
	exp = char_append(exp, '=');
	pars = 0, bra = 0;
	//---------------start analyzing
	for (uint32 i = 0; i < str_length(exp); i++) {
		uint32 buf_len = str_length(buf);
		//---------------check is string
		if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\')) {
			is_string = switch_bool(is_string);
		}
		if (!is_string) {
			if (exp[i] == ' ')continue;
			else if (exp[i] == '(' && bra == 0) {
				pars++;
				op = 0, bool1 = 0, bool2 = 0, buf = 0;
				starting = -1;
				//msg("PPPPPPPPPP")
				continue;
			} else if (exp[i] == ')')pars--;
			else if (exp[i] == '[')bra++;
			else if (exp[i] == ']')bra--;
		}
		
		//msg("DDD:", buf, i, string(exp[i]))
		//---------------allocate bool1
		if (!is_string && buf != 0 &&
				(str_has_suffix(buf, "&&") || str_has_suffix(buf, "||") || str_has_suffix(buf, "~~") ||
						(exp[i] == ')' && pars == 0)) && bool1 == 0 && op == 0 && bra == 0) {
			String tmp = 0;
			//printf("#EW:%s,%s\n",buf,tmp);
			if (str_has_suffix(buf, "&&") || str_has_suffix(buf, "||") || str_has_suffix(buf, "~~")) {
				op = str_substring(buf, buf_len - 2, 0);
				tmp = str_substring(buf, 0, buf_len - 2);
				ending = i - 3;
			} else {
				ending = i;
				str_init(&tmp, buf);
			}
			
			if (str_equal(tmp, "!true"))
				str_init(&tmp, "false");
			else if (str_equal(tmp, "!false"))
				str_init(&tmp, "true");
			
			if (str_equal(tmp, "true") || str_equal(tmp, "false")) {
				str_init(&bool1, tmp);
			}
			//*************
			//fmt.Printf("QQQQ:%s[%s];%s\n", bool1, op, buf, tmp)
			//msg(ending)
			buf = 0;
			ending = -1;
			//continue
		}
		//---------------end of calculation
		if (!is_string && buf != 0 && (str_equal(buf, "true") || str_equal(buf, "false") || is_valid_name(buf, true)) &&
				bool1 == 0 && bool2 == 0 && op == 0 && (exp[i] == '=' && i + 1 == str_length(exp))) {
			if (is_valid_name(buf, true))
				buf = return_var_memory_value(buf).data;
			str_init(&final_exp, buf);
			break;
		}
		//---------------allocate bool2
		if (!is_string && buf != 0 &&
				(str_has_suffix(buf, "&&") || str_has_suffix(buf, "||") || str_has_suffix(buf, "~~") ||
						(exp[i] == '=' && i + 1 == str_length(exp)) || (exp[i] == ')' && pars == 0)) && bool2 == 0 &&
				((exp[i] == '=' && bool1 == 0 && op == 0) || (bool1 != 0 && op != 0)) && bra == 0) {
			String tmp = 0;
			if (str_has_suffix(buf, "&&") || str_has_suffix(buf, "||") || str_has_suffix(buf, "~~")) {
				//op = buf[len(buf)-2:]
				tmp = str_substring(buf, 0, buf_len - 2);
				ending = i - 3;
			} else {
				str_init(&tmp, buf);
				ending = i;
			}
			if (str_equal(tmp, "!true"))
				str_init(&tmp, "false");
			else if (str_equal(tmp, "!false"))
				str_init(&tmp, "true");
			
			if (str_equal(tmp, "true") || str_equal(tmp, "false")) {
				str_init(&bool2, tmp);
			}
			//fmt.Printf("EEEE:%s[%s]%s{%s}\n\t%s;%s\n", bool1, op, bool2, tmp, buf, exp)
			//**************analyze bool1&bool2
			
			//*************calculate bool1&bool2
			String result = 0;
			str_init(&result, "false");
			if (str_equal(op, "&&") && str_equal(bool1, "true") && str_equal(bool2, "true")) str_init(&result, "true");
			else if (str_equal(op, "||") && (str_equal(bool1, "true") || str_equal(bool2, "true")))
				str_init(&result, "true");
			else if (str_equal(op, "~~") && ((str_equal(bool1, "true") && str_equal(bool2, "false")) ||
					(str_equal(bool1, "false") && str_equal(bool2, "true"))))
				str_init(&result, "true");
			else if (op == 0 && bool1 == 0 && (str_equal(bool2, "true") || str_equal(bool2, "false")))
				str_init(&result, bool2);
			//*************replace the result
			String s1 = 0, s2 = 0, expression = 0;
			s1 = str_trim_space(str_substring(exp, 0, starting));
			s2 = str_trim_space(str_substring(exp, ending, 0));
			uint32 s1_len = str_length(s1);
			if (s1_len > 0 && s1[s1_len - 1] == '(' && s2[0] == ')') {
				s1 = str_substring(s1, 0, s1_len - 1);
				s2 = str_substring(s2, 1, 0);
			}
			expression = str_multi_append(s1, result, s2, 0, 0, 0);
			str_init(&exp, expression);
			//*************end calculate
			//msg("DDDD:", starting, ending)
			//fmt.Printf("CALC2~%s:%s:%s=%s\n\t%s\n", bool1, op, bool2, result, exp)
			op = 0, bool1 = 0, bool2 = 0, buf = 0;
			starting = -1, pars = 0;
			i = -1;
			continue;
		}
		
		//---------------append to buf
		buf = char_append(buf, exp[i]);
		if (starting == -1) {
			starting = i;
		}
		/*if (!is_string && exp[i] == '!' && (i + 1 >= str_length(exp) || exp[i + 1] != '='))
		{
		}*/
		
	}
	//printf("#EEEE12:%s,%s,%s\n", EXP, exp, final_exp);
	//***********************return final_exp
	//fmt.Printf("RESULT:%s=>%s\n", exp, final_exp)
	return final_exp;
}

String return_true_false(String exp) {
	/**
	1- c_name=="AliReza" 		---OK---
	2- init() 					---OK---
	3- init()>=4+6 				---OK---
	4- !IsDir(path) 			---OK---
	5- __define["c"]==45
	----------types of values:
	1- variables(normal vars,global vars)
	2- functions call
	3- magic macros
	4- values (str,num,bool values)
	*/
	
	//***********************define variables
	String final_exp = 0, buf = 0, op = 0, operand1 = 0, operand2 = 0;
	str_init(&final_exp, "false");
	uint8 type1 = '0', type2 = '0';
	Boolean is_string = false, is_not = false;
	int32 pars = 0, bra = 0, stop_buf = 0;
	//***********************start analyzing
	exp = remove_unused_parenthesis(exp);
	//msg("&TRUE_FALSE:", exp)
	if (str_length(exp) > 1 && exp[0] == '!') {
		exp = str_substring(exp, 1, 0);
		is_not = true;
	}
	
	for (uint32 i = 0; i < str_length(exp); i++) {
		uint32 exp_len = str_length(exp);
		//---------------check is string
		if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\')) {
			is_string = switch_bool(is_string);
		}
		if (!is_string) {
			if (exp[i] == ' ')continue;
			else if (exp[i] == '(' && bra == 0)pars++;
			else if (exp[i] == ')' && bra == 0)pars--;
			else if (exp[i] == '[' && bra == 0)bra++;
			else if (exp[i] == ']' && bra == 0)bra--;
		}
		//---------------find comparative operator
		String tmp1 = 0;
		if (!is_string && i + 1 < exp_len && (exp[i] == '=' || exp[i] == '>' || exp[i] == '<' || exp[i] == '!')) {
			tmp1 = char_append(tmp1, exp[i]);
			tmp1 = char_append(tmp1, exp[i + 1]);
		}
		if (tmp1 != 0) {
			if (str_search(comparative_operators, tmp1, StrArraySize(comparative_operators))) {
				str_init(&op, tmp1);
				stop_buf = 2;
			} else if (tmp1[0] == '<' || tmp1[0] == '>') {
				op = char_to_str(tmp1[0]);
				stop_buf = 1;
			}
		}
		//---------------calculate operand1,operand2
		if (!is_string && ((op != 0 && operand1 == 0 && buf != 0) || i + 1 == exp_len)) {
			if (i + 1 == exp_len) {
				buf = char_append(buf, exp[i]);
			}
			//msg("WWW:", buf)
			if (operand1 == 0)str_init(&operand1, buf);
			else if (operand2 == 0)str_init(&operand2, buf);
			buf = 0;
		}
		//---------------append to buf
		if (stop_buf == 0) {
			buf = char_append(buf, exp[i]);
		} else {
			stop_buf--;
		}
		
	}
	//msg("&Operands:", operand1, op, operand2)
	//***********************start calculating
	//------------------------------------------analyzing operand1,operand2
	for (uint8 i = 0; i < 2; i++) {
		String oprd = 0, final_res = 0;
		uint8 tyy = '0';
		//---------------
		if (i == 0) {
			str_init(&oprd, operand1);
		} else {
			str_init(&oprd, operand2);
		}
		//---------------
		while (str_trim_space(oprd) != 0) {
			//---------------start analyzing operand
			//............function call
			if (labeled_instruction(oprd) == FUNC_CALL_LBL_INST) {
				str_init(&oprd, function_call(oprd, 2));
				//msg("&FFF1:", oprd, value)
				if (is_valid_name(oprd, true)) {
					Mpoint po = return_var_memory_value(oprd);
					str_init(&final_res, po.data);
					tyy = po.type_data;
					oprd = 0;
				}
				//msg("&FFGG1:", operand1)
				
			}
				//............variable
			else if (is_valid_name(oprd, true)) {
				Mpoint po = return_var_memory_value(oprd);
				//msg("&VVV1:", operand1, value,string(ty))
				if (po.type_data == '0') {
					exception_handler("not_exist_var", __func__, oprd, 0);
					return "false";
				}
				str_init(&final_res, po.data);
				tyy = po.type_data;
				oprd = 0;
				//............value
			} else {
				uint8 typ = '0';
				String value = determine_unknown_value(oprd, &typ);
				final_res = str_reomve_quotations(value, char_to_str(typ));
				//msg("&QQQ1:", operand1, value)
				tyy = typ;
				oprd = 0;
			}
		}
		//---------------
		if (i == 0) {
			str_init(&operand1, final_res);
			type1 = tyy;
		} else {
			str_init(&operand2, final_res);
			type2 = tyy;
		}
	}
	//msg("&&", operand1, operand2)
	//---------------calculate operand1,operand2 with op
	if (op == 0 && operand2 == 0 && (str_equal(operand1, "true") || str_equal(operand1, "false"))) {
		str_init(&operand1, final_exp);
	} else {
		String nums_state = 0;
		if (str_equal(op, ">=") || str_equal(op, "<=") || str_equal(op, ">") || str_equal(op, "<")) {
			//-----------remove '+'
			if (str_length(operand1) > 1 && operand1[0] == '+') {
				operand1 = str_substring(operand1, 1, 0);
			}
			if (str_length(operand2) > 1 && operand2[0] == '+') {
				operand2 = str_substring(operand2, 1, 0);
			}
			//-----------remove 000
			operand1 = str_trim_number(operand1);
			operand2 = str_trim_number(operand2);
			//msg("&Types", operand1, operand2, string(type1), string(type2))
			//------is integer
			if (type1 == 'i' && type2 == 'i') {
				int32 calc1 = str_to_int32(operand1);
				int32 calc2 = str_to_int32(operand2);
				if (calc1 < calc2)str_init(&nums_state, "<");
				else if (calc1 > calc2)str_init(&nums_state, ">");
				
				if (calc1 == calc2) {
					str_init(&nums_state, "=");
				}
			}
				//------is float
			else if ((type1 == 'f' && type2 == 'i') || (type1 == 'i' && type2 == 'f')) {
				double calc1 = str_to_double(operand1);
				double calc2 = str_to_double(operand2);
				if (calc1 < calc2)str_init(&nums_state, "<");
				else if (calc1 > calc2)str_init(&nums_state, ">");
				
				if (calc1 == calc2) {
					str_init(&nums_state, "=");
				}
			}
				//------is huge
			else if (type1 == 'h' || type2 == 'h') {
				//TODO:huge numbers
				//-----------if is negative
				/*if
					len(operand1) > 1 && operand1[0] == '-' && len(operand2) > 0 && operand2[0] != '-'
				{
					nums_state = "<"
				} else if
					len(operand1) > 0 && operand1[0] != '-' && len(operand2) > 1 && operand2[0] == '-'
				{
					nums_state = ">"
				}
				//-----------
				if nums_state == ""
				{
					operand1 = trim_last_float(operand1)
					operand2 = trim_last_float(operand2)
					int_1, float_1 :=
					detachment_float_string(operand1)
					int_2, float_2 :=
					detachment_float_string(operand2)
					var
					is_neg = false
					if
						len(int_1) > 1 && int_1[0] == '-' && len(int_2) > 1 && int_2[0] == '-'
					{
						is_neg = true
						int_1 = int_1[1:]
						int_2 = int_2[1:]
					}
					if float_1 == "" && float_2 == ""
					{
						nums_state = comparison_huge_numbers(int_1, int_2, 'i')
					} else {
						nums_state = comparison_huge_numbers(int_1, int_2, 'i')
						ret2 :=
						comparison_huge_numbers(float_1, float_2, 'f')
						//msg("&iii", int_1, int_2, nums_state, float_1, float_2, ret2)
						if nums_state == "="
						{
							nums_state = ret2
						}
					}
					if is_neg && nums_state == ">"
					{
						nums_state = "<"
					} else if is_neg && nums_state == "<"
					{
						nums_state = ">"
					}
					if nums_state == "="
					{
						nums_state = ",="
					}
					//msg("&GGG", nums_state, op, operand1, operand2, int_1, float_1, int_2, float_2)
				}
				//-----------if is equal
				if operand1 == operand2 && nums_state != ",="
				{
					nums_state += ",="
				}
				*/
			}
			//msg("&MCALC:", nums_state, op, operand1, operand2)
		}
		//printf("%%%%FFFF:%s,%s[%s]=>{%s}\n", operand1, operand2, op, nums_state);
		//------boolean,string,number
		if (str_equal(op, "==") && str_equal(operand1, operand2))
			str_init(&final_exp, "true");
		else if (str_equal(op, "!=") && !str_equal(operand1, operand2))
			str_init(&final_exp, "true");
			//------number
		else if (str_equal(op, ">=") && (str_equal(nums_state, "=") || str_equal(nums_state, ">")))
			str_init(&final_exp, "true");
		else if (str_equal(op, "<=") && (str_equal(nums_state, "=") || str_equal(nums_state, "<")))
			str_init(&final_exp, "true");
		else if (str_equal(op, ">") && str_equal(nums_state, ">"))
			str_init(&final_exp, "true");
		else if (str_equal(op, "<") && str_equal(nums_state, "<"))
			str_init(&final_exp, "true");
		
	}
	//***********************return final_exp
	//msg("&TRUE_FALSE_FINAL", operand1, operand2, op, final_exp)
	if (is_not && str_equal(final_exp, "true")) {
		str_init(&final_exp, "false");
	} else if (is_not && str_equal(final_exp, "false")) {
		str_init(&final_exp, "true");
	}
	return final_exp;
}

//*********************************************************
String determine_unknown_value(String value, uint8 *sub_type) {
	String type_val = determine_value_type(value);
	String ret_val = 0;
	uint8 ret_type = 0;
	calculate_value_of_var(value, type_val, &ret_val, &ret_type);
	(*sub_type) = ret_type;
	return ret_val;
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
				exception_handler("invalid_exp_val", __func__, buf, "str");
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
				exception_handler("invalid_exp_val", __func__, buf, "str");
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
	for (uint32 i = 0; i < str_length(exp); i++) {
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
			//printf("&VVV:%s,%s,%c,%c=>%s\n", num1, num2, op, type_exp, result);
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
			//printf("CALC:%s%c%s=%s\n\t%s\n", num1, op, num2, result, exp);
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
		exception_handler("zero_division", __func__, str_multi_append(num1, "/", num2, 0, 0, 0), "");
		return "0";
	} else if (op == '%' && (type_exp == 'f' || type_exp == 'h')) {
		//com_type, _, _ := fitting_value(string(type_exp), "", "com_type")
		//exception_handler("wrong_remainder", __func__, com_type, "")
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
			exception_handler("zero_division", __func__, str_multi_append(num1, "/", num2, 0, 0, 0), "");
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
					exception_handler("out_of_range_float", __func__, str_multi_append(num1, "^", num2, 0, 0, 0), "");
					return "0";
				}
				//printf("DDDDDDD:%f\n",ret);
				result = str_from_double(ret, (uint8) (str_length(num1) * 2));
				break;
			}
		}
	}
		//----------------huge
	else if (type_exp == 'h') {
		//printf("&HHH:%s,%s,%c\n", num1, num2, op);
		switch (op) {
			case '+':
				result = sum_huge_numbers(num1, num2, false);
				break;
			case '-':
				result = sum_huge_numbers(num1, num2, true);
				break;
			case '*':
				result = muliply_huge_numbers(num1, num2);
				break;
			case '/':
				result = divide_huge_numbers(num1, num2);
				break;
			case '^': {
				str_detachment_float(num2, &num2, 0);
				int32 n2 = str_to_int32(num2);
				//printf("$$RR:%s=>%i\n",num2,n2);
				Boolean is_neg = false;
				int32 b = 0;
				String res = 0;
				str_init(&res, "1");
				if (n2 < 0) {
					is_neg = true;
					n2 = -n2;
				}
				for (uint32 b = 0; b < n2; b++) {
					res = muliply_huge_numbers(res, num1);
				}
				if (is_neg) {
					res = divide_huge_numbers("1", res);
				}
				str_init(&result, res);
				break;
			}
		}
	}
	
	return result;
}

//**************************************************************
/**
 * get two huge numbers and balance its integer and float segments and return it by a number that show position of start float like: 3.46,67=>0346,6700
 * @param num1
 * @param num2
 * @param type_nums
 * @param ret_num1
 * @param ret_num2
 * @return int32
 */
int32 balance_huge_numbers(String num1, String num2, uint8 type_nums, String *ret_num1, String *ret_num2) {
	//****************init vars
	uint32 num1_1_len = 0, num1_2_len = 0, num2_1_len = 0, num2_2_len = 0;
	String num1_1 = 0, num1_2 = 0, num2_1 = 0, num2_2 = 0;
	uint8 op1 = '+', op2 = '+';
	int32 point = -1;
	uint32 num1_len = str_length(num1);
	uint32 num2_len = str_length(num2);
	//****************determine num1,num2 len & determine op1,op2
	/*rnum1 := num1
	rnum2 := num2*/
	Boolean is_point = false;
	uint32 ii = 0;
	if (num1_len > 0 && num1[0] == '-') {
		ii++;
		op1 = '-';
	}
	for (; ii < num1_len; ii++) {
		if (num1[ii] == '.') {
			is_point = true;
			continue;
		}
		if (!is_point) {
			num1_1_len++;
			num1_1 = char_append(num1_1, num1[ii]);
		} else {
			num1_2_len++;
			num1_2 = char_append(num1_2, num1[ii]);
		}
	}
	is_point = false;
	ii = 0;
	if (num2_len > 0 && num2[0] == '-') {
		ii++;
		op2 = '-';
	}
	for (; ii < num2_len; ii++) {
		if (num2[ii] == '.') {
			is_point = true;
			continue;
		}
		if (!is_point) {
			num2_1_len++;
			num2_1 = char_append(num2_1, num2[ii]);
		} else {
			num2_2_len++;
			num2_2 = char_append(num2_2, num2[ii]);
		}
	}
	//printf("#EEEEEE:%s,%s=>[%s,%s];[%s,%s]\n",num1,num2,num1_1,num1_2,num2_1,num2_2);
	//****************balance num1_1,num2_1
	//msg("&UUU:", num1_1)
	String tmp1 = 0, tmp2 = 0;
	str_init(&tmp1, num1_1);
	str_init(&tmp2, num2_1);
	str_empty(&num1_1);
	str_empty(&num2_1);
	if (num1_1_len > num2_1_len) {
		for (uint32 i = 0; i < num1_1_len - num2_1_len; i++) {
			num2_1 = char_append(num2_1, '0');
		}
	} else {
		for (uint32 i = 0; i < num2_1_len - num1_1_len; i++) {
			num1_1 = char_append(num1_1, '0');
		}
	}
	num1_1 = str_append(num1_1, tmp1);
	num2_1 = str_append(num2_1, tmp2);
	//****************balance num1_2,num2_2
	if (num1_2_len > num2_2_len) {
		for (uint32 i = 0; i < num1_2_len - num2_2_len; i++) {
			num2_2 = char_append(num2_2, '0');
		}
	} else {
		for (uint32 i = 0; i < num2_2_len - num1_2_len; i++) {
			num1_2 = char_append(num1_2, '0');
		}
	}
	//printf("#SSSSSS:%s,%s=>[%s,%s];[%s,%s]\n", num1, num2, num1_1, num1_2, num2_1, num2_2);
	//****************join num1_1,num1_2 & num2_1,num2_2
	num1 = str_multi_append(char_to_str(op1), num1_1, num1_2, 0, 0, 0);
	num2 = str_multi_append(char_to_str(op2), num2_1, num2_2, 0, 0, 0);
	//****************determine main point
	if (type_nums == 's'/*if type is sum*/) {
		point = str_length(num1_1);
	} else if (type_nums == 'm')/*if type is multiple */ {
		point = str_length(num1_2) + str_length(num2_2);
	} else if (type_nums == 'd')/*if type is division */ {
		point = str_length(num1_1) + str_length(num1_2);
	}
	//****************return
	if (str_length(num1_2) == 0) {
		point = -1;
	}
	//printf("#QQQQQQ:%s,%s,[%c,%c,%i]\n", num1, num2,op1,op2,point);
	(*ret_num1) = num1;
	(*ret_num2) = num2;
	return point;
}

//**************************************************************
/**
 * get two huge numbers and determine that which one is max ,min and return it by a final operation
 * @param num1
 * @param num2
 * @param type_nums
 * @param mmax
 * @param mmin
 * @param final_type
 * @return int32
 */
int32 determine_max_min_huge_numbers(String num1, String num2, uint8 type_nums, String *mmax, String *mmin,
		uint8 *final_type) {
	//****************init vars
	uint8 final_op = '+', op1 = '+', op2 = '+', max = '0', min = '0';
	String num3 = 0, num4 = 0;
	String num1_1 = 0, num1_2 = 0, num2_1 = 0, num2_2 = 0;
	Boolean is_point = false;
	//****************balance num1,num2
	
	int32 point = balance_huge_numbers(num1, num2, type_nums, &num3, &num4);
	//printf("#TTTTTT:%s,%s=>%s,%s,%i\n", num1, num2, num3, num4, point);
	op1 = num3[0];
	op2 = num4[0];
	num3 = str_substring(num3, 1, 0);
	num4 = str_substring(num4, 1, 0);
	uint32 num3_len = str_length(num3);
	uint32 num4_len = str_length(num4);
	for (uint32 i = 0; i < num3_len; i++) {
		if (!is_point) {
			num1_1 = char_append(num1_1, num3[i]);
			num2_1 = char_append(num2_1, num4[i]);
		} else {
			num1_2 = char_append(num1_2, num3[i]);
			num2_2 = char_append(num2_2, num4[i]);
		}
		if (i + 1 == point)
			is_point = true;
	}
	//printf("#YYYYY:%s,%s;%s,%s\n", num1_1, num1_2, num2_1, num2_2);
	//msg("&$$$$:", num1_1, num1_2, num2_1, num2_2)
	//****************determine max number
	//----------------comparison of num1_1,num2_1
	for (uint32 i = 0; i < str_length(num1_1); i++) {
		int32 n1 = str_to_int32(char_to_str(num1_1[i]));
		int32 n2 = str_to_int32(char_to_str(num2_1[i]));
		if (n1 > n2) {
			max = '1';
			break;
		} else if (n2 > n1) {
			max = '2';
			break;
		}
	}
	//----------------comparison of num1_2,num2_2
	if (max == '0')
		for (uint32 i = 0; i < str_length(num1_2); i++) {
			int32 n1 = str_to_int32(char_to_str(num1_2[i]));
			int32 n2 = str_to_int32(char_to_str(num2_2[i]));
			if (n1 > n2) {
				max = '1';
				break;
			} else if (n2 > n1) {
				max = '2';
				break;
			}
		}
	//----------------else if num1==num2
	if (max == '0')max = '1';
	//****************determine final_op
	if ((op1 == '-' && op2 == '-') || (max == '1' && op1 == '-') || (max == '2' && op2 == '-')) {
		final_op = '-';
	}
	//****************fix max,min
	if (max == '1') {
		(*mmax) = str_multi_append(num1_1, num1_2, 0, 0, 0, 0);
		(*mmin) = str_multi_append(num2_1, num2_2, 0, 0, 0, 0);
	} else {
		(*mmin) = str_multi_append(num1_1, num1_2, 0, 0, 0, 0);
		(*mmax) = str_multi_append(num2_1, num2_2, 0, 0, 0, 0);
	}
	(*final_type) = final_op;
	//****************return
	//msg("&UUUU:", num3, num4, "\n", max, min, string(final_op), point, string(op1), string(op2))
	return point;
}

//*********************************************************
//[OK]
String sum_huge_numbers(String num1, String num2, Boolean is_neg) {
	//****************init vars
	String first = 0, second = 0, result = 0;
	Boolean is_sum = true, is_carry = false;
	uint8 op = '+';
	int32 point_s = 0;
	//****************start analyzing
	//----------------check is_neg
	if (is_neg) num2 = str_make_negative_number(num2);
	//----------------determine first and second based max,min
	point_s = determine_max_min_huge_numbers(num1, num2, 's', &first, &second, &op);
	if ((str_length(num1) > 0 && num1[0] == '-') || (str_length(num2) > 0 && num2[0] == '-')) {
		is_sum = false;
	}
	//----------------reverse first and second
	//printf("#JJJJJJ:%s,%s=>%s,%s\n",num1,num2,first,second);
	first = str_reverse(first);
	second = str_reverse(second);
	uint32 first_len = str_length(first);
	//----------------if is subtraction
	if (!is_sum)
		for (uint32 i = 0; i < first_len; i++) {
			int32 n1 = str_to_int32(char_to_str(first[i]));
			int32 n2 = str_to_int32(char_to_str(second[i]));
			int32 tmp1 = n1 - n2;
			if (is_carry) {
				tmp1 -= 1;
				is_carry = false;
			}
			if (tmp1 < 0) {
				tmp1 += 10;
				is_carry = true;
			}
			result = str_append(result, str_from_int32(tmp1));
		}
		//----------------if is addition
	else
		for (uint32 i = 0; i < first_len; i++) {
			int32 n1 = str_to_int32(char_to_str(first[i]));
			int32 n2 = str_to_int32(char_to_str(second[i]));
			int32 tmp1 = n1 + n2;
			if (is_carry) {
				tmp1 += 1;
				is_carry = false;
			}
			if (tmp1 > 9) {
				tmp1 -= 10;
				is_carry = true;
			}
			result = str_append(result, str_from_int32(tmp1));
		}
	
	//----------------reverse result and finish
	result = str_reverse(result);
	//printf("#QQQQQQ:%s,%s,%i,%i=>%s\n", num1, num2, is_sum, point_s, result);
	if (point_s >= 0) {
		String tmp1 = 0;
		str_init(&tmp1, result);
		str_empty(&result);
		for (uint32 i = 0; i < str_length(tmp1); i++) {
			result = char_append(result, tmp1[i]);
			if (i + 1 == point_s) {
				result = char_append(result, '.');
			}
		}
	}
	//printf("#EEEE:%s\n",result);
	result = str_trim_number(result);
	if (op == '-') {
		result = str_make_negative_number(result);
	} else if (is_sum && is_carry) {
		result = str_multi_append("1", result, 0, 0, 0, 0);
	}
	//****************return
	//if num1 == "2" {
	//msg("&SUM", point_s, first, second, result)
	//}
	return result;
}

//*********************************************************
String muliply_huge_numbers(String num1, String num2) {
	//****************init vars
	uint8 op1 = '0', op2 = '0', final_op = '0';
	String result = 0, zeros = 0, res1 = 0, res2 = 0, num3 = 0, num4 = 0;
	int32 carry = 0, res_ch = 1;
	//****************balance num1,num2
	int32 point_m = balance_huge_numbers(num1, num2, 'm', &num3, &num4);
	op1 = num3[0];
	op2 = num4[0];
	num3 = str_substring(num3, 1, 0);
	num4 = str_substring(num4, 1, 0);
	//printf("#UUUUUU:%s,%s=>%s,%s,%i\n",num1,num2,num3,num4,point_m);
	uint32 num3_len = str_length(num3);
	uint32 num4_len = str_length(num4);
	num3 = str_reverse(num3);
	num4 = str_reverse(num4);
	//****************
	for (uint32 i = 0; i < num3_len; i++) {
		carry = 0;
		if (num3[i] != '0') {
			int32 n1 = str_to_int32(char_to_str(num3[i]));
			for (uint32 j = 0; j < num4_len; j++) {
				int32 n2 = str_to_int32(char_to_str(num4[j]));
				int32 tmp1 = n1 * n2;
				int32 tmp2 = 0;
				if (carry > 0) {
					tmp1 += carry;
					carry = 0;
				}
				int32 tmp3 = tmp1;
				if (tmp1 > 9) {
					tmp2 = (int32) tmp1 / 10;
					tmp3 = tmp1 % 10;
				}
				carry = tmp2;
				if (res_ch == 1) {
					res1 = str_append(res1, str_from_int32(tmp3));
				} else {
					res2 = str_append(res2, str_from_int32(tmp3));
				}
				//msg("&HHH:", res1, res2, string(num4[j]), string(num3[i]))
			}
			if (carry > 0) {
				if (res_ch == 1) {
					res1 = str_append(res1, str_from_int32(carry));
				} else {
					res2 = str_append(res2, str_from_int32(carry));
				}
			}
			
			if (res_ch == 1) {
				res1 = str_reverse(res1);
				res1 = str_append(res1, zeros);
			} else {
				res2 = str_reverse(res2);
				res2 = str_append(res2, zeros);
			}
			
			//msg("&YYYY:", string(num3[i]), res1, res2, zeros)
			res_ch++;
			if (res_ch == 3) {
				res1 = sum_huge_numbers(res1, res2, false);
				res2 = 0;
				res_ch = 2;
			}
		}
		zeros = char_append(zeros, '0');
	}
	str_init(&result, res1);
	//printf("#YYYYYY:%s,%s=>%s,%i\n", num1, num2, result, point_m);
	//****************determine final_op
	if (op1 == '-' || op2 == '-') {
		final_op = '-';
	}
	//****************fix result and finish
	if (point_m >= 0) {
		String tmp1 = 0;
		str_init(&tmp1, result);
		str_empty(&result);
		Boolean is_point = false;
		int32 ind = 0;
		for (int32 i = str_length(tmp1) - 1; i >= 0; i--) {
			ind++;
			result = char_append(result, tmp1[i]);
			if (ind == point_m) {
				result = char_append(result, '.');
				is_point = true;
			}
		}
		if (!is_point) {
			//like 0.0416*1.0016 = 0.04166656
			for (uint32 i = str_length(tmp1); i < point_m; i++) {
				result = char_append(result, '0');
				if (i + 1 == point_m) {
					result = str_append(result, ".0");
					is_point = true;
					break;
				}
			}
		}
		//printf("#IIIIII:%s,%s=>%s\n",tmp1,result,str_reverse(result));
		result = str_reverse(result);
	}
	result = str_trim_number(result);
	if (point_m == str_length(result) - 1) {
		result = str_multi_append("0", result, 0, 0, 0, 0);
	}
	result = str_trim_last_float(result);
	if (final_op == '-') {
		result = str_multi_append("-", result, 0, 0, 0, 0);
	}
	
	//****************return
	return result;
}

//*********************************************************
String divide_huge_numbers(String num1, String num2) {
	//Using Newton–Raphson division algorithm
	//****************init vars
	String result = 0, X0 = 0, Xi = 0, num3 = 0, num4 = 0;
	Boolean is_point = false;
	//****************trim num1,num2
	num1 = str_trim_number(num1);
	num2 = str_trim_number(num2);
	uint32 num1_len = str_length(num1);
	uint32 num2_len = str_length(num2);
	for (uint32 i = 0; i < num2_len; i++) {
		if (num2[i] == '.') {
			is_point = true;
			num2_len--;
			break;
		}
	}
	//======Level 1:Calculate an estimate X_0  for the reciprocal 1/num4.
	//----------------estimate a good number from num2
	uint32 est_nums = num2_len / 2;
	if (est_nums > max_estimate_divide_huge) {
		est_nums = max_estimate_divide_huge;
	}
	String est_num2 = str_substring(num2, 0, est_nums);
	est_num2 = make_valid_double(est_num2);
	if (str_length(est_num2) < 2 && str_length(est_num2) < num2_len) {
		est_nums = num2_len;
		est_num2 = make_valid_double(num2);
	}
	double X0_double = 1 / str_to_double(est_num2);
	//----------------determine X0=1/est(num2)
	X0 = str_from_double(X0_double, max_estimate_divide_huge);
	uint32 x0_len = str_length(X0);
	//----------------check if X0 is overflow from max_float_estimate_huge_X0
	if (x0_len > max_float_estimate_huge_X0 + 2) {
		String tmp1 = str_substring(X0, 0, max_float_estimate_huge_X0 + 2);
		X0 = make_valid_double(tmp1);
	}
	String tmp4 = str_substring(X0, 2, 0);
	str_init(&X0, "0.");
	int32 pre_len = num2_len - est_nums;
	if (pre_len > 0)
		for (uint32 i = 0; i < pre_len; i++) {
			X0 = char_append(X0, '0');
		}
	X0 = str_append(X0, tmp4);
	//printf("#VVVVVV:%s,%s=>%s,%f,%s\n", num1, num2, X0, X0_double, est_num2);
	//======Level 2 :Compute successively more accurate estimates X_1,X_1,..,X_s of the reciprocal. This is where one employs the Newton–Raphson method as such.
	str_init(&Xi, X0);
	for (uint32 i = 0; i < max_steps_estimate_huge; i++) {
		//X_i+1 = X_i*(2-(num2*X_i))
		String t_Xi = 0;
		str_init(&t_Xi, Xi);
		String tmp11 = muliply_huge_numbers(t_Xi, num2);
		//printf("$RT1:%s*%s=%s\n", t_Xi, num2, tmp11);
		String tmp12 = sum_huge_numbers("2", tmp11, true);
		//printf("$RT2:2-%s=%s\n", tmp11, tmp12);
		Xi = muliply_huge_numbers(t_Xi, tmp12);
		//printf("$RT3:%s*%s=%s\n", t_Xi, tmp12, Xi);
		Xi = limit_decimal_huge_numbers(Xi);
		//printf("$RT4:%s\n",Xi);
	}
	//======Level 3 :Compute the quotient by multiplying the dividend by the reciprocal of the divisor: Q=N.X_i
	result = muliply_huge_numbers(num1, Xi);
	//----------------set to result and finish
	result = str_trim_last_float(result);
	//printf("#BBBBB:%s,%s=>%s,%i\n",num1,num2,result);
	//****************return
	//msg("&***:", result, X0, Xi, tmp3)
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



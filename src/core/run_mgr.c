//
// Created by amindelavar on 9/23/2018.
//

#include <MPL/system.h>

//********************************************
uint8 labeled_instruction(String code) {
	//----------------------init variables
	uint8 state = UNKNOWN_LBL_INST;
	Boolean is_string = false, is_equal = false, is_ret = false;
	uint16 par = 0/*count of parenthesis */, bra = 0/*count of brackets*/, aco = 0/*count of acolads*/, store_counter = 0;
	String word = 0/*just create by words_splitter*/, case_word = 0/*create by words_splitter,single_operators*/, com_word = 0/*create by words_splitter,single_operators and skip brackets*/, buffer = 0;
	int16 last_pars = -1;
	String word_store[10];
	uint8 last_sep = 0;
	uint32 len = str_length(code);
	//msg("&LABELED:", code)
	//----------------------analyzing code line
	for (uint32 i = 0; i < len; i++) {
		is_ret = false;
		//------------------check is string
		if (code[i] == '\"' && (i == 0 || code[i - 1] != '\\')) {
			is_string = switch_bool(is_string);
		}
		//------------------continue if ' '
		if (!is_string && i + 1 < len && code[i] == ' ' && (code[i + 1] == '(')) {
			continue;
		}
		//------------------count parenthesis
		if (!is_string && code[i] == '(')
			par++;
		else if (!is_string && code[i] == ')')
			par--;
			//------------------count brackets
		else if (!is_string && code[i] == '[')
			bra++;
		else if (!is_string && code[i] == ']')
			bra--;
			//------------------count acolads
		else if (!is_string && code[i] == '{')
			aco++;
		else if (!is_string && code[i] == '}')
			aco--;
		//**********************************
		//------------------is return keyword
		if (!is_string && (code[i] == ' ' || code[i] == '(' || i + 1 == len) && word != 0 &&
				str_equal(word, "return")) {
			state = RETURN_LBL_INST;
			is_ret = true;
			//break
		}
		//------------------is next or break keyword
		if (!is_string && ((code[i] == ' ' || code[i] == '(' || i + 1 == len) && word != 0 &&
				(str_equal(word, "next") || str_equal(word, "break") || i + 1 == len))) {
			Boolean is_not = false;
			if (i + 1 == len) {
				is_not = true;
				word = char_append(word, code[i]);
				if (str_equal(word, "next") || str_equal(word, "break")) {
					is_not = false;
				}
			}
			if (!is_not) {
				state = NEXT_BREAK_LBL_INST;
				//msg("BBBBBBBBBBBBBBBB")
				//break
			}
		}
		//------------------is define variables
		if (!is_string && !is_equal && code[i] == ' ' && word != 0 &&
				!str_search(keywords_out, word, StrArraySize(keywords_out))) {
			//printf("Data type:%s,%i\n", word,search_datas(word, 0, 0, true).id);
			if (search_datas(word, 0, 0, true).id == 0 && !is_ret) {
				//exception_handler("wrong_type_var", "labeled_instruction:89", word, "")
				//TODO:
				return UNKNOWN_LBL_INST;
			} else {
				state = DEF_VARS_LBL_INST;
				//printf("RRRRRRRRRRRRRRRRRR:%s,%s\n",word,code);
			}
			//break
		}
		//------------------is '='
		if (!is_string && code[i] == '=') {
			is_equal = true;
		}
		//------------------is alloc magic macros
		if (!is_string && code[i] == '=', com_word != 0) {
			//printf("DDDD:%s,%s'n",com_word,buffer);
			String name, index;
			return_name_index_var(com_word, true, &name, &index);
			if (str_search(magic_macros, name, StrArraySize(magic_macros)))return ALLOC_MAGIC_MACROS_LBL_INST;
		}
		//------------------is structure
		if (!is_string && word != 0 && str_indexof(word, STRUCTURES_LABEL, 0) == 0) {
			state = STRUCTURE_LBL_INST;
			break;
		}
		//------------------is function call
		if (!is_string && code[i] == '(' && case_word != 0 && is_valid_name(case_word, false)) {
			state = FUNC_CALL_LBL_INST;
			//printf("SSSSSSSS:%s\n", case_word);
			last_pars = par - 1;
			//break
		} else if (last_pars > -1 && last_pars == par && state == FUNC_CALL_LBL_INST) {
			break;
		}
		//------------------is ++ or --
		if (!is_string && com_word != 0 && i + 1 < len &&
				((code[i] == '+' && code[i + 1] == '+') || (code[i] == '-' && code[i + 1] == '-')) &&
				state == UNKNOWN_LBL_INST && is_valid_name(com_word, true)) {
			//msg("TTT%$:", com_word)
			state = ALLOC_SHORT_LBL_INST;
			break;
		}
		//------------------
		//------------------
		//------------------append to buffer & word & case_word & com_word
		buffer = char_append(buffer, code[i]);
		//--------------
		if (!is_string && char_search(words_splitter, code[i])) {
			word_store[store_counter++] = str_trim_space(word);
			last_sep = code[i];
			if (store_counter >= 10) {
				store_counter = 0;
			}
			//msg("WORD:", word)
			word = 0;
		} else {
			word = char_append(word, code[i]);
			//msg("WORD_C", word)
		}
		//--------------
		if (!is_string && (char_search(words_splitter, code[i]) || char_search(single_operators, code[i]))) {
			case_word = 0;
		} else {
			case_word = char_append(case_word, code[i]);
		}
		//--------------
		if (!is_string && (char_search(words_splitter, code[i]) || char_search(single_operators, code[i])) &&
				bra == 0 && code[i] != '[' && code[i] != ']') {
			//msg("COM_WORD:", com_word)
			com_word = 0;
		} else {
			com_word = char_append(com_word, code[i]);
		}
		
	}
	//------------------final switch
	//msg("&&&&", state)
	if (is_equal && state == UNKNOWN_LBL_INST) {
		state = ALLOC_VARS_LBL_INST;
		//case state == labeled_inst["func_call"]:
	}
	//********************return state
	return state;
}

//****************************************************
String define_vars(String inst) {
	def_var_s vars_store[MAX_VAR_ALLOC_INSTRUCTIONS];
	uint8 vars_counter = define_vars_analyzing(inst, vars_store);
	//msg("&vars", vars_store)
	//*******************************************add to memory
	if (vars_counter == 0) {
		return 0;
	}
	long_int vars_list[MAX_VAR_ALLOC_INSTRUCTIONS];
	uint8 vars_list_counter=0;
	//msg("---------------Add to memory:")
	for(uint8 i = 0; i <= vars_counter; i++) {
		if (vars_store[i].value_var == 0) {
			str_init(&vars_store[i].value_var, "null");
		}
		long_int ret1 = set_memory_var(entry_table.cur_fin, entry_table.cur_sid, vars_store[i].name_var, vars_store[i].value_var, vars_store[i].main_type, true);
		//msg("&INIT_VAR:", st.name_var,st.value_var)
		if (ret1 == 0)
		{
			//delete all prev declared vars. because an error occur for one.
			for(uint8 i=0; i < vars_list_counter;i++)
			{
				//TODO:
				//delete_full_memory_var(find_index_var_memory(vars_list[i]), true);
			}
			return 0;
		}
		vars_list[vars_list_counter++] = ret1;
		
	}
	//show_memory(40)
	//msg("Define:", inst, cur_cid, cur_fid, cur_sid)
	return 0;
}


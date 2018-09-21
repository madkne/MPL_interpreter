//
// Created by amindelavar on 9/19/2018.
//

#include <MPL/system.h>

//****************************const vars
String structures_label = "@@@STRU_";
String switch_label = "@@@SWITCH_";
String segments_split = "\"?\"=!=\"?\""; // "?"=="?"
String short_split = "!@!";
String return_tmp_name = "@ret_";
String param_array_name = "__ArrayParam_";
String white_spaces = " \t\n\v\f\r";
#if LINUX_PLATFORM == 1
uint8 sep_platform         = '/';
String os_type             = "linux";
#elif WINDOWS_PLATFORM == 1
uint8 sep_platform = '\\';
String os_type = "windows";
#endif
//****************************dynamic vars
String interpreter_level = 0;
String project_root = 0;
String main_source_name = 0;
String interpreter_path = 0;
uint8 int_len = 10;
uint8 float_len = 16;
int8 errors_mode = ERROR_ID;
int8 warnings_mode = WARNING_ID;
uint8 is_programmer_debug = 1;
String TAB_SIZE = "\t\t\t\t\t\t"; //6
uint8 TAB_SIZE_INT = 6;

uint8 max_estimate_divide_huge = 8;
uint8 max_float_estimate_huge_X0 = 3;
uint8 max_steps_estimate_huge = 4;
String set_logfile_path = 0;
String new_line_char = "\n";
String os_tmp_dir = 0;
uint32 BufferSize = 1024;
String hostname = 0;
String os_version = 0;
uint64 os_total_memory = 0; //in bytes
uint64 os_total_disk = 0; //in kilo-bytes
long_int max_size_id = 0;
//****************************settings
Boolean is_check_inherits = true;
String is_add_to_mahlib = 0;
String is_set_logfile = 0;

String exceptions_group[] = {"ImportError",           //0
                             "MahlibError",          //1
                             "SyntaxError",          //2
                             "InterruptedError",     //3
                             "ValueError",           //4
                             "NotExistError",        //5
                             "RuntimeError",         //6
                             "IOError",              //7
                             "NotAccessError",       //8
                             "OSError",              //9
                             "OverflowError",        //10
                             "TypeError",            //11
                             "FloatingPointError",   //12
                             "CommandError"          //13
};
String exceptions_type[4] = {"CANCEL", "FATAL", "ERROR", "WARNING"};
String keywords[18] = {"func", "true", "false", "null", "if", "elif", "else", "loop", "manage", "next", "break",
                       "return", "num", "str", "bool", "struct", "vars", "import"};
String keywords_out[13] = {"func", "true", "false", "null", "if", "elif", "else", "loop", "manage", "next", "break",
                           "return", "import"};
String magic_macros[2] = {"__config", "__define"};
String block_instructions[5] = {"loop", "if", "elif", "else", "manage"};
//uint8 keywords_splitter[5] = {':', ' ', '{', '=', '('};
uint8 single_operators[] = {'+', '-', '*', '/', '%', '^', '|', '&', '~'};
String comparative_operators[] = {"==", ">=", "<=", ">", "<", "!="};
String alloc_operators[] = {"=", "+=", "-=", "*=", "/=", "%%=", "^=", ":=", "|=", "&=", "~="};
String boolean_operators[] = {"&&", "||", "~~"};
String basic_types[5] = {"str", "num", "bool", "struct", "vars"};
uint8 golden_bytes[] = {7, 11, 27, 127, 223};
String manage_init_parameters[] = {"str;str", "str;str;num", "str;str;num;num", "str;str;num;num;str;num"};
uint8 splitters[] = {' ', '(', ')', '=', '[', ']', '{', '}', ';', ','};
uint8 words_splitter[] = {':', ' ', '{', '=', '(', ',', '.', '[', ']', '}', ')', ';'};
String sub_types[] = {"s", "b", "i", "f", "h"};

utf8_str_list source_paths;
str_list program_argvs;
uint32 argvs_len = 0;

//*************************************************************
//*********************public functions************************
//*************************************************************
void init_database() {
	interpreter_path = 0;
	//---------------init entry_table
	entry_table.import_start = 0;
	entry_table.import_id = 1;
	entry_table.import_inst_count = 0;
	
	entry_table.exceptions_count = 0;
	entry_table.exli_start = 0;
	
	entry_table.utst_start = 0;
	entry_table.utf8_strings_id = 1;
	
	entry_table.soco_main_count = 0;
	entry_table.source_counter = 0;
	entry_table.soco_tokens_count = 0;
	entry_table.soco_tokens_start = 0;
	
	entry_table.blst_func_start = 0;
	entry_table.blst_stru_start = 0;
	entry_table.func_id = 1;
	entry_table.stru_id = 0;
	entry_table.func_index = 1;
	
	entry_table.datas_start = 0;
	entry_table.datas_id = 1;
	
	entry_table.instru_start = 0;
	entry_table.inst_id = 1;
	
	entry_table.inor_start = 0;
	entry_table.inor_count = 0;
	
	//    entry_table.asou_start=0;
	//    entry_table.coli_start=0;
	//    entry_table.vata_start=0;
	entry_table.line_number = 0;
	str_utf8 stdin_src;
	str_to_utf8(&stdin_src, "stdin");
	utf8_str_list_append(&source_paths, stdin_src, entry_table.source_counter++);
	//---------------init data_types
	datas tmp1 = {0, 0, 0, "str", MAIN_DATA_TYPE, 0, 0};
	append_datas(tmp1);
	datas tmp2 = {0, 0, 0, "num", MAIN_DATA_TYPE, 0, 0};
	append_datas(tmp2);
	datas tmp3 = {0, 0, 0, "bool", MAIN_DATA_TYPE, 0, 0};
	append_datas(tmp3);
	datas tmp4 = {0, 0, 0, "vars", MAIN_DATA_TYPE, 0, 0};
	append_datas(tmp4);
	//str;str;num;num;str;num
	
	datas tmp5 = {0, 0, 0, "exception", STRUCT_DATA_TYPE, 0, 0};
	append_datas(tmp5);
	
	//max_size_id=get_max_unsigned_size(sizeof(long_int)-1)-1000;
	//instance_pack = append(instance_pack, instance_pack_struct{"", "", 0})
	/*inst_order = append(inst_order, order_inst_struct{0, 0, 0, 0})*/
}


//*************************************************************
//*******************import_inst functions**********************
//*************************************************************
void append_imin(imin s) {
	imin *q;
	q = (imin *) malloc(sizeof(imin));
	if (q == 0) return;
	q->id = s.id;
	q->is_active = true;
	q->type = s.type;
	utf8_str_init(&q->path, s.path);
	q->line = s.line;
	q->err_code = 0;
	q->max_bytes_per_char = s.max_bytes_per_char;
	q->source_id = s.source_id;
	q->next = 0;
	if (entry_table.import_start == 0) {
		entry_table.import_start = entry_table.import_end = q;
	} else {
		entry_table.import_end->next = q;
		entry_table.import_end = q;
	}
	entry_table.import_inst_count++;
}

//*************************************************************
imin get_imin(long_int id) {
	imin ret = {0, 0, false, 0, 0, 0, 0};
	imin *tmp1 = entry_table.import_start;
	for (;;) {
		if (tmp1->id == id) {
			ret = *tmp1;
			break;
		}
		tmp1 = tmp1->next;
		if (tmp1 == 0) break;
	}
	return ret;
}

//*************************************************************
imin get_first_active_imin() {
	imin ret = {0, 0, false, 0, 0, 0, 0};
	imin *tmp1 = entry_table.import_start;
	for (;;) {
		if (tmp1->is_active) {
			ret = *tmp1;
			break;
		}
		tmp1 = tmp1->next;
		if (tmp1 == 0) break;
	}
	return ret;
}

//*************************************************************
void delete_imin(long_int id, Boolean is_delete) {
	imin *tmp1 = entry_table.import_start;
	for (;;) {
		if (tmp1->is_active && tmp1->id == id) {
			entry_table.import_inst_count--;
			if (!is_delete) {
				tmp1->is_active = false;
			} else {
				//TODO: delete import node
			}
			break;
		}
		tmp1 = tmp1->next;
		if (tmp1 == 0) break;
	}
}

//*************************************************************
//*******************source_code functions*********************
//*************************************************************
void append_soco(uint8 type, soco s) {
	soco *q;
	q = (soco *) malloc(sizeof(soco));
	if (q == 0) return;
	q->line = s.line;
	str_init(&q->code, s.code);
	q->next = 0;
	//soco_main
	if (type == 1) {
		//printf("QW!!!:%s,%i,%s\n", s.code, entry_table.soco_main_start, q->code);
		if (entry_table.soco_main_start == 0) {
			entry_table.soco_main_start = entry_table.soco_main_end = q;
		} else {
			entry_table.soco_main_end->next = q;
			entry_table.soco_main_end = q;
		}
		entry_table.soco_main_count++;
	}
		//soco tokens
	else if (type == 2) {
		if (entry_table.soco_tokens_start == 0)
			entry_table.soco_tokens_start = entry_table.soco_tokens_end = q;
		else {
			entry_table.soco_tokens_end->next = q;
			entry_table.soco_tokens_end = q;
		}
		entry_table.soco_tokens_count++;
	}
}

//*************************************************************
void clear_soco(uint8 type) {
	//soco_main
	if (type == 1 && entry_table.soco_main_start > 0) {
		entry_table.soco_main_count = 0;
		soco *tmp1;
		entry_table.soco_main_end = entry_table.soco_main_start;
		entry_table.soco_main_start = 0;
		for (;;) {
			tmp1 = entry_table.soco_main_end;
			entry_table.soco_main_end = entry_table.soco_main_end->next;
			free(tmp1);
			if (entry_table.soco_main_end == 0) break;
		}
	} else if (type == 2 && entry_table.soco_tokens_start > 0) {
		entry_table.soco_tokens_count = 0;
		soco *tmp1;
		entry_table.soco_tokens_end = entry_table.soco_tokens_start;
		entry_table.soco_tokens_start = 0;
		for (;;) {
			tmp1 = entry_table.soco_tokens_end;
			entry_table.soco_tokens_end = entry_table.soco_tokens_end->next;
			free(tmp1);
			if (entry_table.soco_tokens_end == 0) break;
		}
	}
}

//*************************************************************
soco get_soco(uint8 type, uint32 ind) {
	soco ret = {0, 0, 0};
	uint32 ii = 0;
	soco *tmp1;
	//soco_main
	if (type == 1) {
		tmp1 = entry_table.soco_main_start;
	}
		//soco_tokens
	else if (type == 2) {
		tmp1 = entry_table.soco_tokens_start;
	}
	for (;;) {
		if (ii++ == ind) {
			ret = *tmp1;
			break;
		}
		tmp1 = tmp1->next;
		if (tmp1 == 0) break;
	}
	
	return ret;
}

//*************************************************************
//*****************exceptions_list functions*******************
//*************************************************************
void append_exli(exli s) {
	exli *q;
	q = (exli *) malloc(sizeof(exli));
	if (q == 0) return;
	q->id = s.id;
	q->type = s.type;
	q->group = s.group;
	str_init(&q->lbl, s.lbl);
	str_init(&q->text, s.text);
	q->next = 0;
	if (entry_table.exli_start == 0)
		entry_table.exli_start = entry_table.exli_end = q;
	else {
		entry_table.exli_end->next = q;
		entry_table.exli_end = q;
	}
}

//*************************************************************
exli search_lbl_exli(String lbl) {
	exli ret = {0, 0, 0, 0, 0};
	exli *tmp1 = entry_table.exli_start;
	for (;;) {
		if (str_equal(tmp1->lbl, lbl)) {
			ret = *tmp1;
			break;
		}
		tmp1 = tmp1->next;
		if (tmp1 == 0) break;
	}
	return ret;
}

//*************************************************************
//******************utf8_strings functions*********************
//*************************************************************
void append_utst(utst s) {
	utst *q;
	q = (utst *) malloc(sizeof(utst));
	if (q == 0) return;
	q->id = s.id;
	q->max_bytes_per_char = s.max_bytes_per_char;
	utf8_str_init(&q->utf8_string, s.utf8_string);
	q->next = 0;
	if (entry_table.utst_start == 0)
		entry_table.utst_start = entry_table.utst_end = q;
	else {
		entry_table.utst_end->next = q;
		entry_table.utst_end = q;
	}
}

//*************************************************************
utst get_utst(long_int id) {
	utst ret = {0, 0, 0, 0};
	utst *tmp1 = entry_table.utst_start;
	for (;;) {
		if (tmp1->id == id) {
			ret = *tmp1;
			break;
		}
		tmp1 = tmp1->next;
		if (tmp1 == 0) break;
	}
	return ret;
}

//*************************************************************
//****************block_structures functions*******************
//*************************************************************
void append_blst(blst s) {
	blst *q;
	q = (blst *) malloc(sizeof(blst));
	if (q == 0) return;
	q->func_id = s.func_id;
	q->stru_id = s.stru_id;
	q->type = s.type;
	q->line = s.line;
	q->source_id = s.source_id;
	q->params_len = s.params_len;
	str_init(&q->lbl, s.lbl);
	//printf("FFFFFF:%li,%i\n",s.params,entry_table.params_len);
	str_list_init(&q->params, s.params, s.params_len);
	q->next = 0;
	//blst_func
	if (s.type == FUNC_BLOCK_ID) {
		q->id = entry_table.func_id++;
		if (entry_table.blst_func_start == 0)
			entry_table.blst_func_start = entry_table.blst_func_end = q;
		else {
			entry_table.blst_func_end->next = q;
			entry_table.blst_func_end = q;
		}
	}
		//blst_stru
	else {
		q->id = ++entry_table.stru_id;
		if (entry_table.blst_stru_start == 0)
			entry_table.blst_stru_start = entry_table.blst_stru_end = q;
		else {
			entry_table.blst_stru_end->next = q;
			entry_table.blst_stru_end = q;
		}
	}
}

//*************************************************************
long_int search_lbl_func(String lbl) {
	blst *tmp1;
	tmp1 = entry_table.blst_func_start;
	//------------------
	for (;;) {
		if (str_equal(tmp1->lbl, lbl)) {
			return tmp1->id;
		}
		tmp1 = tmp1->next;
		if (tmp1 == 0) break;
	}
	return 0;
}

//*************************************************************
//******************data_types functions*********************
//*************************************************************
void append_datas(datas s) {
	datas *q;
	q = (datas *) malloc(sizeof(datas));
	if (q == 0) return;
	q->id = ++entry_table.datas_id;
	q->type = s.type;
	q->fid = s.fid;
	q->sid = s.sid;
	str_init(&q->name, s.name);
	
	q->next = 0;
	if (entry_table.datas_start == 0)
		entry_table.datas_start = entry_table.datas_end = q;
	else {
		entry_table.datas_end->next = q;
		entry_table.datas_end = q;
	}
}

//*************************************************************
//******************instructions functions*********************
//*************************************************************
void append_instru(instru s) {
	instru *q;
	q = (instru *) malloc(sizeof(instru));
	if (q == 0) return;
	q->id = entry_table.inst_id++;
	q->func_id = s.func_id;
	q->stru_id = s.stru_id;
	q->order = s.order;
	q->line = q->line;
	q->type = s.type;
	q->source_id = s.source_id;
	str_init(&q->code, s.code);
	q->next = 0;
	if (entry_table.instru_start == 0)
		entry_table.instru_start = entry_table.instru_end = q;
	else {
		entry_table.instru_end->next = q;
		entry_table.instru_end = q;
	}
}

//*************************************************************
//***************instructions_order functions******************
//*************************************************************
void append_inor(inor s) {
	inor *q;
	q = (inor *) malloc(sizeof(inor));
	if (q == 0) return;
	q->fid = s.fid;
	q->sid = s.sid;
	q->order = s.order;
	q->next = 0;
	entry_table.inor_count++;
	if (entry_table.inor_start == 0) {
		entry_table.inor_start = entry_table.inor_end = q;
	} else {
		entry_table.inor_end->next = q;
		entry_table.inor_end = q;
	}
}

//*************************************************************
long_int get_order(long_int fid, long_int sid) {
	inor *tmp1 = entry_table.inor_start;
	if (tmp1 == 0) return 0;
	for (;;) {
		if (tmp1->fid == fid && tmp1->sid == sid) {
			return tmp1->order;
		}
		tmp1 = tmp1->next;
		if (tmp1 == 0) break;
	}
	return 0;
}

//*************************************************************
void set_order(long_int fid, long_int sid, long_int order) {
	uint32 index = 0;
	Boolean is_exist = false;
	//***************search
	inor *tmp1 = entry_table.inor_start;
	if (tmp1 != 0) {
		for (;;) {
			index++;
			if (tmp1->fid == fid && tmp1->sid == sid) {
				is_exist = true;
				break;
			}
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
	}
	//**************set
	if (!is_exist) {
		inor tmp2 = {fid, sid, order, 0};
		append_inor(tmp2);
	} else {
		uint32 ind = 0;
		inor *tmp1 = entry_table.inor_start;
		for (;;) {
			ind++;
			if (ind == index) {
				tmp1->order = order;
				break;
			}
			tmp1 = tmp1->next;
			if (tmp1 == 0) break;
		}
	}
}

//*************************************************************
//***************stru_to_in_struct functions******************
//*************************************************************
void empty_stoi(stoi s[], uint32 size) {
	for (uint32 i = 0; i < size; i++) {
		s[i].id = 0;
		s[i].is_active = false;
		s[i].is_inline = false;
		s[i].stru_pars = 0;
	}
}
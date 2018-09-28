//
// Created by amindelavar on 9/21/2018.
//
#include <MPL/system.h>

Boolean start_runtime() {
	//*************init variables
	entry_table.func_index++; //add one for main() function
	//add_to_prev_fins_array(0)
	entry_table.cur_fid = 0, entry_table.cur_fin = entry_table.func_index, entry_table.cur_sid = 0, entry_table.cur_order = 1;
	//*************find main function
	blst main_func = search_lbl_func("main", 0, 0);
	if (main_func.id == 0) {
		print_error(0, "not_found_main", "stdin", 0, 0, "start_runtime");
		return false;
	}
	entry_table.cur_fid = main_func.id;
	//printf("LLLL:%i,%s\n",main_func.id,main_func.lbl);
	//*************init global vars
	//init_global_vars();
	//*************start program from fid=2,fin=2,sid=0,order=1
	APP_CONTROLLER();
	//*************return
	//msg("\t----------------------\n", run_order)
	//show_memory(0)
	return true;
}

//******************************
Boolean APP_CONTROLLER() {
	//*************browsing execute instructions
	//for (;;) {
	/*if (return_fin == cur_fin && return_fin > 0) {
		return 1;
	} else if (return_fin > 0) {
		return_fin = 0;
	}
	if (break_count > 0 && !is_stop_APP_CONTROLLER) {
		//msg("&BREAKS:", break_count)
		break_count--;
		is_stop_APP_CONTROLLER = true;
	}
	if (is_stop_APP_CONTROLLER) {
		//msg("&BREAKS:", break_count)
		is_stop_APP_CONTROLLER = false;
		return 2;
	}*/
	//***************search for current instruction
	instru *st = entry_table.instru_start;
	if (st == 0) return false;
	for (;;) {
		//printf("XSSSS:%i<>%i,%i<>%i,%i<>%i\n", st->func_id, cur_fid , st->stru_id, cur_sid , st->order, cur_order);
		if (st->func_id == entry_table.cur_fid && st->stru_id == entry_table.cur_sid &&
				st->order == entry_table.cur_order) {
			//printf("IIIII:%s\n", st->code);
			entry_table.Rorder = st->order;
			//************show instruction line by line
			if (is_programmer_debug >= 2) {
				printf("---------INSTRUCTION(i:%i,f:%i,S:%i,Order:%i)\n%s$$$\n", st->id, st->func_id, st->stru_id,
						st->order, st->code);
			}
			//************run instruction
			int8 ret0 = INSTRUCTION_EXECUTOR(st->id);
			//msg("&UUUU")
			//msg("&Inst", st.text, ret0, cur_fin)
			if (ret0 == FAILED_EXECUTE_INSTRUCTION) {
				return false;
			} else if (ret0 == SUCCESS_EXECUTE_INSTRUCTION) {
			
			}
			/*if (is_next_inst_running//for next inst) {
				return 13;
			}*/
			//msg("&YGGGG",ret0)
			entry_table.cur_order++;
		}
		st = st->next;
		if (st == 0) break;
	}
	
	//}
	//show_memory(0)
	//msg("&nnnnnnnnnn", rt)
	return 1;
}

//******************************
int8 INSTRUCTION_EXECUTOR(long_int index) {
	instru st = get_instru_by_id(index);
	String Rcode = str_trim_space(st.code);
	entry_table.Rline = st.line;
	entry_table.Rsrc = utf8_to_bytes_string(source_paths[st.source_id]);
	Boolean is_done = true;
	//	if Rcode == "@__@" {
	//		show_memory(40)
	//		return 0
	//	}
	while ((Rcode = str_trim_space(Rcode)) != 0) {
		//msg("&Rcode", Rcode)
		uint8 state = labeled_instruction(Rcode);
		if (is_programmer_debug >= 0) {
			printf("@###############INST(State:%i,fin:%i,line:%i):\n%s\n@###############\n", state, entry_table.cur_fin,
					entry_table.Rline, Rcode);
		}
		//********************
		switch (state) {
			case UNKNOWN_LBL_INST: {
				is_done = false;
				exception_handler("unknown_instruction", "INSTRUCTION_EXECUTOR", Rcode, "");
				break;
			}
			case DEF_VARS_LBL_INST:
				Rcode = define_vars(Rcode);
				if (str_equal(Rcode, "bad")) is_done = false;
				break;
			case FUNC_CALL_LBL_INST:
				is_done = false;
				//Rcode = function_call(Rcode, 0);
				break;
			case ALLOC_VARS_LBL_INST:
				is_done = false;
				//Rcode = vars_allocation(Rcode)
				break;
			case RETURN_LBL_INST: {
				is_done = false;
				/*Rcode = func_return(Rcode)
				if Rcode == "true"
				{
					return 3
				} else if Rcode == "false"
				{
					return 1
				}*/
				break;
			}
			case STRUCTURE_LBL_INST:
				is_done = false;
				//Rcode = init_structures(Rcode)
				break;
			case ALLOC_SHORT_LBL_INST:
				is_done = false;
				//Rcode = vars_allocation_short(Rcode)
				break;
			case NEXT_BREAK_LBL_INST: {
				is_done = false;
				/*Rcode = next_break_loop(Rcode)
				if Rcode == "-next"
				{
					return 13
				}*/
				break;
			}
			default: {
				is_done = false;
				//exception_handler("unknown_instruction", "INSTRUCTION_EXECUTOR", Rcode, "")
				break;
			}
			
		}
		//********************
		if (!is_done) {
			break;
		}
	}
	//garbage_collector('@')
	//********************is DONE!!
	if (is_programmer_debug >= 2) {
		if (is_done) {
			printf("~~~~~~~~~~~~~~~~~~>DONE :)\n\n");
		} else {
			printf("~~~~~~~~~~~~~~~~~~>BREAK :(\n\n");
		}
		//show_prev_fins_array()
	}
	//show_memory(0)
	return SUCCESS_EXECUTE_INSTRUCTION;
}

//******************************
Boolean init_global_vars() {
	long_int order = 1;
	instru *st = entry_table.instru_start;
	if (st == 0) return false;
	for (;;) {
		if (st->func_id == 0 && st->order == order) {
			printf("IIIII:%s\n", st->code);
			uint8 count = 0;
			//count = alloc_magic_macros(st->code);
			
			if (count == 0) {
				def_var_s vars_store[MAX_VAR_ALLOC_INSTRUCTIONS];
				count = define_vars_analyzing(st->code, vars_store);
				printf("XXXXXX:\n");
				for (int i = 0; i < count; ++i) {
					printf("VVVV:%s,%s\n", vars_store[i].name_var, vars_store[i].value_var);
				}
			}
			
		}
		order++;
		st = st->next;
		if (st == 0) break;
	}
}

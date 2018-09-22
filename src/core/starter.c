//
// Created by amindelavar on 9/21/2018.
//
#include <MPL/system.h>

//**********************************basic variables
long_int cur_fid, cur_fin, cur_sid, cur_order, last_fin, last_sid, prev_fin_index;
long_int prev_fins_array[100];
uint32 condition_level = 0, in_loop = 0, break_count = 0, next_count = 0;
String manage_func_name = 0, Rsrc = 0;
long_int manage_func_id = 0, return_fin = 0;
long_int Rorder = 0, Rline = 0;
Boolean is_stop_APP_CONTROLLER = false, is_next_inst_running = false;

Boolean start_runtime() {
	//*************init variables
	//msg("XXXXXXXXXXXXXXXXXXXX")
	//start Program from cid=0,fid=1,sid=0,order=1
	entry_table.func_index++;
	//add_to_prev_fins_array(0)
	cur_fid=0, cur_fin = entry_table.func_index,cur_sid=0,cur_order = 1;
	//*************find main function
	blst *tmp1;
	tmp1 = entry_table.blst_func_start;
	//------------------
	blst main_func=search_lbl_func("main",0,0);
	if(main_func.id==0){
		print_error(0,"not_found_main","stdin",0,0,"start_runtime");
		return false;
	}
	cur_fid=main_func.id;
	//*************init global vars
	//init_global_vars();
	//*************start program
	//APP_CONTROLLER();
	//*************return
	//msg("\t----------------------\n", run_order)
	//show_memory(0)
	return true;
}

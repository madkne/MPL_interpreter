//
// Created by amindelavar on 9/21/2018.
//
#include <MPL/system.h>
//**********************************************************
Boolean start_runtime() {
  //*************init global vars
  init_global_vars();
  //*************init variables
  entry_table.func_index++; //add one for main() function
  //add_to_prev_fins_array(0)
  entry_table.cur_fid = 0, entry_table.cur_fin = entry_table.func_index, entry_table.cur_sid = 0, entry_table
      .parent_fin = 0,
      entry_table.cur_order = 1;
  //*************find main function
  blst main_func = search_lbl_func("main", 0, 0);
  if (main_func.id == 0) {
    print_error(0, "not_found_main", "stdin", 0, 0, "start_runtime");
    return false;
  }
  entry_table.cur_fid = main_func.id;
  //printf("LLLL:%i,%s\n",main_func.id,main_func.lbl);
  //*************start program from fid=2,fin=2,sid=0,order=1
  APP_CONTROLLER();
  //*************return
  return true;
}

//**********************************************************
int8 APP_CONTROLLER() {
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
        //printf("---------INSTRUCTION(i:%i,f:%i,S:%i,Order:%i)\n%s$$$\n", st->id, st->func_id, st->stru_id,st->order, st->code);
      }
      //************run instruction
      int8 ret0 = INSTRUCTION_EXECUTOR(st->id);
      //msg("&UUUU")
      //msg("&Inst", st.text, ret0, cur_fin)
      if (ret0 == FAILED_EXECUTE_INSTRUCTION) {
        return BAD_RETURN_APP_CONTROLLER;
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
  return NORMAL_RETURN_APP_CONTROLLER;
}

//**********************************************************
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
  if (str_equal(Rcode, "null")) return SUCCESS_EXECUTE_INSTRUCTION;
  while ((Rcode = str_trim_space(Rcode)) != 0) {
    //msg("&Rcode", Rcode)
    uint8 state = labeled_instruction(Rcode);
    if (is_programmer_debug >= 0) {
      printf("@###############INST(fid:%i,sid:%i,order:%i,state:%i,fin:%i,line:%i):\n%s\n", entry_table.cur_fid,
             entry_table.cur_sid, entry_table.Rorder, state, entry_table.cur_fin, entry_table.Rline, Rcode);
    }
    //********************
    if (state == UNKNOWN_LBL_INST) {
      is_done = false;
      if (str_equal(Rcode, "null")) return SUCCESS_EXECUTE_INSTRUCTION;
      exception_handler("unknown_instruction", __func__, Rcode, "");
    } else if (state == DEF_VARS_LBL_INST) Rcode = define_vars(Rcode);
    else if (state == ALLOC_MAGIC_MACROS_LBL_INST)Rcode = alloc_magic_macros(Rcode);
    else if (state == FUNC_CALL_LBL_INST)Rcode = function_call(Rcode);
    else if (state == ALLOC_VARS_LBL_INST) {
      int8 status = vars_allocation(Rcode);
      Rcode = 0;
      if (status == -1)is_done = false;
    } else if (state == RETURN_LBL_INST) {
      Boolean ret = function_return(Rcode);
      Rcode = 0;
      /*if Rcode == "true"
      {
          return 3
      } else if Rcode == "false"
      {
          return 1
      }*/
    } else if (state == ALLOC_SHORT_LBL_INST) {
      Rcode = vars_allocation_short(Rcode);
    }
      //else if(state==STRUCTURE_LBL_INST)Rcode = init_structures(Rcode);
      //		  case NEXT_BREAK_LBL_INST:
      //			{
      //			  is_done = false;
      //			  /*Rcode = next_break_loop(Rcode)
      //			  if Rcode == "-next"
      //			  {
      //				  return 13
      //			  }*/
      //			  break;
      //			}
    else {
      is_done = false;
      //exception_handler("unknown_instruction", "INSTRUCTION_EXECUTOR", Rcode, "")
    }
    if (str_equal(Rcode, BAD_CODE)) is_done = false;
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
      //check post short alloc
      if (entry_table.post_short_alloc_len > 0) {
        for (uint32 i = 0; i < entry_table.post_short_alloc_len; i++) {
          str_list tokens = 0;
          //printf("ER#%s\n",entry_table.post_short_alloc[i]);
          char_split(entry_table.post_short_alloc[i], ';', &tokens, true);
          is_done = do_show_allocation(tokens[0], str_to_bool(tokens[1]));
          if (!is_done) break;
        }
        entry_table.post_short_alloc_len = 0;
      }
    }
    if (!is_done) {
      printf("~~~~~~~~~~~~~~~~~~>BREAK :(\n\n");
    }
    //show_prev_fins_array()
  }
  //show_memory(0)
  return SUCCESS_EXECUTE_INSTRUCTION;
}

//**********************************************************
Boolean init_global_vars() {
  long_int order = 1;
  entry_table.cur_fin = 0;
  entry_table.cur_sid = 0;
  entry_table.cur_fid = 0;
  instru *st = entry_table.instru_start;
  if (st == 0) return false;
  for (;;) {
    if (st->func_id == 0 && st->order == order) {
      String Rcode = str_trim_space(st->code);
      entry_table.Rline = st->line;
      Boolean is_done = true;
      uint8 state = labeled_instruction(Rcode);
      if (is_programmer_debug >= 0) {
        printf("@###############GLOBAL_INST(State:%i,fin:%i,line:%i):\n%s\n@###############\n", state,
               entry_table.cur_fin, entry_table.Rline, Rcode);
      }
      //********************
      switch (state) {
        case DEF_VARS_LBL_INST: {
          Rcode = define_vars(Rcode);
          if (str_equal(Rcode, "bad")) is_done = false;
          break;
        }
        case ALLOC_MAGIC_MACROS_LBL_INST: {
          Rcode = alloc_magic_macros(Rcode);
          if (str_equal(Rcode, "bad")) is_done = false;
          break;
        }
        default: {
          is_done = false;
          exception_handler("incorrect_global_inst", __func__, Rcode, 0);
          break;
        }
      }
      //********************is DONE!!
      if (is_programmer_debug >= 2) {
        if (is_done) {
          printf("~~~~~~~~~~~~~~~~~~>DONE :)\n\n");
        } else {
          printf("~~~~~~~~~~~~~~~~~~>BREAK :(\n\n");
        }
        //show_prev_fins_array()
      }
    }
    order++;
    st = st->next;
    if (st == 0) break;
  }
}

//**********************************************************

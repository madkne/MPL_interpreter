//
// Created by amindelavar on 9/23/2018.
//

#include <MPL/system.h>

//********************************************
uint8 labeled_instruction(String code) {
  //----------------------init variables
  uint8 state = UNKNOWN_LBL_INST;
  Boolean is_string = false, is_equal = false, is_ret = false;
  uint16 par = 0/*count of parenthesis */, bra = 0/*count of brackets*/, aco = 0/*count of acolads*/, store_counter = 0,
      tmp_short_alloc = 0;
  String word = 0/*just create by words_splitter*/, case_word = 0/*create by words_splitter,single_operators*/,
      com_word = 0/*create by words_splitter,single_operators and skip brackets*/, buffer = 0;
  int16 last_pars = -1;
  String word_store[10];
  uint8 last_sep = 0;
  uint32 len = str_length(code);
  //msg("&LABELED:", code)


  //----------------------analyzing code line
  for (uint32 i = 0; i < len; i++) {
    //------------------check is string
    if (code[i] == '\"' && (i == 0 || code[i - 1] != '\\')) {
      is_string = switch_bool(is_string);
    }
    //------------------count parenthesis,brackets,acolads
    if (!is_string) {
      if (i + 1 < len && code[i] == ' ' && code[i + 1] == '(') continue;
        //count parenthesis
      else if (code[i] == '(') par++;
      else if (code[i] == ')') par--;
        //count brackets
      else if (code[i] == '[')bra++;
      else if (code[i] == ']')bra--;
        //count acolads
      else if (code[i] == '{')aco++;
      else if (code[i] == '}')aco--;
    }
    //------------------is return keyword
    if (!is_string && (code[i] == ' ' || code[i] == '(' || i + 1 == len) && word != 0 &&
        str_equal(word, "return")) {
//      printf("*lbl:return\n");
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
        !str_search(keywords_out, word, StrArraySize(keywords_out)) && !is_ret) {
      //printf("Data type:%s,%i\n", word,search_datas(word, 0, 0, true).id);
      if (search_datas(word, 0, true).id == 0) {
        exception_handler("wrong_type_var", __func__, word, 0);
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
      if (str_search(magic_macros, name, StrArraySize(magic_macros)))
        return ALLOC_MAGIC_MACROS_LBL_INST;
    }
    //------------------is structure
    if (!is_string && word != 0 && str_indexof(word, STRUCTURES_LABEL, 0) == 0) {
      state = STRUCTURE_LBL_INST;
      break;
    }
    //------------------is function call
    if (!is_string && code[i] == '(' && case_word != 0 && is_valid_name(case_word, false)) {
      state = FUNC_CALL_LBL_INST;
//      printf("SSSSSSSS:%s\n", case_word);
      last_pars = par - 1;
      //break
    } else if (last_pars > -1 && last_pars == par && state == FUNC_CALL_LBL_INST) {
      break;
    }
    //------------------is ++ or --
    if (!is_string && i + 1 < len
        && ((code[i] == '+' && code[i + 1] == '+') || (code[i] == '-' && code[i + 1] == '-'))) {
      tmp_short_alloc = i + 1;
    }
    if (!is_string && com_word != 0 && (is_valid_name(com_word, true)
        || (com_word == 0 && is_valid_name(char_to_str(code[i]), true))) && (
        (tmp_short_alloc > 0) ||
            (i + 1 < len && ((code[i] == '+' && code[i + 1] == '+') || (code[i] == '-' && code[i + 1] == '-')))
    )) {
      //printf ("TTT:%s\n", com_word);
      tmp_short_alloc = 0;
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
  //printf("!@!!@!@:%s,%i\n",inst,vars_counter);
  //msg("&vars", vars_store)
  //*******************************************add to memory
  if (vars_counter == 0) {
    return 0;
  }
  long_int vars_list[MAX_VAR_ALLOC_INSTRUCTIONS];
  uint8 vars_list_counter = 0;
  //msg("---------------Add to memory:")
  for (uint8 i = 0; i < vars_counter; i++) {
    if (vars_store[i].value_var == 0) {
      str_init(&vars_store[i].value_var, "null");
    }
    //printf("%DEF_VAR:%s;;%s;;%s\n",vars_store[i].main_type,vars_store[i].name_var,vars_store[i].value_var);
    vars_store[i].name_var = str_multi_append(vars_store[i].name_var, "[", vars_store[i].index_var, "]", 0, 0);
    long_int ret1 = set_memory_var(entry_table.cur_fin, entry_table.cur_sid, vars_store[i].name_var, vars_store[i]
        .value_var, vars_store[i].main_type, true);
    //msg("&INIT_VAR:", st.name_var,st.value_var)
    if (ret1 == 0) {
      return BAD_CODE;
    }
    vars_list[vars_list_counter++] = ret1;

  }
  //show_memory(0);
  //msg("Define:", inst, cur_cid, cur_fid, cur_sid)
  return 0;
}

//****************************************************
String alloc_magic_macros(String exp) {
  /**
   * __define["Hello"]=j
   * num gh=__define["Hello"]+3
   */
  //---------------init
  //printf("@SSS:%s\n", exp);
  String ret_exp = 0, buf = 0, mm_name = 0, mm_index = 0;
  uint8 mm_type = 0;
  int32 start = -1, end = -1, bras_start = -1, equal_ind = -1;
  Boolean is_string = false, is_buf = false, is_use_magic = true;
  uint8 pars = 0/*count of parenthesis*/, bras = 0/*count of brackets*/;
  uint32 exp_len = str_length(exp);
  //---------------start analyzing
  for (int32 i = 0; i < exp_len; i++) {
    //=====check is string
    if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\')) {
      is_string = switch_bool(is_string);
    }
    //=====count pars
    if (!is_string) {
      if (exp[i] == ' ') continue;
      else if (exp[i] == '(' && bras == 0)pars++;
      else if (exp[i] == ')' && bras == 0)pars--;
      else if (exp[i] == '[')bras++;
      else if (exp[i] == ']')bras--;
    }
    //=====check if magic macro
    if (!is_string && buf == 0 && i + 2 < exp_len && exp[i] == '_' && exp[i + 1] == '_') {
      is_buf = true;
      start = i;
      bras_start = bras;
    }
    //=====get a magic macro
    if (!is_string && buf != 0 && is_buf && bras_start == bras && exp[i] != ']' &&
        (char_search(single_operators, exp[i]) || char_search(words_splitter, exp[i]) || i + 1 == exp_len)) {
      is_buf = false;
      end = i;
      bras_start = -1;
      //printf("OOOOO:%s$\n", buf);
    }
    //=====is equal
    if (!is_string && exp[i] == '=') {
      equal_ind = i;
//      printf("@@@@@@@@@@@@:%i,%i\n",i,end);
      if (end != -1 && i >= end)is_use_magic = false;
    }

    //=====append to buf
    if (is_buf) {
      buf = char_append(buf, exp[i]);
    }
  }
  if (buf == 0) {
    return BAD_CODE;
  }
  //---------------get name and index of magic macro
  return_name_index_var(buf, false, &mm_name, &mm_index);
  mm_index = str_reomve_quotations(mm_index, "s");
  //---------------determine type of magic macro
  if (str_equal(mm_name, "__define"))mm_type = DEFINE_MAGIC_MACRO_TYPE;
  else if (str_equal(mm_name, "__config"))mm_type = CONFIG_MAGIC_MACRO_TYPE;
  else if (str_equal(mm_name, "__session"))mm_type = SESSION_MAGIC_MACRO_TYPE;
  else {
    exception_handler("unknown_magic_macro", __func__, mm_name, 0);
    return BAD_CODE;
  }
  //---------------is use type
  if (is_use_magic) {
    //printf("##use_magic:%s=>%s(%s,%s)\n", exp, buf, mm_name, mm_index);
    mama s = get_mama(mm_type, mm_index);
    if (s.id == 0) {
      exception_handler("not_defined_mm_key", __func__, mm_index, mm_name);
      return BAD_CODE;
    }
    if (s.sub_type == 's') s.value = convert_to_string(s.value);
    ret_exp = replace_in_expression(exp, s.value, start, end, true, true);
  }
    //---------------is define type
  else {
    String value = str_substring(exp, equal_ind + 1, 0);
    //-------------analyzing vlaue
    //check if has magic macro
    while (true) {
      String tmp1 = alloc_magic_macros(value);
      if (str_equal(tmp1, BAD_CODE))break;
      str_init(&value, tmp1);
    }
    //calculate value
    //printf("EEEEE:%s$\n",value);
    String type = determine_value_type(value);
    uint8 subtype = '0';
    calculate_value_of_var(value, type, &value, &subtype);
    if (mm_type == DEFINE_MAGIC_MACRO_TYPE && get_mama(mm_type, mm_index).id > 0) {
      exception_handler("reinitialized_in__define_mm", __func__, mm_index, mm_name);
      return BAD_CODE;
    } else if (mm_type == CONFIG_MAGIC_MACRO_TYPE && get_mama(CONFIG_MAGIC_MACRO_TYPE, mm_index).id == 0) {
      exception_handler("not_exist__config_mm", __func__, mm_index, 0);
      return BAD_CODE;
    } else if (mm_type == CONFIG_MAGIC_MACRO_TYPE && entry_table.cur_fin != 0) {
      exception_handler("not_global__config_mm", __func__, 0, 0);
      return BAD_CODE;
    }
    //TODO:errors
    //printf("##define_magic:%s=>%s(%s,%s) : %s,%s,%c\n", exp, buf, mm_name, mm_index, value, type, subtype);
    if (subtype == 's')value = str_reomve_quotations(value, "s");
    if (!edit_magic_macro(mm_type, mm_index, value))
      add_to_mama(mm_type, subtype, mm_index, value);
    ret_exp = 0;
  }
  //---------------return
  //printf("@EEEEEEEE:%s\n", ret_exp);
  return ret_exp;
}

//****************************************************
String function_call(String exp) {
  /**
  1- os.out("Hello") ---OK---
  2- inst1.find(5,"hi") ---OK---
  3- inst1[1].find(5,"hi")
  4- inst1.find(5,"hi")[0] ---OK---
  5- st.splite(",").join("--") ---OK---
  6- class1.get()[1].ConvertToBin() //num num1,num2=class1.get() || num2.ConvertToBin()
  7- 89*(num1.ConvertToBin()+23) ---OK---
  8- inst1.find(5,{{"hi"},"ui"}) ---OK---
  9- inst1[inst1[1+dg.size()]+4].find(4) ---OK---
  */
  //********************init variables
  String ret_exp = 0, buffer = 0, word = 0, index = 0, func_name = 0, pack_name = 0, ret_vars = 0;
  uint8 func_type = FUNC_TYPE_NORMAL;
  str_list parameters = 0;
  uint32 params_len = 0;
  Boolean is_string = false, is_par = false, is_struct = false;
  int32 pars = 0, bras = 0, acos = 0, st_func = -1, en_func = -1, tmp1 = -1, pars_num = -1, bras_num = 0, en_pack = -1,
      count_ind = 0, struct_par = -1, equal_pos = -1;
  uint32 exp_len = str_length(exp);
  //********************start analyzing
  for (uint32 i = 0; i < exp_len; i++) {
    Boolean no_buffer = false;
    //------------------check is string
    if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\')) {
      is_string = switch_bool(is_string);
    }
    //------------------check is struct
    if (!is_string && !is_struct && exp[i] == '(' && str_equal(word, "struct")) {
      is_struct = true;
      struct_par = pars + 1;
    } else if (!is_string && is_struct && exp[i] == ')' && struct_par == pars) {
      is_struct = false;
    }
    //------------------if is equal
    if (!is_string && exp[i] == '=' && equal_pos == -1) {
      equal_pos = i;
    }
    //------------------count parenthesis
    if (!is_string) {
      if (exp[i] == '(')pars++;
      else if (exp[i] == ')')pars--;
      else if (exp[i] == '[')bras++;
      else if (exp[i] == ']')bras--;
      else if (exp[i] == '{')acos++;
      else if (exp[i] == '}')acos--;
    }
    //------------------if is "::" (module name) (pack_name)
    if (!is_string && i + 1 < exp_len && exp[i] == ':' && exp[i + 1] == ':' && word != 0 && is_valid_name(word, true)
        && en_func == -1 && acos == 0) {
      st_func = tmp1;
      str_init(&pack_name, word);
      func_name = 0;
      is_par = false;
      en_func = -1;
      en_pack = i + 1;
      i++;
      str_empty(&word);
      func_type = FUNC_TYPE_MODULE;
      continue;
    }
    //------------------if is "->" (pack name) (pack_name)
    if (!is_string && i + 1 < exp_len && exp[i] == '-' && exp[i + 1] == '>' && word != 0 && is_valid_name(word, true)
        && en_func == -1 && acos == 0) {
      st_func = tmp1;
      str_init(&pack_name, word);
      func_name = 0;
      is_par = false;
      en_func = -1;
      en_pack = i + 1;
      i++;
      str_empty(&word);
      func_type = FUNC_TYPE_PACKAGE;
      continue;
    }
    //------------------if is '(' (func_name)
    if (!is_string && exp[i] == '(' && word != 0 && !str_equal(word, "struct") && is_valid_name(word, false) &&
        en_func == -1 && acos == 0) {
      //printf("&HHHH:%s,%s\n", word,print_str_list(parameters, params_len));
//      printf("FFFF:%i,%i,%i,%s\n",en_pack,i,str_length(word),word);
      if (en_pack > -1 && i - en_pack - 1 > str_length(word)) {
        //[sample] fs::abspath("sss",er(45))
        pack_name = 0;
        en_pack = -1;
        func_type = FUNC_TYPE_NORMAL;
        st_func = -1;
      }
      str_init(&func_name, word);
      pars_num = pars - 1;
      bras_num = bras;
      parameters = 0;
      params_len = 0;
      is_par = true;
      is_struct = false;
      str_empty(&word);
      str_empty(&buffer);
      if (st_func == -1)st_func = tmp1;
      continue;
    }
    //------------------ get parameters
    if (!is_string && !is_struct && (exp[i] == ',' || (exp[i] == ')' && pars == pars_num)) && is_par && acos == 0 &&
        bras == bras_num) {
      buffer = str_trim_space(buffer);
      //msg("&DDD:", buffer, pars_num, pars)
      if (buffer != 0)str_list_append(&parameters, buffer, params_len++);

      if (exp[i] == ')' && pars == pars_num) {
        is_par = false;
        en_func = i + 1;
      }
      if (!is_par && exp[i] == ')') {
        //is func has index
        int32 br = 0;
        for (uint32 b = i + 1; b < exp_len; b++) {
          if (exp[b] != '[' && exp[b] != ' ' && br == 0) break;
          else if (exp[b] == '[')br++;
          else if (exp[b] == ']')br--;
          index = char_append(index, exp[b]);
          en_func = b + 1;
          if (br == 0)break;
        }
        index = str_substring(index, 1, str_length(index) - 1);
        break;
      }
      str_empty(&buffer);
      str_empty(&word);
      continue;
    }
    //------------------append to buffer & word
    if (!no_buffer) {
      buffer = char_append(buffer, exp[i]);
    }
    if (!is_string &&
        (exp[i] == ' ' || exp[i] == '.' || exp[i] == ',' || exp[i] == '(' || exp[i] == ')' || exp[i] == '=' ||
            char_search(single_operators, exp[i])) && bras == 0) {
      count_ind = i;
      word = 0;
    } else {
      if (word == 0) tmp1 = i;
      word = char_append(word, exp[i]);
    }
    //msg("TTTT:", word)
  }
  //********************calling functions
//  printf("***FUNC_CALL(%s):\nFtype:%i,Pname:%s,Fname:%s,Params:%s[%i],Index:%s,(start:%i,end:%i),Return:%s=>%s\n", exp,func_type, pack_name, func_name, print_str_list(parameters, params_len),params_len,index, st_func, en_func, ret_exp, str_substring(exp, st_func, en_func));
//  show_memory(0);

  ret_vars = init_calling_function(func_type, pack_name, func_name, parameters, params_len, index);
  if (ret_vars == 0)str_init(&ret_vars, "null");
  //********************return
//  	printf("***return:%i,%i\n",equal_pos,st_func);
  //is function call is alone :) like sum(5)
  if (st_func < 2) {
    return 0;
  }
  if (en_func > -1 && st_func > -1) {
    ret_exp = str_multi_append(str_substring(exp, 0, st_func), ret_vars, str_substring(exp, en_func, 0), 0, 0, 0);
  }
  //msg("&OUT", ret_exp, ret_vars)
  //show_memory(0)
  return ret_exp;
}

//****************************************************
/**
 * get a function arguments and init for entering it then set its parameters and return its return_vars
 * @param pname
 * @param fname
 * @param params
 * @param param_len
 * @param index
 * @return String
 */
String init_calling_function(uint8 func_type,
                             String pname,
                             String fname,
                             str_list params,
                             uint32 param_len,
                             String index) {
  //--------------------init vars
  str_list vars_return = 0;
  uint32 vars_ret_len = 0;
  String ret_vars = 0;
  int8 ret0 = 0;
  //--------------------record all registers
  fust s = {entry_table.cur_fid, entry_table.cur_fin, entry_table.cur_sid, entry_table.cur_order,
      entry_table.parent_fin};
  append_fust(s);
  //--------------------set new parent fin
  entry_table.parent_fin = entry_table.cur_fin;
  //--------------------init function parameters and run function instructions
  if (func_type == FUNC_TYPE_NORMAL)
    ret0 = set_function_parameters(fname, params, param_len);
  else
    ret0 = run_package_module_function(func_type, pname, fname, params, param_len);
//    printf("STOP:%s\n",fname);
  //--------------------analyzing ret0
  Boolean is_return = false;
  if (ret0 == FUNC_RET_BAD) {
    //TODO:
  }
    //if exist a built-in or module or package function
  else if (ret0 == FUNC_RET_BUILTIN || ret0 == FUNC_TYPE_MODULE || ret0 == FUNC_TYPE_PACKAGE) {
    is_return = true;
  } else if (ret0 == FUNC_RET_NORMAL) {

    entry_table.cur_order = 1;
    entry_table.cur_sid = 0;
    APP_CONTROLLER();
    is_return = true;
  }
  //TODO:
  //--------------------if returned values
  if (is_return) {
    long_int ret_po_id = RETURN_TMP_POINTER_ID;
    str_list rets = 0;
    longint_list rets_ind = 0;
    uint32 rets_ind_len = 0;
    uint32 rets_len = char_split(get_Mpoint(ret_po_id).data, ';', &rets, true);
    for (uint32 i = 0; i < rets_len; ++i) {
      longint_list_append(&rets_ind, rets_ind_len++, find_index_var_memory(str_to_long_int(rets[i])));
    }
    edit_Mpoint(ret_po_id, 0, 0, true, false);

    //--------------------analyzing call function index
    if (index != 0) {
      index = simplification_var_index(index);
      int32 real_ind = str_to_int32(index);
      if (real_ind >= rets_len) {
        exception_handler("array_index_overflow", __func__, str_from_int32(real_ind), str_from_int32(
            rets_len - 1));
      } else {
        str_init(&ret_vars, return_value_var_complete(rets_ind[real_ind]));
      }
    } else {
      for (uint32 i = 0; i < rets_len; ++i) {
        String vall = return_value_var_complete(rets_ind[i]);
        // printf ("#@@@@@@@:%s[%i]=>%s\n", rets[i],rets_ind[i], vall);
        ret_vars = str_append(ret_vars, vall);
        if (i + 1 < rets_len)ret_vars = char_append(ret_vars, ',');
      }
    }

  }
  //--------------------get last registers data
  fust lst = get_last_fust();
  //--------------------call garbage_collector(gc)
  if (is_return && lst.fin != entry_table.cur_fin) {
    garbage_collector('f');
  }
  //--------------------delete cole nodes
  for (;;) {
    if (entry_table.cole_end == 0)break;
    if (entry_table.cole_end->fin == entry_table.cur_fin) {
      pop_last_cole();
    } else break;
  }
  //--------------------delete lole nodes
  for (;;) {
    if (entry_table.lole_end == 0)break;
    if (entry_table.lole_end->fin == entry_table.cur_fin) {
      pop_last_lole();
    } else break;
  }
  //--------------------return to parent function
  //print_struct(PRINT_FUNCTIONS_STACK_ST);

  delete_last_fust();
  entry_table.cur_fid = lst.fid;
  entry_table.cur_fin = lst.fin;
  entry_table.cur_sid = lst.sid;
  entry_table.cur_order = lst.order;
  entry_table.parent_fin = lst.parent_fin;
  //show_memory(0)
  //fmt.Scanf("%s")
  return ret_vars;
}
//****************************************************
void garbage_collector(uint8 type) {
  //show_memory (0);
  for (;;) {
    Boolean is_change = false;
    for (long_int i = 0; i < entry_table.var_mem_len; i++) {
      is_change = false;
      Mvar st = get_Mvar(i);
      if (st.name == 0)continue;
      //---------------------delete all current structure variables
      if (type == 'A' && st.func_index == entry_table.cur_fin && st.stru_index == entry_table.cur_sid
          && st.flag != 'l') {
//        printf("####A:%s,%i,%i,%i\n", st.name, st.id, st.stru_index, i);
        delete_full_memory_var(i, true);
        is_change = true;
        break;
      }
      //---------------------delete all current structure variables header
      if (type == 'S' && st.func_index == entry_table.cur_fin && st.stru_index == entry_table.cur_sid
          && st.flag == 'l') {
//        printf("####S:%s,%i,%i,%i\n", st.name, st.id, st.stru_index, i);
        delete_full_memory_var(i, true);
        is_change = true;
        break;
      }
      //---------------------delete all tmp variables that starts with @
      //		  if ((type == '@' ) && st.name != 0 && st.name[0] == '@' && entry_table.cur_fin == st.func_index)
      //			{
      //			  //printf ("##GC[@]:%s\n", st.name);
      //			  delete_full_memory_var (i, true);
      //			  is_change = true;
      //			  break;
      //			}
      //---------------------delete all function variables
      if ((type == 'f') && st.func_index == entry_table.cur_fin) {
        //printf ("##GC[f]:%s\n", st.name);
        delete_full_memory_var(i, true);
        is_change = true;
        break;
      }
    }
    if (!is_change)return;
  }
  //show_memory (0);
}

//****************************************************
Boolean
is_exact_function(str_list func_params,
                  uint32 func_params_len,
                  str_list type_params,
                  uint32 params_len,
                  Boolean is_built_in) {
  //printf ("##FUNCII:%s,%s\n", print_str_list (func_params, func_params_len), print_str_list (type_params, params_len));
  if (func_params_len == 0 && params_len == 0) {
    return true;
  } else if (func_params_len == 0 && params_len != 0) {
    //printf ("0####################\n");
    return false;
  } else {
    //if last parameter of func_params is vars
    Boolean is_last_vars = false;
//    printf("ZZZZZZZZZZZZZZZ:%s\n",func_params[func_params_len - 1]);
    if (func_params_len > 0 && str_indexof(func_params[func_params_len - 1], "vars;", 0) == 0
        || (is_built_in && str_indexof(func_params[func_params_len - 1], "aa..;", 0) == 0))
      is_last_vars = true;
    //if call params is less than func params
    if (params_len < func_params_len && !is_last_vars) {
      //printf ("1####################\n");
      return false;
    }
    //if func params is less than call params
    if (func_params_len < params_len && !is_last_vars) {
      //printf ("2####################\n");
      return false;
    }
    //move on ret_pars array
    for (uint32 i = 0; i < params_len; ++i) {
      Boolean is_a_builtin = false;
      if (func_params_len < i + 1) break;
//      printf ("+STR:%s==%s\n", func_params[i], type_params[i]);
      str_list p1 = 0, p2 = 0;
      char_split(func_params[i], ';', &p1, false);
      char_split(type_params[i], ';', &p2, false);
      //-----check if aa in built-in
      if (is_built_in && str_equal(p1[0], "aa"))continue;
      if (is_built_in && str_equal(p1[0], "aa.."))break;
      else if (is_built_in && str_ch_equal(p1[0], 'a'))is_a_builtin = true;
      //-----check if vars
      if (!is_a_builtin && str_equal(p1[0], "vars")) {
        str_list p3 = 0;
        for (uint32 j = i; j < params_len; ++j) {
          char_split(type_params[j], ';', &p3, false);
          if ((p3[3] != 0 && !str_ch_equal(p3[3], '0')/*if is array*/)
              || str_ch_equal(p3[2], '2')/*if is reference var*/) {
            //printf("3####################:%s\n",type_params[j]);
            return false;
          }
        }
        break;
      }
      //-----check data types
      if (!is_a_builtin && !str_equal(p1[0], p2[0])) {
        //printf("2####################\n");
        return false;
      }
      //-----check dimensions
      //printf("@WWWW:%s,%s\n", p1[2], p2[3]);
      if (!is_equal_arrays_indexes(p1[2], p2[3])) {
        //printf("@##EEE\n");
        return false;
      }
    }
//    printf("FFFFFFFFFFFFFFFF\n");
  }
  //-------- if find function
  //printf("FUNC_PARS:%s,%s\n", func_name, print_str_list(func_params, func_params_len));

  return true;
}
//****************************************************
int8 run_package_module_function(uint8 func_type, String pack_name, String func_name, str_list pars, uint32 pars_len) {
  //-----------------------------init vars
  //-----------------------------determine parameters type
  str_list pars_type = 0;
  uint32 ret_pars_len = determine_type_name_func_parameters(pars, pars_len, &pars_type);
  //-----------------------------if is FUNC_TYPE_MODULE
  if (func_type == FUNC_TYPE_MODULE) {
    str_list returns_module = 0;
    uint32 returns_module_len = call_module_funcs(pack_name, func_name, pars, pars_type, pars_len, &returns_module);
    //if find module function
    if (returns_module_len > 0) {
      String return_module = 0;
      //create a return expression
      str_init(&return_module, "return ");
      for (uint32 i = 0; i < returns_module_len; i++) {
        return_module = str_append(return_module, returns_module[i]);
        if (i + 1 < returns_module_len)return_module = str_append(return_module, ",");
      }
      //----call function_return
      function_return(return_module);
      return FUNC_RET_MODULE;
    }
    //if not a module function
    if (returns_module_len == 0)
      exception_handler("not_exist_mod_func", __func__, func_name, pack_name);
    return FUNC_RET_BAD;
  }
    //-----------------------------if is FUNC_TYPE_PACKAGE
  else if (func_type == FUNC_TYPE_PACKAGE) {
    //TODO:not completed
  }

  return FUNC_RET_BAD;
}
//****************************************************
int8 set_function_parameters(String func_name, str_list pars, uint32 pars_len) {
  /**
  func_params types:
  1- normal: string gh,gh1,digit f1
  2- with array limit : string gh[?],num jk[3,?]
  3- with unlimited parameters: digit b,vars pars
  call_params types:
  1- values: "reza",67
  2- variables: gp[4,8],bn
  2- refrence variables: &hel,&tt
  3- expanded arrays : {5,9.6,-6},{struct(9,"78")}
  4- expanded structs : struct(0,"fsdg",true)
  */
//  printf("SDDDDDDD:%s,%i\n", print_str_list(pars, pars_len), pars_len);
  //-----------------------------init vars
  Boolean is_user_func = false;
  str_list func_params = 0;
  uint32 func_params_len = 0;
  long_int next_fin = 0;
  //-----------------------------determine parameters type
  str_list ret_pars = 0;
  uint32 ret_pars_len = determine_type_name_func_parameters(pars, pars_len, &ret_pars);
//  printf("######PARS:%s\n%s\n", print_str_list(ret_pars, ret_pars_len), print_str_list(pars, pars_len));
  //-----------------------------search for function
  blst *tmp1;
  tmp1 = entry_table.blst_func_start;
  //------------------
  for (;;) {
    if (str_equal(tmp1->lbl, func_name)) {
//       printf("##FUNC:%s;%s;%s\n", tmp1->lbl, print_str_list(tmp1->params, tmp1->params_len), print_str_list(ret_pars, ret_pars_len));
      //-------- if find function
      if (is_exact_function(tmp1->params, tmp1->params_len, ret_pars, ret_pars_len, false)) {
//        printf("FUNC_PARS:%s,%s\n", func_name, print_str_list(tmp1->params, tmp1->params_len));
        entry_table.cur_fid = tmp1->id;
        func_params = tmp1->params;
        func_params_len = tmp1->params_len;
        is_user_func = true;
        break;
      }
    }
    ENDLOOP:
    tmp1 = tmp1->next;
    if (tmp1 == 0) break;
  }
  //-----------------------------if is_user_func
  if (is_user_func) {
    next_fin = ++entry_table.func_index;
//    entry_table.cur_fin = ++entry_table.func_index;
  }
    //-----------------------------maybe is a built-in function or not valid a function
  else {
    str_list returns_builtin = 0;
    uint32 returns_builtin_len = call_built_in_funcs(func_name, pars, ret_pars, pars_len, &returns_builtin);
    //if find built-in function
    if (returns_builtin_len > 0) {
      String return_builtin = 0;
      //create a return expression
      str_init(&return_builtin, "return ");
      for (uint32 i = 0; i < returns_builtin_len; i++) {
        return_builtin = str_append(return_builtin, returns_builtin[i]);
        if (i + 1 < returns_builtin_len)return_builtin = str_append(return_builtin, ",");
      }
      //----go to next_fin
      entry_table.cur_fin = next_fin;
      //printf("Built-in return:%s\n",return_builtin);
      //call function_return
      function_return(return_builtin);
      return FUNC_RET_BUILTIN;
    }
    //if not a built-in function
    exception_handler("not_exist_func", __func__, func_name, print_str_list(pars, pars_len));
    return FUNC_RET_BAD;
  }
  //-----------------------------init & allocate function params
  Boolean is_vars = false;
  String vars_name = 0;
  String vars_po_ids = 0;
  for (uint32 i = 0; i < func_params_len; ++i) {
    str_list p1 = 0, p2 = 0;
    uint32 p1_len = 0;
    char_split(func_params[i], ';', &p2, false);
    if (i < ret_pars_len) {
      p1_len = char_split(ret_pars[i], ';', &p1, false);
      //printf("#STR:%s==%s&&%s (%i,%i)\n", ret_pars[i], func_params[i], pars[i], p1_len, func_params_len);
      if (p1_len == 0)break;
    }
    //====is vars
    if (!is_vars && str_equal(p2[0], "vars")) {
      is_vars = true;
      str_init(&vars_name, p2[1]);
      //if called function parameters has more than one vars values
      if (ret_pars_len > func_params_len) {
        for (uint32 j = 0; j < func_params_len - ret_pars_len; ++j)
          str_list_append(&func_params, "tmp;;0", func_params_len++);
      }
        //if called function parameters has no any vars values
      else if (ret_pars_len < func_params_len) {
        String po_id = str_from_long_int(add_to_pointer_memory("null", '0'));
        str_init(&vars_po_ids, po_id);
        break;
      }
    }
    //====define var by value
    if (str_ch_equal(p1[2], '0')) {
      if (is_vars) {
        String val = 0;
        uint8 sub = '0';
        calculate_value_of_var(pars[i], p1[0], &val, &sub);
        String po_id = str_from_long_int(add_to_pointer_memory(val, sub));
        if (vars_po_ids != 0)vars_po_ids = char_append(vars_po_ids, ';');
        vars_po_ids = str_append(vars_po_ids, po_id);
      } else {
        String namei = 0;
        if (p1[3] == 0 || str_ch_equal(p1[3], '0'))str_init(&namei, p2[1]);
        else namei = str_multi_append(p2[1], "[", p1[3], "]", 0, 0);
        set_memory_var(next_fin, 0, namei, pars[i], p2[0], true);
      }
    }
      //====define var by var
    else if (str_ch_equal(p1[2], '1')) {
      if (is_vars) {
        Mpoint v = get_Mpoint(get_data_memory_id(get_Mvar(str_to_long_int(p1[1])).pointer_id, "0"));
        String po_id = str_from_long_int(add_to_pointer_memory(v.data, v.type_data));
        if (vars_po_ids != 0)vars_po_ids = char_append(vars_po_ids, ';');
        vars_po_ids = str_append(vars_po_ids, po_id);
      } else {
        copy_memory_var(str_to_long_int(p1[1]), p2[1], next_fin);
      }
    }
      //====define var by reference var
    else if (str_ch_equal(p1[2], '2')) {
      Mvar ref_var = get_Mvar(str_to_long_int(p1[1]));
      add_to_var_memory(ref_var.pointer_id, next_fin, 0, ref_var.type_var, p2[1], 0);
    }
  }

  if (is_vars) {
    long_int main_pointer_id = add_to_pointer_memory(vars_po_ids, 'p');
    add_to_var_memory(main_pointer_id, next_fin, 0, search_datas("vars", 0, true).id, vars_name, 0);
    //printf("$RRRRR:%i,%i,%i,%s\n", main_pointer_id, entry_table.cur_fin,search_datas("vars", 0, true).id,vars_name);
  }
  //-----------------------------go to next_fin
  entry_table.cur_fin = next_fin;
  //show_memory(0);
  return FUNC_RET_NORMAL;
}
//****************************************************

//****************************************************
uint32 determine_type_name_func_parameters(str_list params, uint32 params_len, str_list *ret) {
  //printf("$$$$TP:%s,%i\n", print_str_list(params, params_len), params_len);
  //-----------init vars
  uint32 ret_len = 0;
  //ret[i]="type;var_index;state;var_dimensions" state: 0=value,1=var,2=reference
  for (uint32 i = 0; i < params_len; ++i) {
    uint32 param_len = str_length(params[i]);
    //printf("@@@PAR:%s,%i\n", params[i],is_valid_name(params[i], true));
    //====if is a variable or reference variable
    if (is_valid_name(params[i], true) ||
        (param_len > 1 && params[i][0] == '&' && is_valid_name(str_substring(params[i], 1, 0), false))) {
      uint8 state = '1';
      String par_var = 0;
      if (params[i][0] == '&') {
        str_init(&par_var, str_substring(params[i], 1, 0));
        state = '2';
      } else str_init(&par_var, params[i]);
      String state_s = str_multi_append(";", char_to_str(state), ";", 0, 0, 0);
      String name = 0, index = 0;
      return_name_index_var(par_var, true, &name, &index);
      long_int var_id = return_var_id(name, "0");
      if (var_id > 0) {
        long_int real_id = find_index_var_memory(var_id);
        Mvar var = get_Mvar(real_id);
        String type_name = 0;
        str_init(&type_name, get_datas(var.type_var).name);
        //if is complete var
        if (index == 0) {
          str_list tmp1;
          uint32 tmp1_len = return_array_dimensions(var.pointer_id, &tmp1);
          String var_dim = char_join(tmp1, ',', tmp1_len, true);
          //printf("^^^FF:%s=>%i,%s\n",params[i],tmp1_len,tmp1[0]);
          if (str_ch_equal(var_dim, '1') || str_ch_equal(var_dim, '0'))var_dim = 0;
          str_list_append(&(*ret),
                          str_multi_append(type_name, ";", str_from_long_int(real_id), state_s, var_dim, 0),
                          ret_len++);
        }
          //if is a room of var
        else {
          str_list_append(&(*ret),
                          str_multi_append(type_name, ";", str_from_long_int(real_id), state_s, index, 0),
                          ret_len++);
        }
        continue;
      }
    }
    //====if is a value
    String value_type = 0, var_dim = 0, value = 0;
    //get var dimensions
    int32 indexes[MAX_ARRAY_DIMENSIONS];
    uint8 count = return_size_value_dimensions(params[i], indexes, &value);
    for (uint8 j = 0; j < count; ++j) {
      var_dim = str_append(var_dim, str_from_int32(indexes[j]));
      if (j + 1 < count)var_dim = char_append(var_dim, ',');
    }
    //determine value type
    value_type = determine_value_type(value);
    //if not array
    if (str_ch_equal(var_dim, '1') && !has_two_limiting(params[i], '{', '}', true)) str_init(&var_dim, "0");
    //append to ret
//    printf("#VAL:%s=>%s,%s,%s\n", params[i],var_dim, value,value_type);
    str_list_append(&(*ret), str_multi_append(value_type, ";0;0;", var_dim, 0, 0, 0), ret_len++);
  }

  return ret_len;
}

//****************************************************
Boolean function_return(String exp) {
  /**
  1- return age,df[0,1] ---OK---
  2- return ("G"*3)+"hj",56+24 ---OK---
  Types of return parameters:
  1- variable : age
  2- value : df[1,0] || "hello" || 45+3
  3- array : {{56,9},{7.9,90}}
  4- struct : struct(56,"tt",true,struct(45,false))
  */
  //****************init vars
  Boolean is_string = false;
  String word = 0, return_ids = 0;
  int32 pars = 0, bras = 0, acos = 0, tmp_names_count = 0;
  str_list return_vals = 0, return_types = 0;;
  uint32 return_vals_len = 0, return_types_len = 0;
  uint32 len = str_length(exp);
  //****************analyzing
  for (uint32 i = 0; i < len; i++) {
    //------------------check is string
    if (exp[i] == '"' && (i == 0 || exp[i - 1] != '\\')) {
      is_string = switch_bool(is_string);
    }
    //------------------count pars
    if (!is_string) {
      if (exp[i] == '(')pars++;
      else if (exp[i] == ')')pars--;
      else if (exp[i] == '{')acos++;
      else if (exp[i] == '}')acos--;
      else if (exp[i] == '[')bras++;
      else if (exp[i] == ']')bras--;
    }
    //------------------find return keyword
    if (!is_string && (exp[i] == ' ' || (exp[i] == '(' && pars == 1)) && str_equal(word, "return")) {
      str_empty(&word);
      if (exp[i] != '(')continue;

    }
    //------------------append to word
    if (!is_string && ((exp[i] == ',' && bras == 0 && pars == 0 && acos == 0) || i + 1 == len)) {
      if (i + 1 == len) {
        word = char_append(word, exp[i]);
      }
      str_list_append(&return_vals, str_trim_space(word), return_vals_len++);
      str_empty(&word);
    } else word = char_append(word, exp[i]);
  }
  //msg("&XXX:", return_vals)
  //****************init return parameters
  return_types_len = determine_type_name_func_parameters(return_vals, return_vals_len, &return_types);
  //msg("&YYYY:", return_params)
  //****************allocate parameters together
  for (uint32 i = 0; i < return_types_len; i++) {
    //msg("&RET:", return_params[i], last_fin, cur_fin)
    str_list p1 = 0;
    char_split(return_types[i], ';', &p1, false);
    tmp_names_count++;
    String tmp2 = generate_return_var_name(0, &tmp_names_count);
    //printf("$WQQQQ:%s,%s=>%s\n",return_vals[i],return_types[i],tmp2);
    if (str_equal(p1[0], "str") && str_ch_equal(p1[2], '0')/*is value*/ && str_length(return_vals[i]) > 0
        && return_vals[i][0] != '\"') {
      return_vals[i] = str_multi_append("\"", return_vals[i], "\"", 0, 0, 0);
    }
    //---------------define var by value
    if (str_ch_equal(p1[2], '0')) {
      //msg("RET_MMM:", tmp2, tmp1[1], tmp1[0])
      long_int var_id = set_memory_var(entry_table.cur_fin, 0, tmp2, return_vals[i], p1[0], true);
      if (return_ids != 0)return_ids = char_append(return_ids, ';');
      return_ids = str_append(return_ids, str_from_long_int(var_id));
    }
      //---------------define var by variable
    else if (str_ch_equal(p1[2], '1')) {
      long_int var_id = copy_memory_var(str_to_long_int(p1[1]), tmp2, entry_table.cur_fin);
      if (return_ids != 0)return_ids = char_append(return_ids, ';');
      return_ids = str_append(return_ids, str_from_long_int(var_id));
    }
  }
  //****************set in RETURN_TMP_POINTER_ID
  edit_Mpoint(RETURN_TMP_POINTER_ID, return_ids, 0, true, false);
  //****************return
  //show_memory(40)
  return true;
}

//****************************************************
Boolean vars_allocation(String exp) {
  /**
	* value operators:
	* = , += , -= , *= , /= , %= , ^= , :=
    * ---- bool operators: =,:=
    * ---- str operators: =,+=,:=
    * ---- num operators: =, += , -= , *= , /= , %= , ^= , :=
	* ---- array operators: =,:=
    * ---- struct operators: =,:=
    *
    * ******* =
    * 1- st1=st2 //structs
    * 2- st1.gg=gg1 //structs
    * 3- st1=struct(45,"gh") //structs
    * 4- n1=n2 //arrays
    * 5- n1={{5,8},{6,9}} //arrays
    * 6- st1.n=n2 //arrays,structs
    * 7- s1="gh" //values
    * 8- s1=s2[0] //values
    *
	* 1- nu1[0]+=45 || st1*=2 ---..---
	* 2- st1[0,1],st1="Amin"*3,"Reza" ---..---
	* 3- st1[0,8]=st2[9,4] ---..---
	* 4- st1[0],nu2+="Hi",(45^f[0]) ---..---
	* 5- st1="Am";st2="Re"; st1:=st2 => st1="Re";st2="Am"; ---..---
	* 6- st1=st2 //st1={4,7,9},st2={5,8,9,0} ---..---
	* 7- st1:=st2 //st1=struct(67,true),st2=struct(0.7567,false) ---..---
	* 8- st1={5,9,0} //st1={1,2} => st1={5,9,0} ---..---
    * 9- st1=st2 //st1=struct(67,true),st2=struct(0.7567,false) ---..---
	* Samples:
	* 1- st1,d1[0,0],d1[0,1]+=" Jone",(60*10),d1[1,0] ---..---
	* 2- st1:=st2[0] ---...---
	* 3- st1,st2[0]="HELLO" ---..---
	*/
  //--------------init vars
  uint32 len = str_length(exp);
  int8 status = 0;
  String word = 0, equal_type = 0;
  Boolean is_string = false, is_equal = false, is_empty = false, is_struct = false;
  int32 vars_counter = 0, vals_counter = 0, pars = 0, bras = 0, acos = 0;
  struct alloc_var_struct alloc_var[MAX_VAR_ALLOC_INSTRUCTIONS];
  String public_ops[6] = {"+=", "-=", "*=", "/=", "%=", "^="};
  //printf("ALLOC_EXP:%s\n",exp);
  //***********************************parsing alloc expression
  for (uint32 i = 0; i < len; i++) {
    is_empty = false;
    //----check is string
    if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\'))
      is_string = switch_bool(is_string);
    //----check is struct
    if (pars == 0 && exp[i] == '(' && str_equal(word, "struct"))
      is_struct = true;
    else if (pars == 1 && exp[i] == ')')
      is_struct = false;
    //----continue if ' '
    if (!is_string && i + 1 < len && exp[i] == ' ' && (exp[i + 1] == '(' || exp[i + 1] == '['))continue;
    //----count bras,pars,acos
    if (!is_string) {
      if (exp[i] == '(')pars++;
      else if (exp[i] == ')')pars--;
      else if (exp[i] == '{')acos++;
      else if (exp[i] == '}')acos--;
      else if (exp[i] == '[')bras++;
      else if (exp[i] == ']')bras--;
      else if (exp[i] == '=')is_equal = true;
    }
    //----is a allocation operator
    String temp1 = 0;
    Boolean is_alloc_op = false;
    if (!is_string && i + 1 < len) {
      temp1 = str_substring(exp, i, i + 2);
      if (str_search(alloc_operators, temp1, StrArraySize (alloc_operators)))is_alloc_op = true;
    }
    if (!is_string && temp1 != 0 && (is_alloc_op || exp[i + 1] == '=')) {
      is_equal = true;
      if (is_alloc_op)
        str_init(&equal_type, temp1);
      else {
        str_init(&equal_type, "=");
        word = char_append(word, exp[i]);
      }
      //printf("QQQQ:%s,%s\n",equal_type,word);
      is_empty = true;
    }
    //----store a variable
    if (!is_string && pars == 0 && bras == 0 && word != 0 && ((exp[i] == ',' && !is_equal) || (is_empty && is_equal))
        ) {
      //------finding name and index of var
      word = str_trim_space(word);
      String name = 0, index = 0;
      return_name_index_var(word, true, &name, &index);
      index = simplification_var_index(index);
      vars_counter++;
      String type_name = get_datas(get_Mvar(find_index_var_memory(return_var_id(name, index))).type_var).name;
      //msg("&GGGG:", return_var_id(name, index), name, index)
      str_init(&alloc_var[vars_counter - 1].type, type_name);
      str_init(&alloc_var[vars_counter - 1].name, name);
      str_init(&alloc_var[vars_counter - 1].index, index);
      alloc_var[vars_counter - 1].alloc = 0;
      //printf ("@@@:%i,%s,%s\n", return_var_id (name, index), name, index);
      word = 0;
      if (is_empty)i++;
      continue;
    }
    //----allocate values to variables
    if (is_equal && !is_string && !is_struct && bras == 0 && acos == 0 && (word != 0 || i + 1 == len)
        && ((exp[i] == ',' && pars == 0) || (i + 1 == len && pars < 2)) && vals_counter < vars_counter) {
      if (i + 1 == len) word = char_append(word, exp[i]);
      //msg("UUU:", word)
      vals_counter++;
      str_init(&alloc_var[vals_counter - 1].alloc, word);
      word = 0;
      continue;
    }
    //----append to word
    if (is_empty) word = 0;
    else word = char_append(word, exp[i]);
  }
  //--------------alloc one value to many variables
  if (vals_counter == 1 && vars_counter > 1) {
    for (uint32 i = 1; i < vars_counter; i++) {
      if (!str_equal(alloc_var[0].type, alloc_var[i].type))
        continue;
      str_init(&alloc_var[i].alloc, alloc_var[0].alloc);
    }
  }
  //show alloc_var array
  //  printf ("$$$QQQQ:%s[%i,%i]:\n", equal_type, vars_counter, vals_counter);
  //  for (int j = 0; j < vars_counter; ++j)
  //	{
  //	  printf ("%i=>%s %s[%s]= %s\n", j, alloc_var[j]
  //		  .type, alloc_var[j].name, alloc_var[j].index, alloc_var[j].alloc);
  //	}
  //***********************************allocation vars with new values
  for (uint32 i = 0; i < vars_counter; i++) {
    struct alloc_var_struct st = alloc_var[i];
    //init1
    String origin_type = 0; //=>type of original variable like num,st1
    uint8 origin_sub_type = 0; //=>sub type of original value(just main sub types)
    long_int origin_pointer_id = 0; //pointer id of original var with its index
    Boolean is_origin_array = false; //=>is original var in its index is an array or no!
    Boolean is_origin_struct = false; //=>is original var in its index is a struct or no!
    //-----------------------------
    //init2
    long_int var_id = return_var_id(st.name, 0);
    long_int Tid = find_index_var_memory(var_id); //=>get index of original variable
    Mvar origin_var = get_Mvar(Tid); //=>get Mvar of original variable
    Mpoint origin_po_val; //=>get Mpoint of original variable in its index
    str_init(&origin_type, get_datas(origin_var.type_var).name); //=>set origin_type
    //-----------------------------
    //=>get original var pointer id in its index and get Mpoint from it
    origin_pointer_id = get_data_memory_id(origin_var.pointer_id, st.index);
    origin_po_val = get_Mpoint(origin_pointer_id);
    //=>if not index and just exist one pointer, then select it
    if (st.index == 0 && char_last_indexof(origin_po_val.data, ';') == -1) {
      origin_pointer_id = get_data_memory_id(origin_var.pointer_id, "0");
      origin_po_val = get_Mpoint(origin_pointer_id);
    }
//    printf("@WWW:%s=>%i,%s,%i\n",st.name,origin_var.pointer_id, st.index,origin_pointer_id);
    //=>check if exist this var with its index
    if (origin_po_val.id == 0) {
      //TODO:error
      printf("#ERR1345889\n");
      return false;
    }
    //=>check if original var with its index is array or no!
    if (origin_po_val.type_data == 'p')is_origin_array = true;
      //=>check if original var with its index is struct or no!
    else if (origin_po_val.type_data == 'l')is_origin_struct = true;
    //=>so original var with its index now is an atomic value!
    origin_sub_type = origin_po_val.type_data;
    //TODO:errors,warnings
    //init3
    String final_res = 0;
    //.......................................operator '='
    if (str_ch_equal(equal_type, '=')) {
      //init vars
      str_list tmp = 0;
      uint32 tmp_len = 0;
      Boolean valid_var = false;
      //------------------------------------
      //=>get original var indexes like '2,4'
      tmp_len = return_array_dimensions(origin_var.pointer_id, &tmp);
      String origin_indexes = char_join(tmp, ',', tmp_len, true);
//      printf("WWWWWWWW:%s;%s=>%s,%i(%i,%i)\n",
//             st.name,
//             st.alloc,
//             origin_val,
//             origin_po_val.id,
//             is_origin_array,
//             is_origin_struct);
      //-----------------------------------------------
      //=>(Type1-array)if original var with its index is an array
      if (is_origin_array) {
        //=>(Type1.1-var)if array alloc is a var
        if ((valid_var = is_valid_name(st.alloc, true)) || is_valid_struct_entry(st.alloc)) {
          if (!vars_assign_allocation(valid_var, true, st.alloc, origin_indexes, origin_pointer_id))return false;
        }
          //=>(Type1.2-value)if array alloc is a var
        else {
          if (!alloc_array_var(Tid, st.alloc, origin_type)) return false;
        }
      }
        //-----------------------------------------------
        //=>(Type2-struct)else if array alloc is a struct
      else if (is_origin_struct) {
        //=>(Type1.1-var)if struct alloc is a var
        if ((valid_var = is_valid_name(st.alloc, true)) || is_valid_struct_entry(st.alloc)) {
//          printf("EEEEE:%s\n", st.alloc);
          if (!vars_assign_allocation(valid_var, false, st.alloc, 0, origin_pointer_id))return false;
        }
          //=>(Type1.2-value)if struct alloc is a var
        else {
//          printf("EEEEE:%s\n", st.alloc);
          String struct_id = calculate_struct_expression(st.alloc, origin_type, &origin_sub_type);
          if (origin_sub_type == '0' || !alloc_struct_var(
              search_datas(origin_type, 0, true),
              origin_po_val.id,
              get_stde(str_to_long_int(struct_id)).st))
            return false;
        }
      }
        //-----------------------------------------------
        //=>(Type3-value)else if array alloc is an atomic data
      else {
//          printf("EEEEE2:%s\n", st.alloc);
          String ret_val = 0;
          uint8 ret_subtype = 0;
          //=>calculate alloc value like var or struct entry or atomic data!
          calculate_value_of_var(st.alloc, origin_type, &ret_val, &ret_subtype);
          ret_val = str_reomve_quotations(ret_val, origin_type);
          //=>check if sub types of original var and alloc data is equal
          if (origin_sub_type != ret_subtype) {
            //TODO:error
            printf("R#ERRR3456754g89\n");
            return false;
          }
          edit_Mpoint(origin_pointer_id, ret_val, 0, true, false);
      }
    }
      //.......................................operator ':='
    else if (str_equal(equal_type, ":=")) {
      if (!vars_swap_allocation(origin_var, st.alloc, origin_pointer_id)) return false;
    }
      //.......................................operator +=,-=,*=,/=,%=,^=
    else if (origin_sub_type != 0 && str_search(public_ops, equal_type, 6)) {
      uint8 rettype = 0;
      if (str_equal(origin_type, "num")) {
        if (origin_sub_type == 0)origin_sub_type = '_';
        calculate_math_expression(str_multi_append(origin_val, char_to_str(equal_type[0]), st
            .alloc, 0, 0, 0), origin_sub_type, &final_res, &rettype);
      } else if (equal_type[0] == '+' && str_equal(origin_type, "str")) {
        calculate_string_expression(str_multi_append(origin_val, "+", st
            .alloc, 0, 0, 0), &final_res, &rettype);
        final_res = str_reomve_quotations(final_res, "s");
      }
      //printf ("@##DDDD:%i-%s(%c):%s,%s=>%s\n", value_pointer_ind, origin_type, origin_sub_type, origin_val, st.alloc, final_res);
      if (rettype == '_' || rettype == '0' || rettype == 0) {
        //TODO:error
        printf("#ERRR3456789\n");
        return false;
      }
      edit_Mpoint(origin_pointer_id, final_res, rettype, true, false);
    }
  }
  //--------------return
  return true;
}
//****************************************************
Boolean vars_assign_allocation(Boolean valid_var,
                               Boolean is_array,
                               String alloc,
                               String origin_indexes,
                               long_int origin_pointer_id) {
  //init vars
  long_int alloc_pointer_id = 0;
  str_list tmp = 0;
  //---------------------------
  //=>if is a var name
  if (valid_var) {
    //=>split alloc var name and index
    String al_name = 0, al_index = 0;
    return_name_index_var(alloc, true, &al_name, &al_index);
    //=>get alloc var index and then get Mvar of alloc var
    long_int alloc_ind = find_index_var_memory(return_var_id(al_name, 0));
    Mvar alloc_var = get_Mvar(alloc_ind);
    //=>get alloc pointer id in its index
    alloc_pointer_id = get_data_memory_id(alloc_var.pointer_id, al_index);
    //=>if not index and just exist one pointer, then select it
    if (al_index == 0 && char_last_indexof(get_Mpoint(alloc_pointer_id).data, ';') == -1)
      alloc_pointer_id = get_data_memory_id(alloc_var.pointer_id, "0");
  }
    //=>else if is a struct entry var
  else {
    alloc_pointer_id = return_struct_entry_pointer_id(alloc);
  }
  //=>check if exist alloc var
  if (alloc_pointer_id == 0) {
    //TODO:error
    printf("#ERR1345845:%s\n", alloc);
    return false;
  }
  if (is_array) {
    //=>check if indexes of origin and alloc is same
    uint32 tmp_len = return_array_dimensions(alloc_pointer_id, &tmp);
    String alloc_indexes = char_join(tmp, ',', tmp_len, true);
    if (!is_equal_arrays_indexes(origin_indexes, alloc_indexes)) {
      //TODO:error
      printf("R#ERR34567890:%s;%s(%i)%s\n", origin_indexes, alloc_indexes);
      return false;
    }
  }
  if (!recursive_alloc_vars_pointers_data(origin_pointer_id, alloc_pointer_id))
    return false;
  //---------------------------
  return true;
}
//****************************************************
Boolean vars_swap_allocation(Mvar origin, String alloc, long_int origin_po_val) {
  //init vars
  String al_name = 0, al_index = 0;
  long_int alloc_id = 0;
  Mpoint alloc_point, origin_point;
  Mvar alloc_var;
  //------------------------------
  //=>get var_index of original var
  long_int origin_index = find_index_var_memory(origin.id);
  //=>split alloc var name and index
  Boolean not_index = false, is_alloc_array = false;
  return_name_index_var(alloc, true, &al_name, &al_index);
  //=>check if alloc var not have index
  if (al_index == 0) {
    not_index = true;
    str_init(&al_index, "0");
  }
  //=>get alloc_id from alloc var name
  alloc_id = find_index_var_memory(return_var_id(al_name, 0));
  //=>check if exist alloc var
  if (alloc_id == 0) {
    //TODO:error
    printf("#ERR134582\n");
    return -1;
  }
  //=>get Mvar of alloc var by alloc_id
  alloc_var = get_Mvar(alloc_id);
  //=>get Mpoint of alloc var with its index
  alloc_point = get_Mpoint(get_data_memory_id(alloc_var.pointer_id, al_index));
  //=>get Mpoint of original var with its index
  origin_point = get_Mpoint(origin_po_val);
  //=>check if original var type is equal alloc var type
  if (alloc_var.type_var != origin.type_var) {
    //TODO:error
    printf("R#ERR134567\n");
    return false;
  }
  //=>if alloc point sub type is equal with original point sub type and is not one of main sub types so swap just pointers!
  if (alloc_point.type_data == origin_point.type_data
      && (alloc_point.type_data == 'p' || alloc_point.type_data == 'l')) {
    //=>change original pointer data in its index
    edit_Mpoint(origin_po_val, alloc_point.data, 0, true, false);
    //=>change alloc pointer data in its index
    edit_Mpoint(alloc_point.id, origin_point.data, 0, true, false);
  }
    //=>else if alloc var and origin var have atomic data
  else {
    //=>check if original data type is equal with alloc data type
    if (!is_equal_data_types(origin_point.type_data, alloc_point.type_data)) {
      //TODO:error
      printf("R#ERR13456847\n");
      return false;
    }
    //=>change original var data in its index
    edit_Mpoint(origin_po_val, alloc_point.data, alloc_point.type_data, true, true);
    //=>change alloc var data in its index
    edit_Mpoint(alloc_point.id, origin_point.data, origin_point.type_data, true, true);
  }
  return true;
}
//****************************************************
String vars_allocation_short(String exp) {
  /**
   * n[w++]=3 //=>n[w]=3,w++
   * g=++n[2] //=>n[2]++,g=n[2]
   */
  //-----------------init vars
  Boolean is_string = false, is_plusplus = true, is_right = true;
  uint32 start = 0, end = 0;
  String var_name = 0, result = 0;
  uint32 len = str_length(exp);
  //-----------------start analyzing
  for (uint32 i = 0; i < len; i++) {
    //------------------check is string
    if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\'))
      is_string = switch_bool(is_string);
    //------------------find alloc short
    if (!is_string && i + 1 < len && exp[i] == exp[i + 1] && (exp[i] == '+' || exp[i] == '-')) {
      if (exp[i] == '-')is_plusplus = false;

      uint32 point = 0;
      if (i > 0/*var++*/) {
        var_name = find_first_var_name(exp, i - 1, true, &point);
        if (var_name != 0)is_right = false;
        start = (point == 0) ? 0 : point + 1;
        end = i + 2;
      }
      if (var_name == 0 && i + 2 < len/*++var*/) {
        var_name = find_first_var_name(exp, i + 2, false, &point);
        start = i;
        end = point;
      }

      break;
    }
  }
  //-----------------short allocation
  if (is_right/*++var*/) {
    do_show_allocation(var_name, is_plusplus);
  } else/*var++*/{
    str_list_append(&entry_table.post_short_alloc,
                    str_multi_append(var_name, ";", str_from_bool(is_plusplus), 0, 0, 0),
                    entry_table.post_short_alloc_len++);
  }
  str_init(&result, var_name);
  //-----------------return exp
  String tmp1 = str_substring(exp, 0, start);
  String tmp2 = str_substring(exp, end, 0);
  if (tmp1 == 0 && tmp2 == 0) return 0;
  exp = str_multi_append(tmp1, result, tmp2, 0, 0, 0);
//  printf("***alloc_short***:%s;%s,%i(%i,%i)[%s,%s]\n", exp, var_name, is_plusplus, start, end, tmp1, tmp2);
  return exp;
}
//****************************************************
Boolean check_post_short_alloc() {
  if (entry_table.post_short_alloc_len > 0) {
    for (uint32 i = 0; i < entry_table.post_short_alloc_len; i++) {
      str_list tokens = 0;
      //printf("ER#%s\n",entry_table.post_short_alloc[i]);
      uint32 cc = char_split(entry_table.post_short_alloc[i], ';', &tokens, true);
      if (cc < 2) {
        //TODO:error
        printf("#ERR34567\n");
        entry_table.post_short_alloc_len = 0;
        return false;
      }
      Boolean ret = do_show_allocation(tokens[0], str_to_bool(tokens[1]));
      if (!ret)return false;
    }
    entry_table.post_short_alloc_len = 0;
  }
  return true;
}
//****************************************************
Boolean do_show_allocation(String var_name, Boolean is_plusplus) {
  //init
  Mpoint val = return_var_memory_value(var_name);
  String result = 0;
  //determine result
  if (val.type_data == 's'/*is string*/) {
    if (is_plusplus) result = str_append(val.data, " ");
    else result = str_substring(val.data, 0, str_length(val.data) - 1);
  } else if (is_equal_data_types(val.type_data, 'i')/*is num*/) {
    uint8 ret1 = 0;
    if (is_plusplus)
      calculate_math_expression(str_multi_append(val.data, "+", "1", 0, 0, 0),
                                val.type_data,
                                &result,
                                &ret1);
    else calculate_math_expression(str_multi_append(val.data, "-", "1", 0, 0, 0), val.type_data, &result, &ret1);
  }
  //set result
//  printf("++--:%s=>((%s))%s;\n",var_name,val.data,result);
  edit_Mpoint(val.id, result, 0, true, false);
  return true;
}

//****************************************************
Boolean init_structures(String exp) {
/**
	1- manage(e) ---OK---
	2- if(ty==45) ---OK---
	3- loop(num i,j=0;ty.IsExist();i+=j)
	*/
  blst *rec = entry_table.blst_stru_start;
  for (;;) {
    //printf("@@@@:%s,%s\n",exp,rec->lbl);
    if (str_equal(rec->lbl, exp)) {
      if (rec->type == MANAGE_STRU_ID) {
        return structure_MANAGE(rec->id, rec->params[0]);
      } else if (rec->type == IF_STRU_ID
          || rec->type == ELIF_STRU_ID
          || rec->type == ELSE_STRU_ID) {
        //print_struct(PRINT_STRU_ST);
        return structure_CONDITION(rec->id, rec->type, rec->params[0]);
      } else if (rec->type == LOOP_STRU_ID) {
        return structure_LOOP(rec->id, rec->type, rec->params);
      }
      return true;
    }
    rec = rec->next;
    if (rec == 0) break;
  }

  return false;
}
//****************************************************
Boolean structure_MANAGE(long_int st_id, String value) {
/**
 * manage(e)
 * manage()
 */
  //--------------------determine value
  value = str_trim_space(value);
  //printf("###manage:%s\n", value);
  if (str_length(value) == 0)str_init(&value, "null");
  else {
    long_int id = return_var_id(value, 0);
    if (id == 0) {
      //TODO:error
      return false;
    }
    value = str_from_long_int(id);
  }
  //--------------------record all registers
  stst
      s = {MANAGE_STRU_ID, entry_table.cur_fid, entry_table.cur_fin, st_id, entry_table.cur_sid, entry_table.cur_order,
      value};
  append_stst(s);

//--------------------set new registers
  entry_table.cur_order = 1;
  entry_table.cur_sid = st_id;
  //--------------------start point
  int8 ret1 = APP_CONTROLLER();
  /*if return_fin > 0 && return_fin == cur_fin {
      is_stop_APP_CONTROLLER = true
  }*/
//  if (ret1 == 2) {
//
//  }
  //****************end of manage
  //--------------------call garbage_collector(gc)
  garbage_collector('A');
  //TODO
  //--------------------return to parent structure
  //print_struct(PRINT_FUNCTIONS_STACK_ST);
  stst lst = get_last_stst();
  entry_table.cur_sid = lst.parent_sid;
  entry_table.cur_order = lst.order;
  delete_last_stst();
  return true;
}
//****************************************************
Boolean structure_CONDITION(long_int st_id, uint8 type, String value) {
/**
	1- if(ex.is_Ok)
	2- elif (4==fg) ---OK---
	3- else ---OK---
	-------Samples:
	if (pk1.xid()>=10000)print("NUM1\n")
    elif(pk1.xid()>=1000)print("NUM2\n")
	elif(pk1.xid()>=100)print("NUM3\n")
	else print("NUM4\n")
	*/
//  printf("##condition:id:%i,type:%i,fin:%i,sid:%i,/%s/,%s(%i)\n",
//         st_id,
//         type,
//         entry_table.cur_fin,
//         entry_table.cur_sid,
//         value,
//         str_from_bool(get_cole_by_id(entry_table.cole_len).is_complete),
//         entry_table.cole_len);
  //----------------check for elif , else that have if
  if ((type == ELSE_STRU_ID || type == ELIF_STRU_ID)) {
    Boolean failed = false;
    if (entry_table.cur_order == 1)failed = true;
    else {
      uint8 tt = search_lbl_stru(get_instru_by_params(entry_table.cur_fid,
                                                      entry_table.cur_sid,
                                                      (uint32) entry_table.cur_order - 1).code).type;
      if (type == ELIF_STRU_ID && tt != IF_STRU_ID)failed = true;
      else if (type == ELSE_STRU_ID && tt != IF_STRU_ID && tt != ELIF_STRU_ID)failed = true;
    }
    if (failed) {
      //TODO:error
      printf("#ERR:failed elif,else\n");
      return false;
    }
  }
  //----------------
  if (type == IF_STRU_ID) {
    //if exist last node same this,just do false its is_complete
    if (entry_table.cole_end != 0 && entry_table.cole_end->fin == entry_table.cur_fin
        && entry_table.cole_end->sid == entry_table.cur_sid)
      entry_table.cole_end->is_complete = false;
      //else then create new node
    else {
      cole s = {0, entry_table.cur_fin, entry_table.cur_sid, false, 0, 0};
      append_cole(s);
    }
  } else if (get_cole_by_id(entry_table.cole_len).is_complete) {
    return true;
  }
  //----------------check if value is true
  Boolean bool_true = true;
  String bool_out = 0;
  if (type == IF_STRU_ID || type == ELIF_STRU_ID) {
    uint8 sub = 0;
    bool_out = calculate_boolean_expression(value, &sub);
    bool_true = str_to_bool(bool_out);
  }
//  printf("--Condition:%s=>%s(%i)\n", value, bool_out, bool_true);
//  print_struct(PRINT_CONDITION_LEVEL_ST);
//  display_all_registers();
  //----------------if bool_true is true
  if (bool_true) {
    Boolean ret = set_cole_complete(entry_table.cole_len);
    if (!ret) {
      //TODO:error
      printf("#ERR:%s\n", __func__);
      return false;
    }
    //--------------------record all registers
    stst s =
        {IF_STRU_ID, entry_table.cur_fid, entry_table.cur_fin, st_id, entry_table.cur_sid, entry_table.cur_order, 0};
    append_stst(s);
    //--------------------set new registers
    entry_table.cur_order = 1;
    entry_table.cur_sid = st_id;
    //--------------------start point
    int8 ret1 = APP_CONTROLLER();
    /*if return_fin > 0 && return_fin == cur_fin {
        is_stop_APP_CONTROLLER = true
    }*/
//  if (ret1 == 2) {
//
//  }
    //****************end of manage
    //--------------------call garbage_collector(gc)
    garbage_collector('A');
    //TODO
    //--------------------return to parent structure
    //print_struct(PRINT_FUNCTIONS_STACK_ST);
    stst lst = get_last_stst();
    entry_table.cur_sid = lst.parent_sid;
    entry_table.cur_order = lst.order;
    delete_last_stst();
  }
  return true;
}

//****************************************************
Boolean structure_LOOP(long_int st_id, uint8 type, str_list params) {
//  printf("LOOP[%i]:%s|%s|%s\n", st_id, params[0], params[1], params[2]);
  //--------------------init vars
  str_list init_vars = 0, conditions, do_more = 0;
  uint32 loop_number = 0, array_review_ind = 0;
  int8 status = 0;
  //--------------------record all registers
  stst s1 =
      {LOOP_STRU_ID, entry_table.cur_fid, entry_table.cur_fin, st_id, entry_table.cur_sid, entry_table.cur_order, 0};
  append_stst(s1);
  //--------------------set new registers
  entry_table.cur_order = 1;
  entry_table.cur_sid = st_id;
  //--------------------set new loop_level
  lole s = {0, entry_table.cur_fin, entry_table.cur_sid, 0, 0};
  append_lole(s);
  //----------------analyze part I (init vars)
  uint32 init_vars_len = structure_split_segments(params[0], &init_vars);
//  printf("@@(%i):%s\n", all_inst_len, print_str_list(all_inst, all_inst_len));
  for (uint32 i = 0; i < init_vars_len; i++) {
    //-------add to memory
    def_var_s vars_store[MAX_VAR_ALLOC_INSTRUCTIONS];
    uint8 vars_counter = define_vars_analyzing(init_vars[i], vars_store);
    for (uint8 j = 0; j < vars_counter; j++) {
      if (vars_store[j].value_var == 0) str_init(&vars_store[j].value_var, "null");
      vars_store[j].name_var = str_multi_append(vars_store[j].name_var, "[", vars_store[j].index_var, "]", 0, 0);
      long_int ret1 = set_memory_var(entry_table.cur_fin, entry_table.cur_sid, vars_store[j].name_var, vars_store[j]
          .value_var, vars_store[j].main_type, true);
      if (ret1 == 0) return false;
      //change flag field
      change_Mvar_flag(find_index_var_memory(ret1), 'l'/*loop header*/);
    }
  }
  //----------------short analyze part II,III (check conditions,do more!)
  uint32 conditions_len = structure_split_segments(params[1], &conditions);
  uint32 do_more_len = structure_split_segments(params[2], &do_more);
  //----------------start point
  for (;;) {
    //-------check conditions
    status = structure_loop_run_header(conditions, conditions_len, 2, &array_review_ind);
    if (status) break;
    else if (status == -1) return false;
    //-------execute loop block
    entry_table.cur_order = 1;
    status = APP_CONTROLLER();
    garbage_collector('A');
    loop_number++;
    //-------if get break signal
    if (status == BREAK_RETURN_APP_CONTROLLER || status == BAD_RETURN_APP_CONTROLLER) break;
    //-------do more!
    status = structure_loop_run_header(do_more, do_more_len, 3, &array_review_ind);
    if (status == -1) return false;
//    show_memory(0);
  }
  //--------------------delete header vars
  garbage_collector('S');
  //--------------------pop from loop_level
//  print_struct(PRINT_LOOP_LEVEL_ST);
  for (;;) {
    lole tmp = pop_last_lole();
    if (tmp.id != 0 && tmp.fin == entry_table.cur_fin && tmp.sid == entry_table.cur_sid) break;
  }
  //--------------------return to parent structure
  //print_struct(PRINT_FUNCTIONS_STACK_ST);
  stst lst = get_last_stst();
  entry_table.cur_sid = lst.parent_sid;
  entry_table.cur_order = lst.order;
  delete_last_stst();

  return true;
}
//****************************************************
/**
 * get list of loop header instructions for part2 or part3 and analyze their then if finished loop return true
 * @param insts
 * @param insts_len
 * @param part
 * @return Boolean
 */
int8 structure_loop_run_header(str_list insts, uint32 insts_len, uint8 part, uint32 *array_review_ind) {
  /**
 * instruction types:
 * ALLOC_MAGIC_MACROS_LBL_INST (p2,3) [OK]
 * ALLOC_VARS_LBL_INST (p3)           [OK]
 * FUNC_CALL_LBL_INST (p2,3)          [OK]
 * ALLOC_SHORT_LBL_INST (p3)          [OK]
 * LOGIC_CALC_LBL_INST (p2)           [OK]
 * REVIEW_ARRAY_LBL_INST (p2)         [..]
 */
  //init vars
  Boolean is_finished = false;
  String parcode = 0;
  Boolean is_error = false;
  //start analyzing
  for (uint32 i = 0; i < insts_len; i++) {
    if (is_finished) break;
    str_init(&parcode, insts[i]);
//    printf("check_cond[%i](%i):%s\n", i, part, parcode);
    while ((parcode = str_trim_space(parcode)) != 0) {
      uint8 state = labeled_loop_instruction(parcode, part);
      //-------------analyzing normal states
      if (state == UNKNOWN_LBL_INST) {
        exception_handler("unknown_instruction", __func__, parcode, 0);
        is_error = true;
        break;
      } else if (state == ALLOC_MAGIC_MACROS_LBL_INST)parcode = alloc_magic_macros(parcode);
      else if (state == FUNC_CALL_LBL_INST)parcode = function_call(parcode);
      else if (state == ALLOC_VARS_LBL_INST) {
        Boolean status = vars_allocation(parcode);
        parcode = 0;
        if (!status)is_error = true;
      } else if (state == ALLOC_SHORT_LBL_INST) parcode = vars_allocation_short(parcode);
        //-------------analyzing finish states
      else if (part == 2 && state == LOGIC_CALC_LBL_INST) {
        uint8 sub = 0;
        String tmp1 = calculate_boolean_expression(parcode, &sub);
        if (sub == 0) is_error = true;
//        printf("&TERMINAL(logic):%s=>%s\n", parcode,tmp1);
        if (str_equal(tmp1, "false")) is_finished = true;
        parcode = 0;
      }
// else if (part == 2 && state == REVIEW_ARRAY_LBL_INST) {
//        long_int array_id = 0;
//        longint_list alloc_ids = 0, alloc_inds = 0;
//        uint32 alloc_len = init_loop_review_array(parcode, &array_id, &alloc_ids, &alloc_inds);
//        printf("review_array\n");
//        if (alloc_len == 0) return -1;
//        int8 ret1 = structure_loop_review_array((*array_review_ind), array_id, alloc_ids, alloc_inds, alloc_len);
//        if (ret1 == true) is_finished = true;
//        parcode = 0;
//        (*array_review_ind)++;
//      }
    }
    if (is_error) return -1;
  }
  if (part == 3) {
    Boolean ret = check_post_short_alloc();
    if (!ret)return -1;
  }
  return is_finished;
}
//****************************************************
/**
 * get a part of code that include multiple instructions and split by ','. this function return all instructions from a part codes of a structure
 * @param part
 * @param segments
 * @return uint32
 */
uint32 structure_split_segments(String part, str_list *segments) {
  //-------------init vars
  uint32 len = str_length(part);
  uint32 slen = 0, pars = 0, acos = 0, bras = 0;
  if (len == 0) return 0;
  Boolean is_str = false;
  String buf = 0;
  //-------------start analyzing
  for (uint32 i = 0; i <= len; i++) {
    if (i < len && part[i] == '\"' && (i == 0 || part[i - 1] != '\\')) is_str = switch_bool(is_str);
    if (!is_str) {
      if (part[i] == '(')pars++;
      else if (part[i] == ')')pars--;
      else if (part[i] == '[')bras++;
      else if (part[i] == ']')bras--;
      else if (part[i] == '{')acos++;
      else if (part[i] == '}')acos--;
    }
    if (!is_str && pars == 0 && bras == 0 && acos == 0 && (part[i] == ',' || i == len)) {
      //printf("init_var:%s\n", buf);
      str_list_append(&(*segments), buf, slen++);
      buf = 0;
      continue;
    }
    buf = char_append(buf, part[i]);
  }
  return slen;
}

//****************************************************
uint8 labeled_loop_instruction(String code, uint8 part) {
  //----------------------init variables
  uint8 state = UNKNOWN_LBL_INST;
  Boolean is_string = false, is_equal = false, is_ret = false;
  uint16 par = 0/*count of parenthesis */, bra = 0/*count of brackets*/, aco = 0/*count of acolads*/, store_counter = 0,
      tmp_short_alloc = 0;
  String word = 0/*just create by words_splitter*/, case_word = 0/*create by words_splitter,single_operators*/,
      com_word = 0/*create by words_splitter,single_operators and skip brackets*/, buffer = 0;
  int16 last_pars = -1;
  String word_store[10];
  uint8 last_sep = 0;
  uint32 len = str_length(code);
  //msg("&LABELED:", code)
  /**
   * instruction types:
   * ALLOC_MAGIC_MACROS_LBL_INST (p2,3) [OK]
   * ALLOC_VARS_LBL_INST (p3)           [OK]
   * FUNC_CALL_LBL_INST (p2,3)          [OK]
   * ALLOC_SHORT_LBL_INST (p3)          [OK]
   * LOGIC_CALC_LBL_INST (p2)           [OK]
   * REVIEW_ARRAY_LBL_INST (p2)         [OK]
   */
  //----------------------analyzing code line
  for (uint32 i = 0; i < len; i++) {
    //------------------check is string
    if (code[i] == '\"' && (i == 0 || code[i - 1] != '\\')) {
      is_string = switch_bool(is_string);
    }
    //------------------count parenthesis,brackets,acolads
    if (!is_string) {
      if (i + 1 < len && code[i] == ' ' && code[i + 1] == '(') continue;
        //count parenthesis
      else if (code[i] == '(') par++;
      else if (code[i] == ')') par--;
        //count brackets
      else if (code[i] == '[')bra++;
      else if (code[i] == ']')bra--;
        //count acolads
      else if (code[i] == '{')aco++;
      else if (code[i] == '}')aco--;
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
      if (str_search(magic_macros, name, StrArraySize(magic_macros)))
        return ALLOC_MAGIC_MACROS_LBL_INST;
    }
    //------------------is function call
    if (!is_string && code[i] == '(' && case_word != 0 && is_valid_name(case_word, false)) {
      state = FUNC_CALL_LBL_INST;
//      printf("SSSSSSSS:%s\n", case_word);
      last_pars = par - 1;
      //break
    } else if (last_pars > -1 && last_pars == par && state == FUNC_CALL_LBL_INST) {
      break;
    }
    //------------------is ++ or --
    if (part == 3 && !is_string && i + 1 < len
        && ((code[i] == '+' && code[i + 1] == '+') || (code[i] == '-' && code[i + 1] == '-'))) {
      tmp_short_alloc = i + 1;
    }
    if (part == 3 && !is_string && (is_valid_name(com_word, true)
        || (com_word == 0 && is_valid_name(char_to_str(code[i]), true))) && (
        (tmp_short_alloc > 0) ||
            (i + 1 < len && ((code[i] == '+' && code[i + 1] == '+') || (code[i] == '-' && code[i + 1] == '-')))
    )) {
//      printf("TTT:%s\n", com_word);
      tmp_short_alloc = 0;
      state = ALLOC_SHORT_LBL_INST;
      break;
    }
    //------------------is review_array
    if (part == 2 && !is_string && code[i] == ':' && i + 1 < len) {
      state = REVIEW_ARRAY_LBL_INST;
    }
    //------------------is logic_calc
    String tmp1 = 0;
    if (!is_string && (code[i] == '=' || code[i] == '>' || code[i] == '<' || code[i] == '!')) {
      tmp1 = char_append(tmp1, code[i]);
      if (i + 1 < len && code[i + 1] == '=')tmp1 = char_append(tmp1, code[i + 1]);
    }
    if (part == 2 && !is_string && state == UNKNOWN_LBL_INST
        && str_search(comparative_operators, tmp1, StrArraySize(comparative_operators))) {
      state = LOGIC_CALC_LBL_INST;
    }
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
  if (state == UNKNOWN_LBL_INST) {
    if (part == 2)state = LOGIC_CALC_LBL_INST;
    else if (part == 3 && is_equal) state = ALLOC_VARS_LBL_INST;
  }

  //********************return state
  return state;
}
//****************************************************
Boolean structure_loop_next_break(String code) {
/**
	1- break || next
	2- break 1 || next (1+2)/3
	3- break 1 //break || next 1 //next
	4- break(1) || next (2)
	*/
  //********************init variables
  String inst = 0, counter = 0, word = 0;
  Boolean is_string = false;
  int32 par = 0, bra = 0;
  int8 mode = 0;
  uint32 len = str_length(code);
  //********************start analyzing exp
  for (uint32 i = 0; i < len; i++) {
    //------------------check is string
    if (code[i] == '\"' && (i == 0 || code[i - 1] != '\\'))is_string = switch_bool(is_string);
    //------------------count parenthesis,brackets
    if (!is_string) {
      if (code[i] == '(')par++;
      else if (code[i] == ')')par--;
      else if (code[i] == '[')bra++;
      else if (code[i] == ']')bra--;
    }
    //------------------find break or next
    if (!is_string && inst == 0 && (code[i] == '(' || code[i] == ' ' || i + 1 == len)) {
      if (i + 1 == len)word = char_append(word, code[i]);
      word = str_trim_space(word);
      if (str_equal(word, "break") || str_equal(word, "next")) {
        if (str_equal(word, "break"))mode = BREAK_INST;
        else mode = NEXT_INST;
        str_init(&inst, word);
        word = 0;
        if (i + 1 == len) {
          str_init(&counter, "1");
          break;
        }
      }
    }
    //------------------find counter
    if (!is_string && i + 1 == len) {
      word = char_append(word, code[i]);
      uint8 retsub = 0;
      calculate_math_expression(word, 'i', &counter, &retsub);
      break;
    }
    //------------------append to word
    word = char_append(word, code[i]);
  }
  //----------------determine loop_count,max_loop
  int32 loop_count = str_to_int32(counter);
  uint32 max_loop = entry_table.lole_len;
  //get loop count for cur_fin
  if (max_loop > 1 && loop_count > 1) {
    lole *tmp1 = entry_table.lole_end;
    max_loop = 0;
    for (;;) {
      if (tmp1->fin == entry_table.cur_fin) {
        max_loop++;
      } else break;
      tmp1 = tmp1->prev;
      if (tmp1 == 0) break;
    }
  }
//  print_struct(PRINT_LOOP_LEVEL_ST);
//  printf("next_break:%s[%s]:{%i,%i[%i]}\n", inst, counter,loop_count,max_loop,entry_table.cur_fin);
  //----------------check errors
  if (entry_table.lole_len == 0) {
    exception_handler("not_using_next_break", __func__, 0, 0);
    return false;
  }
  if (mode == BREAK_INST && (loop_count > max_loop || loop_count < 1)) {
    exception_handler("out_of_range_break", __func__, str_from_int32(max_loop), 0);
    return false;
  } else if (mode == NEXT_INST && loop_count != 1) {
    exception_handler("invalid_next_inst", __func__, 0, 0);
    return false;
  }
  //----------------set next_break_inst
  entry_table.next_break_inst = mode;
  //----------------if is break inst
  if (mode == BREAK_INST) {
    entry_table.break_count = loop_count;
  }
  //----------------return
  return true;
}
//****************************************************
/**
 * get review array instruction code,analyze it and return array var id and series of vars alloc ids for using structure_loop_review_array
 * @param code
 * @param array_id
 * @param alloc_ids
 * @return uint32
 */
uint32 init_loop_review_array(String code, long_int *array_id, longint_list *alloc_ids, longint_list *alloc_indexes) {
  //------------------------init vars
  Boolean is_string = false, is_med = false;
  uint32 bra = 0, alloc_len = 0;
  String word = 0, array_var = 0;
  uint32 len = str_length(code);
  //------------------------analyzing code
  for (uint32 i = 0; i < len; i++) {
    //------------------check is string
    if (code[i] == '\"' && (i == 0 || code[i - 1] != '\\'))is_string = switch_bool(is_string);
    //------------------count bra
    if (!is_string) {
      if (code[i] == '[')bra++;
      else if (code[i] == ']')bra--;
    }
    //------------------find alloc vars
    if (!is_string && !is_med && word != 0 && bra == 0 && (code[i] == ',' || code[i] == ':')) {
      String name = 0, index = 0;
      return_name_index_var(word, true, &name, &index);
      long_int var_id = return_var_id(name, 0);
      if (var_id == 0) {
        //TODO:error
        printf("#ERR1\n");
        return 0;
      }
      longint_list_append(&(*alloc_ids), alloc_len, var_id);
      if (index == 0)longint_list_append(&(*alloc_indexes), alloc_len, 0);
      else
        longint_list_append(&(*alloc_indexes), alloc_len, str_to_long_int(index));
      alloc_len++;
      word = 0;
      if (code[i] == ',')continue;
    }
    //------------------find array var
    if (!is_string && is_med && i + 1 == len) {
      word = char_append(word, code[i]);
      (*array_id) = return_var_id(str_trim_space(word), 0);
      if ((*array_id) == 0) {
        //TODO:error
        printf("#ERR2\n");
        return 0;
      }
    }
    //------------------find ':'
    if (!is_string && code[i] == ':') {
      is_med = true;
      word = 0;
      continue;
    }
    //------------------append to word
    word = char_append(word, code[i]);
  }
  return alloc_len;
}

//****************************************************
int8 structure_loop_review_array(uint32 review_number,
                                 long_int array_id,
                                 longint_list alloc_ids,
                                 longint_list alloc_inds,
                                 uint32 alloc_len) {
/**
 * loop(str s1[4],s2[4];s1,s2:ll) //ll[3,2,4] => s1[4],s2[4] (3)
 * loop(str s;s[0],s[1]:arr) //arr[3,2] => (3)
 * loop(str s[2,4];s:ll) //ll[3,2,4] => s[2,4] (3)
 */
  //----------------------init vars
  //----------------------check errors
  if (review_number == 0) {
    //TODO: if alloc_len>array_id[?][]
    //TODO: if alloc_inds>0&&array_id[?][?]
  }
  //----------------------
  //----------------------return
  return false;
}
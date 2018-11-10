//
// Created by amindelavar on 9/23/2018.
//

#include <MPL/system.h>

//********************************************
uint8 labeled_instruction (String code)
{
  //----------------------init variables
  uint8 state = UNKNOWN_LBL_INST;
  Boolean is_string = false, is_equal = false, is_ret = false;
  uint16 par = 0/*count of parenthesis */, bra = 0/*count of brackets*/, aco = 0/*count of acolads*/,
	  store_counter = 0;
  String word = 0/*just create by words_splitter*/, case_word = 0/*create by words_splitter,single_operators*/,
	  com_word = 0/*create by words_splitter,single_operators and skip brackets*/, buffer = 0;
  int16 last_pars = -1;
  String word_store[10];
  uint8 last_sep = 0;
  uint32 len = str_length (code);
  //msg("&LABELED:", code)
  //----------------------analyzing code line
  for (uint32 i = 0; i < len; i++)
	{
	  is_ret = false;
	  //------------------check is string
	  if (code[i] == '\"' && (i == 0 || code[i - 1] != '\\'))
		{
		  is_string = switch_bool (is_string);
		}
	  //------------------count parenthesis,brackets,acolads
	  if (!is_string)
		{
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
	      str_equal (word, "return"))
		{
		  state = RETURN_LBL_INST;
		  is_ret = true;
		  //break
		}
	  //------------------is next or break keyword
	  if (!is_string && ((code[i] == ' ' || code[i] == '(' || i + 1 == len) && word != 0 &&
	                     (str_equal (word, "next") || str_equal (word, "break") || i + 1 == len)))
		{
		  Boolean is_not = false;
		  if (i + 1 == len)
			{
			  is_not = true;
			  word = char_append (word, code[i]);
			  if (str_equal (word, "next") || str_equal (word, "break"))
				{
				  is_not = false;
				}
			}
		  if (!is_not)
			{
			  state = NEXT_BREAK_LBL_INST;
			  //msg("BBBBBBBBBBBBBBBB")
			  //break
			}
		}
	  //------------------is define variables
	  if (!is_string && !is_equal && code[i] == ' ' && word != 0 &&
	      !str_search (keywords_out, word, StrArraySize(keywords_out)))
		{
		  //printf("Data type:%s,%i\n", word,search_datas(word, 0, 0, true).id);
		  if (search_datas (word, 0, true).id == 0 && !is_ret)
			{
			  //exception_handler("wrong_type_var", "labeled_instruction:89", word, "")
			  //TODO:
			  return UNKNOWN_LBL_INST;
			}
		  else
			{
			  state = DEF_VARS_LBL_INST;
			  //printf("RRRRRRRRRRRRRRRRRR:%s,%s\n",word,code);
			}
		  //break
		}
	  //------------------is '='
	  if (!is_string && code[i] == '=')
		{
		  is_equal = true;
		}
	  //------------------is alloc magic macros
	  if (!is_string && code[i] == '=', com_word != 0)
		{
		  //printf("DDDD:%s,%s'n",com_word,buffer);
		  String name, index;
		  return_name_index_var (com_word, true, &name, &index);
		  if (str_search (magic_macros, name, StrArraySize(magic_macros)))
			return ALLOC_MAGIC_MACROS_LBL_INST;
		}
	  //------------------is structure
	  if (!is_string && word != 0 && str_indexof (word, STRUCTURES_LABEL, 0) == 0)
		{
		  state = STRUCTURE_LBL_INST;
		  break;
		}
	  //------------------is function call
	  if (!is_string && code[i] == '(' && case_word != 0 && is_valid_name (case_word, false))
		{
		  state = FUNC_CALL_LBL_INST;
		  //printf("SSSSSSSS:%s\n", case_word);
		  last_pars = par - 1;
		  //break
		}
	  else if (last_pars > -1 && last_pars == par && state == FUNC_CALL_LBL_INST)
		{
		  break;
		}
	  //------------------is ++ or --
	  if (!is_string && com_word != 0 && i + 1 < len &&
	      ((code[i] == '+' && code[i + 1] == '+') || (code[i] == '-' && code[i + 1] == '-')) &&
	      state == UNKNOWN_LBL_INST && is_valid_name (com_word, true))
		{
		  //msg("TTT%$:", com_word)
		  state = ALLOC_SHORT_LBL_INST;
		  break;
		}
	  //------------------
	  //------------------
	  //------------------append to buffer & word & case_word & com_word
	  buffer = char_append (buffer, code[i]);
	  //--------------
	  if (!is_string && char_search (words_splitter, code[i]))
		{
		  word_store[store_counter++] = str_trim_space (word);
		  last_sep = code[i];
		  if (store_counter >= 10)
			{
			  store_counter = 0;
			}
		  //msg("WORD:", word)
		  word = 0;
		}
	  else
		{
		  word = char_append (word, code[i]);
		  //msg("WORD_C", word)
		}
	  //--------------
	  if (!is_string && (char_search (words_splitter, code[i]) || char_search (single_operators, code[i])))
		{
		  case_word = 0;
		}
	  else
		{
		  case_word = char_append (case_word, code[i]);
		}
	  //--------------
	  if (!is_string && (char_search (words_splitter, code[i]) || char_search (single_operators, code[i])) &&
	      bra == 0 && code[i] != '[' && code[i] != ']')
		{
		  //msg("COM_WORD:", com_word)
		  com_word = 0;
		}
	  else
		{
		  com_word = char_append (com_word, code[i]);
		}

	}
  //------------------final switch
  //msg("&&&&", state)
  if (is_equal && state == UNKNOWN_LBL_INST)
	{
	  state = ALLOC_VARS_LBL_INST;
	  //case state == labeled_inst["func_call"]:
	}
  //********************return state
  return state;
}

//****************************************************
String define_vars (String inst)
{
  def_var_s vars_store[MAX_VAR_ALLOC_INSTRUCTIONS];
  uint8 vars_counter = define_vars_analyzing (inst, vars_store);
  //printf("!@!!@!@:%s,%i\n",inst,vars_counter);
  //msg("&vars", vars_store)
  //*******************************************add to memory
  if (vars_counter == 0)
	{
	  return 0;
	}
  long_int vars_list[MAX_VAR_ALLOC_INSTRUCTIONS];
  uint8 vars_list_counter = 0;
  //msg("---------------Add to memory:")
  for (uint8 i = 0; i < vars_counter; i++)
	{
	  if (vars_store[i].value_var == 0)
		{
		  str_init (&vars_store[i].value_var, "null");
		}
	  //printf("%DEF_VAR:%s;;%s;;%s\n",vars_store[i].main_type,vars_store[i].name_var,vars_store[i].value_var);
	  vars_store[i].name_var = str_multi_append (vars_store[i].name_var, "[", vars_store[i].index_var, "]", 0, 0);
	  long_int ret1 = set_memory_var (entry_table.cur_fin, entry_table.cur_sid, vars_store[i].name_var, vars_store[i]
		  .value_var, vars_store[i].main_type, true);
	  //msg("&INIT_VAR:", st.name_var,st.value_var)
	  if (ret1 == 0)
		{
		  return BAD_CODE;
		}
	  vars_list[vars_list_counter++] = ret1;

	}
  //show_memory(0);
  //msg("Define:", inst, cur_cid, cur_fid, cur_sid)
  return 0;
}

//****************************************************
String alloc_magic_macros (String exp)
{
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
  uint32 exp_len = str_length (exp);
  //---------------start analyzing
  for (int32 i = 0; i < exp_len; i++)
	{
	  //=====check is string
	  if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\'))
		{
		  is_string = switch_bool (is_string);
		}
	  //=====count pars
	  if (!is_string)
		{
		  if (exp[i] == ' ') continue;
		  else if (exp[i] == '(' && bras == 0)pars++;
		  else if (exp[i] == ')' && bras == 0)pars--;
		  else if (exp[i] == '[')bras++;
		  else if (exp[i] == ']')bras--;
		}
	  //=====check if magic macro
	  if (!is_string && buf == 0 && i + 2 < exp_len && exp[i] == '_' && exp[i + 1] == '_')
		{
		  is_buf = true;
		  start = i;
		  bras_start = bras;
		}
	  //=====get a magic macro
	  if (!is_string && buf != 0 && is_buf && bras_start == bras && exp[i] != ']' &&
	      (char_search (single_operators, exp[i]) || char_search (words_splitter, exp[i]) || i + 1 == exp_len))
		{
		  is_buf = false;
		  end = i;
		  bras_start = -1;
		  //printf("OOOOO:%s$\n", buf);
		}
	  //=====is equal
	  if (!is_string && exp[i] == '=')
		{
		  equal_ind = i;
		  if (i >= end)is_use_magic = false;
		}

	  //=====append to buf
	  if (is_buf)
		{
		  buf = char_append (buf, exp[i]);
		}
	}
  if (buf == 0)
	{
	  return BAD_CODE;
	}
  //---------------get name and index of magic macro
  return_name_index_var (buf, false, &mm_name, &mm_index);
  mm_index = str_reomve_quotations (mm_index, "s");
  //---------------determine type of magic macro
  if (str_equal (mm_name, "__define"))mm_type = DEFINE_MAGIC_MACRO_TYPE;
  else if (str_equal (mm_name, "__config"))mm_type = CONFIG_MAGIC_MACRO_TYPE;
  else if (str_equal (mm_name, "__session"))mm_type = SESSION_MAGIC_MACRO_TYPE;
  else
	{
	  exception_handler ("unknown_magic_macro", __func__, mm_name, 0);
	  return BAD_CODE;
	}
  //---------------is use type
  if (is_use_magic)
	{
	  //printf("##use_magic:%s=>%s(%s,%s)\n", exp, buf, mm_name, mm_index);
	  mama s = get_mama (mm_type, mm_index);
	  if (s.id == 0)
		{
		  exception_handler ("not_defined_mm_key", __func__, mm_index, mm_name);
		  return BAD_CODE;
		}
	  if (s.sub_type == 's') s.value = convert_to_string (s.value);
	  ret_exp = replace_in_expression (exp, s.value, start, end, true, true);
	}
	//---------------is define type
  else
	{
	  String value = str_substring (exp, equal_ind + 1, 0);
	  //-------------analyzing vlaue
	  //check if has magic macro
	  while (true)
		{
		  String tmp1 = alloc_magic_macros (value);
		  if (str_equal (tmp1, BAD_CODE))break;
		  str_init (&value, tmp1);
		}
	  //calculate value
	  //printf("EEEEE:%s$\n",value);
	  String type = determine_value_type (value);
	  uint8 subtype = '0';
	  calculate_value_of_var (value, type, &value, &subtype);
	  if (mm_type == DEFINE_MAGIC_MACRO_TYPE && get_mama (mm_type, mm_index).id > 0)
		{
		  exception_handler ("reinitialized_in__define_mm", __func__, mm_index, mm_name);
		  return BAD_CODE;
		}
	  else if (mm_type == CONFIG_MAGIC_MACRO_TYPE && get_mama (CONFIG_MAGIC_MACRO_TYPE, mm_index).id == 0)
		{
		  exception_handler ("not_exist__config_mm", __func__, mm_index, 0);
		  return BAD_CODE;
		}
	  else if (mm_type == CONFIG_MAGIC_MACRO_TYPE && entry_table.cur_fin != 0)
		{
		  exception_handler ("not_global__config_mm", __func__, 0, 0);
		  return BAD_CODE;
		}
	  //TODO:errors
	  //printf("##define_magic:%s=>%s(%s,%s) : %s,%s,%c\n", exp, buf, mm_name, mm_index, value, type, subtype);
	  if (subtype == 's')value = str_reomve_quotations (value, "s");
	  add_to_mama (mm_type, subtype, mm_index, value);
	  ret_exp = 0;
	}
  //---------------return
  //printf("@EEEEEEEE:%s\n", ret_exp);
  return ret_exp;
}

//****************************************************
String function_call (String exp)
{
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
  str_list parameters = 0;
  uint32 params_len = 0;
  Boolean is_string = false, is_par = false, is_struct = false;
  int32 pars = 0, bras = 0, acos = 0, st_func = -1, en_func = -1, tmp1 = -1, pars_num = -1, bras_num = 0,
	  count_ind = 0, struct_par = -1;
  uint32 exp_len = str_length (exp);
  //********************start analyzing
  for (uint32 i = 0; i < exp_len; i++)
	{
	  Boolean no_buffer = false;
	  //------------------check is string
	  if (exp[i] == '\"' && (i == 0 || exp[i - 1] != '\\'))
		{
		  is_string = switch_bool (is_string);
		}
	  //------------------check is struct
	  if (!is_string && !is_struct && exp[i] == '(' && str_equal (word, "struct"))
		{
		  is_struct = true;
		  struct_par = pars + 1;
		}
	  else if (!is_string && is_struct && exp[i] == ')' && struct_par == pars)
		{
		  is_struct = false;
		}
	  //------------------continue if ' '
	  if (!is_string && i + 1 < exp_len && exp[i] == ' ' && (exp[i + 1] == '(' || exp[i + 1] == '['))continue;
	  //------------------count parenthesis
	  if (!is_string)
		{
		  if (exp[i] == '(')pars++;
		  else if (exp[i] == ')')pars--;
		  else if (exp[i] == '[')bras++;
		  else if (exp[i] == ']')bras--;
		  else if (exp[i] == '{')acos++;
		  else if (exp[i] == '}')acos--;
		}
	  //------------------if is '.' (pack_name)
	  if (!is_string && exp[i] == '.' && word != 0 && is_valid_name (word, true) && en_func == -1 && acos == 0)
		{
		  st_func = tmp1;
		  str_init (&pack_name, word);
		  func_name = 0;
		  is_par = false;
		  en_func = -1;
		  str_empty (&word);
		  continue;
		}
	  //------------------if is '(' (func_name)
	  if (!is_string && exp[i] == '(' && word != 0 && !str_equal (word, "struct") && is_valid_name (word, false) &&
	      en_func == -1 && acos == 0)
		{
		  //printf("&HHHH:%s,%s\n", word,print_str_list(parameters, params_len));
		  str_init (&func_name, word);
		  pars_num = pars - 1;
		  bras_num = bras;
		  parameters = 0;
		  params_len = 0;
		  is_par = true;
		  is_struct = false;
		  str_empty (&word);
		  str_empty (&buffer);
		  if (st_func == -1)st_func = tmp1;
		  continue;
		}
	  //------------------ get parameters
	  if (!is_string && !is_struct && (exp[i] == ',' || (exp[i] == ')' && pars == pars_num)) && is_par && acos == 0 &&
	      bras == bras_num)
		{
		  buffer = str_trim_space (buffer);
		  //msg("&DDD:", buffer, pars_num, pars)
		  if (buffer != 0)str_list_append (&parameters, buffer, params_len++);

		  if (exp[i] == ')' && pars == pars_num)
			{
			  is_par = false;
			  en_func = i + 1;
			}
		  if (!is_par && exp[i] == ')')
			{
			  //is func has index
			  int32 br = 0;
			  for (uint32 b = i + 1; b < exp_len; b++)
				{
				  if (exp[b] != '[' && exp[b] != ' ' && br == 0) break;
				  else if (exp[b] == '[')br++;
				  else if (exp[b] == ']')br--;
				  index = char_append (index, exp[b]);
				  en_func = b + 1;
				  if (br == 0)break;
				}
			  index = str_substring (index, 1, str_length (index) - 1);
			  break;
			}
		  str_empty (&buffer);
		  str_empty (&word);
		  continue;
		}
	  //------------------append to buffer & word
	  if (!no_buffer)
		{
		  buffer = char_append (buffer, exp[i]);
		}
	  if (!is_string &&
	      (exp[i] == ' ' || exp[i] == '.' || exp[i] == ',' || exp[i] == '(' || exp[i] == ')' || exp[i] == '=' ||
	       char_search (single_operators, exp[i])) && bras == 0)
		{
		  count_ind = i;
		  word = 0;
		}
	  else
		{
		  if (word == 0)
			{
			  tmp1 = i;
			}
		  word = char_append (word, exp[i]);
		}
	  //msg("TTTT:", word)
	}
  //********************calling functions
  //printf("&DDD:%s=>%s,%s,%s,%s,%i,%i,%s=>%s\n", exp, pack_name, func_name, print_str_list(parameters, params_len),index, st_func, en_func, ret_exp, str_substring(exp, st_func, en_func));
  ret_vars = init_calling_function (pack_name, func_name, parameters, params_len, index);
  if (ret_vars == 0)str_init (&ret_vars, "null");
  //	//********************return
  //	//msg("&PP", func_pack, func_name, parameters, "\n", index, st_func, en_func, ret_exp, ret_vars, pars_num)
  //	//show_memory(40)
  //	if st_func == 0 && status == 0
  //	{
  //		return ""
  //	}
  //	if status == 1
  //	{
  //		return ret_vars
  //	}
  if (en_func > -1 && st_func > -1)
	{
	  ret_exp = str_multi_append (str_substring (exp, 0, st_func), ret_vars, str_substring (exp, en_func, 0), 0, 0, 0);
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
String init_calling_function (String pname, String fname, str_list params, uint32 param_len, String index)
{
  //--------------------init vars
  str_list vars_return = 0;
  uint32 vars_ret_len = 0;
  String ret_vars = 0;
  //--------------------record all registers
  fust s = {entry_table.cur_fid, entry_table.cur_fin, entry_table.cur_sid, entry_table.cur_order,
            entry_table.parent_fin};
  append_fust (s);
  //--------------------set new parent fin
  entry_table.parent_fin = entry_table.cur_fin;
  //--------------------init function parameters
  int32 ret0 = set_function_parameters (pname, fname, params, param_len);
  //********************
  //msg("&YYY", func_f, ret0)
  Boolean is_return = false;
  if (ret0 == -1)
	{
	  //TODO:
	}
  else if (ret0 == 2)
	{
	  //*********************search for built-in functions
	  is_return = true;
	}
  else if (ret0 == 1)
	{

	  entry_table.cur_order = 1;
	  entry_table.cur_sid = 0;
	  APP_CONTROLLER ();
	  is_return = true;
	}
  //TODO:
  if (is_return)
	{
	  long_int ret_po_ind = find_index_pointer_memory (RETURN_TMP_POINTER_ID);
	  str_list rets = 0;
	  longint_list rets_ind = 0;
	  uint32 rets_ind_len = 0;
	  uint32 rets_len = char_split (get_Mpoint (ret_po_ind).data, ';', &rets, true);
	  for (uint32 i = 0; i < rets_len; ++i)
		{
		  longint_list_append (&rets_ind, rets_ind_len++, find_index_var_memory (str_to_long_int (rets[i])));
		}
	  edit_Mpoint (ret_po_ind, 0, 0, true, false);

	  //*********************return and reset settings
	  if (index != 0)
		{
		  String reti = 0;
		  uint8 rett = 0;
		  calculate_math_expression (index, '_', &reti, &rett);
		  int32 real_ind = str_to_int32 (reti);
		  if (rett == 'f' || rett == 'h')
			{
			  exception_handler ("not_float_array_index", __func__, 0, 0);
			}
		  else if (real_ind < 0 || real_ind >= rets_len)
			{
			  exception_handler ("array_index_out_of_range", __func__, str_from_int32 (real_ind), str_from_int32 (
				  rets_len - 1));
			}
		  else
			{
			  str_init (&ret_vars, return_value_var_complete (rets_ind[real_ind]));
			}
		}
	  else
		{
		  for (uint32 i = 0; i < rets_len; ++i)
			{
			  String vall = return_value_var_complete (rets_ind[i]);
			  // printf ("#@@@@@@@:%s[%i]=>%s\n", rets[i],rets_ind[i], vall);
			  ret_vars = str_append (ret_vars, vall);
			  if (i + 1 < rets_len)ret_vars = char_append (ret_vars, ',');
			}
		}
	}
  //	//*********************close function
  //	//msg("&####&&:", last_fin, cur_fin)
  //	if ret0 != 2
  //	{
  //		garbage_collector('f')
  //	}
  //print_struct(PRINT_FUNCTIONS_STACK_ST);
  fust lst = get_last_fust ();
  delete_last_fust ();
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
Boolean
is_exact_function (str_list func_params, uint32 func_params_len, str_list type_params, uint32 params_len, Boolean is_built_in)
{
  //printf ("##FUNCII:%s,%s\n", print_str_list (func_params, func_params_len), print_str_list (type_params, params_len));
  if (func_params_len == 0 && params_len != 0)
	{
	  //printf ("0####################\n");
	  return false;
	}
  else
	{
	  //if last parameter of func_params is vars
	  Boolean is_last_vars = false;
	  if (str_indexof (func_params[func_params_len - 1], "vars;", 0) == 0
	      || (is_built_in && str_indexof (func_params[func_params_len - 1], "aa..;", 0) == 0))
		is_last_vars = true;
	  //if call params is less than func params
	  if (params_len < func_params_len && !is_last_vars)
		{
		  //printf ("1####################\n");
		  return false;
		}
	  //if func params is less than call params
	  if (func_params_len < params_len && !is_last_vars)
		{
		  //printf ("2####################\n");
		  return false;
		}
	  //move on ret_pars array
	  for (uint32 i = 0; i < params_len; ++i)
		{
		  Boolean is_a_builtin = false;
		  if (func_params_len < i + 1) break;
		  //printf ("+STR:%s==%s\n", func_params[i], type_params[i]);
		  str_list p1 = 0, p2 = 0;
		  char_split (func_params[i], ';', &p1, false);
		  char_split (type_params[i], ';', &p2, false);
		  //-----check if aa in built-in
		  if (is_built_in && str_equal (p1[0], "aa"))continue;
		  if (is_built_in && str_equal (p1[0], "aa.."))break;
		  else if (is_built_in && str_ch_equal (p1[0], 'a'))is_a_builtin = true;
		  //-----check if vars
		  if (!is_a_builtin && str_equal (p1[0], "vars"))
			{
			  str_list p3 = 0;
			  for (uint32 j = i; j < params_len; ++j)
				{
				  char_split (type_params[j], ';', &p3, false);
				  if ((p3[3] != 0 && !str_ch_equal (p3[3], '0')/*if is array*/)
				      || str_ch_equal (p3[2], '2')/*if is reference var*/)
					{
					  //printf("3####################:%s\n",type_params[j]);
					  return false;
					}
				}
			  break;
			}
		  //-----check data types
		  if (!is_a_builtin && !str_equal (p1[0], p2[0]))
			{
			  //printf("2####################\n");
			  return false;
			}
		  //-----check dimensions
		  //printf("@WWWW:%s,%s\n", p1[2], p2[3]);
		  if (!is_equal_arrays_indexes (p1[2], p2[3]))
			{
			  //printf("@##EEE\n");
			  return false;
			}
		}
	  //printf("FFFFFFFFFFFFFFFF\n");
	}
  //-------- if find function
  //printf("FUNC_PARS:%s,%s\n", func_name, print_str_list(func_params, func_params_len));

  return true;
}

//****************************************************
int32 set_function_parameters (String pack_name, String func_name, str_list pars, uint32 pars_len)
{
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
  //printf("SDDDDDDD:%s,%i\n", print_str_list(pars, pars_len), pars_len);
  //-----------------------------init vars
  Boolean is_user_func = false;
  str_list func_params = 0;
  uint32 func_params_len = 0;
  //-----------------------------determine parameters type
  str_list ret_pars = 0;
  uint32 ret_pars_len = determine_type_name_func_parameters (pars, pars_len, &ret_pars);
  //printf("######PARS:%s\n%s\n", print_str_list(ret_pars, ret_pars_len), print_str_list(pars, pars_len));
  //-----------------------------search for function
  blst *tmp1;
  tmp1 = entry_table.blst_func_start;
  //------------------
  for (;;)
	{
	  if (str_equal (tmp1->lbl, func_name))
		{
		  // printf("##FUNC:%s,%s\n", tmp1->lbl, print_str_list(tmp1->params, tmp1->params_len));
		  //-------- if find function
		  if (is_exact_function (tmp1->params, tmp1->params_len, ret_pars, ret_pars_len, false))
			{
			  //printf("FUNC_PARS:%s,%s\n", func_name, print_str_list(tmp1->params, tmp1->params_len));
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
  if (is_user_func)
	{
	  entry_table.cur_fin = ++entry_table.func_index;
	}
	//-----------------------------maybe is a built-in function or not valid a function
  else
	{
	  //TODO:create built-in function
	  str_list return_builtin = 0;
	  uint32 return_builtin_len = call_built_in_funcs (func_name, pars, ret_pars, pars_len, &return_builtin);
	  //if not a built-in function
	  exception_handler ("not_exist_func", __func__, func_name, print_str_list (pars, pars_len));
	  return 0;
	}
  //-----------------------------init & allocate function params
  Boolean is_vars = false;
  String vars_name = 0;
  String vars_po_ids = 0;
  for (uint32 i = 0; i < func_params_len; ++i)
	{
	  str_list p1 = 0, p2 = 0;
	  uint32 p1_len = 0;
	  char_split (func_params[i], ';', &p2, false);
	  if (i < ret_pars_len)
		{
		  p1_len = char_split (ret_pars[i], ';', &p1, false);
		  //printf("#STR:%s==%s&&%s (%i,%i)\n", ret_pars[i], func_params[i], pars[i], p1_len, func_params_len);
		  if (p1_len == 0)break;
		}
	  //====is vars
	  if (!is_vars && str_equal (p2[0], "vars"))
		{
		  is_vars = true;
		  str_init (&vars_name, p2[1]);
		  //if called function parameters has more than one vars values
		  if (ret_pars_len > func_params_len)
			{
			  for (uint32 j = 0; j < func_params_len - ret_pars_len; ++j)
				str_list_append (&func_params, "tmp;;0", func_params_len++);
			}
			//if called function parameters has no any vars values
		  else if (ret_pars_len < func_params_len)
			{
			  String po_id = str_from_long_int (add_to_pointer_memory ("null", '0'));
			  str_init (&vars_po_ids, po_id);
			  break;
			}
		}
	  //====define var by value
	  if (str_ch_equal (p1[2], '0'))
		{
		  if (is_vars)
			{
			  String val = 0;
			  uint8 sub = '0';
			  calculate_value_of_var (pars[i], p1[0], &val, &sub);
			  String po_id = str_from_long_int (add_to_pointer_memory (val, sub));
			  if (vars_po_ids != 0)vars_po_ids = char_append (vars_po_ids, ';');
			  vars_po_ids = str_append (vars_po_ids, po_id);
			}
		  else
			{
			  String namei = 0;
			  if (p1[3] == 0 || str_ch_equal (p1[3], '0'))str_init (&namei, p2[1]);
			  else namei = str_multi_append (p2[1], "[", p1[3], "]", 0, 0);
			  set_memory_var (entry_table.cur_fin, 0, namei, pars[i], p2[0], true);
			}
		}
		//====define var by var
	  else if (str_ch_equal (p1[2], '1'))
		{
		  if (is_vars)
			{
			  Mpoint v = get_Mpoint (get_data_memory_index (get_Mvar (str_to_long_int (p1[1])).pointer_id, "0"));
			  String po_id = str_from_long_int (add_to_pointer_memory (v.data, v.type_data));
			  if (vars_po_ids != 0)vars_po_ids = char_append (vars_po_ids, ';');
			  vars_po_ids = str_append (vars_po_ids, po_id);
			}
		  else
			{
			  copy_memory_var (str_to_long_int (p1[1]), p2[1], entry_table.cur_fin);
			}
		}
		//====define var by reference var
	  else if (str_ch_equal (p1[2], '2'))
		{
		  Mvar ref_var = get_Mvar (str_to_long_int (p1[1]));
		  add_to_var_memory (ref_var.pointer_id, entry_table.cur_fin, 0, ref_var.type_var, p2[1], 0);
		}
	}

  if (is_vars)
	{
	  long_int main_pointer_id = add_to_pointer_memory (vars_po_ids, 'p');
	  add_to_var_memory (main_pointer_id, entry_table.cur_fin, 0, search_datas ("vars", 0, true).id, vars_name, 0);
	  //printf("$RRRRR:%i,%i,%i,%s\n", main_pointer_id, entry_table.cur_fin,search_datas("vars", 0, true).id,vars_name);
	}
  //show_memory(0);
  return 1;
}

//****************************************************
uint32 determine_type_name_func_parameters (str_list params, uint32 params_len, str_list *ret)
{
  //printf("$$$$TP:%s,%i\n", print_str_list(params, params_len), params_len);
  //-----------init vars
  uint32 ret_len = 0;
  //ret[i]="type;var_index;state;var_dimensions" state: 0=value,1=var,2=reference
  for (uint32 i = 0; i < params_len; ++i)
	{
	  uint32 param_len = str_length (params[i]);
	  //printf("@@@PAR:%s,%i\n", params[i],is_valid_name(params[i], true));
	  //====if is a variable or reference variable
	  if (is_valid_name (params[i], true) ||
	      (param_len > 1 && params[i][0] == '&' && is_valid_name (str_substring (params[i], 1, 0), false)))
		{
		  uint8 state = '1';
		  String par_var = 0;
		  if (params[i][0] == '&')
			{
			  str_init (&par_var, str_substring (params[i], 1, 0));
			  state = '2';
			}
		  else str_init (&par_var, params[i]);
		  String state_s = str_multi_append (";", char_to_str (state), ";", 0, 0, 0);
		  String name = 0, index = 0;
		  return_name_index_var (par_var, true, &name, &index);
		  long_int var_id = return_var_id (name, "0");
		  if (var_id > 0)
			{
			  long_int real_id = find_index_var_memory (var_id);
			  Mvar var = get_Mvar (real_id);
			  String type_name = 0;
			  str_init (&type_name, get_datas (var.type_var).name);
			  //if is complete var
			  if (index == 0)
				{
				  str_list tmp1;
				  uint32 tmp1_len = return_var_dimensions (real_id, &tmp1);
				  String var_dim = char_join (tmp1, ',', tmp1_len, true);
				  //printf("^^^FF:%s=>%i,%s\n",params[i],tmp1_len,tmp1[0]);
				  if (str_ch_equal (var_dim, '1') || str_ch_equal (var_dim, '0'))var_dim = 0;
				  str_list_append (&(*ret),
				                   str_multi_append (type_name, ";", str_from_long_int (real_id), state_s, var_dim, 0),
				                   ret_len++);
				}
				//if is a room of var
			  else
				{
				  str_list_append (&(*ret),
				                   str_multi_append (type_name, ";", str_from_long_int (real_id), state_s, index, 0),
				                   ret_len++);
				}
			  continue;
			}
		}
	  //====if is a value
	  String value_type = 0, var_dim = 0, value = 0;
	  //get var dimensions
	  int32 indexes[MAX_ARRAY_DIMENSIONS];
	  uint8 count = return_size_value_dimensions (params[i], indexes, &value);
	  for (uint8 j = 0; j < count; ++j)
		{
		  var_dim = str_append (var_dim, str_from_int32 (indexes[j]));
		  if (j + 1 < count)var_dim = char_append (var_dim, ',');
		}
	  //determine value type
	  value_type = determine_value_type (value);
	  //if not array
	  if (str_ch_equal (var_dim, '1') && !has_two_limiting (params[i], '{', '}', true)) str_init (&var_dim, "0");
	  //append to ret
	  //printf("#VAL:%s=>%s,%s,%s\n", params[i],var_dim, value,value_type);
	  str_list_append (&(*ret), str_multi_append (value_type, ";0;0;", var_dim, 0, 0, 0), ret_len++);
	}

  return ret_len;
}

//****************************************************
Boolean function_return (String exp)
{
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
  uint32 len = str_length (exp);
  //****************analyzing
  for (uint32 i = 0; i < len; i++)
	{
	  //------------------check is string
	  if (exp[i] == '"' && (i == 0 || exp[i - 1] != '\\'))
		{
		  is_string = switch_bool (is_string);
		}
	  //------------------count pars
	  if (!is_string)
		{
		  if (exp[i] == '(')pars++;
		  else if (exp[i] == ')')pars--;
		  else if (exp[i] == '{')acos++;
		  else if (exp[i] == '}')acos--;
		  else if (exp[i] == '[')bras++;
		  else if (exp[i] == ']')bras--;
		}
	  //------------------find return keyword
	  if (!is_string && (exp[i] == ' ' || (exp[i] == '(' && pars == 1)) && str_equal (word, "return"))
		{
		  str_empty (&word);
		  if (exp[i] != '(')continue;

		}
	  //------------------append to word
	  if (!is_string && ((exp[i] == ',' && bras == 0 && pars == 0 && acos == 0) || i + 1 == len))
		{
		  if (i + 1 == len)
			{
			  word = char_append (word, exp[i]);
			}
		  str_list_append (&return_vals, str_trim_space (word), return_vals_len++);
		  str_empty (&word);
		}
	  else word = char_append (word, exp[i]);
	}
  //msg("&XXX:", return_vals)
  //****************init return parameters
  return_types_len = determine_type_name_func_parameters (return_vals, return_vals_len, &return_types);
  //msg("&YYYY:", return_params)
  //****************allocate parameters together
  for (uint32 i = 0; i < return_types_len; i++)
	{
	  //msg("&RET:", return_params[i], last_fin, cur_fin)
	  str_list p1 = 0;
	  char_split (return_types[i], ';', &p1, false);
	  tmp_names_count++;
	  String tmp2 = generate_return_var_name (0, &tmp_names_count);
	  //printf("$WQQQQ:%s,%s=>%s\n",return_vals[i],return_types[i],tmp2);
	  if (str_equal (p1[0], "str") && str_ch_equal (p1[2], '0')/*is value*/ && str_length (return_vals[i]) > 0
	      && return_vals[i][0] != '\"')
		{
		  return_vals[i] = str_multi_append ("\"", return_vals[i], "\"", 0, 0, 0);
		}
	  //---------------define var by value
	  if (str_ch_equal (p1[2], '0'))
		{
		  //msg("RET_MMM:", tmp2, tmp1[1], tmp1[0])
		  long_int var_id = set_memory_var (entry_table.parent_fin, 0, tmp2, return_vals[i], p1[0], true);
		  if (return_ids != 0)return_ids = char_append (return_ids, ';');
		  return_ids = str_append (return_ids, str_from_long_int (var_id));
		}
		//---------------define var by variable
	  else if (str_ch_equal (p1[2], '1'))
		{
		  long_int var_id = copy_memory_var (str_to_long_int (p1[1]), tmp2, entry_table.parent_fin);
		  if (return_ids != 0)return_ids = char_append (return_ids, ';');
		  return_ids = str_append (return_ids, str_from_long_int (var_id));
		}
	}
  //****************set in RETURN_TMP_POINTER_ID
  edit_Mpoint (find_index_pointer_memory (RETURN_TMP_POINTER_ID), return_ids, 0, true, false);
  //****************return
  //show_memory(40)
  return true;
}

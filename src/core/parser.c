//
// Created by amindelavar on 9/19/2018.
//
#include <MPL/system.h>
#include <wchar.h>

//**********************************basic variables
long_int Aline = 0;
Boolean is_in_func = false;
uint16 parse_pars = 0;
uint32 Apath = 0;
uint8 block_id = 0; //index of is_in_stru
long_int cur_func_id = 0;
long_int cur_stru_id = 0;
Boolean is_inline_stru = false;
stoi is_in_stru[MAX_INTO_IN_STRUCTURES];

//**********************************
Boolean analyze_source_code() {
//  convert_single_quotations_to_std();
  //-------------------reset all
  clear_soco(2);
  empty_stoi(is_in_stru, MAX_INTO_IN_STRUCTURES);
  parse_pars = 0;
  block_id = 0;
  cur_stru_id = 0;
  cur_func_id = 0;
  is_in_func = false;
  is_inline_stru = false;
  //-------------------get tokens
  get_all_tokens();
  //print_struct(PRINT_TOKENS_SOURCE_ST);
  //-------------------init vars
  uint8 state = 0; //1:func , 2:stru , 3:struct
  uint32 ret = source_paths_search(entry_table.cur_source_path);
  entry_table.cur_ascii_source_path = utf8_to_bytes_string(entry_table.cur_source_path);
  //printf("CALL:%s\n",entry_table.cur_ascii_source_path);
  if (ret > 0)Apath = ret;
  else {
    utf8_str_list_append(&source_paths, entry_table.cur_source_path, entry_table.source_counter++);
    Apath = entry_table.source_counter - 1;
  }
  //printf("GGGGG:%s,%i\n", print_str_list(source_paths, entry_table.source_counter), entry_table.source_counter);
  //-------------------start parsing codes line by line
  for (uint32 i = 0; i < entry_table.soco_tokens_count; i++) {
    soco token_item = get_soco(2, i);
    String Acode = token_item.code;
    Aline = token_item.line;
    //printf("$%s,%i,%i\n", Acode,i,is_in_func);
    //-------------count pars
    if (str_ch_equal(Acode, '{')) {
      parse_pars++;
      if (state == 1/*func*/) {
        is_in_func = true;
        state = 0;
      } else if (state == 2/*stru*/ && block_id > 0) {
        /*is_in_stru[block_id - 1].is_active = true;
        is_in_stru[block_id - 1].stru_pars = parse_pars - 1;*/
        state = 0;
      }
      //msg("&OP", parse_pars)
    } else if (str_ch_equal(Acode, '}')) {
      parse_pars--;
      //is func
      if (parse_pars == 0) {
        is_in_func = false;
        cur_func_id = cur_stru_id = 0;
      }
      //is stru
      if (block_id > 0) {
        uint8 c = block_id;
        for (;;) {
          c--;
          //printf("FFFF:%i,%i\n",c,is_in_stru[0].stru_pars);
          if (is_in_stru[c].is_active && is_in_stru[c].stru_pars == parse_pars) {
            is_in_stru[c].is_active = false;
            is_in_stru[c].stru_pars = 0;
            break;
          }
          if (c == 0) break;
        }
        cur_stru_id = get_last_id_active_is_in_stru();
      }
    }
    //-------------import keyword
    if (state == 0 && str_equal("import", Acode)) {
      i++;
      //printf("SSSSSS:%s,%i\n",Acode,StrArraySize(sharp_instructions));
      manage_import_keywords(&i);
      continue;
    }
    //-------------structs
    if (state == 0 && str_equal(Acode, "struct") && i + 3 < entry_table.soco_tokens_count) {
      //state = 3;
      i++;
      manage_structs(&i);
      continue;
    }
    //-------------function headers
    if (state == 0 && str_equal(Acode, "func") && i + 3 < entry_table.soco_tokens_count) {
      //if is_in_func
      if (is_in_func) {
        String name = 0;
        str_init(&name, get_soco(2, i + 1).code);
        print_error(Aline, "define_func_in", entry_table.cur_ascii_source_path, name, "", "analyze_source_code");
        break;
      }
      state = 1;
      i++;
      manage_functions(&i);
      continue;
    }
    //-------------structure headers
    if (state == 0 && str_search(block_instructions, Acode, StrArraySize(block_instructions))) {
      state = 2;
      i++;
      if (str_equal(Acode, "else"))i--;
      is_inline_stru = false;
      manage_structures(&i, Acode);
      //if stru is inline
      if (is_inline_stru) {
        is_in_stru[block_id - 1].is_inline = true;
        is_in_stru[block_id - 1].is_active = true;
//        printf("FFDDDD:%s\n", Acode);
        state = 0;
      }
      continue;
    }
    //-------------normal instructions
    if (state == 0 && !str_ch_equal(Acode, '{') && !str_ch_equal(Acode, '}') && !str_ch_equal(Acode, ';')) {
      manage_normal_instructions(&i);
    }
    //-------------inline stru
    if (block_id > 0 && !str_ch_equal(Acode, ';')) {
      uint8 c = block_id;
      for (;;) {
        c--;
        if (is_in_stru[c].is_inline && is_in_stru[c].is_active) {
          is_in_stru[c].is_active = false;
          is_in_stru[c].is_inline = false;
          //println("GGGG:", is_in_stru[c].id)
          break;
        }
        if (c == 0) break;
      }
      cur_stru_id = get_last_id_active_is_in_stru();
    }

  }
  //-------------check for syntax error
  //msg("&%%%", parse_pars)
  if (parse_pars > 0) {
    print_error(Aline, "not_end_acod", entry_table.cur_ascii_source_path, "", "", "analyze_source_code");
    return false;
  } else if (parse_pars < 0) {
    print_error(Aline, "not_start_acod", entry_table.cur_ascii_source_path, "", "", "analyze_source_code");
    return false;
  }
  //--------------------------------
  //print_struct(PRINT_IMPORT_ST);
  //print_struct(PRINT_PACK_ST);
  //print_struct(7);*/
  return true;
}

//**********************************
void manage_import_keywords(uint32 *i) {
  //-----------------init vars
  String path = 0;
  //-----------------analyze import inst
  for (; *i < entry_table.soco_tokens_count; (*i)++) {
    soco token_item = get_soco(2, *i);
    String Acode = token_item.code;
    Aline = token_item.line;
//    printf("gggg:%i,%i,%s\n", *i, Aline, Acode);
    //---------finish loop
    if (str_equal(Acode, ";")) {
      if (path == 0) {
        print_error(Aline, "import_syntax_error", entry_table.cur_ascii_source_path, "", "",
                    "manage_import_keywords:173");
        return;
      } else {

        //analyze type of import
        utst ret = get_utst_by_string(path);
        if (ret.id == 0) {
          path = str_reomve_quotations(path, "s");
          ret.id = ++entry_table.utf8_strings_id;
          utf8_str_init(&ret.utf8_string, utf8_encode_bytes(validate_path(path)));
          ret.max_bytes_per_char = utf8_str_max_bytes(ret.utf8_string, false);
          append_utst(ret);
        } else {
          ret.utf8_string = utf8_encode_bytes(validate_path(utf8_to_bytes_string(ret.utf8_string)));
        }

//        printf("WWWW:%s;%s;%s\n", project_root, utf8_to_bytes_string(ret.utf8_string), path);
        uint8 import_type = 0;
        String tmp1 = utf8_to_str(ret.utf8_string);
        tmp1 = str_trim_space(tmp1);
        if (str_indexof(tmp1, "file:", 0) == 0) {
          import_type = IMPORT_FILE;
          ret.utf8_string = utf8_str_simple_replace(ret.utf8_string, "file:", 0, 1);
        } else if (str_indexof(tmp1, "mod:", 0) == 0) {
          import_type = IMPORT_MODULE;
          ret.utf8_string = utf8_str_simple_replace(ret.utf8_string, "mod:", 0, 1);
          #if WINDOWS_PLATFORM == 1
          if (!str_has_suffix(tmp1, ".dll"))
            ret.utf8_string = utf8_str_append(ret.utf8_string, utf8_encode_bytes(".dll"));
          #elif LINUX_PLATFORM == 1
          //TODO
          #endif
        } else {
          print_error(Aline, "import_not_support_protocol", entry_table.cur_ascii_source_path, tmp1, "",
                      "manage_import_keywords");
          return;
        }

        String abspath = convert_mplpath_to_abspath(utf8_to_bytes_string(ret.utf8_string));
        ret.utf8_string = utf8_encode_bytes(abspath);
//        String StrSep = char_to_str(OS_SEPARATOR);
//        //replace project_root by $ sign
//        ret.utf8_string = utf8_str_simple_replace(ret.utf8_string, "$", project_root, 1);
//        //replace mpl_root by $$ sign
//        ret.utf8_string = utf8_str_simple_replace(ret.utf8_string, "$$", interpreter_path, 1);
//        //replace mpl_modules by @ sign
//        ret.utf8_string = utf8_str_simple_replace(ret.utf8_string,
//                                                  "@",
//                                                  str_multi_append(interpreter_path, StrSep, "modules", 0, 0, 0),1);
//        //replace mpl_packs by @@ sign
//        ret.utf8_string = utf8_str_simple_replace(ret.utf8_string,
//                                                  "@", str_multi_append(interpreter_path, StrSep, "packs", 0, 0, 0), 1);
        imin tmp2 = {entry_table.import_id++, import_type, true, ret.utf8_string, ret.max_bytes_per_char, 0,
            Aline, Apath};
        append_imin(tmp2);

      }
      break;
    }
      //---------set value
    else if (path == 0) {
      str_init(&path, Acode);
    }
    //---------
  }
}

//**********************************
void manage_structs(uint32 *i) {
  //-----------------init vars
  Boolean is_par = false, is_bra = false;
  String buf = 0, name = 0, Acode = 0, params = 0;
  str_list parameters = 0;
  uint32 params_len = 0;
  //-----------------analyze blocks header
  for (; *i < entry_table.soco_tokens_count; (*i)++) {
    soco token_item = get_soco(2, *i);
    uint32 next = (*i) + 1;
    str_init(&Acode, token_item.code);
    if (str_equal(Acode, ";"))continue;
    //printf("SSSS:%s,%s\n", Acode,buf);
    Aline = token_item.line;
    //----is_bra,is_par
    if (str_equal(Acode, "{")) {
      if (name == 0) {
        print_error(Aline, "not_exist_struct_name", entry_table.cur_ascii_source_path, 0, 0, "manage_structs");
        break;
      }
      parse_pars++;
      is_par = true;
      continue;
    } else if (str_equal(Acode, "}")) {
      parse_pars--;
      is_par = false;
    } else if (str_equal(Acode, "[")) is_bra = true;
    else if (str_equal(Acode, "]")) is_bra = false;
    //----get name
    if (name == 0) {
      str_init(&name, Acode);
      continue;
    }
    //----get parameters
    if (name != 0 && !is_bra && ((is_par && str_equal(Acode, ",")) || (!is_par && str_equal(Acode, "}")))) {
      params = str_append(params, buf);
      if (is_par)params = char_append(params, ',');
    }
    //----append to buf
    if (is_par && !is_bra && str_equal(Acode, ",")) {
      buf = 0;
    } else {
      buf = str_append(buf, Acode);
      if (next < entry_table.soco_tokens_count && !str_equal(Acode, ",") &&
          !str_equal(get_soco(2, next).code, ",")) {
        buf = char_append(buf, ' ');
      }
    }
    //----finish
    if (str_equal(Acode, "}")) {
      break;
    }
  }
  //printf("WW####:%s,%s\n", name, params);
  //-----------------analyzing func params
  if (cur_stru_id > 0) {
    print_error(Aline, "not_defined_struct", entry_table.cur_ascii_source_path, name, 0, "manage_structs");
    return;
  }
  def_var_s main_params[MAX_PARAMS_STRUCT];
  uint8 vars_counter = define_vars_analyzing(params, main_params);
  if (vars_counter == 0) {
    parameters = 0;
    params_len = 0;
    //TODO:error
  } else {
    for (uint32 i = 0; i < vars_counter; i++) {
      //-------check for syntax errors
      if (!str_is_empty(main_params[i].value_var)) {
        //TODO:error
      }
      String val = str_multi_append(main_params[i].main_type, ";", main_params[i].name_var, ";",
                                    main_params[i].index_var, 0);

      str_list_append(&parameters, val, params_len++);
      //printf("FFFFFFFF:%s=>%i\n",val,vars_counter);
    }
  }
  //TODO:error for same functions
  //printf("FUNC %s(%s)\n", name, params);
  //-----------------append to blst_func
  datas tmp1 = {0, cur_func_id, name, STRUCT_DATA_TYPE, parameters, params_len};
  append_datas(tmp1);
}

//**********************************
void manage_normal_instructions(uint32 *i) {
  //-----------------init vars
  String code = 0, last = 0;
  uint32 order = 0;
  //-----------------analyze instruction
  for (; *i < entry_table.soco_tokens_count; (*i)++) {
    uint32 ind = *i;
    soco token_item = get_soco(2, ind);
    String Acode = token_item.code;
    Aline = token_item.line;
    //----simi exceptions
    //func() [0]
    if (str_ch_equal(Acode, ';') && ind - 1 > 0 && ind + 1 < entry_table.soco_tokens_count &&
        str_ch_equal(last, ')') && str_ch_equal(get_soco(2, ind + 1).code, '['))
      continue;
      //----finish
    else if (str_ch_equal(Acode, ';')) break;
    //----append to code
    code = str_append(code, Acode);
    if (ind + 1 < entry_table.soco_tokens_count && !char_search(splitters, Acode[0]) &&
        !char_search(splitters, get_soco(2, ind + 1).code[0])) {
      code = char_append(code, ' ');
    }
    str_init(&last, Acode);
  }
  //-----------------
  order = get_order(cur_func_id, cur_stru_id);
  set_order(cur_func_id, cur_stru_id, ++order);
  //-----------------trim code
  code = trim_instruction_code(code);
  //-----------------append to instru
  instru tmp1 = {0, cur_func_id, cur_stru_id, order, code, UNKNOWN_LBL_INST, Aline, Apath, 0};
//  printf("XXSSSS:fid:%i,sid:%i,line:%i=>%s\n", cur_func_id, cur_stru_id, tmp1.line, code);
  append_instru(tmp1);
//  printf("PPPP:%li,%s(%li,%li,%li)\n",order,code,pid,fid,sid);
}
//**********************************
String trim_instruction_code(String code) {
  //------------------init vars
  Boolean is_string = false;
  String ret = 0;
  uint32 len = str_length(code);
  //------------------start analyzing
  for (uint32 i = 0; i < len; i++) {
    //------------------check is string
    if (code[i] == '\"' && (i == 0 || code[i - 1] != '\\')) {
      is_string = switch_bool(is_string);
    }
    //------------------continue if ' ' is before
    if (!is_string && i + 1 < len && code[i] == ' '
        && (char_search(single_operators, code[i + 1]) || char_search(words_splitter, code[i + 1]))) {
      continue;
    }
      //------------------continue if ' ' is after
    else if (!is_string && i - 1 >= 0 && code[i] == ' '
        && (char_search(single_operators, code[i - 1]) || char_search(words_splitter, code[i - 1]))) {
      continue;
    }
    //------------------append to ret
    ret = char_append(ret, code[i]);
  }
  //------------------return
//  printf("@Code:%s=>%s$\n", code, ret);
  return ret;
}
//**********************************
void manage_functions(uint32 *i) {
  //-----------------init vars
  Boolean is_par = false;
  String buf = 0, params = 0, name = 0;
  str_list parameters = 0;
  uint32 params_len = 0;
  uint8 pars = 0;
  //-----------------analyze blocks header
  for (; *i < entry_table.soco_tokens_count; (*i)++) {
    soco token_item = get_soco(2, *i);
    uint32 next = (*i) + 1;
    buf = token_item.code;
    //printf("SSSS:%s\n", buf);
    Aline = token_item.line;
    if (str_equal(buf, ";"))continue;
    //----is_bra,is_par
    if (str_equal(buf, "("))pars++;
    else if (str_equal(buf, ")")) pars--;
    //----get name
    if (name == 0) {
      str_init(&name, buf);
      continue;
    }
    //----get parameters
    if (name != 0 && pars == 1 && str_equal(buf, "(")) {
      is_par = true;
      continue;
    }
    if (is_par && pars > 0) {
      params = str_append(params, buf);
      if (next < entry_table.soco_tokens_count && !str_equal(buf, ",") &&
          !str_equal(get_soco(2, next).code, ",")) {
        params = char_append(params, ' ');
      }
    } else if (is_par && pars == 0 && str_equal(buf, ")")) {
      is_par = false;
      break;
    }
  }
  //-----------------analyzing func params
  def_var_s main_params[MAX_VAR_ALLOC_INSTRUCTIONS];
  uint8 vars_counter = define_vars_analyzing(params, main_params);
  //printf("#############:%s\n",params);
  if (vars_counter == 0) {
    parameters = 0;
    params_len = 0;
  } else {
    for (uint32 i = 0; i < vars_counter; i++) {
      //-------check for syntax errors
      if (!str_is_empty(main_params[i].value_var)) {
        print_error(Aline, "param_def_val", entry_table.cur_ascii_source_path, main_params[i].name_var, name,
                    "manage_functions");
      }
      String val = str_multi_append(main_params[i].main_type, ";", main_params[i].name_var, ";",
                                    main_params[i].index_var, 0);

      str_list_append(&parameters, val, params_len++);
      //printf("FFFFFFFF:%s=>%i\n",val,vars_counter);
    }
  }
  //TODO:error for same functions
  //printf("FUNC %s(%s)\n", name, params);
  //-----------------append to blst_func
  blst tmp1 = {0, 0, 0, FUNC_BLOCK_ID, name, parameters, params_len, Aline, Apath, 0};
  append_blst(tmp1);
  cur_func_id = entry_table.func_id;
  cur_stru_id = 0;
}

//**********************************
void manage_structures(uint32 *i, String lbl) {
  //-----------------init vars
  int32 is_par = 0;
  uint8 type = LOOP_STRU_ID;
  String buf = 0;
  uint32 params_len = 0;
  str_list params = 0;
  if (str_equal(lbl, "if")) type = IF_STRU_ID;
  else if (str_equal(lbl, "elif")) type = ELIF_STRU_ID;
  else if (str_equal(lbl, "else")) type = ELSE_STRU_ID;
  else if (str_equal(lbl, "manage")) type = MANAGE_STRU_ID;
  else if (str_equal(lbl, "switch")) type = SWITCH_STRU_ID;

  //-----------------analyze structure
  if (type != ELSE_STRU_ID)
    for (; *i < entry_table.soco_tokens_count; (*i)++) {
      soco token_item = get_soco(2, *i);
      String Acode = token_item.code;
      Aline = token_item.line;
//      printf("IIO:%s\n", Acode);
      if (str_ch_equal(Acode, ';') && type == ELSE_STRU_ID) {
        break;
      }
      //----count pars
      if (str_ch_equal(Acode, '(')) {
        is_par++;
        if (is_par == 1)continue;
      }
      /*if (str_ch_equal(Acode, '{'))parse_pars++;
      else if (str_ch_equal(Acode, '}'))parse_pars--;*/
      //----count pars
      if (str_ch_equal(Acode, ')')) {
        is_par--;
        //break all
        if (is_par == 0) break;
      }
      //----append to buf
      buf = str_append(buf, Acode);
      if ((*i) + 1 < entry_table.soco_tokens_count && !char_search(splitters, Acode[0]) &&
          !char_search(splitters, get_soco(2, (*i) + 1).code[0])) {
        buf = char_append(buf, ' ');
      }
    }
  //-----------else type
  if (type == ELSE_STRU_ID) {
    str_list_append(&params, "null", params_len++);
  }
    //-----------other types
  else {
    buf = trim_instruction_code(buf); //trim code
    str_list_append(&params, buf, params_len++);
  }
//  printf("@@@DDDD:%s,%s\n", lbl, buf);
  //-----------
  uint32 cur_ind = *i;
  if (cur_ind + 2 <= entry_table.soco_tokens_count && !str_ch_equal(get_soco(2, cur_ind + 1).code, '{') &&
      !str_ch_equal(get_soco(2, cur_ind + 2).code, '{')) {
    is_inline_stru = true;
  }
  //printf("XXXXXX:%s=>%s,%s\n", lbl, get_soco(2, cur_ind + 1).code, get_soco(2, cur_ind + 2).code);
  //-----------------
//  printf("OPPPP:lbl[%i]:%s,cur_sid:%i,params:%s,is_inline:%i(%s;%s)\n", type, lbl, cur_stru_id,print_str_list(params, params_len), is_inline_stru,get_soco(2, cur_ind + 1).code,get_soco(2, cur_ind + 2).code);

  //-----------------analyze loop stru
  if (type == LOOP_STRU_ID) {
    String buffer = 0, tmp = 0;
    str_init(&buffer, params[0]);
    params_len = 0;
    params = 0;
    Boolean is_str = false;
    uint32 blen = str_length(buffer);
    for (uint32 j = 0; j <= blen; j++) {
      if (j < blen && buffer[j] == '\"' && (j == 0 || buffer[j - 1] != '\\'))
        is_str = switch_bool(is_str);
      if (!is_str && (buffer[j] == ';' || j == blen)) {
        str_list_append(&params, tmp, params_len++);
        tmp = 0;
        continue;
      }
      tmp = char_append(tmp, buffer[j]);
    }
    if (params_len < 3)while (params_len < 3)str_list_append(&params, 0, params_len++);
    else if (params_len > 3) {
      //TODO:error
    }
    if (params[1] == 0) {
      //TODO:error
    }
//    printf("#WW:%i;;;%s;;;%s\n", params_len, print_str_list(params, params_len), buf);
  }
  //-----------------append to blst
  if (type == SWITCH_STRU_ID) {
    //TODO:convert switch to if,elif,else
  } else {
    long_int sid_t = cur_stru_id;
    //printf("BLOCK:%i\n", block_id);
    block_id++;
    if (block_id >= MAX_INTO_IN_STRUCTURES) {
      //TODO:error
      //block_id = 1;
      return;
    }
    //printf("CFFFF:%s,%s,%i\n",lbl, print_str_list(params, params_len), params_len);

    //-----------------append to blst
    String inst = str_multi_append(STRUCTURES_LABEL, str_from_long_int(entry_table.stru_id + 1), 0, 0, 0, 0);
    blst tmp1 = {0, cur_func_id, cur_stru_id, type, inst, params, params_len, Aline, Apath, 0};
    append_blst(tmp1);

    cur_stru_id = entry_table.stru_id;
    is_in_stru[block_id - 1].is_active = true;
    is_in_stru[block_id - 1].is_inline = false;
    is_in_stru[block_id - 1].id = cur_stru_id;
    is_in_stru[block_id - 1].stru_pars = parse_pars;
    //printf("PPP:%i,%s\n",block_id,lbl);
    //-----------------append to instru
    long_int order = get_order(cur_func_id, sid_t);
    set_order(cur_func_id, sid_t, ++order);

    instru tmp2 = {0, cur_func_id, sid_t, order, inst, STRUCTURE_LBL_INST, Aline, Apath, 0};
    append_instru(tmp2);
    //printf("GGGGGGGGGGGG:%s\n",inst);
  }
}

//**********************************
long_int get_last_id_active_is_in_stru() {
  if (block_id == 0)return 0;
  uint8 c = block_id;
  for (;;) {
    c--;
    if (is_in_stru[c].is_active) {
      return is_in_stru[c].id;
    }
    if (c == 0) break;
  }
  return 0;
}

//**********************************
void get_all_tokens() {
  Boolean is_string = false, is_valid_func = false;
  String word = 0;
  int8 pars = 0, acol = 0, bras = 0; //pars:(),acol:{},bras:[]
  for (uint32 ii = 0; ii < entry_table.soco_main_count; ii++) {
    soco source_code = get_soco(1, ii);
    String inst = source_code.code;
    uint32 line = source_code.line;
    //printf("line:%i,code:%s,path:%s\n",line,inst,(String)cur_ascii_source_path);
    acol = 0;
    for (uint32 i = 0; i < str_length(inst); i++) {
      //----------------check is str
      if (inst[i] == '\"' && (i == 0 || inst[i - 1] != '\\')) {
        is_string = switch_bool(is_string);
      }
      //----------------count pars
      if (!is_string && inst[i] == '(') {
        pars++;
        if (pars == 1)
          is_valid_func = false;
        if (pars == 1 && entry_table.soco_tokens_count > 0 && is_valid_name(word, false)) {
          //msg("~~!!~~CCCC:", word)
          is_valid_func = true;
        }
      } else if (!is_string && inst[i] == ')') {
        pars--;
      }
        //----------------count acol
      else if (!is_string && inst[i] == '{') {
        acol++;
      } else if (!is_string && inst[i] == '}') {
        acol--;
      }
        //----------------count bras
      else if (!is_string && inst[i] == '[') {
        bras++;
      } else if (!is_string && inst[i] == ']') {
        bras--;
      }
      //------------------append to buffer and word
      if (!is_string && (char_search(splitters, inst[i]) || i + 1 == str_length(inst))) {
        Boolean is_simi = false;
        if (i + 1 == str_length(inst) && !char_search(splitters, inst[i])) {
          word = char_append(word, inst[i]);
          is_simi = true;
        }
        if (pars == 0 && is_valid_func && (inst[i] == ')')) {
          is_simi = true;
          is_valid_func = false;
          //msg("SSSSS:", word)
        }
        if (i + 1 == str_length(inst) && (inst[i] == ')' || inst[i] == '}' || inst[i] == ']') && pars == 0 &&
            acol == 0) {
          is_simi = true;
        }
        //**********************
        word = str_trim_space(word);
        if (word != 0) {
          soco tmp = {line, word};
          append_soco(2, tmp);
        }
        //**********************
        if ((inst[i] != ' ' && i + 1 != str_length(inst)) ||
            (i + 1 == str_length(inst) && char_search(splitters, inst[i]))) {
          soco tmp = {line, char_to_str(inst[i])};
          append_soco(2, tmp);
        }
        //**********************
        if (is_simi) {
          soco tmp = {line, ";"};
          append_soco(2, tmp);
        }
        //**********************
        word = 0;
        continue;
      } else {
        word = char_append(word, inst[i]);
      }
    }
  }
  //*******************fix else inst
  /*for i:=0;i<len(token_words);i++{
      if i+1<len(token_words)&&token_words[i].token=="else"&&token_words[i+1].token=="{"{
          var tmp1 []token_words_struct
          }
  }*/
  //*******************print all
  /*for i := 0; i < len(token_words); i++ {
      msg("&CCCC", token_words[i].token, token_words[i].line)
  }*/
  //msg("&\n\n")
}

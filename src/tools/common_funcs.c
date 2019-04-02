#include <MPL/system.h>

//******************************************
String convert_mplpath_to_abspath(String mplpath) {
  //init vars
  String StrSep = char_to_str(OS_SEPARATOR);
  String ret = str_trim_space(mplpath);
  //----------------------------
  //=>replace mpl_root by $$ sign
  if (str_indexof(ret, "$$", 0) == 0)
    ret = str_replace(ret, "$$", interpreter_path, 1);
    //=>replace project_root by $ sign
  else if (str_indexof(ret, "$", 0) == 0)
    ret = str_replace(ret, "$", project_root, 1);
    //=>replace mpl_packs by @@ sign
  else if (str_indexof(ret, "@@", 0) == 0)
    ret = str_replace(ret, "@@",
                      str_multi_append(interpreter_path, StrSep, "packs", 0, 0, 0), 1);
    //=>replace mpl_modules by @ sign
  else if (str_indexof(ret, "@", 0) == 0)
    ret = str_replace(ret, "@",
                      str_multi_append(interpreter_path, StrSep, "modules", 0, 0, 0), 1);
    //=>if not has any signs
  else
    ret = str_multi_append(__syscall_homedir(), StrSep, ret, 0, 0, 0);

  return ret;

}
//******************************************
String get_mpl_dir_path() {
  String out = 0;
  #if LINUX_PLATFORM == 1

  #elif WINDOWS_PLATFORM == 1
  uint8 ownPth[MAX_PATH];
  HMODULE hModule = GetModuleHandle(NULL);
  if (hModule != NULL) {
    // Use GetModuleFileName() with module handle to get the path
    GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
    str_list entries = 0;
    uint32 size = char_split(ownPth, OS_SEPARATOR, &entries, true);
    for (uint32 i = 0; i < size - 1; i++) {
      out = str_append(out, entries[i]);
      if (i + 1 < size - 1)out = char_append(out, OS_SEPARATOR);
    }
  }
  #endif
  return out;
}

//******************************************
Boolean switch_bool(Boolean b) {
  if (b) return false;
  return true;
}

//******************************************
void msg(String format, ...) {
  String *argvs = (String *) &format;
  argvs++;
  uint32 c = 0;
  String str;
  uint32 u32;
  uint8 u8;
  while ((c = *format++) != 0) {
    if (c != '%') {
      //dbg_putc(c);
      printf("%c", c);
    } else {
      c = *format++;
      switch (c) {
        case 'c': u8 = *(String) argvs++;
          printf("%c", c);
          break;
        case 's':
          //str_init(&str,*argvs++);
          str = (String) argvs++;
          if (!str) str_init(&str, "[NULL]");
          printf("%s", str);

          break;
        case 'b': u32 = *(int *) argvs++;
          if (u32 == false)
            printf("FALSE");
          else
            printf("TRUE");
          break;
        case 'i': printf("%i", *(int32 *) (argvs++));
          break;
          //default:
          //printf();

      }
    }
  }
}

//******************************************
long_int longint_list_delete_first(longint_list *s, uint32 len) {
  if (len == 0) {
    (*s) = 0;
    return 0;
  }
  longint_list tmp = 0;
  long_int ret = 0;
  longint_list_init(&tmp, (*s), len);
  free((*s));
  (*s) = (longint_list) malloc((len - 1) * sizeof(longint_list));
  uint32 ind = 0;
  for (uint32 i = 1; i < len; i++) {
    //str_init(&(*s)[i],val[i]);
    (*s)[ind++] = tmp[i];
  }
  ret = tmp[0];
  free(tmp);
  return ret;
}

//******************************************
void longint_list_append(longint_list *s, uint32 len, long_int n) {
  longint_list tmp = 0;
  longint_list_init(&tmp, *s, len);
  //  free (*s);
  (*s) = (longint_list) malloc((len + 1) * sizeof(longint_list));
  if (*s == 0)return;
  for (uint32 i = 0; i < len; i++) {
    (*s)[i] = tmp[i];
  }
  (*s)[len] = n;
}

//*************************************************************
uint32 longint_list_join(longint_list *s, longint_list a1, uint32 a1_len, longint_list a2, uint32 a2_len) {
  //if(s!=0)free(*s);
  uint32 ind = 0;
  (*s) = (longint_list) malloc((a1_len + a2_len) * sizeof(longint_list));
  if (*s == 0)return 0;
  for (uint32 i = 0; i < a1_len; i++) {
    (*s)[ind++] = a1[i];
  }
  for (uint32 i = 0; i < a2_len; i++) {
    (*s)[ind++] = a2[i];
  }
  return ind;
}

//*************************************************************
void longint_list_init(longint_list *s, longint_list val, uint32 len) {
  if (len == 0) {
    (*s) = 0;
    return;
  }
  (*s) = (longint_list) malloc(len * sizeof(longint_list));
  for (uint32 i = 0; i < len; i++) {
    //str_init(&(*s)[i],val[i]);
    (*s)[i] = val[i];
  }
}

//*************************************************************
String longint_list_print(longint_list s, uint32 len) {
  if (len == 0) return "[NULL]";
  String ret = 0;
  ret = char_append(ret, '{');
  for (uint32 i = 0; i < len; i++) {
    //printf("%s,%i\n",s[i],str_length(s[i]));
    ret = str_append(ret, str_from_long_int(s[i]));
    if (i + 1 < len) ret = char_append(ret, ',');
  }
  ret = char_append(ret, '}');
  return ret;
}

//******************************************
double int32_power(double base, int32 power) {
  double ret = 1;
  Boolean is_neg = false;
  if (power < 0) {
    is_neg = true;
    power *= -1;
  }
  for (uint32 b = 0; b < power; b++) {
    ret *= base;
  }
  if (is_neg) {
    ret = (double) 1 / ret;
  }
  return ret;
}

//******************************************

//******************************************
//******************************************
String print_str_list(str_list s, uint32 len) {
  if (len == 0) return "[NULL]";
  String ret = 0;
  ret = char_append(ret, '{');
  for (uint32 i = 0; i < len; i++) {
    //printf("%s,%i\n",s[i],str_length(s[i]));
    ret = str_append(ret, s[i]);
    if (i + 1 < len) ret = char_append(ret, ',');
  }
  ret = char_append(ret, '}');
  return ret;
}

//******************************************
void print_struct(uint8 which) {
  if (which == 0 || which == PRINT_IMPORT_ST) {
    imin *tmp1 = entry_table.import_start;
    printf("=====Print import_inst_struct :\n");
    for (;;) {
      printf("Active:%i,id:%li,type:%i,line:%i,source:[%i]\n", tmp1->is_active, tmp1->id, tmp1->type, tmp1->line,
             tmp1->source_id);
      utf8_str_print("path:", tmp1->path, true);
      utf8_str_print("source:", source_paths[tmp1->source_id], true);
      printf("\n");
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_EXCEPTIONS_ST) {
    exli *tmp1 = entry_table.exli_start;
    printf("=====Print exceptions_list_struct :\n");
    for (;;) {
      printf("id:%i,type:%i,group:%s,lbl:%s,text:%s\n", tmp1->id, tmp1->type, exceptions_group[tmp1->group],
             tmp1->lbl, tmp1->text);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_UTF8_ST) {
    utst *tmp1 = entry_table.utst_start;
    if (tmp1 == 0) return;
    printf("=====Print utf8_strings_struct :\n");
    for (;;) {
      printf("id:%li,line:%i,max_bytes:%i, ", tmp1->id, tmp1->line, tmp1->max_bytes_per_char);
      utf8_str_print("string", tmp1->utf8_string, true);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_MAIN_SOURCE_ST) {
    soco *tmp1 = entry_table.soco_main_start;
    if (tmp1 == 0) return;
    printf("=====Print source_code_struct :\n");
    for (;;) {
      printf("line:%i,code:%s\n", tmp1->line, tmp1->code);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_TOKENS_SOURCE_ST) {
    soco *tmp1 = entry_table.soco_tokens_start;
    if (tmp1 == 0) return;
    printf("=====Print source_code_tokens_struct :\n");
    for (;;) {
      printf("id:%i,code:%s\n", tmp1->line, tmp1->code);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_FUNC_ST) {
    blst *tmp1 = entry_table.blst_func_start;
    if (tmp1 == 0) return;
    printf("=====Print func_block_struct :\n");
    for (;;) {
      printf("id:%li,name:%s,params:%s\n", tmp1->id, tmp1->lbl, print_str_list(tmp1->params, tmp1->params_len));
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_INSTRU_ST) {
    instru *tmp1 = entry_table.instru_start;
    if (tmp1 == 0) return;
    printf("=====Print instructions_struct :\n");
    for (;;) {
      printf("(id:%li,fid:%li,sid:%li,order:%li,type:%i,line:%i),code:%s\n\tfile:%s[%i]\n",
             tmp1->id,
             tmp1->func_id,
             tmp1->stru_id,
             tmp1->order,
             tmp1->type,
             tmp1->line,
             tmp1->code,
             utf8_to_bytes_string(source_paths[tmp1->source_id]), tmp1->line);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_STRUCT_ST) {
    datas *tmp1 = entry_table.datas_start;
    if (tmp1 == 0) return;
    printf("=====Print struct_struct :\n");
    for (;;) {
      printf("(id:%li,fid:%li,len:%i),name:%s,params:%s\n", tmp1->id, tmp1->fid, tmp1->params_len, tmp1->name,
             print_str_list(tmp1->params, tmp1->params_len));
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_STRU_ST) {
    blst *tmp1 = entry_table.blst_stru_start;
    if (tmp1 == 0) return;
    printf("=====Print block_structure_struct :\n");
    for (;;) {
      printf("id:%li,lbl[%i]:%s,(fid:%li,sid:%li),argvs:%s\n", tmp1->id, tmp1->type, tmp1->lbl, tmp1->func_id,
             tmp1->stru_id, print_str_list(tmp1->params, tmp1->params_len));
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_STRUCT_DES_ST) {
    stde *tmp1 = entry_table.stde_start;
    if (tmp1 == 0) return;
    printf("=====Print struct descriptor :\n");
    for (;;) {
      printf("%i(%s):\n", tmp1->id, tmp1->type);
      print_vaar(tmp1->st);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_MAGIC_MACROS_ST) {
    mama *tmp1 = entry_table.mama_start;
    if (tmp1 == 0) return;
    printf("=====Print magic macros :\n");
    for (;;) {
      printf("[id:%i,type:%i]%s;%s:%s\n", tmp1->id, tmp1->type, tmp1->value_type, tmp1->key, tmp1->value);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_FUNCTIONS_STACK_ST) {
    fust *tmp1 = entry_table.fust_start;
    if (tmp1 == 0) return;
    printf("=====Print functions stack :\n");
    for (;;) {
      printf("fid:%i,fin:%i,Pfin:%i,sid:%i,order:%i\n", tmp1->fid, tmp1->fin, tmp1->parent_fin, tmp1->sid, tmp1
          ->order);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_STRUCTURES_STACK_ST) {
    stst *tmp1 = entry_table.stst_start;
    if (tmp1 == 0) return;
    printf("=====Print structures stack :\n");
    for (;;) {
      printf("type:%i,fid:%i,fin:%li,sid:%li,Psid:%li,order:%li,extra:%s\n",
             tmp1->type,
             tmp1->fid,
             tmp1->fin,
             tmp1->sid,
             tmp1->parent_sid,
             tmp1
                 ->order,
             tmp1->extra);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_CONDITION_LEVEL_ST) {
    cole *tmp1 = entry_table.cole_start;
    if (tmp1 == 0) {
      printf("(null) condition level\n");
      return;
    }
    printf("=====Print condition level :\n");
    for (;;) {
      printf("[id:%i,fin:%i,sid:%i]:%i\n", tmp1->id, tmp1->fin, tmp1->sid, tmp1->is_complete);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  } else if (which == 0 || which == PRINT_LOOP_LEVEL_ST) {
    lole *tmp1 = entry_table.lole_start;
    if (tmp1 == 0) {
      printf("(null) loop level\n");
      return;
    }
    printf("=====Print loop level :\n");
    for (;;) {
      printf("id:%i,fin:%i,sid:%i\n", tmp1->id, tmp1->fin, tmp1->sid);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
    printf("=====End printed\n");
  }
}
//*************************************************************
void print_vaar(vaar_en s) {
  vaar *tmp1 = s.start;
  if (tmp1 == 0) {
    printf("--NULL-- Vars_Array_struct\n");
    return;
  }
  printf("=====Print Vars_Array_struct :\n");
  if (tmp1 != 0) {
    for (;;) {
      printf("id:%i,%type:%c,index:%s,value:%s\n", tmp1->data_id, tmp1->sub_type, tmp1->index, tmp1->value);
      tmp1 = tmp1->next;
      if (tmp1 == 0) break;
    }
  }
  printf("=====End printed\n");
}
//*************************************************************
uint32 source_paths_search(str_utf8 path) {
  for (uint32 i = 0; i < entry_table.source_counter; i++) {
    if (utf8_str_equal(path, source_paths[i]))
      return i;
  }
  return 0;
}
//*************************************************************
String validate_path(String s) {
  #if WINDOWS_PLATFORM == true
  String ret = 0;
  for (uint32 i = 0; i < str_length(s); i++) {
    if (s[i] == '/') {
      ret = char_append(ret, '\\');
      continue;
    }
    ret = char_append(ret, s[i]);
  }
  return ret;
  #elif LINUX_PLATFORM == true
  String ret = 0;
  for (uint32 i = 0; i < str_length(s); i++) {
    if (s[i] == '\\') {
      ret = char_append(ret, '/');
      continue;
    }
    ret = char_append(ret, s[i]);
  }
  return ret;
  #endif
  return s;
}

//*************************************************************
void str_list_append(str_list *s, String s1, uint32 len) {
  //printf("CCCC:%s,%i\n",s1,len);
  str_list tmp = 0;
  str_list_init(&tmp, *s, len);
  //if(*s!=NULL)free(*s);
  (*s) = (str_list) malloc((len + 1) * sizeof(str_list));
  if (*s == 0)return;
  for (uint32 i = 0; i < len; i++) {
    (*s)[i] = tmp[i];
  }
  (*s)[len] = str_append(0, s1);
  //printf("CCVVV:%s,%i\n",(*s)[len],str_length((*s)[len]));
}

//*************************************************************
void str_list_init(str_list *s, str_list val, uint32 len) {
  if (len == 0) {
    (*s) = 0;
    return;
  }
  free(*s);
  (*s) = (str_list) malloc(len * sizeof(str_list));
  for (uint32 i = 0; i < len; i++) {
    //str_init(&(*s)[i],val[i]);
    uint32 le = str_length(val[i]);
    (*s)[i] = (String) malloc((le + 1) * sizeof(String));
    for (uint32 ii = 0; ii < le; ii++) (*s)[i][ii] = val[i][ii];
    (*s)[i][le] = 0;
  }
}

//*************************************************************
void str_list_delete_first(str_list *s, uint32 len) {
  if (len == 0) {
    (*s) = 0;
    return;
  }
  str_list tmp = 0;
  str_list_init(&tmp, (*s), len);
  free((*s));
  (*s) = (str_list) malloc((len - 1) * sizeof(str_list));
  uint32 ind = 0;
  for (uint32 i = 1; i < len; i++) {
    //str_init(&(*s)[i],val[i]);
    str_init(&(*s)[ind++], tmp[i]);
  }
  free(tmp);
}

//*************************************************************
uint32 str_list_join(str_list *s, str_list a1, uint32 a1_len, str_list a2, uint32 a2_len) {
  //if(s!=0)free(*s);
  uint32 ind = 0;
  (*s) = (str_list) malloc((a1_len + a2_len) * sizeof(str_list));
  if (*s == 0)return 0;
  for (uint32 i = 0; i < a1_len; i++) {
    str_init(&(*s)[ind++], a1[i]);
  }
  for (uint32 i = 0; i < a2_len; i++) {
    str_init(&(*s)[ind++], a2[i]);
  }
  return ind;
}

//*************************************************************
Boolean str_list_equal(str_list s1, uint32 len1, str_list s2, uint32 len2) {
  if (len1 != len2) return false;
  if (len1 == 0)return true;
  for (uint32 i = 0; i < len1; ++i) {
    if (!str_equal(s1[i], s2[i]))return false;
  }
  return true;
}

//*************************************************************
void utf8_str_list_append(utf8_str_list *s, str_utf8 s1, uint32 len) {
  //printf("CCCC:%s,%i\n",s1,len);
  utf8_str_list tmp = 0;
  utf8_str_list_init(&tmp, *s, len);
  free(*s);
  (*s) = (utf8_str_list) malloc((len + 1) * sizeof(utf8_str_list));
  if (*s == 0)return;
  for (uint32 i = 0; i < len; i++) {
    (*s)[i] = tmp[i];
  }
  (*s)[len] = utf8_str_append(0, s1);
  //printf("CCVVV:%s,%i\n",(*s)[len],str_length((*s)[len]));
}

//*************************************************************
String remove_incorrect_pars(String str, int32 *invalid_pars) {
  String final = 0;
  str_init(&final, str);
  Boolean is_string = false;
  int32 pars = 0;
  for (uint32 i = 0; i < str_length(str); i++) {
    if (str[i] == '\"' && (i == 0 || str[i - 1] != '\\')) {
      is_string = switch_bool(is_string);
    }
    if (!is_string) {
      switch (str[i]) {
        case '(': pars++;
          break;
        case ')': pars--;
          break;
      }
    }
  }
  //*********************
  uint32 final_len = str_length(final);
  if (pars > 0 && pars < final_len) {
    final = str_substring(final, pars, 0);
  } else if (pars < 0 && (-pars) < final_len) {
    final = str_substring(final, 0, final_len - (-pars));
  }
  //msg("&***", pars)
  (*invalid_pars) = pars;
  return final;

}

//*************************************************************
FILE *utf8_file_open(String filename, String mode) {
  #if LINUX_PLATFORM == 1
  //TODO:utf8 fopen for linux
  #elif WINDOWS_PLATFORM == 1
  wchar_t wfile[260];
  wchar_t wmode[32];
  MultiByteToWideChar(CP_UTF8, 0, filename, -1, wfile, 260);
  MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, 32);
  return _wfopen(wfile, wmode);
  #endif
  return NULL;
}

//*************************************************************
String replace_in_expression(String exp, String rep, int32 start, int32 end, Boolean remove_pars, Boolean is_trim) {
  //msg("&@@@", exp, rep, start, end)
  String s1 = 0, s2 = 0, expression = 0;
  if (is_trim) {
    s1 = str_trim_space(str_substring(exp, 0, start));
    s2 = str_trim_space(str_substring(exp, end, 0));
  } else {
    s1 = str_substring(exp, 0, start);
    s2 = str_substring(exp, end, 0);
  }
  uint32 s1_len = str_length(s1);
  if (remove_pars && s1_len > 0 && s1[s1_len - 1] == '(' && s2[0] == ')') {
    s1 = str_substring(s1, 0, s1_len - 1);
    s2 = str_substring(s2, 1, 0);
  }
  expression = str_multi_append(s1, rep, s2, 0, 0, 0);
  return expression;
}

//*************************************************************
String make_valid_double(String s) {
  uint32 s_len = str_length(s);
  if (s_len == 0 || (s_len == 1 && s[0] == '.'))return "0";
  if (s_len > 1 && s[0] == '.')return str_multi_append("0", s, 0, 0, 0, 0);
  if (s_len > 1 && s[s_len - 1] == '.')return str_substring(s, 0, s_len - 1);
  return s;
}

//*************************************************************
String limit_decimal_huge_numbers(String s) {
  uint32 s_len = str_length(s);
  uint32 ind = 0, limit = 0;
  Boolean is_point = false;
  for (uint32 i = 0; i < s_len; i++) {
    if (s[i] == '.') {
      is_point = true;
      continue;
    }
    if (is_point) {
      limit++;
      if (limit == max_decimal_has_huge) {
        ind = i + 1;
        break;
      }
    }
  }
  if (ind == 0)ind = s_len;
  return str_substring(s, 0, ind);
}

//*************************************************************
Boolean is_equal_arrays_indexes(String s1, String s2) {
  //printf("#WWWWW:%s@@%s\n",s1,s2);
  if ((s1 == 0 && str_ch_equal(s2, '0')) || (s2 == 0 && str_ch_equal(s1, '0')))return true;
  if ((str_ch_equal(s1, '_') && str_ch_equal(s2, '0')) || (str_ch_equal(s2, '_') && str_ch_equal(s1, '0')))return true;
  str_list sl1 = 0, sl2 = 0;
  uint32 sl1_len = char_split(s1, ',', &sl1, true);
  uint32 sl2_len = char_split(s2, ',', &sl2, true);
  if (sl1_len != sl2_len)return false;
  for (uint32 i = 0; i < sl1_len; ++i) {
    if (str_ch_equal(sl1[i], '?') || str_ch_equal(sl2[i], '?'))continue;
    if (!str_equal(sl1[i], sl2[i]))return false;
  }
  return true;
}
//*************************************************************
Boolean has_two_limiting(String s, uint8 l1, uint8 l2, Boolean ignore_space) {
  if (ignore_space)s = str_trim_space(s);
  uint32 len = str_length(s);
  if (len < 2)return false;
  if (s[0] == l1 && s[len - 1] == l2)return true;
  return false;
}

//*************************************************************
String replace_control_chars(String val) {
  uint32 len = str_length(val);
  if (len < 2)return val;
  String ret = 0;
  for (uint32 i = 0; i < len; i++) {
    if (val[i] == '\\' && i + 1 < len) {
      if (val[i + 1] == 'n')ret = char_append(ret, '\n');
      else if (val[i + 1] == 't') ret = char_append(ret, '\t');
      else if (val[i + 1] == '\"') ret = char_append(ret, '\"');
      else if (val[i + 1] == '\'') ret = char_append(ret, '\'');
      else if (val[i + 1] == '\\') ret = char_append(ret, '\\');
        //backspace
      else if (val[i + 1] == 'b') ret = char_append(ret, '\b');//ret = char_backspace (ret);
        //alert
      else if (val[i + 1] == 'a') ret = char_append(ret, '\a');
      i++;
    } else ret = char_append(ret, val[i]);
  }
  return ret;

}
//*************************************************************
String set_valid_control_chars(String val) {
  uint32 len = str_length(val);
  if (len < 1)return val;
  String ret = 0;
  for (uint32 i = 0; i < len; i++) {
//    printf("Dsfsdgdrgh:%c\n",val[i]);
    if ((i == 0 || val[i - 1] != '\\')
        && (val[i] == '\n' || val[i] == '\t' || val[i] == '\"' || val[i] == '\'' || val[i] == '\\' || val[i] == '\b'
            || val[i] == '\a')) {

      if (val[i] == '\n')ret = str_append(ret, "\\n");
      else if (val[i] == 't') ret = str_append(ret, "\\t");
      else if (val[i] == '\"') ret = str_append(ret, "\\\"");
      else if (val[i] == '\'') ret = str_append(ret, "\\'");
      else if (val[i] == '\\') ret = str_append(ret, "\\\\");
        //backspace
      else if (val[i] == 'b') ret = str_append(ret, "\\b");
        //alert
      else if (val[i] == 'a') ret = str_append(ret, "\\a");
    } else ret = char_append(ret, val[i]);
  }
  return ret;

}
//*************************************************************
Boolean is_equal_data_types(uint8 t1, uint8 t2) {
  if (t1 == 0 || t2 == 0 || t1 == '0' || t2 == '0')return false;
  if (t1 == t2)return true;
  if ((t1 == 'i' || t1 == 'f' || t1 == 'h') && (t2 == 'i' || t2 == 'f' || t2 == 'h'))return true;
  return false;

}
//*************************************************************
int32 search_int32_array(int32 a[], uint32 al, int32 n) {
  if (al < 1)return -1;
  for (int32 i = 0; i < al; ++i) {
    if (a[i] == n)return i;
  }
  return -1;
}
//*************************************************************
Boolean delete_int32_element_array(int32 a[], uint32 al, int32 n, Boolean delete_last) {
  int32 tmp[al - 1];
  uint32 co = 0;
  Boolean is_find = false;
  if (delete_last) {
    co = al - 2;
    for (int32 i = al - 1; i >= 0; i--) {
      if (i < 0)break;
      if (a[i] == n && !is_find) is_find = true;
      else tmp[co--] = a[i];
    }
  } else
    for (int32 i = 0; i < al; ++i) {
      if (a[i] == n && !is_find) is_find = true;
      else tmp[co++] = a[i];
    }
  for (int32 i = 0; i < al - 1; ++i) {
    a[i] = tmp[i];
  }
  a[al - 1] = -1;
  return is_find;
}
//*************************************************************
uint8 convert_index_to_int32(String ind, int32 ret[], Boolean manage_ques) {
  str_list indexes = 0;
  uint8 indexes_len = 0;
  uint8 ret_len = 0;
  indexes_len = (uint8) char_split(ind, ',', &indexes, true);
  if (indexes_len > MAX_ARRAY_DIMENSIONS) {
    //TODO:error
    return 0;
  }
  for (uint32 i = 0; i < indexes_len; i++) {
    if (manage_ques && str_ch_equal(indexes[i], '?'))ret[ret_len++] = 1;
    else if (str_ch_equal(indexes[i], '?'))ret[ret_len++] = -1;
    else ret[ret_len++] = str_to_int32(indexes[i]);
  }
  return ret_len;
}
//*************************************************************

//*************************************************************
String find_first_var_name(String exp, uint32 start, Boolean is_inverse, uint32 *endpoint) {
  String ret = 0;
  String buf = 0;
  int32 bra = 0;
  //is inverse----------------------------------
  if (is_inverse) {
    for (uint32 i = start; i >= 0; i--) {
      if (i == 0) {
        buf = char_append(buf, exp[i]);
        (*endpoint) = 0;
        break;
      }
      if ((exp[i] == ']') && bra == 0) {
        bra--;
        buf = char_append(buf, exp[i]);
      } else if ((bra == 0) && exp[i] != '.' &&
          (char_search(single_operators, exp[i]) ||
              char_search(words_splitter, exp[i]) ||
              exp[i] == '\"')) {
        (*endpoint) = i;
        break;
      } else {
        if (exp[i] == '[')bra++;
        else if (exp[i] == ']')bra--;
        buf = char_append(buf, exp[i]);
      }
      //printf ("$$$$:%s=>%s(%i,%c)\n", buf, str_reverse (buf), i, exp[i]);
    }
    buf = str_reverse(buf);
    str_init(&ret, buf);
  }
    //not inverse----------------------------------
  else {
    uint32 len = str_length(exp);
    for (uint32 i = start; i < len; i++) {
      if ((exp[i] == '[') && bra == 0) {
        bra++;
        buf = char_append(buf, exp[i]);
      } else if ((bra == 0) && exp[i] != '.' &&
          (char_search(single_operators, exp[i]) ||
              char_search(words_splitter, exp[i]) ||
              exp[i] == '\"')) {
        (*endpoint) = i;
        break;
      } else {
        if (exp[i] == '[')bra++;
        else if (exp[i] == ']')bra--;
        buf = char_append(buf, exp[i]);
      }
    }
    str_init(&ret, buf);
  }
  return ret;
}
//*************************************************************
String format_int32_array(int32 s[], uint32 start, uint32 end) {
  String ret = "{";
  for (uint32 i = start; i < end; i++) {
    ret = str_append(ret, str_from_int32(s[i]));
    if (i + 1 < end)ret = char_append(ret, ',');
  }
  ret = char_append(ret, '}');
  return ret;
}

//*************************************************************
String return_type_structure(uint8 t) {
  if (t == IF_STRU_ID)return "if";
  if (t == ELIF_STRU_ID)return "elif";
  if (t == ELSE_STRU_ID)return "else";
  if (t == LOOP_STRU_ID)return "loop";
  if (t == MANAGE_STRU_ID)return "manage";
  if (t == SWITCH_STRU_ID)return "switch";
}
//*************************************************************
/**
 * get a valid path (relative or absolute) and return filename if exist and if ext not zero return file extension
 * @param path
 * @param ext
 * @return String
 */
String return_file_name_extension_path(String path, String *ext, Boolean must_ext) {
  str_list ret = 0;
  uint32 len = 0;
  int32 pos = char_last_indexof(path, OS_SEPARATOR);
  if (pos == -1) {
    len = char_split(path, '.', &ret, true);
    if (must_ext && len < 2)return 0;
  } else {
    String sub = str_substring(path, pos + 1, 0);
    len = char_split(sub, '.', &ret, true);
    if (must_ext && len < 2)return 0;
  }
  if (ext != 0 && len > 1) {
    str_list_delete_first(&ret, len--);
    (*ext) = char_join(ret, '.', len, true);
  }
//  printf("Filename:%s=>%s\n", path, ret[0]);
  return ret[0];
}
//*************************************************************
String convert_sub_type_to_type(uint8 sub_type) {
  if (sub_type == 0)return 0;
  if (sub_type == 'i' || sub_type == 'f' || sub_type == 'h')return "num";
  if (sub_type == 's' || sub_type == 'u')return "str";
  if (sub_type == 'b')return "bool";
  return 0;
}
//*************************************************************
Boolean has_suffix_for_array(String s, str_list ar, uint32 ar_len) {
  for (uint32 i = 0; i < ar_len; i++) {
    if (str_has_suffix(s, ar[i]))return true;
  }
  return false;
}
//*************************************************************
double calculate_period_time(long_int start_time, String *unit) {
  long_int during = (long_int) clock() - start_time;
  double time_taken = ((double) during) / CLOCKS_PER_SEC; // in seconds
  if (time_taken == 0) time_taken += 0.000001;
  if (time_taken >= 60) {
    time_taken /= 60;
    str_init(&(*unit), "minutes");
  } else str_init(&(*unit), "seconds");
  return time_taken;
}
//*************************************************************

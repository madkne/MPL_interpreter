
#include <MPL/system.h>

//******************************************
String read_input() {
    String ret = "";
    uint8 c = 0;
    for (;;) {
        c = fgetc(stdin);
        if (c == EOF || c == '\n'/*ENTER*/) break;
        ret = char_append(ret, c);
    }
    return ret;
}

//******************************************
String get_absolute_path(String path) {
    //uint8 resolved_path[PATH_MAX+1];
#if LINUX_PLATFORM == 1
    String abs_path=0;
    abs_path=realpath(path, 0);
    //printf("\n%s=>%s\n",path,abs_path);
    return abs_path;
#elif WINDOWS_PLATFORM == 1
    char tmp[BUFSIZE];
    String abs_path = 0;
    GetFullPathName(path, BUFSIZE, tmp, 0);
    str_init(&abs_path, tmp);
    if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(abs_path) && GetLastError() == ERROR_FILE_NOT_FOUND) return 0;
    //printf("\nSSS::%s=>%s\n",path,abs_path);
    return abs_path;
#endif
    return 0;
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
                case 'c':
                    u8 = *(String) argvs++;
                    printf("%c", c);
                    break;
                case 's':
                    //str_init(&str,*argvs++);
                    str = (String) argvs++;
                    if (!str) str_init(&str, "[NULL]");
                    printf("%s", str);

                    break;
                case 'b':
                    u32 = *(int *) argvs++;
                    if (u32 == false)
                        printf("FALSE");
                    else
                        printf("TRUE");
                    break;
                case 'i':
                    printf("%i", *(int32 *) (argvs++));
                    break;
                    //default:
                    //printf();

            }
        }
    }
}

//******************************************
long_int get_max_unsigned_size(uint8 bytes) {
    long_int ret = 1;
    for (uint8 i = 0; i < bytes; i++) {
        ret *= 250;
    }
    return ret;
}

//******************************************
uint32 long_int_append(long_int **s, long_int n) {
    uint32 len = long_int_size(*s);
    (*s) = (long_int *) malloc((len + 1) * sizeof(long_int *));
    if (*s == 0)return 0;
    (*s)[len] = n;
    return len + 1;
}

//******************************************
uint32 long_int_size(long_int *s) {
    if (s == 0)return 0;
    uint32 len = 0;
    for (;;)
        if (s[len++] == 0) break;
    return len - 1;
}

//******************************************
String get_current_datetime(uint8 type) {
    time_t t = time(NULL);
    struct tm tim = *localtime(&t);
    //char s[64];
    String ret = 0;
    //date and time : 2018-6-23 3:45:12
    if (type == 1) {
        ret = malloc(6 * sizeof(int) + 5);
        sprintf(ret, "%i-%i-%i %i:%i:%i", tim.tm_year + 1900, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min,
                tim.tm_sec);
    }

    //strftime(s, sizeof(s), "%c", tm);
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
            printf("id:%li,max_bytes:%i, ", tmp1->id, tmp1->max_bytes_per_char);
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
            printf("id:%i,code:%s\n", tmp1->line, tmp1->code);
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
            printf("(id:%li,fid:%li,sid:%li,order:%li,type:%i),code:%s\n", tmp1->id,
                   tmp1->func_id, tmp1->stru_id, tmp1->order, tmp1->type, tmp1->code);
            tmp1 = tmp1->next;
            if (tmp1 == 0) break;
        }
        printf("=====End printed\n");
    } else if (which == 0 || which == PRINT_STRU_ST) {
        blst *tmp1 = entry_table.blst_stru_start;
        if (tmp1 == 0) return;
        printf("=====Print block_structure_struct :\n");
        for (;;) {
            printf("id:%li,lbl[%i]:%s,(fid:%li,sid:%li),argvs:%s\n", tmp1->id, tmp1->type,tmp1->lbl, tmp1->func_id, tmp1->stru_id,
                   print_str_list(tmp1->params, tmp1->params_len));
            tmp1 = tmp1->next;
            if (tmp1 == 0) break;
        }
        printf("=====End printed\n");
    } else if (which == 0 || which == PRINT_VIRTUAL_MEMORY_ST) {
//        vias *tmp1=entry_table.vias_start;
//        if(tmp1==0) return;
//        printf("=====Print virtual_assemble_struct :\n");
//        for(;;){
//            printf("inst(%i):%s %s , %s ;%i\n",tmp1->segment,tmp1->inst,tmp1->op1,tmp1->op2,tmp1->description);
//            tmp1=tmp1->next;
//            if(tmp1==0) break;
//        }
//        printf("=====End printed\n");
    }
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
utst return_utf8_string_value(String s) {
    utst nil = {0, 0, 0};
    uint32 len = str_length(s);
    String val = 0;
    if (len > 2 && s[0] == '\"' && s[len - 1] == '\"') {
        val = str_substring(s, 1, len - 1);
    } else {
        str_init(&val, s);
    }
    //------------
    // printf("OOOOO:%s,%s,%i\n", s, val, str_is_int32(val));
    if (str_is_int32(val)) {
        int32 utst_id = str_to_int32(val);
        //printf("OOOOOO:%i\n", utst_id);
        utst ut = get_utst((long_int) utst_id);//TODO:
        return ut;
        //printf("UTT:%s=>%s,%i\n", s, utf8_to_unicode_str(ut.utf8_string), ut.max_bytes_per_char);
        //utf8_str_print("UTF8", ut.utf8_string, false);
    } else
        return nil;

}

//*************************************************************
void str_list_append(str_list *s, String s1, uint32 len) {
    //printf("CCCC:%s,%i\n",s1,len);
    str_list tmp = 0;
    str_list_init(&tmp, *s, len);
    free(*s);
    (*s) = (str_list) malloc((len + 1) * sizeof(str_list));
    if (*s == 0)return;
    for (uint32 i = 0; i < len; i++) {
        (*s)[i] = tmp[i];
    }
    (*s)[len] = str_append(0, s1);
    //printf("CCVVV:%s,%i\n",(*s)[len],str_length((*s)[len]));
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
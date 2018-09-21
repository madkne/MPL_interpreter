//
// Created by amindelavar on 9/19/2018.
//
#include<MPL/system.h>

void init_exceptions_list_data(){
    define_new_exception(0,FATAL_ID, 0, 0, 0);
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
    define_new_exception(1,FATAL_ID, "bad_exit", CommandError, "returned 1 exit status");
    define_new_exception(2,ERROR_ID, "unknown_opt", CommandError, "unknown option '!1@1!'");
    define_new_exception(3,FATAL_ID, "not_exist_file", CommandError, "can't open file '!1@1!',No such file.");
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//MahlibError
    define_new_exception(1,FATAL_ID, "lib_not_find_dir", MahlibError, "can't find or access to '!1@1!'");
    define_new_exception(2,FATAL_ID, "lib_not_find_config", MahlibError, "can't find 'info.config' in '!1@1!'");
    define_new_exception(3,ERROR_ID, "lib_not_analyze_file", MahlibError, "can't analyzing '!1@1!' file");
    define_new_exception(4,ERROR_ID, "lib_not_create_bin", MahlibError, "can't create binary file in '!1@1!'");
    define_new_exception(5,ERROR_ID, "lib_not_read_bin", MahlibError, "can't read binary file in '!1@1!'");
    define_new_exception(6,ERROR_ID, "lib_not_create_mahlib", MahlibError, "can't create mahlib file in '!1@1!'");
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//ImportError
    define_new_exception(1,ERROR_ID, "bad_place_using_utf8", ImportError, "using UTF-8 in bad place or bad character ");
    define_new_exception(2,ERROR_ID, "not_open_file", ImportError, "can't open file '!1@1!'");
    define_new_exception(3,ERROR_ID, "file_not_support_utf8", ImportError, "'!1@1!' file not support UTF-8");
    define_new_exception(4,ERROR_ID, "import_syntax_error", ImportError, "A syntax error occurs for '!1@1!' import instruction");
    define_new_exception(5,ERROR_ID, "import_not_support_protocol", ImportError, "not support the protocol in this "
                                                                                 "path '!1@1!' "
                                                                                 "import instruction");
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//InterruptedError
    define_new_exception(1,ERROR_ID, "zero_division", InterruptedError, "division by zero in '!1@1!' expression");
    define_new_exception(2,ERROR_ID, "wrong_remainder", InterruptedError, "remainder(%%) is not determined for '!1@1!' numbers and expressions");
    define_new_exception(3,ERROR_ID, "out_of_range_index", InterruptedError, "'!1@1!' out of range for '!2@2!' variable");
    define_new_exception(4,ERROR_ID, "out_of_range_break_next", InterruptedError, "'!1@1!' is not between 1 .. !2@2!");

    define_new_exception(5,ERROR_ID, "review_array_out_of_range", InterruptedError, "review array can not assign '!1@1!' to more than '!2@2!' variables");
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//SyntaxError
    define_new_exception(1,ERROR_ID, "non_dec_stat", SyntaxError, "'!1@1!' is non-declaration statement outside a body");
    define_new_exception(2,FATAL_ID, "invalid_name_block", SyntaxError, "'!1@1!' is not a valid name for a function or package");
    define_new_exception(3,FATAL_ID, "redeclared_package", SyntaxError, "'!1@1!' redeclared as a package before");
    define_new_exception(4,FATAL_ID, "param_def_val", SyntaxError, "'!1@1!' as a '!2@2!' function parameter can not have any default value");
    define_new_exception(5,ERROR_ID, "reiterative_attr", SyntaxError, "'!1@1!' reiterative before as a attribute");
    define_new_exception(6,FATAL_ID, "not_end_acod", SyntaxError, "expected '}' at end of input");
    define_new_exception(7,FATAL_ID, "not_start_acod", SyntaxError, "expected '{' at start of input");
    define_new_exception(8,ERROR_ID, "not_used_attr", SyntaxError, "'!1@1!' attribute is not used for !2@2!");
    define_new_exception(9,FATAL_ID, "define_func_in", SyntaxError, "'!1@1!' is defined into another function");

    define_new_exception(10,ERROR_ID, "redeclared_var", SyntaxError, "'!1@1!' redeclared as a variable before in this function or package");
    define_new_exception(11,ERROR_ID, "redeclared_func", SyntaxError, "'!1@1!' redeclared as a function before with same parameters");
    define_new_exception(12,ERROR_ID, "wrong_def_var", SyntaxError, "'!1@1!' is a wrong expression for variable declaration");
    define_new_exception(13,ERROR_ID, "invalid_name_var", SyntaxError, "'!1@1!' is not a valid name for a variable");
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//ValueError
    define_new_exception(1,ERROR_ID, "val_def_var", ValueError, "'!1@1!' cannot define as type '!2@2!' in variable declaration");
    define_new_exception(2,ERROR_ID, "invalid_exp_val", ValueError, "'!1@1!' is invalid as a '!2@2!' expression");
    define_new_exception(3,ERROR_ID, "not_alloc_val", ValueError, "'!1@1!' can not get '!2@2!' as itself value");
    define_new_exception(4,ERROR_ID, "not_equal_vals_vars", ValueError, "count of variables is not equal with count of initialization values");
    define_new_exception(5,ERROR_ID, "not_valid_val_param", ValueError, "'!1@1!' is not a valid value as a function parameter");
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//NotExistError
    define_new_exception(1,ERROR_ID, "not_exist_func", NotExistError, "'!1@1!' with '!2@2!' parameters, not exist as a function");
    define_new_exception(2,ERROR_ID, "wrong_type_var", NotExistError, "'!1@1!' is not a valid type for variable declaration");
    define_new_exception(3,ERROR_ID, "wrong_call_const", NotExistError, "'!1@1!' package has not '!2@2!' constant variable ");
    define_new_exception(4,ERROR_ID, "not_exist_pack", NotExistError, "'!1@1!' is not exist as a package block");
    define_new_exception(5,ERROR_ID, "not_exist_var", NotExistError, "'!1@1!' is not exist as a variable");
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//RuntimeError
    define_new_exception(6,ERROR_ID, "unknown_instruction", RuntimeError, "'!1@1!' is not a valid instruction");
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
	//NotAccessError
    define_new_exception(7,ERROR_ID, "access_private_var", NotAccessError, "'!1@1!' is a private variable and not access here");
}

//**************************************************************
void define_new_exception(uint32 id,int8 type, String lbl, uint8 group, String text) {
    //id;type;group;label;text
    entry_table.exceptions_count++;
    exli tmp={id,type,group,lbl,text};
    append_exli(tmp);
}
//**************************************************************
int8 print_error(long_int line_err,String name_err,String file_err,String rep1,String rep2,String occur_func){
    uint8 group_err = RuntimeError;
    int8 type_err = ERROR_ID;
    String text_err = 0;
    long_int id_err = 0;
    //---------------------search in exceptions_list
    exli search=search_lbl_exli(name_err);
    id_err = search.id;
    text_err = search.text;
    type_err = search.type;
    group_err = search.group;

    //----------------------modes
    switch(type_err) {
        case ERROR_ID:
            type_err = errors_mode;
            break;
        case WARNING_ID:
            type_err = warnings_mode;
            break;
    }
    if (type_err == CANCEL_ID) {
        return type_err;
    }
    //printf("%s,%s,%i,\n%s\n",rep1,rep2,group_err,print_str_list(exceptions_group,14));
    //----------------------replace items
    text_err = str_replace(text_err, "!1@1!", rep1, -1);
    text_err = str_replace(text_err, "!2@2!", rep2, -1);
    //-----------------------print exception
    String exception_msg = malloc(7+str_length(file_err)+(2*sizeof(long_int))+str_length(text_err)+str_length(exceptions_group[group_err])+1);
    if(line_err==0)
        sprintf(exception_msg,"%s : %s (%s::Errno%li)", exceptions_type[-type_err], text_err, exceptions_group[group_err], id_err);
    else
        sprintf(exception_msg,"%s[%s:%li] %s (%s::Errno%li)", exceptions_type[-type_err], file_err, line_err, text_err, exceptions_group[group_err], id_err);
    if (is_programmer_debug >= 1) {
        //y, mo, d := time.Now().Date()
       // h, mi, s := time.Now().Clock()
        //time_now := fmt.Sprintf("%v:%v:%v - %v.%v.%v", h, mi, s, y, mo, d)
       // exception_msg += new_line_char + fmt.Sprintf("Occurs in:[%s] [%s]", occur_func, time_now)
        printf("%s {occur_func:%s}\n",exception_msg,occur_func);
    }else{
        printf("%s\n",exception_msg);
    }

    if (is_set_logfile[0] != 0) {
        //append_to_file(project_root+separator+is_set_logfile+".log", exception_msg+new_line_char+"---------------------------"+new_line_char)
        //msg("&YYYYY", project_root+separator+is_set_logfile+".log")
    }
    //-----------------------handle fatal
    if (type_err == FATAL_ID) {
        exit(1);
    }
    return type_err;
}


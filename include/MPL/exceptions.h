#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

void init_exceptions_list_data();
int8 exception_handler(String lbl_err, const char func_occur[], String rep1, String rep2);
exli search_lbl_exli(String lbl);
exli get_exli(long_int i);
String create_exli_struct(exli s, String rep1, String rep2);
void define_new_exception(uint32 id, int8 type, String lbl, uint8 group, String text);
int8 print_error(long_int line_err, String name_err, String file_err, String rep1, String rep2, String occur_func);
Boolean set_to_logfile(String exception_msg);
Boolean exception_user_handler(int8 err_type, String err_name, String err_des, String err_func);
#endif /*__EXCEPTIONS_H*/
#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

void init_exceptions_list_data();
int8 exception_handler(String lbl_err,String func_occur,String rep1,String rep2);
Boolean set_exception_function(long_int excep_id, String rep1, String rep2);
exli search_lbl_exli(String lbl);
exli get_exli(long_int i);
void define_new_exception(uint32 id,int8 type, String lbl, uint8 group, String text);
int8 print_error(long_int line_err,String name_err,String file_err,String rep1,String rep2,String occur_func);



#endif /*__EXCEPTIONS_H*/
#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

void init_exceptions_list_data();
void define_new_exception(uint32 id,int8 type, String lbl, uint8 group, String text);
int8 print_error(long_int line_err,String name_err,String file_err,String rep1,String rep2,String occur_func);



#endif /*__EXCEPTIONS_H*/
//
// Created by amindelavar on 9/19/2018.
//

#ifndef MPL_DATA_DEFINED_H
#define MPL_DATA_DEFINED_H

//******************************data values
clock_t AppStartedClock;
String AppStartedTime;
uint8 is_programmer_debug;
String project_root;
String stdin_source_path;
String main_source_name;
String interpreter_level;
String interpreter_path;
uint8 MAX_INT_LEN;
uint8 MAX_FLOAT_LEN;
int8 errors_mode;
int8 warnings_mode;
Boolean using_custom_tab;
String tab_size;
uint8 tab_size_int;
uint8 max_estimate_divide_huge;
uint8 max_float_estimate_huge_X0;
uint8 max_steps_estimate_huge;
uint32 max_decimal_has_huge;
String set_logfile_path;
String new_line_char;
String os_tmp_dir;
uint32 BufferSize;
String hostname;
String os_version;
uint64 os_total_memory;
uint64 os_total_disk;
long_int max_size_id;
String logfile_path;
//******************************
String exceptions_group[14];
String exceptions_type[4];
String keywords[18];
String keywords_out[13];
String magic_macros[3];
String block_instructions[5];
uint8 single_operators[9];
String comparative_operators[6];
String alloc_operators[11];
String boolean_operators[3];
String basic_types[3];
uint8 golden_bytes[5];
uint8 splitters[10];
uint8 words_splitter[12];
String sub_types[5];
String control_chars[5];
utf8_str_list source_paths;
str_list program_argvs;
uint32 argvs_len;
//****************************import struct
typedef struct import_inst_struct {
  long_int id;
  uint8 type;
  Boolean is_active;
  str_utf8 path;
  uint8 max_bytes_per_char;
  uint8 err_code;

  uint32 line;
  uint32 source_id;

  struct import_inst_struct *next;
} imin;
//****************************source_code struct
typedef struct source_code_struct {
  uint32 line;
  String code;

  struct source_code_struct *next;
} soco;
//****************************exceptions_list struct
typedef struct exceptions_list_struct {
  uint32 id;
  int8 type;
  uint8 group;
  String lbl;
  String text;

  struct exceptions_list_struct *next;
} exli;
//****************************utf8_strings struct
typedef struct utf8_strings_struct {
  long_int id;
  str_utf8 utf8_string;
  uint8 max_bytes_per_char;

  struct utf8_strings_struct *next;
} utst;
//****************************instructions struct
typedef struct instructions_struct {
  long_int id;
  long_int func_id;
  long_int stru_id;
  uint32 order;
  String code;
  uint8 type;

  uint32 line;
  uint32 source_id;
  struct instructions_struct *next;
} instru;
//****************************block_structures struct
typedef struct block_structures_struct {
  long_int id;
  long_int func_id;
  long_int stru_id;
  uint8 type; //1:loop,2:if,3:elif,4:else,5:manage,6:func
  String lbl;
  str_list params;
  uint32 params_len;

  uint32 line;
  uint32 source_id;
  struct block_structures_struct *next;
} blst;
//****************************data_types struct
typedef struct data_types_struct {
  long_int id;
  long_int fid;
  String name;
  uint8 type; //1:main,2:struct
  str_list params;
  uint32 params_len;

  struct data_types_struct *next;
} datas;
//****************************def_var struct
typedef struct def_var_struct {
  String main_type;
  uint8 sub_type;
  String name_var;
  String index_var;
  String value_var;
  long_int fid;
  long_int sid;

} def_var_s;

//**********************vals_array_struct
typedef struct vals_array_struct {
  long_int data_id;
  uint8 sub_type;
  String value;
  String index;
  struct vals_array_struct *next;
} vaar;
typedef struct vals_array_entries {
  vaar *start;
  vaar *end;
  uint32 count;
} vaar_en;
//****************************instructions_order struct
typedef struct instructions_order_struct {
  long_int fid;
  long_int sid;
  uint32 order;

  struct instructions_order_struct *next;
} inor;
//****************************stru_to_in struct
typedef struct stru_to_in_struct {
  long_int id;
  uint32 stru_pars;
  Boolean is_active;
  Boolean is_inline;
} stoi;
//****************************built_in_funcs struct
typedef struct built_in_funcs_struct {
  uint32 id;
  uint8 type;
  String func_name;
  String params;
  uint8 params_len;
  String returns;
  uint8 returns_len;

  struct built_in_funcs_struct *next;
} bifs;
//****************************magic_macros struct
typedef struct magic_macros_struct {
  uint32 id;
  uint8 type;
  uint8 sub_type;
  String key;
  String value;
  struct magic_macros_struct *next;
} mama;
//****************************struct_descriptor struct
typedef struct struct_descriptor_struct {
  long_int id;
  String type;
  vaar_en st;
  struct struct_descriptor_struct *next;
} stde;
//****************************functions_stack struct
typedef struct functions_stack_struct {
  long_int fid;
  long_int fin;
  long_int sid;
  long_int order;
  long_int parent_fin;

  struct functions_stack_struct *next;
} fust;
//****************************structures_stack struct
typedef struct structures_stack_struct {
  uint8 type;
  long_int fid;
  long_int fin;
  long_int sid;
  long_int parent_sid;
  long_int order;
  String extra;

  struct structures_stack_struct *next;
} stst;
//****************************virtual memory
//runtime
typedef struct var_memory_struct {
  long_int id;
  long_int pointer_id;
  long_int stru_index;
  long_int func_index; //0:just global vars
  long_int type_var;
  String name;
  String extra;
  struct var_memory_struct *next;
} Mvar;
typedef struct pointer_memory_struct {
  long_int id;
  String data;
  uint8
      type_data; //'0':null , 'i':int , 'f':float , 'h':huge , 's':string , 'u':utf8 , 'b':boolean , 'p':Pointer To Mpoint , 'v':Pointer To Mvar , 'l': struct list
  struct pointer_memory_struct *next;
} Mpoint;



//****************************call_func struct
/*typedef struct call_func_struct {
    String func_name;
    String func_index;
    String func_rets;
    str_list params;
    uint32 params_len;
    Boolean is_core_func;
} call_func_s;
//****************************vars_table struct
typedef struct vars_table_table {
    long_int id;
    String name;
    uint8 var_type;
    String pack_name;
    Boolean is_static;
    Boolean is_const;
    Boolean is_private;
    long_int sid;
    long_int fid;
    long_int pid;

    struct vars_table_table *next;
} vata;
//****************************core_lib struct
typedef struct core_lib_struct {
    uint32 id;
    uint8 pack_id;
    String func_name;
    String params;
    String returns;
    uint8 params_len;
    uint8 returns_len;
    Boolean is_static;
    Boolean is_implement;

    struct core_lib_struct *next;
} coli;*/
//****************************entry_table struct
struct entry_table_struct {
  imin *import_start;
  imin *import_end;
  long_int import_id;
  uint32 import_inst_count;

  soco *soco_main_start;
  soco *soco_main_end;
  str_utf8 cur_source_path;
  String cur_ascii_source_path;
  long_int soco_main_count;

  soco *soco_tokens_start;
  soco *soco_tokens_end;
  long_int soco_tokens_count;

  exli *exli_start;
  exli *exli_end;
  long_int exceptions_count;

  utst *utst_start;
  utst *utst_end;
  long_int utf8_strings_id;

  instru *instru_start;
  instru *instru_end;
  long_int inst_id;

  blst *blst_stru_start;
  blst *blst_stru_end;
  long_int stru_id;

  blst *blst_func_start;
  blst *blst_func_end;
  long_int func_id;
  long_int func_index;

  uint32 source_counter;

  datas *datas_start;
  datas *datas_end;
  long_int datas_id;

  inor *inor_start;
  inor *inor_end;
  long_int inor_count;

  bifs *bifs_start;
  bifs *bifs_end;
  uint32 bifs_len;

  mama *mama_start;
  mama *mama_end;
  uint32 mama_id;

  stde *stde_start;
  stde *stde_end;
  long_int stde_id;

  fust *fust_start;
  fust *fust_end;
  uint32 fust_len;

  stst *stst_start;
  stst *stst_end;
  uint32 stst_len;

  Mvar *var_memory_start;
  Mvar *var_memory_end;
  Mpoint *pointer_memory_start;
  Mpoint *pointer_memory_end;
  long_int var_mem_id;
  long_int var_mem_len;
  long_int pointer_mem_id;
  long_int pointer_mem_len;

  long_int cur_fid, cur_fin, cur_sid, cur_order, parent_fin;
  uint32 condition_level, in_loop, break_count, next_count;
  String Rsrc;
  long_int return_fin;
  uint32 Rorder, Rline;
  Boolean is_stop_APP_CONTROLLER, is_next_inst_running,is_occur_error_exception;

  str_list post_short_alloc;
  uint32 post_short_alloc_len;

};
struct entry_table_struct entry_table;

//****************************functions
void init_database();

//-------------------------imin funcs
void append_imin(imin s);

imin get_imin(long_int id);

imin get_first_active_imin();

void delete_imin(long_int id, Boolean is_delete);

//-------------------------soco funcs
void append_soco(uint8 type, soco s);

void clear_soco(uint8 type);

soco get_soco(uint8 type, uint32 ind);

//-------------------------utst funcs
void append_utst(utst s);

utst get_utst(long_int id);

//-------------------------blst funcs
void append_blst(blst s);

blst search_lbl_func(String lbl, str_list params, uint32 par_len);

//-------------------------datas funcs
void append_datas(datas s);

datas get_datas(long_int id);

datas search_datas(String name, long_int fid, Boolean is_all);

//-------------------------instru funcs
void append_instru(instru s);

instru get_instru_by_id(long_int id);

//-------------------------inor funcs
void append_inor(inor s);

uint32 get_order(long_int fid, long_int sid);

void set_order(long_int fid, long_int sid, uint32 order);

//-------------------------stoi funcs
void empty_stoi(stoi s[], uint32 size);

//-------------------------bifs funcs
void append_bifs(bifs s);

void add_to_bifs(long_int id, uint8 type, String func_name, String params, String returns);

//-------------------------mama funcs
void append_mama(mama s);

void add_to_mama(uint8 type, uint8 sub_type, String key, String value);

mama get_mama(uint8 type, String key);

//-------------------------vaar funcs
void append_vaar(vaar s, vaar_en *s1);

void print_vaar(vaar_en s);

//-------------------------stde funcs
void append_stde(stde s);

stde get_stde(long_int id);

//-------------------------fust funcs
void append_fust(fust s);

fust get_last_fust();

void delete_last_fust();
//-------------------------stst funcs
void append_stst(stst s);
stst get_last_stst();
void delete_last_stst();

#endif //MPL_DATA_DEFINED_H

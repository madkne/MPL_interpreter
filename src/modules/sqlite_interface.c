//
// Created by amindelavar on 1/19/2019.
//

#include <MPL/system.h>

//***************************************
mofu *sqlite_start = 0;
mofu *sqlite_end = 0;
sqlite3 *sqlite_db_instance[MAX_SQLITE_OPEN_DB] = {0};
String columns_name = 0;
str_list rows_entries = 0;
uint32 rows_count = 0;
//***************************************
int32 run_sqlite_mod_funcs(String func_name,
                           str_list params,
                           str_list pars_type,
                           uint32 params_len,
                           str_list *rets_mod) {
  //-----------------------init vars
  uint32 rets_mod_len = 0;
  uint32 func_id = 0;
  str_list argvs = 0;
  long_int var0_ind = 0;
  //-----------------------search for sqlite3 function
//  mofu *tmp1 = sqlite_start;
//  if (tmp1 == 0) printf("--NULL fs functions--\n");
//  else
//    for (;;) {
//      if (tmp1->id != 0)
//        printf("Sqlite_func[%i]:%s %s\n", tmp1->id, tmp1->func_name, tmp1->params);
//      tmp1 = tmp1->next;
//      if (tmp1 == 0) break;
//    }
  mofu *st = sqlite_start;
  for (;;) {
    //if func_name is true
    if (str_equal(st->func_name, func_name)) {
      Boolean is_exist = check_built_in_module_function(st->params, pars_type, params_len);
      if (is_exist) {
        func_id = st->id;
        break;
      }
    }
    st = st->next;
    if (st == 0) break;
  }
  //-----------------------check and convert
  if (func_id == 0)return 0;
  //convert vars to values
  convert_built_in_module_vars_to_values(pars_type, params, params_len, &argvs, &var0_ind);
//  printf("SSSS:%i,%s,%s\n", func_id, st->func_name, st->params);
  //-----------------------call function
  if (func_id == 1/*version*/) {
    str_list_append(&(*rets_mod), convert_to_string(SQLITE_VERSION_INTERFACE), rets_mod_len++);
  } else if (func_id == 2/*LibVersion*/) {
    String ret = call_module_type1("sqlite3_libversion", SQLITE_MODULE_ID);
//    printf("libversion:%s\n", ret);
    str_list_append(&(*rets_mod), convert_to_string(ret), rets_mod_len++);
  } else if (func_id == 3 /*open*/) {
    String path = convert_mplpath_to_abspath(argvs[0]);
    uint32 ret = __sqlite_open_database(path);
    str_list_append(&(*rets_mod), str_from_long_int(ret), rets_mod_len++);
  } else if (func_id == 4 /*close*/) {
    Boolean ret = __sqlite_close_database((uint32) str_to_long_int(argvs[0]));
    str_list_append(&(*rets_mod), str_from_bool(ret), rets_mod_len++);
  } else if (func_id == 5 /*exec*/) {
    String ret = __sqlite_exec_database((uint32) str_to_long_int(argvs[0]), replace_control_chars(argvs[1]));
    str_list_append(&(*rets_mod), ret, rets_mod_len++);
  }
  //-----------------------return
  return rets_mod_len;
}
//***************************************
void init_sqlite_module_funcs() {
  append_to_mofu(0, SQLITE_MODULE_ID, 0, 0, 0);
  append_to_mofu(1, SQLITE_MODULE_ID, "version", 0, "str");
  append_to_mofu(2, SQLITE_MODULE_ID, "LibVersion", 0, "str");
  append_to_mofu(3, SQLITE_MODULE_ID, "open", "str", "num");
  append_to_mofu(4, SQLITE_MODULE_ID, "close", "num", "bool");
  append_to_mofu(5, SQLITE_MODULE_ID, "exec", "num|str", "str;?,?");
  append_to_mofu(6, SQLITE_MODULE_ID, "LastExeption", "num", "exception");
}
//***************************************
sqlite3 *__sqlite_return_instance(uint32 index) {
  //=>if index not valid, then raise an error
  if (index < 1 || index >= MAX_SQLITE_OPEN_DB || sqlite_db_instance[index] == 0) {
    exception_user_handler(ERROR_ID,
                           "sqlite_invalid", "sqlite database pointer not valid", "sqlite");
    return 0;
  }
  //=>return sqlite3 * from sqlite_db_instance at index
  return sqlite_db_instance[index];
}
//***************************************
uint32 __sqlite_open_database(String path) {
  //init vars
  int32 state = 0;
  sqlite3 *db;
  //------------------------------------
  //=>call sqlite3_open function to create a db instance
  #if WINDOWS_PLATFORM == true
  OPENSQLITE Proc = (OPENSQLITE) GetProcAddress(mpl_modules_instance[SQLITE_MODULE_ID], "sqlite3_open");
  if (NULL != Proc) state = (int) Proc(path, &db);
  #endif
  //=>if exist any error in sqlite open db, then raise an error
//  printf("sqlopen:%s=>%i,%i,%s\n", path, state, db, errmsg);
  if (state != SQLITE_OK) {
    exception_user_handler(ERROR_ID,
                           "open_failed", __sqlite_return_errmsg(db), "sqlite.open");
    return 0;
  }
  //=>set db in sqlite_db_instance array and return its index
  for (uint32 i = 1; i < MAX_SQLITE_OPEN_DB; i++) {
    if (sqlite_db_instance[i] == 0) {
      sqlite_db_instance[i] = db;
      return i;
    }
  }
  return 0;
}
//***************************************
Boolean __sqlite_close_database(uint32 index) {
  //init vars
  sqlite3 *db;
  int32 state = 0;
  //----------------------------------
  //=>set null in sqlite_db_instance array at index
  db = __sqlite_return_instance(index);
  if (db == 0)return false;
  sqlite_db_instance[index] = 0;
  //=>call sqlite3_close to close db
  #if WINDOWS_PLATFORM == true
  CLOSESQLITE Proc = (CLOSESQLITE) GetProcAddress(mpl_modules_instance[SQLITE_MODULE_ID], "sqlite3_close");
  if (NULL != Proc) state = (int) Proc(db);
  #endif
  //=>if exist any error in sqlite close db, then raise an error
  if (state != SQLITE_OK) {
    exception_user_handler(ERROR_ID,
                           "close_failed", __sqlite_return_errmsg(db), "sqlite.close");
    return false;
  }
  return true;
}
//***************************************
String __sqlite_return_errmsg(sqlite3 *db) {
  #if WINDOWS_PLATFORM == true
  SQLITEERRMSG Proc1 = (SQLITEERRMSG) GetProcAddress(mpl_modules_instance[SQLITE_MODULE_ID], "sqlite3_errmsg");
  if (NULL != Proc1) return (String) Proc1(db);
  #endif
  return 0;
}
//***************************************
String __sqlite_exec_database(uint32 index, String query) {
//init vars
  sqlite3 *db;
  int32 state = 0;
  String errmsg = 0;
  columns_name = 0;
  rows_entries = 0;
  rows_count = 0;
  String final = 0;
//  printf("EXEC(%i)=>%s\n",index,query);
  //----------------------------------
  //=>get sqlite_db_instance array at index
  db = __sqlite_return_instance(index);
  if (db == 0)return 0;
  //=>call sqlite3_prepare_v2 function to execute query string
  #if WINDOWS_PLATFORM == true
  SQLITEEXEC Proc = (SQLITEEXEC) GetProcAddress(mpl_modules_instance[SQLITE_MODULE_ID], "sqlite3_exec");
  if (NULL != Proc) state = (int32) Proc(db, query, __sqlite_exec_callback, 0, &errmsg);
  #endif
  //=>if exist any error in sqlite query execute, then raise an error
  if (errmsg != 0 || state != SQLITE_OK) {
    if (errmsg == 0)errmsg = __sqlite_return_errmsg(db);
    exception_user_handler(ERROR_ID, "exec_failed", errmsg, "sqlite.exec");
    return 0;
  }
  //=> if not exist any results, then return 'null'
  if (rows_count == 0) return "null";
  //=> format results in a str[?,?]
  final = char_append(final, '{');
  final = str_multi_append(final, "{", columns_name, "},", 0, 0);
  for (uint32 i = 0; i < rows_count; i++) {
    final = str_multi_append(final, "{", rows_entries[i], "}", 0, 0);
    if (i + 1 < rows_count)final = char_append(final, ',');
  }
  final = char_append(final, '}');
//  printf("Result(%i):%s\n%s\n>>>%s\n", rows_count, columns_name,
//         print_str_list(rows_entries, rows_count), final);
  return final;
}
//***************************************
static int __sqlite_exec_callback(void *data, int argc, char **argv, char **azColName) {
  //=>get column names if not exist
  if (columns_name == 0)
    for (uint32 i = 0; i < argc; i++) {
      columns_name = str_multi_append(columns_name, "\"", azColName[i], "\"", 0, 0);
      if (i + 1 < argc) columns_name = char_append(columns_name, ',');
    }
  //=>get row entries and then append in rows_entries
  String row = 0;
  for (uint32 i = 0; i < argc; i++) {
    row = str_multi_append(row, "\"", argv[i], "\"", 0, 0);
    if (i + 1 < argc) row = char_append(row, ',');
  }
  str_list_append(&rows_entries, row, rows_count++);
  return 0;
}
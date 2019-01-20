//
// Created by amindelavar on 1/19/2019.
//

#ifndef MPL_SQLITE_INTERFACE_H
#define MPL_SQLITE_INTERFACE_H

#include "sqlite3.h"
//--------------------------------constants
#define SQLITE_VERSION_INTERFACE            "1.4"
#define MAX_SQLITE_OPEN_DB                  21
//--------------------------------typedef
#if WINDOWS_PLATFORM == true
typedef int (WINAPI *OPENSQLITE)(String, sqlite3 **);
typedef int (WINAPI *CLOSESQLITE)(sqlite3 *);
typedef String (WINAPI *SQLITEERRMSG)(sqlite3 *);
typedef int (WINAPI *SQLITEEXEC)(sqlite3 *, String, int (*callback)(void *, int, char **, char **), void *, String *);
#endif
//--------------------------------variables
mofu *sqlite_start;
mofu *sqlite_end;
sqlite3 *sqlite_db_instance[MAX_SQLITE_OPEN_DB];
String columns_name;
str_list rows_entries;
uint32 rows_count;
//--------------------------------functions
int32 run_sqlite_mod_funcs(String func_name,
                           str_list params,
                           str_list pars_type,
                           uint32 params_len,
                           str_list *rets_mod);
void init_sqlite_module_funcs();

sqlite3 *__sqlite_return_instance(uint32 index);
uint32 __sqlite_open_database(String path);
Boolean __sqlite_close_database(uint32 index);
String __sqlite_return_errmsg(sqlite3 *db);
String __sqlite_exec_database(uint32 index, String query);

//static int32 __sqlite_exec_callback(void *data, int32 argc, str_list argv, str_list azColName);
static int __sqlite_exec_callback(void *data, int argc, char** argv, char** azColName);
#endif //MPL_SQLITE_INTERFACE_H

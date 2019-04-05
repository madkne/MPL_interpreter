//
// Created by amindelavar on 9/19/2018.
//

#ifndef MPL_SYSTEM_H
#define MPL_SYSTEM_H
//---------------------------------settings information
#include "settings.h"
//---------------------------------standard C functions
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <sys/stat.h>
//---------------------------------platform based C functions
#if LINUX_PLATFORM == 1
#include <linux/limits.h>
#include <stdint.h>
#elif WINDOWS_PLATFORM == 1
#include <ctype.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <windows.h>

#endif
//---------------------------------basics informations
#include "constants.h"
#include "data_defined.h"
//---------------------------------normal functions
#include "exceptions.h"
#include "built_in.h"
#include "mpl_help.h"
#include "debugger.h"
#include "builder.h"
//---------------------------------core functions
#include "core/importer.h"
#include "core/parser.h"
#include "core/starter.h"

#include "core/vars_mgr.h"
#include "core/run_mgr.h"
#include "core/virtual_memory.h"
#include "core/magic_macro.h"
//---------------------------------tools functions
#include "tools/common_funcs.h"
#include "tools/strings.h"
#include "tools/utf8.h"
#include "tools/syscalls.h"
#include "tools/encoder.h"
#include "tools/console.h"
//---------------------------------built-in functions
#include "built_in/mpl_builtin.h"
#include "built_in/os_builtin.h"
#include "built_in/data_builtin.h"
#include "built_in/fs_builtin.h"
//---------------------------------modules
#include "module.h"
#include "modules/sqlite_interface.h"
#endif //MPL_SYSTEM_H

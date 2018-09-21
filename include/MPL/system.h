//
// Created by amindelavar on 9/19/2018.
//

#ifndef MPL_SYSTEM_H
#define MPL_SYSTEM_H

#include "settings.h"
//---------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
//---------------------------------
#if LINUX_PLATFORM == 1
#include <linux/limits.h>
#include <stdint.h>
#elif WINDOWS_PLATFORM == 1

#include <windows.h>

#endif
//---------------------------------basics informations
#include "constants.h"
#include "data_defined.h"
//---------------------------------normal functions
#include "exceptions.h"
//---------------------------------core functions
#include "core/importer.h"
#include "core/parser.h"
#include "core/starter.h"
#include "core/vars_mgr.h"
//---------------------------------tools functions
#include "tools/common_funcs.h"
#include "tools/strings.h"
#include "tools/utf8.h"
//---------------------------------
#endif //MPL_SYSTEM_H
